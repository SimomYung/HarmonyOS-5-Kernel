// SPDX-License-Identifier: GPL-2.0
/*
 * mt5788_hw_test.c
 *
 * mt5788 hardware test driver
 *
 * Copyright (c) 2022-2022 Huawei Technologies Co., Ltd.
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

#include "mt5788.h"

#define HWLOG_TAG wireless_mt5788_hw_test
HWLOG_REGIST();

static int mt5788_hw_test_pwr_good_gpio(void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	wlps_control(WLTRX_IC_MAIN, WLPS_RX_EXT_PWR, true);
	power_usleep(DT_USLEEP_10MS);
	if (!mt5788_is_pwr_good(di)) {
		hwlog_err("pwr_good_gpio: failed\n");
		wlps_control(WLTRX_IC_MAIN, WLPS_RX_EXT_PWR, false);
		return -EINVAL;
	}
	wlps_control(WLTRX_IC_MAIN, WLPS_RX_EXT_PWR, false);

	hwlog_info("[pwr_good_gpio] succ\n");
	return 0;
};

static int mt5788_hw_test_reverse_cp(int type)
{
	return 0;
}

static struct wireless_hw_test_ops g_mt5788_hw_tst_ops = {
	.chk_pwr_good_gpio = mt5788_hw_test_pwr_good_gpio,
	.chk_reverse_cp_prepare = mt5788_hw_test_reverse_cp,
};

int mt5788_hw_test_ops_register(struct wltrx_ic_ops *ops, struct mt5788_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->hw_tst_ops = kzalloc(sizeof(*(ops->hw_tst_ops)), GFP_KERNEL);
	if (!ops->hw_tst_ops)
		return -ENODEV;

	(void)memcpy_s(ops->hw_tst_ops, sizeof(g_mt5788_hw_tst_ops), &g_mt5788_hw_tst_ops, sizeof(g_mt5788_hw_tst_ops));
	ops->hw_tst_ops->dev_data = (void *)di;
	return wireless_hw_test_ops_register(ops->hw_tst_ops);
};
