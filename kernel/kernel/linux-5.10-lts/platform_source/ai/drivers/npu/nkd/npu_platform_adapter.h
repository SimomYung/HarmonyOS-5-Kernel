/*
 * npu_platform_adapter.h
 *
 * about npu platform adapter
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#ifndef __NPU_PLATFORM_ADAPTER_H
#define __NPU_PLATFORM_ADAPTER_H

#include <linux/platform_device.h>
#include "npu_proc_ctx.h"
#include "npu_rt_task.h"
#include "npu_except_mgr.h"
#include "npu_pool.h"
#include "npu_platform.h"
#include "npd_interface.h"
#include "npu_ipc.h"

void npu_set_npd_registry(struct npd_registry *npd);
void npu_set_npd_unregistry(void);
int npu_plat_notify(struct npu_dev_ctx *dev_ctx, npd_notice_t pn, long para1, long para2);
int npu_plat_send_mail(void *mailbox, unsigned long mailbox_len, const void *msg, unsigned long msg_len);
int npu_plat_get_work_mode(struct npu_proc_ctx *proc_ctx, npu_rt_task_t *comm_task, u32 *work_mode);
int npu_plat_dump_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er);
int npu_plat_handle_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er);
int npu_plat_get_feature_mask(u32 *feature_mask);
bool npu_plat_query_sku_value(enum npu_sku_var_e sv);
int npu_plat_verify_hwts_sqe(struct npu_proc_ctx *proc_ctx, void *stream_buf_addr, u32 stream_len, u32 *core_type);
int npu_plat_save_hwts_sqe(npu_model_desc_info_t *model_desc_info, void *stream_buf_addr,
    struct npu_entity_info *hwts_sq_sub_info, u64 ts_sq_len);
int npu_plat_save_sw_sqe(npu_model_desc_t *model_desc,
    void *stream_buf_addr, u16 *first_task_id, u8 devid, int stream_idx);
int npu_plat_switch_unit(struct npu_dev_ctx *dev_ctx, enum npu_sfu_e sfu,
    enum npu_workmode_flag mode, void __iomem* g_npu_autofs_crg_vaddr);

#ifdef CONFIG_NPU_FFRT
int npu_plat_ffts_entry_is_clear(void);
int npu_plat_ffts_clear_entry_by_taskidx(uint16_t task_idx);
int npu_plat_proc_block_resource_clear(u16 uid);
int npu_plat_ffrt_load_model_buf(struct npu_ffrt_load_model_buf_info *model_desc);
int npu_plat_ffrt_set_ssid(uint32_t model_id, uint16_t ssid);
#endif

int npu_plat_set_res_mem(struct npu_platform_info *plat_info);
int npu_plat_send_ipc_with_power_check(u32 cmd_type, u32 msg_sync_type,
	const u8 *send_msg, u32 send_len, enum ipc_send_ack_type ack_type);

#endif // __NPU_PLATFORM_ADAPTER_H