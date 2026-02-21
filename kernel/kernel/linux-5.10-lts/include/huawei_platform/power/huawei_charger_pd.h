/*
 * huawei_charger_pd.h
 *
 * huawei charger pd
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

#ifndef _HUAWEI_CHARGER_PD_H_
#define _HUAWEI_CHARGER_PD_H_

#include <linux/workqueue.h>

#ifdef CONFIG_HUAWEI_CHARGER_AP
void emark_detect_complete(void);
void charge_pd_try_to_dc(void);
void charge_pd_try_to_fcp(void);
void charge_pd_init(struct delayed_work *pd_voltage_change_work);
void charge_pd_check(void);
void charge_pd_clear_data(void);
bool charge_pd_bat_vol_support_9v(void);
#else
static inline void emark_detect_complete(void)
{
}

static inline void charge_pd_try_to_dc(void)
{
}

static inline void charge_pd_try_to_fcp(void)
{
}

static inline void charge_pd_init(struct delayed_work *pd_voltage_change_work)
{
}

static inline void charge_pd_check(void)
{
}

static inline void charge_pd_clear_data(void)
{
}

static inline bool charge_pd_bat_vol_support_9v(void)
{
	return true;
}
#endif /* CONFIG_HUAWEI_CHARGER_AP */

#endif /* _HUAWEI_CHARGER_PD_H_ */
