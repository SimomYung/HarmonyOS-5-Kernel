/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device ppoll support
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 21 16:03:29 2023
 */
#include "devhost_poll.h"
#include <devhost/poll.h>

#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_select_common.h>
#include <libhmsrv_sys/hm_ctrlmem_syscalls_layer.h>
#include <libsysif/devmgr/types.h>
#include <libsysif/sysmgr/api.h>
#include <libfdtable/fdtable.h>

#include "types.h"
#include <devhost/log.h>
#include <devhost/file.h>
#include <devhost/plugin.h>

struct dh_poll_fdtable_rref_tree {
	struct raw_rwlock lock;
	struct rbt_tree tree;
};
struct dh_poll_fdtable_rref_node {
	struct rbt_node node;
	unsigned int cnode_idx;
	unsigned long long fdtable_rref;
};
static struct dh_poll_fdtable_rref_tree dh_fdtbale_rref_tree = {
	.lock = RAW_RDLOCK_INITIALIZER,
	.tree = RBT_ROOT,
};

DEFINE_rbt_search(dh_fdt_rref, struct dh_poll_fdtable_rref_node, node, cnode_idx);

static struct raw_mutex g_dh_fdtbale_rref_tree_mutex = RAW_MUTEX_INITIALIZER;

static void dh_fdt_rref_rbt_insert(struct rbt_node *parent_node,
				   struct dh_poll_fdtable_rref_node *new_pnode)
{
	bool is_rchild = false;
	struct dh_poll_fdtable_rref_node *parent = NULL;

	if (parent_node != NULL) {
		parent = rbt_entry(parent_node, struct dh_poll_fdtable_rref_node, node);
		if (parent->cnode_idx < new_pnode->cnode_idx) {
			is_rchild = true;
		}
	}

	RAW_RWLOCK_WR_GUARD_START(_, &dh_fdtbale_rref_tree.lock);
	rbt_insert(&dh_fdtbale_rref_tree.tree, &new_pnode->node, parent_node, is_rchild);
	RAW_RWLOCK_WR_GUARD_END(_, &dh_fdtbale_rref_tree.lock);
}

static int devhost_acquire_fdtable_rref(unsigned int cnode_idx,
					unsigned long long *fdtable_rref)
{
	int ret;
	struct dh_poll_fdtable_rref_node *pnode = NULL;
	struct dh_poll_fdtable_rref_node *new_pnode = NULL;
	struct rbt_node *parent_node = NULL;
	rref_t fdt_rref_ret;

	RAW_RWLOCK_RD_GUARD_START(_, &dh_fdtbale_rref_tree.lock);
	pnode = dh_fdt_rref_rbt_search(&dh_fdtbale_rref_tree.tree, cnode_idx, NULL);
	if (pnode != NULL) {
		*fdtable_rref = pnode->fdtable_rref;
		return E_HM_OK;
	}
	RAW_RWLOCK_RD_GUARD_END(_, &dh_fdtbale_rref_tree.lock);

	new_pnode =  malloc(sizeof(*pnode));
	if (new_pnode == NULL) {
		return E_HM_NOMEM;
	}

	/* new acquire fdtable rref */
	ret = hm_fdtable_require_grant(cnode_idx, &fdt_rref_ret);
	if (ret != E_HM_OK) {
		free(new_pnode);
		dh_error("devhost acquire_fdtable_rref failed: %s\n", hmstrerror(ret));
		return ret;
	}
	new_pnode->cnode_idx = cnode_idx;
	new_pnode->fdtable_rref = fdt_rref_ret;
	*fdtable_rref = new_pnode->fdtable_rref;

	RAW_MUTEX_GUARD_START(_, &g_dh_fdtbale_rref_tree_mutex);
	/* recheck */
	pnode = dh_fdt_rref_rbt_search(&dh_fdtbale_rref_tree.tree, cnode_idx, &parent_node);
	if (pnode != NULL) {
		*fdtable_rref = pnode->fdtable_rref;
		goto recheck_find_out;
	}

	dh_fdt_rref_rbt_insert(parent_node, new_pnode);
	RAW_MUTEX_GUARD_END(_, &g_dh_fdtbale_rref_tree_mutex);

	return E_HM_OK;

recheck_find_out:
	(void)hm_ucap_reject(0, new_pnode->fdtable_rref);
	free(new_pnode);
	return E_HM_OK;
}

