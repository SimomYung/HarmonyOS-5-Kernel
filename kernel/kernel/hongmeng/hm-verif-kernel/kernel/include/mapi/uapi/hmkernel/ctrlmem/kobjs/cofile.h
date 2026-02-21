/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for cofile object
 * Author: Huawei OS Kernel Lab
 * Created: Wed Jun 28 01:47:59 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_COFILE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_COFILE_H

#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/kobjs/syspa.h>

#define __KOBJCOF_TYPE_PROCFS		0U
#define __KOBJCOF_TYPE_INVALID		1U

#define __KOBJCOF_FDATA_PAGE_NR		120U

/* The placeholder_ members is only accessed by kernel. */
struct __ctrlmem_kobj_cofile_s {
	__u32 placeholder_refcnt;
	__u32 placeholder_lock;
	__s64 pos;
	__u64 size;
	__u32 type;

	struct {
		__u32 status;
		__s64 placeholder_pages_begin_pos;
		union {
			__vaddr_t kva;
			__u64 padding;
		} pages_kva[__KOBJCOF_FDATA_PAGE_NR];
	} fdata;
};

#define __KOBJCOF_FDATA_CAN_READ	0U
#define __KOBJCOF_FDATA_NEED_FETCH	1U
#define __KOBJCOF_FDATA_INVALID		2U

#endif
