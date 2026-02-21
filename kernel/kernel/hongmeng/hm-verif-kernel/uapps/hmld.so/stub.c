/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Stub functions in hmld
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 11:24:58 2019
 */

#include <limits.h>
#include <signal.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libstrict/strict.h>
#include <hmasm/stackprotector.h>

unsigned long __stack_chk_guard;

__attribute__((visibility("hidden")))
void raw_mutex_lock(struct raw_mutex *mutex)
{
	UNUSED(mutex);
}

__attribute__((visibility("hidden")))
void raw_mutex_unlock(struct raw_mutex *mutex)
{
	UNUSED(mutex);
}

__attribute__((visibility("hidden")))
int raise(int sig)
{
	hm_panic("don't support raise: %d in ldso\n", sig);
	return 0;
}

void __stack_chk_fail(void)
{
	hm_panic("stack check failed\n");
}

/*
 * Below stub functions for C++ exceptions should not be eliminated
 * by LTO even we are pure C code.
 */
void __aeabi_unwind_cpp_pr0(void);
void __used __aeabi_unwind_cpp_pr0(void)
{
}

void __aeabi_unwind_cpp_pr1(void);
void __used __aeabi_unwind_cpp_pr1(void)
{
}

void __aeabi_unwind_cpp_pr2(void);
void __used __aeabi_unwind_cpp_pr2(void)
{
}
