/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of pgtable for module
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 25 11:36:36 CST 2020
 */

#ifndef MAPI_HM_KERNEL_MM_PGTABLE_H
#define MAPI_HM_KERNEL_MM_PGTABLE_H

#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <uapi/hmkernel/mm/pgtable.h>

#ifdef CONFIG_LASP_DOMAIN
#define pgtable_attr_domain(domain) \
	(__u64)((unsigned long)(domain) << PGTABLE_ATTR_DOMAIN_SHIFT)
#define pgtable_attr_domain_of(attr) \
	((unsigned)((attr) >> PGTABLE_ATTR_DOMAIN_SHIFT) & \
	 ((U(1) << PGTABLE_ATTR_DOMAIN_BITS) - U(1)))
#else
#define pgtable_attr_domain(domain) ((__u64)0UL)
#define pgtable_attr_domain_of(attr) 0U
#endif

struct CNode_s;

typedef int (*pt_visit_cb)(unsigned int level, uptr_t vstart, u64 pa,
			   void *pte, bool is_pt, size_t pt_size, void *arg);

int pgtable_visit(const struct capability_s *vspace,
		  uptr_t vstart, uptr_t vend,
		  pt_visit_cb cb, void *arg);

#endif /* end of MAPI_HM_KERNEL_MM_PGTABLE_H */
