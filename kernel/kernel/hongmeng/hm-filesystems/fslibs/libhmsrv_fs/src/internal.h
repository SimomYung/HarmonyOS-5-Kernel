/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of internal
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 18 15:23:43 2019
 */

#ifndef LIBHMSRV_FS_INTERNAL_H
#define LIBHMSRV_FS_INTERNAL_H

#include <fs_server.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(VFSCORE);

/**
 * cpath - Client path
 * the wrapper for path resolution in fslibs
 */
struct cpath {
	int cp_dfd;			  /* dfd in server not user input */
	struct server_info *cp_srv;	  /* the server we expect */
	char *cp_pathbuf;		  /* send buf and receive buf */
	size_t cp_pathlen;		  /* the len of path in pathbuf  */
	char cp_srvname[SRVNAME_MAX_LEN]; /* new server name */
	int cp_me_idx;
};

/* src/fs_malloc.c */
#include <utils/fs_malloc.h>

/* src/internal.c */
int check_path(const char *path);
int str_ncheck(const char *src, const unsigned long long src_max_len);
int str_check(const char *src);

/* src/clock.c */
#define CLOCKFD			3U
#define fd_to_clockid(fd)	(clockid_t)(((~((unsigned int)(fd))) << 3) | CLOCKFD)
#define clockid_to_fd(clk)	(int)(~((unsigned int)((clk) >> 3)))

/* src/fs_server.c */
void g_sil_update_rref(void);
struct server_info *g_sil_acquire_si_by_rref(rref_t rref);
int g_sil_rebuild(const struct meta_server_info *msi, uint32_t num);
void g_sil_rebuild_fsmgr(void);

static inline bool is_same_server(const struct server_info *si1,
				  const struct server_info *si2)
{
	return (si1->cnode_idx == si2->cnode_idx);
}

bool get_ksys_param_is_fwd(void);
#endif
