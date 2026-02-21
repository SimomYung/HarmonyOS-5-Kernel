/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: jitfort copgtable header
 * Author: Huawei
 * Create: Wen Mar 17 09:43:10 2024
*/

#ifndef SYSMGR_INTAPI_MEM_JITFORT_COPGTABLE_H
#define SYSMGR_INTAPI_MEM_JITFORT_COPGTABLE_H

struct vspace_s;
struct vregion_s;
struct copgtable_s;
struct page_s;
struct __ctrlmem_kobj_vspace_s;

#ifdef CONFIG_SECURITY_JITFORT
/*
 * copgtable_create
 *
 * Create copgtable.
 *
 * @param vspace		[I] Vspace of user process
 *
 * @return Return E_HM_OK if success, error number if fail.
 */
int copgtable_create(struct vspace_s *vspace);

/*
 * copgtable_destroy
 *
 * Destroy copgtable if no thread using copgtable.
 * `is_exiting` should be set when process crashes or exits
 * to force reclaim resources.
 *
 * @param vspace		[I] Vspace of user process
 * @param is_exiting	[I] If the process is exiting
 *
 * @return Return E_HM_OK if destroy success, E_HM_AGAIN if copgtable is in use.
 */
int copgtable_destroy(struct vspace_s *vspace, bool is_exiting);

/*
 * copgtable_unmap_range
 *
 * unmap mappings of a range in copgtable, the range must be within a vregion
 *
 * @param vr			[I] Vregion of the range
 * @param start			[I] Start of the range
 * @param end			[I] End of the range
 *
 * @return Return E_HM_OK if unmap success, error number if fail.
 */
int copgtable_unmap_range(struct vregion_s *vr, uintptr_t start, uintptr_t end);

/*
 * copgtable_unmap_range_nolock
 *
 * unmap mappings of a range in copgtable, the range must be within a vregion
 * caller should hold vr->vspace->copgtable_rwlock
 *
 * @param vr			[I] Vregion of the range
 * @param start			[I] Start of the range
 * @param end			[I] End of the range
 *
 * @return Return E_HM_OK if unmap success, error number if fail.
 */
int copgtable_unmap_range_nolock(struct vregion_s *vr, uintptr_t start, uintptr_t end);

/*
 * copgtable_handle_pagefault
 *
 * Handles pagefault in copgtable. Only copies pte from vspace.
 * Will do a pagefault if the required pte in vspace is invalid.
 *
 * @param vspace		[I] Vspace of user process
 * @param far			[I] FAR of fault
 * @param fsr			[I] FSR of fault
 * @param cnt			[I] Fault counter
 * @param is_data_fault	[I] If the fault is a data fault
 *
 * @return Return E_HM_OK if pagefault hanle success, error number if fail.
 */
int copgtable_handle_pagefault(struct vspace_s *vs, unsigned long far, unsigned long fsr,
			    struct page_fault_count_s *cnt, bool is_data_fault);

void copgtable_unmap_pte_by_vaddr_nofail(const struct vregion_s *vr, uint64_t vaddr, struct page_s *pg);

struct pgtable_s* copgtable_pgtable_of_vspace(const struct vspace_s *vspace);

cref_t copgtable_kobjvs_cref_of_vspace(const struct vspace_s *vspace);

struct __ctrlmem_kobj_vspace_s *copgtable_kobjvs_of_vspace(const struct vspace_s *vspace);

bool pa_is_gzp(uint64_t paddr);

int copgtable_handle_kcall_abort(struct vspace_s *vs,
			     unsigned long far, unsigned long write,
			     size_t fault_sz, struct page_fault_count_s *cnt);

struct copgtable_map_args_s {
	uintptr_t paddr;
	uintptr_t vaddr;
	bool is_data_fault;
};
#else
static inline int copgtable_create(struct vspace_s *vspace)
{
	UNUSED(vspace);
	return E_HM_OK;
}

static inline int copgtable_destroy(struct vspace_s *vspace, bool is_exiting)
{
	UNUSED(vspace, is_exiting);
	return E_HM_OK;
}

static inline int
copgtable_unmap_range_nolock(struct vregion_s *vr, uintptr_t start, uintptr_t end)
{
	UNUSED(vr, start, end);
	return E_HM_OK;
}

static inline int
copgtable_unmap_range(struct vregion_s *vr, uintptr_t start, uintptr_t end)
{
	UNUSED(vr, start, end);
	return E_HM_OK;
}

static inline int
copgtable_handle_pagefault(struct vspace_s *vs, unsigned long far,
			    unsigned long fsr, struct page_fault_count_s *cnt, bool is_data_fault)
{
	UNUSED(vs, far, fsr, cnt, is_data_fault);
	return E_HM_OK;
}

static inline void
copgtable_unmap_pte_by_vaddr_nofail(const struct vregion_s *vr, uint64_t vaddr, struct page_s *pg)
{
	UNUSED(vr, vaddr, pg);
}

static inline struct pgtable_s* copgtable_pgtable_of_vspace(const struct vspace_s *vspace)
{
	UNUSED(vspace);
	hm_panic("not supported");
	return NULL;
}

static inline cref_t copgtable_kobjvs_cref_of_vspace(const struct vspace_s *vspace)
{
	UNUSED(vspace);
	hm_panic("not supported");
	return 0;
}

static inline struct __ctrlmem_kobj_vspace_s *copgtable_kobjvs_of_vspace(const struct vspace_s *vspace)
{
	UNUSED(vspace);
	hm_panic("not supported");
	return NULL;
}

static inline int copgtable_handle_kcall_abort(struct vspace_s *vs,
			     unsigned long far, unsigned long write,
			     size_t fault_sz, struct page_fault_count_s *cnt)
{
	UNUSED(vs, far, write, fault_sz, cnt);
	hm_panic("not supported");
	return E_HM_FAULT;
}
#endif

#endif
