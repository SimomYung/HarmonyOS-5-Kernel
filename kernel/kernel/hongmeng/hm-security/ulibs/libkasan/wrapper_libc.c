/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of wrapped libc function
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
/* direct header */
#include "macro.h"
#include "wrapper_libc.h"

/* libc */
#include <dlfcn.h>
#include <stdint.h>

/* hongmeng ulibs */
#include <libmem/utils.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhwsecurec/securec.h>

/* hm-security */
#include <libkasan/kasan.h>
#include <libkasan/kasan_init.h>

#include "utils.h"
#include "shadow.h"
#include "asan_print.h"
#include "wrapper_libc_define.h"

#define KASAN_DELAY_FREE_MAX_SIZE (size_t)(CONFIG_KASAN_DELAY_FREE_SIZE_MB << 20)
#define KASAN_DELAY_FREE_WATER_MARK (size_t)(KASAN_DELAY_FREE_MAX_SIZE * 4)
#define FREE_COUNT_MAX 5

DLIST_HEAD(delay_free_list);
DLIST_HEAD(delay_unmap_list);

size_t total_delay_free_size = 0;
size_t total_delay_unmap_size = 0;
__thread unsigned int wrap_malloc_count = 0;

static inline size_t *delay_free_size(size_t free_size)
{
	return (free_size > __PAGE_SIZE) ? &total_delay_unmap_size : &total_delay_free_size;
}

static inline struct dlist_node *_delay_free_list(size_t free_size)
{
	return (free_size > __PAGE_SIZE) ? &delay_unmap_list : &delay_free_list;
}

/* allocator type, FROM_OTHER_ALLOC must be max value */
#define FROM_MALLOC_ALLOC (uint8_t)0 /* alloc by libc malloc */
#define FROM_NEW_ALLOC    (uint8_t)1 /* alloc by new */
#define FROM_NEW_BR_ALLOC (uint8_t)2 /* alloc by "new []" */
#define FROM_OTHER_ALLOC  (uint8_t)3 /* alloc by user alloc interface */

/* chunk poison value for different allocators, just like malloc, new, VOS_malloc */
#define ALLOCATOR_POISON_MAGIC_BASE (uint8_t)0xf0
#define MALLOC_POISON_MAGIC ALLOCATOR_POISON_MAGIC_BASE
#define NEW_POISON_MAGIC (uint8_t)(ALLOCATOR_POISON_MAGIC_BASE + FROM_NEW_ALLOC)
#define NEW_BR_POISON_MAGIC (uint8_t)(ALLOCATOR_POISON_MAGIC_BASE + FROM_NEW_BR_ALLOC)
#define OTHER_POISON_MAGIC (uint8_t)(ALLOCATOR_POISON_MAGIC_BASE + FROM_OTHER_ALLOC)

#define FUNC_NAME_LEN 32

struct asan_allocator_s {
	char name[FUNC_NAME_LEN];
	uint8_t poison_magic;
};

const struct asan_allocator_s asan_allocator[] = {
	{.name = "malloc", .poison_magic = MALLOC_POISON_MAGIC},
	{.name = "new", .poison_magic = NEW_POISON_MAGIC},
	{.name = "new_br", .poison_magic = NEW_BR_POISON_MAGIC},
	{.name = "other", .poison_magic = OTHER_POISON_MAGIC},
};

struct raw_mutex kasan_delay_free_mutex = RAW_MUTEX_INITIALIZER;

static raw_atomic_int_t kasan_map_ref_cnt = {0};

void kasan_init_map_ref(void)
{
	raw_atomic_int_init(&kasan_map_ref_cnt, 0);
}

void kasan_set_map_ref(void)
{
	int ret = 0;

	ret = raw_atomic_int_inc_unless_negative(&kasan_map_ref_cnt);
	if (ret != 0) {
		hm_warn("failed to increase map ref\n");
	}
}

void kasan_unset_map_ref(void)
{
	raw_atomic_int_dec(&kasan_map_ref_cnt);
}

