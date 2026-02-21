/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Basic interfaces of all thread models
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019
 */

#include <hongmeng/errno.h>
#include <hongmeng/compiler.h>
#include <libhmsync/thread.h>
#include <libsysif/sysmgr/api.h>
#include <hmasm/lsyscall.h>

void hm_thread_exit(int exitcode)
{
	(void)lsyscall_syscall1(__NR_exit, (long)exitcode);
}

int hm_thread_set_lt_group(int tid, int group)
{
	return actvcall_hmcall_thread_set_lt_group(tid, group);
}

int hm_thread_set_lt_boost(int tid, int boost)
{
	return actvcall_hmcall_thread_set_lt_boost(tid, boost);
}

int hm_thread_freezer_count(int tid, bool count, bool fwd)
{
	int ret;

	if (fwd) {
		/* 1  0 : Forward the transaction but not allow the callee actv to forward it again. */
		ret = actvxactcall_hmcall_thread_freezer_count(true, false, tid, count);
	} else {
		ret = actvcall_hmcall_thread_freezer_count(tid, count);
	}

	return ret;
}
