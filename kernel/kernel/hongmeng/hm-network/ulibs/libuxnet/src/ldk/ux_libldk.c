/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: LDK library implementation
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 19:10:23 2022
 */

#include <errno.h>
#include <stdint.h>

#include <libstrict/strict.h>
#include <hmkernel/errno.h>
#include <hmkernel/capability.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_io/net_socket.h>

#include "ux_netlog.h"
#include "ux_libldk.h"
#include "ux_libldk_pub.h"

#if defined(CONFIG_NET_FUSION) && !defined(CONFIG_NETFS_FUSION)

#include "net_socket_if.h"

static inline int net_socket_socket(hmsrv_io_domain_ctx *domainctx, int type, int protocol,
			     unsigned long ino, struct hmsrv_io_ctx *skctx)
{
	int file_id;
	net_socket_ret_t socket_ret;

	file_id = devhost_net_socket_directcall_socket(domainctx->dh_cnode_idx, domainctx->domain, type, protocol, ino,
						       &socket_ret);
	if (file_id > 0) {
		net_init_sk_ctx(skctx, domainctx->dh_ap_rref, domainctx->dh_cnode_idx, file_id,
				socket_ret.filp, socket_ret.kshm_key, socket_ret.kobj_offset);
	}

	return file_id;
}

static inline int net_socket_close(struct hmsrv_io_ctx *ctx)
{
	return devhost_net_socket_directcall_close(ctx->dh_cnode_idx, ctx->file_id);
}

#define DEFINE_NET_SOCKET_CALLx(x, func, ...)	\
static inline int net_socket_##func(struct hmsrv_io_ctx *ctx,	\
					 __scopedptr_map(x, __scopedptr_def, __VA_ARGS__))	\
{	\
	int ____ret;	\
	\
	SYSIF_DIRECTCALL_FWD_GUARD();	\
	if (ctx->dh_ap_rref == 0) {	\
		____ret = E_HM_AFNOSUPPORT;	\
	} else {	\
		____ret = devhost_net_socket_directcall_##func(ctx->filp, __scopedptr_map(x, __scopedptr_call, __VA_ARGS__));	\
	}	\
	\
	return ____ret;	\
}

#else
static inline int net_socket_socket(hmsrv_io_domain_ctx *domainctx, int type, int protocol,
				    unsigned long ino, struct hmsrv_io_ctx *skctx)
{
	return hmsrv_io_socket_socket(domainctx, type, protocol, ino, skctx);
}

static inline int net_socket_close(struct hmsrv_io_ctx *ctx)
{
	return hmsrv_io_socket_close(ctx);
}

#define DEFINE_NET_SOCKET_CALLx(x, func, ...)	\
static inline int net_socket_##func(struct hmsrv_io_ctx *ctx,	\
					 __scopedptr_map(x, __scopedptr_def, __VA_ARGS__))	\
{	\
	return hmsrv_io_socket_##func(ctx, __scopedptr_map(x, __scopedptr_call, __VA_ARGS__));	\
}

#endif /* CONFIG_NET_FUSION */

#define ____NET_ARG_NR(t1, a1, t2, a2, t3, a3, t4, a4, t5, a5,			\
			t6, a6, t7, a7, t8, a8, t9, a9, t10, a10, t11, a11, t12, a12,	\
			t13, a13, t14, a14, t15, a15, n, x, ...) n
#define __NET_ARG_NR(...)	\
	____NET_ARG_NR(__VA_ARGS__,						\
			  15, x, 14, x, 13, x, 12, x, 11, x, 10, x, 9, x,	\
			  8, x, 7, x, 6, x, 5, x, 4, x, 3, x, 2, x, 1, x, 0, x)

#define DEFINE_NET_SOCKET_CALL(func, ...)	\
	DEFINE_NET_SOCKET_CALLx(__NET_ARG_NR(__VA_ARGS__), func, __VA_ARGS__)

DEFINE_NET_SOCKET_CALL(send, const void *, buf, size_t, len, int, flags)
DEFINE_NET_SOCKET_CALL(recv, void *, buf, size_t, len, int, flags)
DEFINE_NET_SOCKET_CALL(sendto, const void *, buf, size_t, len, int, flags,
		       const struct sockaddr *, daddr, socklen_t, addrlen)
DEFINE_NET_SOCKET_CALL(recvfrom, void *, buf, size_t, len, int, flags,
		       struct sockaddr *, saddr, socklen_t *, addrlen)
DEFINE_NET_SOCKET_CALL(sendmsg, const struct msghdr *, msg, int, flags)
DEFINE_NET_SOCKET_CALL(recvmsg, struct msghdr *, msg, int, flags)
DEFINE_NET_SOCKET_CALL(getsockopt, int, level, int, optname, void *, optval, socklen_t *, optlen)
DEFINE_NET_SOCKET_CALL(setsockopt, int, level, int, optname, const void *, optval, socklen_t, optlen)

int libldk_domain_init(hmsrv_io_domain_ctx *domainctx, int family, rref_t ap_rref)
{
	return hmsrv_io_domain_init(domainctx, family, ap_rref);
}

