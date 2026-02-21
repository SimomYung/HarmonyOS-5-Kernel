/*
 * huawei_charger_context.h
 *
 * huawei charger context
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

#ifndef _HUAWEI_CHARGE_CONTEXT_H_
#define _HUAWEI_CHARGE_CONTEXT_H_

#include <chipset_common/hwpower/buck_charge/buck_charge_ic.h>
#include <chipset_common/hwpower/common_module/power_time.h>

#define PD_TO_9V_ERR_MAX_COUNT             3
#define PD_TO_DC_MAX_COUNT                 3
#define BUCK_9V_VBAT_TH_HYSTER             200

struct buck_charge_context {
	bool support_new_pd_process;
	bool wait_pd_init;
	bool pd_charge_flag;
	bool pd_done;
	bool pd_adapter_support_dc;
	bool pd_to_dc_flag;

	int last_pd_vset;
	int fcp_vbus_low_cnt;
	int try_pd_to_dc_cnt;
	int try_pd_to_fcp_cnt;
	int buck_9v_enter_th;
	int try_pd_to_9v_err_cnt;

	time64_t pd_time; /* pd vbus voltage abnormal time */
	time64_t fcp_time; /* fcp vbus voltage abnormal time */
	time64_t nonfcp_time; /* nonfcp vbus voltage abnormal time */

	struct charge_init_data init_data;
};

struct buck_charge_context *charge_get_buck_charge_context(void);
#define charge_context(param) (charge_get_buck_charge_context()->param)

#endif /* _HUAWEI_CHARGE_CONTEXT_H_ */
