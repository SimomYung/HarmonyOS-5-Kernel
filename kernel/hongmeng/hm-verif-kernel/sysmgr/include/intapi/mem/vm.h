/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Function declare of vm memory management
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 28 16:41:21 2021
 */
#ifndef SYSMGR_INTAPI_MEM_VM_H
#define SYSMGR_INTAPI_MEM_VM_H
#include "intapi/mem/page.h"
#include <intapi/mem/vspace.h>
#include <intapi/ctrlmem/syscalls.h>
#include <libstrict/strict.h>

struct vm_madvise_args {
	struct vspace_s *uvmm_vspace;
	struct vspace_s *vm_vspace;
	uintptr_t uvmm_hva;
	uintptr_t vm_gpa;
	uint64_t length;
	uint32_t flags;
};

struct page_s;
struct vregion_s;
int vm_page_fault_handler(struct vspace_s *uvmm_vspace, struct vspace_s *vm_vspace,
			  uintptr_t uvmm_hva, uintptr_t vm_gpa);
int vm_madvise(struct vm_madvise_args *vm_madvise_arg);
#ifdef CONFIG_VM_DYNAMIC_MEMORY
int vm_page_fault_dyn_handler(struct vspace_s *uvmm_vspace,
			      struct vspace_s *vm_vspace,
			      uintptr_t uvmm_hva,
			      uintptr_t vm_gpa);
int vm_build_pte(struct vregion_s *vr_vm, uintptr_t vm_gpa,
		 uint64_t hint, struct page_s *page);
void vm_clear_mapping(struct vregion_s *vr_vm, struct page_s *page, uintptr_t vm_gpa, bool is_lock_page);
/*
 * If this is a vm region we need to update kernel bookkeeping of vm regions.
 */
int vm_register_mem_region(struct process_s *process, uintptr_t hva_start,
		       uintptr_t gpa_start, uintptr_t gpa_end);
int vm_unregister_mem_region_range(struct process_s *process,
			       uintptr_t gpa_start, uintptr_t gpa_end);
#else /* CONFIG_VM_DYNAMIC_MEMORY */
static inline int vm_register_mem_region(struct process_s *process, uintptr_t hva_start,
				     uintptr_t gpa_start, uintptr_t gpa_end)
{
	UNUSED(process, hva_start, gpa_start, gpa_end);

	return E_HM_OK;
}

static inline int vm_unregister_mem_region_range(struct process_s *process,
					     uintptr_t gpa_start, uintptr_t gpa_end)
{
	UNUSED(process, gpa_start, gpa_end);

	return E_HM_OK;
}
#endif /* CONFIG_VM_DYNAMIC_MEMORY */
#endif
