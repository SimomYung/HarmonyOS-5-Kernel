/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Page table types definitions.
 *
 * Based on arch/riscv/include/asm/pgtable-types.h
 */
#ifndef __LIBLINUX_ASM_PGTABLE_TYPES_H
#define __LIBLINUX_ASM_PGTABLE_TYPES_H

#include <asm/types.h>

typedef unsigned long pteval_t;
typedef unsigned long pmdval_t;
typedef unsigned long pudval_t;
typedef unsigned long p4dval_t;
typedef unsigned long pgdval_t;

/*
 * These are used to make use of C type-checking..
 */
typedef struct { pteval_t pte; } pte_t;
#define pte_val(x)	((x).pte)
#define __pte(x)	((pte_t) { (x) } )

#if CONFIG_PGTABLE_LEVELS > 2
typedef struct { pmdval_t pmd; } pmd_t;
#define pmd_val(x)	((x).pmd)
#define __pmd(x)	((pmd_t) { (x) } )
#endif

#if CONFIG_PGTABLE_LEVELS > 3
typedef struct { pudval_t pud; } pud_t;
#define pud_val(x)	((x).pud)
#define __pud(x)	((pud_t) { (x) } )
#endif

typedef struct { pgdval_t pgd; } pgd_t;
#define pgd_val(x)	((x).pgd)
#define __pgd(x)	((pgd_t) { (x) } )

typedef struct { pteval_t pgprot; } pgprot_t;
#define pgprot_val(x)	((x).pgprot)
#define __pgprot(x)	((pgprot_t) { (x) } )

#if CONFIG_PGTABLE_LEVELS == 2
#include <asm-generic/pgtable-nopmd.h>
#elif CONFIG_PGTABLE_LEVELS == 3
#include <asm-generic/pgtable-nopud.h>
#elif CONFIG_PGTABLE_LEVELS == 4
#include <asm-generic/pgtable-nop4d.h>
#endif

#endif	/* __LIBLINUX_ASM_PGTABLE_TYPES_H */
