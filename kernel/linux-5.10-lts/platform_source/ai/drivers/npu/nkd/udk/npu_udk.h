/*
 * npu_udk.h
 *
 * about ldk_npu to udk communicate
 *
 * Copyright (c) 2012-2024 Huawei Technologies Co., Ltd.
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
#ifndef _NPU_UDK_H_
#define _NPU_UDK_H_

#include <linux/types.h>
#include "npu_except_mgr.h"
#include "npu_proc_ctx.h"

enum {
	HW_BLOCK_IDLE = 0,
	HW_BLOCK_RUNNING = 1,
	HW_BLOCK_RUNNING_ = 2,
	HW_BLOCK_COMPLETE = 3,
};

typedef enum {
	FFTS_HW_GPU,
	FFTS_HW_NPU,
	FFTS_HW_MAX
} ffts_hw_type;

enum UDK_NPU_MINOR_CMD {
	UDK_MINOR_CMD_NPU_DUMP_EXCEPTION = 0,
	UDK_MINOR_CMD_NPU_HWTS_QUERY_SQE_TYPE,
	UDK_MINOR_CMD_NPU_HANDLE_TS_EXCEPTION,
	UDK_MINOR_CMD_NPU_S4_RSV_DDR_BACKUP,
	UDK_MINOR_CMD_NPU_S4_RSV_DDR_RECOVER,
#ifdef CONFIG_NPU_FFRT
	UDK_MINOR_CMD_NPU_HANDLE_FFTS_EXCEPTION,
	UDK_MINOR_CMD_NPU_HANDLE_GLOBAL_EXCEPTION,
	UDK_MINOR_CMD_NPU_HANDLE_CLEAR_BLOCK_RESOURCE,
	UDK_MINOR_CMD_NPU_CLEAR_HTS_EVENT_TBL,
	UDK_MINOR_CMD_NPU_FFTS_LOAD_MODEL_BUF,
	UDK_MINOR_CMD_NPU_FFTS_ENTRY_IS_CLEAR,
	UDK_MINOR_CMD_NPU_HWTS_CLEAR_ENTRY,
	UDK_MINOR_CMD_NPU_HWTS_SET_SSID,
#endif
	UDK_MINOR_CMD_NPU_UDK_BYPASS,
	UDK_MINOR_CMD_NPU_MAX,
};

struct npu_block_ffts_global_excp_info {
	uint32_t ssid;
	uint32_t modelid;
	uint32_t taskidx;
};

enum FFAP1_STATUS {
	FFAP1_DISCONNECTED = 0,
	FFAP1_CONNECTING = 1,
	FFAP1_CONNECTED = 2,
	FFAP1_DISCONNECTING = 3
};

#ifdef CONFIG_NPU_FFRT
struct npu_ffrt_load_model_buf_info {
	u16 model_id;
	u16 stream_cnt;
	u16 stream_id[NPU_MODEL_STREAM_NUM];
	u16 stream_tasks[NPU_MODEL_STREAM_NUM];
	u64 aiv_csw_buff_addr[NPU_MODEL_STREAM_NUM];
	u32 priority;
	uint16_t uid;
};

struct npu_ffrt_model_ssid_info {
	uint32_t model_id;
	uint16_t ssid;
};

struct npu_ffrt_task_entry_info {
	uint16_t task_id;
	uint16_t entry_id;
};
#endif

int ffts_check_global_block_state(ffts_hw_type hw_type);
uint32_t ffts_check_hw_block_state(ffts_hw_type hw_type);
void ffts_hw_block_dis(ffts_hw_type hw_type);
int ffts_hw_block_en(ffts_hw_type hw_type);
void ffts_insert_global_excp_info(uint16_t uid, uint16_t modelid, uint16_t taskid);

int npd_dump_exception_without_power_check(struct npu_exception_record *er);
int npu_exception_aicore_reset(u16 channel_id);
int npu_hwts_query_sqe_type(u16 channel_id, u8 *reg_val);

#ifdef CONFIG_NPU_FFRT
int npu_handle_ffts_exception(struct ffts_exception_report_info *report);
int npu_handle_global_exception_for_ffts(void);
int npu_proc_block_kill_task(u16 uid);
int npu_clear_hts_event_tbl(uint32_t event_id);
int npu_ffrt_load_model_buf(struct npu_proc_ctx *proc_ctx, npu_model_desc_t *model_desc, uint16_t uid);
int npu_ffts_clear_entry(uint16_t task_idx, uint16_t entry_idx);
int npu_ffrt_set_ssid(uint32_t model_id, uint16_t ssid);
int npu_ffts_entry_is_clear(void);
#endif
#ifdef CONFIG_HIBERNATION
int npu_s4_rsv_ddr_backup(void);
int npu_s4_rsv_ddr_recover(void);
#endif
void npu_udk_bypass(void);
#endif