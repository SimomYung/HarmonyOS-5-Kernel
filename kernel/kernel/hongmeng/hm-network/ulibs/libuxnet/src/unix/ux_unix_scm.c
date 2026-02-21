/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Scm function for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 12 10:21:00 2020
 */

#include <stdio.h>

#include <vfs.h>
#include <fs_transfer_fd.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_procmgr.h>

#include "osal.h"
#include "ucopy.h"
#include "ux_fdtable.h"
#include "ux_unix_util.h"
#include "ux_unix_scm.h"
#include "ux_compat.h"
#include "uev/ux_uev.h"

#ifdef CONFIG_SCM_RIGHTS

#define SENDING_SOCKS_SIZE_DEFAULT 8
#define SENDING_SOCKS_SIZE_EXPAND_SHIFT 1

static int sending_socks_size_expand(int count)
{
	uint32_t cnode_idx = osal_hm_actv_src_cnode();
	unsigned long expand_count;
	struct rlimit nofile;
	int ret;

	ret = hm_rlimit_by_cnode_idx(cnode_idx, RLIMIT_NOFILE, NULL, &nofile);
	if (ret < 0) {
		net_error(HMNET_UNIX, "get rlimit err: %d\n", ret);
		return count;
	}

	if ((unsigned long)count >= (nofile.rlim_cur >> SENDING_SOCKS_SIZE_EXPAND_SHIFT)) {
		expand_count = nofile.rlim_cur;
	} else {
		expand_count = (unsigned long)count << SENDING_SOCKS_SIZE_EXPAND_SHIFT;
	}

	return (int)min(expand_count, INT_MAX / sizeof(ux_sock_trans_info_t));
}

/*
 * Send default credential info if user does not provide scm_cred info
 * explicitly.
 *
 * Here, we choose to send valid cred info always for simplicity. So
 * it is not compatible with Linux in the following case:
 * -----------------------------------------------------------------
 *		client			server
 *					1. listen
 *		2. connect
 *					3. accept
 *		4. send
 *					5. setsockopt(..., SO_PASSCRED, ...)
 *					6. recvmsg
 *------------------------------------------------------------------
 */
static void scm_fill_default_cred(const struct ucred *ucred,
				  ux_unix_buf_t *unix_buf)
{
	unix_buf->creds.passcd = 1;
	unix_buf->creds.pid = ucred->pid;
	unix_buf->creds.uid = ucred->uid;
	unix_buf->creds.gid = ucred->gid;
}

void scm_send(ux_scm_cookie_t *scm, const struct ucred *ucred,
	      ux_unix_buf_t *unix_buf)
{
	if (scm == NULL) {
		scm_fill_default_cred(ucred, unix_buf);
		return;
	}

	if (scm->fds.count != 0) {
		if (memcpy_s(&unix_buf->scm_fd, sizeof(unix_buf->scm_fd),
			     &scm->fds, sizeof(scm->fds)) != 0) {
			net_info(HMNET_UNIX, "copy scm fds to peer recv queue failed\n");
		}

		unix_buf->scm_fd.send_owner = scm->fds.send_owner;
		scm->fds.send_owner = NULL;
	}

	if (scm->creds.passcd != 0) {
		if (memcpy_s(&unix_buf->creds, sizeof(unix_buf->creds),
			     &scm->creds, sizeof(scm->creds)) != 0) {
			net_info(HMNET_UNIX, "copy scm creds to peer recv queue failed\n");
		}
	} else {
		scm_fill_default_cred(ucred, unix_buf);
	}
}

