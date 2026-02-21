/*
 * audio_wakeup_info.h
 *
 * audio wakeup info driver
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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

#ifndef __AUDIO_WAKEUP_INFO__
#define __AUDIO_WAKEUP_INFO__

#include <linux/mutex.h>
#include <linux/device.h>
#include "../audio_misc.h"

#define AUDIO_WAKEUP_INFO_IOCTL_TYPE 'N'
#define AUDIO_WAKEUP_INFO_EVENT _IOWR(AUDIO_WAKEUP_INFO_IOCTL_TYPE, 0x01, int)
#define AUDIO_WAKEUP_INFO_LENTH 1024

struct audio_wakeup_priv {
    struct mutex do_ioctl_lock;
};

#ifdef CONFIG_AUDIO_WAKEUP_INFO
int audio_wakeup_info_probe(struct device *dev, struct device_node *np);
#else
static inline int audio_wakeup_info_probe(struct device *dev, struct device_node *np)
{
    return 0;
}
#endif /* CONFIG_AUDIO_WAKEUP_INFO */
#endif /* __AUDIO_WAKEUP_INFO__ */
