/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Unmap thread stack self
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 12 16:20:27 2022
 */

#include <hongmeng/syscall.h>
#include <libhmsrv_sys/hm_mem.h>
#include <hmasm/lsyscall.h>

#include "raw_tls.h"
#include "raw_thread_unmapself.h"

static const void *unmap_base;
static size_t unmap_size;

/* 2048: Used to replace stack when detached raw_thread exits */
static char shared_stack[2048];

static void __noreturn __unmapself(void)
{
	int err;

	err = (int)lsyscall_syscall2(__NR_munmap, (long)(uintptr_t)unmap_base, (long)unmap_size);
	if (err != E_HM_OK) {
		hm_warn("hm_mem_munmap map_base failed %s\n",
			hmstrerror(err));
	}
	for (;;) {
		(void)lsyscall_syscall1(__NR_exit, 0);
	}
}

void raw_thread_unmapself(void *base, size_t size)
{
	char *stack;

	stack = shared_stack + sizeof(shared_stack);
	stack -= (uintptr_t)stack % 16; /* 16-byte alignment */
	unmap_base = base;
	unmap_size = size;
	crt_jmp(__unmapself, stack);
}

