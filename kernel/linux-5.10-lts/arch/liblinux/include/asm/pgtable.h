/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on arch/riscv/include/asm/pgtable.h
 */
#ifndef __LIBLINUX_ASM_PGTABLE_H
#define __LIBLINUX_ASM_PGTABLE_H

#include <linux/mmzone.h>
#include <linux/sizes.h>

#ifndef __ASSEMBLY__

/* Page Upper Directory not used */
#include <asm-generic/pgtable-nopud.h>
#include <asm/page.h>
#include <asm/tlbflush.h>
#include <linux/mm_types.h>

#ifdef CONFIG_ARM64
#  define VA_BITS		(CONFIG_ARM64_VA_BITS)
#else
#  define VA_BITS		39
#endif

extern unsigned long kmap_offset;
/* for liblinux, we can use the entire virtual address space */
#define VMALLOC_START	(0)
#define VMALLOC_END	(-1UL)

/* overwrite VMEMMAP/PAGE_OFFSET */
#define VMEMMAP_SIZE	(UL(1) << (VA_BITS - PAGE_SHIFT - 1 + STRUCT_PAGE_MAX_SHIFT))
#define VMEMMAP_START	(PAGE_OFFSET - VMEMMAP_SIZE)
#define PAGE_OFFSET	((UL(1) << (VA_BITS - 1)) + kmap_offset)
#define LIBLINUX_KMAP_SIZE	(1UL << CONFIG_LIBLINUX_KMAP_BITS)

/* BPF JIT region */
#define BPF_JIT_REGION_SIZE	(SZ_128M)
#define BPF_JIT_REGION_START	(PAGE_OFFSET - BPF_JIT_REGION_SIZE)
#define BPF_JIT_REGION_END	(VMALLOC_END)

/* fake module region */
#define MODULES_END		((unsigned long)__liblinux_modules_pool_end)
#define MODULES_VADDR		((unsigned long)__liblinux_modules_pool)
#define MODULES_VSIZE		(MODULES_END - MODULES_VADDR)

/*
 * Define vmemmap for pfn_to_page & page_to_pfn calls. Needed if kernel
 * is configured with CONFIG_SPARSEMEM_VMEMMAP enabled.
 */
#define vmemmap		((struct page *)VMEMMAP_START - (memstart_addr >> PAGE_SHIFT))

#define PCI_IO_SIZE      SZ_16M
#define PCI_IO_END       VMEMMAP_START
#define PCI_IO_START     (PCI_IO_END - PCI_IO_SIZE)

#define FIXADDR_TOP      PCI_IO_START
#ifdef CONFIG_64BIT
#define FIXADDR_SIZE     PMD_SIZE
#else
#define FIXADDR_SIZE     PGDIR_SIZE
#endif
#define FIXADDR_START    (FIXADDR_TOP - FIXADDR_SIZE)

#ifdef CONFIG_ARM64
#include <asm/pgtable-arm64.h>
#else
/* generic start */
#include <asm/pgtable-bits.h>
#ifdef CONFIG_64BIT
#include <asm/pgtable-64.h>
#else
#include <asm/pgtable-32.h>
#endif
#include <asm/pgtable-generic.h>
/* generic end */
#endif
#include <asm/pgtable-prot.h>

extern pgd_t swapper_pg_dir[];

static inline void set_pud(pud_t *pudp, pud_t pud)
{
	*pudp = pud;
}

static inline void pud_clear(pud_t *pudp)
{
	set_pud(pudp, __pud(0));
}

static inline unsigned long pud_page_vaddr(pud_t pud)
{
	return (unsigned long)pfn_to_virt(__pud_to_pfn(pud_val(pud)));
}

static inline void set_pmd(pmd_t *pmdp, pmd_t pmd)
{
	*pmdp = pmd;
}

static inline void pmd_clear(pmd_t *pmdp)
{
	set_pmd(pmdp, __pmd(0));
}

static inline struct page *pmd_page(pmd_t pmd)
{
	return pfn_to_page(__pmd_to_pfn(pmd_val(pmd)));
}

static inline unsigned long pmd_page_vaddr(pmd_t pmd)
{
	return (unsigned long)pfn_to_virt(__pmd_to_pfn(pmd_val(pmd)));
}

static inline unsigned long pte_pfn(pte_t pte)
{
	return __pte_to_pfn(pte_val(pte));
}

static inline pte_t pfn_pte(unsigned long pfn, pgprot_t prot)
{
	return __pte(__mk_pte(pfn, pgprot_val(prot)));
}
#define mk_pte(page, prot)       pfn_pte(page_to_pfn(page), prot)

#define __HAVE_ARCH_PTE_SAME
static inline int pte_same(pte_t pte_a, pte_t pte_b)
{
	return pte_val(pte_a) == pte_val(pte_b);
}

