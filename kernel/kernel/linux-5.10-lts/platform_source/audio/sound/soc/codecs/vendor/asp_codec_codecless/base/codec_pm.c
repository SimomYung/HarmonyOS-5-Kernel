/*
 * codec_pm.c
 *
 * codec pm process
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#include "codec_pm.h"
#include "asp_codec_comm.h"

#include "audio_log.h"

#define CODEC_PM_SWITCHES_MAX_COUNT 10
#define LOG_TAG "asp_codec_pm"

void save_switch_value(struct snd_soc_component *codec)
{
	if (codec == NULL) {
		AUDIO_LOGE("parameter is null");
		return;
	}
	struct snd_card *card = codec->card->snd_card;
	struct snd_kcontrol *kctl = NULL;
	struct snd_ctl_elem_value *ucontrol = NULL;
	struct codec_data *priv = snd_soc_component_get_drvdata(codec);
	int i, result;
	int count = priv->pm.switches_count;

	for (i = 0; i < count; ++i) {
		ucontrol = &priv->pm.switches[i];

		kctl = snd_ctl_find_id(card, &ucontrol->id);
		if (kctl == NULL || kctl->get == NULL) {
			AUDIO_LOGE("can't find %s, or switch callback is NULL", ucontrol->id.name);
			continue;
		}

		result = kctl->get(kctl, ucontrol);
		AUDIO_LOGI("kctl %s get value %ld result is %d", ucontrol->id.name, ucontrol->value.integer.value[0],
			result);
	}
}

void clear_kctl_value(struct snd_kcontrol *kctl, struct snd_ctl_elem_value *ucontrol)
{
	int result;
	int value = ucontrol->value.integer.value[0];

	ucontrol->value.integer.value[0] = 0;
	result = kctl->put(kctl, ucontrol);
	AUDIO_LOGI("kctl %s put value %ld result is %d", ucontrol->id.name, ucontrol->value.integer.value[0], result);

	ucontrol->value.integer.value[0] = value;
}

void set_kctl_value(struct snd_kcontrol *kctl, struct snd_ctl_elem_value *ucontrol)
{
	int result;
	int value = ucontrol->value.integer.value[0];

	if (value == 0)
		return;

	result = kctl->put(kctl, ucontrol);

	AUDIO_LOGI("kctl %s put value %d result is %d", ucontrol->id.name, value, result);
}

void set_switch_value(struct snd_soc_component *codec,
	void (*put)(struct snd_kcontrol *, struct snd_ctl_elem_value *))
{
	if (codec == NULL) {
		AUDIO_LOGE("parameter is null");
		return;
	}
	struct snd_card *card = codec->card->snd_card;
	struct snd_kcontrol *kctl = NULL;
	struct snd_ctl_elem_value *ucontrol = NULL;
	struct codec_data *priv = snd_soc_component_get_drvdata(codec);
	int i;

	int count = priv->pm.switches_count;

	for (i = 0; i < count; ++i) {
		ucontrol = &priv->pm.switches[i];

		kctl = snd_ctl_find_id(card, &ucontrol->id);
		if (kctl == NULL || kctl->put == NULL) {
			AUDIO_LOGE("can not find %s, or switch callback is NULL", ucontrol->id.name);
			continue;
		}

		put(kctl, ucontrol);
	}
}

static void get_codec_pm_switch(struct snd_soc_component *codec)
{
	struct codec_data *priv = snd_soc_component_get_drvdata(codec);
	struct device *dev = codec->dev;
	struct snd_ctl_elem_value *switches = NULL;
	int switches_count, ret, len, i;
	const char *prop_name = "codec_pm_switches";
	const char *name = NULL;

	switches_count = of_property_count_strings(dev->of_node, prop_name);
	if ((switches_count <= 0) || (switches_count > CODEC_PM_SWITCHES_MAX_COUNT)) {
		AUDIO_LOGE("get properity %s's count failed with error %d", prop_name, switches_count);
		return;
	}

	switches = devm_kzalloc(dev, switches_count * sizeof(*switches), GFP_KERNEL);
	if (switches == NULL) {
		AUDIO_LOGE("switches is null");
		return;
	}

	priv->pm.switches = switches;
	len = sizeof(switches->id.name);

	for (i = 0; i < switches_count; ++i) {
		ret = of_property_read_string_index(dev->of_node, prop_name, i, &name);
		if (ret) {
			AUDIO_LOGE("property %s index %d read failed with error %d", prop_name, i, ret);
			return;
		}

		ret = strlcpy(switches[i].id.name, name, len);
		if (ret >= len)
			AUDIO_LOGW("switch '%s' is too long was truncated", name);

		switches[i].id.iface = SNDRV_CTL_ELEM_IFACE_MIXER;
		AUDIO_LOGI("switch '%s' support codec pm", name);
	}

	priv->pm.switches_count = switches_count;
}

void codec_pm_init(struct snd_soc_component *codec)
{
	struct codec_data *priv = NULL;

	if (codec == NULL) {
		AUDIO_LOGE("codec parameter is null");
		return;
	}

	priv = snd_soc_component_get_drvdata(codec);
	if (of_property_read_bool(codec->dev->of_node, "support_save_switch")) {
		priv->pm.support_save_switch = true;
	} else {
		priv->pm.support_save_switch = false;
		AUDIO_LOGI("support_save_switch: %d", priv->pm.support_save_switch);
		return;
	}

	AUDIO_LOGI("support_save_switch: %d", priv->pm.support_save_switch);

	get_codec_pm_switch(codec);
}
