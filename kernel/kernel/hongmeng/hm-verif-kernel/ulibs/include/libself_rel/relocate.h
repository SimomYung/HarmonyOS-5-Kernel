/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide self relocate interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 28 20:31:36 2019
 */
#ifndef ULIBS_LIBSELF_RELOCATE_H
#define ULIBS_LIBSELF_RELOCATE_H

#include <hmkernel/const.h>
#include <hmkernel/capability.h>
#include <elf.h>

#define DYN_COUNT __U(32)
#define RELOCATE_NO_DYNAMIC 0x67777777UL
#if __SIZEOF_LONG__ == 4
#define REL_NONE        R_ARM_NONE
#define REL_RELATIVE    R_ARM_RELATIVE
#define REL_TPOFF       R_ARM_TLS_TPOFF32
#define GAP_ABOVE_TP 8
#define r_type(x) ((x) & 255UL)
#else
#define r_type(x) ((x) & 0x7fffffffUL)
#define REL_NONE        R_AARCH64_NONE
#define REL_RELATIVE    R_AARCH64_RELATIVE
#define REL_TPOFF       R_AARCH64_TLS_TPREL64
#define GAP_ABOVE_TP 16
#endif

void selfreloc_relocate(uintptr_t load_start);
size_t *dynamic_of_elf(void);

#endif
