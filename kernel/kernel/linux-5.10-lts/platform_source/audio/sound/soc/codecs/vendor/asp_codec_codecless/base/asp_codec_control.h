/*
 * asp_codec_control.h -- codec driver
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

#ifndef __ASP_CODEC_CONTROL_H__
#define __ASP_CODEC_CONTROL_H__

#include "asp_codec_utils.h"
#include "asp_codec_pc_sr.h"

void asp_codec_driver_init(struct codec_data *base_data);
int asp_codec_resume(struct device *dev);
int asp_codec_suspend(struct device *dev);
void asp_resource_disable(struct snd_soc_component *codec);
int asp_codec_asp_power_off_check(struct snd_soc_component *codec);
#endif

