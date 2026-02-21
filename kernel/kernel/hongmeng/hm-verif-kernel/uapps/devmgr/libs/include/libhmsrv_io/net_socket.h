/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: API interfaces of net socket plugin
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 14:37:38 2022
 */
#ifndef ULIBS_LIBHMSRV_IO_NET_SOCKET_H
#define ULIBS_LIBHMSRV_IO_NET_SOCKET_H

#include <unistd.h>
#include <sys/socket.h>

#include <hmkernel/capability.h>
#include <libhmsrv_io/type.h>

struct net_mmap_info {
	uint32_t cnode_idx;
	uint32_t prot;
	uint32_t flags;
	int64_t offset;
	rref_t vs_rref;
	uintptr_t vaddr;
	size_t aligned_len;
};

static inline void net_init_sk_ctx(struct hmsrv_io_ctx *skctx, rref_t dh_ap_rref, unsigned int dh_cnode_idx,
	int file_id, unsigned long long filp, unsigned long long kshm_key, unsigned long kobj_offset)
{
	skctx->dh_ap_rref = dh_ap_rref;
	skctx->dh_cnode_idx = dh_cnode_idx;
	skctx->file_id = file_id;
	skctx->fastpath = 0;
	skctx->filp = filp;
	skctx->kshm_key = kshm_key;
	skctx->kobj_offset = kobj_offset;
}

int hmsrv_io_domain_init(hmsrv_io_domain_ctx *ctx, int family, rref_t ap_rref);
int hmsrv_io_socket_bpf(struct hmsrv_io_ctx *ctx, int cmd, void *attr, unsigned int size);
int hmsrv_io_socket_socket(hmsrv_io_domain_ctx *domainctx, int type, int protocol,
				unsigned long ino, struct hmsrv_io_ctx *skctx);
int hmsrv_io_socket_connect(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen);
int hmsrv_io_socket_bind(struct hmsrv_io_ctx *ctx, const struct sockaddr *addr, socklen_t addrlen);
int hmsrv_io_socket_listen(struct hmsrv_io_ctx *ctx, int backlog);
int hmsrv_io_socket_accept(struct hmsrv_io_ctx *ctx, struct sockaddr *addr,
				socklen_t *addrlen, int flags, unsigned long ino, struct hmsrv_io_ctx *new_ctx);
int hmsrv_io_socket_ioctl(struct hmsrv_io_ctx *ctx, int cmd, void *arg);
int hmsrv_io_socket_dev_ioctl(struct hmsrv_io_ctx *ctx, int cmd, void *arg);
int hmsrv_io_socket_fcntl(struct hmsrv_io_ctx *ctx, int cmd, int arg);
int hmsrv_io_socket_send(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags);
int hmsrv_io_socket_recv(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags);
int hmsrv_io_socket_sendmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags);
int hmsrv_io_socket_recvmmsg(struct hmsrv_io_ctx *ctx, const void *msgvec, size_t vlen, int flags, void *timeout);
int hmsrv_io_socket_sendmsg(struct hmsrv_io_ctx *ctx, const struct msghdr *msg, int flags);
int hmsrv_io_socket_recvmsg(struct hmsrv_io_ctx *ctx, struct msghdr *msg, int flags);
int hmsrv_io_socket_readv(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt);
int hmsrv_io_socket_writev(struct hmsrv_io_ctx *ctx, const struct iovec *iov, int iovcnt, int flags);
int hmsrv_io_socket_getsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, void *optval, socklen_t *optlen);
int hmsrv_io_socket_setsockopt(struct hmsrv_io_ctx *ctx, int level, int optname, const void *optval, socklen_t optlen);
int hmsrv_io_socket_getsockname(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen);
int hmsrv_io_socket_getpeername(struct hmsrv_io_ctx *ctx, struct sockaddr *addr, socklen_t *addrlen);
int hmsrv_io_socket_sendto(struct hmsrv_io_ctx *ctx, const void *buf, size_t len, int flags,
			   const struct sockaddr *daddr, socklen_t addrlen);
int hmsrv_io_socket_recvfrom(struct hmsrv_io_ctx *ctx, void *buf, size_t len, int flags,
			     struct sockaddr *saddr, socklen_t *addrlen);
int hmsrv_io_socket_close(struct hmsrv_io_ctx *ctx);
int hmsrv_io_socket_shutdown(struct hmsrv_io_ctx *ctx, int how);
int hmsrv_io_socket_fchown(struct hmsrv_io_ctx *ctx, unsigned int uid, unsigned int gid);
int hmsrv_io_socket_fstat(struct hmsrv_io_ctx *ctx, uid_t *uid);

int hmsrv_io_socket_alloc_new_ns(struct hmsrv_io_ctx *ctx, int nsid);
int hmsrv_io_socket_destroy_ns(struct hmsrv_io_ctx *ctx, int nsid);
int hmsrv_io_socket_mmap(struct hmsrv_io_ctx *ctx, const struct net_mmap_info *info, uintptr_t *rvaddr);

#endif /* ULIBS_LIBHMSRV_IO_NET_SOCKET_H */
