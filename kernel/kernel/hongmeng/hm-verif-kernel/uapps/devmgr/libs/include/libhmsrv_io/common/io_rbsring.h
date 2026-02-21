/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Ring buffer based on shm_node in hongmeng
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 25 14:49:27 2021
 */
#ifndef ULIBS_LIBHMSRV_IO_IO_RBSRING_H
#define ULIBS_LIBHMSRV_IO_IO_RBSRING_H

#include <stddef.h>
#include <stdint.h>
#include <hmkernel/capability.h>

/* base */
#define IO_URING_WITHOUT_CQ	(0x1)
#define IO_URING_INSIDE_PROCESS	(0x2)
#define IO_URING_SHOULD_STOP	(0x10000000)
struct uring_sqe {
	unsigned int opcode;
	long long user_data;
};

struct uring_cqe {
	long long user_data;
	int result;
};

struct io_uring;
struct io_uring *io_uring_create(unsigned int item_cnt, unsigned int item_size, unsigned int flags);
void io_uring_stop(struct io_uring *uring);
void io_uring_destroy(struct io_uring *uring);
int io_uring_grant_to_caller(const struct io_uring *uring, uint64_t *ret_key);
int io_uring_grant_by_xref(const struct io_uring *uring, xref_t xref, uint64_t *ret_key);
struct io_uring *io_uring_import(uint64_t fd, uint32_t cnode_idx);

int io_uring_wait_sqe(struct io_uring *uring, struct uring_sqe *sqe, unsigned int sz);
int io_uring_wait_cqe(struct io_uring *uring, struct uring_cqe *cqe);

int io_uring_submit_sqe(struct io_uring *uring, struct uring_sqe *sqe, unsigned int sz);
int io_uring_complete_sqe(struct io_uring *uring, const struct uring_sqe *sqe, int err);

#endif
