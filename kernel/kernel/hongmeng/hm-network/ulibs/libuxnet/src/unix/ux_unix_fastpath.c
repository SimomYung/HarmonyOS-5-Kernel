/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Implementation of unix fastpath
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 10 09:52:38 2024
 */

#include <libhmsync/raw_mutex.h>
#include "ucopy.h"
#include "osal.h"
#include "ux_socket.h"
#include "ux_netlog.h"
#include "ux_utils.h"
#include "ux_ringbuffer.h"
#include "ux_unix_util.h"
#include "ux_unix_scm.h"
#include "ux_unix_fastpath.h"

struct unix_fastpath_entry {
	enum unix_fastpath_type type;
	char path[USK_PATH_LOCAL_MAX];
	int (*set_client_ops)(struct ux_unix_sock *usk);
	void (*set_server_ops)(struct ux_unix_sock *usk, enum unix_fastpath_type type);
};

struct nto1_msghdr {
	int len;
	ux_scm_creds_t scm;
};

#define FASTIOV_CNT 8

static inline int __msglen_of_iovs(struct iovec *iov, unsigned int iovcnt)
{
	int totlen = 0;

	for (unsigned int i = 0; i < iovcnt; i++) {
		size_t len = (size_t)iov[i].iov_len;
		if (len > (size_t)(MAX_RW_COUNT - totlen)) {
			return E_HM_INVAL;
		}
		totlen += (int)len;
	}

	return totlen;
}

static int __construct_scm(ux_scm_creds_t *scm)
{
	scm->pid = osal_hm_root_ns_pid();
	scm->uid = 0;
	scm->gid = 0;
	scm->passcd = 1;

	return E_HM_OK;
}

static void __scm_detach_cred(struct msghdr *msg, ux_scm_creds_t *scm_creds)
{
	struct ucred ucreds = {
		.pid = scm_creds->pid,
		.uid = scm_creds->uid,
		.gid = scm_creds->gid,
	};

	(void)ucopy_cmsg_write(msg, SOL_SOCKET, SCM_CREDENTIALS, sizeof(ucreds), &ucreds);
}

static void __scm_recv(struct msghdr *msg, ux_scm_creds_t *scm)
{
	/*
	 * The function of transferring scm to fd is not supported.
	 */
	if (msg->msg_control == NULL || msg->msg_controllen == 0) {
		if (scm->passcd == 1) {
			msg->msg_flags = msg->msg_flags | MSG_CTRUNC;
		}
		return;
	}

	if (scm->passcd == 1) {
		__scm_detach_cred(msg, scm);
	}
}

static int __nto1_memcpy(void *dst, const void *src, unsigned long n)
{
	int ret = memcpy_s(dst, n, src, n);
	return ret != E_HM_OK ? E_HM_POSIX_FAULT : ret;
}

static int __read_msghdr_from_rb(struct ringbuffer_s *rb, struct nto1_msghdr *hdr)
{
	/* write_buffer ensures that the data in the ring buffer is complete, including (nto1_msghdr + data). */
	return ux_rb_read_nolock(rb, sizeof(struct nto1_msghdr), hdr, sizeof(struct nto1_msghdr), __nto1_memcpy);
}

static int __read_iovs_from_rb(struct ringbuffer_s *rb, int len, struct iovec *iov, unsigned int iovcnt)
{
	int ret;
	int copied = 0;
	unsigned int i;
	for (i = 0; i < iovcnt; i++) {
		ret = ux_rb_read_nolock(rb, len - copied, iov[i].iov_base,
				       (int)(unsigned int)iov[i].iov_len, osal_write_xact_vm);
		if (ret < 0) {
			return ret;
		}
		copied += ret;
	}

	if (len > copied) {
		/* Packets are truncated and the remaining packets are discarded in this scenario. */
		ux_rb_discard_nolock(rb, len - copied);
	}

	return copied;
}

static int __read_buf_from_rb(struct ringbuffer_s *rb, int len, void *ubuf, int ulen)
{
	int copied;

	copied = ux_rb_read_nolock(rb, len, ubuf, ulen, osal_write_xact_vm);
	if (len > copied) {
		/* Packets are truncated and the remaining packets are discarded in this scenario. */
		ux_rb_discard_nolock(rb, len - copied);
	}

	return copied;
}

