/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Shadow memory operation for kasan
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
/* direct header */
#include "shadow.h"

/* libc */
#include <stdbool.h>

/* hongmeng ulibs */
#include <libhmlog/hmlog.h>
#include <libmem/utils.h>
#include <libkasan/kasan_init.h>

#include "asan_print.h"

#define MIN_PREFIX_LEN (KASAN_SHADOW_SCALE_SIZE * 2)

uintptr_t shadow_beg;
struct spg_ref *shadow_pgref;

uintptr_t kasan_get_shadow_beg(void)
{
	return shadow_beg;
}

struct spg_ref *kasan_get_shadow_pgref(void)
{
	return shadow_pgref;
}

#ifdef CONFIG_PRIVILEGED_KASAN
uintptr_t shadow_beg_uapp;

uintptr_t kasan_get_shadow_beg_uapp(void)
{
	return shadow_beg_uapp;
}
#endif

#if defined(__clang__)
#pragma clang attribute push (__attribute__((optnone)), apply_to=function)
#else
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif
/* len should be aligned to ASAN_SHADOW_SCALE_SIZE */
void kasan_poison_shadow(uintptr_t addr, size_t len, uint8_t value)
{
	uintptr_t i;
	uintptr_t s_bgn, s_end;

	if (!KASAN_INITED)
		return;
	s_bgn = kasan_mem_to_shadow(addr);
	s_end = kasan_mem_to_shadow(addr + len);

	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach (s_end - s_bgn). Since i is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: len is limited, so the
	 * loop condition is reasonable.
	 */
	for (i = 0; i < (s_end - s_bgn); i++)
		*((uint8_t *)s_bgn + i) = value;
}
#if defined(__clang__)
#pragma clang attribute pop
#else
#pragma GCC pop_options
#endif

void kasan_unpoison_shadow(uintptr_t addr, size_t len)
{
	if (!KASAN_INITED)
		return;
	kasan_poison_shadow(addr, len, 0);

	if ((len & KASAN_SHADOW_MASK) != 0U) {
		uint8_t *shadow = (uint8_t *)kasan_mem_to_shadow(addr + len);
		*shadow = len & KASAN_SHADOW_MASK;
	}
}

static inline unsigned long mem_is_zero(uintptr_t bgn,
					  size_t len)
{
	uintptr_t tmp_bgn = bgn;
	size_t tmp_len = len;
	/*
	 * Loop termination condition:
	 * Following loop terminate when len is 0, Since len is monotonic
	 * (keep decreasing in loop body), it has an lower bound.
	 * 0 is constant and reasonable: len is less than 16 according to memory_is_zero, 0 is reasonable.
	 */
	while (tmp_len != 0U) {
		if ((*(uint8_t *)tmp_bgn) != 0U) {
			return (uintptr_t)tmp_bgn;
		}
		tmp_bgn++;
		tmp_len--;
	}
	return 0;
}

static inline bool memory_is_poisoned_1(uintptr_t addr)
{
	int8_t s_value = *(int8_t *)kasan_mem_to_shadow(addr);
	if (s_value != 0) {
		int8_t last_accessible_byte = (uintptr_t)addr & KASAN_SHADOW_MASK;
		return (last_accessible_byte >= s_value);
	} else {
		return false;
	}
}

static bool memory_is_poisoned_2(uintptr_t bgn)
{
	uint16_t *s_addr = (uint16_t *)kasan_mem_to_shadow(bgn);

	if (ALIGN_DOWN((uintptr_t)s_addr, __PAGE_SIZE) == ALIGN_DOWN((uintptr_t)s_addr + 1, __PAGE_SIZE)) {
		if (*s_addr == 0U) {
			return false;
		}
	}
	if (*((uint8_t *)s_addr) != 0U) {
		if (memory_is_poisoned_1(bgn + 1)) {
			/* if last byte is poisoned, then it is poisoned */
			return true;
		}
		if ((((bgn + 1) & KASAN_SHADOW_MASK) != 0)) {
			/* if shadow does not cross two bytes,
			 * then it is not poisoned */
			return false;
		}
		return (bool)((*(uint8_t *)s_addr));
	} else {
		return false;
	}
}

