/*
 *  Copyright (c) Huawei Technologies Co., Ltd. 2024-. All rights reserved.
 *  Description: soc spec
 *  Author : HISI_DRV
 *  Create : 2024/01/23
 */
#ifndef __SOC_SPEC_COMMON_H__
#define __SOC_SPEC_COMMON_H__

enum soc_component_tailor_strategy {
	SOC_COMPONENT_NOT_TAILOR,         // not tailor or use default freq
	SOC_COMPONENT_TAILOR_STRATEGY_1,  // tailor 1 core or 1M, or use lower 1 gear freq
	SOC_COMPONENT_TAILOR_STRATEGY_2,  // tailor 2 core or 2M, or use lower 2 gear freq
	SOC_COMPONENT_TAILOR_STRATEGY_3,  // tailor 3 core or 4M, or use upper 1 gear freq
	SOC_COMPONENT_TAILOR_STRATEGY_4,  // tailor 4 core or 8M, or use upper 2 gear freq
	SOC_COMPONENT_TAILOR_STRATEGY_5,
	SOC_COMPONENT_TAILOR_STRATEGY_6,
	SOC_COMPONENT_TAILOR_STRATEGY_7,
	SOC_COMPONENT_TAILOR_STRATEGY_8,
	SOC_COMPONENT_TAILOR_STRATEGY_9,
	SOC_COMPONENT_TAILOR_STRATEGY_10,
	SOC_COMPONENT_TAILOR_STRATEGY_11,
	SOC_COMPONENT_TAILOR_STRATEGY_12,
	SOC_COMPONENT_TAILOR_STRATEGY_13,
	SOC_COMPONENT_TAILOR_STRATEGY_14,
	SOC_COMPONENT_TAILOR_STRATEGY_15,
	SOC_COMPONENT_TAILOR_STRATEGY_INVALID,
	SOC_COMPONENT_TAILOR_STRATEGY_MAX,
};

struct soc_component_pg_info {
	unsigned int efuse_value;
	unsigned int tailor_strategy;
};

#define SOC_SPEC_EFUSE_INVALID_VALUE 0xFFFFFFFF

#if defined(CONFIG_SOC_PG_INFO_IN_LPRAM) || defined(CONFIG_SOC_PG_INFO_IN_SC) || defined(CONFIG_SOC_PG_INFO_IN_DDR)
static inline void get_soc_pg_info_by_index(
	enum soc_component_pg_info_index pg_info_index, struct soc_component_pg_info *pg_info)
{
	struct soc_component_pg_info *soc_component_pg_info_head = (struct soc_component_pg_info *)SOC_PG_INFO_ADDR;

	if (pg_info_index >= SOC_COMPONENT_INFO_END)
		goto err;

	if (soc_component_pg_info_head[SOC_COMPONENT_INFO_END].efuse_value != SOC_SPEC_EFUSE_INVALID_VALUE ||
		soc_component_pg_info_head[SOC_COMPONENT_INFO_END].tailor_strategy != SOC_COMPONENT_TAILOR_STRATEGY_MAX)
		goto err;

	*pg_info = soc_component_pg_info_head[pg_info_index];

	return;

err:
	pg_info->efuse_value     = SOC_SPEC_EFUSE_INVALID_VALUE;
	pg_info->tailor_strategy = SOC_COMPONENT_TAILOR_STRATEGY_INVALID;
}
#endif

#endif // __SOC_SPEC_COMMON_H__