static int __write_to_rb(struct ringbuffer_s *rb, struct nto1_msghdr *hdr, struct iovec *iov, unsigned int iovcnt,
			 int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	int ret;

	ux_rb_backup_nolock(rb);
	/* After passing ux_rb_is_full verification, the rb can write a complete packet, including (hdr + data). */
	ret = ux_rb_write_nolock(rb, (void *)hdr, sizeof(struct nto1_msghdr), __nto1_memcpy);
	if (ret != E_HM_OK) {
		ux_rb_rollback_nolock(rb);
		return ret;
	}

	for (unsigned int i = 0; i < iovcnt; i++) {
		ret = ux_rb_write_nolock(rb, (void *)iov[i].iov_base,
					(unsigned int)iov[i].iov_len, copy_fun);
		if (ret != E_HM_OK) {
			ux_rb_rollback_nolock(rb);
			break;
		}
	}

	return ret;
}

static ssize_t __do_writev(ux_unix_sock_t *peer, struct nto1_msghdr *hdr, struct iovec *iov, int iovcnt,
			   int (*copy_fun)(void *dst, const void *src, unsigned long n))
{
	ssize_t ret = E_HM_OK;

	ux_unix_sock_lock(peer);

	if (ux_check_sock_flags(&peer->sk, SOCK_DEAD)) {
		ret = E_HM_CONNREFUSED;
	}

	if (ret == E_HM_OK) {
		if (peer->rbuffer == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		/* A complete packet (include hdr and data) */
		if (ux_rb_is_full(peer->rbuffer, hdr->len + sizeof(struct nto1_msghdr))) {
			ret = E_HM_AGAIN;
		}
	}

	if (ret == E_HM_OK) {
		ret = __write_to_rb(peer->rbuffer, hdr, iov, iovcnt, copy_fun);
	}

	ux_unix_sock_unlock(peer);

	if (ret == E_HM_OK) {
		ux_wq_wakeup_all(&peer->recvwq);
	}

	return ret;
}

static ssize_t __do_writev_fast(ux_unix_sock_t *peer, struct nto1_msghdr *hdr, struct iovec *iov, int iovcnt)
{
	return __do_writev(peer, hdr, iov, iovcnt, osal_read_xact_vm_nopf);
}

static ssize_t __do_writev_slow(ux_unix_sock_t *peer, struct nto1_msghdr *hdr, struct iovec *iov, int iovcnt)
{
	struct iovec tmp_iov;
	ssize_t ret = E_HM_OK;
	char *ptr;

	/* hdr->len has been verified in the writev_fast process. */
	tmp_iov.iov_len = hdr->len;
	tmp_iov.iov_base = osal_calloc(1, hdr->len);
	if (tmp_iov.iov_base == NULL) {
		return E_HM_NOMEM;
	}

	ptr = tmp_iov.iov_base;
	for (int i = 0; i < iovcnt; i++) {
		ret = osal_copy_from_user(ptr, iov[i].iov_base, iov[i].iov_len);
		if (ret != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
			break;
		}
		ptr += iov[i].iov_len;
	}

	if (ret == E_HM_OK) {
		ret = __do_writev(peer, hdr, &tmp_iov, 1, __nto1_memcpy);
	}

	osal_free(tmp_iov.iov_base);
	return ret;
}

static ssize_t ux_unix_nto1_writev(ux_socket_t *sock, const struct iovec *uiovs, int iovcnt, int flags)
{
	UNUSED(flags);

	struct ux_unix_sock *usk = unix_sock_from_socket(sock);
	struct iovec iov[FASTIOV_CNT] = {0};
	struct nto1_msghdr hdr = {0};
	ux_unix_sock_t *peer;
	ssize_t ret = E_HM_OK;

	if (usk == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (iovcnt > FASTIOV_CNT || iovcnt < 0) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		if (ucopy_iovec(uiovs, iovcnt, FASTIOV_CNT, iov) == NULL) {
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		ret = __construct_scm(&hdr.scm);
	}

	if (ret == E_HM_OK) {
		ret = unix_get_pusk(usk, NULL, &peer);
	}

	if (ret == E_HM_OK) {
		hdr.len = __msglen_of_iovs(iov, iovcnt);
		if (hdr.len > 0) {
			ret = __do_writev_fast(peer, &hdr, iov, iovcnt);
			if (ret == E_HM_POSIX_FAULT) {
				ret = __do_writev_slow(peer, &hdr, iov, iovcnt);
			}
		} else {
			net_info(HMNET_UNIX, "unix nto1 writev msg len is out of range\n");
			ret = E_HM_INVAL;
		}
		ux_sock_put(&peer->sk);
	}

	return ret == E_HM_OK ? hdr.len : ret;
}

static ssize_t __do_recvmsg(struct ux_unix_sock *usk, struct msghdr *msg, struct iovec *iov)
{
	struct ringbuffer_s *rbuffer = (struct ringbuffer_s *)usk->rbuffer;
	struct ux_wq_entry waiter = {0};
	struct nto1_msghdr hdr = {0};
	ssize_t ret = E_HM_OK;

	ux_unix_sock_lock(usk);
	while (!ux_is_rd_shutdown(usk) && ux_rb_is_empty(rbuffer)) {
		ux_wq_prepare_wait(&usk->recvwq, &waiter);
		ux_unix_sock_unlock(usk);
		ret = ux_wq_do_wait(&waiter, usk->sk.rcvtimeo);
		ux_wq_finish_wait(&usk->recvwq, &waiter);
		ux_unix_sock_lock(usk);

		if (ret != E_HM_OK) {
			net_error(HMNET_UNIX, "unix nto1 wq wait failed, errno = %s\n", hmstrerror(ret));
			break;
		}
	}

	if (ret == E_HM_OK && !ux_is_rd_shutdown(usk)) {
		ux_rb_backup_nolock(rbuffer);
		ret = __read_msghdr_from_rb(rbuffer, &hdr);
		if (ret > 0) {
			ret = __read_iovs_from_rb(rbuffer, hdr.len, iov, msg->msg_iovlen);
		}
		if (ret < 0) {
			ux_rb_rollback_nolock(rbuffer);
		}
	}

	ux_unix_sock_unlock(usk);

	if (ret > 0) {
		__scm_recv(msg, &hdr.scm);
	}

	return ret;
}

static ssize_t ux_unix_nto1_recvmsg(ux_socket_t *sock, struct msghdr *umsg, int flags)
{
	UNUSED(flags);

	struct ux_unix_sock *usk = unix_sock_from_socket(sock);
	struct iovec iov[FASTIOV_CNT] = {0};
	struct msghdr msg = {0};
	ssize_t ret = E_HM_OK;

	if (usk == NULL || usk->rbuffer == NULL) {
		net_error(HMNET_UNIX, "unix nto1 usk is null\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (osal_read_xact_vm(&msg, umsg, sizeof(struct msghdr)) < 0) {
			net_error(HMNET_UNIX, "unix nto1 read umsg failed\n");
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		if (msg.msg_iovlen < 0 || msg.msg_iovlen > FASTIOV_CNT) {
			net_error(HMNET_UNIX, "unix nto1 msg_iovlen is too big or too small\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		if (osal_read_xact_vm(iov, msg.msg_iov, sizeof(struct iovec) * msg.msg_iovlen) < 0) {
			net_error(HMNET_UNIX, "unix nto1 read umsg_iov failed\n");
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		ret = __do_recvmsg(usk, &msg, iov);
	}

	return ret;
}

static ssize_t __do_recvfrom(struct ux_unix_sock *usk, void *buf, size_t len, int flags)
{
	struct ringbuffer_s *rbuffer = (struct ringbuffer_s *)usk->rbuffer;
	struct ux_wq_entry waiter = {0};
	struct nto1_msghdr hdr = {0};
	ssize_t ret = E_HM_OK;

	ux_unix_sock_lock(usk);
	while (!ux_is_rd_shutdown(usk) && ux_rb_is_empty(rbuffer)) {
		ux_wq_prepare_wait(&usk->recvwq, &waiter);
		ux_unix_sock_unlock(usk);
		ret = ux_wq_do_wait(&waiter, usk->sk.rcvtimeo);
		ux_wq_finish_wait(&usk->recvwq, &waiter);
		ux_unix_sock_lock(usk);

		if (ret != E_HM_OK) {
			break;
		}
	}

	if (ret == E_HM_OK && !ux_is_rd_shutdown(usk)) {
		ux_rb_backup_nolock(rbuffer);
		ret = __read_msghdr_from_rb(rbuffer, &hdr);
		if (ret > 0) {
			ret = __read_buf_from_rb(rbuffer, hdr.len, buf, len);
		}
		if (ret < 0 || flags == MSG_PEEK) {
			ux_rb_rollback_nolock(rbuffer);
		}
	}
	ux_unix_sock_unlock(usk);

	return ret;
}

static ssize_t ux_unix_nto1_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
				     struct sockaddr *uaddr, socklen_t *uaddrlen)
{
	UNUSED(uaddr, uaddrlen);

	struct ux_unix_sock *usk = unix_sock_from_socket(sock);
	if (usk == NULL || usk->rbuffer == NULL) {
		return E_HM_INVAL;
	}

	return __do_recvfrom(usk, buf, len, flags);
}

/* In the current scenario, it is recommended that 500 KB memory be reserved. */
#define RB_SIZE (125 * PAGE_SIZE)

static ux_proto_ops_t usk_shm_server_ops;
static ux_proto_ops_t usk_shm_client_ops;

static int ux_unix_shm_set_client_ops(struct ux_unix_sock *usk)
{
	usk->sk.socket->ops = &usk_shm_client_ops;
	return E_HM_OK;
}

static void ux_unix_shm_set_server_ops(struct ux_unix_sock *usk, enum unix_fastpath_type type)
{
	struct ringbuffer_s *rbuffer = NULL;

	usk->rbuffer = calloc(1, sizeof(struct ringbuffer_s));
	if (usk->rbuffer == NULL) {
		net_info(HMNET_UNIX, "unix shm malloc rbuffer failed\n");
		return;
	}

	rbuffer = (struct ringbuffer_s *)usk->rbuffer;
	rbuffer->regions = calloc(1, RB_SIZE);
	if (rbuffer->regions == NULL) {
		net_info(HMNET_UNIX, "unix shm malloc regions failed\n");
		free(usk->rbuffer);
		usk->rbuffer = NULL;
		return;
	}

	ux_rb_init(usk->rbuffer, RB_SIZE);
	usk->sk.socket->ops = &usk_shm_server_ops;
	usk->fp_type = type;
}

void ux_unix_fastpath_init(ux_proto_ops_t *usk_ops)
{
	NOFAIL(memcpy_s(&usk_shm_server_ops, sizeof(ux_proto_ops_t), usk_ops, sizeof(ux_proto_ops_t)));
	usk_shm_server_ops.recvmsg = ux_unix_nto1_recvmsg;
	usk_shm_server_ops.recvfrom = ux_unix_nto1_recvfrom;
	NOFAIL(memcpy_s(&usk_shm_client_ops, sizeof(ux_proto_ops_t), usk_ops, sizeof(ux_proto_ops_t)));
	usk_shm_client_ops.writev = ux_unix_nto1_writev;
}

struct unix_fastpath_entry fp_array[FP_MAX] = {
	{FP_SHM_TYPE, HILOG_PATH, ux_unix_shm_set_client_ops, ux_unix_shm_set_server_ops},
};

void ux_unix_try_server_fastpath(struct ux_unix_sock *usk, char *path)
{
	for (int i = 0; i < FP_MAX; i++) {
		if (strcmp(path, fp_array[i].path) == 0) {
			fp_array[i].set_server_ops(usk, fp_array[i].type);
			return;
		}
	}
}

int ux_unix_try_client_fastpath(struct ux_unix_sock *usk, enum unix_fastpath_type fp_type)
{
	int ret = E_HM_OK;

	if (fp_type > FP_INVAL && fp_type < FP_MAX) {
		ret = fp_array[fp_type].set_client_ops(usk);
	}

	return ret;
}

void ux_unix_fastpath_disable(struct ux_unix_sock *usk)
{
	struct ringbuffer_s *rbuffer = (struct ringbuffer_s *)usk->rbuffer;

	usk->fp_type = FP_INVAL;
	if (rbuffer != NULL) {
		free(rbuffer->regions);
		free(rbuffer);
		usk->rbuffer = NULL;
	}
}
