/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#ifndef VENC_WIRELESS_CONFIG_H
#define VENC_WIRELESS_CONFIG_H
#include "drv_venc_ioctl.h"
#include "hal_venc.h"
#include "drv_venc_wireless.h"

#define MSDU_LEN_START_OFFSET 82
#define NEXT_PACKAGE_MSDU_OFFSET 96
void venc_cfg_qos_layer_info(uint64_t data_addr, u_qos_layer_info *qos_layer_info);
void venc_cfg_layer_stream_addr_base_info(const struct enhance_layer_info *info, uint32_t *reg_base);
void venc_cfg_enhance_layer_intra_inter_cu_en(struct encode_info *channel_cfg, uint32_t *reg_base);
#endif