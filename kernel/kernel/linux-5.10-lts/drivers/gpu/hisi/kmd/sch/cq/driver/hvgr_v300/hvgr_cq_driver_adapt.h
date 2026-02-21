/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_CQ_DRIVER_ADAPT_H
#define HVGR_CQ_DRIVER_ADAPT_H

#include <linux/types.h>
#include "hvgr_regmap.h"
#include "hvgr_dm_api.h"
#include "hvgr_cq.h"

static inline uint32_t hvgr_get_bind_status(struct hvgr_device *gdev, uint32_t chn)
{
#if hvgr_version_ge(350)
	return 0;
#else
	return hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_status(chn)));
#endif
}

static inline void hvgr_set_send_mode(struct hvgr_device *gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

	gcqctx->hw_cfg.bc_send_mode = CQ_BC_SEND_PARALLEL;
#if hvgr_version_ge(350)
	gcqctx->hw_cfg.rt_rbc_send_mode = CQ_BC_SEND_PARALLEL;
#endif
}

#define CQ_SEM_MEM_PROPERTY      HVGR_MEM_PROPERTY_4G_VA
#define CQ_IRQ_MASK_VALUE           0x67fb
#define CQ_BIND_TIME_OUT_VALUE      65535
#define CQ_TIMESLICE_VALUE          3000000

#if hvgr_version_eq(300)
/* entrys_os received range of five_filed os value */
#define CQ_SUBQ_ENTRY_OS_PRES_L_H_CFG   0x4040404
#endif

#if hvgr_version_ge(350)
/* The totally size of TCAM is 12K. */
#define CQ_BINNING_RAM_SIZE_VALUE    0x00002800
#define CQ_RENDER_RAM_SIZE_VALUE     0x00000000
#define CQ_COMPUTER_RAM_SIZE_VALUE   0x00000400
#define CQ_BVH_RAM_SIZE_VALUE        0x00000000
#define CQ_RT_RAM_SIZE_VALUE         0x00000400
#else
/* The totally size of TCAM is 8K. */
#define CQ_BINNING_RAM_SIZE_VALUE    0x00001400
#define CQ_RENDER_RAM_SIZE_VALUE     0x00000200
#define CQ_COMPUTER_RAM_SIZE_VALUE   0x00000400
#define CQ_BVH_RAM_SIZE_VALUE        0x00000200
#define CQ_RT_RAM_SIZE_VALUE         0x00000400
#endif

#if hvgr_version_ge(350)
#define CQ_MAX_RMU_COUNT_VALUE			1024
#define CQ_MAX_RMU_SLICE_COUNT_VALUE	64
#define CQ_MAX_VB_SHARP_COUNT_VALUE		32
#define CQ_RENDER_NODE_STYPE_VALUE		0x7FFFFFFA
#define CQ_MESH_NODE_STYPE_VALUE		0x7FFFFFF9
#define CQ_INTERNAL_UNIFORM_SIZE_VALUE	544
#endif

/* Recommended value in the register manual */
#if hvgr_version_ge(350)
#define CQ_ENTRYS_OS_CFG  0x22525
#else
#define CQ_ENTRYS_OS_CFG  0x22225
#endif

/* CQ entry send out mode  */
#define CQ_ENTRY_REORDER            0U
#define CQ_ENTRY_ORDER              1U

#define CQ_SEM_WAIT_MODE_POLLING  0
#define CQ_SEM_WAIT_MODE_TRIGGER  1

#define CQ_ENTRY_TIMEOUT_SWITCH     true
#define CQ_EMPTY_TIMEOUT_IN_FPGA    10000     /* 20MHZ */
#define CQ_EMPTY_TIMEOUT_IN_CHIP    119500    /* 300MHZ~750MHZ */

#define HVGR_ENTRY_DMUP_DIR                CONFIG_BBOX_LOG_DIR "entrydump/"
#define HVGR_ENTRY_DMUP_FILE               "cq_%u_%u_%s_info.bin"
#define HVGR_ENTRY_DMUP_DIR_MODE           0770

#define HVGR_ENTRY_DUMP_HEAD_LEN    32

#define HVGR_WAIT_ENTRY_FENCE_FLAG  13

enum hvgr_cq_subchn_index {
	RENDER_IDX = 0,
	BIN_IDX,
	COMPUTE_IDX,
	BVHG_IDX,
	RT_IDX,
	SUBCHN_MAX_IDX,
};

enum hvgr_cq_subchn_index_200 {
	RENDER_LOW_IDX = 10,
	RENDER_HIGH_IDX,
	BIN_LOW_IDX,
	BIN_HIGH_IDX,
	COMPUTE_LOW_IDX,
	COMPUTE_HIGH_IDX,
};

#ifdef CONFIG_DFX_DEBUG_FS
void *hvgr_cq_get_entry_buffer_kva(struct hvgr_cq_ctx * const cq_ctx, bool is_order);

void hvgr_cq_clear_buf_full(void *buffer, struct hvgr_cq_ctx * const cq_ctx,
	bool is_order, bool need_lock);
#endif

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
void hvgr_cq_check_wait_entry(struct hvgr_cq_ctx * const cq_ctx, struct hvgr_device *gdev);
#endif
#endif
