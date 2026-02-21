/*
 * soc_spec.c
 *
 * get soc pg info
 *
 * Copyright (c) 2024- Huawei Technologies Co., Ltd.
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

#include <linux/kernel.h>
#include <linux/io.h>
#include <platform_include/basicplatform/linux/soc_spec.h>

#if defined(CONFIG_SOC_PG_INFO)
static struct soc_component_pg_info *g_pg_info_head = NULL;

void get_soc_pg_info_by_index(enum soc_component_pg_info_index pg_info_index, struct soc_component_pg_info *pg_info)
{
	if (!g_pg_info_head) {
		g_pg_info_head =
			(struct soc_component_pg_info *)((unsigned long)ioremap_wc(SOC_PG_INFO_ADDR, SOC_PG_INFO_SIZE));
		if (!g_pg_info_head)
			goto err;
	}

	if (pg_info_index >= SOC_COMPONENT_INFO_END)
		goto err;

	if (g_pg_info_head[SOC_COMPONENT_INFO_END].efuse_value != SOC_SPEC_EFUSE_INVALID_VALUE ||
		g_pg_info_head[SOC_COMPONENT_INFO_END].tailor_strategy != SOC_COMPONENT_TAILOR_STRATEGY_MAX)
		goto err;

	*pg_info = g_pg_info_head[pg_info_index];

	pr_err("[%s]index:%d, efuse_value:0x%x, tailor_strategy:%d\n", __func__,
			pg_info_index, pg_info->efuse_value, pg_info->tailor_strategy);

	return;

err:
	pg_info->efuse_value     = SOC_SPEC_EFUSE_INVALID_VALUE;
	pg_info->tailor_strategy = SOC_COMPONENT_TAILOR_STRATEGY_INVALID;
}
EXPORT_SYMBOL(get_soc_pg_info_by_index);
#endif
