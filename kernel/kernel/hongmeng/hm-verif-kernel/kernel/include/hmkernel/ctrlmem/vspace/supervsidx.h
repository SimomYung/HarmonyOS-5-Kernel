/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Maintain a supervsidx for each supervsable vspace
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 14 10:00:00 2024
 */

#ifndef HMKERNEL_CTRLMEM_VSPACE_SUPERVSIDX_H
#define HMKERNEL_CTRLMEM_VSPACE_SUPERVSIDX_H

/*
 * !!NOTE!! This header file is included by foundamental header hmkernel/ctrlmem/vspace/tlbflush.h
 * and also becomes foundamental. Don't add more header here.
 */

#include <hmkernel/types.h>

struct ctrlmem_kobj_vspace_s;

#define CTRLMEM_VSPACE_INVALID_SLOTIDX		(~(unsigned int)(0U))
#define CTRLMEM_VSPACE_INVALID_SUPERVSIDX	(~(unsigned int)(0U))

#ifdef CONFIG_CTRLMEM_SUPERVS

#define NR_SLOT_POLICYDB _if_def(CONFIG_POLICYDB_USE_SUPERVS, 1U, 0U)
/*
 * for arm64, supervsidx = [0...254]
 * for arm32, not support supervs
 */
#define SUPERVSIDX_FLOOR (0U)
#define SUPERVSIDX_CEILING_UPPERBOUND (SUPERVSIDX_FLOOR + __ARCH_SUPERVSIDX_NR - 1U)
#define SUPERVSIDX_CEILING (SUPERVSIDX_CEILING_UPPERBOUND - NR_SLOT_POLICYDB)
#define SUPERVSIDX_NONE (CTRLMEM_VSPACE_INVALID_SUPERVSIDX)

#define SLOTIDX_FLOOR_BASE (1U)
#define SLOTIDX_FLOOR (SLOTIDX_FLOOR_BASE + NR_SLOT_POLICYDB)
#define SLOTIDX_CEILING (511U) /* At most 511 available slots in one supervs page table. */

#define SLOTIDX_POLICYDB (SLOTIDX_FLOOR_BASE)

void ctrlmem_vspace_supervsidx_init(void);
bool ctrlmem_vspace_supervsidx_is_valid(unsigned int idx);
unsigned int ctrlmem_vspace_supervsidx_to_slotidx(unsigned int supervsidx);
int ctrlmem_vspace_supervsidx_assign(struct ctrlmem_kobj_vspace_s *kobjvs);
void ctrlmem_vspace_supervsidx_revoke(struct ctrlmem_kobj_vspace_s *kobjvs);

#ifdef __HOST_LLT__
unsigned long *ctrlmem_vspace_supervsidx_llt_helper(void);
#endif /* __HOST_LLT__ */

#else /* CONFIG_CTRLMEM_SUPERVS */
static inline void ctrlmem_vspace_supervsidx_init(void)
{
}

static inline bool ctrlmem_vspace_supervsidx_is_valid(unsigned int idx)
{
        UNUSED(idx);
        return false;
}

static inline int ctrlmem_vspace_supervsidx_assign(struct ctrlmem_kobj_vspace_s *kobjvs)
{
        UNUSED(kobjvs);
        return 0;
}
static inline void ctrlmem_vspace_supervsidx_revoke(struct ctrlmem_kobj_vspace_s *kobjvs)
{
        UNUSED(kobjvs);
}
#endif /* CONFIG_CTRLMEM_SUPERVS */

#endif
