// SPDX-License-Identifier: GPL-2.0
/*
 * bk96285_hw_test.c
 *
 * bk96285 hardware test driver
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

#include "bk96285.h"

#define HWLOG_TAG wireless_bk96285_hw_test
HWLOG_REGIST();

static int bk96285_hw_test_pwr_good_gpio(void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	wlps_control(WLTRX_IC_MAIN, WLPS_RX_EXT_PWR, true);
	power_usleep(DT_USLEEP_10MS);
	if (!bk96285_is_pwr_good(di)) {
		hwlog_err("pwr_good_gpio: failed\n");
		wlps_control(WLTRX_IC_MAIN, WLPS_RX_EXT_PWR, false);
		return -EINVAL;
	}
	wlps_control(WLTRX_IC_MAIN, WLPS_RX_EXT_PWR, false);

	hwlog_info("[pwr_good_gpio] succ\n");
	return 0;
};

static int bk96285_hw_test_reverse_cp(int type)
{
	return 0;
}

static struct wireless_hw_test_ops g_bk96285_hw_test_ops = {
	.chk_pwr_good_gpio = bk96285_hw_test_pwr_good_gpio,
	.chk_reverse_cp_prepare = bk96285_hw_test_reverse_cp,
};

int bk96285_hw_test_ops_register(struct bk96285_dev_info *di)
{
	if (!di)
		return -ENODEV;

	g_bk96285_hw_test_ops.dev_data = (void *)di;
	return wireless_hw_test_ops_register(&g_bk96285_hw_test_ops);
};
