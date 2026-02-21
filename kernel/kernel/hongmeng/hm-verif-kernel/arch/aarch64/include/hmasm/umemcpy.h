/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 27 18:25:34 2019
 * Notes: NA
 * History: 2019-11-27: Newly Created
 */
#ifndef AARCH64_KERNEL_UMEMCPY_H
#define AARCH64_KERNEL_UMEMCPY_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>

#define __UMEMCPY_FAULT_SRC	(-1)
#define __UMEMCPY_FAULT_DST	(-2)

extern long __umemcpy(void *dst, const void *src, size_t length);
extern long __umemcpy_slow(void *dst, const void *src, size_t length);

extern long __arch_copy_to_user(void __user *dst, const void *src, size_t length);
extern long __arch_copy_from_user(void __user *dst, const void *src, size_t length);
extern long __arch_copy64_to_user(void __user *dst, u64 src);
extern long __arch_copy32_from_user(const void __user *src);
extern long __arch_copy32_to_user(void __user *dst, u32 src);
extern long __arch_cmpxchg_user(u32 *uval, int __user *uaddr, u32 oldval, u32 newval);

extern long __arch_copy32_from_kernel(const __u32 *src);
struct __arch_copy64_from_kernel_ret {
	__u64 v;
	__u64 e;
};
extern struct __arch_copy64_from_kernel_ret __arch_copy64_from_kernel(const __u64 *src);

/*
 * Different from __raw_copy32_from_user. It return errornumber
 * and value through retval, make it hard to determine if the
 * copy is success or value is -1.
 *
 * Mark it as __always_inline so '*dst = v' can be optimized.
 */
#ifndef __HOST_LLT__

#define ARCH_PROVIDE_COPY32_FROM_KERNEL
static inline __always_inline int arch_copy32_from_kernel(__u32 *dst, const __u32 *src)
{
	long v = __arch_copy32_from_kernel(src);
	if (v >= 0) {
		*dst = (__u32)(((unsigned long)v) & 0xffffffffUL);
	}
	return (v >= 0) ? 0 : E_HM_POSIX_FAULT;
}

#define ARCH_PROVIDE_COPY32_NON_ZERO_FROM_KERNEL
static inline __always_inline int arch_copy32_non_zero_from_kernel(__u32 *dst, const __u32 *src)
{
	long v = __arch_copy32_from_kernel(src);
	if (v > 0) {
		*dst = (__u32)(((unsigned long)v) & 0xffffffffUL);
	}
	return (v > 0) ? 0 : (v == 0 ? E_HM_NOENT : E_HM_POSIX_FAULT);
}

#define ARCH_PROVIDE_COPY64_FROM_KERNEL
static inline __always_inline int arch_copy64_from_kernel(__u64 *dst, const __u64 *src)
{
	struct __arch_copy64_from_kernel_ret r = __arch_copy64_from_kernel(src);
	if (r.e == 0) {
		*dst = r.v;
	}
	return (r.e == 0) ? 0 : E_HM_POSIX_FAULT;
}

#endif

#endif
