/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Interface of lookup symbol virtual address
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 20 15:43:06 2020
 */

#ifndef ULIBS_INCLUDE_LIBHMELF_VSYMBOL_H
#define ULIBS_INCLUDE_LIBHMELF_VSYMBOL_H

#include <stddef.h>

struct vsymbol_info {
	const char *fname; /* symbol in which binary file */
	const char *sname; /* symbol name */
	size_t sname_size;
	unsigned long addr; /* symbol vspace address */
	unsigned long size; /* symbol size in byte */
	unsigned long offset; /* symbol offset */
	unsigned long load_offset; /* load offset of this binary file */
};

int hm_elf_lookup_vsymbol_by_name(const char *filename,
				  const char *name,
				  struct vsymbol_info *info);
int hm_elf_lookup_vsymbol_by_addr(unsigned long addr,
				  struct vsymbol_info *info);
#endif
