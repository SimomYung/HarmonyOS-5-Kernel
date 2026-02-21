/*
 * npu_ffts_model.h
 *
 * about npu ffts model
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */
#ifndef __NPU_FFTS_MODEL_H
#define __NPU_FFTS_MODEL_H

#include "npu_osal.h"
#include "npu_soc_defines.h"
#include "npu_model_description.h"

struct npu_ffrt_model_buff* get_ffrt_mdc_buf(uint32_t model_id);
struct npu_hwts_static_swap_buff* get_ffrt_swap_buf_phy(uint32_t model_id);
struct npu_hwts_static_swap_buff* get_ffrt_swap_buf_index(int swap_buf_index, uint32_t model_id);
int npu_ffrt_load_model_buf(struct npu_ffrt_load_model_buf_info *model_desc);
int npu_ffrt_set_ssid(uint32_t model_id, uint16_t ssid);

#endif