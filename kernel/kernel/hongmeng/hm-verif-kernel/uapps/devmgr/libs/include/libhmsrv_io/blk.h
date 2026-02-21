/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service APIs of block device operation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#ifndef ULIBS_LIBHMSRV_IO_BLK_H
#define ULIBS_LIBHMSRV_IO_BLK_H

#include <stdlib.h>
#include <libhmsrv_io/type.h>

#define IO_RQ_OP_SHIFT		8
#define IO_RQ_SHIFT		32
#define IO_PG_SHIFT		32

#define IO_RQ_MASK		((1ULL << IO_RQ_SHIFT) - 1ULL)
#define IO_RQ_OPT_MASK		((1ULL << IO_RQ_OP_SHIFT) - 1ULL)
#define IO_RQ_IO_FLAGS_MASK	((~IO_RQ_OPT_MASK) & (IO_RQ_MASK))

/*
 * |---- 32 bits ----|---- 32 bits ----|
 *  <-    rq idx   -> <-   rw ops    ->
 */
#define hmsrv_io_get_rw(io_rw) ((io_rw) & (IO_RQ_MASK))
#define hmsrv_io_get_op(io_rw) ((io_rw) & (IO_RQ_OPT_MASK))
#define hmsrv_io_get_io_flags(io_rw) ((io_rw) & (IO_RQ_IO_FLAGS_MASK))
#define hmsrv_io_get_rq_idx(io_rw) ((io_rw) >> (IO_RQ_SHIFT))
#define hmsrv_io_get_io_rw(rw, rq_idx) \
	(((unsigned long long)(rq_idx) << (IO_RQ_SHIFT)) + (unsigned long long)(unsigned int)(rw))

#define INVALID_IO_IDX 0xFFFFFFFF
struct hmsrv_io_blk_vec {
	/* page frame number */
	unsigned long long pfn;
	unsigned int	len;
	unsigned int	offset;
};

int hmsrv_io_blk_check_feature(const struct hmsrv_io_ctx *ctx,
			       int idx, uint64_t feature, uint64_t *feature_out);

int hmsrv_io_blk_register_bio_notify(const struct hmsrv_io_ctx *ctx,
				     cref_t ap_cref,
				     unsigned int method,
				     size_t bd_shm_size,
				     uint64_t *submit_fd, uint64_t *end_fd,
				     unsigned long *shm_vaddr,
				     size_t *obj_cnt);

int hmsrv_io_blk_submit_bio(const struct hmsrv_io_ctx *ctx,
			    unsigned long long rw, unsigned long long sector,
			    struct hmsrv_io_blk_vec *vec, int cnt,
			    unsigned int priority, void *bi_crypt_context,
				void (*dump_fs)(void));

int hmsrv_io_blk_submit_cmd(const struct hmsrv_io_ctx *ctx,
			    unsigned long long cmd, unsigned long long sector,
			    unsigned int size, unsigned int priority);

int hmsrv_io_blk_run_queue(const struct hmsrv_io_ctx *ctx, unsigned int flags,
			   unsigned int idx);

void hmsrv_io_blk_polling(const struct hmsrv_io_ctx *ctx, int rw);
#endif	/* ULIBS_LIBHMSRV_IO_BLK_H */
