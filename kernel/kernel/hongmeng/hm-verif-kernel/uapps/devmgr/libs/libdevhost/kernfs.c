/*
* Copyright (c) Huawei Technologies Co., Ltd. 2020-2020 All rights reserved.
* Description: Libdh kernfs helper
* Author: Huawei OS Kernel Lab
* Create: Sat Jul 18 23:51:28 2020
*/

#include "kernfs_internal.h"

#include <stdarg.h>
#include <sys/stat.h>
#include <hongmeng/errno.h>
#include <libsysif/devmgr/api.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libdevhost/kernfs.h>
#include <libdevhost/devhost.h>

static struct kernfs_entry invalid_entry = {0};

int libdh_kernfs_init(struct libdh_kernfs_context *fs_ctx)
{
	int ret = E_HM_INVAL;

	if (fs_ctx != NULL) {
		mem_zero_a(fs_ctx->base);
		idr_init(&fs_ctx->idr);
		raw_rwlock_init(&fs_ctx->mutex, NULL);

		ret = E_HM_OK;
	}
	return ret;
}

static void __kernfs_uobject_release(struct uobject *uobj)
{
	struct kernfs_entry *entry = NULL;

	BUG_ON(uobj == NULL);
	entry = container_of(uobj, struct kernfs_entry, uobj);
	if (entry->release != NULL) {
		entry->release(entry);
	}
}

int libdh_kernfs_insert(struct libdh_kernfs_context *fs_ctx,
			struct kernfs_entry *entry, int *tagid_out)
{
	int tagid = 0;
	int ret = E_HM_OK;

