/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ops for hongmeng ulibs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 19 08:34:53 2023
 */

#include <hongmeng/hongmeng.h>
#include <hmkernel/capability.h>

static bool g_is_fork_allowed = true;

void hm_set_fork_not_allowed(void)
{
	g_is_fork_allowed = false;
}

bool hm_fetch_fork_allowd_flag(void)
{
	return g_is_fork_allowed;
}
