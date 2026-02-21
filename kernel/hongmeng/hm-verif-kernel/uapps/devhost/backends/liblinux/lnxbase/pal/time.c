/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Get task sched time.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 2 10:40:22 2024
 */

#include <liblinux/pal.h>

#include <devhost/log.h>
#include <libsec/posix_cap.h>
#include <libhmsrv_sys/hm_timer.h>

int liblinux_pal_get_task_sched_time(struct liblinux_tcb_time *time)
{
	unsigned long flags = 0UL;
	struct sysarg_thread_stat stat;
	int err;

	if (unlikely(time == NULL)) {
		return -EINVAL;
	}

	flags |= THREAD_STAT_TIME;
	err = sysfast_thread_stat(&stat, NULL, flags);
	if (err < 0) {
		dh_error("sysfast_thread_stat failed, err=%s\n", hmstrerror(err));
		return -hmerrno2posix(err);
	}

	time->total = tocks_to_ns((uint64_t)stat.tcb_time.total);
	time->runtime = tocks_to_ns((uint64_t)stat.tcb_time.run_sys + (uint64_t)stat.tcb_time.run_usr);
	time->ready = tocks_to_ns((uint64_t)stat.tcb_time.ready);
	return 0;
}
