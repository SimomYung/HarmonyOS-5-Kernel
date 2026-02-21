/*
 * huawei_charger_check.h
 *
 * huawei charger check headfile
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

#ifndef _HUAWEI_CHARGE_CHECK_H_
#define _HUAWEI_CHARGE_CHECK_H_

#ifdef CONFIG_HUAWEI_CHARGER_AP
int charge_check_pd_ufcs_adapter_vbus(void);
void charge_check_vbus_voltage(void);
void charge_check_reset_vbus_check_info(void);
#else
static inline int charge_check_pd_ufcs_adapter_vbus(void)
{
	return 0;
}

static inline void charge_check_vbus_voltage(void)
{
}

static inline void charge_check_reset_vbus_check_info(void)
{
}
#endif /* CONFIG_HUAWEI_CHARGER_AP */
#endif /* _HUAWEI_CHARGE_CHECK_H_ */
