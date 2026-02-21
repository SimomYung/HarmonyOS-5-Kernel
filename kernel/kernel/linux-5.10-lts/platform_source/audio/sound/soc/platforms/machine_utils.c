/*
 * machine_utils.c
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

#include "machine_utils.h"

#include <linux/of.h>
#include <sound/pcm.h>

#include "audio_log.h"

#define LOG_TAG "machine_utils"

void set_dlc_of_node(struct snd_soc_card *card, void *np)
{
	struct device_node *codec_of_node = NULL;
	struct device_node *pcm_of_node = NULL;
	struct snd_soc_dai_link *dai_link = NULL;
	int i = 0;
    if (card == NULL || np == NULL) {
        AUDIO_LOGE("card or np is NULL");
        return;
    }

	codec_of_node = of_parse_phandle(np, "codec-handle", 0);
	pcm_of_node = of_parse_phandle(np, "pcm-handle", 0);

	/* 如果没有定义dai_link->name,则使用dt中的of_node。有name的其节点名称都是固定的 */
	for_each_card_prelinks(card, i, dai_link) {
		if(dai_link->codecs->name == NULL) {
			dai_link->codecs->of_node = codec_of_node;
		} else {
			AUDIO_LOGD("codec name:%s", dai_link->codecs->name);
		}

		if(dai_link->platforms->name == NULL) {
			dai_link->platforms->of_node = pcm_of_node;
		} else {
			AUDIO_LOGD("platforms name:%s", dai_link->platforms->name);
		}
	}
}