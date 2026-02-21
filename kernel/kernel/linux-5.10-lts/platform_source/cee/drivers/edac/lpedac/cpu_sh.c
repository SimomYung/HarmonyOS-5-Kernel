/*
 * cpu_sh.c
 *
 * CPU SH API
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/topology.h>
#include <linux/string.h>
#include "ras_common.h"
#include "dfx_sh.h"
#include "cpu_sh.h"

#define CE_SH_CNT 5
void ras_ce_sh(struct edac_device_ctl_info *edac_dev, u32 cpu, char* instance_name)
{
	if (IS_ERR_OR_NULL(edac_dev) || IS_ERR_OR_NULL(instance_name)) {
		pr_err("[%s %d] edac_dev or instance_name is NULL\n", __func__, __LINE__);
		return;
	}

	static int cluster_ce_cnt[FREQ_DOMAIN_MAX] = {0};
	bool is_l3_ecc_flag = is_l3_ecc(instance_name);
	int cluster = is_l3_ecc_flag ? (FREQ_DOMAIN_MAX - 1) : topology_physical_package_id(cpu);
	int cpu_sh_type = is_l3_ecc_flag ? sh_type_l3 : cluster;

	if (g_cpu_pll_source[cluster] == CLK_SEL_APLL) {
		if (cluster_ce_cnt[cluster] >= CE_SH_CNT - 1) {
			edac_device_printk(edac_dev, KERN_WARNING, "CE: SH triggering within cluster: %d.\n", cluster);
			cpu_ecc_do_sh(SH_CPU_ECC_CE, cpu_sh_type);
		} else {
			cluster_ce_cnt[cluster]++;
		}
	}
}

#define DE_SH_CNT 5
void ras_de_sh(struct edac_device_ctl_info *edac_dev, u32 cpu, char* instance_name)
{
	if (IS_ERR_OR_NULL(edac_dev) || IS_ERR_OR_NULL(instance_name)) {
		pr_err("[%s %d] edac_dev or instance_name is NULL\n", __func__, __LINE__);
		return;
	}

	static int cluster_de_cnt[FREQ_DOMAIN_MAX] = {0};
	bool is_l3_ecc_flag = is_l3_ecc(instance_name);
	int cluster = is_l3_ecc_flag ? (FREQ_DOMAIN_MAX - 1) : topology_physical_package_id(cpu);
	int cpu_sh_type = is_l3_ecc_flag ? sh_type_l3 : cluster;

	if (g_cpu_pll_source[cluster] == CLK_SEL_APLL) {
		if (cluster_de_cnt[cluster] >= DE_SH_CNT - 1) {
			edac_device_printk(edac_dev, KERN_WARNING, "DE: SH triggering within cluster: %d.\n", cluster);
			cpu_ecc_do_sh(SH_CPU_ECC_DE, cpu_sh_type);
		} else {
			cluster_de_cnt[cluster]++;
		}
	}
}

void ras_ue_sh(struct edac_device_ctl_info *edac_dev, u32 cpu, char* instance_name)
{
	if (IS_ERR_OR_NULL(edac_dev) || IS_ERR_OR_NULL(instance_name)) {
		pr_err("[%s %d] edac_dev or instance_name is NULL\n", __func__, __LINE__);
		return;
	}

	bool is_l3_ecc_flag = is_l3_ecc(instance_name);
	int cluster = is_l3_ecc_flag ? (FREQ_DOMAIN_MAX - 1) : topology_physical_package_id(cpu);
	int cpu_sh_type = is_l3_ecc_flag ? sh_type_l3 : cluster;

	edac_device_printk(edac_dev, KERN_WARNING, "UE: SH triggering within cluster: %d.\n", cluster);
	cpu_ecc_do_sh(SH_CPU_ECC_UE, cpu_sh_type);
}