bool scm_recv(const ux_unix_sock_t *usk, ux_scm_cookie_t *scm,
	      ux_unix_buf_t *unix_buf)
{
	bool recvd = false;
	if (scm == NULL) {
		return recvd;
	}

	if (unix_buf->scm_fd.count != 0) {
		if (memcpy_s(&scm->fds, sizeof(scm->fds), &unix_buf->scm_fd,
			     sizeof(unix_buf->scm_fd)) != 0) {
			net_error(HMNET_UNIX, "copy fds to scm failed, count %c\n",
				  unix_buf->scm_fd.count);
		}

		scm->fds.send_owner = unix_buf->scm_fd.send_owner;
		unix_buf->scm_fd.send_owner = NULL;
		unix_buf->scm_fd.count = 0;

		recvd = true;
	}

	if (unix_buf->creds.passcd != 0) {
		if (usk->sk.passcred == 0) {
			scm->creds.passcd = 0;
			return recvd;
		}
		if (memcpy_s(&scm->creds, sizeof(scm->creds), &unix_buf->creds,
			     sizeof(unix_buf->creds)) != 0) {
			net_error(HMNET_UNIX, "copy creds to scm failed\n");
		}
		recvd = true;
	}

	return recvd;
}

static int unix_scm_copy_cred(struct cmsghdr *cmsg, ux_scm_creds_t *cred)
{
	struct ucred *ucred;

	if (cmsg->cmsg_len != CMSG_LEN(sizeof(struct ucred))) {
		return E_HM_INVAL;
	}

	/* XXX: validate pid, uid, gid */
	cred->passcd = 1;
	ucred = (struct ucred *)(uintptr_t)CMSG_DATA(cmsg);
	cred->pid = ucred->pid;
	cred->uid = ucred->uid;
	cred->gid = ucred->gid;

	uev_trace_scm_send_cred(cred);

	return E_HM_OK;
}

static void __vfs_put_send_fd(uint32_t sender_cidx, int fd)
{
	int err;
	struct server_info *si = NULL;
	vfs_transfer_fd_args_t xfer_fd_args = {0};

	/* don't support fs multi-sever */
	err = get_server_info("fsmgr", &si);
	if (err != E_HM_OK) {
		net_warn(HMNET_UNIX, "failed to get fsmgr server info, err=%d\n", err);
		return;
	}

	xfer_fd_args.send_cidx = sender_cidx;
	xfer_fd_args.recv_cidx = 0;
	xfer_fd_args.sfiles_idx = fd;
	err = vfs_put_send_fd(si, &xfer_fd_args);
	if (err != E_HM_OK) {
		net_warn(HMNET_UNIX,
			 "failed to put vfs send fd, sfiles_idx=%d, err=%d\n", fd, err);
	}

	put_server_info(si);
}

static int __vfs_send_fd(uint32_t sender_cidx, int fd)
{
	int err;
	struct server_info *si = NULL;
	vfs_transfer_fd_args_t xfer_fd_args = {0};

	/* don't support fs multi-sever */
	err = get_server_info("fsmgr", &si);
	if (err != E_HM_OK) {
		net_warn(HMNET_UNIX, "failed to get fsmgr server info, err=%d\n", err);
		goto out;
	}

	xfer_fd_args.send_cidx = sender_cidx;
	xfer_fd_args.recv_cidx = 0;
	xfer_fd_args.send_fd = fd;
	err = vfs_send_fd(si, &xfer_fd_args);
	if (err < 0) {
		net_warn(HMNET_UNIX, "failed to send vfs fd, fd=%d, err=%d\n", fd, err);
	}

	put_server_info(si);
out:
	return err;
}

static int ux_sock_owner_expand_ssocks_table(ux_sock_owner_t *owner)
{
	int ret = E_HM_OK;
	size_t new_ssocks_size;
	ux_sock_trans_info_t *ssocks = NULL;
	int new_count;

	new_count = sending_socks_size_expand(owner->ssocks_count);
	if (new_count <= owner->ssocks_count) {
		return E_HM_MFILE;
	}

	new_ssocks_size = new_count * sizeof(ux_sock_trans_info_t);

	ssocks = osal_malloc(new_ssocks_size);
	if (ssocks == NULL) {
		ret = E_HM_NOMEM;
	} else {
		NOFAIL(memset_s(ssocks, new_ssocks_size, 0, new_ssocks_size));
		NOFAIL(memcpy_s(ssocks, new_ssocks_size, owner->ssocks,
				owner->ssocks_count * sizeof(ux_sock_trans_info_t)));
		free(owner->ssocks);
		owner->ssocks = ssocks;
		owner->ssocks_count = new_count;
	}

	return ret;
}

