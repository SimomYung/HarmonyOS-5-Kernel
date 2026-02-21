/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on arch/riscv/include/asm/page.h
 */
#ifndef __LIBLINUX_ASM_PAGE_H
#define __LIBLINUX_ASM_PAGE_H

#include <linux/pfn.h>
#include <linux/const.h>

/* liblinux: need page->virtual */
#define WANT_PAGE_VIRTUAL

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	(12)
#define PAGE_SIZE	(_AC(1, UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE - 1))

#ifndef __ASSEMBLY__

#define PAGE_UP(addr)	(((addr)+((PAGE_SIZE)-1))&(~((PAGE_SIZE)-1)))
#define PAGE_DOWN(addr)	((addr)&(~((PAGE_SIZE)-1)))

/* align addr on a size boundary - adjust address up/down if needed */
#define _ALIGN_UP(addr, size)	(((addr)+((size)-1))&(~((size)-1)))
#define _ALIGN_DOWN(addr, size)	((addr)&(~((size)-1)))

extern void copy_page(void *to, const void *from);
extern void clear_page(void *to);

#define clear_user_page(page, vaddr, pg)	clear_page(page)
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)

/*
 * pgtable type definiations
 */
#include <asm/pgtable-types.h>

typedef struct page *pgtable_t;
extern int pfn_valid(unsigned long);

#define __pte(x) ((pte_t) { (x) } )
#define __pgd(x) ((pgd_t) { (x) } )
#define __pgprot(x)     ((pgprot_t) { (x) } )

#ifdef CONFIG_64BIT
#define PTE_FMT "%016lx"
#else
#define PTE_FMT "%08lx"
#endif

/*
 * convert virt/phys/page
 */
extern s64		memstart_addr;
#define PHYS_OFFSET	({ VM_BUG_ON(memstart_addr & 1); memstart_addr; })

#define PAGE_END (PAGE_OFFSET + LIBLINUX_KMAP_SIZE)

#define __pa_to_va_nodebug(x) ((unsigned long)((x) - PHYS_OFFSET) + PAGE_OFFSET)
#define __va_to_pa_nodebug(x) (((phys_addr_t)(x) - PAGE_OFFSET) + PHYS_OFFSET)

#ifdef CONFIG_DEBUG_VIRTUAL
extern phys_addr_t __virt_to_phys(unsigned long x);
extern phys_addr_t __phys_addr_symbol(unsigned long x);
#else
#define __virt_to_phys(x)	__va_to_pa_nodebug(x)
#define __phys_addr_symbol(x)	__va_to_pa_nodebug(x)
#endif /* CONFIG_DEBUG_VIRTUAL */

#define __pa_symbol(x)	__phys_addr_symbol(RELOC_HIDE((unsigned long)(x), 0))
#define __pa(x)		__virt_to_phys((unsigned long)(x))
#define __va(x)		((void *)__pa_to_va_nodebug((phys_addr_t)(x)))

#define phys_to_pfn(phys)	(PFN_DOWN(phys))
#define pfn_to_phys(pfn)	(PFN_PHYS(pfn))

#define virt_to_pfn(vaddr)	(phys_to_pfn(__pa(vaddr)))
#define pfn_to_virt(pfn)	(__va(pfn_to_phys(pfn)))

#define virt_to_page(vaddr)	(pfn_to_page(virt_to_pfn(vaddr)))
#define page_to_virt(page)	(pfn_to_virt(page_to_pfn(page)))

#define page_to_phys(page)	(pfn_to_phys(page_to_pfn(page)))
#define page_to_bus(page)	(page_to_phys(page))
#define phys_to_page(paddr)	(pfn_to_page(phys_to_pfn(paddr)))

struct vm_area_struct;
struct pfn_range;
int remap_pfn_range_batch(struct vm_area_struct *, unsigned long addr,
			  const struct pfn_range *array, unsigned long cnt, pgprot_t);
int remap_pfn_range_batch_sp(struct vm_area_struct *vma, unsigned long addr,
			  const struct pfn_range *array, unsigned long cnt, pgprot_t prot);

void *__populate_vmemmap(unsigned long long pa, unsigned long size, unsigned long zone);
void __prepare_pages_info(void *addr, size_t size, unsigned long zone);
struct page *__alloc_pages_internal(gfp_t gfp_mask, unsigned order);
void __free_pages_internal(struct page *page, unsigned int order);
extern struct page *alloc_pt_page(unsigned int order);
extern void free_pt_page(struct page *page, unsigned int order);
extern struct page *alloc_pgd_page(unsigned int order);
extern void free_pgd_page(struct page *page, unsigned int order);

#endif /* !__ASSEMBLY__ */

#define __is_lm_address(addr)	(((u64)(addr) - PAGE_OFFSET) < (PAGE_END - PAGE_OFFSET))
#define virt_addr_valid(addr)	({				\
	__is_lm_address(addr) && pfn_valid(virt_to_pfn(addr));	\
})

#include <asm-generic/memory_model.h>
#include <asm-generic/getorder.h>

#ifdef CONFIG_MM_LB
#ifdef CONFIG_MM_LB_GID_PBHA
#define PGPROT_PBHA_SHIFT  (59)
#define PGPROT_LB_MASK 0x7800000000000000ULL
#define PGPROT_GID(pgprot) (((pgprot) & PGPROT_LB_MASK) >> PGPROT_PBHA_SHIFT)
#define page_to_pfn_lb(page, gid) page_to_pfn(page)
#define page_to_phys_lb(page, gid) page_to_phys(page)
#else
#define page_to_pfn_lb(page, gid) (page_to_pfn(page) | ((gid) << (36 - PAGE_SHIFT)))
#define page_to_phys_lb(page, gid) (page_to_phys(page) | ((gid) << 36))
#define PGPROT_LB_MASK 0xf000000000
#define PGPROT_GID(pgprot) (((pgprot) & PGPROT_LB_MASK) >> 36)
#define PROT_LB(gid)   ((gid) << 20)
#define PROT_GID(prot) (((prot) >> 20) & 0xf)
#endif
#ifdef CONFIG_MM_LB_FULL_COHERENCY
#define PGPROT_SH_SHIFT  (8)
#define PGPROT_SH_MASK 0x300ULL
#define PGPROT_SH(pgprot) (((pgprot) & PGPROT_SH_MASK) >> PGPROT_SH_SHIFT)
#endif
#endif

#endif /* __LIBLINUX_ASM_PAGE_H */
