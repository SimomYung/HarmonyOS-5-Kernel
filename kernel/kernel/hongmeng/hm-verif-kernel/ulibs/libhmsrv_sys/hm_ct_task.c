/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Define hongmeng ct lock tasks func
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 27 15:47:47 2020
 */
#include <hongmeng/errno.h>
#include <libsched/priority.h>
#include <hmkernel/sched/sched.h>

#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_ct_task.h>

#include <sched.h>

int hm_ct_lock_task(void)
{
	int ret;
	struct sched_param param_user;

	param_user.sched_priority = HM_PRIORITY_USER_MAX;
	ret =  hm_thread_sched_setscheduler(0, (int)SCHED_FIFO, &param_user);
	if (ret < 0) {
		hm_debug("sched set scheduler failed: %s\n", hmstrerror(ret));
	}

	return (ret < 0) ? ret : 0;
}

int hm_ct_unlock_task(void)
{
	int ret;
	struct sched_param param_user;

	param_user.sched_priority = HM_PRIORITY_USER_DEFAULT;
	ret =  hm_thread_sched_setscheduler(0, (int)SCHED_RR, &param_user);
	if (ret < 0) {
		hm_debug("sched set scheduler failed: %s\n", hmstrerror(ret));
	}

	return (ret < 0) ? ret : 0;
}
