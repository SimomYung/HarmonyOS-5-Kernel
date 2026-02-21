/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: util functions to do userspace memory copy
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 5 15:48:34 2023
 */

#include "ucopy.h"

#include "osal.h"
#include "ux_socket.h"
#include "ux_compat.h"

#include <stdint.h>
#include <sys/uio.h>
#include <sys/socket.h>

#include <libhmiov/iov_iter.h>

#define MAX_RW_COUNT (INT_MAX & PAGE_MASK)

static int __ucopy_compat_iovec(struct iovec *iov, const struct iovec32 *uiov, size_t nr_segs)
{
	int ret;
	struct iovec32 iovstack[UIO_FASTIOV];
	struct iovec32 *compat_iov;

	if (nr_segs > UIO_FASTIOV) {
		compat_iov = osal_calloc(nr_segs, sizeof(struct iovec32));
		if (compat_iov == NULL) {
			return E_HM_NOMEM;
		}
	} else {
		compat_iov = iovstack;
	}

	if (osal_copy_from_user(compat_iov, uiov, sizeof(*compat_iov) * nr_segs) != E_HM_OK) {
		ret = E_HM_POSIX_FAULT;
		goto free_iov;
	}

	for (unsigned i = 0; i < nr_segs; i++) {
		if ((int)compat_iov[i].iov_len < 0) {
			ret = E_HM_INVAL;
			goto free_iov;
		}
		iov[i].iov_base = compat_ptr(compat_iov[i].iov_base);
		iov[i].iov_len = compat_iov[i].iov_len;
	}

	ret = E_HM_OK;

free_iov:
	if (compat_iov != iovstack) {
		osal_free(compat_iov);
	}

	return ret;
}

