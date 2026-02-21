/*
 * npu_platform_adapter.c
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
#include "npu_platform_adapter.h"
#include "npd_interface.h"
#include "npu_proc_ctx.h"
#include "npu_common.h"
#include "npu_user_common.h"

static struct npd_registry* g_npd; // NPD driver registry data

void npu_set_npd_registry(struct npd_registry *npd)
{
    g_npd = npd;
}

void npu_set_npd_unregistry(void)
{
    g_npd = NULL;
}

int npu_plat_notify(struct npu_dev_ctx *dev_ctx, npd_notice_t pn, long para1, long para2)
{
    if(g_npd != NULL && g_npd->npd_notify != NULL)
        return g_npd->npd_notify(dev_ctx, pn, para1, para2);
    return -1;
}

int npu_plat_send_mail(void *mailbox, unsigned long mailbox_len, const void *msg, unsigned long msg_len)
{
    if (g_npd != NULL && g_npd->npd_send_mail != NULL)
        return g_npd->npd_send_mail(mailbox, mailbox_len, msg, msg_len);
    return -1;
}

int npu_plat_get_work_mode(struct npu_proc_ctx *proc_ctx, npu_rt_task_t *comm_task, u32 *work_mode)
{
    cond_return_error(work_mode == NULL, -1, "invalid param\n");
    if (g_npd != NULL && g_npd->npd_nosec_get_workmode != NULL) {
        *work_mode = (u32)g_npd->npd_nosec_get_workmode(proc_ctx, comm_task);
        return 0;
    }
    return -1;
}

int npu_plat_dump_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
    if (g_npd != NULL && g_npd->npd_dump_exception != NULL)
        return g_npd->npd_dump_exception(dev_ctx, er);
    return -1;
}

int npu_plat_handle_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
    if (g_npd != NULL && g_npd->npd_handle_exception != NULL)
        return g_npd->npd_handle_exception(dev_ctx, er);
    return -1;
}

int npu_plat_get_feature_mask(u32 *feature_mask)
{
    cond_return_error(feature_mask == NULL, -1, "invalid param\n");
    if (g_npd != NULL) {
        *feature_mask = g_npd->feature_mask;
        return 0;
    }
    return -1;
}

bool npu_plat_query_sku_value(enum npu_sku_var_e sv)
{
    if (g_npd != NULL && g_npd->npd_query_sku_value != NULL)
        return g_npd->npd_query_sku_value(sv);
    return false;
}

int npu_plat_verify_hwts_sqe(struct npu_proc_ctx *proc_ctx, void *stream_buf_addr, u32 stream_len, u32 *core_type)
{
    if (g_npd != NULL && g_npd->npd_verify_sink_tasks != NULL)
        return g_npd->npd_verify_sink_tasks(proc_ctx, stream_buf_addr, stream_len, core_type);
    return -1;
}

int npu_plat_save_hwts_sqe(npu_model_desc_info_t *model_desc_info, 
    void *stream_buf_addr, struct npu_entity_info *hwts_sq_sub_info, u64 ts_sq_len)
{
    if (g_npd != NULL && g_npd->npd_format_hw_sqe != NULL)
        return g_npd->npd_format_hw_sqe(model_desc_info, stream_buf_addr,
		(void *)(uintptr_t)hwts_sq_sub_info->vir_addr, ts_sq_len);
    return -1;
}

int npu_plat_save_sw_sqe(npu_model_desc_t *model_desc,
    void *stream_buf_addr, u16 *first_task_id, u8 devid, int stream_idx)
{
    if (g_npd != NULL && g_npd->npd_format_sw_sqe != NULL)
        return g_npd->npd_format_sw_sqe(model_desc, stream_buf_addr, first_task_id, devid, stream_idx);
    return -1;
}

int npu_plat_switch_unit(struct npu_dev_ctx *dev_ctx, enum npu_sfu_e sfu,
    enum npu_workmode_flag mode, void __iomem* g_npu_autofs_crg_vaddr)
{
    if (g_npd != NULL && g_npd->npd_switch_unit != NULL)
        return g_npd->npd_switch_unit(dev_ctx, sfu, mode, g_npu_autofs_crg_vaddr);
    return -1;
}

#ifdef CONFIG_NPU_FFRT
int npu_plat_ffts_entry_is_clear(void)
{
    if (g_npd != NULL && g_npd->npd_ffts_entry_is_clear != NULL)
        return g_npd->npd_ffts_entry_is_clear();
    return -1;
}
#ifdef CONFIG_NPU_FFRT_V100
int npu_plat_ffts_clear_entry_by_taskidx(uint16_t task_idx)
{
    if (g_npd != NULL && g_npd->npd_ffts_clear_entry_by_taskidx != NULL)
        return g_npd->npd_ffts_clear_entry_by_taskidx(task_idx);
    return -1;
}

int npu_plat_proc_block_resource_clear(u16 uid)
{
    if (g_npd != NULL && g_npd->npd_proc_block_resource_clear != NULL)
        return g_npd->npd_proc_block_resource_clear(uid);
    return -1;
}
#endif
int npu_plat_ffrt_load_model_buf(struct npu_ffrt_load_model_buf_info *model_desc)
{
    if (g_npd != NULL && g_npd->npd_ffrt_load_model_buf != NULL)
        return g_npd->npd_ffrt_load_model_buf(model_desc);
    return -1;
}

int npu_plat_ffrt_set_ssid(uint32_t model_id, uint16_t ssid)
{
    if (g_npd != NULL && g_npd->npd_ffrt_set_ssid != NULL)
        return g_npd->npd_ffrt_set_ssid(model_id, ssid);
    return -1;
}
#endif

int npu_plat_set_res_mem(struct npu_platform_info *plat_info)
{
	if (g_npd != NULL && g_npd->npd_set_resmem != NULL)
		return g_npd->npd_set_resmem(plat_info);
	return -1;
}

int npu_plat_send_ipc_with_power_check(u32 cmd_type, u32 msg_sync_type,
	const u8 *send_msg, u32 send_len, enum ipc_send_ack_type ack_type)
{
    if (g_npd != NULL && g_npd->npd_send_ipc_with_power_check != NULL)
		return g_npd->npd_send_ipc_with_power_check(cmd_type, msg_sync_type,
            send_msg, send_len, ack_type);
	return -1;
}
