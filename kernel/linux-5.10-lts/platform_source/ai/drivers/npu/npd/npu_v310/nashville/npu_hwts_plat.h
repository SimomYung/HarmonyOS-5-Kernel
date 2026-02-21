/*
 * npu_hwts_plat.h
 *
 * about npu hwts plat
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __NPU_HWTS_PLAT_H
#define __NPU_HWTS_PLAT_H

#include "npu_platform_register.h"
#include "npu_platform.h"
#include "npu_common.h"

#define TEST_AICORE_SHIFT 0x14  // aicore 1 base addr: aicore 0 + 1 << 0x14
#define HWTS_CHANNEL_NUM 64
#define HWTS_ACSQ_NUM    8
#define AIC_NUM 1
#define AIV_NUM 1
#define EFF_AIC_NUM 1

struct hwts_interrupt_info {
	u8 aic_task_runtime_limit_exp;
	u8 aiv_task_runtime_limit_exp;
	u8 notify_wait_task_runtime_limit_exp;
	u8 eff_aic_task_runtime_limit_exp;
	u32 l1_normal_ns_interrupt;
	u32 l1_debug_ns_interrupt;
	u32 l1_error_ns_interrupt;
	u32 l1_dfx_interrupt;
};

struct sq_exception_info {
	u16 sq_head;
	u16 sq_length;
	u16 sq_tail;
	u16 acsq_id;
	u32 sqcq_fsm_state;
	u32 sq_prefetch_busy;
	u32 sqe_type;
	u32 blk_dim;
};

u64 npu_hwts_get_base_addr(void);

u16 hwts_get_acsq_id_by_rtsq_id(u16 channel_id);
u16 hwts_get_rtsq_id_by_acsq_id(u16 channel_id);

int npu_hwts_query_aic_pool_status(
	u8 index, u8 *reg_val);

int npu_hwts_query_aiv_pool_status(
	u8 index, u8 *reg_val);

int npu_hwts_query_aic_eff_pool_status(
	u8 index, u8 *reg_val);

int npu_hwts_query_sw_status(u16 channel_id, u32 *reg_val);

int npu_hwts_query_sq_head(u16 channel_id, u16 *reg_val);

int npu_hwts_query_sqe_type(u16 channel_id, u8 *reg_val);

int npu_hwts_query_aicore_own_bitmap(u16 channel_id, u8 *reg_val);

int npu_hwts_query_aicore_exception_bitmap(u16 channel_id, u8 *reg_val);

int npu_hwts_query_aicore_trap_bitmap(u16 channel_id, u8 *reg_val);

int npu_hwts_query_aic_task_config(void);

int npu_hwts_query_aiv_task_config(void);

int npu_hwts_query_eff_aic_task_config(void);

int npu_hwts_query_interrupt_info(
	struct hwts_interrupt_info *interrupt_info);

int npu_hwts_query_sq_info(
	u16 channel_id, struct sq_exception_info *sq_info);

int npu_hwts_polling_aic_fsm_state(u64 timeout);

int npu_hwts_polling_aiv_fsm_state(u64 timeout);

int npu_hwts_polling_eff_aic_fsm_state(u64 timeout);

int npu_hwts_polling_acsq_fsm_state(u16 acsq_id, u64 timeout);

int npu_hwts_polling_rtsq_fsm_state(u16 channel_id, u64 timeout);

int npu_hwts_polling_acsq_ip_own_state(u16 acsq_id, u64 timeout, uint32_t *need_reset);

int npu_clear_hts_event_tbl(uint32_t event_id);

#endif
