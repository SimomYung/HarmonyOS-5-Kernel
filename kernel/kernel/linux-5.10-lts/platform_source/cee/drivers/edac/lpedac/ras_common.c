/*
 * ras_common.c
 *
 * RAS COMMON API
 *
 * Copyright (c) 2019-2021 Huawei Technologies Co., Ltd.
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

#include <asm/page.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>
#include <linux/highmem.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/sysctl.h>
#include <linux/timer.h>
#include <linux/topology.h>
#include <linux/string.h>
#if defined(CONFIG_IMPROVE_YIELD)
#include <platform_include/basicplatform/linux/improve_yield.h>
#endif
#include <securec.h>
#include <platform_include/cee/linux/rdr_get_cpu_rt_freq_volt.h>
#include <rdr_cpu_rt_freq_volt_params.h>
#include "ras_common.h"
#ifdef CONFIG_DFX_SH
#include "dfx_sh.h"
#endif
#ifdef CONFIG_RAS_EDAC_UNCORRECTED_ERROR
#include "ras_ecc.h"
#endif

static bool is_l3_ecc(char* str)
{
	char* l3_instance_name = "share_l3";
	return (strstr(str, l3_instance_name) == NULL) ? false : true;
}

static bool is_bus_error(u64 status)
{
	if (ERR_STATUS_GET_FIELD(status, SERR) == BUS_ERROR)
		return true;

	return false;
}

static void ras_device_panic(struct irq_nodes *pdata, int cpu_id,
			     int type, int cpu)
{
	u32 panic_id;

	if (pdata->mod_id == SHARE_L2 && cpu_id >= 0) {
		panic_id = MODID_AP_S_CPU01_L2_CE + cpu_id * 2 + type;
	} else if (pdata->mod_id == SHARE_L3) {
		panic_id = MODID_AP_S_L3_CE + type;
	} else {
		if (cpu_id < 0)
			panic_id = MODID_AP_S_CPU0_CE + cpu * 2 + type;
		else
			panic_id = MODID_AP_S_CPU0_CE + cpu_id * 2 + type;
	}

	rdr_syserr_process_for_ap(panic_id, 0ULL, 0ULL);
}

static bool ras_nr_check(struct edac_device_ctl_info *edac_dev,
		  int inst_nr, int block_nr)
{
	struct edac_device_instance *instance = NULL;

	if (edac_dev == NULL)
		return false;

	if (((u32)inst_nr >= edac_dev->nr_instances) || (inst_nr < 0)) {
		edac_device_printk(edac_dev, KERN_ERR,
				   "INTERNAL ERROR: 'instance' out of range "
				   "(%d >= %d)\n", inst_nr,
				   edac_dev->nr_instances);
		return false;
	}

	instance = edac_dev->instances + inst_nr;
	if (((u32)block_nr >= instance->nr_blocks) || (block_nr < 0)) {
		edac_device_printk(edac_dev, KERN_ERR,
				   "INTERNAL ERROR: instance %d 'block' "
				   "out of range (%d >= %d)\n",
				   inst_nr, block_nr,
				   instance->nr_blocks);
		return false;
	}

	return true;
}

#if defined(CONFIG_IMPROVE_YIELD)
#define CPU_IY_NV_INFO_MAX_LENGTH 256

int cpu_register_iy_nvme(char *buffer, unsigned int *length)
{
	int ret;
	u32 cluster;
	improve_yield_cpu_nv_t cpu_info;
	char *cluster_str[] = {"CPUC0", "CPUC1", "CPUC2"};

	ret = get_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_info, sizeof(improve_yield_cpu_nv_t));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
		return -EINVAL;
	}

	*length = 0;
	for (cluster = CPU_LIT_CLUSTER; cluster < CPU_CLUSTER_CNT; cluster++) {
		if (cpu_info.cluster_info[cluster].bits.pv_flag == 1) {
			ret = sprintf_s(buffer + *length, CPU_IY_NV_INFO_MAX_LENGTH, "%s%s0x%x%s0x%x%s", cluster_str[cluster],
				"=flag:", cpu_info.cluster_info[cluster].value & 0xff,
				",freqMask:", cpu_info.cluster_info[cluster].bits.freq_ce_mask,
				";");
			if (ret == -1) {
				pr_err("snprintf_s CPU nv info fail\n");
				*length = 0;
				return -EINVAL;
			}
			*length += ret;
		}
	}

	return 0;
}
#endif

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
#define FREQ_MAP_CNT  24
#define FREQ_MAP_MASK (BIT(FREQ_MAP_CNT) - 1)

static void cpu_improve_yield_write_nv(u32 cluster, u32 freq_idx_mask, bool all_freq_boost_flag)
{
	int ret = 0;
	improve_yield_cpu_nv_t cpu_info;

	ret = get_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_info, sizeof(improve_yield_cpu_nv_t));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
		return;
	}

	cpu_info.cluster_info[cluster].bits.pv_flag = 1;
	cpu_info.cluster_info[cluster].bits.all_freq_pv_flag |= all_freq_boost_flag ? 1 : 0;
	cpu_info.cluster_info[cluster].bits.freq_ce_mask |= (freq_idx_mask & FREQ_MAP_MASK);

	ret = set_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_info, sizeof(improve_yield_cpu_nv_t));
	if (ret != 0)
		pr_err("[%s %d]: set improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
}

static int cpu_boost_volt(u32 cluster, u32 freq_idx_mask, u32 is_all_freq)
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
	improve_yield_cpu_nv_t cpu_info;
	u32 cluster;
	u32 freq_index;
	static bool has_init = false;

	if (has_init)
		return;

	ret = get_improve_yield_nv_info(MODULE_ID_CPU, (char *)&cpu_info, sizeof(improve_yield_cpu_nv_t));
	if (ret != 0) {
		pr_err("[%s %d]: get improve yield cpu info fail, ret:%d\n", __func__, __LINE__, ret);
		return;
	}

	for (cluster = 0; cluster < CPU_CLUSTER_CNT; cluster++) {
		freq_ce_mask[cluster] = cpu_info.cluster_info[cluster].bits.freq_ce_mask;
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
#endif

static bool cpu_improve_yield(u32 cpu)
{
	bool dmd_on_off_flag = true;
#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
	u32 cluster = topology_physical_package_id(cpu);
	if (is_module_improve_yield_enable(MODULE_ID_CPU) == 1)
		dmd_on_off_flag = _cpu_improve_yield(cluster);
#endif

	return dmd_on_off_flag;
}

void ras_device_handle_ce(struct edac_device_ctl_info *edac_dev,
			  int inst_nr, int block_nr, const char *msg)
{
	struct edac_device_instance *instance = NULL;
	struct edac_device_block *block = NULL;
	struct err_record *err_data = NULL;
	struct irq_nodes *pdata = NULL;
	struct irq_node *irq_data = NULL;
	struct cpu_ecc_dfx_info dfx_info;
	int ret;

	if (!ras_nr_check(edac_dev, inst_nr, block_nr))
		return;

	instance = edac_dev->instances + inst_nr;
	if (instance->nr_blocks > 0) {
		block = instance->blocks + block_nr;
		block->counters.ce_count++;
	}

	/* Propagate the count up the 'totals' tree */
	instance->counters.ce_count++;
	edac_dev->counters.ce_count++;

	if (edac_dev->log_ce)
		edac_device_printk(edac_dev, KERN_WARNING,
				   "CE: %s instance: %s block: %s '%s'\n",
				   edac_dev->ctl_name, instance->name,
				   block ? block->name : "N/A", msg);
	pdata = edac_dev->pvt_info;
	irq_data = pdata->irq_data + inst_nr;
	err_data = irq_data->err_record + block_nr;

	read_cpu_ecc_dfx_info(&dfx_info, err_data->cpu);
	edac_device_printk(edac_dev, KERN_WARNING,"freq:%u,volt:%u-%u,temp:%d\n",
					   dfx_info.freq_mhz, dfx_info.logic_volt, dfx_info.mem_volt, dfx_info.temp);
	if (cpu_improve_yield(err_data->cpu)) {
		ret = bbox_diaginfo_record(irq_data->dmd_id[block_nr], NULL,
					"CE instance: %s cpuid:%d block %s errxstatus=0x%llx,errxmisc0=0x%llx,errxmisc1=0x%llx,freq:%u,volt:%u-%u,temp:%d",
					instance->name, err_data->cpu, block ? block->name : "N/A",
					err_data->errstatus, err_data->errmisc0, err_data->errmisc1,
					dfx_info.freq_mhz, dfx_info.logic_volt, dfx_info.mem_volt, dfx_info.temp);
		if (ret)
			edac_device_printk(edac_dev, KERN_WARNING, "bbox_diaginfo_record fail, ret : %d.\n", ret);
	}

	if (pdata->ce_panic)
		ras_device_panic(pdata, irq_data->cpu_id, (u32)CE, err_data->cpu);
}
EXPORT_SYMBOL_GPL(ras_device_handle_ce);