	if ((fs_ctx == NULL) || (entry == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		uobject_init(&entry->uobj, 1, __kernfs_uobject_release);
		tagid = idr_alloc_cyclic(&fs_ctx->idr, entry,
					 LIBDH_KERNFS_TAG_MIN,
					 LIBDH_KERNFS_TAG_MAX);
		if (tagid < 0) {
			ret = tagid;
		}
	}

	if ((ret == E_HM_OK) && (tagid_out != NULL)) {
		*tagid_out = tagid;
	}

	return ret;
}

static inline void libdh_kernfs_release_refcnt(struct kernfs_entry *entry)
{
	if (entry == &invalid_entry) {
		return;
	}

	(void)uobject_release(&entry->uobj);
}

void libdh_kernfs_release(struct libdh_kernfs_context *fs_ctx, int tagid)
{
	struct kernfs_entry *entry = NULL;

	if (fs_ctx != NULL) {
		RAW_RWLOCK_WR_GUARD(_, &fs_ctx->mutex);
		entry = (struct kernfs_entry *)idr_remove(&fs_ctx->idr,
							  (unsigned int)tagid);
		if (entry != NULL) {
			libdh_kernfs_release_refcnt(entry);
		}
	}
}

void libdh_kernfs_remove(struct libdh_kernfs_context *fs_ctx, int tagid)
{
	struct kernfs_entry *entry = NULL;

	if (fs_ctx != NULL) {
		RAW_RWLOCK_WR_GUARD(_, &fs_ctx->mutex);
		entry = (struct kernfs_entry *)idr_remove(&fs_ctx->idr,
							  (unsigned int)tagid);
	}
	if (entry != NULL) {
		libdh_kernfs_put(entry);
	}
}

int libdh_kernfs_entry_acquire(struct libdh_kernfs_context *fs_ctx,
				  unsigned int tagid, struct kernfs_entry **entry_out)
{
	struct kernfs_entry *entry = NULL;
	int ret = E_HM_INVAL;

	if (entry_out != NULL) {
		ret = E_HM_POSIX_NOENT;
		RAW_RWLOCK_RD_GUARD(_, &fs_ctx->mutex);
		entry = (struct kernfs_entry *)idr_find(
				&fs_ctx->idr, tagid);
		if (entry != NULL) {
			if ((entry->ops == NULL) || (entry == &invalid_entry)) {
				ret = E_HM_NOSYS;
			} else {
				libdh_kernfs_get(entry);
				*entry_out = entry;
				ret = E_HM_OK;
			}
		}
	}

	return ret;
}

void libdh_kernfs_tagid_invalid(struct libdh_kernfs_context *fs_ctx, int tagid)
{
	struct kernfs_entry *entry = NULL;

	RAW_RWLOCK_WR_GUARD(_, &fs_ctx->mutex);
	entry = idr_replace(&fs_ctx->idr, (void *)&invalid_entry, (unsigned int)tagid);
	if (entry != NULL) {
		libdh_kernfs_release_refcnt(entry);
	}
}

struct __kernfs_rw_args {
	struct libdh_kernfs_context *fs_ctx;
	unsigned int tagid;
	const struct libdh_kernfs_rw_args *args;
	uintptr_t ctx;
};

int libdh_fs_check(uintptr_t cnode_index)
{
	int ret = E_HM_OK;
	__u32 self_cnode_idx = hm_ucap_self_cnode_idx();
	if (is_cnode_idx_err(self_cnode_idx)) {
		ret = cnode_idx_to_err(self_cnode_idx);
	}

	if (ret == E_HM_OK) {
		if (cnode_index != (uintptr_t)self_cnode_idx) {
			ret = E_HM_INVAL;
		}
	}

	return ret;
}

static int __do_kernfs_rw(struct __kernfs_rw_args *args, bool is_read, size_t *psize)
{
	int ret = E_HM_OK;
	struct kernfs_entry *entry = NULL;
	unsigned long size = 0;
	int (*ops)(const struct kernfs_entry *entry,
		   const struct libdh_kernfs_rw_args *args,
		   unsigned long *rsize) = NULL;

	ret = libdh_fs_check(args->ctx);
	if (ret == E_HM_OK) {
		if ((args->fs_ctx == NULL) || (args->args == NULL)) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = libdh_kernfs_entry_acquire(args->fs_ctx, args->tagid, &entry);
	}

	if (ret == E_HM_OK) {
		if (is_read) {
			ops = entry->ops->read;
		} else {
			ops = entry->ops->write;
		}
		if (ops != NULL) {
			/* see libdevhost/kernfs.h libdh_kernfs_ops return hm errno */
			ret = (*ops)(entry, args->args, &size);
			if (ret >= 0 && psize) {
				*psize = (size_t) size;
			}
		} else {
			ret = E_HM_NOSYS;
		}
		libdh_kernfs_put(entry);
	}

	return ret;
}

int libdh_kernfs_read(struct libdh_kernfs_context *fs_ctx, unsigned int tagid,
		      const struct libdh_kernfs_rw_args *read_args,
		      uintptr_t ctx, size_t *rsize /* out */)
{
	struct __kernfs_rw_args args = {
		.fs_ctx = fs_ctx,
		.tagid = tagid,
		.args = read_args,
		.ctx = ctx,
	};

	return __do_kernfs_rw(&args, true, rsize);
}

int libdh_kernfs_write(struct libdh_kernfs_context *fs_ctx, unsigned int tagid,
		       const struct libdh_kernfs_rw_args *write_args,
		       uintptr_t ctx, size_t *wsize /* out */)
{
	struct __kernfs_rw_args args = {
		.fs_ctx = fs_ctx,
		.tagid = tagid,
		.args = write_args,
		.ctx = ctx,
	};

	return __do_kernfs_rw(&args, false, wsize);
}

int libdh_kernfs_poll(struct libdh_kernfs_context *fs_ctx, unsigned int tagid,
		      uintptr_t ctx, bool pollable)
{
	int ret = E_HM_OK;
	__u32 self_cnode_idx = hm_ucap_self_cnode_idx();
	struct kernfs_entry *entry = NULL;
	int (*ops)(const struct kernfs_entry *entry, bool pollable) = NULL;

	if (is_cnode_idx_err(self_cnode_idx)) {
		ret = cnode_idx_to_err(self_cnode_idx);
	}

	if (ret == E_HM_OK) {
		if ((ctx != (uintptr_t)self_cnode_idx) || (fs_ctx == NULL)) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = libdh_kernfs_entry_acquire(fs_ctx, tagid, &entry);
	}

	if (ret == E_HM_OK) {
		ops = entry->ops->poll;
		if (ops != NULL) {
			/* see libdevhost/kernfs.h libdh_kernfs_ops return hm errno */
			ret = (*ops)(entry, pollable);
		} else {
			ret = E_HM_NOSYS;
		}
		libdh_kernfs_put(entry);
	}

	return ret;
}

/*
 * The __kernfs_notify_tree stores the kernfs nodes that need to be notified
 * by the `kernfs_notify`. Currently, the __kernfs_notify_tree manages only
 * poll-related nodes. Later, inotfy-related node can also be managed here.
 */
static struct rbt_tree __kernfs_notify_tree = RBT_ROOT;
static struct raw_rwlock __kernfs_notify_lock = RAW_RDLOCK_INITIALIZER;
DEFINE_rbt_search(kernfs_notify, struct libdh_kernfs_notify_node, node, ctx);

static int __libdh_kernfs_notify_insert(struct libdh_kernfs_notify_node *pnode)
{
	int ret = E_HM_OK;
	struct rbt_node *p = __kernfs_notify_tree.root;
	struct libdh_kernfs_notify_node *cur = NULL;
	bool is_right = false;

	RAW_RWLOCK_WR_GUARD(_, &__kernfs_notify_lock);

	while (p != NULL) {
		cur = rbt_entry(p, struct libdh_kernfs_notify_node, node);
		if (pnode->ctx > cur->ctx) {
			if (p->rbt_right == NULL) {
				is_right = true;
				break;
			}
			p = p->rbt_right;
		} else if (pnode->ctx < cur->ctx) {
			if (p->rbt_left == NULL) {
				is_right = false;
				break;
			}
			p = p->rbt_left;
		} else {
			ret = E_HM_POSIX_EXIST;
			break;
		}
	}

	if (ret == E_HM_OK) {
		rbt_insert(&__kernfs_notify_tree, &pnode->node, p, is_right);
	}

	return ret;
}

int libdh_kernfs_notify_insert(uintptr_t kernfs_node_ctx)
{
	int ret = E_HM_OK;
	struct libdh_kernfs_notify_node *pnode = NULL;

	pnode = (struct libdh_kernfs_notify_node *)malloc(sizeof(*pnode));
	if (pnode == NULL) {
		return E_HM_NOMEM;
	}
	pnode->ctx = kernfs_node_ctx;

	ret = __libdh_kernfs_notify_insert(pnode);
	if (ret < 0) {
		free(pnode);
	}

	return ret;
}

void libdh_kernfs_notify_remove(uintptr_t kernfs_node_ctx)
{
	struct libdh_kernfs_notify_node *pnode = NULL;

	RAW_RWLOCK_WR_GUARD(_, &__kernfs_notify_lock);

	pnode = kernfs_notify_rbt_search(&__kernfs_notify_tree, kernfs_node_ctx, NULL);
	if (pnode != NULL) {
		rbt_remove(&__kernfs_notify_tree, &pnode->node);
		free(pnode);
	}
}

int libdh_kernfs_notify_search(uintptr_t kernfs_node_ctx)
{
	int ret = E_HM_POSIX_NOENT;
	struct libdh_kernfs_notify_node *pnode = NULL;

	RAW_RWLOCK_RD_GUARD(_, &__kernfs_notify_lock);

	pnode = kernfs_notify_rbt_search(&__kernfs_notify_tree, kernfs_node_ctx, NULL);
	if (pnode != NULL) {
		ret = E_HM_OK;
	}

	return ret;
}

static struct libdh_kernfs_ops_simple *kernfs_ops_list[(int)DEVHOST_FS_MAX] __read_mostly = {NULL};

struct libdh_kernfs_ops_simple *libdh_get_kernfs_ops(unsigned int tagid, uintptr_t ctx)
{
	unsigned int type;

