/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Aug 04 14:18:02 2019
 */

#ifndef __UDK_TEST_BASE_TESTS_H__
#define __UDK_TEST_BASE_TESTS_H__

#include <libtest/test.h>

int test_udk__net(void);
int udk_test__block(void);
int udk_test__shmlock(void);
int udk_test__unload(void);

#include "udk_test__fops.h"

extern struct test udk_tests[];
extern struct test_modules tests[];

#endif /* __UDK_TEST_BASE_TESTS_H__ */
