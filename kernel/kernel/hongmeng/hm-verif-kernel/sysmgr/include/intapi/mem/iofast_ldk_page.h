/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Interfaces for accessing to ldk page of iofast memory
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 19 10:07:36 2024
 */
#ifndef SYSMGR_INTAPI_MEM_IOFAST_LDK_PAGE_H
#define SYSMGR_INTAPI_MEM_IOFAST_LDK_PAGE_H

#include <intapi/mem/vspace.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(MEM);

/*
 * iofast_ldk_pg_info_init
 *
 * Initialize some information about the LDK page
 *
 * @param pg_struct_va_start	[I] starting address of the vaddr of structure page
 * @param pg_struct_size	[I] size of the structure page
 * @param pg_slab_shift		[I] shift of PG_Slab
 *
 * @return Return E_HM_OK if init success, error number if fail.
 */
int iofast_ldk_pg_info_init(unsigned long pg_struct_va_start, size_t pg_struct_size,
			    unsigned int pg_slab_shift);

/*
 * iofast_ldk_pg_flags
 *
 * Returns the LDK pg_flags for the iofast page
 *
 * @param vs			[I] Vspace of user process
 * @param pfn			[I] Pfn of iofast page
 *
 * @return Returns the LDK pg_flags for the iofast page
 */
unsigned long iofast_ldk_pg_flags(const struct vspace_s *vs, uint64_t pfn);

/*
 * iofast_ldk_pg_filter_by_flags
 *
 * Filter iofast LDK pages
 *
 * @param pg_filter		[I] Pointer to pg_filter function to filter ldk page
 * @param vs			[I] Vspace of user process
 * @param pfn			[I] Pfn of iofast page
 * @param pfn_nr		[I] Num of iofast page
 *
 * @return Returns True if pg_filter is empty, otherwise returns True when any page
 *         is successfully filtered
 */
bool iofast_ldk_pg_filter_by_flags(bool (*pg_filter)(unsigned long pg_flags),
				   const struct vspace_s *vs,
				   uint64_t pfn, uint64_t pfn_nr);

/*
 * iofast_ldk_pg_is_slab
 *
 * Check if the Slab bit of LDK pg_flags is set
 *
 * @param pg_flags		[I] page flags
 *
 * @return Returns whether the slab bit of ldk pg_flags is set.
 */
bool iofast_ldk_pg_is_slab(unsigned long pg_flags);
#endif
