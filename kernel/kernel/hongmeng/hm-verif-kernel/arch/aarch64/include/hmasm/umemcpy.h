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

#endif
