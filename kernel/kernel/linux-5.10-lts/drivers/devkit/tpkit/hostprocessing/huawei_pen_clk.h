/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2021-2021 Huawei Technologies Co., Ltd.
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
#ifndef _HUAWEI_PEN_CLK_H_
#define _HUAWEI_PEN_CLK_H_

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pwm.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/timekeeping.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <linux/pm_wakeup.h>
#include <linux/workqueue.h>
#include <linux/alarmtimer.h>
#include <linux/pinctrl/pinctrl-state.h>
#include <linux/pinctrl/consumer.h>
#include "huawei_thp.h"


enum pen_clk_status {
	CLK_DISABLE,
	CLK_ENABLE,
};

enum pen_clk_source {
	CLK_FROM_SOC = 0,
	CLK_FROM_POLYMERIC_CHIP,
};

struct thp_pen_clk {
	struct platform_device *pen_clk_dev;
	struct clk *clk;
	const char *clk_name;
	struct pinctrl *clk_ctrl;
	struct pinctrl_state *pwm_disable;
	struct pinctrl_state *pwm_enable;
	unsigned int clk_freq;
	unsigned int clk_source;
	atomic_t status;
	struct mutex clk_mutex;
};

struct thp_pen_clk_common {
	struct alarm alarm;
	ktime_t cur_ts;
	ktime_t last_ts;
	struct mutex timer_mutex;
	struct wakeup_source *pen_wake_lock;
};


#if IS_ENABLED(CONFIG_HUAWEI_THP_PEN_CLK)
int thp_pen_clk_disable(u32 panel_id);
int thp_pen_clk_enable(u32 panel_id);
void start_pen_clk_timer(void);
void stop_pen_clk_timer(void);
void pen_clk_control(int status, u32 panel_id);
void pen_clk_calibration(void);
int pen_clk_init(void);
void pen_clk_exit(void);

#else
__attribute__((weak)) int thp_pen_clk_disable(u32 panel_id)
{
	return NO_ERR;
}

__attribute__((weak)) int thp_pen_clk_enable(u32 panel_id)
{
	return NO_ERR;
}

__attribute__((weak)) void pen_clk_control(int status, u32 panel_id)
{
	return;
}

__attribute__((weak)) void start_pen_clk_timer(void)
{
	return;
}

__attribute__((weak)) void stop_pen_clk_timer(void)
{
	return;
}

__attribute__((weak)) void pen_clk_calibration(void)
{
	return;
}

__attribute__((weak)) int pen_clk_init(void)
{
	return NO_ERR;
}

__attribute__((weak)) void pen_clk_exit(void)
{
	return;
}
#endif

#endif