void ras_device_handle_de(struct edac_device_ctl_info *edac_dev,
			  int inst_nr, int block_nr, const char *msg)
{
	struct edac_device_instance *instance = NULL;
	struct edac_device_block *block = NULL;
	struct err_record *err_data = NULL;
	struct irq_nodes *pdata = NULL;
	struct irq_node *irq_data = NULL;
	struct cpu_ecc_dfx_info dfx_info;
	int ret;

	if (!ras_nr_check(edac_dev, inst_nr, block_nr))
		return;

	instance = edac_dev->instances + inst_nr;
	if (instance->nr_blocks > 0)
		block = instance->blocks + block_nr;

	edac_device_printk(edac_dev, KERN_WARNING,
			   "DE: %s instance: %s block: %s '%s'\n",
			   edac_dev->ctl_name, instance->name,
			   block ? block->name : "N/A", msg);

	pdata = edac_dev->pvt_info;
	irq_data = pdata->irq_data + inst_nr;
	err_data = irq_data->err_record + block_nr;

	read_cpu_ecc_dfx_info(&dfx_info, err_data->cpu);
	edac_device_printk(edac_dev, KERN_WARNING,"freq:%u,volt:%u-%u,temp:%d\n",
					   dfx_info.freq_mhz, dfx_info.logic_volt, dfx_info.mem_volt, dfx_info.temp);
	ret = bbox_diaginfo_record(ECC_DE, NULL,
			     "DE instance: %s cpuid:%d block %s errxstatus=0x%llx,errxmisc0=0x%llx,errxmisc1=0x%llx,freq:%u,volt:%u-%u,temp:%d",
			     instance->name, err_data->cpu, block ? block->name : "N/A",
			     err_data->errstatus, err_data->errmisc0, err_data->errmisc1,
			     dfx_info.freq_mhz, dfx_info.logic_volt, dfx_info.mem_volt, dfx_info.temp);
	if (ret)
		edac_device_printk(edac_dev, KERN_WARNING,
				   "bbox_diaginfo_record fail, ret : %d.\n", ret);
#ifdef CONFIG_RAS_EDAC_UNCORRECTED_ERROR
	ras_uncorrected_error_handle(pdata->mod_id, inst_nr, block_nr, err_data);
#endif
}
EXPORT_SYMBOL_GPL(ras_device_handle_de);

