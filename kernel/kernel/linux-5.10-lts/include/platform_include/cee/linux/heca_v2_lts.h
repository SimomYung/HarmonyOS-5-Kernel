/*
 * heca_v2_lts.h
 *
 * hisilicon efficinecy control algorithm header file.
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

#ifndef _HECA_V2_LTS_H_
#define _HECA_V2_LTS_H_

#ifdef CONFIG_HECA_V2_LTS
void set_heca_state(bool enabled);
#else
static inline void set_heca_state(bool enabled) {};
#endif

#ifdef CONFIG_HECA_GPU_LTS
bool get_gpu_heca_freq(unsigned long *value);
bool heca_gpu_enable(void);
void update_heca_gpu_min_max_freq(unsigned long min_freq, unsigned long max_freq);
#else
static inline bool get_gpu_heca_freq(unsigned long *value)
{
	return false;
}

static inline bool heca_gpu_enable(void)
{
	return false;
}

static inline void update_heca_gpu_min_max_freq(unsigned long min_freq, unsigned long max_freq) {};
#endif

#endif