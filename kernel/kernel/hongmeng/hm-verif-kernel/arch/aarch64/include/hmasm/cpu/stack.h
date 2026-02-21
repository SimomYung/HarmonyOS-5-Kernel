/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Stack manipulation
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 29 19:44:59 2023
 */

#ifndef ARCH_AARCH64_HMASM_CPU_STACK_H
#define ARCH_AARCH64_HMASM_CPU_STACK_H

#ifndef __ASSEMBLY__

#include <hmkernel/compiler.h>

struct arch_cpu_local_s;
struct arch_uctx;

typedef void (*arch_switch_stack_to_centry_next_stage_t)(struct arch_uctx *uctx,
			    struct arch_cpu_local_s *cpu_local);

typedef void (*arch_switch_stack_to_sysmgr_next_stage_t)(const struct arch_uctx *,
			    const struct arch_uctx *, void *);

extern void __noreturn
arch_switch_stack_to_centry(const struct arch_uctx *uctx,
			    const struct arch_cpu_local_s *cpu_local,
			    arch_switch_stack_to_centry_next_stage_t next_stage);

extern void __noreturn
arch_switch_stack_to_sysmgr(const struct arch_uctx *base_uctx,
			    const struct arch_uctx *curr_uctx,
			    void __user *sysmgr_stack,
			    arch_switch_stack_to_sysmgr_next_stage_t next_stage);

extern int
arch_switch_stack_to_emrg_domain(unsigned long emrg_sp, unsigned long emrg_pc);

#endif

#endif
