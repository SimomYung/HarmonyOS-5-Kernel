/*
 * npu_arch_ffts_model.c
 *
 * about npu ffts model arch adapter
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */

#include "npu_arch_ffts_model.h"

#include "npu_common_resource.h"
#include "npu_ddr_map.h"
#include "npu_log.h"
#include "npu_platform_resource.h"
#include "npu_shm.h"
#include "npu_soc_defines.h"
#include "npu_user_common.h"
#include "npu_ffts_model.h"

int npu_ffrt_load_mdc_buf(struct npu_ffrt_load_model_buf_info *model_desc)
{
	int ret = 0;
	struct npu_ffrt_model_buff* ffrt_mdc_buf = get_ffrt_mdc_buf(model_desc->model_id);
	struct npu_hwts_static_swap_buff* ffrt_swap_buf_phy = get_ffrt_swap_buf_phy(model_desc->model_id);
	cond_return_error(ffrt_mdc_buf == NULL, -1, "get ffrt_mdc_buf failed\n");
	cond_return_error(ffrt_swap_buf_phy == NULL, -1, "get ffrt_swap_buf_phy failed\n");

	/* mdl buff config, reserved err_mode pmu_id, only 1 stream */
	ret = memset_s(ffrt_mdc_buf, sizeof(struct npu_ffrt_model_buff), 0, sizeof(struct npu_ffrt_model_buff));
	cond_return_error(ret != 0, -1, "memset_s mdc_buf failed");

	ffrt_mdc_buf->va = 0;
	ffrt_mdc_buf->ssid = model_desc->ssid;
	ffrt_mdc_buf->r0_vld = 1;
	ffrt_mdc_buf->p0_pri = model_desc->priority;
	ffrt_mdc_buf->r0_head = 0;
	ffrt_mdc_buf->r0_tail = model_desc->stream_tasks[0];
	ffrt_mdc_buf->r0_swapbuf_addr_l = (uint32_t)((uintptr_t)ffrt_swap_buf_phy & UINT32_MAX);
	ffrt_mdc_buf->r0_swapbuf_addr_h = (uint16_t)(((uintptr_t)ffrt_swap_buf_phy >> 32) & UINT16_MAX);

	if (model_desc->stream_cnt > 1) {
		struct npu_hwts_static_swap_buff* ffrt_swap_buf_phy_1 = ffrt_swap_buf_phy + 1;
		ffrt_mdc_buf->r1_vld = 1;
		ffrt_mdc_buf->p1_pri = model_desc->priority;
		ffrt_mdc_buf->r1_head = 0;
		ffrt_mdc_buf->r1_tail = model_desc->stream_tasks[1];
		ffrt_mdc_buf->r1_swapbuf_addr_l = (uint32_t)((uintptr_t)ffrt_swap_buf_phy_1 & UINT32_MAX);
		ffrt_mdc_buf->r1_swapbuf_addr_h = (uint16_t)(((uintptr_t)ffrt_swap_buf_phy_1 >> 32) & MAX_UINT16_NUM);
	}
	npu_drv_warn("model_desc: model_id = %u, uid = %u, priority = %u, stream_cnt = %u, stream_tasks[0] = %u, "
		"stream_tasks[1] = %u", model_desc->model_id, model_desc->ssid, model_desc->priority, model_desc->stream_cnt,
		model_desc->stream_tasks[0], model_desc->stream_tasks[1]);
	return ret;
}

