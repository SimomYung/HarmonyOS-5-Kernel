/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#ifndef HVGR_SCHEDULE_H
#define HVGR_SCHEDULE_H
#include <linux/types.h>
#include "hvgr_version.h"

struct hvgr_ctx;
struct hvgr_device;
struct hvgr_cq_ctx;

#define HVGR_SCH_MODE_JOBSLOT    0
#define HVGR_SCH_MODE_CQ         1

#define HVGR_SCH_SUBMIT_IGNORE   0
#define HVGR_SCH_SUBMIT_ALLOW    1

#define HVGR_SCH_INTRDISABLE     0
#define HVGR_SCH_INTRENABLE      1

#if hvgr_version_ge(HVGR_V350)
#define HVGR_SSID_INVALID             (-1)
#define HVGR_NORMAL_SSID_NUM          64
#define HVGR_NORMAL_DYNAMIC_SSID_NUM  8
#define HVGR_ALL_SSID_NUM             64
#endif

enum sch_rst_status {
	HVGR_SCH_SOFT_CLEAR = 0,
	HVGR_SCH_SOFT_STOP,
	HVGR_SCH_SOFT_CLEAR_GPU_TASK,
	HVGR_SCH_DUMP_REGISTER,
};

enum sch_type {
	HVGR_SCH_SCHEDULE_ALL = 0,
	HVGR_SCH_SUBMIT_ALL,
	HVGR_SCH_ONLY_INQUQUE,
};

struct hvgr_sch_backend_ops {
	int (*init)(struct hvgr_device * const gdev);
	void (*term)(struct hvgr_device * const gdev);
	long (*ctx_create)(struct hvgr_ctx * const ctx);
	void (*ctx_destroy)(struct hvgr_ctx * const ctx);
	long (*cmd_dispatch)(struct hvgr_ctx *const ctx, unsigned int cmd, unsigned long arg);
	int (*suspend)(struct hvgr_device * const gdev);
	void (*resume)(struct hvgr_device * const gdev);
	void (*start)(struct hvgr_device * const gdev);
	void (*stop)(struct hvgr_device * const gdev);
	void (*ctx_stop)(struct hvgr_ctx * const ctx);
	void (*reset)(struct hvgr_device * const gdev, enum sch_rst_status reset_stage);
	void (*set_mode)(struct hvgr_device * const gdev, uint32_t mode, uint32_t sch_flag);
	void (*intr_switch)(struct hvgr_device * const gdev, uint32_t flag);
	void (*hts_schedule)(struct hvgr_device * const gdev, struct hvgr_cq_ctx *cq_ctx,
		enum sch_type type);
};

union hvgr_sch_sub_dev {
	struct hvgr_cq_dev *cq_dev;
	/* add other struct in here. */
};

struct hvgr_sch_dev {
	union hvgr_sch_sub_dev sch_sub_dev;
	struct hvgr_sch_backend_ops *ops;
};

struct hvgr_sch_backend_ops *hvgr_sched_get_ops(void);

#if hvgr_version_ge(HVGR_V500)

struct cq_ring_buf {
	uint64_t reserve1;
	uint64_t ring_buf_w_ptr;
	uint64_t ring_buf_r_ptr;
	uint8_t reserve2[40]; /* reserve 320bits */

	uint8_t jcd_array[4][64]; /* Jcd buffer size is 512bits * 4 */

	/* Follow memory is ptr_buf array */
};

struct cq_memfree_ring_buf {
	uint64_t ring_buf_w_ptr;
	uint64_t ring_buf_r_ptr;
	uint8_t reserve[48]; /* reserve 384bit */
};

/* Hardware need start address align to 64Bytes */
#define CQ_RINGBUF_ALIGN_BYTES 64u
#define cq_ringbuf_align(size) \
	(((size) + (CQ_RINGBUF_ALIGN_BYTES - 1)) & (~(CQ_RINGBUF_ALIGN_BYTES - 1)))

/* Job ptr_buffer size is 64 bits, unit is byte */
#define CQ_RINGBUF_ENTRY_SIZE 8u

#define CQ_REND_RING_ENTRY_NUMS     8u
#define CQ_REND_RING_BUF_SIZE \
	cq_ringbuf_align(sizeof(struct cq_ring_buf) + \
		CQ_RINGBUF_ENTRY_SIZE * CQ_REND_RING_ENTRY_NUMS)

#define CQ_BIN_RING_ENTRY_NUMS      128u
#define CQ_BIN_RING_BUF_SIZE \
	cq_ringbuf_align(sizeof(struct cq_ring_buf) + \
		CQ_RINGBUF_ENTRY_SIZE * CQ_BIN_RING_ENTRY_NUMS)

#define CQ_COMPUTER_RING_ENTRY_NUMS 8u
#define CQ_COMPUTER_RING_BUF_SIZE \
	cq_ringbuf_align(sizeof(struct cq_ring_buf) + \
		CQ_RINGBUF_ENTRY_SIZE * CQ_COMPUTER_RING_ENTRY_NUMS)

#define CQ_BVHG_RING_ENTRY_NUMS     8u
#define CQ_BVHG_RING_BUF_SIZE \
	cq_ringbuf_align(sizeof(struct cq_ring_buf) + \
		CQ_RINGBUF_ENTRY_SIZE * CQ_BVHG_RING_ENTRY_NUMS)

