/*
 * vendorcountry_state.h
 *
 * vendorcountry_state driver
 *
 * Copyright (c) 2024-2025 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __VENDORCOUNTRY_STATE_H__
#define __VENDORCOUNTRY_STATE_H__

#include "../audio_misc.h"

#define VENDORCOUNTRY_STATE_IOCTL_TYPE 'V'
#define VENDORCOUNTRY_STATE_MAX_LENGTH  64
#define VENDORCOUNTRY_STATE_READ        _IOR(VENDORCOUNTRY_STATE_IOCTL_TYPE, 0x01, char[64])
#define CMDLINE_MAX_LENGTH              4096

#ifdef CONFIG_AUDIO_VENDORCOUNTRY_STATE
int vendorcountry_state_probe(struct device *dev, struct device_node *np);
#else
static inline int vendorcountry_state_probe(struct device *dev, struct device_node *np)
{
    return 0;
}
#endif  /* CONFIG_AUDIO_VENDORCOUNTRY_STATE */

#endif  /* __VENDORCOUNTRY_STATE_H__ */