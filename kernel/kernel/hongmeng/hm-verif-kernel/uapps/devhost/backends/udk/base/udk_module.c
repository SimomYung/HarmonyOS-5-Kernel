/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Udk module
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <udk/module.h>
#include <hongmeng/errno.h>
#include <devhost/backend.h>

int udk_request_module(const char *name)
{
	return -hmerrno2posix(devhost_pal_request_module(name));
}