int npu_ffrt_load_swap_buf_index(struct npu_ffrt_load_model_buf_info *model_desc, int swap_buf_index)
{
	struct npu_stream_info *stream_info = NULL;
	struct npu_hwts_sq_info *hwts_sq_info = NULL;
	struct npu_hwts_cq_info *hwts_cq_info = NULL;
	struct npu_stream_info ffrt_stream_info = {0};
	u64 sq_iova_addr = 0;
	u64 cq_iova_addr = 0;

	struct npu_hwts_static_swap_buff* ffrt_swap_buf_index = get_ffrt_swap_buf_index(swap_buf_index, model_desc->model_id);
	cond_return_error(ffrt_swap_buf_index == NULL, -1, "get ffrt_swap_buf_index failed\n");

	/* get swap buff config */
	stream_info = npu_calc_stream_info(0, model_desc->stream_id[swap_buf_index]);
	cond_return_error(stream_info == NULL, -1,
		"stream_id= %u stream_info is null\n", model_desc->stream_id[swap_buf_index]);

	ffrt_stream_info.sq_index = stream_info->sq_index;
	ffrt_stream_info.cq_index = stream_info->cq_index;
	ffrt_stream_info.id = stream_info->id;

	hwts_sq_info = npu_calc_hwts_sq_info(0, ffrt_stream_info.sq_index);
	cond_return_error(hwts_sq_info == NULL, -1, "stream= %d, sq_index= %u, hwts_sq_info is null\n",
		ffrt_stream_info.id, ffrt_stream_info.sq_index);

	sq_iova_addr = hwts_sq_info->iova_addr;
	cond_return_error(sq_iova_addr == 0, -1, "iova_addr is empty: stream= %d, sq_index= %u\n",
		ffrt_stream_info.id, ffrt_stream_info.sq_index);

	hwts_cq_info = npu_calc_hwts_cq_info(0, ffrt_stream_info.cq_index);
	cond_return_error(hwts_cq_info == NULL, -1, "stream= %d, cq_index= %u, hwts_cq_info is null\n",
		ffrt_stream_info.id, ffrt_stream_info.cq_index);

	cq_iova_addr = hwts_cq_info->iova_addr;
	cond_return_error(cq_iova_addr == 0, -1, "cq_iova_addr is empty: stream= %d, cq_index= %u\n",
		ffrt_stream_info.id, ffrt_stream_info.cq_index);

	/* sq base addr */
	ffrt_swap_buf_index->sq_base_addr0.reg.rtsq_base_addr_l = sq_iova_addr & UINT32_MAX;
	ffrt_swap_buf_index->sq_base_addr1.reg.rtsq_base_addr_h = (sq_iova_addr >> 32) & MAX_UINT16_NUM;
	ffrt_swap_buf_index->sq_base_addr1.reg.rtsq_base_addr_is_virtual = 1;
	/* sq cfg, reserved for config, ssid, rtsq_rtcqid */
	ffrt_swap_buf_index->sq_cfg0.reg.rtsq_smmu_substream_id = model_desc->ssid;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_rtcqid = ffrt_stream_info.cq_index; // reserved cq id
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_log_bufid = 0;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_profile_bufid = 0;
#ifdef AI_SUPPORT_NPU_HIPERF_TRACE_ENABLE
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_log_task_en = 1;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_log_block_en = 1;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_profile_task_en = 1;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_profile_block_en = 1;
#else
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_log_task_en = 0;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_log_block_en = 0;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_profile_task_en = 0;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_profile_block_en = 0;
#endif
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_rsp_mode = 0;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_sat_mode = 0;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_ban_wr_cqe = 1;
	ffrt_swap_buf_index->sq_cfg1.reg.rtsq_smmu_two_stage_en = 0;
	ffrt_swap_buf_index->sq_cfg2.reg.rtsq_aiv_poolid = 0;
	ffrt_swap_buf_index->sq_cfg2.reg.rtsq_gen_aic_poolid = 0;
	ffrt_swap_buf_index->sq_cfg2.reg.rtsq_eff_aic_poolid = 0;
	ffrt_swap_buf_index->sq_cfg4.reg.rtsq_length = model_desc->stream_tasks[swap_buf_index];
	/* cq cfg  */
	ffrt_swap_buf_index->cq_base_addr0.reg.rtcq_base_addr_l = cq_iova_addr & UINT32_MAX;
	ffrt_swap_buf_index->cq_base_addr1.reg.rtcq_base_addr_h = (cq_iova_addr >> 32) & UINT32_MAX;
	ffrt_swap_buf_index->cq_base_addr1.reg.rtcq_base_addr_is_virtual = 1;
	ffrt_swap_buf_index->cq_cfg0.reg.rtcq_length = 1024;
	ffrt_swap_buf_index->sq_context_switch_ctrl0.reg.rtsq_context_switch_buf_base_addr_l =
		(uint32_t)(model_desc->aiv_csw_buff_addr[swap_buf_index] & UINT32_MAX);
	ffrt_swap_buf_index->sq_context_switch_ctrl1.reg.rtsq_context_switch_buf_base_addr_h =
		(uint16_t)(model_desc->aiv_csw_buff_addr[swap_buf_index] >> 32);
	npu_drv_info("sq_base_addr 0x%lx 0x%lx, cfg0 0x%lx, cfg1 0x%lx, cfg3 0x%lx "
		"cq_base_addr 0x%lx 0x%lx, cfg0 0x%lx",
		ffrt_swap_buf_index->sq_base_addr0.value, ffrt_swap_buf_index->sq_base_addr1.value,
		ffrt_swap_buf_index->sq_cfg0.value, ffrt_swap_buf_index->sq_cfg1.value, ffrt_swap_buf_index->sq_cfg3.value,
		ffrt_swap_buf_index->cq_base_addr0.value, ffrt_swap_buf_index->cq_base_addr1.value,
		ffrt_swap_buf_index->cq_cfg0.value);

	return 0;
}