static bool memory_is_poisoned_4(uintptr_t bgn)
{
	uint16_t *s_addr = (uint16_t *)kasan_mem_to_shadow(bgn);

	if (ALIGN_DOWN((uintptr_t)s_addr, __PAGE_SIZE) == ALIGN_DOWN((uintptr_t)s_addr + 1, __PAGE_SIZE)) {
		if (*s_addr == 0U) {
			return false;
		}
	}
	if (*((uint8_t *)s_addr) != 0U) {
		if (memory_is_poisoned_1(bgn + 3)) {
			/* if last byte is poisoned, then it is poisoned */
			return true;
		}

		if ((((unsigned long)(bgn + 3) & KASAN_SHADOW_MASK) >= 3)) {
			/* if shadow does not cross two bytes,
			 * then it is not poisoned
			 */
			return false;
		}
		return (bool)((*(uint8_t *)s_addr));
	} else {
		return false;
	}
}

static bool memory_is_poisoned_8(uintptr_t bgn)
{
	uint16_t *s_addr = (uint16_t *)kasan_mem_to_shadow(bgn);

	if (ALIGN_DOWN((uintptr_t)s_addr, __PAGE_SIZE) == ALIGN_DOWN((uintptr_t)s_addr + 1, __PAGE_SIZE)) {
		if (*s_addr == 0U) {
			return false;
		}
	}
	if (*((uint8_t *)s_addr) != 0U) {
		if (memory_is_poisoned_1(bgn + 7)) {
			/* if last byte is poisoned, then it is poisoned */
			return true;
		}
		if ((IS_ALIGNED(bgn, KASAN_SHADOW_SCALE_SIZE))) {
			/* if shadow does not cross two bytes,
			 * then it is not poisoned
			 */
			return false;
		}
		return (bool)((*(uint8_t *)s_addr));
	} else {
		return false;
	}
}

static bool memory_is_poisoned_16(uintptr_t bgn)
{
	uint32_t *s_addr = (uint32_t *)kasan_mem_to_shadow(bgn);

	if (ALIGN_DOWN((uintptr_t)s_addr, __PAGE_SIZE) ==
	    ALIGN_DOWN((uintptr_t)s_addr + sizeof(uint32_t) - 1, __PAGE_SIZE)) {
		if (*s_addr == 0U) {
			return false;
		}
	}
	if (*((uint16_t *)s_addr) != 0U) {
		uint16_t shadow_first_bytes = *ptr_to_type(s_addr, uint16_t *);
		if (shadow_first_bytes != 0U) {
			/* if first 2 shadow bytes are poisoned,
			 * then the range is poisoned.
			 */
			return true;
		}
		if ((IS_ALIGNED(bgn, KASAN_SHADOW_SCALE_SIZE))) {
			/*
			 * if shadow range do not cross 3 bytes,
			 * the mem range is not poisoned.
			 */
			return false;
		}
		return memory_is_poisoned_1(bgn + 15);
	} else {
		return false;
	}
}

/* to test whether memory is zero */
static unsigned long memory_is_zero(uintptr_t bgn, uintptr_t end)
{
	unsigned int bytes;
	unsigned long rc;
	uintptr_t tmp_bgn = bgn;
	uintptr_t prfx = (uintptr_t)(bgn - KASAN_ADDR_BASE) % KASAN_SHADOW_SCALE_SIZE;
	if (end - tmp_bgn <= MIN_PREFIX_LEN) {
		/* it is too short to test prefix */
		return mem_is_zero(tmp_bgn, end - tmp_bgn);
	}
	if (prfx != 0U) {
		prfx = KASAN_SHADOW_SCALE_SIZE - prfx;
		rc = mem_is_zero(tmp_bgn, prfx);
		if (rc != 0U) {
			return rc;
		}
		tmp_bgn += prfx;
	}
	bytes = (unsigned int)((end - tmp_bgn) / KASAN_SHADOW_SCALE_SIZE);
	/*
	 * Loop termination condition:
	 * Following loop terminate when bytes is 0, Since len is monotonic
	 * (keep decreasing in loop body), it has an lower bound.
	 * 0 is constant and reasonable: bytes is less than 16 according to memory_is_poisoned, 0 is reasonable.
	 */
	while (bytes != 0U) {
		if ((*(uint64_t *)tmp_bgn) != 0U) {
			return mem_is_zero(tmp_bgn, KASAN_SHADOW_SCALE_SIZE);
		}
		tmp_bgn += KASAN_SHADOW_SCALE_SIZE;
		bytes--;
	}
	return mem_is_zero(tmp_bgn, (end - tmp_bgn) % KASAN_SHADOW_SCALE_SIZE);
}

