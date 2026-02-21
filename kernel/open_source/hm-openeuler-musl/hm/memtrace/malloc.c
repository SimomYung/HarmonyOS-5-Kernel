/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020 ~ 2020. All rights reserved.
 * Description: Wrapper for malloc/free with memtrace for shared library
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 17 16:07:18 2020
 */
#if defined(COMPILE_SHARE) && defined(DYN_MEMTRACE)

#include "memtrace.h"

#include <stdint.h>
#include <stdbool.h>
#include <dlfcn.h>

typedef void *(*malloc_func_t)(size_t n);
typedef void (*free_func_t)(void *p);
typedef void *(*realloc_func_t)(void *p, size_t n);

static malloc_func_t __malloc = NULL;
static free_func_t __free = NULL;
static realloc_func_t __realloc = NULL;

void *malloc(size_t n)
{
	void *ptr = NULL;
	if (__malloc == NULL) {
		__malloc = (malloc_func_t)dlsym(RTLD_NEXT, "malloc");
		if (__malloc == NULL) {
			return NULL;
		}
	}
	ptr = __malloc(n);
	if (ptr != NULL) {
		mtrace_record_malloc(ptr, n);
	}
	return ptr;
}

void free(void *p)
{
	if (__free == NULL) {
		__free = (free_func_t)dlsym(RTLD_NEXT, "free");
		if (__free == NULL) {
			return;
		}
	}
	mtrace_record_free(p);
	__free(p);
}

void *realloc(void *p, size_t n)
{
	size_t size = 0UL;
	void *ptr = NULL;

	if (__realloc == NULL) {
		__realloc = (realloc_func_t)dlsym(RTLD_NEXT, "realloc");
		if (__realloc == NULL) {
			return NULL;
		}
	}
	size = mtrace_get_obj_size(p);
	ptr = __realloc(p, n);
	if (ptr != NULL) {
		if (p != NULL) {
			mtrace_record_free(p);
		}
		mtrace_record_malloc(ptr, n);
	}
	return ptr;
}
#endif /* DYN_MEMTRACE && COMPILE_SHARE */