/* should call with owner->lock, return idx if success */
static int __expand_ssocks_and_insert_trans_info(ux_sock_owner_t *owner, ux_sock_trans_info_t *trans_info)
{
	int origin_ssocks_count = owner->ssocks_count;
	int ret;

	ret = ux_sock_owner_expand_ssocks_table(owner);
	if (ret == E_HM_OK) {
		ret = origin_ssocks_count;
		owner->ssocks[ret] = *trans_info;
	}

	return ret;
}

static int __save_fd_trans_info(ux_sock_owner_t *owner, int fdtype, ux_socket_t *sock, int fd)
{
	int ret = E_HM_OK;
	ux_sock_trans_info_t trans_info;

	NOFAIL(memset_s(&trans_info, sizeof(trans_info), 0, sizeof(trans_info)));
	trans_info.send_cidx = osal_hm_actv_src_cnode();
	trans_info.fstype = fdtype;
	trans_info.sock = sock;
	trans_info.vfsfd_idx = fd;
	NOFAIL(strcpy_s(trans_info.fssrv_name, FSSRV_PATHNAME_LEN, "fsmgr"));

	raw_mutex_lock(&owner->sslock);

	if (owner->ssocks == NULL) {
		owner->ssocks = osal_calloc(SENDING_SOCKS_SIZE_DEFAULT, sizeof(ux_sock_trans_info_t));
		if (owner->ssocks == NULL) {
			ret = E_HM_NOMEM;
		} else {
			owner->ssocks_count = SENDING_SOCKS_SIZE_DEFAULT;
		}
	}

	if (ret == E_HM_OK) {
		ret = E_HM_MFILE;
		for (int i = 0; i < owner->ssocks_count; ++i) {
			if (owner->ssocks[i].fstype != SCM_FS_INVALID) {
				continue;
			}

			NOFAIL(memcpy_s(&owner->ssocks[i], sizeof(ux_sock_trans_info_t),
					&trans_info, sizeof(ux_sock_trans_info_t)));
			ret = i;
			break;
		}
	}

	if (ret == E_HM_MFILE) {
		ret = __expand_ssocks_and_insert_trans_info(owner, &trans_info);
	}

	raw_mutex_unlock(&owner->sslock);

	return ret;
}

