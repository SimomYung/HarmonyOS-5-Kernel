/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 28 06:35:02 2018
 */
#ifndef A64_ASM_UACCESS_H
#define A64_ASM_UACCESS_H

#include <hmkernel/compiler.h>
#include <hmkernel/activation.h>
#include <hmkernel/kernel.h>
#include <hmkernel/strict.h>
#include <hmkernel/types.h>
#include <lib/string.h>
#include <hmasm/cpu_local.h>
#include <hmasm/memory.h>
#include <hmasm/umemcpy.h>
#include <hmasm/ustrcpy.h>
#include <hmasm/processor.h>
#include <uapi/hmasm/ucontext.h>

static inline bool arch_uaccess_raw_is_addr_ok(bool privileged, const void __user *va)
{
	bool ret = false;
	if (ptr_to_u64(va) < (u64)(1ULL << VA_BITS)) {
		ret = true;
#ifdef CONFIG_PRIVILEGED_SERVICE
	} else if (privileged && (ptr_to_u64(va) < PRIVILEGE_VA_LIMIT)) {
		ret = true;
#endif
	} else {
		/* do nothing */
	}
	UNUSED(privileged);
	return ret;
}

static inline bool arch_uaccess_raw_is_addr_owned(bool privileged, const void __user *va)
{
	bool ret = false;
	if (privileged) {
#ifdef CONFIG_PRIVILEGED_SERVICE
		ret = ((ptr_to_u64(va) >= VA_START) && (ptr_to_u64(va) < PRIVILEGE_VA_LIMIT));
#endif
	} else {
		ret = (ptr_to_u64(va) < (u64)(1ULL << VA_BITS));
	}
	return ret;
}

static inline bool __arch_uaccess_in_range(const void __user *va, unsigned long size,
					   u64 range_st, u64 range_ed)
{
	u64 va_u64 = ptr_to_u64(va);
	return ((range_st <= va_u64) && (va_u64 < range_ed)) &&
		((range_ed - va_u64) >= (u64)size);
}

static inline bool arch_uaccess_raw_is_range_ok(bool privileged, const void __user *va, unsigned long size)
{
	bool ret = false;
	if (__arch_uaccess_in_range(va, size, 0ULL, ((u64)(1ULL) << VA_BITS))) {
		ret = true;
#ifdef CONFIG_PRIVILEGED_SERVICE
	} else if (privileged && __arch_uaccess_in_range(va, size, VA_START, PRIVILEGE_VA_LIMIT)) {
		ret = true;
#endif
	} else {
		/* do nothing */
	}
	UNUSED(privileged);
	return ret;
}

static inline bool arch_uaccess_raw_is_range_owned(bool privileged, const void __user *va, unsigned long size)
{
	bool ret = false;
	if (privileged) {
#ifdef CONFIG_PRIVILEGED_SERVICE
		ret = __arch_uaccess_in_range(va, size, VA_START, PRIVILEGE_VA_LIMIT);
#endif
	} else {
		ret = __arch_uaccess_in_range(va, size, 0UL, 1UL << VA_BITS);
	}
	return ret;
}

extern int __raw_copy_from_user(void __user *dst, const void *src, size_t length);
extern int __raw_copy_to_user(void __user *dst, const void *src, size_t length);
extern int __raw_copy64_to_user(void __user *dst, u64 src);
extern int __raw_copy32_from_user(const void __user *src);
extern int __raw_copy32_to_user(void __user *dst, u32 src);
extern int __raw_cmpxchg_user(u32 *uval, int __user *_uaddr,
			      u32 oldval, u32 newval);

#ifdef __HOST_LLT__
static inline bool is_kvspace_access_ok(unsigned long va_start, unsigned long va_end)
{
	return true;
}
#else

static inline bool __in_kvspace_id_map_area(unsigned long va_start, unsigned long va_end)
{
	return ((va_end > va_start) && (va_start >= VA_ID_MAP_START) && (va_end < VA_ID_MAP_END));
}

static inline bool __in_kvspace_img_map_area(unsigned long va_start, unsigned long va_end)
{
	return ((va_end > va_start) && (va_start >= VA_KERNEL_IMG_START) && (va_end < VA_KERNEL_IMG_END));
}

static inline bool __in_kvspace_stack_area(unsigned long va_start, unsigned long va_end)
{
	return ((va_end > va_start) && (va_start >= VA_KSTACK_START) && (va_end < VA_KSTACK_END));
}

static inline bool is_kvspace_access_ok(unsigned long va_start, unsigned long va_end)
{
	bool is_ok = false;
	if (__in_kvspace_id_map_area(va_start, va_end)) {
		is_ok = true;
	} else if (__in_kvspace_img_map_area(va_start, va_end)) {
		is_ok = true;
	} else if (__in_kvspace_stack_area(va_start, va_end)) {
		is_ok = true;
	}
	return is_ok;
}

#endif /* __HOST_LLT__ */

static inline bool is_transaction_addr(uptr_t va)
{
	bool ret = false;
#ifdef CONFIG_PRIVILEGED_SERVICE
	if (va < (1UL << VA_BITS)) {
		ret = true;
	}
#endif
	return ret;
}

static inline int
raw_copy_to_user(void __user *dst, const void *src, size_t len)
{
	return __raw_copy_to_user(dst, src, len);
}

/* must be called after disable PAN, and enable PAN after this func */
static inline int
raw_copy_to_user_unsafe(void __user *dst, const void *src, size_t len)
{
	return __arch_copy_to_user(dst, src, len);
}

static inline int
raw_copy_from_user(void *dst, const void __user *src, size_t len)
{
	return __raw_copy_from_user(dst, src, len);
}

static inline int
raw_copy64_to_user(void __user *dst, u64 src)
{
	return __raw_copy64_to_user(dst, src);
}

static inline int
raw_copy32_from_user(const void __user *src)
{
	return __raw_copy32_from_user(src);
}

static inline int
raw_copy32_to_user(void __user *dst, u32 src)
{
	return __raw_copy32_to_user(dst, src);
}

int raw_cmpxchg_user(u32 *uval, int __user *uaddr, u32 oldval, u32 newval);

#ifdef CONFIG_AARCH64_PAN
static inline void disable_pan(void)
{
	write_sysreg("pan", 0);
}

static inline void enable_pan(void)
{
	write_sysreg("pan", 1);
}

/*
 * Notice: Usage scenes
 * This function is just used in fastpath for performance.
 * Normally, copy_user/to_user should be used
 */
static inline void copy_to_user_aligned(void __user *_dst, void *_src, unsigned long len)
{
	disable_pan();
	cpymem_aligned(_dst, _src, len);
	enable_pan();
}

static inline void copy_from_user_aligned(void *_dst, void __user *_src, unsigned long len)
{
	disable_pan();
	cpymem_aligned(_dst, _src, len);
	enable_pan();
}

#else
static inline void copy_to_user_aligned(void __user *_dst, void *_src, unsigned long len)
{
	cpymem_aligned(_dst, _src, len);
}

static inline void copy_from_user_aligned(void *_dst, void __user *_src, unsigned long len)
{
	cpymem_aligned(_dst, _src, len);
}
#endif

static inline void raw_uaccess_start(void)
{
#ifdef CONFIG_AARCH64_PAN
	disable_pan();
#endif
}

static inline void raw_uaccess_end(void)
{
#ifdef CONFIG_AARCH64_PAN
	enable_pan();
#endif
}

#endif
