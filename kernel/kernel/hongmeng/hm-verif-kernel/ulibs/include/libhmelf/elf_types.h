/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for elf types
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 5 20:26:31 2019
 */

#ifndef ULIBS_LIBHMELF_ELF_TYPES_H
#define ULIBS_LIBHMELF_ELF_TYPES_H

#include <elf.h>

#if (ELF_CLASS != 32) && (ELF_CLASS != 64)
#error "You should define ELF_CLASS first."
#endif

#if ELF_CLASS == 32

#define ELFCLASS	ELFCLASS32
#define Elf_Dyn		Elf32_Dyn
#define Elf_Sym		Elf32_Sym
#define Elf_Rel		Elf32_Rel
#define Elf_Rela	Elf32_Rela
#define Elf_Ehdr	Elf32_Ehdr
#define Elf_Phdr	Elf32_Phdr
#define Elf_Shdr	Elf32_Shdr
#define Elf_Nhdr	Elf32_Nhdr
#define Elf_Half	Elf32_Half
#define Elf_Dval	Elf32_Word
#define Elf_Addr	Elf32_Addr
#define Elf_Word	Elf32_Word
#define Elf_Off		Elf32_Off

#else

#define ELFCLASS	ELFCLASS64
#define Elf_Dyn		Elf64_Dyn
#define Elf_Sym		Elf64_Sym
#define Elf_Rel		Elf64_Rel
#define Elf_Rela	Elf64_Rela
#define Elf_Ehdr	Elf64_Ehdr
#define Elf_Phdr	Elf64_Phdr
#define Elf_Shdr	Elf64_Shdr
#define Elf_Nhdr	Elf64_Nhdr
#define Elf_Half	Elf64_Half
#define Elf_Dval	Elf64_Xword
#define Elf_Addr	Elf64_Addr
#define Elf_Word	Elf64_Word
#define Elf_Off		Elf64_Off

#endif
#endif
