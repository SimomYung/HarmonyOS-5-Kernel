/*
 * asp_codec_ana_utils.c -- codec driver
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
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/soc-dapm.h>
#include <sound/tlv.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>

#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include "audio_log.h"
#include "asp_codec_ana_comm.h"
#include "asp_codec_ana_utils.h"

#define LOG_TAG "asp_codec_ana_utils"

#define MAX_INT32 0x7FFFFFFF

void asp_codec_ana_reg_seq_write_array(struct snd_soc_component *codec,
	const struct reg_seq_config *reg_seq_array, size_t len)
{
	unsigned int i;

	if (codec == NULL) {
		AUDIO_LOGE("parameter is null");
		return;
	}

	if (reg_seq_array == NULL) {
		AUDIO_LOGE("reg array is null");
		return;
	}

	for (i = 0; i < len; i++) {
		if (reg_seq_array[i].cfg.update_bits)
			snd_soc_component_update_bits(codec, reg_seq_array[i].cfg.addr,
				reg_seq_array[i].cfg.mask, reg_seq_array[i].cfg.val);
		else
			 snd_soc_component_write(codec, reg_seq_array[i].cfg.addr,
				reg_seq_array[i].cfg.val);

		switch (reg_seq_array[i].type) {
		case RANGE_SLEEP:
			usleep_range(reg_seq_array[i].us,
				reg_seq_array[i].us + reg_seq_array[i].us / 10);
			break;
		case MSLEEP:
			msleep(reg_seq_array[i].us / 1000);
			break;
		case MDELAY:
			mdelay((unsigned long)(reg_seq_array[i].us / 1000));
			break;
		default:
			break;
		}
	}
}

void ibias_work_enable(struct snd_soc_component *codec, bool enable)
{
	struct codec_ana_data *priv = snd_soc_component_get_drvdata(codec);

	IN_FUNCTION;

	mutex_lock(&priv->ibias.ibias_mutex);
	if (enable) {
		if (priv->ibias.ibias_work == 0)
			priv->codec_ops.enable_ibias(codec, true);

		if (priv->ibias.ibias_work >= MAX_INT32) {
			AUDIO_LOGE("ibias_work is MAX value, fail to enable ibias");
			mutex_unlock(&priv->ibias.ibias_mutex);
			return;
		}
		++priv->ibias.ibias_work;
	} else {
		if (priv->ibias.ibias_work <= 0) {
			AUDIO_LOGE("ibias_work is %d, fail to disable ibias", priv->ibias.ibias_work);
			mutex_unlock(&priv->ibias.ibias_mutex);
			return;
		}

		--priv->ibias.ibias_work;

		if (priv->ibias.ibias_work == 0)
			priv->codec_ops.enable_ibias(codec, false);
	}
	mutex_unlock(&priv->ibias.ibias_mutex);

	OUT_FUNCTION;
}

static int ana_get_reg_array_index(unsigned int reg, struct asp_codec_ana_reg_page *reg_array, unsigned int size)
{
	int i;
	for (i = 0; i < size; i++) {
		if (reg >= reg_array[i].page_reg_begin && reg <= reg_array[i].page_reg_end)
			return i;
	}

	AUDIO_LOGE("can not find reg index, reg 0x%x", reg);
	return -1;
}

static_t unsigned int ana_vircodec_reg_read(struct codec_ana_data *priv, unsigned int reg_offset)
{
	if (reg_offset >= priv->v_codec_num)
		return 0;

	return priv->v_codec_reg[reg_offset];
}

static_t unsigned int ana_reg_read(struct codec_ana_data *priv, unsigned int reg)
{
	unsigned int ret = 0;
	unsigned long flags;
	unsigned int reg_offset = 0;

	struct asp_codec_ana_reg_page *reg_array = priv->platform_reg_array;
	unsigned int array_size = priv->platform_reg_size;

	int index = ana_get_reg_array_index(reg, reg_array, array_size);
	if (index < 0)
		return EINVAL;

	reg_offset = reg - reg_array[index].page_reg_begin;

	spin_lock_irqsave(&priv->lock, flags);
	if (reg_array[index].is_virtual_reg)
		ret = ana_vircodec_reg_read(priv, reg_offset);
	else
		ret = pmic_read_reg(reg);
	spin_unlock_irqrestore(&priv->lock, flags);

	AUDIO_LOGD("reg read reg = 0x%x, ret = 0x%x", (void *)(uintptr_t)reg, ret);

	return ret;
}

static_t void ana_vircodec_reg_write(struct codec_ana_data *priv, unsigned int reg_offset, unsigned int value)
{
	if (reg_offset < priv->v_codec_num)
		priv->v_codec_reg[reg_offset] = value;
}

static_t void ana_reg_write(struct codec_ana_data *priv, unsigned int reg, unsigned int value)
{
	unsigned long flags;
	unsigned int reg_offset = 0;

	struct asp_codec_ana_reg_page *reg_array = priv->platform_reg_array;
	unsigned int array_size = priv->platform_reg_size;

	int index = ana_get_reg_array_index(reg, reg_array, array_size);
	if (index < 0)
		return;

	reg_offset = reg - reg_array[index].page_reg_begin;

	spin_lock_irqsave(&priv->lock, flags);

	AUDIO_LOGD("write reg = 0x%x, value:0x%x", (void *)(uintptr_t)reg, value);
	if (reg_array[index].is_virtual_reg)
		ana_vircodec_reg_write(priv, reg_offset, value);
	else
		pmic_write_reg(reg, value);
	spin_unlock_irqrestore(&priv->lock, flags);
}

unsigned int asp_codec_ana_reg_read(struct snd_soc_component *codec, unsigned int reg)
{
	unsigned int ret;
	struct codec_ana_data *priv = NULL;

	IN_FUNCTION;

	if (codec == NULL) {
		AUDIO_LOGE("parameter is null");
		return EINVAL;
	}

	priv = snd_soc_component_get_drvdata(codec);
	if (priv == NULL) {
		AUDIO_LOGE("codec data is null");
		return EINVAL;
	}

	ret = ana_reg_read(priv, reg);
	if (ret == EINVAL)
		AUDIO_LOGW("reg %pK read value 0x%x maybe invalid",
			(void *)(uintptr_t)reg, ret);

	return ret;
}

int asp_codec_ana_reg_write(struct snd_soc_component *codec, unsigned int reg,
	unsigned int value)
{
	struct codec_ana_data *priv = NULL;

	if (codec == NULL) {
		AUDIO_LOGE("parameter is null");
		return -EINVAL;
	}

	priv = snd_soc_component_get_drvdata(codec);
	if (priv == NULL) {
		AUDIO_LOGE("codec data is null");
		return -EINVAL;
	}

	ana_reg_write(priv, reg, value);

	return 0;
}