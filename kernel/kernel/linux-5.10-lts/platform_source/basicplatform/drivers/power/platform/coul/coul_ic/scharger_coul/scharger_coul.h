/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2020. All rights reserved.
 * Description: coulometer hardware driver headfile
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __SCHARGER_COUL_H__
#define __SCHARGER_COUL_H__

#include <asm/irq.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <platform_include/cee/linux/adc.h>
#include <linux/mfd/pmic_platform.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_core.h>

#define coul_hardware_err(fmt, args...) \
	printk(KERN_ERR    "[scharger_coul]" fmt, ## args)
#define coul_hardware_evt(fmt, args...) \
	printk(KERN_WARNING"[scharger_coul]" fmt, ## args)
#define coul_hardware_inf(fmt, args...) \
	printk(KERN_INFO   "[scharger_coul]" fmt, ## args)
#define coul_hardware_dbg(fmt, args...)

enum {
	CHIP_VERSION_V600 = 0,
	CHIP_VERSION_V700,
	CHIP_VERSION_V800,
	CHIP_VERSION_END,
};

#ifndef BAT_PARALLEL_MODE
#define BAT_PARALLEL_MODE 0
#define BAT_SERIES_MODE   1
#endif

#define PMU_ENABLE                              1
#define PMU_DISABLE                             0

#define LOCK                                    1
#define UNLOCK                                  0

#define R_COUL_UOHM                             10000 /* resisitance uohm */
#define INVALID_TEMP                            (-99)
#define COUL_HARDWARE                             0x36
#define COUL_PMIC6X21V700                         0x700
#define DEFAULT_BATTERY_VOL_2_PERCENT           3350
#define DEFAULT_BATTERY_VOL_0_PERCENT           3150
#define DEFAULT_I_GATE_VALUE                    5000 /* 5000 mA */
#define COUL_BIT_MASK                           0x800000
#define COUL_NEG_EXPAND                         0xff000000
#define COUL_FIFO_VOL_DEFAULT                   0xffffff
#define PERMILLAGE                              1000
#define SECONDS_PER_HOUR                        3600
#define COUL_VBAT_DROP_VTH                      150
#define COUL_VBAT_DROP_TH_GAP                   50
#define NEGATIVE_CC_OFFSET						10000

#define ADC_CALI_TIMEOUT                        3000 /* 3s */
#define ADC_DELAY                               100  /* 100ms */

#define CHIP_ID_V700                    0x7000
#define CHIP_ID_V710                    0x0007

#ifndef BIT
#define BIT(x)                          (1 << (x))
#endif

#define REG_NUM                         3
#define CC_REG_NUM                      5
#define FIFO_DEPTH                      8
#define V_DROP_FIFO_DEPTH               3
#define COUL_WORKING                    0x5
#define COUL_CALI_ING                   0x4
#define VERSION_VAL                     0x36
#define INVALID_IRQ_MASK                0
#define VBAT_EN_MASK_NUM                1
#define VBAT_TH_MASK_NUM                3
#define COUL_REFRESH_TIME_MS            240
#define BASE_INFO_PRINT_INTER           10

enum coul_cache_type {
	CC_CACHE = 0,
	VOLT_CACHE,
	CURRENT_CACHE,
	CACHE_TOTAL,
};

struct coul_data_cache {
	struct mutex lock;
	int data;
	u64 last_time;
};

struct batt_regs {
	/* read only */
	unsigned int cl_in;
	unsigned int cl_out;

	/* read & write */
	unsigned int cl_in_rw;
	unsigned int cl_out_rw;

	unsigned int cl_in_time_ro;
	unsigned int cl_out_time_ro;
	unsigned int cl_in_time_rw;
	unsigned int cl_out_time_rw;

	unsigned int vol_fifo;
	unsigned int cur_fifo;
	unsigned int v_drop_fifo;
	unsigned int state;
	unsigned int adc_cali;
	unsigned int vol;
	unsigned int cur;
	unsigned int ate_a_b;

	unsigned int eco_vol_fifo;
	unsigned int eco_cur_fifo;
	unsigned int eco_temp_fifo;

	unsigned int eco_out_clin;
	unsigned int eco_out_clout;

	unsigned int irq_mask;
	unsigned int irq_flag;

	unsigned int  offset_voltage;

	unsigned int vbat_drop_fifo;
	unsigned int vbat_drop;
	unsigned char vbat_drop_en_shift;
	unsigned char vbat_drop_th_shift;
};

struct eco_leak_info {
	int rst_uah;
	int eco_uah;
	int cur_uah;
	s64 eco_in_uah;
	s64 eco_out_uah;
	s64 present_in_uah;
	s64 present_out_uah;
	u64 in_val;
	u64 out_val;
};

struct batt_reg_val {
	unsigned char coul_cali_enable;
	unsigned char eco_coul_ctrl_val;
	unsigned char default_coul_ctrl_val;
	unsigned char mask_cali_auto_off;
	unsigned char wait_comp_en;

	unsigned char coul_clear_irq;
	unsigned char coul_vbat_drop_mask;
	unsigned char coul_i_out_mask;
	unsigned char coul_i_in_mask;
	unsigned char coul_vbat_int_mask;
	unsigned char coul_cl_in_mask;
	unsigned char coul_cl_out_mask;
	unsigned char coul_cl_int_mask;
	unsigned char coul_int_mask_all;

	unsigned char eco_fifo_clear;
	unsigned char eco_fifo_en;
	unsigned char eco_enter;
	unsigned char eco_filter_out_en;
	unsigned char data_clear;
};

struct write_protect {
	unsigned int reg;
	unsigned char lock_val;
	unsigned char unlock_val;
};

struct coul_device_info {
	struct device *dev;
	struct regmap *regmap;
	struct work_struct irq_work;
	struct delayed_work chip_cali_work;
	struct mutex ops_lock;
	struct wakeup_source *wake_lock;
	unsigned int chip_version;
	unsigned short scharger_version;
	/* bit[0] batt_L enter eco
	   bit[1] batt_H enter eco */
	unsigned char vote_count;
	int low_vol_mv;
	int irq;
	int batt_l_index;
	int batt_h_index;
	/* Registers address info */
	unsigned int rd_req;
	unsigned int version0;
	unsigned int version4;
	unsigned int irq_flag;
	unsigned int v_int;
	unsigned int i_in_gate;
	unsigned int i_out_gate;
	unsigned int bat_cnct_sel;

	unsigned int clj_ctrl;
	unsigned int clj_ctrl1;
	unsigned int clj_ctrl2;
	unsigned int clj_ctrl3;
	unsigned int clj_ctrl4;
	unsigned int coul_mstate;
	struct batt_reg_val regs_val;
	struct batt_regs h_batt;
	struct batt_regs l_batt;
	struct write_protect wp;

	struct coul_cali_params batt_h_cali_params;
	struct coul_cali_params batt_l_cali_params;

	u64 g_last_eco_in_h;
	u64 g_last_eco_out_h;
	u64 g_last_eco_in_l;
	u64 g_last_eco_out_l;

	unsigned int vbat_l_drop_en;
	unsigned int vbat_l_drop_th;
	unsigned int vbat_h_drop_en;
	unsigned int vbat_h_drop_th;
};

void coul_schargerv700_regs_cfg(struct coul_device_info *di);
void coul_schargerv800_regs_cfg(struct coul_device_info *di);

#endif