/*
 * Certain architectures need to do special things when PTEs within
 * a page table are directly modified.  Thus, the following hook is
 * made available.
 */
static inline void set_pte(pte_t *ptep, pte_t pteval)
{
	*ptep = pteval;
}

static inline void flush_icache_pte(pte_t pte)
{
	/* just do nothing */
}

static inline void set_pte_at(struct mm_struct *mm,
	unsigned long addr, pte_t *ptep, pte_t pteval)
{
	set_pte(ptep, pteval);
}

static inline void pte_clear(struct mm_struct *mm,
	unsigned long addr, pte_t *ptep)
{
	set_pte_at(mm, addr, ptep, __pte(0));
}

#define __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
static inline int ptep_set_access_flags(struct vm_area_struct *vma,
					unsigned long address, pte_t *ptep,
					pte_t entry, int dirty)
{
	if (!pte_same(*ptep, entry))
		set_pte_at(vma->vm_mm, address, ptep, entry);
	/*
	 * update_mmu_cache will unconditionally execute, handling both
	 * the case that the PTE changed and the spurious fault case.
	 */
	return true;
}

#define __HAVE_ARCH_PTEP_GET_AND_CLEAR
static inline pte_t ptep_get_and_clear(struct mm_struct *mm,
				       unsigned long address, pte_t *ptep)
{
	return __pte(atomic_long_xchg((atomic_long_t *)ptep, 0));
}

#define __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG
static inline int ptep_test_and_clear_young(struct vm_area_struct *vma,
					    unsigned long address,
					    pte_t *ptep)
{
	pte_t pte = *ptep;
	if (!pte_young(*ptep))
		return 0;

	pte = pte_mkold(pte);
	*ptep = pte;
	return pte_young(pte);
}

#define __HAVE_ARCH_PTEP_SET_WRPROTECT
static inline void ptep_set_wrprotect(struct mm_struct *mm,
				      unsigned long address, pte_t *ptep)
{
	pte_t pte = *ptep;

	pte = pte_wrprotect(pte);
	*ptep = pte;
}

#define __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH
static inline int ptep_clear_flush_young(struct vm_area_struct *vma,
					 unsigned long address, pte_t *ptep)
{
	return ptep_test_and_clear_young(vma, address, ptep);
}


#define pgd_ERROR(e) \
	pr_err("%s:%d: bad pgd " PTE_FMT ".\n", __FILE__, __LINE__, pgd_val(e))

#define pmd_ERROR(e) \
	pr_err("%s:%d: bad pmd " PTE_FMT ".\n", __FILE__, __LINE__, pmd_val(e))

#define pte_ERROR(e) \
	pr_err("%s:%d: bad pte " PTE_FMT ".\n", __FILE__, __LINE__, pte_val(e))

/* Commit new configuration to MMU hardware */
static inline void update_mmu_cache(struct vm_area_struct *vma,
	unsigned long address, pte_t *ptep)
{
	/*
	 * The kernel assumes that TLBs don't cache invalid entries, but
	 * in RISC-V, SFENCE.VMA specifies an ordering constraint, not a
	 * cache flush; it is necessary even after writing invalid entries.
	 * Relying on flush_tlb_fix_spurious_fault would suffice, but
	 * the extra traps reduce performance.  So, eagerly SFENCE.VMA.
	 */
	local_flush_tlb_page(address);
}

#define MAX_SWAPFILES_CHECK()	\
	BUILD_BUG_ON(MAX_SWAPFILES_SHIFT > __SWP_TYPE_BITS)

#define __swp_type(x)	(((x).val >> __SWP_TYPE_SHIFT) & __SWP_TYPE_MASK)
#define __swp_offset(x)	((x).val >> __SWP_OFFSET_SHIFT)
#define __swp_entry(type, offset) ((swp_entry_t) \
	{ ((type) << __SWP_TYPE_SHIFT) | ((offset) << __SWP_OFFSET_SHIFT) })

#define __pte_to_swp_entry(pte)	((swp_entry_t) { pte_val(pte) })
#define __swp_entry_to_pte(x)	((pte_t) { (x).val })

/*
 * Task size is 0x4000000000 for RV64 or 0x9fc00000 for RV32.
 * Note that PGDIR_SIZE must evenly divide TASK_SIZE.
 */
#ifdef CONFIG_64BIT
#define TASK_SIZE (PGDIR_SIZE * PTRS_PER_PGD / 2)
#else
#define TASK_SIZE FIXADDR_START
#endif

#define kern_addr_valid(addr)   (1) /* FIXME */

#define FIRST_USER_ADDRESS  0

/*
 * ZERO_PAGE is a global shared page that is always zero,
 * used for zero-mapped memory areas, etc.
 */
extern struct page *empty_zero_page;
#define ZERO_PAGE(vaddr) (empty_zero_page)

#endif /* !__ASSEMBLY__ */

#endif /* __LIBLINUX_ASM_PGTABLE_H */