	if (libdh_fs_check(ctx) < 0) {
		return NULL;
	}

	type = DEVHOST_ID2TYPE(tagid);
	if ((type >= DEVHOST_FS_MAX) || kernfs_ops_list[type] == NULL) {
		hm_error_ratelimited("kernfs[%u] ops is not support\n", type);
		return NULL;
	}

	return kernfs_ops_list[type];
}

int libdh_kernfs_register(int type, void *data)
{
	if ((data == NULL) || (type >= (int)DEVHOST_FS_MAX) || (type < 0)) {
		return E_HM_INVAL;
	}

	if (kernfs_ops_list[type] != NULL) {
		return E_HM_POSIX_EXIST;
	}

	kernfs_ops_list[type] = (struct libdh_kernfs_ops_simple *)data;
	return E_HM_OK;
}

int libdh_devhost_flush_async_kernfs(void)
{
	int ret = 0;
	unsigned int type = DEVHOST_COMMON;
	struct libdh_kernfs_ops_simple *ops = NULL;

	for (; type < DEVHOST_FS_MAX; type++) {
		ops = kernfs_ops_list[type];
		if (ops == NULL)
			continue;

		if (ops->flush != NULL) {
			ret = ops->flush();
		}

		if (ret != 0 && ret != E_HM_NOSYS) {
			hm_info("flush async sysfs (type=%d) ret=%s\n",
				 type, hmstrerror(ret));
		}
	}
	return ret;
}