#ifdef CONFIG_NETFS_FUSION
static int unix_scm_send_fd(ux_sock_owner_t *owner, ux_scm_fd_t *fdp,
			    int send_fd)
{
	int ret = E_HM_OK;
	struct sysarg_fdtable_fd_stat fd_stat = {0};
	ux_socket_t *sock = NULL;
	int fdtype, fsfd = -1; /* -1 is an invalid fd number */

	ret = syscap_FDTableGetFd(owner->fdtable_rref, send_fd, FD_MIXED_FS_NET, &fd_stat);
	if (ret < 0) {
		return ret;
	}
	if (fd_stat.fd_type == FD_SOCKFS) {
		sock = ux_socket_get(fdp->src_cnode_idx, send_fd);
		if (sock == NULL) {
			uev_trace_scm_send_log(__func__, "send sockfs fd failed", send_fd);
			return E_HM_BADF;
		}
		fdtype = SCM_FS_SOCKFS;
	} else if (fd_stat.fd_type == FD_FS) {
		fsfd = __vfs_send_fd(fdp->src_cnode_idx, send_fd);
		if (fsfd < 0) {
			uev_trace_scm_send_log(__func__, "send fs fd failed", ret);
			return fsfd;
		}
		fdtype = SCM_FS_VFS;
	} else if (fd_stat.fd_type == FD_NET) {
		sock = hm_net_sys_socket_get(fdp->src_cnode_idx, send_fd);
		if (sock == NULL) {
			return E_HM_BADF;
		}
		fdtype = SCM_FS_NET;
	} else {
		net_error(HMNET_UNIX, "not support fd_type %d\n", fd_stat.fd_type);
		return E_HM_INVAL;
	}

	int slot = __save_fd_trans_info(owner, fdtype, sock, fsfd);
	if (slot < 0) {
		if (fdtype == SCM_FS_NET) {
			hm_net_sys_socket_put(sock);
		} else if (fdtype == SCM_FS_SOCKFS) {
			ux_socket_put(sock);
		} else {
			__vfs_put_send_fd(fdp->src_cnode_idx, fsfd);
		}
		uev_trace_scm_send_log(__func__, "save trans info failed", ret);
		return slot;
	}

	/* scm_cookie construction */
	fdp->fds[fdp->count] = slot;
	fdp->type[fdp->count] = fdtype;
	fdp->file_type[fdp->count] = 0; /* 0 is NORMAL_FD */
	NOFAIL(strcpy_s(fdp->fssrv_name[fdp->count], FSSRV_PATHNAME_LEN, "fsmgr"));
	fdp->count++;

	return ret;
}
#else /* !CONFIG_NETFS_FUSION */
static int unix_scm_send_fd(ux_sock_owner_t *owner, ux_scm_fd_t *fdp,
			    int send_fd)
{
	int ret = E_HM_OK;
	int fdtype, fsfd = -1; /* -1 is an invalid fd number */
	ux_socket_t *sock = ux_socket_get(fdp->src_cnode_idx, send_fd);
	if (sock != NULL) {
		uev_trace_scm_send_log(__func__, "send net fd", send_fd);
		fdtype = SCM_FS_NET;
	} else {
		uev_trace_scm_send_log(__func__, "send fs fd", send_fd);
		fsfd = __vfs_send_fd(fdp->src_cnode_idx, send_fd);
		if (fsfd < 0) {
			ret = fsfd;
			uev_trace_scm_send_log(__func__, "send fs fd failed", ret);
			return ret;
		}
		fdtype = SCM_FS_VFS;
	}

	int slot = __save_fd_trans_info(owner, fdtype, sock, fsfd);
	if (slot < 0) {
		if (fdtype == SCM_FS_NET) {
			ux_socket_put(sock);
		} else {
			__vfs_put_send_fd(fdp->src_cnode_idx, fsfd);
		}
		ret = slot;
		uev_trace_scm_send_log(__func__, "save trans info failed", ret);
		return ret;
	}

	/* scm_cookie construction */
	fdp->fds[fdp->count] = slot;
	fdp->type[fdp->count] = fdtype;
	fdp->file_type[fdp->count] = 0; /* 0 is NORMAL_FD */
	NOFAIL(strcpy_s(fdp->fssrv_name[fdp->count], FSSRV_PATHNAME_LEN, "fsmgr"));
	fdp->count++;

	return ret;
}
#endif /* CONFIG_NETFS_FUSION */

static int unix_scm_copy_fd(struct cmsghdr *cmsg, ux_scm_fd_t *fdp)
{
	int *fds = (int *)CMSG_DATA(cmsg);
	int i, num, ret = E_HM_OK;
	uint32_t sender_cidx = osal_hm_actv_src_cnode();

	num = (int)((cmsg->cmsg_len - sizeof(struct cmsghdr)) / sizeof(int));
	if (num <= 0) {
		return E_HM_OK;
	}

	uev_trace_scm_send_fd(cmsg, fdp);
	if (fdp->count >= SCM_MAX_FD || (unsigned)num > SCM_MAX_FD - fdp->count) {
		net_warn(HMNET_UNIX, "too much fds, count=%d, num=%d\n", fdp->count, num);
		return E_HM_INVAL;
	}

	ux_sock_owner_t *owner = ux_sock_owner_get(sender_cidx, 0);
	BUG_ON(owner == NULL);
	fdp->send_owner = owner;
	fdp->src_cnode_idx = sender_cidx;
	for (i = 0; i < num; i++) {
		ret = unix_scm_send_fd(owner, fdp, fds[i]);
		if (ret < 0) {
			break;
		}
	}

	return ret;
}

#ifndef CMSG_OK
#define CMSG_OK(mhdr, cmsg) \
	((cmsg)->cmsg_len >= (socklen_t)sizeof(struct cmsghdr) && \
	 (cmsg)->cmsg_len <= (socklen_t)((mhdr)->msg_controllen - \
	 (unsigned int)((char *)(cmsg) - (char *)(mhdr)->msg_control)))