static bool memory_is_poisoned_n(uintptr_t addr, size_t size)
{
	unsigned long rc;
	rc = memory_is_zero(kasan_mem_to_shadow(addr),
			    kasan_mem_to_shadow(addr + size - 1) + 1);
	if (rc != 0U) {
		uintptr_t last_byte = (uintptr_t)addr + size - 1;
		int8_t *last_shadow = (int8_t *)kasan_mem_to_shadow(last_byte);
		if (rc != (uintptr_t)last_shadow ||
		    ((long)(last_byte & KASAN_SHADOW_MASK) >= *last_shadow)) {
			/*
			 * check last shadow addr and value
			 */
			return true;
		}
	}
	/* else false */
	return false;
}

static bool memory_is_poisoned(uintptr_t bgn, size_t len)
{
#ifdef CONFIG_PRIVILEGED_KASAN
	if (bgn > KASAN_ADDR_LIMIT || bgn < KASAN_ADDR_BASE) {
		return false;
	}
#else
	if (bgn > KASAN_ADDR_LIMIT) {
		return false;
	}
#endif
	if (__builtin_constant_p(len)) {
		switch (len) {
		case 1: /* fast path of len 1 */
			return memory_is_poisoned_1(bgn);
		case 2: /* fast path of len 2 */
			return memory_is_poisoned_2(bgn);
		case 4: /* fast path of len 4 */
			return memory_is_poisoned_4(bgn);
		case 8: /* fast path of len 8 */
			return memory_is_poisoned_8(bgn);
		case 16: /* fast path of len 16 */
			return memory_is_poisoned_16(bgn);
		default:
			hm_panic("wrong constant size!\n");
		}
	}
	return memory_is_poisoned_n(bgn, len);
}

static inline bool kasan_address_in_shadow_range(uintptr_t addr)
{
	return ((addr >= shadow_beg) && (addr < KASAN_ADDR_LIMIT));
}

static inline void check_memory_region_inline(uintptr_t addr, size_t size,
					      bool write,
					      uintptr_t ret_addr)
{
	if (size == 0 || !KASAN_INITED) {
		return;
	}

	if (kasan_address_in_shadow_range(addr) || memory_is_poisoned(addr, size)) {
		asan_report_address(addr, size, write, ret_addr);
		return;
	}
}

void check_memory_region(uintptr_t addr, size_t size, bool write, uintptr_t ret_addr)
{
	check_memory_region_inline(addr, size, write, ret_addr);
}

void __asan_poison_stack_memory(const void *addr, size_t len)
{
	kasan_poison_shadow((uintptr_t)addr, ALIGN_UP(len, KASAN_SHADOW_SCALE_SIZE),
			    STACK_POISON_VALUE);
}

void __asan_unpoison_stack_memory(const void *addr, size_t len)
{
	kasan_unpoison_shadow((uintptr_t)addr, len);
}

/* stub for __asan_handle_no_return */
void __asan_handle_no_return(void) {}

#define alias_attr(sym) __attribute__((alias(#sym)))

