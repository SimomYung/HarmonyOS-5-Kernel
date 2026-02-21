/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 09:28:44 2019
 */

#ifndef __UDKTEST_HELPER_H__
#define __UDKTEST_HELPER_H__

#include <udk/log.h>

#define PROLOGUE \
	int ret = 0, err __maybe_unused = 0

#define TCALL_EC(desc, api, err_cond, args...)                                 \
	do {                                                                   \
		err = api(args);                                               \
		if (err_cond) {                                                \
			udk_error("   fail\t%s\n", desc);                      \
			ret = -1;                                              \
			return ret;                                            \
		}                                                              \
		ret = ret;                                                     \
		udk_info("    pass\t%s\n", desc);                              \
	} while (0)

#define TCALL(desc, api, args...)                                              \
	do {                                                                   \
		TCALL_EC(desc, api, (err < 0), args);                          \
	} while (0)

#define ASSERT_ZERO(ret)					\
	do {							\
		if (ret != 0) {					\
			udk_error("return err. ret:%d\n", ret); \
			return -1;				\
		};						\
	} while(0)

#endif /* __UDKTEST_HELPER_H__ */