void devhost_reject_fdtable_rref(unsigned int cnode_idx)
{
	int ret;
	struct dh_poll_fdtable_rref_node *pnode = NULL;

	/* Assume most cases are misses */
	RAW_RWLOCK_RD_GUARD_START(_, &dh_fdtbale_rref_tree.lock);
	pnode = dh_fdt_rref_rbt_search(&dh_fdtbale_rref_tree.tree, cnode_idx, NULL);
	if (pnode == NULL) {
		return;
	}
	RAW_RWLOCK_RD_GUARD_END(_, &dh_fdtbale_rref_tree.lock);

	RAW_MUTEX_GUARD_START(_, &g_dh_fdtbale_rref_tree_mutex);
	pnode = dh_fdt_rref_rbt_search(&dh_fdtbale_rref_tree.tree, cnode_idx, NULL);
	if (pnode == NULL) {
		return;
	}

	RAW_RWLOCK_WR_GUARD_START(_, &dh_fdtbale_rref_tree.lock);
	rbt_remove(&dh_fdtbale_rref_tree.tree, &pnode->node);
	RAW_RWLOCK_WR_GUARD_END(_, &dh_fdtbale_rref_tree.lock);
	RAW_MUTEX_GUARD_END(_, &g_dh_fdtbale_rref_tree_mutex);

	ret = hm_ucap_reject(0, pnode->fdtable_rref);
	if (ret < E_HM_OK) {
		dh_error("hm_ucap_reject failed: %s\n", hmstrerror(ret));
	}

	free(pnode);
}

#define POLL_RECHECK   0x800
static int __devhost_recheck_revents(struct devhost_poll_target_info *t, unsigned int *revents)
{
	int ret;
	struct device_info *devinfo = NULL;
	struct filp_node *fnode = NULL;
	unsigned int __revents = 0;

	fnode = t->fnode;
	if (fnode == NULL || fnode->devinfo == NULL) {
		dh_debug("invalid fnode params\n");
		return -hmerrno2posix(E_HM_INVAL);
	}

	devinfo = fnode->devinfo;
	if (device_flag_test(devinfo->p->flags, DEVICE_SAFEPOLL)) {
		/* the driver poll function of this device must be safe in the atomic context */
		ret = devinfo->p->ex_fops->poll(fnode->filp, NULL, &__revents);
		if (ret < 0) {
			dh_debug("recheck dev %s poll failed, ret=%d\n", devinfo->devname, ret);
			return ret;
		}
		/* if the value of *revents is 0, it indicates that the recheck finds
		 * that no waiting event occurs.
		 */
		*revents = __revents & t->poll_args.events;
	} else {
		*revents = POLL_RECHECK;
	}

	return 0;
}

static int __devhost_poll_wakeup(void *target, unsigned int revents)
{
	int ret;
	unsigned int __revents = revents;
	struct devhost_poll_target_info *t = NULL;

	if (target == NULL) {
		dh_warn("invalid params\n");
		return -hmerrno2posix(E_HM_INVAL);
	}

	t = (struct devhost_poll_target_info *)target;

	/* maybe need to recheck the device events */
	if ((__revents = (__revents & t->poll_args.events)) == 0) {
		ret = __devhost_recheck_revents(t, &__revents);
		if (ret < 0 || __revents == 0) {
			return ret;
		}
	}

	ret = syscap_FDTableWakeupPoll(t->poll_args.fdtable_rref,
				       t->poll_args.tcb_key,
				       t->poll_args.fd,
				       t->poll_args.idx,
				       (short)__revents);
	if (ret == E_HM_CNODE_LOOKUP_R || ret == E_HM_NOENT) {
		/* ok, that means micro kernel is not interested to this fd any more.
		 * and this node will be removed soon in devhost_release_pwq.
		 */
		ret = E_HM_OK;
	}

	return -hmerrno2posix(ret);
}

static int __init_poll_node(struct devhost_poll_node **node,
			    struct devhost_poll_info *pinfo,
			    struct devhost_poll_target_info *tinfo)
{
	int ret;
	struct devhost_poll_node *pnode  = NULL;
	struct devhost_poll_target_info *target = NULL;
	unsigned long long fdtable_rref = 0ULL;

	 ret = devhost_acquire_fdtable_rref(tinfo->cnode_idx, &fdtable_rref);
	 if (ret != E_HM_OK) {
		return ret;
	 }

	pnode = malloc(sizeof(*pnode));
	if (pnode == NULL) {
		dh_error("malloc poll node failed\n");
		return E_HM_NOMEM;
	}

