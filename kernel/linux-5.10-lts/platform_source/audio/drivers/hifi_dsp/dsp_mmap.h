/*
 * head of dsp mmap.c
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#ifndef __DSP_MMAP_H__
#define __DSP_MMAP_H__

#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/mm.h>

#include "audio_common_msg.h"

enum dsp_mmap_type {
	DSP_MMAP_TYPE_BASE,
	DSP_MMAP_TYPE_OFFLOAD,
#ifdef CONFIG_HIFI_SPATIAL_AUDIO_SUPPORT
	DSP_MMAP_TYPE_HVS,
#endif
#ifdef CONFIG_HIFI_EXTEND_MISC
	DSP_MMAP_TYPE_MMAP,
	DSP_MMAP_TYPE_MMAP_MAX = DSP_MMAP_TYPE_MMAP + MAX_MMAP_STREAM_NUM - 1,
#endif
#ifdef CONFIG_HIFI_AUDIO_NN_SUPPORT
	DSP_MMAP_TYPE_AUDIO_NN,
#endif
	DSP_MMAP_TYPE_FASTBOOT_CMA_MANAGER,
	DSP_MMAP_TYPE_BUTT
};

unsigned long dsp_misc_get_phys(void);
#ifdef CONFIG_HIFI_EXTEND_MISC
unsigned long dsp_misc_get_mmap_phys(unsigned int mmap_type);
#endif
int dsp_misc_mmap(struct file *file, struct vm_area_struct *vma);
int dsp_misc_mmap_common(struct file *file, struct vm_area_struct *vma, unsigned long offset);
void dsp_misc_mmap_probe(struct platform_device *pdev, uintptr_t base_phy);
void dsp_misc_mmap_remove(void);
int dsp_misc_spatial_audio_proc_sync(const void *para_in, unsigned int para_size_in);

#endif /* __DSP_MISC_H__ */

