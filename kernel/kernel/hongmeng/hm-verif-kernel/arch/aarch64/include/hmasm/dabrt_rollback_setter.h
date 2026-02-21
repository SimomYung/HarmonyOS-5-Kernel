/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: data abort rollback setter
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 19 08:11:53 2022
 */

#ifndef A64_ASM_DABRT_ROLLBACK_SETTER_H
#define A64_ASM_DABRT_ROLLBACK_SETTER_H

#include <hmasm/cpu_local.h>

#ifndef __HOST_LLT__
static inline __always_inline int setup_dabrt_rollback_point(void)
{
	int err;
	struct dabrt_hook *hook = &arch_current_cpu_local()->dabrt_hook;

	hook->type = DABRT_HOOK_TYPE_ROLLBACK;

	err = dabrt_rollback_prepare(&hook->rollback_buf);

	return err;
}

static inline __always_inline void discard_dabrt_rollback_point(void)
{
	struct dabrt_hook *hook = &arch_current_cpu_local()->dabrt_hook;

	hook->type = DABRT_HOOK_TYPE_NO_FIXUP;
}
#else
int setup_dabrt_rollback_point(void);
void discard_dabrt_rollback_point(void);
#endif

#endif
