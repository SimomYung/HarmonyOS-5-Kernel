/*
 * machine_utils.h
 *
 * machine pulic function
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

#ifndef __MACHINE_UTILS_H__
#define __MACHINE_UTILS_H__

#include <sound/soc.h>

/* following name of dai_link_component must is same with dt node define */
#define DLC_PLATFORM_NAME_HIFI     "asp_pcm_hifi"
#define DLC_CODEC_NAME_ANA         "codec_ana"

void set_dlc_of_node(struct snd_soc_card *card, void *np);

#endif /* __MACHINE_UTILS_H__ */