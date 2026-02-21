/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Page fault management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 27 09:48:53 2019
 */
#ifndef SYSMGR_MEM_FAULT_H
#define SYSMGR_MEM_FAULT_H

#include <hmasm/exception.h>
#include <intapi/mem/vspace.h>

struct page_fault_count_s {
	/* minor page fault */
	long long min_flt;
	/* major page fault */
	long long maj_flt;
};

/*
 * Data abort handler
 *
 * @param vs       [I] vspace of fault process
 *
 * @param far      [I] fault virtual address
 *
 * @param fsr      [I] fault status
 *
 * @param cnt      [O] fault stat info
 *
 * @return         E_HM_OK for success, error code for failure
 */
int fault_handle_data_abort(struct vspace_s *vs,
			    unsigned long far, unsigned long fsr,
			    struct page_fault_count_s *cnt);

/*
 * Instruction abort handler
 *
 * @param vs       [I] vspace of fault process
 *
 * @param far      [I] fault virtual address
 *
 * @param fsr      [I] fault status
 *
 * @param cnt      [O] fault stat info
 *
 * @return         E_HM_OK for success, error code for failure
 *
 */
int fault_handle_inst_abort(struct vspace_s *vs,
			    unsigned long far, unsigned long fsr,
			    struct page_fault_count_s *cnt);

/*
 * Kpagefault abort handler
 *
 * @param vs       [I] vspace of fault process
 *
 * @param far      [I] fault virtual address
 *
 * @param flags    [I] fault status
 *
 * @param fault_sz [I] fault length
 *
 * @param cnt      [O] fault stat info
 *
 * @return         E_HM_OK for success, error code for failure
 *
 */
int fault_handle_kcall_abort(struct vspace_s *vs,
			     unsigned long far, unsigned long write,
			     size_t fault_sz, struct page_fault_count_s *cnt);

/*
 * VSFault handler
 *
 * @param vs       [I] vspace deduced from credential
 *
 * @param vs_cref  [I] faulty cref delivered from kernel
 */
int fault_handle_vsfault(const struct process_s *process, cref_t fault_vspace_cref);

int try_fixup_exception_pc(const struct process_s *process, bool *need_pagefault);

#endif
