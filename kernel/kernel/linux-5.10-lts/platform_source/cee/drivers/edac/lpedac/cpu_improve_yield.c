/*
 * cpu_improve_yield.c
 *
 * CPU IMPROVE YIELD API
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
#include <securec.h>
#include <platform_include/basicplatform/linux/improve_yield.h>
#include <platform_include/cee/linux/rdr_get_cpu_rt_freq_volt.h>
#include <rdr_cpu_rt_freq_volt_params.h>
#include <improve_yield_cpu_info.h>
#include "ras_common.h"
#include "cpu_improve_yield.h"

static int cpu_ecc_register_iy_nvme(char *buffer, unsigned int *length)
{
	int ret;
	u32 cluster;
	improve_yield_cpu_ecc_nv_t cpu_ecc_info;
	char *cluster_str[] = {"CPUC0", "CPUC1", "CPUC2"};

	if (IS_ERR_OR_NULL(buffer) || IS_ERR_OR_NULL(length)) {
		pr_err("[%s %d] buffer or length is NULL\n", __func__, __LINE__);
		return -EINVAL;
	}

	ret = get_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_ecc_info, sizeof(improve_yield_cpu_ecc_nv_t));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
		return -EINVAL;
	}

	for (cluster = CPU_LIT_CLUSTER; cluster < CPU_CLUSTER_CNT; cluster++) {
		if (cpu_ecc_info.cluster_ecc_info[cluster].bits.pv_flag == 1) {
			ret = sprintf_s(buffer + *length, CPU_IY_NV_INFO_MAX_LENGTH, "%s%s0x%x%s0x%x%s", cluster_str[cluster],
				"=flag:", cpu_ecc_info.cluster_ecc_info[cluster].value & 0xff,
				",freqMask:", cpu_ecc_info.cluster_ecc_info[cluster].bits.freq_ce_mask,
				";");
			if (ret == -1) {
				pr_err("snprintf_s CPU nv info fail\n");
				*length = 0;
				return -EINVAL;
			}
			*length += ret;
		}
	}

	*length = (*length > CPU_IY_NV_INFO_MAX_LENGTH) ? CPU_IY_NV_INFO_MAX_LENGTH : *length;

	return 0;
}

static int cpu_panic_register_iy_nvme(char *buffer, unsigned int *length)
{
	int ret;
	u32 cluster;
	improve_yield_cpu_panic_nv_t cpu_panic_info;
	char *cluster_str[] = {"CPUC0", "CPUC1", "CPUC2"};

	if (IS_ERR_OR_NULL(buffer) || IS_ERR_OR_NULL(length)) {
		pr_err("[%s %d] buffer or length is NULL\n", __func__, __LINE__);
		return -EINVAL;
	}

#if defined(CONFIG_IMPROVE_YIELD_PANIC)
	ret = get_improve_yield_nv_info(MODULE_ID_CPU_PANIC, (char *)&cpu_panic_info, sizeof(improve_yield_cpu_panic_nv_t));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield cpu panic info fail, ret:%d\n", __func__, __LINE__, ret);
		return -EINVAL;
	}

	for (cluster = CPU_LIT_CLUSTER; cluster < CPU_CLUSTER_CNT; cluster++) {
		if (cpu_panic_info.cluster_panic_info[cluster].bits.pv_flag == 1) {
			ret = sprintf_s(buffer + *length, CPU_IY_NV_INFO_MAX_LENGTH, "%s%s0x%x%s", cluster_str[cluster],
				"_PANIC=flag:", cpu_panic_info.cluster_panic_info[cluster].value & 0xff,
				";");
			if (ret == -1) {
				pr_err("snprintf_s CPU nv info fail\n");
				*length = 0;
				return -EINVAL;
			}
			*length += ret;
		}
	}

	*length = (*length > CPU_IY_NV_INFO_MAX_LENGTH) ? CPU_IY_NV_INFO_MAX_LENGTH : *length;
#endif

	return 0;
}

int cpu_register_iy_nvme(char *buffer, unsigned int *length)
{
	int ret;

	if (IS_ERR_OR_NULL(buffer) || IS_ERR_OR_NULL(length)) {
		pr_err("[%s %d] buffer or length is NULL\n", __func__, __LINE__);
		return -EINVAL;
	}

	*length = 0;
	ret = cpu_ecc_register_iy_nvme(buffer, length);
	if (ret != 0)
		return -EINVAL;

	ret = cpu_panic_register_iy_nvme(buffer, length);
	if (ret != 0)
		return -EINVAL;

	return 0;
}

#if defined(CONFIG_FACTORY_MODE)
#define FREQ_MAP_CNT  24
#define FREQ_MAP_MASK (BIT(FREQ_MAP_CNT) - 1)

static void cpu_improve_yield_write_nv(u32 cluster, u32 freq_idx_mask, bool all_freq_boost_flag)
{
	int ret = 0;
	improve_yield_cpu_ecc_nv_t cpu_ecc_info;

	ret = get_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_ecc_info, sizeof(improve_yield_cpu_ecc_nv_t));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
		return;
	}

	cpu_ecc_info.cluster_ecc_info[cluster].bits.pv_flag = 1;
	cpu_ecc_info.cluster_ecc_info[cluster].bits.all_freq_pv_flag |= all_freq_boost_flag ? 1 : 0;
	cpu_ecc_info.cluster_ecc_info[cluster].bits.freq_ce_mask |= (freq_idx_mask & FREQ_MAP_MASK);

	ret = set_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_ecc_info, sizeof(improve_yield_cpu_ecc_nv_t));
	if (ret != 0)
		pr_err("[%s %d]: set improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
}

int cpu_boost_volt(u32 cluster, u32 freq_idx_mask, u32 is_all_freq)
{
	int ret = set_cpu_improve_yield_data_via_tzsp(cluster, freq_idx_mask, is_all_freq);
	if (ret != 0)
		pr_err("%s: ret:%d, failed\n", __func__, ret);

	return ret;
}

static void cpu_boost_volt_write_nv(u32 cluster, u32 per_freq_ce_cnt[CPU_CLUSTER_CNT][FREQ_MAP_CNT],
									u32 freq_idx_mask, bool all_freq_boost_flag)
{
	u32 index;

	for (index = 0; index < FREQ_MAP_CNT; index++) {
		if (all_freq_boost_flag || per_freq_ce_cnt[cluster][index] > 0) {
			cpu_improve_yield_write_nv(cluster, freq_idx_mask, all_freq_boost_flag);
			cpu_boost_volt(cluster, freq_idx_mask, all_freq_boost_flag);
			if (all_freq_boost_flag)
				break;
		}
	}
}

static u32 get_exception_freq_cnt(u32 cluster, u32 per_freq_ce_cnt[CPU_CLUSTER_CNT][FREQ_MAP_CNT])
{
	u32 cnt = 0;
	u32 index;

	for (index = 0; index < FREQ_MAP_CNT; index++) {
		if (per_freq_ce_cnt[cluster][index] != 0)
			cnt++;
	}

	return cnt;
}

static void cpu_improve_yield_data_init(u32 per_freq_ce_cnt[CPU_CLUSTER_CNT][FREQ_MAP_CNT], u32 freq_ce_mask[CPU_CLUSTER_CNT])
{
	int ret = 0;
	improve_yield_cpu_ecc_nv_t cpu_ecc_info;
	u32 cluster;
	u32 freq_index;
	static bool has_init = false;

	if (has_init)
		return;

	ret = get_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_ecc_info, sizeof(improve_yield_cpu_ecc_nv_t));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
		return;
	}

	for (cluster = 0; cluster < CPU_CLUSTER_CNT; cluster++) {
		freq_ce_mask[cluster] = cpu_ecc_info.cluster_ecc_info[cluster].bits.freq_ce_mask;
		for (freq_index = 0; freq_index < FREQ_MAP_CNT; freq_index++) {
			per_freq_ce_cnt[cluster][freq_index] = (((freq_ce_mask[cluster] >> freq_index) & 0x1) == 0x1) ? 1 : 0;
		}
	}

	has_init = true;
}

#define DMD_ON_CNT 2
#define ALL_BOOST_EXCEPTION_FREQ_CNT 2
#define _IS_ALL_FREQ_BOOST(n) ((n) >= ALL_BOOST_EXCEPTION_FREQ_CNT)
#define IS_ALL_FREQ_BOOST(n) (_IS_ALL_FREQ_BOOST(n) ? true : false)
#define IS_DMD_ON(all_freq_boost_flag, per_freq_ce_cnt) ((all_freq_boost_flag) || ((per_freq_ce_cnt) >= DMD_ON_CNT))

static bool _cpu_improve_yield(u32 cluster)
{
	bool all_freq_boost_flag = true;
	u32 exception_freq_cnt;
	static u32 per_freq_ce_cnt[CPU_CLUSTER_CNT][FREQ_MAP_CNT] = {0};
	static u32 freq_ce_mask[CPU_CLUSTER_CNT] = {0};
	u32 freq_index = get_cpu_rt_freq_index_via_tzsp(cluster);
	u32 cnt = 0;

	cpu_improve_yield_data_init(per_freq_ce_cnt, freq_ce_mask);
	if (freq_index >= 0 && freq_index <= FREQ_MAP_CNT) {
		cnt = per_freq_ce_cnt[cluster][freq_index];
		per_freq_ce_cnt[cluster][freq_index] = (cnt > DMD_ON_CNT) ? cnt : ++cnt;
		freq_ce_mask[cluster] |= BIT(freq_index);

		exception_freq_cnt = get_exception_freq_cnt(cluster, per_freq_ce_cnt);
		all_freq_boost_flag = IS_ALL_FREQ_BOOST(exception_freq_cnt);
		cpu_boost_volt_write_nv(cluster, per_freq_ce_cnt, freq_ce_mask[cluster], all_freq_boost_flag);
	}

	return IS_DMD_ON(all_freq_boost_flag, per_freq_ce_cnt[cluster][freq_index]);
}

bool cpu_improve_yield(u32 cpu)
{
	bool dmd_on_off_flag = true;
	u32 cluster = topology_physical_package_id(cpu);

	if (is_module_improve_yield_enable(MODULE_ID_CPU) == 1)
		dmd_on_off_flag = _cpu_improve_yield(cluster);

	return dmd_on_off_flag;
}
#endif
