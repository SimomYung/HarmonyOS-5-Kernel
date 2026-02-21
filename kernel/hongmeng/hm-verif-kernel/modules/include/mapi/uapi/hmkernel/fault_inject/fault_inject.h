/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Inject fault into kernel
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 30 10:17:33 CST 2020
 */

#ifndef __UAPI_HMKERNEL_FAULT_INJECT_H
#define __UAPI_HMKERNEL_FAULT_INJECT_H
#include <hmasm/types.h>

struct __fault_inject {
	__u32 type;
	__u32 sub_type;
	union {
		void *data_addr;
		int (*code_addr)(void);
	} extra;
};

struct __fault_inject_range {
	void *addr;
	__u32 len;
};

enum __fault_inject_type {
	__FAULT_INJECT_CRASH = 0,
	__FAULT_INJECT_MEM,
	__FAULT_INJECT_SCHED,
	/* privilege access */
	__FAULT_INJECT_PRI_ACCESS,
	__FAULT_INJECT_UCOPY,
	__FAULT_INJECT_PTR_AUTH,
	__FAULT_INJECT_SEC_REGION,
	__FAULT_INJECT_KDP,
	__FAULT_INJECT_UNKNOWN,
};

enum __fault_inject_crash_type {
	__FAULT_INJECT_CRASH_PANIC = 0,
	__FAULT_INJECT_CRASH_BUGON,
	__FAULT_INJECT_CRASH_DIVIDE_ZERO,
	__FAULT_INJECT_CRASH_UNKNOWN,
};

enum __fault_inject_mem_type {
	__FAULT_INJECT_MEM_BIG_ARRAY = 0,
	__FAULT_INJECT_MEM_ACCESS_NULL,
	__FAULT_INJECT_MEM_CPU_EXCEPTION_IRQ_STACK_OVERFLOW,
	__FAULT_INJECT_MEM_CPU_EXCEPTION_FIQ_STACK_OVERFLOW,
	__FAULT_INJECT_MEM_CPU_EXCEPTION_STACK_OVERFLOW,
	__FAULT_INJECT_MEM_EXCEPTION_LINEAR_MEM_RO,
	__FAULT_INJECT_MEM_WRITE_RO,
	__FAULT_INJECT_MEM_KASLR_LAYOUT,
	__FAULT_INJECT_MEM_UNKNOWN,
};

enum __fault_inject_sched_type {
	__FAULT_INJECT_SCHED_LOOP_DEAD = 0,
	__FAULT_INJECT_SCHED_UNKNOWN,
};

/* privilege access subtype */
enum __fault_inject_pri_access_type {
	__FAULT_INJECT_PRI_ACCESS_PAN = 0,
	__FAULT_INJECT_PRI_ACCESS_PXN,
	__FAULT_INJECT_RRI_ACCESS_UNKNOWN,
};

enum __fault_inject_ucopy_type {
	__FAULT_INJECT_UCOPY_TO_TEXT = 0,
	__FAULT_INJECT_UCOPY_TO_STACK_FRAME,
	__FAULT_INJECT_UCOPY_FROM_STACK_FRAME,
	__FAULT_INJECT_UCOPY_UNKNOWN,
};

enum __fault_inject_ptr_auth_type {
	__FAULT_INJECT_PAC_FWCFI = 0,
	__FAULT_INJECT_PAC_BWCFI,
	__FAULT_INJECT_PAC_DFI,
	__FAULT_INJECT_PAC_DFI_KERNEL_CUSTOM,
	__FAULT_INJECT_PAC_UNKNOWN,
};

enum __fault_inject_sec_region_type {
	__FAULT_INJECT_SEC_REGION_KERNEL_CODE = 0,
	__FAULT_INJECT_SEC_REGION_KERNEL_RO,
	__FAULT_INJECT_SEC_REGION_KERNEL_RO_AFI,
	__FAULT_INJECT_SEC_REGION_UNKNOWN,
};

enum __fault_inject_kdp_type {
	__FAULT_INJECT_KDP_LAYOUT_KERNEL = 0,
	__FAULT_INJECT_KDP_LAYOUT_KSTACK,
	__FAULT_INJECT_KDP_READ_FILE_STRUCT,
	__FAULT_INJECT_KDP_READ_ARBITRARY_ADDR,
	__FAULT_INJECT_KDP_WRITE_ARBITRARY_ADDR,
	__FAULT_INJECT_KDP_UNKNOWN,
};

#endif
