/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition of resource info for resmgr
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 06 10:25:04 2023
 */
#ifndef SYSMGR_INTAPI_RES_RES_INFO_H
#define SYSMGR_INTAPI_RES_RES_INFO_H

#include <hmkernel/capability.h>

/*
 * ID for each resource
 */
#define RES_CONTROLLER(_x) _x##_controller_id,
enum res_id {
#include "controller.h"
	MAX_RES_COUNT,
};
#undef RES_CONTROLLER

struct res_group;

#endif
