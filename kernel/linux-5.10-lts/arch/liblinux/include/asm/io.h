/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBILNUX_ASM_IO_H
#define __LIBILNUX_ASM_IO_H

#include <linux/pgtable.h>

#define KMAP_VIRT_START PAGE_OFFSET
#define KMAP_VIRT_END ((unsigned long)PAGE_OFFSET + LIBLINUX_KMAP_SIZE)

enum ioremap_mode {
	MODE_DEVICE,
	MODE_NORMAL_NC,
	MODE_NORMAL_CACHE,
};

extern void __iomem *__ioremap(phys_addr_t phys_addr, size_t size, pgprot_t prot,
			       enum ioremap_mode mode);

/*
 * String version of I/O memory access operations.
 */
extern void __memcpy_fromio(void *, const volatile void __iomem *, size_t);
extern void __memcpy_toio(volatile void __iomem *, const void *, size_t);
extern void __memset_io(volatile void __iomem *, int, size_t);

#define memset_io(c,v,l)	__memset_io((c),(v),(l))
#define memcpy_fromio(a,c,l)	__memcpy_fromio((a),(c),(l))
#define memcpy_toio(c,a,l)	__memcpy_toio((c),(a),(l))

#define ioremap(addr, size)		__ioremap((addr), (size), PAGE_KERNEL, MODE_DEVICE)
#define ioremap_wc(addr, size)		__ioremap((addr), (size), PAGE_KERNEL, MODE_NORMAL_NC)
#define ioremap_wt(addr, size)		__ioremap((addr), (size), PAGE_KERNEL, MODE_NORMAL_NC)
#define ioremap_cache(addr, size)	__ioremap((addr), (size), PAGE_KERNEL, MODE_NORMAL_CACHE)

extern void iounmap(volatile void __iomem *addr);
#define iounmap iounmap

#define virt_to_phys virt_to_phys
static inline unsigned long virt_to_phys(const volatile void *x)
{
	/* check virt addr validity first */
	VM_BUG_ON(x != NULL &&
		((unsigned long)(x) < (unsigned long)KMAP_VIRT_START || (unsigned long)(x) >= KMAP_VIRT_END));
	return __pa((unsigned long)(x));
}

#define phys_to_virt phys_to_virt
static inline void *phys_to_virt(phys_addr_t x)
{
	return __va(x);
}

#ifdef CONFIG_ARM64
#include <asm/io_arm64.h>
#endif

/*
 * I/O port access
 */
#define arch_has_dev_port()	(1)
#define IO_SPACE_LIMIT		(PCI_IO_SIZE - 1)
#define PCI_IOBASE		((void __iomem *)PCI_IO_START)

#include <asm-generic/io.h>

#endif /* __LIBILNUX_ASM_IO_H */
