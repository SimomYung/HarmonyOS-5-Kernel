/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: For ILP32 compat
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 25 10:34:28 2020
 */

#include <hmkernel/errno.h>
#include <hongmeng/errno.h>
#include <stdbool.h>
#include <errno.h>
#include "ux_event.h"
#include "ux_compat.h"
#include "ux_netlog.h"

int ux_get_compat_fprog(const void *optval, struct ux_sock_fprog *fprog)
{
	struct ux_compat_sock_fprog compat_fprog = {0};

	if (hm_actv_read_xact_vm(&compat_fprog, optval, sizeof(compat_fprog)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	fprog->len = compat_fprog.len;
	fprog->filter = (void *)(uintptr_t)compat_fprog.filter;

	return 0;
}

int ux_get_compat_timeval(const void *optval, struct timeval *tv)
{
	struct timeval32 tv32 = {0};

	if (hm_actv_read_xact_vm(&tv32, optval, sizeof(tv32)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	tv->tv_sec = tv32.tv_sec;
	tv->tv_usec = tv32.tv_usec;

	return 0;
}

int ux_set_compat_timeval(const struct timeval *tv, void *optval)
{
	struct timeval32 tv32 = {0};

	tv32.tv_sec = (int)tv->tv_sec;
	tv32.tv_usec = (int)tv->tv_usec;

	if (hm_actv_write_xact_vm(optval, &tv32, sizeof(tv32)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

int ux_get_compat_msghdr(const void *msg, struct msghdr *srv_msg)
{
	struct msghdr32 msg32 = {0};

	if (hm_actv_read_xact_vm(&msg32, msg, sizeof(struct msghdr32)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	srv_msg->msg_name = (void *)(uintptr_t)msg32.msg_name;
	srv_msg->msg_namelen = msg32.msg_namelen;
	srv_msg->msg_iov = (void *)(uintptr_t)msg32.msg_iov;
	srv_msg->msg_iovlen = msg32.msg_iovlen;
	srv_msg->msg_control = (void *)(uintptr_t)msg32.msg_control;
	srv_msg->msg_controllen = msg32.msg_controllen;
	srv_msg->msg_flags = msg32.msg_flags;

	return 0;
}

int ux_get_compat_iov(const void *iov, size_t iovlen, struct iovec *srv_iov)
{
	struct iovec32 *iov32 = NULL;
	int i;
	int ret;

	if ((int)iovlen < 0) {
		return E_HM_INVAL;
	}

	if (iovlen == 0) {
		return E_HM_OK;
	}

	iov32 = calloc(iovlen, sizeof(struct iovec32));
	if (iov32 == NULL) {
		return E_HM_NOMEM;
	}

	ret = hm_actv_read_xact_vm(iov32, iov, iovlen * sizeof(struct iovec32));
	if (ret != 0) {
		ret = E_HM_POSIX_FAULT;
		goto out;
	}
	for (i = 0; i < (int)iovlen; i++) {
		srv_iov[i].iov_base = (void *)(uintptr_t)iov32[i].iov_base;
		srv_iov[i].iov_len = (size_t)iov32[i].iov_len;
	}

out:
	free(iov32);
	return ret;
}

#ifndef CMSG_OK
#define CMSG_OK(mhdr, cmsg) \
	((cmsg)->cmsg_len >= (socklen_t)sizeof(struct cmsghdr) && \
	 (cmsg)->cmsg_len <= (socklen_t)((mhdr)->msg_controllen - \
	 (unsigned int)((char *)(cmsg) - (char *)(mhdr)->msg_control)))
#endif

#ifdef CONFIG_SCM_RIGHTS
#define CMSG_ASSIGN_SCM(cmsghdr, data)	\
	(*(ux_scm_cookie_t *)(uintptr_t)((char *)(cmsghdr) + MAX_MSG_CTRL_LEN) = 	\
	*(ux_scm_cookie_t *)(uintptr_t)(((char *)(data) + MAX_MSG_CTRL_LEN)))
#endif

unsigned int ux_get_compat_cmsglen(const struct msghdr *msg)
{
	struct cmsghdr32 *cmsg32 = NULL;
	unsigned int cm32len = 0;
	unsigned int cmlen = 0;
	unsigned int tmp = 0;

	cmsg32 = CMSG32_FIRSTHDR(msg);
	while (cmsg32 != NULL) {
		cm32len = cmsg32->cmsg_len;
		if (!CMSG32_OK(cm32len, cmsg32, msg->msg_control, msg->msg_controllen)) {
			return 0;
		}
		tmp = (cm32len - sizeof(struct cmsghdr32)) + sizeof(struct cmsghdr);
		cmlen += CMSG_ALIGN(tmp);
		cmsg32 = cmsg32_nxthdr(msg->msg_control, msg->msg_controllen, cmsg32, cm32len);
	}

	return cmlen;
}

int ux_compat_cmsghdr2cmsghdr(struct msghdr *msg)
{
	struct cmsghdr32 *cmsg32 = NULL;
	struct cmsghdr *cmsg_out = NULL;
	struct cmsghdr *cmsg = NULL;
	unsigned int cm32len, tmp;

	if (msg == NULL || msg->msg_control == NULL) {
		return E_HM_INVAL;
	}
	socklen_t msg_ctl_len = msg->msg_controllen;
	void *head = msg->msg_control;
	unsigned int cmlen = ux_get_compat_cmsglen(msg);
	if (cmlen == 0) {
		return E_HM_INVAL;
	}

#ifdef CONFIG_SCM_RIGHTS
	cmsg_out = calloc(MAX_MSG_TOLCTRL_LEN, 1);
#else
	cmsg_out = calloc(cmlen, 1);
#endif
	if (cmsg_out == NULL) {
		return E_HM_NOBUFS;
	}

	cmsg = cmsg_out;
	cmsg32 = head;
	while (cmsg32 != NULL) {
		cm32len = cmsg32->cmsg_len;
		if (!CMSG32_OK(cm32len, cmsg32, msg->msg_control, msg->msg_controllen)) {
			net_warn(HMNET_CORE, "cmsg32->cmsg_len=%u, cmsg_type=%d, cmsg_level=%d\n",
				 cmsg32->cmsg_len, cmsg32->cmsg_type, cmsg32->cmsg_level);
			break;
		}
		tmp = (cm32len - sizeof(struct cmsghdr32)) + sizeof(struct cmsghdr);
		cmsg->cmsg_len = tmp;
		cmsg->cmsg_level = cmsg32->cmsg_level;
		cmsg->cmsg_type = cmsg32->cmsg_type;
		tmp = CMSG_ALIGN(tmp);

		if (cm32len > sizeof(struct cmsghdr32)) {
			NOFAIL(memcpy_s(CMSG_DATA(cmsg), cm32len - sizeof(struct cmsghdr32),
				CMSG32_DATA(cmsg32), cm32len - sizeof(struct cmsghdr32)));
		}

		cmsg = (struct cmsghdr *)(uintptr_t)((char *)cmsg + tmp);
		cmsg32 = cmsg32_nxthdr(head, msg_ctl_len, cmsg32, cm32len);
	}
#ifdef CONFIG_SCM_RIGHTS
	CMSG_ASSIGN_SCM(cmsg_out, msg->msg_control);
#endif
	free(msg->msg_control);
	msg->msg_control = cmsg_out;
	msg->msg_controllen = cmlen;
	return 0;
}

int ux_cmsghdr2compat_cmsghdr(struct msghdr *msg, char *msg_control)
{
	struct cmsghdr32 *cmsg32 = (struct cmsghdr32 *)(void *)msg_control;
	struct cmsghdr *cmsg = NULL;
	unsigned int tmp;
	int ret;

	if (msg == NULL || msg_control == NULL) {
		return E_HM_INVAL;
	}
	cmsg = CMSG_FIRSTHDR(msg);
	while (cmsg != NULL) {
		if (!CMSG_OK(msg, cmsg)) {
			net_warn(HMNET_CORE, "cmsg->cmsg_len=%u, cmsg_type=%d, cmsg_level=%d\n",
				 cmsg->cmsg_len, cmsg->cmsg_type, cmsg->cmsg_level);
			break;
		}
		cmsg32->cmsg_level = cmsg->cmsg_level;
		cmsg32->cmsg_type = cmsg->cmsg_type;
		tmp = (cmsg->cmsg_len - (unsigned int)sizeof(struct cmsghdr)) +
			(unsigned int)sizeof(struct cmsghdr32);
		cmsg32->cmsg_len = tmp;

		ret = memcpy_s(CMSG32_DATA(cmsg32), tmp - (unsigned int)sizeof(struct cmsghdr32),
				CMSG_DATA(cmsg), cmsg->cmsg_len - sizeof(struct cmsghdr));
		if (ret != 0) {
			net_warn(HMNET_CORE, "ret=%d, cmsg->cmsg_len=%u, cmsg_type=%d, cmsg_level=%d\n",
				 ret, cmsg->cmsg_len, cmsg->cmsg_type, cmsg->cmsg_level);
			break;
		}

		cmsg = CMSG_NXTHDR(msg, cmsg);
		cmsg32 = (struct cmsghdr32 *)(uintptr_t)((char *)cmsg32 + tmp);
	}

#ifdef CONFIG_SCM_RIGHTS
	if (msg->msg_control != NULL) {
		CMSG_ASSIGN_SCM(msg_control, msg->msg_control);
	}
#endif
	msg->msg_controllen = (socklen_t)((char *)cmsg32 - msg_control);
	return E_HM_OK;
}

void ux_rtentry2trentry32(const struct rtentry *rt, struct rtentry32 *rt32)
{
	rt32->rt_pad1 = (unsigned int)rt->rt_pad1;
	rt32->rt_dst = rt->rt_dst;
	rt32->rt_gateway = rt->rt_gateway;
	rt32->rt_genmask = rt->rt_genmask;
	rt32->rt_flags = rt->rt_flags;
	rt32->rt_pad2 = rt->rt_pad2;
	rt32->rt_pad3 = (unsigned int)rt->rt_pad3;
	rt32->rt_tos = rt->rt_tos;
	/* if needed, please do rt_pad4 convert at here */
	rt32->rt_class = rt->rt_class;
	rt32->rt_metric = rt->rt_metric;
	rt32->rt_dev = (unsigned int)(uintptr_t)rt->rt_dev;
	rt32->rt_mtu = (unsigned int)rt->rt_mtu;
	rt32->rt_window = (unsigned int)rt->rt_window;
	rt32->rt_irtt = rt->rt_irtt;
}

void ux_rtentry322trentry(const struct rtentry32 *rt32, struct rtentry *rt)
{
	rt->rt_pad1 = rt32->rt_pad1;
	rt->rt_dst = rt32->rt_dst;
	rt->rt_gateway = rt32->rt_gateway;
	rt->rt_genmask = rt32->rt_genmask;
	rt->rt_flags = rt32->rt_flags;
	rt->rt_pad2 = rt32->rt_pad2;
	rt->rt_pad3 = rt32->rt_pad3;
	rt->rt_tos = rt32->rt_tos;
	/* if needed, please do rt_pad4 convert at here */
	rt->rt_class = rt32->rt_class;
	rt->rt_metric = rt32->rt_metric;
	rt->rt_dev = (void *)(uintptr_t)rt32->rt_dev;
	rt->rt_mtu = rt32->rt_mtu;
	rt->rt_window = rt32->rt_window;
	rt->rt_irtt = rt32->rt_irtt;
}

void ux_msghdr32_to_msghdr(const struct msghdr32 *msg32, struct msghdr *msg)
{
	msg->msg_name = (void *)(uintptr_t)(msg32->msg_name);
	msg->msg_namelen = msg32->msg_namelen;
	msg->msg_iov = (struct iovec *)(uintptr_t)(msg32->msg_iov);
	msg->msg_iovlen = msg32->msg_iovlen;
	msg->msg_control = (void *)(uintptr_t)(msg32->msg_control);
	if (msg32->msg_controllen >= CMSG32_ALIGN(sizeof(struct cmsghdr32))) {
		msg->msg_controllen = (msg32->msg_controllen - CMSG32_ALIGN(sizeof(struct cmsghdr32))) + \
				       CMSG_ALIGN(sizeof(struct cmsghdr));
	} else {
		msg->msg_controllen = msg32->msg_controllen;
	}
	msg->msg_flags = msg32->msg_flags;
}

void ux_iovec32_to_iovec(const struct iovec32 *iov32, struct iovec *iov)
{
	iov->iov_base = (void *)(uintptr_t)(iov32->iov_base);
	iov->iov_len = (size_t)(iov32->iov_len);
}

void ux_compat_fixup_stat(struct compat_stat *dst, const struct stat *src)
{
	dst->st_dev = (unsigned long long)src->st_dev;
	dst->st_ino = (unsigned long long)src->st_ino;
	dst->st_nlink = (unsigned int)src->st_nlink;
	dst->st_uid = (unsigned int)src->st_uid;
	dst->st_gid = (unsigned int)src->st_gid;
	dst->st_mode = (unsigned int)src->st_mode;
	dst->st_rdev = (unsigned long long)src->st_rdev;
	dst->st_size = (long long)src->st_size;
	dst->st_blksize = (int)src->st_blksize;
	dst->st_blocks = (long long)src->st_blocks;
	/* timspec musl 2038 */
	dst->st_atim.tv_sec = (int)src->st_atim.tv_sec;
	dst->st_atim.tv_nsec = (int)src->st_atim.tv_nsec;
	dst->st_ctim.tv_sec = (int)src->st_ctim.tv_sec;
	dst->st_ctim.tv_nsec = (int)src->st_ctim.tv_nsec;
	dst->st_mtim.tv_sec = (int)src->st_mtim.tv_sec;
	dst->st_mtim.tv_nsec = (int)src->st_mtim.tv_nsec;
}
