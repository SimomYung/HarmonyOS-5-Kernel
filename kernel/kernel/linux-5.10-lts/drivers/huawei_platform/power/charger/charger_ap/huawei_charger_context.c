// SPDX-License-Identifier: GPL-2.0
/*
 * huawei_charger_context.c
 *
 * huawei charger context data
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

#include <huawei_platform/power/huawei_charger_context.h>

static struct buck_charge_context g_buck_charge_context = {
	.support_new_pd_process = false,
	.wait_pd_init = false,
	.pd_charge_flag = false,
	.pd_done = false,
	.pd_adapter_support_dc = true,
	.pd_to_dc_flag = false,

	.last_pd_vset = 0,
	.fcp_vbus_low_cnt = 0,
	.try_pd_to_dc_cnt = 0,
	.try_pd_to_fcp_cnt = 0,
	.buck_9v_enter_th = 0,
	.try_pd_to_9v_err_cnt = 0,

	.pd_time = 0,
	.fcp_time = 0,
	.nonfcp_time = 0,

	.init_data = { 0, 0 },
};

struct buck_charge_context *charge_get_buck_charge_context(void)
{
	return &g_buck_charge_context;
}
