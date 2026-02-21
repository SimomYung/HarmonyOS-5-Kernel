/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of fs server
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 15 09:32:08 2019
 */

#include <fs_server.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhwsecurec/securec.h>
#include <utils/fs_malloc.h>
#include <fs_mutex.h>
#include <vfs.h>
#include <hongmeng/hongmeng.h>

#include "internal.h"

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(VFSCORE);

#ifdef CONFIG_FS_LLT
rref_t g_fsmgr_rref = FS_INVALID_CNODE_RREF;
#else
static rref_t g_fsmgr_rref = FS_INVALID_CNODE_RREF;
#endif
static struct server_info_list g_sil = {
	.lock = FS_MUTEX_INITIALIZER_RECURSIVE,
	.cnt = 0,
	.waterline = SIL_WATERLINE,
	.head = DLIST_HEAD_INIT(g_sil.head),
};

unsigned int cnt_of_g_sil(void)
{
	return g_sil.cnt;
}

struct dlist_node *head_of_g_sil(void)
{
	return &g_sil.head;
}

void lock_g_sil(void)
{
	fs_mutex_lock(&g_sil.lock);
}

void unlock_g_sil(void)
{
	fs_mutex_unlock(&g_sil.lock);
}

void init_g_sil_lock(void)
{
	fs_mutex_init_recursive(&g_sil.lock);
}

static int vfs_client_init(rref_t rref, uref_t uref)
{
	int err = actvcapcall_fscall_vfs_client_init(rref, uref, NULL, NULL, 0);
	if (err < 0) {
		hm_error("client_init failed, err=%s\n", hmstrerror(err));
	}
	return err;
}

static int si_statrref(struct server_info *si, uref_t *uref_ret)
{
	struct sysarg_cnode_statrref statr = {0};
	int err;

	err = sysfast_cnode_stat_rref(&statr, si->actv_rref,
				      CNODE_STATRREF_SRC_CNODE_IDX |
				      CNODE_STATRREF_USLOT_IDX);
	if (err < 0) {
		hm_debug("statrref failed, err=%s\n", hmstrerror(err));
		return err;
	}
	si->cnode_idx = statr.src_cnode_idx;
	*uref_ret = ((uint64_t)statr.src_cnode_idx << CNODE_IDX_SHIFT) |
		    statr.uslot_idx;
	return E_HM_OK;
}

#define BOUND_NOTDO	0
#define BOUND_READY	1
#define BOUND_FAILED	2
static __thread int bound = BOUND_NOTDO;

static void do_bind_fsmgr(rref_t fsmgr_rref)
{
	struct __actvret_hmcall_thread_bind_info ret = {0};
	struct path_name name = {"fsmgr-bind-actv"};
	int err;

	err = actvcall_hmcall_thread_bind_info(name, &ret);
	if (err != E_HM_OK || ret.nobuf_uref == 0) {
		bound = BOUND_FAILED;
		return;
	}
	err = actvcapcall_fscall_vfs_bind_update(fsmgr_rref,
						 ret.nobuf_uref,
						 ret.buf_uref);
	if (err < 0) {
		bound = BOUND_FAILED;
		return;
	}
	bound = BOUND_READY;
}

bool is_fsmgr_bound(void)
{
	if (bound == BOUND_READY || bound == BOUND_FAILED) {
		return (bound == BOUND_READY);
	}

	rref_t fsmgr_rref = acquire_cache_fsmgr_rref();
	if (fsmgr_rref == FS_INVALID_CNODE_RREF) {
		hm_debug("cannot get global fsmgr_rref\n");
		return false;
	}
	do_bind_fsmgr(fsmgr_rref);

	return (bound == BOUND_READY);
}

static void bind_fsmgr(struct server_info *si)
{
	if (strcmp(si->pathname, FSMGR_SVRNAME) != 0) {
		return;
	}
	if (bound == BOUND_READY || bound == BOUND_FAILED) {
		return;
	}

	do_bind_fsmgr(si->actv_rref);
	if (bound == BOUND_READY) {
		si->flags |= SERVER_IS_BOUND;
	}
}

