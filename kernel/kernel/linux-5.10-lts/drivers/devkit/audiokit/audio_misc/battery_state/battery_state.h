/*
 * battery_state.h
 *
 * battery_state driver
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

#ifndef __BATTERY_STATE_H__
#define __BATTERY_STATE_H__

#include <linux/mutex.h>
#include <linux/device.h>
#include "../audio_misc.h"

struct battery_state_priv {
    uint32_t lower_limit;
    uint32_t upper_limit;
    int battery_state;
    struct notifier_block battery_state_notifier;
    struct work_struct adjust_work;
};

#ifdef CONFIG_AUDIO_BATTERY_STATE
int battery_state_probe(struct device *dev, struct device_node *np);
void force_adjust_current(void);
#else
static inline int battery_state_probe(struct device *dev, struct device_node *np)
{
	return 0;
}

static inline void force_adjust_current(void)
{
}
#endif /* CONFIG_AUDIO_BATTERY_STATE */
#endif  /* __BATTERY_STATE_H__ */