/*
* Copyright (c) Huawei Technologies Co., Ltd. 2022- All rights reserved.
* Description: Implement
* Author: Huawei OS Kernel Lab
* Create: Thu Mar 17 17:26:07 2022
*/
#include <devhost/log.h>
#include <lnxbase/lnxbase.h>
#include <libsysif/devmgr/api.h>
#include <libdevhost/devhost.h>

#include "../../../utils/utils.h"

int lnxbase_dev_synchronize(void)
{
	int err;

	err = actvcapcall_drvcall_devmgr_mgmt_sync(libdh_get_devmgr_ap());
	if (err < 0) {
		dh_error("cpustat failed err: %s\n", hmstrerror(err));
	}

	return err;
}

struct lnxbase_invoke_args {
	void (*callback)(void *);
	void *data;
	unsigned int cpu;
};

static void __invoke_helper(void *data)
{
	struct lnxbase_invoke_args *args = (struct lnxbase_invoke_args *)data;
	unsigned long mask;
	int ret = 0;

	liblinux_pal_thread_set_slv(NULL, THREAD_SCHED_LEVEL_MAX);
	ret = liblinux_pal_thread_getaffinity(NULL, &mask);
	if (ret == 0) {
		ret = liblinux_pal_thread_setaffinity(NULL, 1UL << args->cpu);
	}
	if (ret == 0) {
		args->callback(args->data);
	}
	if (ret == 0) {
		ret = liblinux_pal_thread_setaffinity(NULL, mask);
	}
	liblinux_pal_thread_set_slv(NULL, 0);
	if (ret < 0) {
		dh_error("invoke on cpu#%u failed, ret=%d\n", args->cpu, ret);
	}
	free(args);
}

void lnxbase_invoke_on_cpu(unsigned int cpu,
			   void (*callback)(void *), void *data)
{
	struct lnxbase_invoke_args *args;
	/* free in __invoke_helper */
	args = (struct lnxbase_invoke_args *)malloc(sizeof(*args));
	if (args == NULL) {
		dh_error("malloc failed, message to cpu %u will be dropped\n",
			 cpu);
		return;
	}

	args->callback = callback,
	args->data = data,
	args->cpu = cpu,
	devhost_async_schedule(__invoke_helper, args);
}