#endif
static int __unix_scm_send(struct msghdr *msg, ux_scm_cookie_t *scm)
{
	int err;
	struct cmsghdr *cmsg;

	for (cmsg = CMSG_FIRSTHDR(msg); cmsg != NULL; cmsg = CMSG_NXTHDR(msg, cmsg)) {
		err = E_HM_INVAL;
		if (!CMSG_OK(msg, cmsg)) {
			goto error;
		}

		uev_trace_scm_send(cmsg);
		if (cmsg->cmsg_level != SOL_SOCKET) {
			continue;
		}

		switch (cmsg->cmsg_type) {
		case SCM_RIGHTS:
			err = unix_scm_copy_fd(cmsg, &scm->fds);
			if (err != E_HM_OK) {
				goto error;
			}
			break;
		case SCM_CREDENTIALS:
			err = unix_scm_copy_cred(cmsg, &scm->creds);
			if (err != E_HM_OK) {
				goto error;
			}
			break;
		default:
			/* unknown cmsg_type, return EINVAL */
			goto error;
		}
	}

	return E_HM_OK;
error:
	unix_scm_destroy(scm);
	return err;
}

#define INVALID_UID ((uid_t)-1)
#define INVALID_GID ((gid_t)-1)
int unix_scm_send(struct msghdr *msg, ux_scm_cookie_t *scm)
{
	scm->creds.uid = INVALID_UID;
	scm->creds.gid = INVALID_GID;
	scm->creds.passcd = 0;

	if (msg->msg_controllen == 0) {
		return E_HM_OK;
	}

	return __unix_scm_send(msg, scm);
}

static int __vfs_recv_fd(ux_sock_trans_info_t *trans_info, int fd)
{
	int ret;
	int file_type, remote_fd;
	struct server_info *si = NULL;
	vfs_transfer_fd_args_t vfs_recv_fd_args = {0};

	/* don't support fs multi-sever */
	ret = get_server_info("fsmgr", &si);
	if (ret != E_HM_OK) {
		net_warn(HMNET_UNIX, "failed to get fsmgr server info, err=%d\n", ret);
		return ret;
	}

	vfs_recv_fd_args.sfiles_idx = trans_info->vfsfd_idx;
	vfs_recv_fd_args.send_cidx = trans_info->send_cidx;
	vfs_recv_fd_args.recv_fd = fd;
	vfs_recv_fd_args.recv_cidx = osal_hm_actv_src_cnode();
	ret = vfs_recv_fd(si, &vfs_recv_fd_args, &file_type, &remote_fd);

	put_server_info(si);
	return ret;
}

static int __scm_recv_fd(ux_sock_owner_t *send_owner, ux_scm_fd_t *fdp,
			 unsigned i, int fd)
{
	int ret = E_HM_OK;
	int slot;
	ux_sock_trans_info_t trans_info;
	unsigned int cnode = osal_hm_actv_src_cnode();
	ux_sock_owner_t *recv_owner = NULL;

	slot = fdp->fds[i];
	/* fetch xfer fd info */
	raw_mutex_lock(&send_owner->sslock);
	if (send_owner->ssocks == NULL || slot >= send_owner->ssocks_count) {
		net_warn(HMNET_UNIX, "send_owner invalid, slot: %d, send_owner->ssocks_count: %d\n",
				      slot, send_owner->ssocks_count);
		raw_mutex_unlock(&send_owner->sslock);
		return E_HM_INVAL;
	}

	trans_info = send_owner->ssocks[slot];
	/* reset to unused, SCM_FS_INVALID is 0 */
	NOFAIL(memset_s(&send_owner->ssocks[slot],
			sizeof(trans_info), 0, sizeof(trans_info)));
	raw_mutex_unlock(&send_owner->sslock);
	if (trans_info.fstype == SCM_FS_INVALID || trans_info.fstype != fdp->type[i]) {
		net_warn(HMNET_UNIX, "fstype mismatch, i=%u, slot=%d\n", i, slot);
		return E_HM_INVAL;
	}

	recv_owner = ux_sock_owner_get(cnode, 1);
	if (recv_owner == NULL) {
		return E_HM_NOMEM;
	}

#ifndef CONFIG_NETFS_FUSION
	if (fdp->type[i] == SCM_FS_NET) {
		ret = ux_socket_attach(recv_owner, trans_info.sock, fd);
		if (ret == E_HM_OK) {
			/* put the reference got in sendmsg */
			ux_socket_put(trans_info.sock);
		}
	} else {
		ret = __vfs_recv_fd(&trans_info, fd);
	}
#else
	if (fdp->type[i] == SCM_FS_NET) {
		ret = hm_net_sys_socket_attach(trans_info.sock, cnode, fd);
		if (ret == E_HM_OK) {
			hm_net_sys_socket_put(trans_info.sock);
		}
	} else if (fdp->type[i] == SCM_FS_SOCKFS) {
		ret = ux_socket_attach(recv_owner, trans_info.sock, fd);
		if (ret == E_HM_OK) {
			/* put the reference got in sendmsg */
			ux_socket_put(trans_info.sock);
		}
	} else {
		ret = __vfs_recv_fd(&trans_info, fd);
	}
#endif /* CONFIG_NETFS_FUSION */

	ux_sock_owner_put(recv_owner);

	return ret;
}

