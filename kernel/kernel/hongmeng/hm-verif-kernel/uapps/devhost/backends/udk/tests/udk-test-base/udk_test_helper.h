/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 20 17:49:32 2019
 */

#ifndef __UDK_TEST_BASE_HELPER_H__
#define __UDK_TEST_BASE_HELPER_H__

#include <stdio.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <hongmeng/compiler.h>

#include "setup.h"

struct ioctl_test_arg {
	int a;
};

int udk_test_device_ioctl(int cmd);
int udk_test_setup(void);
int udk_test_teardown(void);

#endif /* __UDK_TEST_BASE_HELPER_H__ */
