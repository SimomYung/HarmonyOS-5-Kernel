/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_PGTABLE_ARM64_H
#define __LIBLINUX_ASM_PGTABLE_ARM64_H

/*
 * Encode and decode a swap entry:
 *	bits 0-1:	present (must be zero)
 *	bits 2-7:	swap type
 *	bits 8-57:	swap offset
 *	bit  58:	PTE_PROT_NONE (must be zero)
 */
#define __SWP_TYPE_SHIFT	2
#define __SWP_TYPE_BITS		6
#define __SWP_OFFSET_BITS	50
#define __SWP_TYPE_MASK		((1 << __SWP_TYPE_BITS) - 1)
#define __SWP_OFFSET_SHIFT	(__SWP_TYPE_BITS + __SWP_TYPE_SHIFT)
#define __SWP_OFFSET_MASK	((1UL << __SWP_OFFSET_BITS) - 1)


#define PTRS_PER_PTE	(1 << (PAGE_SHIFT - 3))

#define PGDIR_SHIFT	30
#define PGDIR_SIZE	(_AC(1, UL) << PGDIR_SHIFT)
#define PGDIR_MASK	(~(PGDIR_SIZE - 1))
#define PTRS_PER_PGD	(1 << (VA_BITS - PGDIR_SHIFT))

#define PMD_SHIFT	21
#define PMD_SIZE	(_AC(1, UL) << PMD_SHIFT)
#define PMD_MASK	(~(PMD_SIZE - 1))
#define PTRS_PER_PMD	PTRS_PER_PTE

/*
 * Hardware page table definitions.
 *
 * Level 1 descriptor (PUD).
 */
#define PUD_TYPE_TABLE		(_AT(pudval_t, 3) << 0)
#define PUD_TABLE_BIT		(_AT(pudval_t, 1) << 1)
#define PUD_TYPE_MASK		(_AT(pudval_t, 3) << 0)
#define PUD_TYPE_SECT		(_AT(pudval_t, 1) << 0)
#define PUD_SECT_RDONLY		(_AT(pudval_t, 1) << 7)		/* AP[2] */

/*
 * Level 2 descriptor (PMD).
 */
#define PMD_TYPE_MASK		(_AT(pmdval_t, 3) << 0)
#define PMD_TYPE_TABLE		(_AT(pmdval_t, 3) << 0)
#define PMD_TYPE_SECT		(_AT(pmdval_t, 1) << 0)
#define PMD_TABLE_BIT		(_AT(pmdval_t, 1) << 1)

/*
 * Section
 */
#define PMD_SECT_VALID		(_AT(pmdval_t, 1) << 0)
#define PMD_SECT_USER		(_AT(pmdval_t, 1) << 6)		/* AP[1] */
#define PMD_SECT_RDONLY		(_AT(pmdval_t, 1) << 7)		/* AP[2] */
#define PMD_SECT_S		(_AT(pmdval_t, 3) << 8)
#define PMD_SECT_AF		(_AT(pmdval_t, 1) << 10)
#define PMD_SECT_NG		(_AT(pmdval_t, 1) << 11)
#define PMD_SECT_CONT		(_AT(pmdval_t, 1) << 52)
#define PMD_SECT_PXN		(_AT(pmdval_t, 1) << 53)
#define PMD_SECT_UXN		(_AT(pmdval_t, 1) << 54)

/*
 * AttrIndx[2:0] encoding (mapping attributes defined in the MAIR* registers).
 */
#define PMD_ATTRINDX(t)		(_AT(pmdval_t, (t)) << 2)
#define PMD_ATTRINDX_MASK	(_AT(pmdval_t, 7) << 2)

#define PMD_PRESENT_INVALID	(_AT(pteval_t, 1) << 59) /* only when !PMD_SECT_VALID */

/*
 * Level 3 descriptor (PTE).
 */