bool is_bindsrv(struct server_info *si)
{
	BUG_ON(si == NULL);
	if ((bound == BOUND_NOTDO) && ((si->flags & SERVER_IS_BOUND) != 0)) {
		bind_fsmgr(si);
	}
	return (bound == BOUND_READY) && ((si->flags & SERVER_IS_BOUND) != 0);
}

static int si_register(struct server_info *si)
{
	int err;
	uref_t uref;
	rref_t rref = 0;
	struct pathmgr_name actv_vfs_path = {0};

	err = snprintf_s(actv_vfs_path.path, PATHMGR_PATH_NAME_MAX,
			 PATHMGR_PATH_NAME_MAX - 1, "%s-actv", si->pathname);
	if (err < 0) {
		return E_HM_POSIX_FAULT;
	}

	err = hm_path_acquire(actv_vfs_path.path, &rref);
	if (err != E_HM_OK) {
		hm_debug("acquire path failed %s\n", hmstrerror(err));
		return err;
	}
	si->actv_rref = rref;

	err = si_statrref(si, &uref);
	if (err != E_HM_OK) {
		goto release;
	}
	err = vfs_client_init(rref, uref);
	if (err < 0) {
		hm_error("client_init failed:%s\n", hmstrerror(err));
		goto release;
	}
	bind_fsmgr(si);
	return E_HM_OK;

release:
	if (hm_path_release(actv_vfs_path.path, rref) != E_HM_OK) {
		hm_warn("release unused rref failed\n");
	}

	return err;
}

static void si_deregister(const struct server_info *si)
{
	struct pathmgr_name actv_vfs_path = {0};
	int err;

	err = snprintf_s(actv_vfs_path.path, PATHMGR_PATH_NAME_MAX,
			 PATHMGR_PATH_NAME_MAX - 1, "%s-actv", si->pathname);
	if (err < 0) {
		hm_error("snprintf_s failed, err=%d\n", err);
		return;
	}
	err = hm_path_release(actv_vfs_path.path, si->actv_rref);
	if (err != E_HM_OK) {
		hm_debug("release rref failed, err=%s\n", hmstrerror(err));
	}
}

static int si_init(struct server_info *si, const char *si_name)
{
	raw_refcnt_nr_init(&si->refcnt, 1);
	dlist_init(&si->link);
	NOFAIL(memcpy_s(si->pathname, sizeof(si->pathname),
			si_name, strlen(si_name) + 1));
	si->flags = 0;
	return si_register(si);
}

static int si_create(struct server_info **si_ret, const char *si_name)
{
	struct server_info *si = NULL;
	int err;

	if (si_ret == NULL || si_name == NULL) {
		return E_HM_INVAL;
	}
	si = fs_zmalloc(sizeof(struct server_info));
	if (si == NULL) {
		return E_HM_NOMEM;
	}
	err = si_init(si, si_name);
	if (err == E_HM_OK) {
		*si_ret = si;
	} else {
		free(si);
	}
	return err;
}

static inline int si_put(struct server_info *si)
{
	if (si == NULL) {
		return E_HM_INVAL;
	}
	return raw_refcnt_nr_put(&si->refcnt);
}

static int si_alive(struct server_info *si)
{
	struct pathmgr_name actv_vfs_path = {0};
	rref_t rref = 0;
	int err;

	err = snprintf_s(actv_vfs_path.path, PATHMGR_PATH_NAME_MAX,
			 PATHMGR_PATH_NAME_MAX - 1, "%s-actv", si->pathname);
	if (err < 0) {
		return E_HM_POSIX_FAULT;
	}

	err = hm_path_acquire(actv_vfs_path.path, &rref);
	if (err == E_HM_OK) {
		if (hm_path_release(actv_vfs_path.path, rref) != E_HM_OK) {
			hm_warn("release unused rref failed\n");
		}
		return E_HM_OK;
	} else if (err == E_HM_POSIX_NOENT) {
		put_server_info(si);
	}
	return err;
}

