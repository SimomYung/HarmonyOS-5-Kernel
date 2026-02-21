/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 2 15:53:35 2018
 */
#ifndef ASM_AARCH64_BOOT_H
#define ASM_AARCH64_BOOT_H

#include <hmasm/bootargs.h>
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>

extern void __head
boot_setup(struct arch_bootargs_s *bootargs, const void *early_mem,
	   void *pa_text, unsigned long kernel_size);

void boot_mmu_setup(void);
void boot_mmu_enable(void);

void arch_idmap_save(unsigned int cpuid);
void arch_install_idmap(void);

#ifdef CONFIG_WXN
void arch_enable_wxn(void);
#else
static inline void arch_enable_wxn(void) { };
#endif

#endif
