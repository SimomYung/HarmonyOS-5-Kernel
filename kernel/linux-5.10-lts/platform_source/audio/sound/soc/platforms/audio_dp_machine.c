/*
 * audio_dp_machine.c
 *
 * audio dp machine asoc driver
 *
 * Copyright (c) 2015-2020 Huawei Technologies Co., Ltd.
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

#include <linux/module.h>
#include <sound/soc.h>
#include <linux/version.h>

#include "machine_utils.h"
#include "audio_log.h"

#define LOG_TAG "audio_dp_machine"

SND_SOC_DAILINK_DEFS(audio_dp_dai_link,
	DAILINK_COMP_ARRAY(COMP_CPU("snd-soc-dummy-dai")),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "audio-dp-dai")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("audio-pcm-dp")));

static struct snd_soc_dai_link g_audio_dp_dai_link[] = {
	{
		/* dai link name */
		.name = "audio-dp-dai-link",
		/* stream name same as name */
		.stream_name = "dp",
		SND_SOC_DAILINK_REG(audio_dp_dai_link),
	},
};

/* Audio dp machine driver */
static struct snd_soc_card g_snd_soc_audio_dp = {
	.name = "audio-dp",
	.owner = THIS_MODULE,
	.dai_link = g_audio_dp_dai_link,
	.num_links = ARRAY_SIZE(g_audio_dp_dai_link),
};

static int audio_dp_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &g_snd_soc_audio_dp;
	int ret;
	const char *card_name = NULL;

	if (!pdev) {
		AUDIO_LOGE("enter with null pointer, fail");
		return -EINVAL;
	}

	AUDIO_LOGI("in");

	card->dev = &pdev->dev;

	ret = of_property_read_string(pdev->dev.of_node, "card_name", &card_name);
	if (ret == 0) {
		AUDIO_LOGI("dp card name: %s in dt node", card_name);
		card->name = card_name;
	}
	set_dlc_of_node(card, pdev->dev.of_node);
	ret = snd_soc_register_card(card);
	if (ret != 0) {
		AUDIO_LOGE("snd soc register card failed, ret: %d", ret);
		return ret;
	}

	AUDIO_LOGI("out");

	return 0;
}

static int audio_dp_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	AUDIO_LOGI("in");

	if (card)
		snd_soc_unregister_card(card);

	AUDIO_LOGI("out");

	return 0;
}

static const struct of_device_id g_audio_dp_of_match[] = {
	{ .compatible = "hisilicon,audio-dp-machine" },
	{ } /* sentinel */
};

MODULE_DEVICE_TABLE(of, audio_dp_dt_ids);

static struct platform_driver g_audio_dp_driver = {
	.driver = {
		.name = "audio-dp-machine",
		.owner = THIS_MODULE,
		.of_match_table = g_audio_dp_of_match,
	},
	.probe = audio_dp_probe,
	.remove = audio_dp_remove,
};

static int __init audio_dp_init(void)
{
	AUDIO_LOGI("dp init");
	return platform_driver_register(&g_audio_dp_driver);
}
late_initcall(audio_dp_init);

static void __exit audio_dp_exit(void)
{
	AUDIO_LOGI("dp exit");
	platform_driver_unregister(&g_audio_dp_driver);
}
module_exit(audio_dp_exit);

/* Module information */
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("DP machine ASoC driver");
