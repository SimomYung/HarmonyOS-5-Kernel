/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Header file for kasan initialization
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */

#ifndef __KASAN_INIT_H_
#define __KASAN_INIT_H_

#include <stdbool.h>
#include <libkasan/mutex.h>
#include <libmem/utils.h>

#define MAPS_SIZE (1024 * 1024)
#define KASAN_SHADOW_BEG_SCALE 16
#define KASAN_SHADOW_SHIFT 3
#define KASAN_SHADOW_SCALE_SIZE 8
#define KASAN_SHADOW_MASK (KASAN_SHADOW_SCALE_SIZE - 1)

#if defined (__aarch64__)
/*
 * kasan only support 39 bit va bits, and will fixup the kernel config
 * when enable kasan feature
 */
#if defined (CONFIG_PRIVILEGED_KASAN)
/*
 * kasan addr adaption for sysmgr privilege
 */
#define KASAN_ADDR_BASE  0xffffff8000000000ULL
#define KASAN_ADDR_BEGIN (KASAN_ADDR_BASE + PAGE_ALIGN_DOWN(shadow_size(KASAN_ADDR_LIMIT - KASAN_ADDR_BASE)))
#define KASAN_ADDR_LIMIT (0xffffff8000000000ULL + CONFIG_PRIV_VS_SIZE_SYSMGR)
#define KASAN_ADDR_BEGIN_UAPP 0x6666666000ULL
#define KASAN_ADDR_LIMIT_UAPP 0x8000000000ULL
#else
#define KASAN_ADDR_BASE  0x0ULL
#define KASAN_ADDR_BEGIN 0x6666666000ULL
#define KASAN_ADDR_LIMIT 0x8000000000ULL
#endif
#else
#define KASAN_ADDR_BASE  0x0ULL
#define KASAN_ADDR_BEGIN 0x66666000ULL
#define KASAN_ADDR_LIMIT 0x80000000ULL
#endif

struct spg_ref {
	uint8_t ref;
};

void kasan_init_shadow_beg(void);
void kasan_map_calculate_range(uintptr_t start, uintptr_t end,
			       uintptr_t *map_start, uintptr_t *map_end);
void kasan_add_ref(uintptr_t start, uintptr_t end);
uint8_t kasan_range_to_ref(uintptr_t start, uintptr_t end);
void kasan_unpoison_shadow(uintptr_t addr, size_t len);
void kasan_poison_shadow(uintptr_t addr, size_t len, uint8_t value);
void kasan_init_map_ref(void);
void kasan_set_map_ref(void);
void kasan_unset_map_ref(void);

/* NOTICE: For external usage, like kasan-sysmgr, avoid pointer usage */
bool kasan_get_inited(void);
void kasan_set_inited(bool val);
uintptr_t kasan_get_shadow_beg(void);
struct spg_ref *kasan_get_shadow_pgref(void);
void kasan_init_shadow_ref_mutex(void);
void kasan_lock_shadow_ref_mutex(void);
void kasan_unlock_shadow_ref_mutex(void);
void kasan_heap_mutex_init(void);

uintptr_t kasan_get_heap_start(void);
uintptr_t kasan_get_heap_end(void);
void kasan_set_heap_start(uintptr_t new_heap_start);
void kasan_set_heap_end(uintptr_t new_heap_end);
int kasan_calc_shadowref_len(uintptr_t maps, uintptr_t shadow_beg,
			    uintptr_t shadow_pgref, size_t *ref_len);
int kasan_mprotect_shadow_range(uintptr_t start, uintptr_t end);
#define KASAN_INITED (kasan_get_inited())
#define KASAN_SHADOW_BEG (kasan_get_shadow_beg())
#define KASAN_SHADOW_PGREF (kasan_get_shadow_pgref())

// 8 for shadow range shift
#define shadow_page(addr) (KASAN_SHADOW_BEG + PAGE_ALIGN_DOWN(((addr) - KASAN_ADDR_BASE) / 8))

#define shadow_size(addr) ((addr) * 4 / 5)

static inline uintptr_t kasan_mem_to_shadow(uintptr_t addr)
{
	return ((addr - KASAN_ADDR_BASE) >> KASAN_SHADOW_SHIFT) + KASAN_SHADOW_BEG;
}

#ifdef CONFIG_PRIVILEGED_KASAN
#define KASAN_SHADOW_BEG_UAPP (kasan_get_shadow_beg_uapp())
void kasan_init_shadow_beg_uapp(void);
uintptr_t kasan_get_shadow_beg_uapp(void);
#define shadow_page_uapp(addr) (KASAN_SHADOW_BEG_UAPP + PAGE_ALIGN_DOWN((addr) / 8))

static inline uintptr_t kasan_mem_to_shadow_uapp(uintptr_t addr)
{
	return (addr >> KASAN_SHADOW_SHIFT) + KASAN_SHADOW_BEG_UAPP;
}
#else
#define KASAN_SHADOW_BEG_UAPP (kasan_get_shadow_beg())
#define shadow_page_uapp(addr) (shadow_page(addr))
#define kasan_mem_to_shadow_uapp(addr) (kasan_mem_to_shadow(addr))
#endif

/*
 * Brief: asan alloc function for different allocator
 *
 * @param		the parameters are the same as the original function
 *
 * @return		alloc address
 * @note		alloc and free should be matched.
 *			if alloc and free are mismatched, process will exit
 *			and print error info
 * @see			None
 * @deprecated		None
 */
void *asan_libc_malloc(size_t sz);
void *asan_libc_realloc(void *p, size_t n);
void *asan_libc_calloc(size_t m, size_t n);
void *asan_new_alloc(size_t sz);
void *asan_new_br_alloc(size_t sz);
void *asan_other_malloc(size_t sz);
void *asan_other_calloc(size_t m, size_t n);
void *asan_other_realloc(void *p, size_t n);

/*
 * Brief: asan free function for different allocator
 *
 * @param		the parameters are the same as the original function
 *
 * @return		None
 * @note		alloc and free should be matched
 * @see			None
 * @deprecated		None
 */
void asan_libc_free(void *p);
void asan_new_free(void *p);
void asan_new_br_free(void *p);
void asan_other_free(void *p);

void asan_set_trap_enabled(bool val);
unsigned asan_report_num(void);

#ifdef __HOST_LLT__
void kasan_set_shadow_beg(uintptr_t bgn_addr);
#endif

size_t kasan_get_real_sz(size_t sz);
uintptr_t kasan_fill_chunk_and_acquire_user_start(uintptr_t real_start, size_t sz,
						  size_t real_sz, unsigned int alloc_type);

/* every 8 page can be mapped into one shadow page */
#define SHADOW_PAGE_RANGE (8 * __PAGE_SIZE)
#define SPG_SHIFT (3 + __PAGE_SHIFT)
#define REF_RANGE SHADOW_PAGE_RANGE

#endif
