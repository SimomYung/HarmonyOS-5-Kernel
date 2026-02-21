/*
 * asp_codec_ana_comm.h -- codec driver
 *
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
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

#ifndef __ASP_CODEC_ANA_COMM_H__
#define __ASP_CODEC_ANA_COMM_H__

#include <linux/platform_device.h>
#include <sound/soc.h>

#include "codec_debug.h"
#ifdef CONFIG_SND_SOC_DA_SEPARATE_MBHC
#include "da_separate_mbhc.h"
#endif

struct ibias_data {
	struct mutex ibias_mutex;
	int ibias_work;
};

struct ana_codec_ops {
	void (*ana_priv_data_init)(struct snd_soc_component *codec);
	void (*ana_priv_data_deinit)(struct snd_soc_component *codec);
	void (*ana_reg_init)(struct snd_soc_component *codec);
	void (*ana_reset_codec)(struct snd_soc_component *codec);
	void (*enable_ibias)(struct snd_soc_component *codec, int on);
	void (*shutdown)(struct platform_device *);
};

struct ana_codec_resource {
	const struct snd_kcontrol_new *controls;
	unsigned int controls_num;
	const struct snd_soc_dapm_widget *widgets;
	unsigned int widgets_num;
	const struct snd_soc_dapm_route *routes;
	unsigned int routes_num;
};

struct asp_codec_ana_reg_page {
	unsigned int page_reg_begin;
	unsigned int page_reg_end;
	bool is_virtual_reg;
};

struct codec_ana_data {
	struct snd_soc_component *codec;
	struct regulator *pmu_regulator;
	spinlock_t lock;
	struct ana_codec_ops codec_ops;
	unsigned int *v_codec_reg;
	unsigned int v_codec_num;
	struct ibias_data ibias;
	struct ana_codec_resource *codec_resource;
	struct asp_codec_ana_reg_page *platform_reg_array;
	unsigned int platform_reg_size;
#ifdef CONFIG_SND_SOC_DA_SEPARATE_MBHC
	struct da_separate_mbhc *mbhc;
	struct da_separate_mbhc_ops *mbhc_ops;
#endif
	struct hicodec_dump_reg_info *debug_reg_info;
};

#endif

