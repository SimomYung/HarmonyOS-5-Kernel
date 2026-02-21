/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 2 09:58:12 2018
 */
#ifndef A64_ASM_PAGEFAULT_H
#define A64_ASM_PAGEFAULT_H

#include <hmkernel/compiler.h>
#include <hmasm/registers.h>
#include <hmasm/ucontext.h>
#include <hmasm/cpu_local.h>

extern void
kernel_data_abort(struct arch_regs *regs, u64 esr, u64 far, bool is_from_priv_el1);
extern void
spinhdlr_handle_pagefault(struct arch_uctx *uctx, struct arch_cpu_local_s *cpu_local,
			  void *uspinhdlr_pc);
#endif