#define PTE_VALID		(_AT(pteval_t, 1) << 0)
#define PTE_TYPE_MASK		(_AT(pteval_t, 3) << 0)
#define PTE_TYPE_PAGE		(_AT(pteval_t, 3) << 0)
#define PTE_TABLE_BIT		(_AT(pteval_t, 1) << 1)
#define PTE_USER		(_AT(pteval_t, 1) << 6)		/* AP[1] */
#define PTE_RDONLY		(_AT(pteval_t, 1) << 7)		/* AP[2] */
#define PTE_SHARED		(_AT(pteval_t, 3) << 8)		/* SH[1:0], inner shareable */
#define PTE_AF			(_AT(pteval_t, 1) << 10)	/* Access Flag */
#define PTE_NG			(_AT(pteval_t, 1) << 11)	/* nG */
#define PTE_GP			(_AT(pteval_t, 1) << 50)	/* BTI guarded */
#define PTE_DBM			(_AT(pteval_t, 1) << 51)	/* Dirty Bit Management */
#define PTE_CONT		(_AT(pteval_t, 1) << 52)	/* Contiguous range */
#define PTE_PXN			(_AT(pteval_t, 1) << 53)	/* Privileged XN */
#define PTE_UXN			(_AT(pteval_t, 1) << 54)	/* User XN */

/*
 * AttrIndx[2:0] encoding (mapping attributes defined in the MAIR* registers).
 */
#define PTE_ATTRINDX(t)		(_AT(pteval_t, (t)) << 2)
#define PTE_ATTRINDX_MASK	(_AT(pteval_t, 7) << 2)

/*
 * Software defined PTE bits definition.
 */
#define PTE_WRITE		(PTE_DBM)		 /* same as DBM (51) */
#define PTE_DIRTY		(_AT(pteval_t, 1) << 55)
#define PTE_SPECIAL		(_AT(pteval_t, 1) << 56)
#define PTE_DEVMAP		(_AT(pteval_t, 1) << 57)
#define PTE_PROT_NONE		(_AT(pteval_t, 1) << 58) /* only when !PTE_VALID */

#define PTE_ADDR_MASK		(((_AT(pteval_t, 1) << (48 - PAGE_SHIFT)) - 1) << PAGE_SHIFT)
#define __pte_to_phys(val)	(val & PTE_ADDR_MASK)
#define __pte_to_pfn(val)	(__pte_to_phys(val) >> PAGE_SHIFT)
#define __mk_pte(pfn, prot)	((pfn << PAGE_SHIFT) | prot)

#define __pgd_to_phys(val)	__pte_to_phys(val)
#define __pgd_to_pfn(val)	__pte_to_pfn(val)
#define __mk_pgd(pfn, prot)	__mk_pte(pfn, prot)

#define __pud_to_phys(val)	__pte_to_phys(val)
#define __pud_to_pfn(val)	__pte_to_pfn(val)
#define __mk_pud(pfn, prot)	__mk_pte(pfn, prot)

#define __pmd_to_phys(val)	__pte_to_phys(val)
#define __pmd_to_pfn(val)	__pte_to_pfn(val)
#define __mk_pmd(pfn, prot)	__mk_pte(pfn, prot)

/* TODO not need */
#define USER_PTRS_PER_PGD   (TASK_SIZE / PGDIR_SIZE)

#define pud_sect(pud)	((pud_val(pud) & PUD_TYPE_MASK) == PUD_TYPE_SECT)
#define pud_table(pud)	((pud_val(pud) & PUD_TYPE_MASK) == PUD_TYPE_TABLE)
#define pud_none(pud)		(!pud_val(pud))
#define pud_bad(pud)		(!pud_table(pud))
#define pud_present(pud)	(pud_val(pud) & (PTE_VALID | PTE_PROT_NONE))
#define pud_leaf(pud)		(pud_present(pud) && !pud_table(pud))
#define pud_valid(pud)		(pud_val(pud) & PTE_VALID)

#define pmd_table(pmd)		((pmd_val(pmd) & PMD_TYPE_MASK) == \
				 PMD_TYPE_TABLE)
#define pmd_sect(pmd)		((pmd_val(pmd) & PMD_TYPE_MASK) == \
				 PMD_TYPE_SECT)
#define pmd_none(pmd)		(!pmd_val(pmd))
#define pmd_present(pmd)	((pmd_val(pmd) & PMD_PRESENT_INVALID) | \
				 (pmd_val(pmd) &(PTE_VALID | PTE_PROT_NONE)))
#define pmd_leaf(pmd)		(pmd_present(pmd) && !pmd_table(pmd))
#define pmd_bad(pmd)		(!pmd_table(pmd))

#define pte_none(pte)		(!pte_val(pte))
#define pte_page(pte)		(pfn_to_page(pte_pfn(pte)))
/*
 * The following only work if pte_present(). Undefined behaviour otherwise.
 */
