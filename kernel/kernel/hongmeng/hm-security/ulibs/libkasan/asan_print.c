/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Output asan log and backtrace handling
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 03 20:17:18 2022
 */
// libc
#include <execinfo.h>

// hongmeng
#include <vfs.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/atomic.h>

// kasan
#include <libkasan/mutex.h>
#include <libkasan/kasan_init.h>

#include "macro.h"
#include "shadow.h"
#include "asan_env.h"
#include "asan_test.h"
#include "asan_print.h"
#include "utils.h"

#define DEFAULT_SYM_LEN 256
#define DEFAULT_CALL_STACK_LEVEL 128

static kasan_mutex_t print_mutex;

#ifdef KASAN_SHARED
void kasan_set_inited(bool val);

void asan_print_with_len(const char *output, size_t len)
{
	if (output == NULL) {
		hm_debug("output info is NULL\n");
		return;
	}

	if (asan_env_gs()->log_file_fd != -1) {
		int ret = (int)(ssize_t)vfs_write(asan_env_gs()->log_file_fd, output, strnlen(output, len));
		if (ret < 0) {
			hm_error("write asan log to file failed, ret is %s\n", hmstrerror(ret));
		}
	} else {
		hm_error("%s", output);
	}
}

static bool asan_need_trap(void)
{
	return (asan_env_gs()->options[HALT_ON_ERROR] == NULL || asan_env_gs()->options[HALT_ON_ERROR]->str[0] == '1');
}

static void asan_backtrace(void)
{
	void *frame_buf[DEFAULT_CALL_STACK_LEVEL] = {0};
	char sym_buf[DEFAULT_SYM_LEN] = {0};

	int nptrs = backtrace(frame_buf, DEFAULT_CALL_STACK_LEVEL);
	char **res = backtrace_symbols(frame_buf, nptrs);
	if (res == NULL) {
		asan_print("Failed to get function symbol !\n");
	}

	for (int i = 0; res != NULL && i < nptrs; i++) {
		int ret = snprintf_s(sym_buf, DEFAULT_SYM_LEN, DEFAULT_SYM_LEN - 1, "%s\n", res[i]);
		if (ret < 0) {
			asan_print("Failed to generate symbol buffer !\n");
		}
		asan_print_with_len(sym_buf, DEFAULT_SYM_LEN);
	}
}

void asan_set_trap_enabled(bool val)
{
	(void)val;
}

unsigned int asan_report_num(void)
{
	return 0;
}
#else
void asan_print_with_len(const char *output, size_t len)
{
	(void)len;
	if (output == NULL) {
		hm_debug("output info is NULL\n");
		return;
	}
	hm_error("%s", output);
}

static bool asan_trap_enabled = true;
static raw_atomic_int_t asan_trace_num = {0};

static bool asan_need_trap(void)
{
	return asan_trap_enabled;
}

static void asan_backtrace(void)
{
	(void)raw_atomic_int_inc(&asan_trace_num);
}

void asan_set_trap_enabled(bool val)
{
	if (!val) {
		raw_atomic_int_set(&asan_trace_num, 0);
	}
	asan_trap_enabled = val;
}

unsigned int asan_report_num(void)
{
	return (unsigned int)raw_atomic_int_read(&asan_trace_num);
}
#endif

static void asan_trap(void)
{
	asan_env_fini();
#ifndef __HOST_LLT__
	hm_panic("kasan detected poisoned memory accessed\n");
#endif
	__builtin_trap();
}

void asan_handle_error(void)
{
	if (asan_need_trap()) {
		asan_trap();
	} else {
		asan_backtrace();
	}
}

static inline void common_report_start(void)
{
	kasan_mutex_lock(&print_mutex);
	/* Temporary disabling of the kasan check.
	 * Because some kasan check is in libc and vfs, this operation is to
	 * prevent log processing from triggering the kasan check.
	 * In multi-thread mode, to prevent other threads from failing to perform
	 * kasan check due to disabling the global switch, set kasan_inited to a
	 * TLS variable.
	 */
	kasan_set_inited(false);
}

static inline void common_report_end(void)
{
	/* Enable kasan check */
	kasan_set_inited(true);
	kasan_mutex_unlock(&print_mutex);
}

