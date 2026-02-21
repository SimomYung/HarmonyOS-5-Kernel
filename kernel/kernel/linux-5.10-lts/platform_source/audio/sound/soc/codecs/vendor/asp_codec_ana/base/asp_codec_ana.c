/*
 * asp_codec_ana.c -- codec driver
 *
 * Copyright (c) 2021 Huawei Technologies Co., Ltd.
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

#include "asp_codec_ana.h"

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/soc-dapm.h>
#include <sound/tlv.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>


#include "audio_log.h"
#include "asoc_adapter.h"
#include "asp_codec_ana_reg_def.h"
#include "asp_codec_ana_comm.h"
#include "asp_codec_ana_utils.h"

#define LOG_TAG "asp_codec_ana"

static struct codec_ana_data *g_ana_codec;

#define PB_MIN_CHANNELS                    1
#define PB_MAX_CHANNELS                    2
#define CP_MIN_CHANNELS                    1
#define CP_MAX_CHANNELS                    4
#define LDO8_VOLTAGE                       1800000

#define PCM_FORMATS     (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE \
                        | SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_S32_LE)
#define PCM_RATES       SNDRV_PCM_RATE_8000_48000

static void codec_ana_priv_deinit(struct snd_soc_component *codec)
{
	struct codec_ana_data *priv = snd_soc_component_get_drvdata(codec);
	if (priv == NULL || priv->codec_ops.ana_priv_data_deinit == NULL || priv->v_codec_reg == NULL)
		return;

	priv->codec_ops.ana_priv_data_deinit(codec);
	kfree(priv->v_codec_reg);
	priv->v_codec_reg = NULL;
	priv->codec = NULL;
	mutex_destroy(&priv->ibias.ibias_mutex);
	snd_soc_component_set_drvdata(codec, NULL);
	AUDIO_LOGI("priv deinit ok");
}

static int codec_ana_priv_init(struct snd_soc_component *codec)
{
	struct codec_ana_data *priv = g_ana_codec;

	if (priv == NULL) {
		AUDIO_LOGE("priv is null");
		return -EINVAL;
	}

	snd_soc_component_set_drvdata(codec, priv);
	mutex_init(&priv->ibias.ibias_mutex);
	spin_lock_init(&priv->lock);
	priv->codec = codec;
	priv->v_codec_reg = kzalloc(sizeof(unsigned int) * priv->v_codec_num, GFP_KERNEL);
	if (priv->v_codec_reg == NULL) {
		AUDIO_LOGE("memory alloc error");
		mutex_destroy(&priv->ibias.ibias_mutex);
		priv->codec = NULL;
		return -ENOMEM;
	}

	if (priv->codec_ops.ana_priv_data_init != NULL)
		priv->codec_ops.ana_priv_data_init(codec);

	AUDIO_LOGI("priv init ok");

	return 0;
}

static int ana_resource_init(struct snd_soc_component *codec)
{
	int ret;
	struct codec_ana_data *priv = NULL;

	if (codec == NULL) {
		AUDIO_LOGE("codec is null");
		return -EINVAL;
	}

	priv = snd_soc_component_get_drvdata(codec);
	if (priv == NULL) {
		AUDIO_LOGE("priv is null");
		return -EINVAL;
	}

	priv->pmu_regulator = devm_regulator_get(codec->dev, "codec_ldo8");
	if (IS_ERR(priv->pmu_regulator)) {
		AUDIO_LOGE("couldn't get pmu regulator ret: %ld", PTR_ERR(priv->pmu_regulator));
		ret = PTR_ERR(priv->pmu_regulator);
		priv->pmu_regulator = NULL;
		return -EINVAL;
	}

	ret = regulator_set_voltage(priv->pmu_regulator, LDO8_VOLTAGE, LDO8_VOLTAGE);
	if (ret) {
		AUDIO_LOGE("regulator set pmu regulator voltage failed");
		return -EINVAL;
	}

	ret = regulator_enable(priv->pmu_regulator);
	if (ret) {
		AUDIO_LOGE("couldn't enable pmu regulator %d", ret);
		return -EINVAL;
	}

	if (priv->codec_ops.ana_reset_codec != NULL)
		priv->codec_ops.ana_reset_codec(codec);
	if (priv->codec_ops.ana_reg_init != NULL)
		priv->codec_ops.ana_reg_init(codec);

	return 0;
}

static int add_platform_dapm_path(struct snd_soc_component *codec)
{
	struct codec_ana_data *ana_priv = g_ana_codec;
	struct snd_soc_dapm_context *dapm = NULL;
	int ret = 0;

	dapm = snd_soc_component_get_dapm(codec);
	if (dapm == NULL) {
		AUDIO_LOGE("dapm is null");
		return -EINVAL;
	}

	ret = snd_soc_dapm_new_controls(dapm, ana_priv->codec_resource->widgets,
		ana_priv->codec_resource->widgets_num);
	if (ret != 0) {
		AUDIO_LOGE("add widget failed, ret: %d", ret);
		return ret;
	}

	ret = snd_soc_add_component_controls(codec, ana_priv->codec_resource->controls,
		ana_priv->codec_resource->controls_num);
	if (ret != 0) {
		AUDIO_LOGE("add control failed, ret: %d", ret);
		return ret;
	}

	ret = snd_soc_dapm_add_routes(dapm, ana_priv->codec_resource->routes,
		ana_priv->codec_resource->routes_num);
	if (ret != 0) {
		AUDIO_LOGE("add path failed, ret: %d", ret);
		return ret;
	}

	return 0;
}

static void ana_resource_deinit(struct snd_soc_component *codec)
{
	int ret;
	struct codec_ana_data *priv = snd_soc_component_get_drvdata(codec);

	if (!IS_ERR(priv->pmu_regulator)) {
		ret = regulator_disable(priv->pmu_regulator);
		if (ret)
			AUDIO_LOGE("couldn't disable pmu %d", ret);
	}
}

static int codec_ana_probe(struct snd_soc_component *codec)
{
	int ret;
	struct codec_ana_data *priv = NULL;
	if (codec == NULL) {
		AUDIO_LOGE("codec is null");
		return -EINVAL;
	}

	ret = codec_ana_priv_init(codec);
	if (ret) {
		AUDIO_LOGE("ana priv init failed, ret: %d", ret);
		return ret;
	}

	ret = ana_resource_init(codec);
	if (ret) {
		AUDIO_LOGE("set ana info err");
		goto resource_init_err;
	}

	priv = snd_soc_component_get_drvdata(codec);

	ret = add_platform_dapm_path(codec);
	if (ret) {
		AUDIO_LOGE("set ana info err");
		goto add_path_err;
	}


	AUDIO_LOGI("ana probe ok");
	return 0;

add_path_err:
mbhc_init_err:
	ana_resource_deinit(codec);
resource_init_err:
	codec_ana_priv_deinit(codec);

	return ret;
}

static void codec_ana_remove(struct snd_soc_component *codec)
{
	struct codec_ana_data *priv = NULL;
	if (codec == NULL) {
		AUDIO_LOGE("codec is null");
		return;
	}

	priv = snd_soc_component_get_drvdata(codec);

	ana_resource_deinit(codec);
	codec_ana_priv_deinit(codec);
}

static struct snd_soc_dai_driver ana_codec_dai[] = {
	{
		.name = "ana-codec-dai",
		.playback = {
			.stream_name = "Playback",
			.channels_min = PB_MIN_CHANNELS,
			.channels_max = PB_MAX_CHANNELS,
			.rates = PCM_RATES,
			.formats = PCM_FORMATS
		},
		.capture = {
			.stream_name = "Capture",
			.channels_min = CP_MIN_CHANNELS,
			.channels_max = CP_MAX_CHANNELS,
			.rates = PCM_RATES,
			.formats = PCM_FORMATS
		},
	},
};

static struct snd_soc_component_driver ana_codec_driver = {
	.name = "ana-codec",
	.probe = codec_ana_probe,
	.remove = codec_ana_remove,
	.read = asp_codec_ana_reg_read,
	.write = asp_codec_ana_reg_write,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	.idle_bias_on = false,
#else
	.idle_bias_off = true
#endif
};

static int codec_ana_platform_probe(struct platform_device *pdev)
{
	int ret;

	ret = devm_snd_soc_register_component(&pdev->dev, &ana_codec_driver,
		ana_codec_dai, ARRAY_SIZE(ana_codec_dai));

	return ret;
}

static int codec_ana_platform_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id g_ana_codec_of_match[] = {
	{ .compatible = "hisilicon,ana-codec", },
	{ },
};

static struct platform_driver g_ana_codec_platform_driver = {
	.driver = {
		.name           = "ana-codec",
		.owner          = THIS_MODULE,
		.of_match_table = of_match_ptr(g_ana_codec_of_match),
	},
	.probe  = codec_ana_platform_probe,
	.remove = codec_ana_platform_remove,
};

void asp_ana_codec_driver_init(struct codec_ana_data *base_data)
{
	int ret;

	if (base_data == NULL) {
		AUDIO_LOGE("codec data is null");
		return;
	}

	g_ana_codec = base_data;
	if (base_data->codec_ops.shutdown != NULL)
		g_ana_codec_platform_driver.shutdown = base_data->codec_ops.shutdown;
}

static int __init codec_ana_init(void)
{
	return platform_driver_register(&g_ana_codec_platform_driver);
}

module_init(codec_ana_init);

static void __exit codec_ana_exit(void)
{
	platform_driver_unregister(&g_ana_codec_platform_driver);
}
module_exit(codec_ana_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("ALSA SoC for ana machine driver");
