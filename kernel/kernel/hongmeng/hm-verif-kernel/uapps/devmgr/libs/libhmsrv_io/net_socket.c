/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: API implementations of net socket plugin
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 14:46:12 2022
 */
#include <errno.h>

#include <libsysif/devhost/api.h>
#include <libhmsrv_io/net_socket.h>

#define __net_socket_capcall(func, dh_ap_rref, ret_type, ...) 		\
({ 						\
	ret_type ____ret;				\
	if (dh_ap_rref == 0) { 		\
		____ret = E_HM_AFNOSUPPORT; 	\
	} else { 				\
		____ret = actvcapcall_drvcall_devhost_net_socket_##func(dh_ap_rref, __VA_ARGS__); \
	} 					\
	____ret; 				\
})

#define net_socket_capcall_long(func, dh_ap_rref, ...) 		\
	__net_socket_capcall(func, dh_ap_rref, long, __VA_ARGS__)

#define net_socket_capcall(func, dh_ap_rref, ...) 		\
	__net_socket_capcall(func, dh_ap_rref, int, __VA_ARGS__)

#define net_socket_fwdcapcall(func, dh_ap_rref, ...) 	\
({ 						\
	int ____ret;				\
	if (dh_ap_rref == 0) { 		\
		____ret = E_HM_AFNOSUPPORT; 	\
	} else { 				\
		____ret = actvxactcapcall_drvcall_devhost_net_socket_##func(true, true, dh_ap_rref, __VA_ARGS__); \
	} 					\
	____ret; 				\
})

int hmsrv_io_domain_init(hmsrv_io_domain_ctx *ctx, int family, rref_t ap_rref)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	ctx->domain = family;
	ctx->dh_ap_rref = ap_rref;
	ctx->dh_cnode_idx = sysif_actv_src_cnode_idx();

	return E_HM_OK;
}

int hmsrv_io_socket_socket(hmsrv_io_domain_ctx *domainctx, int type, int protocol,
				unsigned long ino, struct hmsrv_io_ctx *skctx)
{
	struct __actvret_drvcall_devhost_net_socket_socket socket_ret;
	int file_id = -1;

	if (domainctx == NULL || skctx == NULL) {
		return E_HM_INVAL;
	}

	file_id = net_socket_capcall(socket, domainctx->dh_ap_rref, domainctx->domain,
					type, protocol, ino, &socket_ret);
	if (file_id >= 0) {
		net_init_sk_ctx(skctx, domainctx->dh_ap_rref, domainctx->dh_cnode_idx, file_id,
				socket_ret.filp, socket_ret.kshm_key, socket_ret.kobj_offset);
	}

	return file_id;
}

int hmsrv_io_socket_connect(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(connect, ctx->dh_ap_rref, ctx->filp, addr, addrlen);
}

int hmsrv_io_socket_bind(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(bind, ctx->dh_ap_rref, ctx->filp, addr, addrlen);
}

int hmsrv_io_socket_listen(struct hmsrv_io_ctx *ctx, int backlog)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(listen, ctx->dh_ap_rref, ctx->filp, backlog);
}

int hmsrv_io_socket_accept(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen,
				int flags, unsigned long ino ,struct hmsrv_io_ctx *new_ctx)
{
	struct __actvret_drvcall_devhost_net_socket_accept accept_ret;
	int file_id = -1;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	file_id = net_socket_capcall(accept, ctx->dh_ap_rref, ctx->filp, addr, addrlen, flags, ino, &accept_ret);
	if (file_id >= 0) {
		new_ctx->dh_ap_rref = ctx->dh_ap_rref;
		new_ctx->dh_cnode_idx = ctx->dh_cnode_idx;
		new_ctx->file_id = file_id;
		new_ctx->fastpath = 0;
		new_ctx->filp = accept_ret.filp;
		new_ctx->kshm_key = accept_ret.kshm_key;
		new_ctx->kobj_offset = accept_ret.kobj_offset;
	}

	return file_id;
}

int hmsrv_io_socket_ioctl(struct hmsrv_io_ctx *ctx, int cmd, void *arg_buf)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(ioctl, ctx->dh_ap_rref, ctx->filp, cmd, arg_buf);
}

int hmsrv_io_socket_dev_ioctl(struct hmsrv_io_ctx *ctx, int cmd, void *arg_buf)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(dev_ioctl, ctx->dh_ap_rref, cmd, arg_buf);
}

int hmsrv_io_socket_fcntl(struct hmsrv_io_ctx *ctx, int cmd, int arg)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(fcntl, ctx->dh_ap_rref, ctx->filp, cmd, arg);
}

