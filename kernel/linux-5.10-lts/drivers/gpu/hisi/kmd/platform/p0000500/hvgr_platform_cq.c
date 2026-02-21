/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "hvgr_platform_api.h"

uint64_t hvgr_platform_get_sch_unbind_timeout(void)
{
	return 2500ULL; /* ms */
}

uint64_t hvgr_platform_get_sch_cl_unbind_timeout(void)
{
	return 3000ULL; /* ms */
}

uint64_t hvgr_platform_get_sch_fence_timeout(void)
{
	return 1000000000ULL; /* ns */
}
