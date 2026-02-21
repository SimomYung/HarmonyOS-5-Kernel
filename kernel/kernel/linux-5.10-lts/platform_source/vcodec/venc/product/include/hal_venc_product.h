/*
 * hal_venc_product.h
 *
 * This is for venc product common declaration 
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __HAL_VENC_PRODUCT_H__
#define __HAL_VENC_PRODUCT_H__

#include "vcodec_type.h"
#include "drv_venc_ioctl.h"
#include "drv_venc_efl.h"

// implement in common
void venc_hal_clr_all_int(S_HEVC_AVC_REGS_TYPE *vedu_reg);
void venc_hal_disable_all_int(S_HEVC_AVC_REGS_TYPE *vedu_reg);
void venc_hal_start_encode(S_HEVC_AVC_REGS_TYPE *vedu_reg);
void venc_hal_get_reg_venc(struct stream_info *stream_info, uint32_t *reg_base);
bool venc_device_need_bypass(void);

// implement in each product
int32_t vedu_hal_check_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, venc_buffer_info_t *mem_info);
#ifdef VENC_MCORE_ENABLE
void venc_mcore_start_encode(struct encode_info *info, S_HEVC_AVC_REGS_TYPE *vedu_reg);
#endif
#ifdef VENC_MUTI_REF_SUPPORT
void venc_hal_get_pic_stat(struct picture_stats *picture_info, uint32_t *reg_base);
void venc_hal_printf_pic_stat(struct picture_stats *picture_info, uint32_t *reg_base, uint32_t id, uint32_t frame);
#endif
#ifdef SLICE_INT_EN
void venc_hal_get_slice_reg(struct stream_info *stream_info, uint32_t *reg_base);
#endif

#endif // __HAL_VENC_PRODUCT_H__