static inline void si_build(const struct meta_server_info *src,
			    struct server_info *dst)
{
	dst->actv_rref = src->actv_rref;
	raw_refcnt_nr_init(&dst->refcnt, 1);
	dst->flags = src->flags;
	mem_assign_a(dst->pathname, src->pathname);
	dlist_init(&dst->link);
}

static struct server_info *
sil_acquire(struct server_info_list *sil, const char *si_name)
{
	struct server_info *si = NULL;

	if (sil == NULL || si_name == NULL) {
		return NULL;
	}
	FS_MUTEX_GUARD(_, &sil->lock);
	dlist_for_each_entry(si, &sil->head, struct server_info, link) {
		if (strcmp(si->pathname, si_name) == 0) {
			return si;
		}
	}
	return NULL;
}

static void sil_reclaim(struct server_info_list *sil)
{
	struct server_info *si = NULL;
	struct server_info *tmp = NULL;
	int err;

	if (sil == NULL) {
		return;
	}

	FS_MUTEX_GUARD(_, &sil->lock);
	dlist_for_each_entry_safe(si, tmp, &sil->head,
				  struct server_info, link) {
		err = si_alive(si);
		if (err != E_HM_OK) {
			hm_debug("si had dead, err=%s\n", hmstrerror(err));
		}
	}
	sil->waterline = sil->cnt + SIL_BUFFER;
}

static void sil_insert(struct server_info_list *sil, struct server_info *si)
{
	if (sil == NULL || si == NULL) {
		return;
	}
	FS_MUTEX_GUARD(_, &sil->lock);
	dlist_insert_head(&sil->head, &si->link);
	sil->cnt++;
}

static void sil_delete(struct server_info_list *sil, struct server_info *si)
{
	FS_MUTEX_GUARD(_, &sil->lock);
	dlist_delete(&si->link);
	sil->cnt--;
}

int get_server_info(const char *si_name, struct server_info **si_ret)
{
	struct server_info *si = NULL;
	int err = E_HM_OK;

	if (si_name == NULL || si_ret == NULL) {
		return E_HM_INVAL;
	}
	FS_MUTEX_GUARD(_, &g_sil.lock);

	si = sil_acquire(&g_sil, si_name);
	if (si == NULL) {
		err = si_create(&si, si_name);
		if (si != NULL) {
			if (g_sil.cnt >= g_sil.waterline - 1) {
				sil_reclaim(&g_sil);
			}
			sil_insert(&g_sil, si);
		}
	}
	if (si != NULL) {
		int cnt = si_get(si);
		if (cnt < 0) {
			hm_error("cnt=%d, %s\n", cnt, hmstrerror(cnt));
			hm_error("srvname=%s\n", si->pathname);
			return cnt;
		}
		if ((bound == BOUND_NOTDO) &&
		    ((si->flags & SERVER_IS_BOUND) != 0)) {
			bind_fsmgr(si);
		}
	}
	*si_ret = si;
	return err;
}

void put_server_info(struct server_info *si)
{
	if (si == NULL) {
		return;
	}
	FS_MUTEX_GUARD(_, &g_sil.lock);

	int cnt = si_put(si);
	BUG_ON(cnt < 0, "cnt=%d, srvname=%s\n", cnt, si->pathname);
	if (cnt == 0) {
		hm_debug("delete si, srvname=%s\n", si->pathname);
		sil_delete(&g_sil, si);
		si_deregister(si);
		free(si);
	}
}

void g_sil_update_rref(void)
{
	struct server_info *si = NULL;
	struct server_info *tmp = NULL;
	int err;

	/*
	 * Note:
	 *
	 * bound is a static __thread defined above bind_fsmgr.
	 *
	 * Its default value is BOUND_NOTDO, and is set BOUND_READY
	 * after we update cred by new uref.
	 *
	 * However, when we fork, bound value (usually BOUND_READY)
	 * will be copied to the new process, and no update will be
	 * done in the new process, cred remains undefined.
	 *
	 * So we need to initialize bound after fork.
	 */
	bound = BOUND_NOTDO;

	FS_MUTEX_GUARD(_, &g_sil.lock);
	dlist_for_each_entry_safe(si, tmp, &g_sil.head,
				  struct server_info, link) {
		if (si_counter(si) == 1) {
			sil_delete(&g_sil, si);
			free(si);
			continue;
		}
		si->actv_rref = FS_INVALID_CNODE_RREF;
		err = si_register(si);
		if (err != E_HM_OK) {
			hm_warn("si register failed, name=%s, err=%s\n",
				si->pathname, hmstrerror(err));
		}
	}
	g_fsmgr_rref = FS_INVALID_CNODE_RREF;
	(void)acquire_cache_fsmgr_rref();
}

