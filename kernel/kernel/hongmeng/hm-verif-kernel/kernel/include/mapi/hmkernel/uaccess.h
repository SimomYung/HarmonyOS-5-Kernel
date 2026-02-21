/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export uaccess api to kernel mapi
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 31 15:19:58 2020
 */
#ifndef MAPI_HMKERNEL_UACCESS_H
#define MAPI_HMKERNEL_UACCESS_H

#include <hmasm/uaccess.h>
#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <hmkernel/mm/ucopy_chk.h>

enum uaccess_direction_e {
	/* Copy from user */
	UACCESS_DIRECTION_U2K,

	/* Copy to user */
	UACCESS_DIRECTION_K2U,
};

struct arch_cpu_local_s;

/*
 * Uaccess without address checking.
 */
JAM_INLINE int
__copy_to_user(void __user *dst, const void *src, size_t len);

JAM_INLINE int
__copy_to_user_unsafe(void __user *dst, const void *src, size_t len);

JAM_INLINE int
__copy_from_user(void *dst, const void __user *src, size_t len);

/*
 * Uaccess with pagefault support. The caller is responsible for address checking.
 */
JAM_INLINE int
__copy_to_user_faultin(void __user *dst, const void *src, size_t len);

JAM_INLINE int
__copy_to_user_faultin_unsafe(void __user *dst, const void *src, size_t len);

JAM_INLINE int
__copy_from_user_faultin(void *dst, const void __user *src, size_t len);

JAM_INLINE int
__cmpxchg_user_faultin(u32 *uval, int __user *uaddr, u32 oldval, u32 newval);

JAM_INLINE bool uaccess_is_range_ok(const void __user *va, unsigned long size);

static inline __always_inline int
copy_to_user(void __user *dst, const void *src, size_t len)
{
	int err = E_HM_OK;

	ucopy_kaddr_range_check(src, len);
	if (!uaccess_is_range_ok(dst, len)) {
		err = E_HM_POSIX_FAULT;
	} else {
		err = __copy_to_user(dst, src, len);
	}
	return err;
}

/*
 * must check uaccess_is_range_ok and disable PAN before call this function.
 * and enable PAN after call this function.
 * So it must be called inside uaccess_start/uaccess_end pair
 */
static inline __always_inline int
copy_to_user_unsafe(void __user *dst, const void *src, size_t len)
{
	return __copy_to_user_unsafe(dst, src, len);
}


static inline __always_inline int
copy_from_user(void *dst, const void __user *src, size_t len)
{
	int err = E_HM_OK;

	ucopy_kaddr_range_check(dst, len);
	if (!uaccess_is_range_ok(src, len)) {
		err = E_HM_POSIX_FAULT;
	} else {
		err = __copy_from_user(dst, src, len);
	}
	return err;
}

static inline __always_inline int
copy_from_kernel(void *dst, const void *src, size_t len)
{
	return raw_copy_from_user(dst, src, len);
}

static inline __always_inline int
copy_to_user_faultin(void __user *dst, const void *src, size_t len)
{
	int err = E_HM_OK;

	ucopy_kaddr_range_check(src, len);
	if (!uaccess_is_range_ok(dst, len)) {
		err = E_HM_POSIX_FAULT;
	} else {
		err = __copy_to_user_faultin(dst, src, len);
	}
	return err;
}

/*
 * must check uaccess_is_range_ok and disable PAN before call this function.
 * and enable PAN after call this function.
 * So it must be called inside uaccess_start/uaccess_end pair
 */
static inline __always_inline int
copy_to_user_faultin_unsafe(void __user *dst, const void *src, size_t len)
{
	return __copy_to_user_faultin_unsafe(dst, src, len);
}

static inline __always_inline int
copy_from_user_faultin(void *dst, const void __user *src, size_t len)
{
	int err = E_HM_OK;

	ucopy_kaddr_range_check(dst, len);
	if (!uaccess_is_range_ok(src, len)) {
		err = E_HM_POSIX_FAULT;
	} else {
		err = __copy_from_user_faultin(dst, src, len);
	}
	return err;
}

static inline int uaccess_start(void __user *dst, void *src, size_t len)
{
	int err = E_HM_OK;

	ucopy_kaddr_range_check(src, len);
	if (!uaccess_is_range_ok(dst, len)) {
		err = E_HM_POSIX_FAULT;
	}

	if (err == E_HM_OK) {
		raw_uaccess_start();
	}
	return err;
}

static inline void uaccess_end(void)
{
	raw_uaccess_end();
}

#ifdef CONFIG_DEBUG_BUILD
JAM_INLINE int copy_vs_to_user(struct ctrlmem_kobj_vspace_s *kobjvs, void __user *dst, const void *src, size_t len);
JAM_INLINE int copy_vs_from_user(struct ctrlmem_kobj_vspace_s *kobjvs,
				 void *dst, const void __user *src, size_t len);
