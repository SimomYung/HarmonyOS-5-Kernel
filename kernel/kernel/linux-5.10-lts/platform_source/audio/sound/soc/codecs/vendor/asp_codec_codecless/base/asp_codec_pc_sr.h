/*
 * asp_codec_pc_sr.h -- codec driver
 *
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
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

#ifndef __ASP_CODEC_PC_SR_H__
#define __ASP_CODEC_PC_SR_H__

#include "asp_codec_comm.h"

int asp_codec_freeze(struct device *dev);
int asp_codec_suspend_late(struct device *dev);
int asp_codec_suspend_extended(struct device *dev);
int asp_codec_resume_extended(struct device *dev);
int asp_codec_freeze_extended(struct device *dev);
int asp_codec_restore_extended(struct device *dev);
int asp_codec_thaw_extended(struct device *dev);
void set_extern_codec_existence_status(struct snd_soc_component *codec);
bool get_extern_codec_existence_status(void);
#endif

