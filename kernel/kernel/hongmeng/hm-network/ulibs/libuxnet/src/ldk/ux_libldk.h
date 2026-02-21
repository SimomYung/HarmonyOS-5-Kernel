/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: LDK library interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 19:30:47 2022
 */

#ifndef __HM_NET_UX_LIBLDK_H
#define __HM_NET_UX_LIBLDK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <libhmsrv_io/type.h>

#include "ux_libldk_pub.h"

int libldk_domain_init(hmsrv_io_domain_ctx *domainctx, int family, rref_t ap_rref);
int libldk_socket(hmsrv_io_domain_ctx *domainctx, int type, int protocol,
			unsigned long ino, struct hmsrv_io_ctx *skctx);
int libldk_close(struct hmsrv_io_ctx *ctx);
int libldk_shutdown(struct hmsrv_io_ctx *ctx, int how);
int libldk_connect(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen);
int libldk_bind(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen);
int libldk_listen(struct hmsrv_io_ctx *ctx, int backlog);
int libldk_accept4(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen,
			int flags, unsigned long ino, struct hmsrv_io_ctx *new_ctx);
int libldk_ioctl(struct hmsrv_io_ctx *ctx, unsigned long request, uintptr_t arg);
int libldk_dev_ioctl(struct hmsrv_io_ctx *ctx, unsigned long request, uintptr_t arg);
int libldk_fcntl(struct hmsrv_io_ctx *ctx, int cmd, int arg);
int libldk_send(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags);
int libldk_sendto(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags,
		  const struct sockaddr *dest_addr, socklen_t addrlen);
int libldk_sendmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags);
int libldk_recvmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags, void *timeout);
int libldk_sendmsg(struct hmsrv_io_ctx *ctx, const struct msghdr *msg, int flags);
int libldk_recv(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags);
int libldk_recvfrom(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags,
		    struct sockaddr *src_addr, socklen_t *addrlen);
int libldk_recvmsg(struct hmsrv_io_ctx *ctx, struct msghdr *msg, int flags);
int libldk_readv(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt);
int libldk_writev(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt, int flags);
int libldk_getsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, void *optval, socklen_t *optlen);
int libldk_setsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, const void *optval, socklen_t optlen);
int libldk_getsockname(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen);
int libldk_getpeername(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen);
int libldk_fchown(struct hmsrv_io_ctx *ctx, unsigned int uid, unsigned int gid);
int libldk_fstat(struct hmsrv_io_ctx *ctx, uid_t *uid);
int libldk_mmap(struct hmsrv_io_ctx *ctx, const struct net_mmap_info *info, uintptr_t *rvaddr);
int libldk_bpf(struct hmsrv_io_ctx *ctx, int cmd, void *attr, unsigned int size);

#ifdef CONFIG_NET_NS
int libldk_namespace_init(void);
#else
static inline int libldk_namespace_init(void) { return 0; }
#endif

int ux_ldk_namespace_init(void);

#endif
