/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hongmeng vdso implementation for lookup symbols.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 18 14:41:28 2021
 */

#include <elf.h>
#include <libhmsrv_sys/hm_vdso.h>
#include <libcrt/hmcrt.h>
#include <hongmeng/types.h>
#include <string.h>

#if defined(UINTPTR_MAX) && defined(UINT32_MAX) && UINTPTR_MAX == UINT32_MAX
typedef Elf32_Sym elf_sym;
typedef Elf32_Phdr elf_phdr;
typedef Elf32_Ehdr elf_ehdr;
#else
typedef Elf64_Sym elf_sym;
typedef Elf64_Phdr elf_phdr;
typedef Elf64_Ehdr elf_ehdr;
#endif

struct vdso_info {
	void *base;
	size_t *dynv;
};
static struct vdso_info g_vdso_info;

static void __fill_vdso_info(void)
{
	size_t i;
	size_t *aux = hm_crt_current_aux();
	/* auxv AT_SYSINFO_EHDR is filled with vdso base address by sysmgr when creating a new process. */
	void *base = (void *)aux[AT_SYSINFO_EHDR];

	if (base != NULL) {
		elf_ehdr *ehdr = base;
		elf_phdr *phdr = (void *)((char *)ehdr + ehdr->e_phoff);
		for (i = 0; i < ehdr->e_phnum; i++, phdr = (void *)((char *)phdr + ehdr->e_phentsize)) {
			if ((phdr->p_type == PT_LOAD) && ((phdr->p_flags & PF_X) != 0)) {
				g_vdso_info.base = (void *)((size_t)(uintptr_t)ehdr + phdr->p_offset - phdr->p_vaddr);
			} else if (phdr->p_type == PT_DYNAMIC) {
				g_vdso_info.dynv = (void *)((char *)ehdr + phdr->p_offset);
			} else {
				/* do nothing */
			}
		}
	}
}

static void *__vdso_search(const char *name)
{
	unsigned int i = 0;
	elf_sym *sym = NULL;
	void *addr = NULL;
	char *strings = NULL;
	unsigned int *hashtab = NULL;

	while (g_vdso_info.dynv[i] != DT_NULL) {
		void *value = (void *)((size_t)g_vdso_info.base + g_vdso_info.dynv[i + 1]);
		switch (g_vdso_info.dynv[i]) {
		case DT_SYMTAB:
			sym = value;
			break;
		case DT_HASH:
			hashtab = value;
			break;
		case DT_STRTAB:
			strings = value;
			break;
		default:
			break;
		}

		i += 2; /* 2:offset */
	}

	if (strings != NULL && sym != NULL && hashtab != NULL) {
		for (i = 0; i < hashtab[1]; i++) {
			if (strcmp(name, strings + sym[i].st_name) == 0) {
				addr = (void *)((size_t)g_vdso_info.base + sym[i].st_value);
				break;
			}
		}
	}
	return addr;
}

void *hm_lookup_vdso_sym(const char *name)
{
	void *addr = NULL;

	if (g_vdso_info.base == NULL) {
		__fill_vdso_info();
	}

	if (g_vdso_info.base != NULL && g_vdso_info.dynv != NULL && name != NULL) {
		addr = __vdso_search(name);
	}
	return addr;
}
