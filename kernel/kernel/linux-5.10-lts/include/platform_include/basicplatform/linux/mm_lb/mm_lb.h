/*
 * Copyright (c) 2018-2019 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __MM_LB_H
#define __MM_LB_H

#ifdef CONFIG_MM_LB

#ifdef CONFIG_MM_LB_V550
#include "mm_lb_v550.h"
#else
#include "mm_lb_legacy.h"
#endif

#else

#include "mm_lb_bypass.h"

#endif

#ifdef CONFIG_LIBLINUX
#ifdef CONFIG_MM_LB
#define PID_ISPNN 11
#define PTE_OUTER_SHARED (_AT(pteval_t, 2) << 8)
#define PTE_SH_MASK (_AT(pteval_t, 3) << 8)
#define PAGE_OUTER_SHARED   __pgprot((PROT_NORMAL & (~PTE_SH_MASK)) | PTE_OUTER_SHARED)
#define PAGE_NON_SHARED     __pgprot(PROT_NORMAL_NC & (~PTE_SH_MASK))
#define PAGE_NON_SHARED_DEVICE __pgprot(PROT_DEVICE_nGnRE & (~PTE_SH_MASK))
#ifdef CONFIG_MM_LB_GID_PBHA
#ifdef CONFIG_MM_LB_GID_PBHA_LITTLE_ENDIAN
#define PBHA_GID_LOW_BIT_SHIFT     40
#define PTE_LB_MASK                (_AT(pteval_t, 15) << PBHA_GID_LOW_BIT_SHIFT)
#define PTE_LB(i)                  (_AT(pteval_t, i) << PBHA_GID_LOW_BIT_SHIFT) /* gid low 4 bit */
#define PBHA_GID_HIGH_BIT_SHIFT    59
#define PBHA_GID_HIGH_BIT_MASK     (_AT(pteval_t, 3) << PBHA_GID_HIGH_BIT_SHIFT)
#define PBHA_GID_HIGH_BIT(i)       (_AT(pteval_t, i) << PBHA_GID_HIGH_BIT_SHIFT)
#else
#define PBHA_GID_LOW_BIT_SHIFT     59
#define PTE_LB_MASK                (_AT(pteval_t, 15) << PBHA_GID_LOW_BIT_SHIFT)
#define PTE_LB(i)                  (_AT(pteval_t, i) << PBHA_GID_LOW_BIT_SHIFT) /* lb attr */
#define PBHA_GID_HIGH_BIT_SHIFT    40
#define PBHA_GID_HIGH_BIT_MASK     (_AT(pteval_t, 3) << PBHA_GID_HIGH_BIT_SHIFT)
#define PBHA_GID_HIGH_BIT(i)       (_AT(pteval_t, i) << PBHA_GID_HIGH_BIT_SHIFT)
#endif

#define PGID_HIGH_BITS_MASK      GENMASK(5, 4)
#define PGID_LOW_BITS_MASK       GENMASK(3, 0)

#define pgprot_lb(prot, gid) __pgprot_modify(prot, \
						PTE_LB_MASK | PBHA_GID_HIGH_BIT_MASK, \
						PTE_LB(gid & PGID_LOW_BITS_MASK) | \
						PBHA_GID_HIGH_BIT((gid & PGID_HIGH_BITS_MASK) >> 4))

#define pte_gid(pte)	(((pte_val(pte) & PTE_LB_MASK) >> PBHA_GID_LOW_BIT_SHIFT) | \
						(((pte_val(pte) & PBHA_GID_HIGH_BIT_MASK) >> PBHA_GID_HIGH_BIT_SHIFT) << 4))

#define PAGE_LB_CACHEABLE(i) __pgprot(PTE_LB(i & PGID_LOW_BITS_MASK) | \
                            PBHA_GID_HIGH_BIT((i & PGID_HIGH_BITS_MASK) >> 4))
#else
#define PTE_LB_MASK     (_AT(pteval_t, 15) << 36)
#define PTE_LB(i)       (_AT(pteval_t, i) << 36) /* lb attr */

#define pgprot_lb(prot, gid) __pgprot_modify(prot, PTE_LB_MASK, PTE_LB(gid))
#define pte_gid(pte)        ((pte_val(pte) & PTE_LB_MASK) >> 36)

#define PAGE_LB_CACHEABLE(i) __pgprot(PTE_LB(i))
#endif
#endif
#endif


#endif