JAM_INLINE int copy_vs_to_user_faultin(struct ctrlmem_kobj_vspace_s *kobjvs, void __user *dst,
				       const void *src, size_t len);
JAM_INLINE int copy_vs_from_user_faultin(struct ctrlmem_kobj_vspace_s *kobjvs,
					 void *dst, const void __user *src, size_t len);

static inline int raw_copy32_vs_to_user(struct ctrlmem_kobj_vspace_s *kobjvs, void __user *dst, u32 src)
{
	return copy_vs_to_user(kobjvs, dst, &src, 4);
}
static inline int raw_copy64_vs_to_user(struct ctrlmem_kobj_vspace_s *kobjvs, void __user *dst, u64 src)
{
	return copy_vs_to_user(kobjvs, dst, &src, 8);
}

#define raw_copy_vs_to_user(kobjvs, dst, src, len) \
	copy_vs_to_user(kobjvs, dst, src, len)

static inline int raw_copy32_vs_from_user(struct ctrlmem_kobj_vspace_s *kobjvs, const void __user *src)
{
	int dst = 0;
	(void)copy_vs_from_user(kobjvs, &dst, src, 4);
	return dst;
}

#define raw_copy_vs_from_user(kobjvs, dst, src, len) \
	copy_vs_from_user(kobjvs, dst, src, len)
#else
#define copy_vs_to_user(kobjvs, dst, src, len) \
	copy_to_user(dst, src, len)

#define copy_vs_from_user(kobjvs, dst, src, len) \
	copy_from_user(dst, src, len)

#define copy_vs_to_user_faultin(kobjvs, dst, src, len) \
	copy_to_user_faultin(dst, src, len)

#define copy_vs_from_user_faultin(kobjvs, dst, src, len) \
	copy_from_user_faultin(dst, src, len)

#define raw_copy32_vs_to_user(kobjvs, dst, src) \
	raw_copy32_to_user(dst, src)

#define raw_copy64_vs_to_user(kobjvs, dst, src) \
	raw_copy64_to_user(dst, src)

#define raw_copy_vs_to_user(kobjvs, dst, src, len) \
	raw_copy_to_user(dst, src, len)

#define raw_copy32_vs_from_user(kobjvs, src) \
	raw_copy32_from_user(src)

#define raw_copy_vs_from_user(kobjvs, dst, src, len) \
	raw_copy_from_user(dst, src, len)
#endif


void __fastpath_check_user_access(struct ctrlmem_kobj_vspace_s *kobjvs,
				  struct arch_cpu_local_s *cpu_local,
				  const void __user *addr, size_t len);
#ifndef __HOST_LLT__
#define fastpath_check_user_access(kobjvs, direction, cpu_local, addr, len)	\
	__fastpath_check_user_access(kobjvs, cpu_local, addr, len)
#else
void __llt_fastpath_check_user_access(struct ctrlmem_kobj_vspace_s *kobjvs,
				      enum uaccess_direction_e direction,
				      struct arch_cpu_local_s *cpu_local,
				      const void __user *addr, size_t len);
#define fastpath_check_user_access(kobjvs, direction, cpu_local, addr, len)	\
	__llt_fastpath_check_user_access(kobjvs, direction, cpu_local, addr, len)

#endif

/*
 * If the address is illegal, return E_HM_POSIX_FAULT,
 * if only virtual memory is allocated but physical memory is not actually allocated,
 * then the faultin process is triggered to allocate the physical memory.
 */
static inline int check_user_va_available(uptr_t va)
{
	const char touch_write = '\0';
	return copy_to_user_faultin(ptr_to_void(va), (const void *)&touch_write, 1UL);
}

static inline __always_inline int copy32_from_kernel(__u32 *dst, const __u32 *src)
{
#ifdef ARCH_PROVIDE_COPY32_FROM_KERNEL
	return arch_copy32_from_kernel(dst, src);
#else
	return raw_copy_from_user(dst, src, sizeof(u32));
#endif
}

static inline __always_inline int copy32_non_zero_from_kernel(__u32 *dst, const __u32 *src)
{
#ifdef ARCH_PROVIDE_COPY32_NON_ZERO_FROM_KERNEL
	return arch_copy32_non_zero_from_kernel(dst, src);
#else
	int r = copy32_from_kernel(dst, src);
	return (r == 0) ? ((*dst == 0) ? E_HM_NOENT : r) : r;
#endif
}

static inline __always_inline int copy64_from_kernel(__u64 *dst, const __u64 *src)
{
#ifdef ARCH_PROVIDE_COPY64_FROM_KERNEL
	return arch_copy64_from_kernel(dst, src);
#else
	return raw_copy_from_user(dst, src, sizeof(u64));
#endif
}
#endif
