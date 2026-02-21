/*
 * npu_ffts_model.c
 *
 * about npu ffts model
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */

#include "npu_ffts_model.h"

#include "npu_common_resource.h"
#include "npu_ddr_map.h"
#include "npu_arch_ffts_model.h"
#include "npu_log.h"
#include "npu_platform_resource.h"
#include "npu_user_common.h"

#ifdef CONFIG_HISI_NPU_UDK
#include "npu_platform.h"

struct npu_ffrt_model_buff* get_ffrt_mdc_buf(uint32_t model_id)
{
	struct npu_platform_info *plat_info = npu_plat_get_info();
	cond_return_error(plat_info == NULL, NULL, "get platform info failed\n");
	return (struct npu_ffrt_model_buff *)(uintptr_t)plat_info->resmem_desc[NPU_FFRT_MDC_IDX].virt_addr + model_id;
}

struct npu_hwts_static_swap_buff* get_ffrt_swap_buf_phy(uint32_t model_id)
{
	struct npu_platform_info *plat_info = npu_plat_get_info();
	cond_return_error(plat_info == NULL, NULL, "get platform info failed\n");
	return (struct npu_hwts_static_swap_buff *)(uintptr_t)plat_info->resmem_desc[NPU_FFRT_SWAP_BUFF_IDX].phy_addr +
		(NPU_FFRT_MODEL_STREAM_NUM_MAX * model_id);
}

struct npu_hwts_static_swap_buff* get_ffrt_swap_buf_index(int swap_buf_index, uint32_t model_id)
{
	struct npu_platform_info *plat_info = npu_plat_get_info();
	cond_return_error(plat_info == NULL, NULL, "get platform info failed\n");
	return (struct npu_hwts_static_swap_buff *)(uintptr_t)plat_info->resmem_desc[NPU_FFRT_SWAP_BUFF_IDX].virt_addr
		+ (NPU_FFRT_MODEL_STREAM_NUM_MAX * model_id) + swap_buf_index;
}
#else
#include "npu_shm.h"

struct npu_ffrt_model_buff* get_ffrt_mdc_buf(uint32_t model_id)
{
	return (struct npu_ffrt_model_buff *)(uintptr_t)get_shm_virt_addr(0, NPU_FFRT_MDC_MEM) + model_id;
}

struct npu_hwts_static_swap_buff* get_ffrt_swap_buf_phy(uint32_t model_id)
{
	return (struct npu_hwts_static_swap_buff *)(uintptr_t)get_shm_phy_addr(0, NPU_FFRT_SWAP_MEM) +
		(NPU_FFRT_MODEL_STREAM_NUM_MAX * model_id);
}

struct npu_hwts_static_swap_buff* get_ffrt_swap_buf_index(int swap_buf_index, uint32_t model_id)
{
	return (struct npu_hwts_static_swap_buff *)(uintptr_t)get_shm_virt_addr(0, NPU_FFRT_SWAP_MEM) +
		(NPU_FFRT_MODEL_STREAM_NUM_MAX * model_id) + swap_buf_index;
}
#endif

static int npu_verify_ffrt_model_desc_info(struct npu_ffrt_load_model_buf_info *model_desc)
{
	u32 idx = 0;
	if (model_desc->stream_cnt > NPU_FFRT_MODEL_STREAM_NUM_MAX || model_desc->stream_cnt > NPU_MODEL_STREAM_NUM ||
		model_desc->stream_cnt == 0) {
		npu_drv_err("stream_cnt = %u invalid\n", model_desc->stream_cnt);
		return -1;
	}

	if (model_desc->model_id >= NPU_MAX_MODEL_ID) {
		npu_drv_err("illegal model_id= %u\n", model_desc->model_id);
		return -1;
	}

	if (model_desc->priority > MAX_PRIORITY_LEVEL) {
		npu_drv_err("invalid priority(%u)\n", model_desc->priority);
		return -1;
	}

	for (idx = 0; idx < model_desc->stream_cnt; idx++) {
		if (model_desc->stream_id[idx] < NPU_MAX_NON_SINK_STREAM_ID ||
			model_desc->stream_id[idx] >= NPU_MAX_STREAM_ID) {
			npu_drv_err("user sink stream_id[%u]= %u invalid\n", idx, model_desc->stream_id[idx]);
			return -1;
		}

		if ((model_desc->stream_id[idx] < (NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_LONG_HWTS_SQ_NUM)) &&
			(model_desc->stream_tasks[idx] > NPU_MAX_LONG_HWTS_SQ_DEPTH || model_desc->stream_tasks[idx] == 0)) {
			npu_drv_err("user sink long stream_tasks[%u]= %u invalid\n",
				idx, model_desc->stream_tasks[idx]);
			return -1;
		}

		if ((model_desc->stream_id[idx] >= (NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_LONG_HWTS_SQ_NUM)) &&
			(model_desc->stream_tasks[idx] > NPU_MAX_HWTS_SQ_DEPTH || model_desc->stream_tasks[idx] == 0)) {
			npu_drv_err("user sink stream_tasks[%u]= %u invalid\n",
				idx, model_desc->stream_tasks[idx]);
			return -1;
		}
	}

	return 0;
}

static int npu_ffrt_load_swap_buf(struct npu_ffrt_load_model_buf_info *model_desc)
{
	int ret = 0;
	int i = 0;
	for (i = 0; i < model_desc->stream_cnt; i++) {
		ret = npu_ffrt_load_swap_buf_index(model_desc, i);
		cond_return_error(ret != 0, -1, "load swap_buf_%d failed", i);
	}
	return 0;
}

int npu_ffrt_load_model_buf(struct npu_ffrt_load_model_buf_info *model_desc)
{
	int ret = 0;

	ret = npu_verify_ffrt_model_desc_info(model_desc);
	cond_return_error(ret != 0, -1, "verify ffrt model desc info failed");

	ret = npu_ffrt_load_mdc_buf(model_desc);
	cond_return_error(ret != 0, -1, "load mdc_buf failed");

	ret = npu_ffrt_load_swap_buf(model_desc);
	cond_return_error(ret != 0, -1, "load swap_buf failed");

	return ret;
}

int npu_ffrt_set_ssid(uint32_t model_id, uint16_t ssid)
{
#ifdef CONFIG_HISI_NPU_UDK
	struct npu_platform_info *plat_info = npu_plat_get_info();
	struct npu_ffrt_model_buff* ffrt_mdc_buf =
		(struct npu_ffrt_model_buff *)(uintptr_t)plat_info->resmem_desc[NPU_FFRT_MDC_IDX].virt_addr + model_id;
#else
	struct npu_ffrt_model_buff* ffrt_mdc_buf =
		(struct npu_ffrt_model_buff *)(uintptr_t)get_shm_virt_addr(0, NPU_FFRT_MDC_MEM) + model_id;
#endif

	cond_return_error(ffrt_mdc_buf == NULL, -EINVAL, "get ffrt_mdc_buf fail\n");

	ffrt_mdc_buf->ssid = ssid;

	return 0;
}