/*
 * npu_resmem.c
 *
 * about npu resmem
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
#include "npu_resmem.h"

#include <linux/of.h>

#include "npu_log.h"
#include "npu_ddr_map.h"
#include "npu_profiling.h"
#include "npu_common_resource.h"

static void npu_plat_set_resmem_desc(
	struct npu_mem_desc *resmem_desc, u64 base, u32 len)
{
	if (resmem_desc != NULL) {
		resmem_desc->base = base;
		resmem_desc->len = (u64)len;
	}
}

static void npu_plat_set_resmem_info(struct npu_resmem_info *resmem_info)
{
	u64 base_addr = resmem_info->resmem_desc[NPU_CTRL_INFO_IDX].base;
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_CTRL_INFO_IDX]),
		base_addr, NPU_NS_CTRL_INFO_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_BBOX_MEM_IDX]),
		(base_addr + NPU_NS_BBOX_BASE_OFFSET), NPU_NS_BBOX_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_LOG_MEM_IDX]),
		(base_addr + NPU_NS_LOG_BASE_OFFSET), NPU_NS_LOG_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_TSCPU_FW_IDX]),
		(base_addr + NPU_NS_CTRL_INFO_BASE_OFFSET), NPU_NS_TSCPU_FW_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_COMM_SQCQ_RESMEM_IDX]),
		(base_addr + NPU_NS_COMM_SQCQ_BASE_OFFSET), NPU_NS_COMM_SQCQ_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_DFX_SQCQ_RESMEM_IDX]),
		(base_addr + NPU_NS_DFX_SQCQ_BASE_OFFSET), NPU_NS_DFX_SQCQ_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_TS_CONFIG_RESMEM_IDX]),
		(base_addr + NPU_NS_TS_CONFIG_BASE_OFFSET), NPU_NS_TS_CONFIG_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_NPU_CFG_IDX]),
		(base_addr + NPU_NS_NPU_CONFIG_BASE_OFFSET), NPU_NS_NPU_CONFIG_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_PROF_MEM_IDX]),
		(base_addr + NPU_PROFILINGL_INFO_OFFSET), NPU_NS_PROF_SIZE);
#ifndef CONFIG_NPU_SINK_UDK
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_FFRT_MDC_IDX]),
		(base_addr + NPU_NS_HWTS_FFRT_MDC_BUFF_BASE_OFFSET), NPU_NS_HWTS_FFRT_MDC_BUFF_SIZE);
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_FFRT_SWAP_BUFF_IDX]),
		(base_addr + NPU_NS_HWTS_FFRT_SWAP_BUFF_BASE_OFFSET), NPU_NS_HWTS_FFRT_SWAP_BUFF_SIZE);
#endif
#ifdef CONFIG_NPU_ISPNN_INFO
	npu_plat_set_resmem_desc(&(resmem_info->resmem_desc[NPU_ISPNN_INFO_BUFF_IDX]),
		(base_addr + NPU_NS_ISPNN_INFO_BUFF_BASE_OFFSET), NPU_NS_ISPNN_INFO_SIZE);
#endif
}

int npu_plat_set_resmem(struct npu_platform_info *plat_info)
{
	struct npu_resmem_info *resmem_info = &(plat_info->resmem_info);

	npu_plat_set_resmem_info(resmem_info);
	resmem_info->info_buf = &(resmem_info->resmem_desc[NPU_CTRL_INFO_IDX]);
#ifndef CONFIG_NPU_SWTS
	resmem_info->info_buf = &(resmem_info->resmem_desc[NPU_CTRL_INFO_IDX]);
	resmem_info->sqcq_buf = &(resmem_info->resmem_desc[NPU_COMM_SQCQ_RESMEM_IDX]);
	resmem_info->npu_cfg_buf = &(resmem_info->resmem_desc[NPU_NPU_CFG_IDX]);
#if defined(CONFIG_NPU_FFRT) && !defined(CONFIG_NPU_SINK_UDK)
	resmem_info->ffrt_mdc_buf = &(resmem_info->resmem_desc[NPU_FFRT_MDC_IDX]);
	resmem_info->ffrt_swap_buf = &(resmem_info->resmem_desc[NPU_FFRT_SWAP_BUFF_IDX]);
#endif
#ifdef CONFIG_NPU_ISPNN_INFO
	resmem_info->ispnn_info_buf = &(resmem_info->resmem_desc[NPU_ISPNN_INFO_BUFF_IDX]);
#endif
#else
	resmem_info->hwts_swap_buf = &(resmem_info->resmem_desc[NPU_SWAP_BUF_IDX]);
	resmem_info->sqcq_buf = &(resmem_info->resmem_desc[NPU_NS_HWTS_SQ_IDX]);
#endif
	return 0;
}
