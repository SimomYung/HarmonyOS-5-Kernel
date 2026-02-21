/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Tsan libc hook function
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 17 15:10:47 2022
 */
#include <stddef.h>
#include <stdbool.h>
#include "tsanhook.h"

/*
 * some libc code will be executed before ldso loaded, so libc
 * cannot directly call libtsan functions. We create wrap tsan
 * functions in libc, which will be set to origin libtsan via
 * a callback when libtsan is ready
 */

static void dummy0(void)
{
}

static void dummy1(unsigned long addr)
{
	(void)addr;
}

static void dummy2_1(unsigned long addr, unsigned long pc)
{
	(void)addr;
	(void)pc;
}

static void dummy2_2(unsigned long addr, unsigned long size)
{
	(void)addr;
	(void)size;
}

#define tsan_loadstore_assign(size)		\
	.__tsan_read##size = dummy2_1,		\
	.__tsan_unaligned_read##size = dummy2_1,	\
	.__tsan_read##size##_pc = dummy2_1,	\
	.__tsan_write##size = dummy2_1,		\
	.__tsan_unaligned_write##size = dummy2_1,	\
	.__tsan_write##size##_pc = dummy2_1,

static struct tsan_callback g_tsan_callback = {
	tsan_loadstore_assign(1)
	tsan_loadstore_assign(2)
	tsan_loadstore_assign(4)
	tsan_loadstore_assign(8)
	tsan_loadstore_assign(16)
	.__tsan_read_range = dummy2_2,
	.__tsan_write_range = dummy2_2,
	.__tsan_func_entry = dummy1,
	.__tsan_func_exit = dummy0,
	.__tsan_init = dummy0,
};

#define weak __attribute__((__weak__))

static bool tsan_enable = false;

#define _RET_IP_ ((unsigned long)__builtin_return_address(0))

#define def_mem_access_check(size)			\
weak void __tsan_read##size(unsigned long addr)		\
{							\
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_read##size(addr, _RET_IP_);	\
	}						\
}							\
weak void __tsan_unaligned_read##size(unsigned long addr)	\
{							\
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_unaligned_read##size(addr, _RET_IP_);	\
	}						\
}							\
weak void __tsan_read##size##_pc(unsigned long addr, unsigned long pc)	\
{							\
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_read##size##_pc(addr, pc);	\
	}						\
}							\
weak void __tsan_write##size(unsigned long addr)		\
{							\
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_write##size(addr, _RET_IP_);	\
	}						\
}							\
weak void __tsan_unaligned_write##size(unsigned long addr)	\
{							\
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_unaligned_write##size(addr, _RET_IP_);	\
	}						\
}							\
weak void __tsan_write##size##_pc(unsigned long addr, unsigned long pc)	\
{							\
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_write##size##_pc(addr, pc);	\
	}						\
}

def_mem_access_check(1)
def_mem_access_check(2)
def_mem_access_check(4)
def_mem_access_check(8)
def_mem_access_check(16)

weak void __tsan_read_range(unsigned long addr, unsigned long size)
{
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_read_range(addr, size);
	}
}

weak void __tsan_write_range(unsigned long addr, unsigned long size)
{
	if (tsan_enable) {				\
		g_tsan_callback.__tsan_write_range(addr, size);
	}
}

weak void __tsan_init(void)
{
	return;
}
weak void __tsan_func_entry(unsigned long pc)
{
	(void)pc;
	return;
}
weak void __tsan_func_exit(void)
{
	return;
}

void set_tsan_callback(const struct tsan_callback *new_ac)
{
	if (new_ac != NULL) {
		/* Avoid using memcpy (gcc optimization) */
		unsigned int i = 0;
		for (; i < sizeof(struct tsan_callback); i++) {
			*((char *)&g_tsan_callback + i) = *((char *)new_ac + i);
		}
		tsan_enable = true;
	}
}