static void ras_ue_sh(struct edac_device_ctl_info *edac_dev, u32 cpu)
{
#ifdef CONFIG_DFX_SH
	int cluster = topology_physical_package_id(cpu);
	edac_device_printk(edac_dev, KERN_WARNING, "UE: SH triggering within cluster: %d.\n", cluster);
	ecc_ue_do_sh(cluster);
#endif
}

void ras_device_handle_ue(struct edac_device_ctl_info *edac_dev,
			  int inst_nr, int block_nr, const char *msg)
{
	struct edac_device_instance *instance = NULL;
	struct edac_device_block *block = NULL;
	struct err_record *err_data = NULL;
	struct irq_nodes *pdata = NULL;
	struct irq_node *irq_data = NULL;
	struct cpu_ecc_dfx_info dfx_info;
	int ret;

	if (!ras_nr_check(edac_dev, inst_nr, block_nr))
		return;

	instance = edac_dev->instances + inst_nr;
	pdata = edac_dev->pvt_info;
	irq_data = pdata->irq_data + inst_nr;
	err_data = irq_data->err_record + block_nr;

	if (is_bus_error(err_data->errstatus)) {
		edac_device_printk(edac_dev, KERN_INFO,
				   "Bus error: ACPU not secure read security.");
		return;
	}

	if (instance->nr_blocks > 0) {
		block = instance->blocks + block_nr;
		block->counters.ue_count++;
	}

	/* Propagate the count up the 'totals' tree */
	instance->counters.ue_count++;
	edac_dev->counters.ue_count++;

	if (edac_dev->log_ue)
		edac_device_printk(edac_dev, KERN_EMERG,
				   "UE: %s instance: %s block: %s '%s'\n",
				   edac_dev->ctl_name, instance->name,
				   block ? block->name : "N/A", msg);

	ras_ue_sh(edac_dev, err_data->cpu);
	read_cpu_ecc_dfx_info(&dfx_info, err_data->cpu);
	edac_device_printk(edac_dev, KERN_WARNING,"freq:%u,volt:%u-%u,temp:%d\n",
				   dfx_info.freq_mhz, dfx_info.logic_volt, dfx_info.mem_volt, dfx_info.temp);
	ret = bbox_diaginfo_record(irq_data->dmd_id[block_nr + instance->nr_blocks], NULL,
			     "UE instance: %s cpuid:%d block %s errxstatus=0x%llx,errxmisc0=0x%llx,errxmisc1=0x%llx,freq:%u,volt:%u-%u,temp:%d",
			     instance->name, err_data->cpu, block ? block->name : "N/A",
			     err_data->errstatus, err_data->errmisc0, err_data->errmisc1,
			     dfx_info.freq_mhz, dfx_info.logic_volt, dfx_info.mem_volt, dfx_info.temp);
	if (ret)
		edac_device_printk(edac_dev, KERN_WARNING,
				   "bbox_diaginfo_record fail, ret : %d.\n", ret);
#ifdef CONFIG_RAS_EDAC_UNCORRECTED_ERROR
	ras_uncorrected_error_handle(pdata->mod_id, inst_nr, block_nr, err_data);
#endif
	if (edac_dev->panic_on_ue)
		ras_device_panic(pdata, irq_data->cpu_id, (u32)UE, err_data->cpu);
}
EXPORT_SYMBOL_GPL(ras_device_handle_ue);

