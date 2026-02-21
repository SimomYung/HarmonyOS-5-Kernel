/*
 * vfmw_pdt.h
 *
 * This is for product global interface.
 *
 * Copyright (c) 2017-2020 Huawei Technologies CO., Ltd.
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

#ifndef PRODUCT_H
#define PRODUCT_H
#include "vcodec_types.h"

#define VFMW_ALIGN_UP(val, align) (((uint32_t)((val) + (align)-1)) & (~((uint32_t)((align)-1))))
#define VFMW_MEM_ALIGN 0x1000
uint32_t dec_hal_calc_mem_size(void);

#endif