struct server_info *g_sil_acquire_si_by_rref(rref_t rref)
{
	struct server_info *si = NULL;
	int err;

	FS_MUTEX_GUARD(_, &g_sil.lock);
	dlist_for_each_entry(si, &g_sil.head, struct server_info, link) {
		if (si->actv_rref == rref) {
			err = si_get(si);
			BUG_ON(err <= 0);
			return si;
		}
	}
	return NULL;
}

int g_sil_rebuild(const struct meta_server_info *msi, uint32_t num)
{
	FS_MUTEX_GUARD(_, &g_sil.lock);

	struct server_info *si = NULL;
	for (uint32_t i = 0; i < num; i++, msi++) {
		si = fs_zmalloc(sizeof(struct server_info));
		if (si == NULL) {
			hm_error("malloc failed\n");
			break;
		}
		si_build(msi, si);
		sil_insert(&g_sil, si);
	}
	return E_HM_OK;
}

static inline void si_build_fsmgr(struct server_info *si)
{
	char pathname[SRVNAME_MAX_LEN] = "fsmgr";
	si->actv_rref = 0;
	raw_refcnt_nr_init(&si->refcnt, 1);
	si->flags = 0;
	mem_assign_a(si->pathname, pathname);
	dlist_init(&si->link);
}

void g_sil_rebuild_fsmgr(void)
{
	FS_MUTEX_GUARD(_, &g_sil.lock);

	struct server_info *si = fs_zmalloc(sizeof(struct server_info));
	if (si == NULL) {
		hm_error("malloc failed\n");
		return;
	}

	si_build_fsmgr(si);
	sil_insert(&g_sil, si);

	return;
}

rref_t acquire_cache_fsmgr_rref(void)
{
	if (g_fsmgr_rref != FS_INVALID_CNODE_RREF) {
		return g_fsmgr_rref;
	}

	int rc = hm_path_acquire(FSMGR_PATH_ACTV, &g_fsmgr_rref);
	if (rc != E_HM_OK) {
		hm_debug("get fsmgr server rref with return: %s\n", hmstrerror(rc));
	}

	return g_fsmgr_rref;
}

void release_cache_fsmgr_rref(void)
{
	(void)hm_path_release(FSMGR_PATH_ACTV, g_fsmgr_rref);
	g_fsmgr_rref = FS_INVALID_CNODE_RREF;
}

rref_t fsmgr_rref_of(void)
{
	int err = E_HM_OK;
	rref_t rref;

	if (g_fsmgr_rref != FS_INVALID_CNODE_RREF) {
		rref = g_fsmgr_rref;
	} else {
		err = hm_path_acquire(FSMGR_PATH_ACTV, &rref);
		if (err != E_HM_OK) {
			hm_debug("get fsmgr server rref with return: %s\n", hmstrerror(err));
			rref = FS_INVALID_CNODE_RREF;
		} else if (!hm_fetch_fork_allowd_flag()) {
			rref_t old = raw_atomic_cmpxchg64(&g_fsmgr_rref, FS_INVALID_CNODE_RREF, rref);
			if (old != FS_INVALID_CNODE_RREF) {
				(void)hm_path_release(FSMGR_PATH_ACTV, rref);
				rref = old;
			}
		} else {
			/* do nothing */
		}
	}

	return rref;
}

void release_fsmgr_rref(rref_t fsmgr_rref)
{
	if (fsmgr_rref != g_fsmgr_rref) {
		(void)hm_path_release(FSMGR_PATH_ACTV, fsmgr_rref);
	}
}
