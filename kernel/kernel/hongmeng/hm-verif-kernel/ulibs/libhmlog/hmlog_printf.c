/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Implementations of __hmlog_printf api
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 12 16:43:55 2024
 */
#include <libhmlog/hmlog.h>
#include <libhmlog/hmlog_stkpool.h>
#include <stdio.h>
#include <stdarg.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_thread.h>

#ifdef CONFIG_HMLOG_STKCHK

/*
 * 1. For kiringrp, __hmlog_vprintf, which is invoked by __hmlog_printf,
 *    uses 2464 bytes stack. Here we privide a larger guard size for check.
 * 2. Log will be printed by switching to new stack, which is allocated
 *    from the g_hmlog_stkpool, if available stack size is smaller than
 *    HMLOG_STKCHK_GUARD_SIZE.
 * 3. g_hmlog_stkpool has HMLOG_STKPOOL_STACK_COUNT stack. Log will be printed
 *    still by the original stack (cause data fault) if all stack in stkpool
 *    have been allocated.
 */
#define HMLOG_STKCHK_GUARD_SIZE 2560U

#ifndef __HOST_LLT__
static bool g_hmlog_stk_chk = false;

void hmlog_enable_stk_chk(void)
{
	g_hmlog_stk_chk = true;
}

void hmlog_disable_stk_chk(void)
{
	g_hmlog_stk_chk = false;
}

bool hmlog_stk_chk_of(void)
{
	return g_hmlog_stk_chk;
}

static inline __always_inline bool __hmlog_stack_is_ok(void)
{
	raw_thread_t self = NULL;
	unsigned int remain_size = 0U;
	register unsigned long curr_sp;
	unsigned long stack_base = 0UL;
	unsigned long stack_size = 0UL;

	if (!g_hmlog_stk_chk) {
		return true;
	}

	self = raw_thread_self();
	stack_base = raw_thread_stack_base_of(self);
	stack_size = raw_thread_actv_stack_size_of(self);
	asm volatile ("mov %0, sp" : "=r" (curr_sp));
	remain_size = curr_sp - (stack_base - stack_size);
	return (remain_size > HMLOG_STKCHK_GUARD_SIZE);
}
#else
void hmlog_enable_stk_chk(void) {}

void hmlog_disable_stk_chk(void) {}

bool hmlog_stk_chk_of(void)
{
	return false;
}

static inline __always_inline bool __hmlog_stack_is_ok(void)
{
	return true;
}
#endif

static struct __hmlog_stkpool *g_hmlog_stkpool = NULL;

void hmlog_stkpool_set(struct __hmlog_stkpool *stkpool)
{
	g_hmlog_stkpool = stkpool;
}

static unsigned int hmlog_stack_to_bitmap_id(void *hmlog_stk)
{
	struct __hmlog_stk_meta *meta = (struct __hmlog_stk_meta *)hmlog_stk;
	return meta->bitmap_id;
}

static void __hmlog_vprintf_stack_free(void *hmlog_stk)
{
	unsigned int bitmap_id = hmlog_stack_to_bitmap_id(hmlog_stk);

	raw_mutex_lock(&g_hmlog_stkpool->mutex);
	if (raw_bitmap_clear_bit(g_hmlog_stkpool->stkmap, HMLOG_STKPOOL_STACK_COUNT, bitmap_id) < 0) {
		/* do nothing since this function is invoked by hmlog api */
	}
	raw_mutex_unlock(&g_hmlog_stkpool->mutex);
}

static void *bitmap_id_to_hmlog_stack(unsigned int bitmap_id)
{
	return g_hmlog_stkpool->stkpool[bitmap_id];
}

static void *__hmlog_vprintf_stack_alloc(void)
{
	void *stack = NULL;
	unsigned int bitmap_id;

	if (g_hmlog_stkpool == NULL) {
		return NULL;
	}

	raw_mutex_lock(&g_hmlog_stkpool->mutex);
	bitmap_id = raw_bitmap_find_next_zero(g_hmlog_stkpool->stkmap, HMLOG_STKPOOL_STACK_COUNT, g_hmlog_stkpool->last);
	if (bitmap_id == HMLOG_STKPOOL_STACK_COUNT) {
		bitmap_id = raw_bitmap_find_next_zero(g_hmlog_stkpool->stkmap, HMLOG_STKPOOL_STACK_COUNT, 0);
	}

	if (bitmap_id != HMLOG_STKPOOL_STACK_COUNT) {
		int err = raw_bitmap_set_bit(g_hmlog_stkpool->stkmap, HMLOG_STKPOOL_STACK_COUNT, bitmap_id);
		BUG_ON(err != E_HM_OK);

		g_hmlog_stkpool->last = bitmap_id;
		stack = bitmap_id_to_hmlog_stack(bitmap_id);
		/* bitmap_id should be set while init the stkpool */
		struct __hmlog_stk_meta *meta = (struct __hmlog_stk_meta *)stack;
		meta->bitmap_id = bitmap_id;
	}
	raw_mutex_unlock(&g_hmlog_stkpool->mutex);

	return stack;
}

#include <hmasm/hmlog_switch_stack.h>

void __attribute__((format(printf, 4, 0)))
__hmlog_vprintf_stkchk(enum hmlog_level level, const char *func,
		       int line, const char *fmt, va_list args)
{
	if (__hmlog_stack_is_ok()) {
		__hmlog_vprintf(level, func, line, fmt, args);
	} else {
		void *stack = __hmlog_vprintf_stack_alloc();
		if (stack != NULL) {
			arch_hmlog_vprintf_switch_stack(level, func, line, fmt, args, stack, (void *)__hmlog_vprintf);
			__hmlog_vprintf_stack_free(stack);
		} else {
			__hmlog_vprintf(level, func, line, fmt, args);
		}
	}
}

#else

void hmlog_enable_stk_chk(void) {}

void hmlog_disable_stk_chk(void) {}

bool hmlog_stk_chk_of(void)
{
	return false;
}

void hmlog_stkpool_set(struct __hmlog_stkpool *stkpool)
{
	UNUSED(stkpool);
}

void __attribute__((format(printf, 4, 0)))
__hmlog_vprintf_stkchk(enum hmlog_level level, const char *func,
		       int line, const char *fmt, va_list args)
{
	__hmlog_vprintf(level, func, line, fmt, args);
}

#endif

void __attribute__((format(printf, 5, 6)))
__hmlog_printf(enum hmlog_level level, const char *file, const char *func,
	       int line, const char *fmt, ...)
{
	UNUSED(file);
	va_list args;

	va_start(args, fmt);
	__hmlog_vprintf_stkchk(level, func, line, fmt, args);
	va_end(args);
}
