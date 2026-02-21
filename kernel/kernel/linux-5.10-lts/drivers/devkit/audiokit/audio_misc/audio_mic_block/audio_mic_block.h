/*
 * audio_mic_block.h
 *
 * audio_mic_block driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __AUDIO_MIC_BLOCK_H__
#define __AUDIO_MIC_BLOCK_H__

#include "../audio_misc.h"

#define AUDIO_MIC_BLOCK_IOCTL_TYPE           'I'
#define AUDIO_MIC_BLOCK_NOTIFIER              _IOW(AUDIO_MIC_BLOCK_IOCTL_TYPE, 0x01, int)

#ifdef CONFIG_AUDIO_MIC_BLOCK
int audio_mic_block_probe(struct device *dev, struct device_node *np);
#else
static inline int audio_mic_block_probe(struct device *dev, struct device_node *np)
{
    return 0;
}
#endif  // CONFIG_AUDIO_MIC_BLOCK

#endif  // __AUDIO_MIC_BLOCK_H__