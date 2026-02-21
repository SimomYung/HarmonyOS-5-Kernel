/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: spinhdlr arch entry
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 30 09:40:54 2023
 */

#ifndef A64_HMASM_SPINHDLR_H
#define A64_HMASM_SPINHDLR_H

#include <hmkernel/compiler.h>

struct arch_uctx;
struct arch_cpu_local_s;
struct actv_s;

JAM_INLINE void arch_spinhdlr_entry(struct actv_s *base_actv, struct actv_s *curr_actv,
				    struct arch_cpu_local_s *cpu_local, void *uspinhdlr_pc);
JAM_INLINE void
arch_lsyscall_spinhdlr_entry(struct actv_s *base_actv, struct actv_s *curr_actv,
			     struct arch_cpu_local_s *cpu_local, void *uspinhdlr_pc);

#endif
