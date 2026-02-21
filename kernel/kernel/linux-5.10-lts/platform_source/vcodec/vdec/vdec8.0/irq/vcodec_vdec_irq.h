/*
 * vcodec_vdec_irq.h
 *
 * This is irq interface declares
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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
#ifndef VCODEC_VDEC_IRQ_H
#define VCODEC_VDEC_IRQ_H
#include "vcodec_types.h"
#include "vfmw_ext.h"

struct vcodec_vdec_irq_ctx;

struct vcodec_vdec_irq_ctx* vcodec_vdec_irq_request_ctx(vdec_dts *dts_info, uint8_t *reg_base);
void vcodec_vdec_irq_free_ctx(struct vcodec_vdec_irq_ctx* ctx);
void vcodec_vdec_enable_irq(vdec_norm_irq_num irq_num);
void vcodec_vdec_disable_irq(vdec_norm_irq_num irq_num);

#endif
