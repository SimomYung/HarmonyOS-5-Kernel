/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Ring buffer based on shm_node in hongmeng
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 25 14:49:27 2021
 */
#ifndef ULIBS_LIBHMSRV_IO_IO_NET_SRING_H
#define ULIBS_LIBHMSRV_IO_IO_NET_SRING_H

#include <libhmsrv_io/common/io_rbsring.h>

/* subclass */
#define URING_OP_NETRX	(1U)
struct uring_net_data {
	uint32_t pool_offset;
	int data_len;
	int data_offset;
	int linux_devno;
};

struct uring_net_sqe {
	struct uring_sqe base; /* must be first */
	struct uring_net_data net;
};

static inline void io_uring_prepare_net_recv(struct uring_net_sqe *sqe, unsigned int pool_offset,
					     int offset, int rx_len, int ifindex)
{
	sqe->base.opcode = URING_OP_NETRX;
	sqe->base.user_data = (long long)pool_offset;
	sqe->net.pool_offset = pool_offset;
	sqe->net.data_offset = offset;
	sqe->net.data_len = rx_len;
	sqe->net.linux_devno = ifindex;
}

static inline int io_uring_submit_netsqe(struct io_uring *uring, struct uring_net_sqe *sqe)
{
	return io_uring_submit_sqe(uring, &sqe->base, sizeof(*sqe));
}

static inline int io_uring_wait_netsqe(struct io_uring *uring, struct uring_net_sqe *sqe)
{
	return io_uring_wait_sqe(uring, &sqe->base, sizeof(*sqe));
}

static inline int io_uring_wait_netcqe(struct io_uring *uring, struct uring_cqe *cqe)
{
	return io_uring_wait_cqe(uring, cqe);
}

static inline int io_uring_complete_netsqe(struct io_uring *uring, struct uring_net_sqe *sqe, int err)
{
	return io_uring_complete_sqe(uring, &sqe->base, err);
}

#endif
