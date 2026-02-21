/*
 * frame_window.c
 *
 * Frame window size aware
 *
 * Copyright (c) 2019-2024 Huawei Technologies Co., Ltd.
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

#include <platform_include/cee/linux/frame_window.h>
#include <linux/printk.h>
#include <linux/tick.h>
#include <linux/ktime.h>
#include <trace/events/power.h>
#include "sched.h"

#define FRAME_RATE_BASE	30
#define FRAME_RATE_MIN	30
#define FRAME_RATE_MAX	120

static unsigned int frame_window = 16666666;
static unsigned int frame_window_ms = 16;
static unsigned int frame_rate = 60;

unsigned int get_frame_window_size_ms(void)
{
	return frame_window_ms;
}

unsigned int get_frame_window_size(void)
{
	return frame_window;
}

static int update_frame_rate(struct notifier_block *nb, unsigned long action, void *data)
{
	if (!data || action != DKMD_EVENT_FRAME_RATE)
		return NOTIFY_OK;
	frame_rate = *((uint32_t *)data);

	frame_rate = frame_rate / FRAME_RATE_BASE * FRAME_RATE_BASE; /* So 75->60. */
	frame_rate = clamp(frame_rate, FRAME_RATE_MIN, FRAME_RATE_MAX);
	frame_window = NSEC_PER_SEC / frame_rate;
	frame_window_ms = frame_window / NSEC_PER_MSEC;
	trace_perf(get_frame_window_size, frame_rate, frame_window);

#ifdef CONFIG_SCHED_PRED_LOAD
	sync_predl_window_size_with_frame();
#endif

	return NOTIFY_OK;
}

static struct notifier_block frame_rate_notify = {
	.notifier_call = update_frame_rate,
};

static int frame_window_init(void)
{
	int ret;

	ret = dkmd_register_client(&frame_rate_notify);

	return ret;
}

late_initcall(frame_window_init);