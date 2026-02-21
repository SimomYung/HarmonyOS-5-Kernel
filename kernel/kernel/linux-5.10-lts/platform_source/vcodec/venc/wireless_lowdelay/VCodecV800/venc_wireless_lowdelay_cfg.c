/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include "venc_wireless_lowdelay_cfg.h"

void venc_cfg_qos_layer_info(uint64_t data_addr, u_qos_layer_info *qos_layer_info)
{
	uint8_t qos;
	qos = *(uint8_t *)(uintptr_t)(data_addr + MSDU_QOS_START_OFFSET);
	qos_layer_info->bits.slice_idx = qos & 0xf0;
	qos = *(uint8_t *)(uintptr_t)(data_addr + MSDU_QOS_START_OFFSET + 1);
	qos_layer_info->bits.is_last_package = qos & 0x1;
	qos_layer_info->bits.version = 1;
}

void venc_cfg_layer_stream_addr_base_info(const struct enhance_layer_info *info, uint32_t *reg_base)
{
	unused(info);
	unused(reg_base);
}

void venc_cfg_enhance_layer_intra_inter_cu_en(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	U_VEDU_VCPI_INTRA_INTER_CU_EN d32 = channel_cfg->all_reg.VEDU_VCPI_INTRA_INTER_CU_EN;
	d32.bits.vcpi_mrg_cu_en = 0; // enhance layer close inquire serch
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_INTRA_INTER_CU_EN), d32.u32);
}