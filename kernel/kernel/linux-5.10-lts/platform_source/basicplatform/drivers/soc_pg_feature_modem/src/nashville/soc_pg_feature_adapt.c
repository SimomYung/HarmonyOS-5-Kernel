/*
 * parse soc pg feature
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
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

#include <linux/platform_drivers/soc_pg_feature.h>
#include <platform_include/basicplatform/linux/dfx_cmdline_parse.h>
#include <linux/export.h>
#include <linux/printk.h>

int get_pg_soc_is_modem_need_load(void)
{
	return NEED_LOAD_MODEM;
}
EXPORT_SYMBOL(get_pg_soc_is_modem_need_load);

/*
 * Description: Get soc chip level.
 *              The soc level info passed by cmd line from bootloader.
 *              interface for modem
 * Input:
 *        void
 * Return:
 *        int soc chip level
 *        LEVEL1   -- return 0
 *        LEVEL2 -- return 1
 *        do not support feature --return -1
 */
int get_soc_modem_regulator_strategy(void)
{
	return NEED_NOTHING;
}
EXPORT_SYMBOL(get_soc_modem_regulator_strategy);

u32 get_kernel_chip_version(void)
{
	return NEED_NOTHING;
}
EXPORT_SYMBOL(get_kernel_chip_version);
