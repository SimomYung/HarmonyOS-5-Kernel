/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MMU_STRUCT_H
#define HVGR_MMU_STRUCT_H

#include <linux/types.h>

/*
 * 4K:
 * | 38 ~ 30 | 29 ~ 21 | 20 ~ 12 | 11 ~ 0
 *     L1         L2        L3      offset
 *
 * 16K:
 * | 38 ~ 36 | 35 ~ 25 | 24 ~ 14 | 13 ~ 0
 *     L1         L2        L3      offset
 *
 * 64K:
 * | 38 ~ 29 | 28 ~ 16 | 15 ~ 0
 *     L2         L3       offset
 */
#define MMU_LEVEL_0 0u
#define MMU_LEVEL_1 0u
#define MMU_LEVEL_2 1u
#define MMU_LEVEL_3 2u
#define MMU_LEVEL_NUM 3u

#define HVGR_PAGE_SHIFT 12u /* 4K */
#define HVGR_PAGE_SIZE (1u << HVGR_PAGE_SHIFT)
#define MMU_ENTRY_SIZE 8u
#define MMU_ENTRY_NUM (HVGR_PAGE_SIZE / MMU_ENTRY_SIZE)
#define MMU_PTE_MASK (MMU_ENTRY_NUM - 1)

#define va_to_vpfn(va) ((va) >> HVGR_PAGE_SHIFT)
/* (HVGR_PAGE_SHIFT - 3) is each level bit nums for 4k/16k/64k */
#define mmu_level_offset(level) ((MMU_LEVEL_3 - (level)) * (HVGR_PAGE_SHIFT - 3))
#define mmu_get_level_idx(vpfn, level) (((vpfn) >> mmu_level_offset(level)) & MMU_PTE_MASK)

/* 16k/64k phy addr must be aligned, so shift 12 is correct */
#define entry_to_phys(p_entry) (((uint64_t)((p_entry)->entry.addr)) << 12)
#define entry_to_page(p_entry) (phys_to_page(entry_to_phys(p_entry)))

static inline bool is_64k_granule(void)
{
	return (HVGR_PAGE_SHIFT == 16u);
}

union hvgr_mmu_entry {
	struct {
		uint64_t flag : 2;
		uint64_t attr_idx : 3;
		uint64_t ignore1 : 1;
		uint64_t access_permisions : 2;
		uint64_t share_ablility : 2;
		uint64_t access_flag : 1;
		uint64_t ignored2 : 1;
		/*
		 * PA bit[0:35].
		 * PA mode/PBHA mode cfg in flag cfg func, not in addr area.
		 * PA mode: bit[36:43] GID. The PBHA area bit[59:62] rsvd.
		 * bit  43  42  41  40  39  38  37  36
		 *      G7  G6  G5  G4  G3  G2  G1  G0
		 * PBHA mode: bit[40:43] GID4-7, bit[59:62] GID0-3.
		 * bit  62  61  60  59 ... 43  42  41  40
		 *      G3  G2  G1  G0     G7  G6  G5  G4
		 * 16K/64K bit[13:12]/[15:12] are rsved, the phy addr must be aligned,
		 * so the rsved bits are zero, can be shift by 12 to get phy addr correctlly.
		 */
		uint64_t addr : 24;
	} entry;
	uint64_t value;
};
#endif
