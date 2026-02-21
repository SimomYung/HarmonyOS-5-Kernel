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

void scm_send(ux_scm_cookie_t *scm, const struct ucred *ucred,
	      ux_unix_buf_t *unix_buf);
bool scm_recv(const ux_unix_sock_t *usk, ux_scm_cookie_t *scm, ux_unix_buf_t *unix_buf);

#ifdef CONFIG_SCM_RIGHTS
int unix_scm_send(struct msghdr *msg, ux_scm_cookie_t *scm);
void unix_scm_recv(struct msghdr *msg, ux_scm_cookie_t *scm, int flags);
void unix_scm_destroy(ux_scm_cookie_t *scm);
void unix_scm_fds_destory(ux_scm_fd_t *fdp);
#else
static inline int unix_scm_send(struct msghdr *msg, ux_scm_cookie_t *scm)
{
	UNUSED(msg, scm);
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
