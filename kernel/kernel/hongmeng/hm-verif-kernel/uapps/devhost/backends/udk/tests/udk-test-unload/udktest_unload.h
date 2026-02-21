/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Define headfile of udk unload test
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 05 17:54:31 2020
 */
#ifndef __UDK_TEST_UNLOAD_H__
#define __UDK_TEST_UNLOAD_H__

#include <libtest/test.h>

#define UDK_TEST_UNLOAD_MODNAME "udktest_unload.udk"
#define UDK_TEST_UNLOAD_MODARGS "udktest_charp_param=\"OK\""
#define DEFAULT_DEVID 0
#define DEFAULT_CNODE_IDX ~0U

extern struct test udk_unload_tests[];

#endif
