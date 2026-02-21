/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 13 16:19:48 2020
 */
#ifndef AARCH64_ASM_BOOT_SYSPROC_H
#define AARCH64_ASM_BOOT_SYSPROC_H

#include <hmkernel/boot/sysproc.h>

#include <hmasm/registers.h>

struct pgentry_vpgdreg;
void arch_sysproc_fill_bootargs(const struct sysproc_init_regs *init_regs,
				bool is_sysmgr, struct pgentry_vpgdreg *pgd);

#endif