#define DEFINE_ASAN_LOADSTORE(size)							\
	void __asan_load##size(unsigned long bgn)					\
	{										\
		check_memory_region_inline((uintptr_t)bgn, size, false, _RET_IP_);	\
	}										\
	alias_attr(__asan_load##size)							\
	void __asan_load##size##_noabort(unsigned long bgn);				\
	void __asan_store##size(unsigned long bgn)					\
	{										\
		check_memory_region_inline((uintptr_t)bgn, size, true, _RET_IP_);	\
	}										\
	alias_attr(__asan_store##size)							\
	void __asan_store##size##_noabort(unsigned long bgn);

DEFINE_ASAN_LOADSTORE(1);
DEFINE_ASAN_LOADSTORE(2);
DEFINE_ASAN_LOADSTORE(4);
DEFINE_ASAN_LOADSTORE(8);
DEFINE_ASAN_LOADSTORE(16);

#define DEFINE_ASAN_REPORT_LOADSTORE(size)						\
	void __asan_report_load##size##_noabort(unsigned long bgn)			\
        {										\
                check_memory_region_inline((uintptr_t)bgn, size, false, _RET_IP_);	\
        }										\
        void __asan_report_store##size##_noabort(unsigned long bgn)			\
        {										\
                check_memory_region_inline((uintptr_t)bgn, size, true, _RET_IP_);	\
        }

DEFINE_ASAN_REPORT_LOADSTORE(1);
DEFINE_ASAN_REPORT_LOADSTORE(2);
DEFINE_ASAN_REPORT_LOADSTORE(4);
DEFINE_ASAN_REPORT_LOADSTORE(8);
DEFINE_ASAN_REPORT_LOADSTORE(16);

void __asan_loadN(unsigned long bgn, size_t size)
{
	check_memory_region_inline((uintptr_t)bgn, size, false, _RET_IP_);
}

alias_attr(__asan_loadN) void __asan_loadN_noabort(unsigned long, size_t);

void __asan_storeN(unsigned long bgn, size_t size)
{
	check_memory_region_inline((uintptr_t)bgn, size, true, _RET_IP_);
}

alias_attr(__asan_storeN) void __asan_storeN_noabort(unsigned long, size_t);

void __asan_report_load_n_noabort(unsigned long bgn, size_t size)
{
	check_memory_region_inline((uintptr_t)bgn, size, false, _RET_IP_);
}

void __asan_report_store_n_noabort(unsigned long bgn, size_t size)
{
	check_memory_region_inline((uintptr_t)bgn, size, true, _RET_IP_);
}

void __asan_alloca_poison(unsigned long bgn, size_t size)
{
	UNUSED(bgn);
	UNUSED(size);
}

void __asan_allocas_unpoison(unsigned long bgn, size_t size)
{
	UNUSED(bgn);
	UNUSED(size);
}

static void register_global(const struct kasan_global *glbl)
{
	size_t aligned_size = ALIGN_UP(glbl->size, KASAN_SHADOW_SCALE_SIZE);

	kasan_unpoison_shadow((uintptr_t)glbl->beg, glbl->size);

	kasan_poison_shadow((uintptr_t)glbl->beg + aligned_size,
			    glbl->size_with_redzone - aligned_size,
			    GLOBAL_POISON_VALUE);
}

void __asan_register_globals(struct kasan_global *glbls, size_t size)
{
	unsigned int i;

	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach size. Since i is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: size is limited, so the
	 * loop condition is reasonable.
	 */
	for (i = 0; i < size; i++)
		register_global(&glbls[i]);
}
void __asan_unregister_globals(__attribute__((unused)) struct kasan_global *glbls,
			       __attribute__((unused)) size_t size)
{
}

void poison_mem_in_aligned_range(uintptr_t vaddr, size_t size)
{
	uintptr_t start_pgalgn = ALIGN_DOWN(vaddr, __PAGE_SIZE);
	uintptr_t start_sclalgn = ALIGN_DOWN(vaddr, KASAN_SHADOW_SCALE_SIZE);
	uintptr_t end_pgalgn = ALIGN_UP(vaddr + size, __PAGE_SIZE);
	uintptr_t end_sclalgn = ALIGN_UP(vaddr + size,
						 KASAN_SHADOW_SCALE_SIZE);
	if (start_pgalgn != start_sclalgn) {
		kasan_poison_shadow(start_pgalgn,
				    (size_t)(start_sclalgn - start_pgalgn),
				    MEM_ALIGN_LEFT_POISON_VALUE);
	}
	if (((uintptr_t)vaddr + size) > start_sclalgn) {
		kasan_unpoison_shadow(start_sclalgn,
				     (size_t)((vaddr + size) - start_sclalgn));
	}
	if (end_pgalgn != end_sclalgn) {
		kasan_poison_shadow(end_sclalgn,
				    (size_t)(end_pgalgn - end_sclalgn),
				    MEM_ALIGN_RIGHT_POISON_VALUE);
	}
	return;
}

void kasan_init_shadow_beg(void)
{
	shadow_beg = (uintptr_t)(KASAN_ADDR_BASE + PAGE_ALIGN_DOWN(shadow_size(KASAN_ADDR_LIMIT -
		KASAN_ADDR_BASE)));
	shadow_pgref = (struct spg_ref *)(uintptr_t)PAGE_ALIGN_UP(shadow_beg +
		(shadow_beg - KASAN_ADDR_BASE) / KASAN_SHADOW_SCALE_SIZE);
}

#ifdef CONFIG_PRIVILEGED_KASAN
void kasan_init_shadow_beg_uapp(void)
{
	shadow_beg_uapp = (uintptr_t)PAGE_ALIGN_DOWN(shadow_size(KASAN_ADDR_LIMIT_UAPP));
}
#endif

/*
 * range is [start, end) , so we should use (end - 1) to caculate
 * the end bit
 */
uint8_t kasan_range_to_ref(uintptr_t start, uintptr_t end)
{
	uint8_t ret = 0;
	uint8_t i;
	if (start >= end) {
		return 0;
	}
	uint8_t start_bit = (uint8_t)(((start - KASAN_ADDR_BASE) % REF_RANGE) / __PAGE_SIZE);
	uint8_t end_bit = (uint8_t)((((end - 1) - KASAN_ADDR_BASE) % REF_RANGE) / __PAGE_SIZE);
	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach end_bit. Since i is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: end_bit is less than 8 according to shadow.h, so the
	 * loop condition is reasonable.
	 */
	for (i = start_bit; i <= end_bit; i++) {
		ret |= (uint8_t)((uint8_t)1 << i);
	}
	return ret;
}

void kasan_add_ref(uintptr_t start, uintptr_t end)
{
	uintptr_t start_up, end_down, temp;
	if (start >= end)  {
		hm_error("weird, mapped range not correct\n");
		return;
	}
	if (start >= shadow_beg) {
		return;
	}
	end_down = ALIGN_DOWN(end, REF_RANGE);
	start_up = ALIGN_UP(start, REF_RANGE);

	/*
	 * Loop termination condition:
	 * Following loop will terminate when temp reach end_down. Since temp is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: end_down is length of address according to kasan_unmap_shadow_range,
	 * which is limited, so the loop condition is reasonable.
	 */
	for (temp = start_up; temp < end_down; temp += REF_RANGE) {
		inc_ref(temp, temp + REF_RANGE);
	}

	if (start_up <= end_down) {
		inc_ref(start, start_up);
		inc_ref(end_down, end);
	} else {
		inc_ref(start, end);
	}
}

void kasan_remove_ref(uintptr_t start, uintptr_t end)
{
	uintptr_t start_up, end_down, temp;
	if (start >= end)  {
		hm_error("weird, mapped range not correct\n");
		return;
	}
	if (start >= shadow_beg) {
		return;
	}
	start_up = ALIGN_UP(start, REF_RANGE);
	end_down = ALIGN_DOWN(end, REF_RANGE);

	/*
	 * Loop termination condition:
	 * Following loop will terminate when temp reach end_down. Since temp is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: end_down is length of address according to init_shadow_ranges,
	 * which is limited, so the loop condition is reasonable.
	 */
	for (temp = start_up; temp < end_down; temp += REF_RANGE) {
		del_ref(temp, temp + REF_RANGE);
	}

	if (start_up <= end_down) {
		del_ref(start, start_up);
		del_ref(end_down, end);
	} else {
		del_ref(start, end);
	}
}

bool kasan_memory_is_invalid(uintptr_t addr, size_t size)
{
	if (size == 0 || !KASAN_INITED) {
		return false;
	}

	return (kasan_address_in_shadow_range(addr) || memory_is_poisoned(addr, size));
}
