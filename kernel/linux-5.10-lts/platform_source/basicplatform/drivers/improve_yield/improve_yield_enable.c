/*
 * improve_yield_enable.c
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#include <linux/errno.h>
#include <linux/printk.h>
#include <platform_include/basicplatform/linux/improve_yield.h>
#include "improve_yield_internal.h"

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
/**
 * @brief Check if the RTB intercept phase is enabled
 *
 * This function checks whether the RTB (Real-Time Bidding) intercept phase is enabled.
 * It retrieves the enable information from the improve yield module and returns the
 * enable flag status.
 *
 * @return int
 *   - 1 if the RTB intercept phase is enabled
 *   - 0 if the RTB intercept phase is disabled or if the information retrieval fails
 */
int is_rtb_intercept_phase(void)
{
	improve_yield_enabale_nv_info enabale_info;
	int                           ret;

	ret = get_improve_yield_nv_info(MODULE_ID_ENABLE, (char *)&enabale_info, sizeof(enabale_info));
	if (ret != 0) {
		pr_err("[%s %d]: get enabale info failed, ret=%d\n", __func__, __LINE__, ret);
		return 0;
	}

	return enabale_info.bits.enable_flag;
}
#endif
