/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Ucopy check
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 22 12:27:44 2020
 */
#ifndef MAPI_HMKERNEL_UCOPY_CHK_H
#define MAPI_HMKERNEL_UCOPY_CHK_H

#include <hmkernel/panic.h>
#include <hmkernel/kernel.h>
#include <hmkernel/compiler.h>

#ifdef CONFIG_HARDENED_USERCOPY
#ifndef __clang__
void __error("kernel buffer size is not big enough to usercopy")
__bad_ucopy_size(void);
#endif

void __ucopy_object_range_check(const void *start, const void *end);

static inline void
__ucopy_logic_size_check(const void *addr, size_t len)
{
	BUG_ON(addr == NULL, "usercopy: NULL address detected");
	BUG_ON((uptr_t)addr + len < (uptr_t)addr, "usercopy: Wrapped address");
}

static inline __always_inline void
__ucopy_buf_size_check(const void *addr, size_t len)
{
	size_t sz = __builtin_object_size(addr, 0);
	if (unlikely(sz != (size_t)-1 && sz < len)) {
		BUG_ON(__builtin_constant_p(len) == 0,
		       "usercopy: buffer overflow detected");
#ifndef __clang__
		__bad_ucopy_size();
#endif
	}
}
#endif

static inline __always_inline void
ucopy_kaddr_range_check(const void *addr, size_t len)
{
#ifdef CONFIG_HARDENED_USERCOPY
	__ucopy_buf_size_check(addr, len);

	if (__builtin_constant_p(len) == 0) {
		__ucopy_logic_size_check(addr, len);

		const void *end = ptr_add(addr, len);
		__ucopy_object_range_check(addr, end);
	}
#else
	UNUSED(addr, len);
#endif
}

#endif
