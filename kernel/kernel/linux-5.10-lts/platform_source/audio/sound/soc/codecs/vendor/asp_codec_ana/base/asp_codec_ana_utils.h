/*
 * asp_codec_ana_utils.h -- codec driver
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

#ifndef __ASP_CODEC_ANA_UTILS_H__
#define __ASP_CODEC_ANA_UTILS_H__

#include "codec_debug.h"

#define max_val_on_bit(bit) ((1U << (bit)) - 1)

#ifdef LLT_TEST
#define static_t
#else
#define static_t static
#endif

#ifndef unused_parameter
#define unused_parameter(x) (void)(x)
#endif

enum reg_delay_type {
	RANGE_SLEEP = 1,
	MSLEEP,
	MDELAY,
	DELAY_BUTT,
};

struct reg_config {
	unsigned int addr;
	unsigned int mask;
	unsigned int val;
	bool update_bits;
};

struct reg_seq_config {
	struct reg_config cfg;
	enum reg_delay_type type;
	unsigned int us;
};

void asp_codec_ana_reg_seq_write_array(struct snd_soc_component *codec,
	const struct reg_seq_config *reg_seq_array, size_t len);
void ibias_work_enable(struct snd_soc_component *codec, bool enable);
unsigned int asp_codec_ana_reg_read(struct snd_soc_component *codec, unsigned int reg);
int asp_codec_ana_reg_write(struct snd_soc_component *codec, unsigned int reg, unsigned int value);

#endif