static int unix_scm_recv_fd(ux_sock_owner_t *send_owner, ux_sock_owner_t *recv_owner,
			    ux_scm_fd_t *fdp, unsigned i, int *ufd, bool cloexec)
{
	int fd, ret;
	unsigned int flags = cloexec ? SOCK_CLOEXEC : 0;

	BUG_ON(i >= fdp->count || i >= SCM_MAX_FD);

	if (fdp->fds[i] == -1 || fdp->type[i] == SCM_FS_INVALID) {
		return E_HM_INVAL;
	}

	if (fdp->type[i] == SCM_FS_SOCKFS) {
		fd = ux_fdtable_alloc_fd(recv_owner->fdtable_rref, flags);
	} else if (fdp->type[i] == SCM_FS_NET) {
		fd = ux_fdtable_alloc_net_fd(recv_owner->fdtable_rref, flags);
	} else {
		fd = ux_fdtable_alloc_fs_fd(recv_owner->fdtable_rref, flags);
	}

	if (fd < 0) {
		return fd;
	}

	if ((ret = osal_copy_to_user(ufd, &fd, sizeof(fd))) != E_HM_OK) {
		goto free_fd;
	}

	ret = __scm_recv_fd(send_owner, fdp, i, fd);
free_fd:
	if (ret != E_HM_OK) {
		/* free fd on error */
		ux_fdtable_free_fd(recv_owner->fdtable_rref, (unsigned int)fd);
	}

	return ret;
}

static unsigned int unix_scm_max_fds_compat(struct msghdr *msg)
{
	if (msg->msg_controllen <= sizeof(struct cmsghdr32)) {
		return 0;
	}

	return (msg->msg_controllen - sizeof(struct cmsghdr32)) / sizeof(int);
}

