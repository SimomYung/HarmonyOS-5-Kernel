/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Implementation of timerfd
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 21 15:52:05 2020
 */

#include <vfs.h>
#include <fd.h>

int vfs_timerfd_next_alarm_pid_and_name(int *pid_rp, char *name_rp, int name_size)
{
	int err;

	err = actvcap_fscall(vfs_timerfd_next_alarm_pid_and_name, pid_rp, name_rp, name_size);
	if (err < 0) {
		hm_debug("fs call timerfd_next_alarm_pid_and_name failed: %s\n", hmstrerror(err));
	}

	return err;
}

void vfs_timerfd_cancel(void)
{
	int err;

	err = actvcap_fscall(vfs_timerfd_cancel);
	if (err < 0) {
		hm_debug("fs call timerfd_cancel failed: %s\n", hmstrerror(err));
	}
}
