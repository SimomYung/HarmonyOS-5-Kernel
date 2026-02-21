/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 30 16:01:07 2023
 */

#ifndef UAPI_MODULE_HMPSF_TEST_MODULE_H
#define UAPI_MODULE_HMPSF_TEST_MODULE_H

#include <hmkernel/capability.h>
#include <hmkernel/cpuset.h>

#define MAX_TEST_POINT_PROG_NUM 5

/* cmd operaiton */
#define __HMPSF_TEST_EXEC	0UL
#define __HMPSF_TEST_ATTACH	1UL
#define __HMPSF_TEST_DETACH	2UL

struct hmpsf_test_detach_args_s {
	unsigned int id;
	bool		force_invalidate;
};

#endif