#define pte_present(pte)	(!!(pte_val(pte) & (PTE_VALID | PTE_PROT_NONE)))
#define pte_young(pte)		(!!(pte_val(pte) & PTE_AF))
#define pte_special(pte)	(!!(pte_val(pte) & PTE_SPECIAL))
#define pte_write(pte)		(!!(pte_val(pte) & PTE_WRITE))
#define pte_user_exec(pte)	(!(pte_val(pte) & PTE_UXN))
#define pte_cont(pte)		(!!(pte_val(pte) & PTE_CONT))
#define pte_devmap(pte)		(!!(pte_val(pte) & PTE_DEVMAP))
#define pte_tagged(pte)		((pte_val(pte) & PTE_ATTRINDX_MASK) == \
				 PTE_ATTRINDX(MT_NORMAL_TAGGED))
#define pte_hw_dirty(pte)	(pte_write(pte) && !(pte_val(pte) & PTE_RDONLY))
#define pte_sw_dirty(pte)	(!!(pte_val(pte) & PTE_DIRTY))
#define pte_dirty(pte)		(pte_sw_dirty(pte) || pte_hw_dirty(pte))
#define pte_valid(pte)		(!!(pte_val(pte) & PTE_VALID))
#define pte_valid_not_user(pte) \
	((pte_val(pte) & (PTE_VALID | PTE_USER)) == PTE_VALID)
#define pte_valid_user(pte) \
	((pte_val(pte) & (PTE_VALID | PTE_USER)) == (PTE_VALID | PTE_USER))

static inline pte_t clear_pte_bit(pte_t pte, pgprot_t prot)
{
	pte_val(pte) &= ~pgprot_val(prot);
	return pte;
}

static inline pte_t set_pte_bit(pte_t pte, pgprot_t prot)
{
	pte_val(pte) |= pgprot_val(prot);
	return pte;
}

static inline pte_t pte_mkwrite(pte_t pte)
{
	pte = set_pte_bit(pte, __pgprot(PTE_WRITE));
	pte = clear_pte_bit(pte, __pgprot(PTE_RDONLY));
	return pte;
}

static inline pte_t pte_mkclean(pte_t pte)
{
	pte = clear_pte_bit(pte, __pgprot(PTE_DIRTY));
	pte = set_pte_bit(pte, __pgprot(PTE_RDONLY));

	return pte;
}

static inline pte_t pte_mkdirty(pte_t pte)
{
	pte = set_pte_bit(pte, __pgprot(PTE_DIRTY));

	if (pte_write(pte))
		pte = clear_pte_bit(pte, __pgprot(PTE_RDONLY));

	return pte;
}

static inline pte_t pte_wrprotect(pte_t pte)
{
	/*
	 * If hardware-dirty (PTE_WRITE/DBM bit set and PTE_RDONLY
	 * clear), set the PTE_DIRTY bit.
	 */
	if (pte_hw_dirty(pte))
		pte = pte_mkdirty(pte);

	pte = clear_pte_bit(pte, __pgprot(PTE_WRITE));
	pte = set_pte_bit(pte, __pgprot(PTE_RDONLY));
	return pte;
}

static inline pte_t pte_mkold(pte_t pte)
{
	return clear_pte_bit(pte, __pgprot(PTE_AF));
}

static inline pte_t pte_mkyoung(pte_t pte)
{
	return set_pte_bit(pte, __pgprot(PTE_AF));
}

static inline pte_t pte_mkspecial(pte_t pte)
{
	return set_pte_bit(pte, __pgprot(PTE_SPECIAL));
}

static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	/*
	 * Normal and Normal-Tagged are two different memory types and indices
	 * in MAIR_EL1. The mask below has to include PTE_ATTRINDX_MASK.
	 */
	const pteval_t mask = PTE_USER | PTE_PXN | PTE_UXN | PTE_RDONLY |
			      PTE_PROT_NONE | PTE_VALID | PTE_WRITE | PTE_GP |
			      PTE_ATTRINDX_MASK;
	/* preserve the hardware dirty information */
	if (pte_hw_dirty(pte))
		pte = pte_mkdirty(pte);
	pte_val(pte) = (pte_val(pte) & ~mask) | (pgprot_val(newprot) & mask);
	return pte;
}

#endif
