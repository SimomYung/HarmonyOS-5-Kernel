/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Uprobe excecption operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 7 15:50:16 2020
 */

#ifndef SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_EXCEPTION_H
#define SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_EXCEPTION_H

#include <stdbool.h>

#include <hongmeng/errno.h>

struct thread_s;
struct process_s;
struct __arch_exception_info;

typedef int (*uprobe_hook_fn)(struct process_s *process, struct thread_s *thread, struct arch_uctx *uctx);
struct uprobe_hook_s {
	uint32_t imm;
	uprobe_hook_fn hook_fn;
};

#define uprobe_register_hook(__imm, __hook_fn) \
static struct uprobe_hook_s uprobe_hook_##__hook_fn  __used \
	__attribute__((section("uprobe_hook_funcs"))) = { \
	.imm = (__imm), \
	.hook_fn = (__hook_fn), \
}

#ifdef CONFIG_UPROBE
int uprobe_exception(struct thread_s *thread, struct process_s *process,
		     const struct __arch_exception_info *info);
const struct uprobe_hook_s *uprobe_find_hook(uint32_t imm);
#else
static inline int uprobe_exception(struct thread_s *thread,
				   struct process_s *process,
				   struct __arch_exception_info *info)
{
	return E_HM_NOSYS;
}
static inline const struct uprobe_hook_s *uprobe_find_hook(uint32_t imm)
{
	return NULL;
}
#endif

#endif
