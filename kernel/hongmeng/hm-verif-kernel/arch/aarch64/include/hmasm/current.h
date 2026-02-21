/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ARM64 use tpidrro to deliver current cpu and others
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 26 13:29:21 2024
 */

#ifndef A64_HMASM_CURRENT_H
#define A64_HMASM_CURRENT_H

#include <mapi/hmasm/current.h>
#include <hmasm/processor.h>
#include <hmkernel/strict.h>

/*
 * For ARM64 since we use tpidrro which is readonly, no need to set cpu
 * during context switching.
 */
static inline void a64_current_set_ctx(const struct arch_uctx *ctx)
{
	UNUSED(ctx);
}

static inline void a64_current_set_cpu(unsigned int cpu)
{
	unsigned long long tpidrrov = 0;
	__a64_tpidrrov_set_cpuid(&tpidrrov, cpu);
	write_sysreg("tpidrro_el0", tpidrrov);
}

#endif
