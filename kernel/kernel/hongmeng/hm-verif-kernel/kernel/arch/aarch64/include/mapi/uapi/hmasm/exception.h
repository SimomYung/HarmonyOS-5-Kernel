/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 03 10:32:21 2019
 */
#ifndef AARCH64_UAPI_ASM_EXCEPTION_H
#define AARCH64_UAPI_ASM_EXCEPTION_H

#include <hmasm/types.h>

#define __ESR_ISS_DABORT_WNR (1u << 6)
#define __ESR_ISS_DABORT_CM  (1u << 8)
#define __ESR_ISS_FAULT_REASON_MASK (0x3FUL)
#define __ESR_ISS_ALIGNMENT_FAULT (0x21UL)

/* floating-point exception */
#define __ESR_ISS_FP_EXC(x)	((x) & 0x9fU)
#define __ESR_ELx_FP_EXC_TFV	(1u << 23)
#define __FPEXC_IDF		(1u << 7)	/* input denormal exception */
#define __FPEXC_IXF		(1u << 4)	/* floating point inexact result */
#define __FPEXC_UFF		(1u << 3)	/* floating point underflow */
#define __FPEXC_OFF		(1u << 2)	/* floating point overflow */
#define __FPEXC_DZF		(1u << 1)	/* floating point divide by zero */
#define __FPEXC_IOF		(1u)		/* floating point invalid operation */

#ifndef __ASSEMBLY__

struct __arch_exception_info {
	__u64 abort_status;
	__u64 abort_addr;
	__u64 elr;
};

struct __arch_fastpfoob_info {
	__u64 vregion;
	__u64 syspg;
	__u64 vaddr;
};

static inline
int __arch_exception_is_write(__u64 esr)
{
	return (((esr) & __ESR_ISS_DABORT_WNR) != 0u) &&
	       (((esr) & __ESR_ISS_DABORT_CM) == 0u) ? 1 : 0;
}

static inline
__u64 __arch_exception_fault_reason(__u64 esr)
{
	return ((esr) & __ESR_ISS_FAULT_REASON_MASK);
}

static inline
int __arch_exception_is_alignment_fault(__u64 esr)
{
	return ((((esr) & __ESR_ISS_FAULT_REASON_MASK) == __ESR_ISS_ALIGNMENT_FAULT)
			? 1 : 0);
}

static inline
__u64 __arch_exception_info_dabort_status(const struct __arch_exception_info *info)
{
	return info->abort_status;
}

static inline
__u64 __arch_exception_info_iabort_status(const struct __arch_exception_info *info)
{
	return info->abort_status;
}

static inline
__u64 __arch_exception_info_abort_addr(const struct __arch_exception_info *info)
{
	return info->abort_addr;
}

static inline
__u64 __arch_exception_info_elr(const struct __arch_exception_info *info)
{
	return info->elr;
}

static inline
int __arch_exception_info_is_alignment_fault(const struct __arch_exception_info *info)
{
	return __arch_exception_is_alignment_fault(info->abort_status);
}

#endif
#endif
