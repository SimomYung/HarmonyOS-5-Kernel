/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: util functions to do userspace memory copy
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 5 15:48:34 2023
 */

#ifndef __NETD_UCOPY_H__
#define __NETD_UCOPY_H__

#include <sys/uio.h>
#include <sys/socket.h>

#include "ux_socket.h"

#ifndef UIO_FASTIOV
#define UIO_FASTIOV 8
#endif

#ifndef UIO_MAXIOV
#define UIO_MAXIOV 1024
#endif

#define MAX_RW_COUNT (INT_MAX & PAGE_MASK)

int ucopy_sendmsg_msghdr_read(struct netd_msghdr *msg_serv,
			      const struct msghdr *msg_user,
			      struct iovec **fastiov);

int ucopy_recvmsg_msghdr_read(struct netd_msghdr *msg_serv,
			      const struct msghdr *msg_user,
			      struct sockaddr **uaddr,
			      struct iovec **fastiov);

int ucopy_cmsg_read(struct netd_msghdr *msg, char **fastcmsg, size_t fastcmsg_len);
int ucopy_cmsg_write(struct msghdr *msg, int level, int type, size_t len, void *data);

struct iovec *ucopy_iovec(const struct iovec *uiov, size_t nr_segs, unsigned fast_segs,
			  struct iovec *fast_iov);
ssize_t ucopy_import_iovec(struct iovec *uiov, size_t nr_segs,
			   unsigned fast_segs, struct iovec **iovp,
			   struct iov_iter *iter);
int ucopy_addr_write(struct sockaddr_storage *kaddr, int klen, void *uaddr, socklen_t *ulen);
int ucopy_addr_read(struct sockaddr_storage *kaddr, const void *uaddr, int ulen);

#endif /* __NETD_UCOPY_H__ */