int libldk_socket(hmsrv_io_domain_ctx *domainctx, int type, int protocol,
			unsigned long ino, struct hmsrv_io_ctx *skctx)
{
	int ret;

	ret = net_socket_socket(domainctx, type, protocol, ino, skctx);
#if !defined(CONFIG_NET_FUSION)
	if (ret >= 0 && skctx != NULL) {
		skctx->filp = (unsigned long long)skctx->file_id;
	}
#endif

	return ret;
}

int libldk_close(struct hmsrv_io_ctx *ctx)
{
	return net_socket_close(ctx);
}

int libldk_shutdown(struct hmsrv_io_ctx *ctx, int how)
{
	return hmsrv_io_socket_shutdown(ctx, how);
}

int libldk_connect(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen)
{
	return hmsrv_io_socket_connect(ctx, addr, addrlen);
}

int libldk_bind(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen)
{
	return hmsrv_io_socket_bind(ctx, addr, addrlen);
}

int libldk_listen(struct hmsrv_io_ctx *ctx, int backlog)
{
	return hmsrv_io_socket_listen(ctx, backlog);
}

int libldk_accept4(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen,
			int flags, unsigned long ino, struct hmsrv_io_ctx *new_ctx)
{
	int ret;

	ret = hmsrv_io_socket_accept(ctx, addr, addrlen, flags, ino, new_ctx);
#if !defined(CONFIG_NET_FUSION)
	if (new_ctx != NULL) {
		new_ctx->filp = (unsigned long long)new_ctx->file_id;
	}
#endif

	return ret;
}

int libldk_ioctl(struct hmsrv_io_ctx *ctx, unsigned long request, uintptr_t arg)
{
	return hmsrv_io_socket_ioctl(ctx, request, (void *)arg);
}

int libldk_dev_ioctl(struct hmsrv_io_ctx *ctx, unsigned long request, uintptr_t arg)
{
	return hmsrv_io_socket_dev_ioctl(ctx, request, (void *)arg);
}

int libldk_fcntl(struct hmsrv_io_ctx *ctx, int cmd, int arg)
{
	return hmsrv_io_socket_fcntl(ctx, cmd, arg);
}

int libldk_send(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags)
{
	return net_socket_send(ctx, buf, len, flags);
}

int libldk_sendto(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags,
		  const struct sockaddr *dest_addr, socklen_t addrlen)
{
	return net_socket_sendto(ctx, buf, len, flags, dest_addr, addrlen);
}

int libldk_sendmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags)
{
	return hmsrv_io_socket_sendmmsg(ctx, msgvec, vlen, flags);
}

int libldk_recvmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	return hmsrv_io_socket_recvmmsg(ctx, msgvec, vlen, flags, timeout);
}

int libldk_sendmsg(struct hmsrv_io_ctx *ctx, const struct msghdr *msg, int flags)
{
	return net_socket_sendmsg(ctx, msg, flags);
}

int libldk_recv(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags)
{
	return net_socket_recv(ctx, buf, len, flags);
}

int libldk_recvfrom(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags,
		    struct sockaddr *src_addr, socklen_t *addrlen)
{
	return net_socket_recvfrom(ctx, buf, len, flags, src_addr, addrlen);
}

int libldk_recvmsg(struct hmsrv_io_ctx *ctx, struct msghdr *msg, int flags)
{
	return net_socket_recvmsg(ctx, msg, flags);
}

int libldk_writev(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt, int flags)
{
	return hmsrv_io_socket_writev(ctx, iov, iovcnt, flags);
}

int libldk_readv(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt)
{
	return hmsrv_io_socket_readv(ctx, iov, iovcnt);
}

int libldk_getsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, void *optval, socklen_t *optlen)
{
	return net_socket_getsockopt(ctx, level, optname, optval, optlen);
}

int libldk_setsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, const void *optval, socklen_t optlen)
{
	return net_socket_setsockopt(ctx, level, optname, optval, optlen);
}

int libldk_getsockname(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen)
{
	return hmsrv_io_socket_getsockname(ctx, addr, addrlen);
}

int libldk_getpeername(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen)
{
	return hmsrv_io_socket_getpeername(ctx, addr, addrlen);
}

int libldk_fchown(struct hmsrv_io_ctx *ctx, unsigned int uid, unsigned int gid)
{
	return hmsrv_io_socket_fchown(ctx, uid, gid);
}

int libldk_fstat(struct hmsrv_io_ctx *ctx, uid_t *uid)
{
	return hmsrv_io_socket_fstat(ctx, uid);
}

int ux_ldk_namespace_init(void)
{
	return libldk_namespace_init();
}

int libldk_mmap(struct hmsrv_io_ctx *ctx, const struct net_mmap_info *info, uintptr_t *rvaddr)
{
	return hmsrv_io_socket_mmap(ctx, info, rvaddr);
}

int libldk_bpf(struct hmsrv_io_ctx *ctx, int cmd, void *attr, unsigned int size)
{
	return hmsrv_io_socket_bpf(ctx, cmd, attr, size);
}
