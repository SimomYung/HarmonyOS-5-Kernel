/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Exported liblinux APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 13 11:15:47 2021
 */

#include <hmkernel/compiler.h>
#include <hongmeng/errno.h>
#include <libhmpm/shutdown.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_timer.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>
#include "libdh_linux.h"
#include <devhost/log.h>

/* used in hm-liblinux anon_inode.c */
EXPORT_SYMBOL(liblinux_pal_get_unused_fd_flags);
EXPORT_SYMBOL(liblinux_pal_fd_install);
EXPORT_SYMBOL(liblinux_pal_put_unused_fd);

void machine_emergency_restart(void)
{
	int ret;

	ret = hm_reboot(lnxbase_backend_args_find("reboot_reason"));
	if (ret < 0) {
		dh_panic("hm_reboot failed err: %s\n", hmstrerror(ret));
	}
}
EXPORT_SYMBOL(machine_emergency_restart);

static __u64 tocks_to_usec(__u64 tocks)
{
	int err;
	struct timespec ts;

	err = tocks_to_timespec(tocks, &ts);
	if (err < 0) {
		dh_error("tocks_to_timespec failed err: %s\n", hmstrerror(err));
		return -1ULL;
	}

	return ts.tv_sec * USEC_PER_SEC + ts.tv_nsec / NSEC_PER_USEC;
}

__u64 get_cpu_idle_time_us(int cpu, __u64 *total_time)
{
	int err;
	struct __sysctrl_cpu_stat stat = {0};

	err = hm_sysctl_cpu_stat_of(cpu, &stat);
	if (err < 0) {
		dh_error("cpustat failed err: %s\n", hmstrerror(err));
		return -1ULL;
	}

	if (total_time != NULL) {
		*total_time = tocks_to_usec(stat.idle_time + stat.run_time + stat.nice_time + stat.irq_time + stat.sys_time);
	}

	return tocks_to_usec(stat.idle_time);
}
EXPORT_SYMBOL_GPL(get_cpu_idle_time_us);