int hmsrv_io_socket_send(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(send, ctx->dh_ap_rref, ctx->filp, buf, len, flags);
}

int hmsrv_io_socket_recv(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(recv, ctx->dh_ap_rref, ctx->filp, buf, len, flags);
}

int hmsrv_io_socket_sendmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(sendmmsg, ctx->dh_ap_rref, ctx->filp, msgvec, vlen, flags);
}

int hmsrv_io_socket_recvmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(recvmmsg, ctx->dh_ap_rref, ctx->filp, msgvec, vlen, flags,timeout);
}

int hmsrv_io_socket_sendmsg(struct hmsrv_io_ctx *ctx, const struct msghdr *msg, int flags)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(sendmsg, ctx->dh_ap_rref, ctx->filp, msg, flags);
}

int hmsrv_io_socket_recvmsg(struct hmsrv_io_ctx *ctx, struct msghdr *msg, int flags)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(recvmsg, ctx->dh_ap_rref, ctx->filp, msg, flags);
}

int hmsrv_io_socket_sendto(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags,
			   const struct sockaddr *daddr, socklen_t addrlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(sendto, ctx->dh_ap_rref, ctx->filp, buf, len, flags, daddr, addrlen);
}

int hmsrv_io_socket_recvfrom(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags,
			     struct sockaddr *saddr, socklen_t *addrlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(recvfrom, ctx->dh_ap_rref, ctx->filp, buf, len, flags, saddr, addrlen);
}

int hmsrv_io_socket_readv(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(readv, ctx->dh_ap_rref, ctx->filp, iov, iovcnt);
}

int hmsrv_io_socket_writev(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt, int flags)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(writev, ctx->dh_ap_rref, ctx->filp, iov, iovcnt, flags);
}

int hmsrv_io_socket_getsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, void *optval, socklen_t *optlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(getsockopt, ctx->dh_ap_rref, ctx->filp, level, optname, optval, optlen);
}

int hmsrv_io_socket_setsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, const void *optval, socklen_t optlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(setsockopt, ctx->dh_ap_rref, ctx->filp, level, optname, optval, optlen);
}

int hmsrv_io_socket_getsockname(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(getsockname, ctx->dh_ap_rref, ctx->filp, addr, addrlen);
}

int hmsrv_io_socket_getpeername(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(getpeername, ctx->dh_ap_rref, ctx->filp, addr, addrlen);
}

int hmsrv_io_socket_close(struct hmsrv_io_ctx *ctx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(close, ctx->dh_ap_rref, ctx->file_id);
}

int hmsrv_io_socket_shutdown(struct hmsrv_io_ctx *ctx, int how)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(shutdown, ctx->dh_ap_rref, ctx->filp, how);
}

int hmsrv_io_socket_fchown(struct hmsrv_io_ctx *ctx, unsigned int uid, unsigned int gid)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(fchown, ctx->dh_ap_rref, ctx->filp, uid, gid);
}

int hmsrv_io_socket_fstat(struct hmsrv_io_ctx *ctx, uid_t *uid)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(fstat, ctx->dh_ap_rref, ctx->filp, uid);
}

int hmsrv_io_socket_alloc_new_ns(struct hmsrv_io_ctx *ctx, int nsid)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(alloc_new_ns, ctx->dh_ap_rref, nsid);
}

int hmsrv_io_socket_destroy_ns(struct hmsrv_io_ctx *ctx, int nsid)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_capcall(destroy_ns, ctx->dh_ap_rref, nsid);
}

int hmsrv_io_socket_mmap(struct hmsrv_io_ctx *ctx, const struct net_mmap_info *info, uintptr_t *rvaddr)
{
	int rc;
	struct __actvret_drvcall_devhost_fops_mmap mmap_ret = {0};

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	rc = actvxactcapcall_drvcall_devhost_fops_mmap(true, /* is_fwd */
							true, /* allow_refwd */
							ctx->dh_ap_rref,
							0,
							info->vaddr,
							ctx->file_id,
							info->aligned_len,
							info->prot,
							info->flags,
							(unsigned long)info->offset,
							false,   /* iomap prepare in process */
							&mmap_ret);
	if (rc == E_HM_OK) {
		*rvaddr = (uintptr_t)mmap_ret.rvaddr;
	}

	return rc;
}

int hmsrv_io_socket_bpf(struct hmsrv_io_ctx *ctx, int cmd, void *attr, unsigned int size)
{
	if (ctx == NULL || attr == NULL) {
		return E_HM_INVAL;
	}

	return net_socket_fwdcapcall(bpf, ctx->dh_ap_rref, cmd, attr, size);
}
