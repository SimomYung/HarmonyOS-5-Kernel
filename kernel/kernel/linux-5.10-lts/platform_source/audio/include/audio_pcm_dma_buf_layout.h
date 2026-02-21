/*
 * audio_pcm_dma_buf_layout.h -- audio pcm dma buf layout info
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
 *
 */

#ifndef __AUDIO_PCM_DMA_BUF_LAYOUT_H__
#define __AUDIO_PCM_DMA_BUF_LAYOUT_H__

#include "audio_common_msg.h"

#if defined (CONFIG_SND_SOC_CDC_PCM)
#include "cdc_pcm_dma_buf_layout.h"
#elif defined (CONFIG_SND_SOC_PC_PCM)
#include "pc_pcm_dma_buf_layout.h"
#else
#include "phone_pcm_dma_buf_layout.h"
#endif

#endif /* __AUDIO_PCM_DMA_BUF_LAYOUT_H__ */
