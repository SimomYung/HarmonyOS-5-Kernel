/*
 * dsp_extend_misc.h
 *
 * dsp extend misc driver.
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

#ifndef __DSP_EXTEND_MISC_H__
#define __DSP_EXTEND_MISC_H__

#include <linux/types.h>

#define DTS_COMP_HIFIDSP_EXTEND_NAME "hisilicon,hifidspextend"

#define AAUDIO_MMAP_SIZE 0x2000
#define AAUDIO_MMAP_VOIP_SIZE_16K 0x3000
#define AAUDIO_MMAP_MAX_SIZE 0x8000

bool is_mmap_size(unsigned long size);

#endif
