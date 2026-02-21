/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of fs server
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 15 09:32:08 2019
 */

#ifndef LIBVFS_FS_SERVER_H
#define LIBVFS_FS_SERVER_H

#include <libhmsync/raw_scopedptr.h>
#include <fs_mutex.h>
#include <stdint.h>
#include <lib/dlist.h>

#define DEF_MAX_MOUNT_PATH_LEN	1024
#define SRVNAME_MAX_LEN		32
/* The initial server info list waterline */
#define SIL_WATERLINE           CONFIG_FSLIB_SERVER_INFO_LIST_SIZE
#define SIL_BUFFER              4

#define SERVER_IS_ROOT		1
#define SERVER_IS_CWD		(1 << 1)
#define SERVER_IS_BOUND         (1 << 2)

#define FS_INVALID_CNODE_RREF 0xFFFFFFFFFFFFFFFF

#define FSMGR_SVRNAME "fsmgr"

struct meta_server_info {
	rref_t actv_rref;
	char pathname[SRVNAME_MAX_LEN];
	unsigned int flags;
};

struct server_info {
	struct dlist_node link;
	char pathname[SRVNAME_MAX_LEN];
	rref_t actv_rref;
	struct raw_refcnt_nr refcnt;
	unsigned int flags;
	uint32_t cnode_idx;
};

struct server_info_list {
	struct fs_mutex lock;
	unsigned int cnt;
	unsigned int waterline;
	struct dlist_node head;
};

unsigned int cnt_of_g_sil(void);
struct dlist_node *head_of_g_sil(void);
void lock_g_sil(void);
void unlock_g_sil(void);
void init_g_sil_lock(void);
bool is_fsmgr_bound(void);

typedef int (*server_info_list_foreach_func)(struct server_info *si);

int get_server_info(const char *srvname, struct server_info **si_ret);
void put_server_info(struct server_info *si);
rref_t acquire_cache_fsmgr_rref(void);
void release_cache_fsmgr_rref(void);
rref_t fsmgr_rref_of(void);
void release_fsmgr_rref(rref_t fsmgr_rref);
bool is_bindsrv(struct server_info *si);

/**
 * USAGE:
 * 1. actv_fscall(name, si)
 * 2. actv_fscall(name, si, arg0, arg1, ...)
 * actv_fscall can deal with 20 arguments at most.
 */
#define actv_fscall1(name, si) \
	(is_bindsrv(si) ? actvcall_fscall_##name() : \
	actvcapcall_fscall_##name((si)->actv_rref))

#define actv_fscall2(name, si, args...) \
	(is_bindsrv(si) ? actvcall_fscall_##name(args) : \
	actvcapcall_fscall_##name((si)->actv_rref, args))

#define __actv_fscall(func, name, si, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, type, ...) func##type
#define _actv_fscall(func, ...) \
	__actv_fscall(func, __VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, ...)
#define actv_fscall(...) _actv_fscall(actv_fscall, __VA_ARGS__)(__VA_ARGS__)

/**
 * USAGE:
 * 1. actvx_fscall(name, si, fwd, refwd)
 * 2. actvx_fscall(name, si, fwd, refwd, arg0, arg1, ...)
 * actvx_fscall can deal with 20 arguments at most.
 */
#define actvx_fscall1(name, si, fwd, refwd) \
	(is_bindsrv(si) ? actvxactcall_fscall_##name(fwd, refwd) : \
	actvxactcapcall_fscall_##name(fwd, refwd, (si)->actv_rref))

#define actvx_fscall2(name, si, fwd, refwd, args...) \
	(is_bindsrv(si) ? actvxactcall_fscall_##name(fwd, refwd, args) : \
	actvxactcapcall_fscall_##name(fwd, refwd, (si)->actv_rref, args))

#define __actvx_fscall(func, name, si, fwd, refwd, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, \
		       type, ...) \
	func##type
#define _actvx_fscall(func, ...) \
	__actvx_fscall(func, __VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, ...)
#define actvx_fscall(...) _actvx_fscall(actvx_fscall, __VA_ARGS__)(__VA_ARGS__)

static inline int get_fsmgr_si(struct server_info **si_ret)
{
	return get_server_info(FSMGR_SVRNAME, si_ret);
}

static inline int si_get(struct server_info *si)
{
	if (si == NULL) {
		return E_HM_INVAL;
	}
	return raw_refcnt_nr_get(&si->refcnt);
}

static inline int si_counter(struct server_info *si)
{
	if (si == NULL) {
		return E_HM_INVAL;
	}
	return raw_refcnt_nr_counter(&si->refcnt);
}

/**
 * USAGE:
 * 1. actvcap_fscall(name)
 * 2. actvcap_fscall(name, arg0, arg1, ...)
 * actvcap_fscall can deal with 20 arguments at most.
 */
#define actvcap_fscall1(name) ({\
	int tmp_ret = E_HM_INVAL; \
	rref_t fsmgr_rref = fsmgr_rref_of(); \
	if (fsmgr_rref != FS_INVALID_CNODE_RREF) { \
		tmp_ret = actvcapcall_fscall_##name(fsmgr_rref); \
		release_fsmgr_rref(fsmgr_rref); \
	} \
	tmp_ret; \
	})

#define actvcap_fscall2(name, args...) ({ \
	long tmp_ret = E_HM_INVAL; \
	rref_t fsmgr_rref = fsmgr_rref_of(); \
	if (fsmgr_rref != FS_INVALID_CNODE_RREF) { \
		tmp_ret = actvcapcall_fscall_##name(fsmgr_rref, args); \
		release_fsmgr_rref(fsmgr_rref); \
	} \
	tmp_ret; \
	})

#define __actvcap_fscall(func, name, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, type, ...) func##type
#define _actvcap_fscall(func, ...) \
	__actvcap_fscall(func, __VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, ...)
#define actvcap_fscall(...) _actvcap_fscall(actvcap_fscall, __VA_ARGS__)(__VA_ARGS__)

/**
 * USAGE:
 * 1. actvxcap_fscall(name, fwd, refwd)
 * 2. actvxcap_fscall(name, fwd, refwd, arg0, arg1, ...)
 * actvxcap_fscall can deal with 20 arguments at most.
 */
#define actvxcap_fscall1(name, fwd, refwd) ({\
	int tmp_ret = E_HM_INVAL; \
	rref_t fsmgr_rref = fsmgr_rref_of(); \
	if (fsmgr_rref != FS_INVALID_CNODE_RREF) { \
		tmp_ret = actvxactcapcall_fscall_##name(fwd, refwd, fsmgr_rref); \
		release_fsmgr_rref(fsmgr_rref); \
	} \
	tmp_ret; \
	})

#define actvxcap_fscall2(name, fwd, refwd, args...) ({ \
	long tmp_ret = E_HM_INVAL; \
	rref_t fsmgr_rref = fsmgr_rref_of(); \
	if (fsmgr_rref != FS_INVALID_CNODE_RREF) { \
		tmp_ret = actvxactcapcall_fscall_##name(fwd, refwd, fsmgr_rref, args); \
		release_fsmgr_rref(fsmgr_rref); \
	} \
	tmp_ret; \
	})

#define __actvxcap_fscall(func, name, fwd, refwd, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, \
			  type, ...) func##type
#define _actvxcap_fscall(func, ...) \
	__actvxcap_fscall(func, __VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, ...)
#define actvxcap_fscall(...) _actvxcap_fscall(actvxcap_fscall, __VA_ARGS__)(__VA_ARGS__)

#endif	/* LIBVFS_FS_SERVER_H */