static int ras_device_register_exception(void)
{
	unsigned int ret;
	unsigned int i, j;

	for (i = 0; i < ARRAY_SIZE(cpu_edac_einfo); i++) {
		ret = rdr_register_exception(&cpu_edac_einfo[i]);
		/* if error, return 0; otherwise return e_modid_end */
		if (ret != cpu_edac_einfo[i].e_modid_end) {
			pr_err("register cpu edac exception failed %u\n", i);
			goto err;
		}
	}

	return 0;
err:
	for (j = 0; j < i; j++)
		(void)rdr_unregister_exception(cpu_edac_einfo[j].e_modid);
	return -EPERM;
}

static void ras_device_unregister_exception(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(cpu_edac_einfo); i++)
		(void)rdr_unregister_exception(cpu_edac_einfo[i].e_modid);
}

static int __init ras_common_init(void)
{
	int err;

	err = ras_device_register_exception();
	if (err != 0)
		goto out_exception;
#ifdef CONFIG_RAS_EDAC_UNCORRECTED_ERROR
	ras_uncorrected_error_init();
#endif
	return 0;

out_exception:
	ras_device_unregister_exception();
	return err;
}

static void __exit ras_common_exit(void)
{
	ras_device_unregister_exception();
}

late_initcall(ras_common_init);

module_exit(ras_common_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("RAS COMMON driver");