static int __ucopy_iovec(struct iovec *iov, const struct iovec *uiov, size_t nr_segs)
{
	if (osal_copy_from_user(iov, uiov, nr_segs * sizeof(*uiov)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	for (unsigned i = 0; i < nr_segs; i++) {
		if ((ssize_t)iov[i].iov_len < 0) {
			return E_HM_INVAL;
		}
	}

	return E_HM_OK;
}

struct iovec *ucopy_iovec(const struct iovec *uiov, size_t nr_segs, unsigned fast_segs,
			  struct iovec *fast_iov)
{
	int err;
	struct iovec *iov = fast_iov;

	/* this is valid in Linux */
	if (nr_segs == 0) {
		return iov;
	}

	if (nr_segs > UIO_MAXIOV) {
		/* FIXME: introduce ERR_PTR and return EINVAL here */
		return NULL;
	}

	if (nr_segs > fast_segs) {
		iov = osal_calloc(nr_segs, sizeof(struct iovec));
		if (iov == NULL) {
			/* FIXME: return ERR_PTR(ENOMEM); */
			return NULL;
		}
	}

	if (ux_is_compat_client()) {
		err = __ucopy_compat_iovec(iov, (const struct iovec32 *)(uintptr_t)uiov, nr_segs);
	} else {
		err = __ucopy_iovec(iov, uiov, nr_segs);
	}

	if (err != E_HM_OK) {
		if (iov != fast_iov) {
			osal_free(iov);
		}
		/* FIXME: return ERR_PTR(err); */
		return NULL;
	}

	return iov;
}

ssize_t ucopy_import_iovec(struct iovec *uiov, size_t nr_segs, unsigned fast_segs,
			   struct iovec **iovp, struct iov_iter *iter)
{
	int err;
	size_t total_len = 0;
	struct iovec *iov;

	iov = ucopy_iovec(uiov, nr_segs, fast_segs, *iovp);
	if (iov == NULL) {
		return E_HM_POSIX_FAULT;
	}

	for (unsigned i = 0; i < nr_segs; i++) {
		ssize_t len = (ssize_t)iov[i].iov_len;
		if (len < 0) {
			/* return EINVAL if element length is < 0 when cast to ssize_t */
			if (iov != *iovp) {
				osal_free(iov);
			}
			return E_HM_INVAL;
		}
		if ((size_t)len > MAX_RW_COUNT - total_len) {
			len = (ssize_t)(MAX_RW_COUNT - total_len);
			iov[i].iov_len = (size_t)len;
		}
		total_len += (size_t)len;
	}

	err = iov_iter_init(iter, iov, nr_segs);
	if (err != E_HM_OK) {
		if (iov != *iovp) {
			osal_free(iov);
		}
		return err;
	}

	/* let caller free iov in case ucopy_iovec did mem alloc */
	if (*iovp != iov) {
		*iovp = iov;
	}

	return (ssize_t)total_len;
}

int ucopy_addr_read(struct sockaddr_storage *kaddr, const void *uaddr, int ulen)
{
	if (ulen < 0 || (unsigned)ulen > sizeof(struct sockaddr_storage)) {
		return E_HM_INVAL;
	}

	if (ulen == 0 || kaddr == NULL) {
		/* not need to copy, return success */
		return E_HM_OK;
	}

	if (osal_copy_from_user(kaddr, uaddr, (unsigned long)(unsigned)ulen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return E_HM_OK;
}

int ucopy_addr_write(struct sockaddr_storage *kaddr, int klen, void *uaddr, socklen_t *ulen)
{
	int err, len;

	BUG_ON((unsigned)klen > sizeof(struct sockaddr_storage));
	err = osal_copy_from_user(&len, ulen, sizeof(*ulen));
	if (err != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (len > klen) {
		len = klen;
	}

	if (len < 0) {
		return E_HM_INVAL;
	}

	if (len) {
		/* XXX: addr audit */
		if (osal_copy_to_user(uaddr, (void *)kaddr, (unsigned long)(unsigned)len) != E_HM_OK) {
			return E_HM_POSIX_FAULT;
		}
	}

	err = osal_copy_to_user(ulen, &klen, sizeof(*ulen));
	if (err != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return E_HM_OK;
}

static int __ucopy_compat_msghdr(struct netd_msghdr *msg_serv,
				 const struct msghdr *msg_user,
				 struct sockaddr **save_addr,
				 struct iovec **uiov, size_t *nsegs)
{
	struct msghdr32 msg;
	struct msghdr32 *cmsghdr_user = (struct msghdr32 *)(uintptr_t)msg_user;

	if (osal_copy_from_user(&msg, cmsghdr_user, sizeof(msg)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	msg_serv->msg_control = compat_ptr(msg.msg_control);
	msg_serv->msg_controllen = msg.msg_controllen;
	msg_serv->msg_flags = msg.msg_flags;

	msg_serv->msg_namelen = msg.msg_namelen;
	if (compat_ptr(msg.msg_name) == NULL) {
		msg_serv->msg_namelen = 0;
	}

	if (msg_serv->msg_namelen > sizeof(struct sockaddr_storage)) {
		msg_serv->msg_namelen = sizeof(struct sockaddr_storage);
	}

	if (save_addr != NULL) {
		/* recvmsg: save userspace msg_name ptr */
		*save_addr = compat_ptr(msg.msg_name);
	}

	if (compat_ptr(msg.msg_name) != NULL && msg_serv->msg_namelen != 0) {
		if (save_addr == NULL) {
			/* sendmsg: copy msg_name from userspace */
			int err = ucopy_addr_read(msg_serv->msg_name,
					compat_ptr(msg.msg_name), (int)msg_serv->msg_namelen);
			if (err != E_HM_OK) {
				return E_HM_POSIX_FAULT;
			}
		}
	} else {
		msg_serv->msg_namelen = 0;
		msg_serv->msg_name = NULL;
	}

	/* msg_iov */
	if (msg.msg_iovlen > UIO_MAXIOV) {
		return E_HM_MSGSIZE;
	}

	*uiov = compat_ptr(msg.msg_iov);
	*nsegs = (size_t)(unsigned)msg.msg_iovlen;

	return E_HM_OK;
}

static int __ucopy_msghdr(struct netd_msghdr *msg_serv,
			  const struct msghdr *msg_user,
			  struct sockaddr **save_addr,
			  struct iovec **uiov, size_t *nsegs)
{
	struct msghdr msg;

	if (osal_copy_from_user(&msg, msg_user, sizeof(msg)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	/* msg_control & msg_flags */
	msg_serv->msg_control = msg.msg_control;
	msg_serv->msg_controllen = msg.msg_controllen;
	msg_serv->msg_flags = msg.msg_flags;

	/* msg_name & msg_namelen */
	msg_serv->msg_namelen = msg.msg_namelen;
	if (msg.msg_name == NULL) {
		msg_serv->msg_namelen = 0;
	}

	if (msg_serv->msg_namelen > sizeof(struct sockaddr_storage)) {
		msg_serv->msg_namelen = sizeof(struct sockaddr_storage);
	}

	if (save_addr != NULL) {
		/* recvmsg: save userspace msg_name ptr */
		*save_addr = msg.msg_name;
	}

	if (msg.msg_name != NULL && msg_serv->msg_namelen != 0) {
		if (save_addr == NULL) {
			/* sendmsg: copy msg_name from userspace */
			int err = ucopy_addr_read(msg_serv->msg_name,
					       msg.msg_name, (int)msg_serv->msg_namelen);
			if (err != E_HM_OK) {
				return E_HM_POSIX_FAULT;
			}
		}
	} else {
		msg_serv->msg_name = NULL;
		msg_serv->msg_namelen = 0;
	}

	/* msg_iov */
	if (msg.msg_iovlen > UIO_MAXIOV) {
		return E_HM_MSGSIZE;
	}

	*uiov = msg.msg_iov;
	*nsegs = (size_t)(unsigned)msg.msg_iovlen;

	return E_HM_OK;
}

static int ucopy_compat_msghdr(struct netd_msghdr *msg_serv,
			       const struct msghdr *msg_user,
			       struct sockaddr **save_addr,
			       struct iovec **fastiov)
{
	ssize_t err;

	err = __ucopy_compat_msghdr(msg_serv, msg_user, save_addr,
				    &msg_serv->msg_iov, &msg_serv->msg_iovlen);
	if (err != E_HM_OK) {
		return (int)err;
	}

	err = ucopy_import_iovec(msg_serv->msg_iov, msg_serv->msg_iovlen,
				 UIO_FASTIOV, fastiov, &msg_serv->msg_iter);
	return err < 0 ? (int)err : E_HM_OK;
}

static int ucopy_msghdr(struct netd_msghdr *msg_serv,
			const struct msghdr *msg_user,
			struct sockaddr **save_addr,
			struct iovec **fastiov)
{
	ssize_t err;

	err = __ucopy_msghdr(msg_serv, msg_user, save_addr,
			    &msg_serv->msg_iov, &msg_serv->msg_iovlen);
	if (err != E_HM_OK) {
		return (int)err;
	}

	err = ucopy_import_iovec(msg_serv->msg_iov, msg_serv->msg_iovlen,
				 UIO_FASTIOV, fastiov, &msg_serv->msg_iter);
	return err < 0 ? (int)err : E_HM_OK;
}

int ucopy_sendmsg_msghdr_read(struct netd_msghdr *msg_serv,
			      const struct msghdr *msg_user,
			      struct iovec **fastiov)
{
	int err = E_HM_OK;

	if (ux_is_compat_client()) {
		err = ucopy_compat_msghdr(msg_serv, msg_user, NULL, fastiov);
	} else {
		err = ucopy_msghdr(msg_serv, msg_user, NULL, fastiov);
	}

	return err;
}

int ucopy_recvmsg_msghdr_read(struct netd_msghdr *msg_serv,
			      const struct msghdr *msg_user,
			      struct sockaddr **uaddr,
			      struct iovec **fastiov)
{
	int err = E_HM_OK;

	if (ux_is_compat_client()) {
		err = ucopy_compat_msghdr(msg_serv, msg_user, uaddr, fastiov);
	} else {
		err = ucopy_msghdr(msg_serv, msg_user, uaddr, fastiov);
	}

	return err;
}

static int __ucopy_compat_cmsg(struct cmsghdr *cmsg, unsigned long cmlen, struct msghdr *msg)
{
	unsigned int cmsg32len;
	unsigned long cmsglen = cmlen, tmp;
	struct cmsghdr32 *cmsg32;

	cmsg32 = CMSG32_FIRSTHDR(msg);
	while (cmsg32 != NULL) {
		cmsg32len = cmsg32->cmsg_len;
		if (!CMSG32_OK(cmsg32len, cmsg32, msg->msg_control, msg->msg_controllen)) {
			net_warn(HMNET_UNIX, "invalid cmsghdr32, len=%u, ctllen=%u\n",
				 cmsg32len, (unsigned int)msg->msg_controllen);
			return E_HM_INVAL;
		}

		tmp = (cmsg32len - sizeof(struct cmsghdr32)) + sizeof(struct cmsghdr);
		if (cmsglen < CMSG_ALIGN(tmp)) {
			net_warn(HMNET_UNIX, "not enough cmsg space, cmsglen=%lu, tmp=%lu\n",
				 cmsglen, CMSG_ALIGN(tmp));
			return E_HM_INVAL;
		}
		/* convert cmsghdr32 to cmsghdr */
		cmsg->cmsg_len = (socklen_t)tmp;
		cmsg->cmsg_level = cmsg32->cmsg_level;
		cmsg->cmsg_type = cmsg32->cmsg_level;
		tmp = CMSG_ALIGN(tmp);

		/* copy cmsg data */
		if (cmsg32len > sizeof(struct cmsghdr32)) {
			NOFAIL(memcpy_s(CMSG_DATA(cmsg), cmsglen - sizeof(struct cmsghdr),
				CMSG32_DATA(cmsg32), cmsg32len - sizeof(struct cmsghdr32)));
		}

		/* advance */
		cmsg = (struct cmsghdr *)(uintptr_t)((char *)cmsg + tmp);
		cmsg32 = cmsg32_nxthdr(msg->msg_control,
				       (socklen_t)msg->msg_controllen, cmsg32, cmsg32len);
		cmsglen -= tmp;
	}

	return E_HM_OK;
}

static int ucopy_compat_cmsg(struct netd_msghdr *msg, char *cmsg_stack, size_t stacklen)
{
	int ret = E_HM_OK;
	struct netd_msghdr mhdr;
	unsigned long cmsglen;
	struct cmsghdr *cmsg;
	char compat_fast[sizeof(struct cmsghdr32) + 20];

	/* hm copy_from_user's performance is not well, avoid multi-times copy */
	mhdr.msg_controllen = msg->msg_controllen;
	if (msg->msg_controllen > sizeof(compat_fast)) {
		mhdr.msg_control = osal_malloc(msg->msg_controllen);
		if (mhdr.msg_control == NULL) {
			return E_HM_NOMEM;
		}
	} else {
		mhdr.msg_control = compat_fast;
	}
	if (osal_copy_from_user(mhdr.msg_control,
				msg->msg_control, msg->msg_controllen) != E_HM_OK) {
		ret = E_HM_POSIX_FAULT;
		goto err_freebuf;
	}

	/* calculate cmsglen needed */
	cmsglen = ux_get_compat_cmsglen((struct msghdr *)&mhdr);
	if (cmsglen == 0) {
		ret = E_HM_INVAL;
		goto err_freebuf;
	}

	cmsg = (struct cmsghdr *)(uintptr_t)cmsg_stack;
	if (cmsglen > stacklen) {
		cmsg = (struct cmsghdr *)osal_malloc(cmsglen);
		if (cmsg == NULL) {
			ret = E_HM_NOMEM;
			goto err_freebuf;
		}
	}

	/* convert compat cmsghdr to native cmsghdr */
	ret = __ucopy_compat_cmsg(cmsg, cmsglen, (struct msghdr *)(void *)&mhdr);
	if (ret != E_HM_OK) {
		goto err_freecmsg;
	}

	msg->msg_control = cmsg;
	msg->msg_controllen = cmsglen;
	/* free allocated mem if needed */
	if (mhdr.msg_control != compat_fast) {
		osal_free(mhdr.msg_control);
	}

	return ret;
err_freecmsg:
	if (cmsg != (struct cmsghdr *)(uintptr_t)cmsg_stack) {
		osal_free(cmsg);
	}
err_freebuf:
	if (mhdr.msg_control != compat_fast) {
		osal_free(mhdr.msg_control);
	}

	return ret;
}

int ucopy_cmsg_read(struct netd_msghdr *msg, char **fastcmsg, size_t fastcmsg_len)
{
	int ret;
	char *ctl_buf = *fastcmsg;
	unsigned long ctl_len;

	ret = E_HM_NOBUFS;
	if (msg->msg_controllen > MAX_MSG_CTRL_LEN) {
		goto out;
	}

	ctl_len = msg->msg_controllen;
	if (ctl_len != 0 && ux_is_compat_client()) {
		ret = ucopy_compat_cmsg(msg, *fastcmsg, fastcmsg_len);
		if (ret != E_HM_OK) {
			goto out;
		}
		ctl_buf = msg->msg_control;
	} else if (ctl_len != 0) {
		if (ctl_len > fastcmsg_len) {
			/* XXX: introduce some limitation mechanism */
			ctl_buf = osal_malloc(ctl_len);
			if (ctl_buf == NULL) {
				goto out;
			}
		}
		ret = E_HM_POSIX_FAULT;
		if (osal_copy_from_user(ctl_buf, msg->msg_control, ctl_len) != E_HM_OK) {
			goto out_freebuf;
		}
		msg->msg_control = ctl_buf;
	}

	if (ctl_buf != *fastcmsg) {
		*fastcmsg = ctl_buf;
	}

	return E_HM_OK;
out_freebuf:
	if (ctl_buf != *fastcmsg) {
		osal_free(ctl_buf);
	}
out:
	return ret;
}

static int ucopy_cmsg_write_compat(struct msghdr *msg,
				   int level, int type, size_t len, void *data)
{
	struct cmsghdr32 cmhdr;
	struct cmsghdr32 *cm = (struct cmsghdr32 *)msg->msg_control;
	unsigned int cmlen;

	if (cm == NULL || msg->msg_controllen < sizeof(*cm)) {
		msg->msg_flags = (int)(((unsigned int)msg->msg_flags) | MSG_CTRUNC);
		return E_HM_OK;
	}

	cmlen = (unsigned int)CMSG32_LEN(len);
	if (msg->msg_controllen < cmlen) {
		msg->msg_flags = (int)(((unsigned int)msg->msg_flags) | MSG_CTRUNC);
		cmlen = msg->msg_controllen;
	}

	cmhdr.cmsg_level = level;
	cmhdr.cmsg_len = cmlen;
	cmhdr.cmsg_type = type;
	if (osal_copy_to_user(cm, &cmhdr, sizeof(cmhdr)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (osal_copy_to_user(CMSG32_DATA(cm), data, cmlen - sizeof(cmhdr)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	cmlen = (unsigned int)CMSG32_SPACE(len);
	if (msg->msg_controllen < cmlen) {
		cmlen = msg->msg_controllen;
	}
	msg->msg_control = (void *)((uintptr_t)msg->msg_control + (uintptr_t)cmlen);
	msg->msg_controllen -= cmlen;
	return E_HM_OK;
}

int ucopy_cmsg_write(struct msghdr *msg, int level, int type, size_t len, void *data)
{
	struct cmsghdr cmhdr = {0};
	struct cmsghdr *cm = (struct cmsghdr *)msg->msg_control;
	unsigned int cmlen = (unsigned int)CMSG_LEN(len);

	if (ux_is_compat_client()) {
		return ucopy_cmsg_write_compat(msg, level, type, len, data);
	}

	if (msg->msg_control == NULL || msg->msg_controllen < sizeof(struct cmsghdr)) {
		msg->msg_flags = (int)(((unsigned int)msg->msg_flags) | MSG_CTRUNC);
		return E_HM_OK;
	}

	if (msg->msg_controllen < cmlen) {
		msg->msg_flags = (int)(((unsigned int)msg->msg_flags) | MSG_CTRUNC);
		cmlen = msg->msg_controllen;
	}

	cmhdr.cmsg_level = level;
	cmhdr.cmsg_type = type;
	cmhdr.cmsg_len = cmlen;
	if (osal_copy_to_user(cm, &cmhdr, sizeof(cmhdr)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (osal_copy_to_user(CMSG_DATA(cm), data, cmlen - sizeof(cmhdr)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	cmlen = (unsigned int)CMSG_SPACE(len);
	if (msg->msg_controllen < cmlen) {
		cmlen = msg->msg_controllen;
	}

	msg->msg_control = (void *)((uintptr_t)msg->msg_control + (uintptr_t)cmlen);
	msg->msg_controllen -= cmlen;
	return E_HM_OK;
}
