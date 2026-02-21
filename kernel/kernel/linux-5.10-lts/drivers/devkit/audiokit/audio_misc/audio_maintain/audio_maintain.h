/*
 * audio_maintain.h
 *
 * audio maintain err upload driver
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
 */

#ifndef __AUDIO_MAINTAIN__
#define __AUDIO_MAINTAIN__

#include <linux/mutex.h>
#include <linux/device.h>
#include "../audio_misc.h"

#define FAULT_NAME_LEN_MAX    32

struct audio_maintain_info {
    char name[FAULT_NAME_LEN_MAX];
};

struct audio_maintain_priv {
    struct mutex do_ioctl_lock;
};

#ifdef CONFIG_CUST_AUDIO_MAINTAIN
int audio_maintain_probe(struct device *dev, struct device_node *np);
#else
static inline int audio_maintain_probe(struct device *dev, struct device_node *np)
{
	return 0;
}
#endif /* CONFIG_CUST_AUDIO_MAINTAIN */
#endif /* __AUDIO_MAINTAIN__ */