	target = &pnode->target;
	dlist_init(&pnode->node);
	devhost_init_pwq_info(&pnode->pwq_info, NULL, NULL);
	ret = memcpy_s(target, sizeof(*target), tinfo, sizeof(*tinfo));
	BUG_ON(ret != 0, "ret=%d\n", ret);
	target->poll_args.fdtable_rref = fdtable_rref;

	pinfo->pwq_info = &pnode->pwq_info;
	pinfo->target = target;
	pinfo->wakeup = __devhost_poll_wakeup;
	*node = pnode;
	return E_HM_OK;
}

/* The caller ensures that the input parameters are valid.  */
static void __free_poll_node(struct devhost_poll_node *node)
{
	devhost_release_pwq(&node->pwq_info);
	free(node);
}

/* The caller ensures that the input parameters are valid.  */
int devhost_poll_add(struct filp_node *fnode,
		     struct devhost_poll_target_info *tinfo,
		     int need_add,
		     unsigned int *revents)
{
	int ret = E_HM_OK;
	struct devhost_poll_node *pnode = NULL;
	struct device_info *devinfo = fnode->devinfo;
	struct devhost_poll_info pinfo = {0};
	struct dlist_node *head = NULL;

	*revents = 0;

	if (need_add == 0) {
		ret = devinfo->p->ex_fops->poll(fnode->filp, NULL, revents);
		*revents = *revents & tinfo->poll_args.events;
		return posix2hmerrno(-ret);
	}

	ret = __init_poll_node(&pnode, &pinfo, tinfo);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = devinfo->p->ex_fops->poll(fnode->filp, &pinfo, revents);
	if ((*revents != 0) && ((*revents = *revents & tinfo->poll_args.events) != 0)) {
		__free_poll_node(pnode);
		return E_HM_OK;
	} else if (ret < 0) {
		__free_poll_node(pnode);
		return posix2hmerrno(-ret);
	}

	head = wait_head_of_filp(fnode);
	filp_poll_list_lock(fnode);
	dlist_insert_tail(head, &pnode->node);
	filp_poll_list_unlock(fnode);

	return ret;
}

/* key: tcb_cref */
void devhost_poll_remove(struct filp_node *fnode,
			 unsigned long long tcb_key)
{
	struct devhost_poll_node *pos = NULL;
	struct devhost_poll_node *next = NULL;
	struct dlist_node *head = wait_head_of_filp(fnode);
	DLIST_HEAD(temp_list);

	filp_poll_list_lock(fnode);
	dlist_for_each_entry_safe(pos, next, head, struct devhost_poll_node, node) {
		struct devhost_poll_target_info *target = &pos->target;
		if (target->poll_args.tcb_key == tcb_key) {
			dlist_delete(&pos->node);
			dlist_insert_tail(&temp_list, &pos->node);
		}
	}
	filp_poll_list_unlock(fnode);

	dlist_for_each_entry_safe(pos, next, &temp_list,
				  struct devhost_poll_node, node) {
		dlist_delete(&pos->node);
		__free_poll_node(pos);
	}
}

/* filp to be destroyed */
PUBLIC_SYMBOL
void devhost_poll_close(struct filp_node *fnode)
{
	struct devhost_poll_node *pos = NULL;
	struct devhost_poll_node *next = NULL;
	struct dlist_node *head = wait_head_of_filp(fnode);

	filp_poll_list_lock(fnode);
	dlist_for_each_entry_safe(pos, next, head, struct devhost_poll_node, node) {
		dlist_delete(&pos->node);
		__free_poll_node(pos);
	}
	filp_poll_list_unlock(fnode);
}

PUBLIC_SYMBOL
void devhost_sync_revents_to_hmkobj_trad(struct filp_node *fnode, unsigned int revents)
{
	if (fnode != NULL && fnode->hmkobj != NULL) {
		(void)sysctrlmem_kobjctnr_sync_polllist_revents(fnode->hmkobj, revents);
	}
}

PUBLIC_SYMBOL
void devhost_sync_revents_to_hmkobj(struct hmkobj_ctx *kobj_ctx, unsigned int revents)
{
	if (kobj_ctx != NULL && kobj_ctx->kobj != NULL) {
		(void)sysctrlmem_kobjctnr_sync_polllist_revents(kobj_ctx->kobj, revents);
	}
}

PUBLIC_SYMBOL
void *devhost_sync_open(struct hmkobj_ctx *kobj_ctx)
{
	devhost_get_kobj(kobj_ctx);
	return kobj_ctx;
}

PUBLIC_SYMBOL
void devhost_sync_close(struct hmkobj_ctx *kobj_ctx)
{
	devhost_put_kobj(kobj_ctx);
}
