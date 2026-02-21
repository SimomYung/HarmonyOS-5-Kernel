/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Inject fault into kernel
 * Author: Huawei OS Kernel Lab
 * Create: Sep 13 2023
 */

#ifndef __UAPI_TEST_MODULE_H
#define __UAPI_TEST_MODULE_H
#include <hmasm/types.h>

enum __test_moudle_cmd_type {
	__TEST_CMD_DEFAULT = 0,
	__TEST_CMD_PAC_GET_KEY_INFO = 1,
	__TEST_CMD_UNKNOWN,
};

struct __key_info {
	__u64 key_high;
	__u64 key_low;
};

struct __pac_key {
	struct __key_info ia_key;
	struct __key_info ib_key;
	struct __key_info da_key;
	struct __key_info db_key;
	struct __key_info ga_key;
};

struct __pac_key_info {
	struct __pac_key kernel_key;
	struct __pac_key user_key;
};

#endif
