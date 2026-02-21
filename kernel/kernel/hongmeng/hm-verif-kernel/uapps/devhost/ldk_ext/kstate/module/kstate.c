/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of kstate
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 22:19:36 CST 2023
 */

#include <linux/file.h>
#include <linux/module.h>
#include <linux/version.h>

#include "kstate_api.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kstate ops");
MODULE_VERSION("0.01");

static int binderinfo_notify(int callingpid, int calledpid)
{
	return hwbinderinfo(callingpid, calledpid);
}

static int killinfo_notify(int pid, int seg)
{
	return hwkillinfo(pid, seg);
}

static int freeze_binder_info_notify(struct kstate_binder_info *info)
{
	return hw_freeze_binder_info(info);
}

static int request_anco_thaw(char *reason, uint32_t size)
{
	return hw_request_anco_thaw(reason, size);
}

static int __init devhost_kstate_init(void)
{
	devhost_kstate_ops_register(binderinfo_notify, killinfo_notify);
	devhost_freeze_binder_info_ops_register(freeze_binder_info_notify);
	devhost_request_anco_thaw_ops_register(request_anco_thaw);

	return 0;
}

module_init(devhost_kstate_init);
