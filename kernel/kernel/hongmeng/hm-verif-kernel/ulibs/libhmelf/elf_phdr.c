/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Read program header infomation
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 26 12:22:40 2020
 */

#if __SIZEOF_LONG__ == 4
#define ELF_CLASS	32
#else
#define ELF_CLASS	64
#endif
#include <libhmelf/elf.h>
#include <libhmelf/elf_types.h>
#include <hongmeng/errno.h>

int elf_phdr_info(uintptr_t load_start, unsigned int type, uintptr_t *addr,
		  size_t *memsz, size_t *align, size_t *filesz)
{
	unsigned int i;
	int rc = E_HM_OK;
	Elf_Ehdr *hdr = (Elf_Ehdr *)(uintptr_t)load_start;
	Elf_Phdr *phdr = (Elf_Phdr *)((uintptr_t)hdr->e_phoff + load_start);
	for (i = 0; i < hdr->e_phnum; i++) {
		if (phdr[i].p_type != type) {
			continue;
		}
		if (addr != NULL) {
			*addr = (uintptr_t)phdr[i].p_vaddr;
		}
		if (memsz != NULL) {
			*memsz = (size_t)phdr[i].p_memsz;
		}
		if (filesz != NULL) {
			*filesz = (size_t)phdr[i].p_filesz;
		}
		if (align != NULL) {
			*align = (size_t)phdr[i].p_align;
		}
		break;
	}
	if (i == hdr->e_phnum) {
		rc = E_HM_NOOBJ;
	}
	return rc;
}
