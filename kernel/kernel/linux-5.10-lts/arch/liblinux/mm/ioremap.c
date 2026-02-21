// SPDX-License-Identifier: GPL-2.0-only
#include <linux/export.h>
#include <linux/mman.h>
#include <linux/io.h>

#include <asm/memory.h>
#include "mm_internal.h"

void __iomem *__ioremap(phys_addr_t phys_addr, size_t size, pgprot_t prot,
			enum ioremap_mode mode)
{
	void __iomem *addr = NULL;
	unsigned long offset = phys_addr & ~PAGE_MASK;
	unsigned long last_addr;
	unsigned long posix_prot = __pgprot_to_posix_prot(prot);
	unsigned int pg_flag;

	/*
	 * Page align the mapping address and size, taking account of any
	 * offset.
	 */
	phys_addr &= PAGE_MASK;
	size = PAGE_ALIGN(size + offset);

	/*
	 * Don't allow wraparound, zero size or outside PHYS_MASK.
	 */
	last_addr = phys_addr + size - 1;
	if (!size || last_addr < phys_addr || (last_addr & ~PHYS_MASK))
		return NULL;

	if (mode == MODE_DEVICE) {
		pg_flag = LIBLINUX_PAL_REMAP_DEVICE;
	} else if (mode == MODE_NORMAL_NC) {
		pg_flag = LIBLINUX_PAL_REMAP_NORMAL_NC;
	} else if (mode == MODE_NORMAL_CACHE) {
		pg_flag = LIBLINUX_PAL_REMAP_CACHE;
	} else {
		pg_flag = LIBLINUX_PAL_REMAP_NONE;
	}

	addr = liblinux_pal_ioremap_ex(phys_addr, size, posix_prot, pg_flag,
				       __builtin_return_address(0));

	return addr ? addr + offset : NULL;
}
EXPORT_SYMBOL(__ioremap);

void iounmap(volatile void __iomem *io_addr)
{
	unsigned long addr = (unsigned long)io_addr & PAGE_MASK;
	liblinux_pal_vm_unmap((void *)addr);
}
EXPORT_SYMBOL(iounmap);

void __init __iomem *
early_ioremap(resource_size_t phys_addr, unsigned long size)
{
	return ioremap(phys_addr, size);
}

void __init early_iounmap(void __iomem *addr, unsigned long size)
{
	return iounmap(addr);
}

unsigned long __pgprot_to_posix_prot(pgprot_t pgprot)
{
#ifdef CONFIG_ARM64
#define pte_kernel_exec(pte)	(!(pte_val(pte) & PTE_PXN))

	unsigned long prot = PROT_READ;
	pte_t pte = __pte(pgprot_val(pgprot));
	if (pte_none(pte))
		return PROT_NONE;
	if (pte_write(pte))
		prot |= PROT_WRITE;
	if (pte_user_exec(pte) || pte_kernel_exec(pte))
		prot |= PROT_EXEC;
	return prot;
#else
#error "support arm64 only"
#endif
}

int ioremap_page_range(unsigned long addr,
		       unsigned long end, phys_addr_t phys_addr, pgprot_t prot)
{
	int flags = LIBLINUX_PAL_REMAP_DEVICE;

	if ((addr >= end) ||
	    (!(PAGE_ALIGNED(addr) && PAGE_ALIGNED(end) && PAGE_ALIGNED(phys_addr)))) {
		return -EINVAL;
	}
	if (pgprot_val(pgprot_writecombine(prot)) == pgprot_val(prot)) {
		flags = LIBLINUX_PAL_REMAP_NORMAL_NC;
	}
	return liblinux_pal_vm_mmap(phys_addr, addr, end - addr, __pgprot_to_posix_prot(prot), flags);
}
EXPORT_SYMBOL_GPL(ioremap_page_range);
