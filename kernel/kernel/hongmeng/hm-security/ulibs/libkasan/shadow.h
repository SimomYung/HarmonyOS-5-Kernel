/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Header file for shadow memory
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
#ifndef __SHADOW_H_
#define __SHADOW_H_

#include <stdint.h>
#include <libmem/utils.h>
#include <libkasan/kasan_init.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>

#define DEFAULT_POISON_VALUE (uint8_t)0xff
#define UNMAP_POISON_VALUE (uint8_t)0xfe
#define FREE_POISON_VALUE (uint8_t)0xfd
#define STACK_POISON_VALUE (uint8_t)0xfc
#define GLOBAL_POISON_VALUE (uint8_t)0xfb
#define MEM_ALIGN_LEFT_POISON_VALUE (uint8_t)0xfa
#define MEM_ALIGN_RIGHT_POISON_VALUE (uint8_t)0xf9
#define MEMPOOL_POISON_VALUE (uint8_t)0xf8
#define ALIGN_POISON_VALUE (uint8_t)0xf7

#define _RET_IP_ ((uintptr_t)__builtin_return_address(0))

/*
 * Loop termination condition:
 * Following loop terminate after one execution, since the loop condition is while (0).
 * 0 is constant and reasonable: this is the functional macro definition, while (0) is reasonable.
 */
/* start~end must be within a REF_RANGE window */
#define inc_ref(start, end) \
	do { \
		struct spg_ref *_tmp; \
		uint8_t ref; \
		uintptr_t idx = ((start) - KASAN_ADDR_BASE) >> SPG_SHIFT; \
		_tmp = KASAN_SHADOW_PGREF + idx; \
		ref = kasan_range_to_ref(start, end); \
		_tmp->ref = _tmp->ref | ref; \
	} while (0)

/*
 * Loop termination condition:
 * Following loop terminate after one execution, since the loop condition is while (0).
 * 0 is constant and reasonable: this is the functional macro definition, while (0) is reasonable.
 */
/* start~end must be within a 32K window */
#define del_ref(start, end) \
	do { \
		uint8_t ref; \
		ref = kasan_range_to_ref(start, end); \
		struct spg_ref *_tmp = KASAN_SHADOW_PGREF + (uintptr_t)(((start) - KASAN_ADDR_BASE) >> SPG_SHIFT); \
		_tmp->ref &= (uint8_t)(~ref); \
		if (_tmp->ref == 0) { \
			(void)hm_mem_madvise(ptr_to_void(shadow_page(start)), __PAGE_SIZE, MADV_DONTNEED); \
		} \
	} while (0)

#define get_ref(start) \
	({ \
		struct spg_ref *_tmp; \
		uintptr_t idx = ((start) - KASAN_ADDR_BASE) >> SPG_SHIFT; \
		_tmp = KASAN_SHADOW_PGREF + idx; \
		_tmp->ref; \
	})


void kasan_poison_shadow(uintptr_t addr, size_t len, uint8_t value);
void kasan_unpoison_shadow(uintptr_t addr, size_t len);
void poison_mem_in_aligned_range(uintptr_t vaddr, size_t len);

#define NO_KASAN __attribute__((no_sanitize_address))

#define ASAN_GUARD_CHUNK_SIZE 32

#define KASAN_HEX_SCALE 16
#define KASAN_DEC_SCALE 10

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000		\
		     + __GNUC_MINOR__ * 100	\
		     + __GNUC_PATCHLEVEL__)
#endif

#ifdef __clang__
#define ASAN_ABI_VERSION 5
#elif defined(__GNUC__)
#if GCC_VERSION >= 70000
#define ASAN_ABI_VERSION 5
#elif GCC_VERSION >= 50000
#define ASAN_ABI_VERSION 4
#elif GCC_VERSION >= 40902
#define ASAN_ABI_VERSION 3
#endif
#endif

#ifndef ASAN_ABI_VERSION
#define ASAN_ABI_VERSION 1
#endif

/* kasan global struct layout is defined by gcc */
struct kasan_global {
	const void *beg;                /* the begin addr of the global variable */
	size_t size;                    /* the size of the global variable. */
	size_t size_with_redzone;       /* the size of the variable with red zone. 32 bytes aligned */
	const void *name;               /* name of the variable */
	const void *mod_name;           /* name of the module that define the global variable */
	unsigned long has_dyn_init;     /* Non-zero if the global has dynamic initializer only for C++ */
#if ASAN_ABI_VERSION >= 4
	struct kasan_source_location *loc; /* the location of the variable */
#endif
#if ASAN_ABI_VERSION >= 5
	char *odr_indtor;               /* the address of the ODR indicator symbol */
#endif
};

void kasan_remove_ref(uintptr_t start, uintptr_t end);
void __asan_poison_stack_memory(const void *addr, size_t len);
void __asan_unpoison_stack_memory(const void *addr, size_t len);
void __asan_handle_no_return(void);
void __asan_load1(unsigned long bgn);
void __asan_load2(unsigned long bgn);
void __asan_load4(unsigned long bgn);
void __asan_load8(unsigned long bgn);
void __asan_load16(unsigned long bgn);
void __asan_loadN(unsigned long bgn, size_t size);

void __asan_load1_noabort(unsigned long bgn);
void __asan_load2_noabort(unsigned long bgn);
void __asan_load4_noabort(unsigned long bgn);
void __asan_load8_noabort(unsigned long bgn);
void __asan_load16_noabort(unsigned long bgn);
void __asan_loadN_noabort(unsigned long bgn, size_t size);

void __asan_store1(unsigned long bgn);
void __asan_store2(unsigned long bgn);
void __asan_store4(unsigned long bgn);
void __asan_store8(unsigned long bgn);
void __asan_store16(unsigned long bgn);
void __asan_storeN(unsigned long bgn, size_t size);

void __asan_store1_noabort(unsigned long bgn);
void __asan_store2_noabort(unsigned long bgn);
void __asan_store4_noabort(unsigned long bgn);
void __asan_store8_noabort(unsigned long bgn);
void __asan_store16_noabort(unsigned long bgn);
void __asan_storeN_noabort(unsigned long bgn, size_t size);

void __asan_register_globals(struct kasan_global *glbls, size_t size);
void __asan_unregister_globals(__attribute__((unused)) struct kasan_global *glbls,
			       __attribute__((unused)) size_t size);

void __asan_report_load1_noabort(unsigned long bgn);
void __asan_report_store1_noabort(unsigned long bgn);
void __asan_report_load2_noabort(unsigned long bgn);
void __asan_report_store2_noabort(unsigned long bgn);
void __asan_report_load4_noabort(unsigned long bgn);
void __asan_report_store4_noabort(unsigned long bgn);
void __asan_report_load8_noabort(unsigned long bgn);
void __asan_report_store8_noabort(unsigned long bgn);
void __asan_report_load16_noabort(unsigned long bgn);
void __asan_report_store16_noabort(unsigned long bgn);
void __asan_report_load_n_noabort(unsigned long bgn, size_t size);
void __asan_report_store_n_noabort(unsigned long bgn, size_t size);

void __asan_alloca_poison(unsigned long bgn, size_t size);
void __asan_allocas_unpoison(unsigned long bgn, size_t size);

void check_memory_region(uintptr_t addr, size_t size, bool write, uintptr_t ret_addr);
void kasan_report(uintptr_t addr, size_t size, bool write, uintptr_t ret_addr);
bool kasan_memory_is_invalid(uintptr_t addr, size_t size);
bool kasan_address_in_shadow_range(uintptr_t addr);
unsigned long memory_is_zero(uintptr_t bgn, uintptr_t end);
#endif
