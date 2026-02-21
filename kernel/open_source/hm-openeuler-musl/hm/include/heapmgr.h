/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 11 17:57:25 2020
 */

#ifndef HEAPMGR_HEAPMGR_H
#define HEAPMGR_HEAPMGR_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <malloc.h>

#define ATTR_HIDDEN __attribute__((__visibility__("hidden")))
#define ATTR_MALLOC __attribute__((malloc))

#define M_HEAPMGR_SET_NUM_HEAPS (-2048)
#define M_HEAPMGR_SET_NUM_CACHED (-2049)

#if SIZE_MAX == UINT_MAX
#define HEAPMGR_CHUNK_ALIGNMENT 0x40000UL
#else
#define HEAPMGR_CHUNK_ALIGNMENT 0x80000UL
#endif

typedef struct heapmgr_state_s {
	size_t total;
	size_t allocated;
	size_t used;
	size_t cached;
} heapmgr_state_t;

/* exported functions */
void *heapmgr_malloc(size_t size) ATTR_MALLOC;
void *heapmgr_calloc(size_t count, size_t size) ATTR_MALLOC;
void *heapmgr_realloc(void *p, size_t newsize);
void heapmgr_free(void *p);
size_t heapmgr_usable_size(void *p);
int heapmgr_config(int param, int value);
void *heapmgr_memalign(size_t alignment, size_t size) ATTR_MALLOC;
int heapmgr_posix_memalign(void **pp, size_t alignment, size_t size);
void *heapmgr_valloc(size_t size) ATTR_MALLOC;
void *heapmgr_pvalloc(size_t size_arg) ATTR_MALLOC;

void heapmgr_shrink(unsigned int flags);
void heapmgr_print_state(unsigned int flags, FILE *f);
void heapmgr_dump_state(unsigned int flags, heapmgr_state_t *s);

/* backend should implement following functions */
void heapmgr_set_errno(int err, const char *func, int line) ATTR_HIDDEN;
void heapmgr_log(const char *fmt, ...) ATTR_HIDDEN  __attribute__((format(printf, 1, 2)));;
void heapmgr_fatal(const char *msg, const char *func, int line) ATTR_HIDDEN;
void *heapmgr_memory_map(size_t size, size_t alignment) ATTR_HIDDEN;
void heapmgr_memory_unmap(void *p, size_t size) ATTR_HIDDEN;
void heapmgr_memory_trim(void *p, size_t size, const size_t alignment) ATTR_HIDDEN;
void heapmgr_fprintf(FILE *f, const char *fmt, ...) ATTR_HIDDEN __attribute__((format(printf, 2, 3)));

#endif /* HEAPMGR_HEAPMGR_H */