static void unix_scm_detach_fd_compat(struct msghdr *msg, ux_scm_fd_t *fdp, int flags)
{
	int err;
	unsigned int fdmax, i;
	struct cmsghdr32 cmhdr;
	bool cloexec = ((unsigned int)flags & MSG_CMSG_CLOEXEC) != 0;
	struct cmsghdr32 *cm = (struct cmsghdr32 *)msg->msg_control;
	int *cmdata = (int *)CMSG32_DATA(cm);
	ux_sock_owner_t *send_owner, *recv_owner;

	send_owner = fdp->send_owner;
	if (send_owner == NULL) {
		net_warn(HMNET_UNIX, "sender not found\n");
		return;
	}

	fdp->send_owner = NULL;

	recv_owner = ux_sock_owner_get(osal_hm_actv_src_cnode(), 0);
	BUG_ON(recv_owner == NULL);

	fdmax = unix_scm_max_fds_compat(msg);
	if (fdmax > fdp->count) {
		fdmax = fdp->count;
	}

	uev_trace_scm_recv_log(__func__, "fdmax is", (int)fdmax);
	/* loop each fdp[i], alloc fd and fill in cmsg data */
	for (i = 0; i < fdmax; i++) {
		err = unix_scm_recv_fd(send_owner, recv_owner, fdp, i, &cmdata[i], cloexec);
		if (err != E_HM_OK) {
			uev_trace_scm_recv_log(__func__, "unix_scm_recv_fd compat failed", err);
			break;
		}
		/* mark as used, no need to care in unix_scm_destroy */
		fdp->fds[i] = -1;
	}

	/* fill in cmsg hdr */
	if (i > 0) {
		cmhdr.cmsg_level = SOL_SOCKET;
		cmhdr.cmsg_type = SCM_RIGHTS;
		cmhdr.cmsg_len = (unsigned int)CMSG32_LEN(sizeof(int) * i);
		err = osal_copy_to_user(cm, &cmhdr, sizeof(cmhdr));
		if (err == E_HM_OK) {
			unsigned int cmlen = (unsigned int)CMSG32_SPACE(i * sizeof(int));
			if (cmlen > msg->msg_controllen) {
				cmlen = msg->msg_controllen;
			}
			msg->msg_controllen -= cmlen;
			msg->msg_control = (void *)((uintptr_t)msg->msg_control + cmlen);
		}
	}

	uev_trace_scm_recv_log(__func__, "compat received fd count", (int)i);
	if (i < fdp->count || (fdp->count != 0 && fdmax == 0)) {
		uev_trace_scm_recv_log(__func__, "not enough space to store fd", (int)i);
		msg->msg_flags = (int)(((unsigned int)msg->msg_flags) | MSG_CTRUNC);
	}

	ux_sock_owner_put(send_owner);
	ux_sock_owner_put(recv_owner);
}

static unsigned int unix_scm_max_fds(struct msghdr *msg)
{
	if (msg->msg_controllen <= sizeof(struct cmsghdr)) {
		return 0;
	}

	return (msg->msg_controllen - sizeof(struct cmsghdr)) / sizeof(int);
}

/* note: modifications need to sync with the _compat version */
static void unix_scm_detach_fd(struct msghdr *msg, ux_scm_fd_t *fdp, int flags)
{
	int err;
	unsigned int fdmax, i;
	struct cmsghdr cmhdr = { 0 };
	bool cloexec = ((unsigned int)flags & MSG_CMSG_CLOEXEC) != 0;
	struct cmsghdr *cm = (struct cmsghdr *)msg->msg_control;
	int *cmdata = (int *)CMSG_DATA(cm);
	ux_sock_owner_t *send_owner, *recv_owner;

	send_owner = fdp->send_owner;
	if (send_owner == NULL) {
		net_warn(HMNET_UNIX, "sender not found\n");
		return;
	}

	fdp->send_owner = NULL;

	recv_owner = ux_sock_owner_get(osal_hm_actv_src_cnode(), 0);
	BUG_ON(recv_owner == NULL);

	fdmax = unix_scm_max_fds(msg);
	if (fdmax > fdp->count) {
		fdmax = fdp->count;
	}

	uev_trace_scm_recv_log(__func__, "fdmax is", (int)fdmax);
	/* loop each fdp[i], alloc fd and fill in cmsg data */
	for (i = 0; i < fdmax; i++) {
		err = unix_scm_recv_fd(send_owner, recv_owner, fdp, i, &cmdata[i], cloexec);
		if (err != E_HM_OK) {
			uev_trace_scm_recv_log(__func__, "unix_scm_recv_fd failed", err);
			break;
		}
		/* mark as used, no need to care in unix_scm_destroy */
		fdp->fds[i] = -1;
	}

	/* fill in cmsg hdr */
	if (i > 0) {
		cmhdr.cmsg_level = SOL_SOCKET;
		cmhdr.cmsg_type = SCM_RIGHTS;
		cmhdr.cmsg_len = (socklen_t)CMSG_LEN(sizeof(int) * i);
		err = osal_copy_to_user(cm, &cmhdr, sizeof(cmhdr));
		if (err == E_HM_OK) {
			unsigned int cmlen = (unsigned int)CMSG_SPACE(i * sizeof(int));
			if (cmlen > msg->msg_controllen) {
				cmlen = msg->msg_controllen;
			}
			msg->msg_control = (void *)((uintptr_t)msg->msg_control + cmlen);
			msg->msg_controllen -= cmlen;
		}
	}

	uev_trace_scm_recv_log(__func__, "received fd count", (int)i);
	if (i < fdp->count || (fdp->count != 0 && fdmax == 0)) {
		uev_trace_scm_recv_log(__func__, "not enough space to store fd", (int)i);
		msg->msg_flags = (int)(((unsigned int)msg->msg_flags) | MSG_CTRUNC);
	}

	ux_sock_owner_put(send_owner);
	ux_sock_owner_put(recv_owner);
}

