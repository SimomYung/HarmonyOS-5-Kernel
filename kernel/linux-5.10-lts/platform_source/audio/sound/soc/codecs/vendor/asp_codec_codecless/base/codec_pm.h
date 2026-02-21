/*
 * codec_pm.h
 *
 * expose route pm api for codec driver
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

#ifndef __CODEC_PM_H__
#define __CODEC_PM_H__

#include <sound/soc.h>
#include <linux/device.h>

void codec_pm_init(struct snd_soc_component *codec);
void save_switch_value(struct snd_soc_component *codec);
void set_switch_value(struct snd_soc_component *codec,
	void (*put)(struct snd_kcontrol *, struct snd_ctl_elem_value *));
void set_kctl_value(struct snd_kcontrol *kctl, struct snd_ctl_elem_value *ucontrol);
void clear_kctl_value(struct snd_kcontrol *kctl, struct snd_ctl_elem_value *ucontrol);
#endif
