/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: header file of cache
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 24 17:08:32 2022
 */
#ifndef HM_INCLUDE_HMEXT_CACHE_H
#define HM_INCLUDE_HMEXT_CACHE_H

#include <hmext/ext.h>
#include <stddef.h>

/* cache cmds and saved in mdoule_rref and same as kernel */
#define HM_EXT_CACHE_CACHE_FLUSH	0x0U
#define HM_EXT_CACHE_DCACHE_FLUSH	0x1U
#define HM_EXT_CACHE_ICACHE_FLUSH	0x2U
#define HM_EXT_CACHE_DCACHE_CLEAN	0x3U
#define HM_EXT_CACHE_DCACHE_INVAL	0x4U

/* libc should not depend on other libs. So we re-write one ptr_add for libc */
#ifndef ptr_add
#define ptr_add(b, o) ({							\
		       uintptr_t __b = (uintptr_t)(b);				\
		       uintptr_t __p = (uintptr_t)&(((uint8_t *)(__b))[o]);	\
										\
		       (void *)__p;						\
})
#endif

/* To reduce the time spent in kernel mode. */
static inline __always_inline int __hm_ext_cache_process(void *va, size_t size, uint32_t cache_cmd)
{
	int ret = 0;
	void *end;
	void *start = va;
	size_t length;
	size_t cur_size = size;

	while (cur_size > 0U) {
		length = (size_t)(unsigned int)hm_ext_syscall(HM_EXT_CMD_FLUSH_CACHE,
							      start, (uint32_t)cur_size,
							      NULL, cache_cmd);
		if (length == 0U || length > cur_size) {
			ret = EINVAL;
			break;
		}
		cur_size -= length;
		end = ptr_add(start, length);
		if (start > end) {
			ret = EINVAL;
			break;
		} else {
			start = end;
		}
	}

	return ret;
}

static inline int hm_icache_flush(void *va, size_t size)
{
	return __hm_ext_cache_process(va, size, (uint32_t)HM_EXT_CACHE_ICACHE_FLUSH);
}

static inline int hm_dcache_flush(void *va, size_t size)
{
	return __hm_ext_cache_process(va, size, (uint32_t)HM_EXT_CACHE_DCACHE_FLUSH);
}

static inline int hm_dcache_invalid(void *va, size_t size)
{
	return __hm_ext_cache_process(va, size, (uint32_t)HM_EXT_CACHE_DCACHE_INVAL);
}

static inline int hm_dcache_clean(void *va, size_t size)
{
	return __hm_ext_cache_process(va, size, (uint32_t)HM_EXT_CACHE_DCACHE_CLEAN);
}

static inline int hm_cache_flush(void *va, size_t size)
{
	return __hm_ext_cache_process(va, size, (uint32_t)HM_EXT_CACHE_CACHE_FLUSH);
}

#endif