#define CQ_RT_RING_ENTRY_NUMS       8u
#define CQ_RT_RING_BUF_SIZE \
	cq_ringbuf_align(sizeof(struct cq_ring_buf) + CQ_RINGBUF_ENTRY_SIZE * CQ_RT_RING_ENTRY_NUMS)

#define CQ_HW_RINGBUF_SIZE (CQ_REND_RING_BUF_SIZE + CQ_BIN_RING_BUF_SIZE + \
		CQ_COMPUTER_RING_BUF_SIZE + CQ_BVHG_RING_BUF_SIZE + CQ_RT_RING_BUF_SIZE)

/* memfree ring buf */
#define CQ_MEMFREE_RINGBUF_ENTRY_SIZE     8u
#define CQ_MEMFREE_RING_ENTRY_NUMS        64u

#define CQ_REND_MEMFREE_RING_BUF_SIZE \
			cq_ringbuf_align(sizeof(struct cq_memfree_ring_buf) + \
				CQ_MEMFREE_RINGBUF_ENTRY_SIZE * CQ_MEMFREE_RING_ENTRY_NUMS)
#define CQ_BIN_MEMFREE_RING_BUF_SIZE \
			cq_ringbuf_align(sizeof(struct cq_memfree_ring_buf) + \
				CQ_MEMFREE_RINGBUF_ENTRY_SIZE * CQ_MEMFREE_RING_ENTRY_NUMS)

#define CQ_COMPUTER_MEMFREE_RING_BUF_SIZE \
			cq_ringbuf_align(sizeof(struct cq_memfree_ring_buf) + \
				CQ_MEMFREE_RINGBUF_ENTRY_SIZE * CQ_MEMFREE_RING_ENTRY_NUMS)

#define CQ_BVHG_MEMFREE_RING_BUF_SIZE \
			cq_ringbuf_align(sizeof(struct cq_memfree_ring_buf) + \
				CQ_MEMFREE_RINGBUF_ENTRY_SIZE * CQ_MEMFREE_RING_ENTRY_NUMS)

#define CQ_RT_MEMFREE_RING_BUF_SIZE \
			cq_ringbuf_align(sizeof(struct cq_memfree_ring_buf) + \
				CQ_MEMFREE_RINGBUF_ENTRY_SIZE * CQ_MEMFREE_RING_ENTRY_NUMS)

#define CQ_HW_MEMFREE_RINGBUF_SIZE (CQ_REND_MEMFREE_RING_BUF_SIZE + CQ_BIN_MEMFREE_RING_BUF_SIZE + \
		CQ_COMPUTER_MEMFREE_RING_BUF_SIZE + CQ_BVHG_MEMFREE_RING_BUF_SIZE + CQ_RT_MEMFREE_RING_BUF_SIZE)

struct cq_sw_writeable_context {
	uint64_t hw_context_address;
	uint64_t queue_base_address;
	uint32_t queue_max_num_entries;
	uint32_t umd_sw_context_address_l;
	uint32_t umd_sw_context_address_h;
	uint32_t priority;
	uint32_t defer_cs_issue_en;
	uint32_t warp_priority;
	uint64_t mem_ctx_address;
	uint32_t context_id;

	uint32_t reserve1[3]; /* reserve 96 bit */

	struct {
		uint64_t rend_ring_buf_struct_ptr;
		uint32_t rend_ring_buf_struct_size;
		uint32_t reserve0;

		uint64_t bin_ring_buf_struct_ptr;
		uint32_t bin_ring_buf_struct_size;
		uint32_t reserve1;

		uint64_t comp_ring_buf_struct_ptr;
		uint32_t comp_ring_buf_struct_size;
		uint32_t reserve2;

		uint64_t bvhg_ring_buf_struct_ptr;
		uint32_t bvhg_ring_buf_struct_size;
		uint32_t reserve3;

		uint64_t rt_ring_buf_struct_ptr;
		uint32_t rt_ring_buf_struct_size;
		uint32_t reserve4;
	} ring_buffer;

	struct {
		uint64_t rend_ring_buf_struct_ptr;
		uint32_t rend_ring_buf_struct_size;
		uint32_t reserve0;

		uint64_t bin_ring_buf_struct_ptr;
		uint32_t bin_ring_buf_struct_size;
		uint32_t reserve1;

		uint64_t comp_ring_buf_struct_ptr;
		uint32_t comp_ring_buf_struct_size;
		uint32_t reserve2;

		uint64_t bvhg_ring_buf_struct_ptr;
		uint32_t bvhg_ring_buf_struct_size;
		uint32_t reserve3;

		uint64_t rt_ring_buf_struct_ptr;
		uint32_t rt_ring_buf_struct_size;
		uint32_t reserve4;
	} memfree_ring_buffer;

	uint32_t reserve2[8]; /* reserve 256 bit */

	struct {
		uint64_t order_buf_ptr;
		uint32_t order_buf_size;
		uint32_t reserve0;

		uint64_t sync_buf_ptr;
		uint32_t sync_buf_size;
		uint32_t reserve1;

		uint32_t reserve3[8];
	} dfx_buf_info;

	uint32_t wait_entry_info[208];
};
#endif
#endif