#define BUF_SIZE 64
#define DUMP_SHADOW_RANGE 32
static void shadow_range_find(uintptr_t addr, size_t size, uintptr_t *pstart, uintptr_t *pend, uintptr_t *paddr)
{
	uintptr_t start = 0UL;
	uintptr_t align_poison = 0UL;
	uintptr_t end = 0UL;
	uintptr_t poison_addr = kasan_mem_to_shadow(addr);
	if (size > KASAN_SHADOW_SCALE_SIZE) {
		poison_addr = memory_is_zero(poison_addr, kasan_mem_to_shadow(addr + size - 1) + 1);
		if (poison_addr == 0U) {
			goto out;
		}
	}
	align_poison = ALIGN_DOWN(poison_addr, KASAN_SHADOW_SCALE_SIZE);
	start = max((align_poison - DUMP_SHADOW_RANGE), ALIGN_DOWN(align_poison, __PAGE_SIZE));
	end = min((align_poison + DUMP_SHADOW_RANGE), ALIGN_UP(align_poison, __PAGE_SIZE));
out:
	*pstart = start;
	*pend = end;
	*paddr = poison_addr;
}

static void asan_print_shadow(uintptr_t addr, size_t size)
{
	uintptr_t start;
	uintptr_t end;
	uintptr_t poison_addr;
	char buf[BUF_SIZE] = {0};
	size_t buf_len = 0UL;
	int ret = 0;

	if (kasan_address_in_shadow_range(addr)) {
		hm_error("access shadow range\n");
		return;
	}
	shadow_range_find(addr, size, &start, &end, &poison_addr);
	if (start == 0UL || end == 0UL || start > end) {
		return;
	}
	for (; start < end; start++) {
		if (IS_ALIGNED(start, KASAN_SHADOW_SCALE_SIZE)) {
			ret = snprintf_s(buf, BUF_SIZE, BUF_SIZE - 1, " %lx: ", start);
			if (ret < 0) {
				hm_error("failed to sprintf\n");
				return;
			}
			buf_len = ret;
		}
		if (start == poison_addr) {
			ret = snprintf_s(buf + buf_len, BUF_SIZE - buf_len, BUF_SIZE - buf_len - 1,
				       "[%2x]", *(uint8_t *)start);
		} else {
			ret = snprintf_s(buf + buf_len, BUF_SIZE - buf_len, BUF_SIZE - buf_len - 1,
				       " %2x ", *(uint8_t *)start);
		}
		if (ret < 0) {
			hm_error("failed to sprintf\n");
			return;
		}
		buf_len += ret;
		if ((start % KASAN_SHADOW_SCALE_SIZE) == KASAN_SHADOW_MASK) {
			hm_error("%s\n", buf);
		}
	}
}

static kasan_dumpstack_func_t kasan_dumpstack_func = NULL;
void asan_report_address(uintptr_t addr, size_t size, bool write, uintptr_t ret_addr)
{
	common_report_start();

	uint8_t magic_num_value = DEFAULT_POISON_VALUE;
	if (addr < KASAN_SHADOW_BEG) {
		uint8_t *magic_num = (uint8_t *)kasan_mem_to_shadow(addr);
		magic_num_value = *magic_num;
	}
	char output_buf[DEFAULT_OUTPUT_BUF_SIZE];
	int ret = snprintf_s(output_buf, DEFAULT_OUTPUT_BUF_SIZE, DEFAULT_OUTPUT_BUF_SIZE - 1,
			"===========kasan detected poisoned memory accessed. (addr=%p, "
			"size=0x%lx, write=%d, ret_addr=%p, magic_num=%x)==========\n",
			(void *)addr, (unsigned long)size, write, (void *)ret_addr, magic_num_value);
	if (ret < 0) {
		hm_error("snprintf address info failed\n");
		goto exit;
	}
	asan_print_with_len(output_buf, DEFAULT_OUTPUT_BUF_SIZE);
	if (kasan_dumpstack_func) {
		kasan_dumpstack_func(addr);
	}
	asan_print_shadow(addr, size);

	asan_handle_error();
exit:
	common_report_end();
}

void asan_report_alloc_mismatch(const char *free_type_name, const char *real_type_name)
{
	common_report_start();

	char output_buf[DEFAULT_OUTPUT_BUF_SIZE] = {0};
	int ret = snprintf_s(output_buf, DEFAULT_OUTPUT_BUF_SIZE, DEFAULT_OUTPUT_BUF_SIZE - 1,
			"===========kasan detected alloc and free are mismatched (free type = %s, "
			"real type = %s)==========\n", free_type_name, real_type_name);
	if (ret < 0) {
		hm_error("snprintf alloc mismatch info failed\n");
		goto exit;
	}
	asan_print_with_len(output_buf, DEFAULT_OUTPUT_BUF_SIZE);

	asan_handle_error();
exit:
	common_report_end();
}

void kasan_dumpstack_func_set(kasan_dumpstack_func_t func)
{
	kasan_dumpstack_func = func;
}
