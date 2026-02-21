// SPDX-License-Identifier: GPL-2.0-only
#include <linux/mman.h>
#include <liblinux/pal.h>

static inline
int change_memory_common(unsigned long addr, int numpages,
			 int set_prot, int clear_prot)
{
	return liblinux_pal_set_pageattr_ex((const void *)addr, numpages,
					    set_prot, clear_prot);
}

int set_memory_ro(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages, PROT_READ, PROT_WRITE);
}

int set_memory_rw(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages, PROT_READ | PROT_WRITE, 0);
}

int set_memory_nx(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages, 0, PROT_EXEC);
}
EXPORT_SYMBOL_GPL(set_memory_nx);

int set_memory_x(unsigned long addr, int numpages)
{
	return change_memory_common(addr, numpages, PROT_EXEC, 0);
}
EXPORT_SYMBOL_GPL(set_memory_x);

int set_memory_rx(unsigned long addr, int numpages)
{
	return change_memory_common((const void *)addr, numpages, PROT_EXEC | PROT_READ, PROT_WRITE);
}