#ifdef KASAN_SHARED
#define KASAN_WRAPPER_DEFINE
#define KASAN_WRAPPER_VOID(libc_func_name, ARG) \
static void (*_##libc_func_name)(_PARA(PACK(ARG))) = NULL; \
void kasan_reg_##libc_func_name(void) \
{ \
	_##libc_func_name = dlsym(RTLD_NEXT, #libc_func_name); \
} \
void ori_##libc_func_name(_PARA(PACK(ARG))) \
{ \
	BUG_ON(_##libc_func_name == NULL); \
	_##libc_func_name(_ARGS(PACK(ARG))); \
	return; \
}
#define KASAN_WRAPPER(ret_type, libc_func_name, ARG) \
static ret_type (*_##libc_func_name)(_PARA(PACK(ARG))) = NULL; \
void kasan_reg_##libc_func_name(void) \
{ \
	_##libc_func_name = dlsym(RTLD_NEXT, #libc_func_name); \
} \
ret_type ori_##libc_func_name(_PARA(PACK(ARG))) \
{ \
	if (_##libc_func_name == NULL) { \
		kasan_reg_##libc_func_name(); \
	} \
	BUG_ON(_##libc_func_name == NULL); \
	return _##libc_func_name(_ARGS(PACK(ARG))); \
}
#include "wrapper_libc_api.h"
#undef KASAN_WRAPPER
#undef KASAN_WRAPPER_VOID
#undef KASAN_WRAPPER_DEFINE
#endif /* KASAN_SHARED */

#define get_shadow_meta(node) container_of(node, shadow_meta, free_node)

struct kasan_malloc_optimal_node {
	size_t upper_limit;
	size_t chunk_size;
};

/*
	Adapt trunk size for malloc
	malloc size in [0, 512 - 64), chunk_size is 64
	malloc size in [512 - 64, 4096 - 128), chunk_size is 128
	malloc size in [4096 - 128, 16384 - 256), chunk_size is 256
	malloc size in [16384 - 256, 32768 - 512), chunk_size is 512
	malloc size in [32768 - 512, 65536 - 1024), chunk_size is 1024
	malloc size is greater than 65536, chunk_size is 2048
*/
#define KASAN_MALLOC_OPTIMAL_NUM	6
static size_t kasan_get_optimal_size(size_t malloc_aligned_size)
{
	int i;
	size_t optimal_size = MALLOC_CHUNK;
	const struct kasan_malloc_optimal_node kasan_optimal[KASAN_MALLOC_OPTIMAL_NUM] = {
		{.upper_limit = 512, .chunk_size = 64},
		{.upper_limit = 4096, .chunk_size = 128},
		{.upper_limit = 16384, .chunk_size = 256},
		{.upper_limit = 32768, .chunk_size = 512},
		{.upper_limit = 65536, .chunk_size = 1024},
		{.upper_limit = UINT32_MAX, .chunk_size = 2048}
	};

	for (i = 0; i < KASAN_MALLOC_OPTIMAL_NUM; i++) {
		if (malloc_aligned_size < kasan_optimal[i].upper_limit - kasan_optimal[i].chunk_size) {
			optimal_size = kasan_optimal[i].chunk_size;
			break;
		}
	}

	return optimal_size;
}

size_t kasan_get_real_sz(size_t sz)
{
	const size_t aligned_size = ALIGN_UP(sz, KASAN_SHADOW_SCALE_SIZE);
	const size_t chunk_size = kasan_get_optimal_size(aligned_size);
	return chunk_size + aligned_size;
}

uintptr_t kasan_fill_chunk_and_acquire_user_start(uintptr_t real_start, size_t sz,
						  size_t real_sz, unsigned int alloc_type)
{
	const size_t aligned_size = ALIGN_UP(sz, KASAN_SHADOW_SCALE_SIZE);
	const size_t chunk_size = kasan_get_optimal_size(aligned_size);
	uintptr_t user_start = real_start + MALLOC_CHUNK;
	/* head chunk has allocator type info */
	kasan_poison_shadow(real_start, MALLOC_CHUNK, asan_allocator[alloc_type].poison_magic);
	kasan_unpoison_shadow(user_start, sz);
	kasan_poison_shadow(user_start + aligned_size, chunk_size - MALLOC_CHUNK,
			    asan_allocator[alloc_type].poison_magic);
	shadow_meta *sm = (shadow_meta *)(user_start - sizeof(shadow_meta));
	sm->rtu = (intptr_t)(real_start - user_start);
	sm->real_sz = real_sz;
	sm->user_sz = sz;
	sm->magic = asan_allocator[alloc_type].poison_magic;
	return user_start;
}

static void *malloc_ex(size_t sz, unsigned int alloc_type)
{
	/*
	 * 1. we get an buf from malloc in libc, then pad it and align up to SHADOW_SCALE
	 * 2. put a kasan chunk before buffer returned to user
	 * 3. put pad after user_buf to align to shadow_scale
	 * 4. put a kasan chunk after pad
	 * 5. poison the padding and chunk before and after the user buffer
		    real_start->|--------|
				|  chunk |
		    user_start->|--------|
				|user_buf|
				|--------|
				|   pad  |
				|--------|<---align to shadow_scale
				|  chunk |
				|--------|
	 */
	uintptr_t real_start = 0;
	const size_t real_sz = kasan_get_real_sz(sz);
	if (real_sz < sz || alloc_type > FROM_OTHER_ALLOC) {
		return NULL;
	}

	if (real_sz <= PAGE_SIZE) {
		BUG_ON(wrap_malloc_count > 0);
		wrap_malloc_count++;
	}
	real_start = (uintptr_t)ori_malloc(real_sz);
	if (real_sz <= PAGE_SIZE) {
		wrap_malloc_count--;
	}
	if (real_start == 0) {
		return NULL;
	}
	return (void *)kasan_fill_chunk_and_acquire_user_start(real_start, sz, real_sz, alloc_type);
}

static void *mmap_ex(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	int ret;
	void *mmap_addr = NULL;

	mmap_addr = ori_mmap(addr, length, prot, flags, fd, offset);
	if (mmap_addr != MAP_FAILED) {
		if (prot != PROT_NONE) {
			ret = kasan_map_shadow_range((uintptr_t)mmap_addr,
						     (uintptr_t)mmap_addr + ALIGN_UP(length, __PAGE_SIZE));
			if (ret < 0) {
				(void)ori_munmap(mmap_addr, length);
				mmap_addr = MAP_FAILED;
			}
		}
	}

	return mmap_addr;
}

static int munmap_ex(void *addr, size_t length)
{
	size_t aligned_len = ALIGN_UP(length, __PAGE_SIZE);
	kasan_unmap_shadow_range((uintptr_t)addr, (uintptr_t)addr + aligned_len);

	return ori_munmap(addr, length);
}

static int mprotect_ex(void *addr, size_t length, int prot)
{
	int ret;
	size_t aligned_len = (size_t)ALIGN_UP(length, __PAGE_SIZE);

	ret = ori_mprotect(addr, length, prot);
	if (ret == E_HM_OK) {
		if (prot != PROT_NONE) {
			(void)kasan_mprotect_shadow_range((uintptr_t)addr, (uintptr_t)addr + aligned_len);
		}
	}

	return ret;
}

static inline size_t kasan_get_delay_free_max_size(void)
{
	return KASAN_DELAY_FREE_MAX_SIZE;
}

static inline size_t kasan_get_delay_free_water_mark(void)
{
	return KASAN_DELAY_FREE_WATER_MARK;
}

static void kasan_delay_free_list(size_t free_size)
{
	struct delay_free_node *pos = NULL;
	struct dlist_node *delnode = NULL;
	size_t *total_free_size = delay_free_size(free_size);
	struct dlist_node *free_list = _delay_free_list(free_size);
	int free_count = FREE_COUNT_MAX;

	while (free_count > 0) {
		if (*total_free_size < kasan_get_delay_free_max_size()) {
			break;
		}

		raw_mutex_lock(&kasan_delay_free_mutex);
		delnode = dlist_get_first(free_list);
		if (delnode != free_list) {
			pos = dlist_entry(delnode, struct delay_free_node, node);
			dlist_delete(delnode);
			*total_free_size -= get_shadow_meta(pos)->real_sz;
		} else {
			pos = NULL;
		}
		raw_mutex_unlock(&kasan_delay_free_mutex);

		if (pos == NULL) {
			break;
		}

		kasan_poison_shadow((uintptr_t)pos->real_free_addr, get_shadow_meta(pos)->real_sz, 0);
		ori_free((void *)pos->real_free_addr);
		free_count--;
	}

	return;
}

static kasan_store_free_func_t kasan_store_free_func = NULL;
void kasan_store_free_func_set(kasan_store_free_func_t func)
{
	kasan_store_free_func = func;
}
static void kasan_delay_free_add_list(uintptr_t free_addr, shadow_meta *sm)
{
	struct delay_free_node *free_node = NULL;
	size_t *real_free_size = delay_free_size(sm->real_sz);

	/* when real_sz is greater than max size, the delay_free_function is useless */
	if (sm->real_sz >= kasan_get_delay_free_max_size()) {
		kasan_poison_shadow(free_addr, sm->real_sz, 0);
		ori_free((void *)free_addr);
		return;
	}

	if (raw_atomic_int_read(&kasan_map_ref_cnt) == 0 &&
	    *delay_free_size(sm->user_sz) >= kasan_get_delay_free_max_size()) {
		kasan_delay_free_list(sm->user_sz);
	}

	raw_mutex_lock(&kasan_delay_free_mutex);
	if (*delay_free_size(sm->real_sz) >= kasan_get_delay_free_water_mark()) {
		kasan_poison_shadow(free_addr, sm->real_sz, 0);
		raw_mutex_unlock(&kasan_delay_free_mutex);
		ori_free((void *)free_addr);
		return;
	}

	free_node = &sm->free_node;
	kasan_poison_shadow((uintptr_t)free_node, sizeof(struct delay_free_node),
				FREE_POISON_VALUE);
	free_node->real_free_addr = free_addr;
	dlist_insert_tail(_delay_free_list(sm->real_sz), &free_node->node);
	*real_free_size += sm->real_sz;
	if (kasan_store_free_func && sm->real_sz <= __PAGE_SIZE) {
		kasan_store_free_func(free_addr);
	}
	raw_mutex_unlock(&kasan_delay_free_mutex);

	return;
}

static void kasan_double_free_check(uintptr_t free_addr, size_t free_size)
{
	struct delay_free_node *pos = NULL;
	struct delay_free_node *next = NULL;

	raw_mutex_lock(&kasan_delay_free_mutex);
	dlist_for_each_entry_safe(pos, next, _delay_free_list(free_size),
				  struct delay_free_node, node) {
		if (free_addr >= pos->real_free_addr &&
		    free_addr < pos->real_free_addr + get_shadow_meta(pos)->real_sz) {
			hm_error("kasan detected double free. "
				 "(addr=%p, pre_freed by code_addr=%p)\n",
				 (void *)free_addr,
				 (void *)get_shadow_meta(pos)->free_addr);
			break;
		}
	}
	raw_mutex_unlock(&kasan_delay_free_mutex);

	return;
}

static void free_ex(void *p, unsigned int alloc_type, uintptr_t ret_ip)
{
	if (p == NULL) {
		return;
	}

	shadow_meta *sm = (shadow_meta *)((uintptr_t)p - sizeof(shadow_meta));
	uintptr_t real_start = (uintptr_t)p + (uintptr_t)sm->rtu;
	uintptr_t real_sz = sm->real_sz;
	/* real_sz must be aligned to KASAN_SHADOW_SCALE_SIZE */
	if (KASAN_INITED) {
		/* check for double free */
		if (kasan_memory_is_invalid((uintptr_t)p, sm->user_sz)) {
			kasan_double_free_check((uintptr_t)p, sm->real_sz);
			asan_report_address((uintptr_t)p, sm->user_sz, false, ret_ip);
		}
		/* check for mismatch */
		if (sm->magic != asan_allocator[alloc_type].poison_magic) {
			unsigned int real_type = (unsigned int)(sm->magic - ALLOCATOR_POISON_MAGIC_BASE);
			const char *alloc_name = (real_type <= FROM_OTHER_ALLOC) ? asan_allocator[real_type].name :
										   "unknown";
			asan_report_alloc_mismatch(asan_allocator[alloc_type].name, alloc_name);
		}
		kasan_poison_shadow(real_start, real_sz, FREE_POISON_VALUE);
		sm->free_addr = ret_ip;
		kasan_delay_free_add_list(real_start, sm);
	} else {
		ori_free((void *)real_start);
	}

	return;
}

static void *calloc_ex(size_t m, size_t n, unsigned int alloc_type, uintptr_t ret_ip)
{
	size_t nbytes = m * n;
	int err = E_HM_OK;
	void *tmp = NULL;

	/* overflow check */
	if (n != 0 && m != nbytes / n) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		tmp = malloc_ex(nbytes, alloc_type);
		if (tmp != NULL) {
			/* memset_s will not fail */
			err = memset_s(tmp, nbytes, 0, nbytes);
			if (err != 0) {
				free_ex(tmp, alloc_type, ret_ip);
				tmp = NULL;
			}
		}
	}

	return tmp;
}

#define get_user_sz(p) (((shadow_meta *)((uintptr_t)(p) - sizeof(shadow_meta)))->user_sz)

static void *realloc_ex(void *p, size_t n, unsigned int alloc_type, uintptr_t ret_ip)
{
	int err;
	if (p == NULL) {
		return malloc_ex(n, alloc_type);
	}
	if (n == 0) {
		free_ex(p, alloc_type, ret_ip);
		return NULL;
	}
	size_t user_sz = get_user_sz(p);
	size_t need_cp = min(n, user_sz);
	void *ret_buf = malloc_ex(n, alloc_type);
	if (ret_buf == NULL) {
		return NULL;
	}
	err = memcpy_s(ret_buf, n, p, need_cp);
	if (err != 0) {
		free_ex(ret_buf, alloc_type, ret_ip);
		return NULL;
	}
	free_ex(p, alloc_type, ret_ip);
	return ret_buf;
}

#ifdef KASAN_SHARED
void *malloc(size_t sz)
#else
void *__wrap_malloc(size_t sz)
#endif
{
	return malloc_ex(sz, FROM_MALLOC_ALLOC);
}

#ifdef KASAN_SHARED
void free(void *p)
#else
void __wrap_free(void *p)
#endif
{
	free_ex(p, FROM_MALLOC_ALLOC, _RET_IP_);
}

#ifdef KASAN_SHARED
void *realloc(void *p, size_t n)
#else
void *__wrap_realloc(void *p, size_t n)
#endif
{
	return realloc_ex(p, n, FROM_MALLOC_ALLOC, _RET_IP_);
}

#ifdef KASAN_SHARED
void *calloc(size_t m, size_t n)
#else
void *__wrap_calloc(size_t m, size_t n)
#endif
{
	return calloc_ex(m, n, FROM_MALLOC_ALLOC, _RET_IP_);
}

#ifdef KASAN_SHARED
void *mmap(void *addr, size_t length, int prot, int flags,
	   int fd, off_t offset)
#else
void *__wrap_mmap(void *addr, size_t length, int prot, int flags,
		  int fd, off_t offset)
#endif
{
	return mmap_ex(addr, length, prot, flags, fd, offset);
}

#ifdef KASAN_SHARED
int munmap(void *addr, size_t length)
#else
int __wrap_munmap(void *addr, size_t length)
#endif
{
	return munmap_ex(addr, length);
}

#ifdef KASAN_SHARED
int mprotect(void *addr, size_t length, int prot)
#else
int __wrap_mprotect(void *addr, size_t length, int prot)
#endif
{
	return mprotect_ex(addr, length, prot);
}

void *asan_libc_malloc(size_t sz)
{
	return malloc_ex(sz, FROM_MALLOC_ALLOC);
}

void *asan_libc_realloc(void *p, size_t n)
{
	return realloc_ex(p, n, FROM_MALLOC_ALLOC, _RET_IP_);
}

void *asan_libc_calloc(size_t m, size_t n)
{
	return calloc_ex(m, n, FROM_MALLOC_ALLOC, _RET_IP_);
}

void *asan_new_alloc(size_t sz)
{
	return malloc_ex(sz, FROM_NEW_ALLOC);
}

void *asan_new_br_alloc(size_t sz)
{
	return malloc_ex(sz, FROM_NEW_BR_ALLOC);
}

void *asan_other_malloc(size_t sz)
{
	return malloc_ex(sz, FROM_OTHER_ALLOC);
}

void *asan_other_realloc(void *p, size_t n)
{
	return realloc_ex(p, n, FROM_OTHER_ALLOC, _RET_IP_);
}

void *asan_other_calloc(size_t m, size_t n)
{
	return calloc_ex(m, n, FROM_OTHER_ALLOC, _RET_IP_);
}

void asan_libc_free(void *p)
{
	free_ex(p, FROM_MALLOC_ALLOC, _RET_IP_);
}

void asan_new_free(void *p)
{
	free_ex(p, FROM_NEW_ALLOC, _RET_IP_);
}

void asan_new_br_free(void *p)
{
	free_ex(p, FROM_NEW_BR_ALLOC, _RET_IP_);
}

void asan_other_free(void *p)
{
	free_ex(p, FROM_OTHER_ALLOC, _RET_IP_);
}

#if defined(KASAN_SHARED) || defined(__HOST_LLT__)
static void *kasan_adjust_shadow_meta(uintptr_t old_addr, size_t alignment, size_t size)
{
	size_t real_sz;
	uintptr_t real_start;
	uintptr_t new_addr;
	shadow_meta *old_sm = NULL;
	shadow_meta *new_sm = NULL;
	uint8_t poison_magic;

	new_addr = ALIGN_UP(old_addr, alignment);

	new_sm = (shadow_meta *)(new_addr - sizeof(shadow_meta));

	if (old_addr != new_addr) {
		old_sm = (shadow_meta *)(old_addr - sizeof(shadow_meta));
		real_sz = old_sm->real_sz;
		real_start = (uintptr_t)old_addr + (uintptr_t)old_sm->rtu;
		poison_magic = old_sm->magic;
		new_sm->rtu = (intptr_t)(real_start - new_addr);
		new_sm->real_sz = real_sz;
		new_sm->magic = poison_magic;
	}

	new_sm->user_sz = size;

	kasan_poison_shadow(old_addr, size + alignment - 1, ALIGN_POISON_VALUE);
	kasan_unpoison_shadow(new_addr, size);

	return (void *)new_addr;
}

#ifdef __HOST_LLT__
void *__wrap_memalign(size_t alignment, size_t size)
#else
void *memalign(size_t alignment, size_t size)
#endif
{
	uintptr_t mem;
	void *ret_addr = NULL;

	if ((alignment & (alignment - 1)) == 0U) {
		if (alignment <= KASAN_SHADOW_SCALE_SIZE) {
			ret_addr = malloc_ex(size, FROM_MALLOC_ALLOC);
		} else {
			mem = (uintptr_t)malloc_ex(size + alignment - 1, FROM_MALLOC_ALLOC);
			if ((void *)mem != NULL) {
				ret_addr = kasan_adjust_shadow_meta(mem, alignment, size);
			}
		}
	}

	return ret_addr;
}

#ifdef __HOST_LLT__
void *__wrap_aligned_alloc(size_t align, size_t len)
#else
void *aligned_alloc(size_t align, size_t len)
#endif
{
	uintptr_t mem;
	void *ret_addr = NULL;

	if ((align & (align - 1)) == 0U) {
		if (align <= KASAN_SHADOW_SCALE_SIZE) {
			ret_addr = malloc_ex(len, FROM_MALLOC_ALLOC);
		} else {
			mem = (uintptr_t)malloc_ex(len + align - 1, FROM_MALLOC_ALLOC);
			if ((void *)mem != NULL) {
				ret_addr = kasan_adjust_shadow_meta(mem, align, len);
			}
		}
	}

	return ret_addr;
}

#ifdef __HOST_LLT__
size_t __wrap_malloc_usable_size(void *ptr)
#else
size_t malloc_usable_size(void *ptr)
#endif
{
	int ret = E_HM_OK;
	size_t ptr_size = 0;
	shadow_meta *sm = NULL;

	if (ptr == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		sm = (shadow_meta *)((uintptr_t)ptr - sizeof(shadow_meta));
		if (sm != NULL) {
			ptr_size = sm->user_sz;
			if (KASAN_INITED) {
				__asan_loadN((uintptr_t)ptr, ptr_size);
			}
		}
	}

	return ptr_size;
}
#endif

#ifndef __HOST_LLT__
#ifdef KASAN_SHARED
void *memcpy(void *__restrict dst, const void *__restrict src, size_t n)
#else
void *__wrap_memcpy(void *__restrict dst, const void *__restrict src, size_t n)
#endif
{
	__asan_storeN_noabort((unsigned long)(uintptr_t)dst, n);
	__asan_loadN_noabort((unsigned long)(uintptr_t)src, n);
	return ori_memcpy(dst, src, n);
}

#ifdef KASAN_SHARED
void *memset(void *s, int c, size_t n)
#else
void *__wrap_memset(void *s, int c, size_t n)
#endif
{
	__asan_storeN_noabort((unsigned long)(uintptr_t)s, n);
	return ori_memset(s, c, n);
}
#endif
