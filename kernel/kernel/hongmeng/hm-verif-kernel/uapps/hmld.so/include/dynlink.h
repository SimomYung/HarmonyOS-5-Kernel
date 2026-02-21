/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of macros related with reloc
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 9:24:58 2019
 */

#ifndef HMLDSO_INCLUDE_DYNLINK_H
#define HMLDSO_INCLUDE_DYNLINK_H

#include <features.h>
#include <elf.h>
#include <stdint.h>

#if __SIZEOF_LONG__ == 4
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Phdr Elf_Phdr;
typedef Elf32_Shdr Elf_Shdr;
typedef Elf32_Sym Elf_Sym;
typedef Elf32_Dyn Elf_Dyn;
#define R_TYPE(x) ((x)&255)
#define R_SYM(x) ((x)>>8)
#else
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Phdr Elf_Phdr;
typedef Elf64_Shdr Elf_Shdr;
typedef Elf64_Sym Elf_Sym;
typedef Elf64_Dyn Elf_Dyn;
#define R_TYPE(x) ((x)&0x7fffffff)
#define R_SYM(x) ((x)>>32)
#endif

#include "reloc.h"
#define REL_SYM_OR_REL (-93)

#define ldso_is_relative(x, s) (R_TYPE(x) == LDSO_REL_RELATIVE)

#define LDSO_AUX_CNT 38
#define LDSO_DYN_CNT 32

#endif
