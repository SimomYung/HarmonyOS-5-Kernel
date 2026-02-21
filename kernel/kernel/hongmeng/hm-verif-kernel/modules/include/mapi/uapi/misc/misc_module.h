/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of misc module (uapi)
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 25 15:46:54 CST 2020
 */

#ifndef UAPI_MISC_MODULE_H
#define UAPI_MISC_MODULE_H

#include <hmasm/types.h>
#include <hmkernel/capability.h>

#define __MISC_VSPACE_SHOW_PTE		0U

struct __misc_vspace_args {
	cref_t cref;
	__u64 vaddr;
};

#define __MAX_PGTABLE_LEVEL		10U
struct __misc_vspace_pte_result {
	__u32 count;
	bool is_lastlv[__MAX_PGTABLE_LEVEL];
	__u64 pte[__MAX_PGTABLE_LEVEL];
	__u64 pa[__MAX_PGTABLE_LEVEL];
};

#endif /* end of UAPI_MISC_MODULE_H */
