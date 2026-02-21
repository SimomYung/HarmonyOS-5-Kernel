/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2028. All rights reserved.
 *
 * hisi_vbat_drop_protect.h
 *
 * driver for vbat drop protect
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef	__HISI_VBAT_DROP_PROTECT_H
#define	__HISI_VBAT_DROP_PROTECT_H

#include <linux/workqueue.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <platform_include/cee/linux/hw_vote.h>
#include <linux/notifier.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <pmic_interface.h>

#ifndef BIT
#define BIT(x)      (1 << (x))
#endif

#define VBAT_DROP_VOL_DEFAULT               3100
#define DELAYED_WORK_TIME_DEFAULT           200

#define TEMP_RARA_SIZE                      2
#define PERF_MAX_LIMIT_CNT                  3
#define PERF_STEP_LIMIT_CNT                 4

#define TEMP_SOC_TABLE_MAX_SIZE             5
#define PERMILLAGE                          1000
#define FULL_OF_PERCENT                     100
#define MAX_STR_SIZE                        64

#define VBAT_DROP_CHG                       "VBAT_DROP_CHG"
#define VBAT_DROP_SOC                       "VBAT_DROP_SOC"
#define VBAT_DROP_IRQ                       "VBAT_DROP_IRQ"
#define VBAT_DROP_ERR                       "VBAT_DROP_ERR"

#define GPU_FREQ_NAME                       "gpufreq"
#define NPU_FREQ_NAME                       "npufreq"


/* pmic reg read and write interface macro */
#define HISI_VBAT_DROP_PMIC_REG_READ(regAddr) pmic_read_reg(regAddr)
#define HISI_VBAT_DROP_PMIC_REG_WRITE(regAddr, regval)                         \
	pmic_write_reg((int)(regAddr), (int)(regval))
#define HISI_VBAT_DROP_PMIC_REGS_READ(regAddr, buf, size)                      \
	pmic_array_read((int)(regAddr), (char *)(buf), (int)(size))
#define HISI_VBAT_DROP_PMIC_REGS_WRITE(regAddr, buf, size)                     \
	pmic_array_write((int)(regAddr), (char *)(buf), (int)(size))

/* auto div core type */
enum core_channel {
	MIDDLE_CPU = 0,
	MIDDLE_CPU_PE = 1,
	BIG_CPU    = 2,
	GPU_CPU    = 3,
	NPU_CPU    = 4,
	ISP_CPU    = 5,
	ALL
};

struct vbat_drop_pmic_config_info {
	u32 irq_addr;
	u32 irq_mask;
	u32 irq_ctrl_addr;
	u32 irq_ctrl_mask;
	u32 vset_addr;
	u32 vset_mask;
	u32 vset_num;
	u32 *vset_table;
};

struct temprature_para {
	 int temp_l;
	 int temp_h;
};

struct active_perf_limit_para {
	unsigned int soc_l;
	unsigned int soc_h;
	unsigned int perf_max_limit;
};

struct passive_perf_limit_para {
	unsigned int soc_l;
	unsigned int soc_h;
	unsigned int step_val;
	unsigned int perf_lowerest_limit;
};

struct cluster_perf_limit_info {
	unsigned int  cluster_id;
	unsigned long max_perf_khz;
	unsigned long limit_perf_khz;
	unsigned int  active_perf_limit_en;
	unsigned int  passvie_perf_limit_en;
	unsigned int  limit_perf_percent;
	unsigned int  temprature_soc_table_num;
	unsigned int  active_perf_limit_para_num;
	unsigned int  passive_perf_limit_para_num;
	struct temprature_para temprature_table[TEMP_SOC_TABLE_MAX_SIZE];
	struct active_perf_limit_para   *perf_active_limit[TEMP_SOC_TABLE_MAX_SIZE];
	struct passive_perf_limit_para  *perf_passive_limit[TEMP_SOC_TABLE_MAX_SIZE];
	struct hvdev  *limit_freq_hvdev;
};

struct hisi_vbat_drop_protect_dev {
	struct device           *dev;
	struct mutex            lock;
	struct wakeup_source    *vbatt_check_lock;
	struct delayed_work     vbat_drop_irq_work;
	struct work_struct      power_supply_changed_work;
	struct notifier_block   power_supply_nf;
	struct vbat_drop_pmic_config_info  vbat_drop_pmic_config;
	struct cluster_perf_limit_info     *cluster_perf_limit[ALL];
	unsigned int            vbat_drop_vol_mv;
	unsigned int            vbat_drop_count;
	unsigned int            delayed_work_time; /* ms */
	unsigned int            batt_cap;
	unsigned int            charge_state;
	int                     vbat_drop_irq;
};

#endif /* __HISI_VBAT_DROP_PROTECT_H */
