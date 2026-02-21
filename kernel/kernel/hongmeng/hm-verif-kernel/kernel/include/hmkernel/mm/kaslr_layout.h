/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2024. All rights reserved.
 * Description: KASLR layout header
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 20 21:59:00 2024
 */
#ifndef AARCH64_ASM_MM_KASLR_LAYOUT_H
#define AARCH64_ASM_MM_KASLR_LAYOUT_H
#include <hmasm/types.h>
#include <hmkernel/kernel.h>

struct kaslr_free_vspace_s {
	unsigned long vstart;
	unsigned long vend;
};

ENUM(2, kaslr_policy_e, KP_,
	RANDOM,
	GROWTH,
)

#define KASLR_AREA_DESC(area_idx, name, sz, add_size, agn, area_parent_idx, area_policy) KASLR_AREA_##area_idx,

/* In multi-layer areas, you must define the top area before the bottom area. */
/* in order to improve the area randomization success rate,
 * the areas sequence need to be sorted by area size in descending order. */

enum kaslr_area_e {
#include <hmasm/mm/kaslr_areas.h>
KASLR_AREA_INVALID,
};

#undef KASLR_AREA_DESC

/* Maximum number of subareas of an area when multi-layer areas are random. */
#define MAX_KASLR_AREAS_PER_VSPACE 8
#define KASLR_AREA_GLOBAL_PARENT KASLR_AREA_INVALID

struct kaslr_area_s {
	uptr_t *vstart;
	uptr_t *vend;
	unsigned long size;    /* real size of the area */
	/* additional alloc size of the area,
	 * Special alignment is required after some areas are allocated(like id_map) */
	unsigned long additional_alloc_size;
	unsigned long align;
	enum kaslr_area_e parent;
	enum kaslr_policy_e policy;
	struct kaslr_free_vspace_s free_vspace[MAX_KASLR_AREAS_PER_VSPACE+1];
};

extern uptr_t __kaslr_kstack_start;
extern struct kaslr_area_s* get_kaslr_area_info(void);
extern void kaslr_flush_kstack(void);
extern void kaslr_record_addr_of_image(uptr_t addr);
extern void kaslr_record_size_of_kernel_image(void);
extern void kaslr_record_size_of_kernel_image_additional(unsigned long additional_size);

#ifdef CONFIG_KDP
extern void kaslr_record_addr_of_kdp(uptr_t addr);
#endif

#ifdef CONFIG_SPIN_BASIC
extern void kaslr_record_addr_of_iregion(uptr_t addr);
#endif

#ifdef CONFIG_KDP
extern void kaslr_randomize_idmap_fix_in_kdp(void);
extern uptr_t get_kdp_free_start_without_idmap(void);
extern void set_kdp_free_start_without_idmap(uptr_t addr);
#endif

#ifdef CONFIG_KASLR
extern void kaslr_randomize_idmap_fix_align(u64 offset);
#else
static inline void kaslr_randomize_idmap_fix_align(u64 offset)
{
	UNUSED(offset);
}
#endif
#endif
