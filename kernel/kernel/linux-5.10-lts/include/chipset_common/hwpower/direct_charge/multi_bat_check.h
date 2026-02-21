/* SPDX-License-Identifier: GPL-2.0 */
/*
 * multi_bat_check.h
 *
 * multi battery check interface for power module
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#ifndef _MULTI_BAT_CHECK_H_
#define _MULTI_BAT_CHECK_H_

#ifdef CONFIG_DIRECT_CHARGER
/**
 * for wired charging
 * @cur_mode: 1-LVC 2-SC 4-SC4
 * @cur_flag: 0-not in dc charging, 1- in dc charging
 * Returns all supported charging mode, like LVC | SC | SC4
 */
int multi_batchk_get_support_mode(int cur_mode, bool cur_flag);

/**
 * multi_bat_match_current_dc_type - for wireless charging
 * @dc_type: defined in struct wldc_init_para
 * @cur_flag: 0-not in dc charging, 1- in dc charging
 * returns true if match, otherwise false
 */
bool multi_batchk_match_curr_dc_type(int dc_type, bool cur_flag);

/**
 * @cur_mode: 1-LVC 2-SC 4-SC4
 * @cur_path: BIT(0)-main, BIT(1)-aux, (BIT(0) | BIT(1))-multi
 * Returns all supported path mode, like BIT(0) | BIT(1)
 */
u32 multi_batchk_get_support_path(int cur_mode, int cur_path);
#else
static inline int multi_batchk_get_support_mode(int cur_mode, bool cur_flag)
{
	return -EPERM;
}

static inline bool multi_batchk_match_curr_dc_type(int dc_type, bool cur_flag)
{
	return false;
}

static inline u32 multi_batchk_get_support_path(int cur_mode, int cur_path)
{
	return EPERM;
}
#endif /* CONFIG_DIRECT_CHARGER */
#endif /* _MULTI_BAT_CHECK_H_ */
