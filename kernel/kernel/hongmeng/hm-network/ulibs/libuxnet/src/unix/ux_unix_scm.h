/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Scm function headfile for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 12 10:21:00 2020
 */
#ifndef __UX_UNIX_SCM_H__
#define __UX_UNIX_SCM_H__

#include "ux_socket.h"
#include "ux_unix_util.h"

#define INVALID_UID ((uid_t)-1)
#define INVALID_GID ((gid_t)-1)

static inline bool uid_valid(uid_t uid)
{
	return uid != INVALID_UID;
}

static inline bool gid_valid(gid_t gid)
{
	return gid != INVALID_GID;
}

static inline bool uid_eq(uid_t left, uid_t right)
{
	return left == right;
}

static inline bool gid_eq(gid_t left, gid_t right)
{
	return left == right;
}

void scm_send(ux_scm_cookie_t *scm, const struct ucred *ucred,
	      ux_unix_buf_t *unix_buf);
bool scm_recv(const ux_unix_sock_t *usk, ux_scm_cookie_t *scm, ux_unix_buf_t *unix_buf);

#ifdef CONFIG_SCM_RIGHTS
int unix_scm_send(struct msghdr *msg, ux_scm_cookie_t *scm,
		   struct sec_chk_cred *cred);
void unix_scm_recv(struct msghdr *msg, ux_scm_cookie_t *scm, int flags);
void unix_scm_destroy(ux_scm_cookie_t *scm);
void unix_scm_fds_destory(ux_scm_fd_t *fdp);
#else
static inline int unix_scm_send(struct msghdr *msg, ux_scm_cookie_t *scm,
				 struct sec_chk_cred *cred)
{
	UNUSED(msg, scm, cred);
	return E_HM_OK;
}

static void unix_scm_recv(struct msghdr *msg, ux_scm_cookie_t *scm, int flags)
{
	UNUSED(msg, scm, flags);
}

static inline void unix_scm_destroy(ux_scm_cookie_t *scm)
{
	UNUSED(scm);
}

static inline void unix_scm_fds_destory(ux_scm_fd_t *fdp)
{
	UNUSED(fdp);
}
#endif

#endif /* __UX_UNIX_SCM_H__ */
