/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023 ~ 2023. All rights reserved.
 * Description: Interfaces for accessing to iofast memory by sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 13 12:16:37 2023
 */
#ifndef SYSMGR_INTAPI_MEM_IOFAST_H
#define SYSMGR_INTAPI_MEM_IOFAST_H

#include <intapi/mem/vspace.h>

/*
 * iofast_traverse
 *
 * Traverse iofast memory
 *
 * @param vspace		[I] Vspace of user process
 * @param pg_filter		[I] Pointer to pg_filter function to filter ldk page
 * @param callback		[I] Pointer to callback function to operate memory of iofast
 * @param data			[I] The target data
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int iofast_traverse(struct vspace_s *vs,
		    bool (*pg_filter)(unsigned long pg_flags),
		    int (*callback)(uintptr_t _pa, uint64_t _va, size_t _sz, void *_data),
		    void *data);

/*
 * iofast_dump
 *
 * Dump iofast memory
 *
 * @param vspace	[I] Vspace of user process
 * @param uptr		[I] Pointer for user address
 * @param buf		[I] The target buf
 * @param len		[I] The length of buf
 *
 * @return Return E_HM_OK if read success, error number if fail.
*/
int iofast_dump(struct vspace_s *vspace, uintptr_t uptr, void *buf, size_t len);

/*
 * Check whether the physical memory mapped by the virtual memory is iofast.
 *
 * @param vs		[I] the vspace
 *
 * @param vaddr		[I] virtual address
 *
 * @Returns:		return true if the physical memory is iofast, otherwise return false.
 */
bool iofast_mem_check(const struct vspace_s *vs, uintptr_t vaddr);
#endif
