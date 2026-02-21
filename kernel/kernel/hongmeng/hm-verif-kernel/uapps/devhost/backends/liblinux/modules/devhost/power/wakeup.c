/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of wakeup
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 11 10:50:47 UTC 2023
 */

#include <linux/device.h>
#include <linux/errno.h>
#include <linux/pm_wakeup.h>
#include <linux/securec.h>
#include <linux/version.h>

#include <lnxbase/lnxbase.h>
#include <libhmpm/wl_stat.h>

#include "devhost.h"
#include "wakeup.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0))
#define for_each_wakeup_source(ws)
#endif

extern int wakeup_sources_read_lock(void);
extern void wakeup_sources_read_unlock(int idx);
extern struct wakeup_source *liblinux_acquire_deleted_ws(void);

struct ws_time {
	ktime_t max_time;
	ktime_t total_time;
	ktime_t active_time;
	ktime_t prevent_sleep_time;
};

static void ws_time_init(struct ws_time *ws_t, struct wakeup_source *ws)
{
	ktime_t now;

	ws_t->max_time = ws->max_time;
	ws_t->total_time = ws->total_time;
	ws_t->prevent_sleep_time = ws->prevent_sleep_time;
	if (!ws->active) {
		ws_t->active_time = ktime_set(0, 0);
	} else {
		now = ktime_get();
		ws_t->active_time = ktime_sub(now, ws->last_time);
		ws_t->total_time = ktime_add(ws_t->total_time, ws_t->active_time);
		if (ktime_compare(ws_t->active_time, ws_t->max_time) > 0) {
			ws_t->max_time = ws_t->active_time;
		}
		if (ws->autosleep_enabled) {
			ws_t->prevent_sleep_time = ktime_add(ws_t->prevent_sleep_time,
							     ktime_sub(now, ws->start_prevent_time));
		}
	}
}

static int wl_stat_init(struct wl_stat *stat, struct ws_time *ws_t,
			struct wakeup_source *ws)
{
	int ret;

	ret = memcpy_s(stat->name, sizeof(stat->name), ws->name,
		       strlen(ws->name) + 1);
	if (ret != 0) {
		pr_err("memcpy stat name failed\n");
		return -EINVAL;
	}
	stat->acquired_count = (uint64_t)ws->active_count;
	stat->total_count = (uint64_t)ws->event_count;
	stat->wakeup_count = (uint64_t)ws->wakeup_count;
	stat->timeout_count = (uint64_t)ws->expire_count;
	stat->this_acquired_time = (uint64_t)ktime_to_ms(ws_t->active_time);
	stat->total_acquired_time = (uint64_t)ktime_to_ms(ws_t->total_time);
	stat->max_acquired_time = (uint64_t)ktime_to_ms(ws_t->max_time);
	stat->last_change_time = (uint64_t)ktime_to_ms(ws->last_time);
	stat->prevent_sleep_time = (uint64_t)ktime_to_ms(ws_t->prevent_sleep_time);

	return 0;
}

static int ws_stat_callback(struct wakeup_source *ws, void *args)
{
	int ret;
	unsigned long flags;
	struct ws_time ws_time;
	struct wl_stat stat;

	if (ws == NULL) {
		pr_warn("wakelock stat is not supported\n");
		return 0;
	}

	spin_lock_irqsave(&ws->lock, flags);
	ws_time_init(&ws_time, ws);
	ret = wl_stat_init(&stat, &ws_time, ws);
	if (ret != 0) {
		pr_err("wakelock stat init failed, ret=%d\n", ret);
		spin_unlock_irqrestore(&ws->lock, flags);
		return ret;
	}
	spin_unlock_irqrestore(&ws->lock, flags);

	ret = lnxbase_wakelock_stat_enqueue(&stat, args);
	if (ret != 0) {
		pr_err("wakelock stat write failed, ret=%d\n", ret);
		return ret;
	}

	return 0;
}

int platform_wakelock_stat(void *args)
{
	int ret, idx;
	struct wakeup_source *ws = NULL;

	idx = wakeup_sources_read_lock();
	for_each_wakeup_source(ws) {
		ret = ws_stat_callback(ws, args);
		if (ret != 0) {
			wakeup_sources_read_unlock(idx);
			return ret;
		}
	}
	wakeup_sources_read_unlock(idx);

	return ws_stat_callback(liblinux_acquire_deleted_ws(), args);
}
