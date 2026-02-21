/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 18 18:48:37 2019
 */
#ifndef __UDK_TEST_NET_H__
#define __UDK_TEST_NET_H__

#include <libtest/test.h>

#define ASSERT_ZERO(ret)					\
	do {							\
		if (ret != 0) {					\
			hm_error("return err. ret:%d\n", ret);	\
			return -1;				\
		};						\
	} while(0)

#define UDK_TEST_NET_MODNAME "udktest_net.udk"
#define DEFAULT_DEVID 0

extern struct test net_tests[];

int setup_test_process_net(void);

#endif
