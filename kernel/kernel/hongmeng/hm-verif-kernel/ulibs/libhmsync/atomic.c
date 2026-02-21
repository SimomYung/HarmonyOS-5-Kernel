/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Basic interfaces of raw_atomic
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 26 20:37:38 2020
 */

#include <libhmsync/atomic.h>
#include <hongmeng/panic.h>

unsigned long __raw_atomic_xchg(unsigned long *ptr, unsigned long x, size_t size)
{
	unsigned long ret;

	switch (size) {
	case sizeof(uint8_t):
		ret = __raw_atomic_xchg8((uint8_t *)(uintptr_t)ptr, (uint8_t)x);
		break;
	case sizeof(uint16_t):
		ret = __raw_atomic_xchg16((uint16_t *)(uintptr_t)ptr, (uint16_t)x);
		break;
	case sizeof(uint32_t):
		ret = __raw_atomic_xchg32((uint32_t *)(uintptr_t)ptr, (uint32_t)x);
		break;
#if __SIZEOF_LONG__==8
	case sizeof(uint64_t):
		ret = __raw_atomic_xchg64((uint64_t *)(uintptr_t)ptr, (uint64_t)x);
		break;
#endif
	default:
		ret = 0;
		BUG_ON(1);
		break;
	}

	return ret;
}

unsigned long __raw_atomic_cmpxchg(unsigned long *ptr, unsigned long o,
				   unsigned long n, size_t size)
{
	unsigned long ret;

	switch (size) {
	case sizeof(uint8_t):
		ret = __raw_atomic_cmpxchg8((uint8_t *)(uintptr_t)ptr,
					    (uint8_t)o, (uint8_t)n);
		break;
	case sizeof(uint16_t):
		ret = __raw_atomic_cmpxchg16((uint16_t *)(uintptr_t)ptr,
					     (uint16_t)o, (uint16_t)n);
		break;
	case sizeof(uint32_t):
		ret = __raw_atomic_cmpxchg32((uint32_t *)(uintptr_t)ptr,
					     (uint32_t)o, (uint32_t)n);
		break;
#if __SIZEOF_LONG__==8
	case sizeof(uint64_t):
		ret = __raw_atomic_cmpxchg64((uint64_t *)(uintptr_t)ptr,
					     (uint64_t)o, (uint64_t)n);
		break;
#endif
	default:
		ret = 0;
		BUG_ON(true);
		break;
	}

	return ret;
}
