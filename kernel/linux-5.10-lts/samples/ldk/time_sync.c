// SPDX-License-Identifier: GPL-2.0
#include <asm/timex.h>
#include <linux/rtc.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/spinlock.h>
#include <linux/seqlock.h>
#include <linux/time.h>
#include <linux/clocksource.h>
#include <linux/timekeeping.h>
#include <linux/timekeeper_internal.h>

#include <trace/hooks/liblinux.h>

#include <liblinux/pal.h>

#include "time.h"

#define TK_MIRROR		(1 << 1)
#define TK_CLOCK_WAS_SET	(1 << 2)

typedef void (*tk_update_func_t)(struct timekeeper *, unsigned int);
typedef void (*tk_set_wtm_func_t)(struct timekeeper *, struct timespec64);

static seqcount_raw_spinlock_t *g_seq = NULL;
static struct timekeeper *g_tk = NULL;
static raw_spinlock_t *g_tk_lock = NULL;
static tk_update_func_t g_tk_update;
static tk_set_wtm_func_t g_tk_set_wtm;

static void tk_set_xtime(struct timekeeper *tk, const struct timespec64 *ts)
{
	tk->xtime_sec = ts->tv_sec;
	tk->tkr_mono.xtime_nsec = (u64)ts->tv_nsec << tk->tkr_mono.shift;
}

static void tk_set_mono_raw(struct timekeeper *tk, struct liblinux_time_udata *tdata)
{
	tk->raw_sec = tdata->mono_sec;
	tk->tkr_raw.xtime_nsec = tdata->mono_nsec << tk->tkr_raw.shift;
}

extern void clock_was_set_delayed(void);

static void liblinux_timekeeper_sync(struct liblinux_time_udata *tdata)
{
	struct timekeeper *tk = g_tk;
	struct timespec64 real;
	unsigned long flags;
	struct rtc_time rtc_current_rtc_time = {0};
	unsigned long rtc_current_time;

	sys_tz.tz_minuteswest = tdata->tz_minuteswest;
	sys_tz.tz_dsttime = tdata->tz_dsttime;

	raw_spin_lock_irqsave(g_tk_lock, flags);
	write_seqcount_begin(g_seq);

	real = ktime_to_timespec64(tdata->real);
	tk_set_xtime(g_tk, &real);
	g_tk_set_wtm(tk, ktime_to_timespec64(0 - tdata->offs_real));
	tk_set_mono_raw(g_tk, tdata);
	tk->offs_boot = tdata->offs_boot;
	tk->tkr_mono.cycle_last = tdata->cycle_last;
	tk->tkr_raw.cycle_last = tdata->cycle_last;
	g_tk_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);

	write_seqcount_end(g_seq);
	raw_spin_unlock_irqrestore(g_tk_lock, flags);
	/* Have to call _delayed version to avoid ABBA-deadlock. */
	clock_was_set_delayed();
}

void liblinux_time_sync(void)
{
	struct liblinux_time_udata data;
	int ret;

	ret = liblinux_read_time_udata(&data);
	if (ret < 0) {
		pr_err("read time data failed\n");
		return;
	}

	liblinux_timekeeper_sync(&data);

	pmu_rtc_synctime(data.rtc_lastts, data.rtc_offset);
}

void liblinux_time_sync_early(void)
{
	struct liblinux_time_udata data;
	int ret;

	ret = liblinux_read_time_udata(&data);
	if (ret < 0) {
		pr_err("read time data failed\n");
		return;
	}

	liblinux_timekeeper_sync(&data);
}

void arch_time_sync_early(void)
{
	liblinux_time_sync_early();
}

void arch_time_sync(void)
{
	liblinux_time_sync();
}

static void hook_tk_init(void *args, struct timekeeper *tk, void *seq,
			 void *tk_lock, tk_update_func_t tk_update,
			 tk_set_wtm_func_t tk_set_wtm)
{
	g_tk = tk;
	g_seq = (seqcount_raw_spinlock_t *)seq;
	g_tk_lock = (raw_spinlock_t *)tk_lock;
	g_tk_update = tk_update;
	g_tk_set_wtm = tk_set_wtm;
}
INIT_VENDOR_HOOK(ldk_vh_tk_init, hook_tk_init);