static void unix_scm_detach_cred(struct msghdr *msg, ux_scm_creds_t *scm_creds)
{
	int rc;

	struct ucred ucreds = {
		.pid = scm_creds->pid,
		.uid = scm_creds->uid,
		.gid = scm_creds->gid,
	};

	uev_trace_scm_detach_cred(scm_creds);

	rc = ucopy_cmsg_write(msg, SOL_SOCKET, SCM_CREDENTIALS, sizeof(ucreds), &ucreds);
	if (rc != E_HM_OK) {
		uev_trace_scm_recv_log(__func__, "failed to write cmsg(SCM_CREDENTIALS)", rc);
	}
}

void unix_scm_recv(struct msghdr *msg, ux_scm_cookie_t *scm, int flags)
{
	if (msg->msg_control == NULL || msg->msg_controllen == 0) {
		if (scm->fds.count != 0 || (scm->creds.passcd == 1)) {
			msg->msg_flags = (int)((unsigned int)msg->msg_flags | MSG_CTRUNC);
			uev_trace_scm_recv_log(__func__,
					"no space, passcd is", (int)scm->creds.passcd);
		}
		return;
	}

	if (scm->creds.passcd == 1) {
		unix_scm_detach_cred(msg, &scm->creds);
	}

	if (scm->fds.count == 0) {
		return;
	}

	uev_trace_scm_detach_fd(msg, scm, flags);
	if (ux_is_compat_client()) {
		unix_scm_detach_fd_compat(msg, &scm->fds, flags);
	} else {
		unix_scm_detach_fd(msg, &scm->fds, flags);
	}
}

void unix_scm_fds_destory(ux_scm_fd_t *fdp)
{
	ux_sock_owner_t *owner;
	ux_sock_trans_info_t trans_info;

	if (fdp->count == 0) {
		return;
	}

	owner = fdp->send_owner;
	if (owner == NULL) {
		/* sock owner has been destroyed, no need to concern */
		return;
	}

	fdp->send_owner = NULL;

	if (owner->ssocks == NULL) {
		ux_sock_owner_put(owner);
		return;
	}

	for (int i = 0; i < fdp->count; i++) {
		int slot = fdp->fds[i];
		if (slot == -1) {
			/* this slot has been consumed, skip */
			continue;
		}

		raw_mutex_lock(&owner->sslock);
		trans_info = owner->ssocks[slot];
		/* reset to unused, SCM_FS_INVALID is 0 */
		NOFAIL(memset_s(&owner->ssocks[slot],
				sizeof(trans_info), 0, sizeof(trans_info)));
		raw_mutex_unlock(&owner->sslock);

		if (trans_info.fstype == SCM_FS_NET) {
			hm_net_sys_socket_put((ux_socket_t *)trans_info.sock);
		}
		if (trans_info.fstype == SCM_FS_VFS) {
			__vfs_put_send_fd(trans_info.send_cidx, trans_info.vfsfd_idx);
		}
		if (trans_info.fstype == SCM_FS_SOCKFS) {
			ux_socket_put((ux_socket_t *)trans_info.sock);
		}
	}
	ux_sock_owner_put(owner);
}

void unix_scm_destroy(ux_scm_cookie_t *scm)
{
	unix_scm_fds_destory(&scm->fds);
}
#endif
