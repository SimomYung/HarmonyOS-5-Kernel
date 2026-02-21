/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: header file for battery tab health monitor
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

#ifndef __TAB_HEALTH_MONITOR_H__
#define __TAB_HEALTH_MONITOR_H__

#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/limits.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/time64.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <securec.h>

#define FIFO_MAX 5
#define DELAY_1_SECOND 1000

#define tab_monitor_debug(fmt, args...) pr_debug("[tab_health_monitor]" fmt, ## args)
#define tab_monitor_info(fmt, args...) pr_info("[tab_health_monitor]" fmt, ## args)
#define tab_monitor_warn(fmt, args...) pr_warn("[tab_health_monitor]" fmt, ## args)
#define tab_monitor_err(fmt, args...) pr_err("[tab_health_monitor]" fmt, ## args)

enum charge_state {
   BAT_PARALLEL_MODE = 0,
   BAT_SERIES_MODE,
};

enum tab_charge_status_event {
   NOT_CHARGING = 0,
   CHARGING,
   CHARGE_DONE,
};

/* 0: time0, 1: time1 */
struct batt_data {
   int fcc; // mah
   int cc_start; // mah
   int cc_end;  
   int ufsoc_start; // %
   int ufsoc_end;
   int resistance; // mΩ
   int cycle;
   int voltage_end; // mv
   int current_end; // ma
   int temp; // degree
   int fifo_vol[FIFO_MAX]; // mv
   int fifo_cur[FIFO_MAX]; // ma
};

struct charge_data {
   int ratio;
   int uisoc_start; // %
   int uisoc_end;
   int temp;
   int cycle;
   struct batt_data batt_0;
   struct batt_data batt_1;
   time64_t start_time; // second
};

struct tab_health_monitor_device {
   struct device *dev;
   struct charge_data charge_done_data;
   struct charge_data charge_series_data;
   enum tab_charge_status_event current_charge_state;
   int current_charge_mode;
   int batt_soc;
   int resistance_differnece; // mΩ
   int fifo_index;
   int valid_fifo_count;
   time64_t charge_stop_time; // second

   struct delayed_work series_charge_handle_delayed_work; 
   struct delayed_work get_cur_vol_delayed_work;

   int max_charge_done_current_ma;
   int max_charge_done_current_diff_ma;
   int max_current_diff_batt_01_ma;
   int max_voltage_diff_mv;
   int max_current_diff_ma;
   int max_series_charge_time_sec;
   int max_parallel_charge_time_sec;
   int min_parallel_dischg_time_sec;
   int min_valid_temperature;
   int max_cc_diff_gap_mah;
   int min_cc_diff_mah;
   int avg_resistance_diff_mohm;
   int series_handle_uisoc_th;
   int start_ufsoc_th;
   int max_ufsoc_diff;
   int k_ratio_soc_percent_per_mv;
   int version;

   struct notifier_block power_supply_nf;
   struct notifier_block charge_state_nf;
   struct notifier_block series_parallel_mode_nf;
};

#endif
