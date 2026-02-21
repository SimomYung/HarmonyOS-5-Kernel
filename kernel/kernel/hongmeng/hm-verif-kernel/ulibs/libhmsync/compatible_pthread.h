/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Compatible description of struct pthread and __libc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 04 14:37:33 2019
 */

#ifndef ULIBS_LIBHMSYNC_COMPATIBLE_PTHREAD_H
#define ULIBS_LIBHMSYNC_COMPATIBLE_PTHREAD_H

#if !defined __HOST_LLT__ && defined CONFIG_RAW_THREAD_COMPATIBLE_WITH_PTHREAD
#include <errno.h>
#include <private/pthread_impl.h>
#define RAW_THREAD_PADDING 16
extern struct __libc *__get_libc(void);

#else

struct __locale_map;
struct __locale_struct {
	/* arrry with length 6 to compatible with libc */
	const struct __locale_map *cat[6];
};

struct pthread {
	pid_t tid;
	void *stack;
	size_t stack_size;
	void *map_base;
	size_t map_size;

	volatile int detach_state;
	void *result;

	void *start_arg;
	void *(*start)(void *);

	void *locale;
	struct {
		volatile void *volatile head;
		long off;
		volatile void *volatile pending;
	} robust_list;
	uintptr_t *dtv;

	struct pthread *prev;
	struct pthread *next;

	/* tsd used bit, will add more bits later */
	unsigned char tsd_used : 1;

	/* mark thread type for lock trace */
	char type;

	/* raw thread specific data */
	void **tsd;

	/* used for himalloc tcache addr */
	long long himalloc_tcache_addr;
};

struct tls_module {
	struct tls_module *next;
	void *image;
	size_t len, size, align, offset;
};

struct __libc {
	size_t tls_size;
	size_t tls_align;
	size_t tls_cnt;
	struct tls_module *tls_head;
	struct __locale_struct global_locale;
	int threads_minus_1;
};

extern struct __libc __libc;

static inline struct __libc *__get_libc(void)
{
	return &__libc;
}

enum {
	DT_EXITING = 0,
	DT_JOINABLE,
	DT_DETACHED,
};

#endif

#ifdef __ELIBC__
#define DETACH_STAT detach_state
#define THREAD_TYPE type
#define THREAD_TSD spec_data
#define THREAD_TSD_USED spec_used
#define THREAD_STACK stack_info.stack_addr
#define THREAD_STACK_INFO_SIZE stack_info.stack_size
#define THREAD_MALLOC_TCACHE malloc_tcache_addr
#define THREAD_MAP_BASE map_addr
#define THREAD_EXITCODE exitcode
#else
#define DETACH_STAT detach_state
#define THREAD_TYPE type
#define THREAD_TSD tsd
#define THREAD_TSD_USED tsd_used
#define THREAD_STACK stack
#define THREAD_STACK_INFO_SIZE stack_size
#define THREAD_MALLOC_TCACHE himalloc_tcache_addr
#define THREAD_MAP_BASE map_base
#define THREAD_EXITCODE result
#define __libc_thread_minus_1(libc) ((libc)->threads_minus_1)
#define __libc_global_locale(libc) (&(libc)->global_locale)
#endif
#endif
