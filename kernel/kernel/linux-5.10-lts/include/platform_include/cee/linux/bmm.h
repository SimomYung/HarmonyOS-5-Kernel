/*
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _PLAT_BMM_H_
#define _PLAT_BMM_H_

#include <linux/types.h>
#include <linux/devfreq.h>

#ifdef CONFIG_BIG_MODEL_MONITOR
void bmm_notify_npu_freq(struct devfreq *devfreq, uint32_t freq);
#else
static inline void bmm_notify_npu_freq(struct devfreq *devfreq, uint32_t freq)
{
}
#endif

#endif