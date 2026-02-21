/*
 * hwts_driver.h
 *
 * about hwts driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
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
#ifndef _HWTS_DRIVER_H_
#define _HWTS_DRIVER_H_

#include "npu_osal.h"
#include "soc_npu_hwts_interface.h"
#ifdef CONFIG_NPU_SWTS
#include "npu_platform_exception_spec.h"
#define NPU_HWTS_MAX_AICORE_POOL_NUM 4

enum hwts_task_ctrl{
    HWTS_TASK_CTRL_TASK_KILL = 0x0,
    HWTS_TASK_CTRL_TASK_RESUME,
    HWTS_TASK_CTRL_EXCEPT_HANDLED,
    HWTS_TASK_CTRL_TASK_TERMINATE,
    HWTS_TASK_CTRL_SWAPIN_DONE,
    HWTS_TASK_CTRL_SWAPOUT_DONE,
    HWTS_TASK_CTRL_MAX,
};

struct list_head {
	struct list_head *next, *prev;
};

struct npu_entity_info {
	struct list_head list;
	uint32_t index;
	unsigned long long vir_addr;
	unsigned long iova;
};

struct schedule_sq_cq {
	uint16_t id;
	uint16_t head;
	uint16_t tail;
	uint16_t length;
	uint64_t base_addr;
};

struct hwts_sq_cq{
	struct list_head node;
	struct schedule_sq_cq schedule_sq;
	struct schedule_sq_cq schedule_cq;
	uint16_t model_id;
	uint16_t channel_id;
	uint16_t smmu_substream_id;
	uint16_t priority;
	uint8_t sched_status;
	uint8_t prof_en;
};
void hwts_driver_reset_event_table(uint16_t event_id);
void hwts_driver_start_exec_swap_buff_cfg(struct hwts_sq_cq *sqcq_info, uint16_t channel_id);
void hwts_driver_start_exec_dfx_cfg(struct hwts_sq_cq *sqcq_info, uint16_t channel_id,bool log_en, bool profiling_en);
void hwts_driver_task_ctrl(enum hwts_task_ctrl type, uint16_t sq_idx);
bool is_sqcq_info_base_addr_valid(struct hwts_sq_cq *sqcq_info);
#endif

void hwts_driver_save_aic_aiv_pool_status(void);
void hwts_driver_restore_aic_aiv_pool_status(uint8_t core_id_bitmap);
void hwts_driver_clear_channel_sq_en(uint16_t channel_id);
void hwts_driver_hwts_task_terminate(uint16_t rtsq);
void hwts_driver_reset_cq_head(uint16_t cq_idx);
#endif