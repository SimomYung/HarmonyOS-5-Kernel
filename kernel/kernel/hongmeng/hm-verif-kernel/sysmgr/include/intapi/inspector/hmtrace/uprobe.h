/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Frontend operations of uprobe
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 7 15:50:16 2020
 */

#ifndef SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_H
#define SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_H

#include <stdint.h>
#include <unistd.h>

#include <intapi/process/process.h>
#include <intapi/thread/thread.h>
#include <hmasm/ucontext.h>

#define UPROBE_ARGS_CNT 4

struct uprobe_args {
	void *arg[UPROBE_ARGS_CNT];
};

struct uprobe_context {
	uintptr_t vaddr;
	struct arch_uctx *uctx;
	struct thread_s *thread;
	struct process_s *process;
	uint32_t sequence; // the times of this uprobe repeat in the same thread
	struct uprobe_args args;
};

enum uprobe_handler_ret {
	UPROBE_HANDLER_OK,
	UPROBE_HANDLER_BREAK,
};
typedef int (*uprobe_handler_t)(struct uprobe_context *, void *);
struct uprobe_handlers {
	uprobe_handler_t pre_handler;
	void *pre_handler_args;
	uprobe_handler_t ret_handler;
	void *ret_handler_args;
};

enum uprobe_handler_type {
	UPROBE_PRE_HANDLER,
	UPROBE_RET_HANDLER,
};

struct uprobe_trace_event;

struct uprobe {
	pid_t pid;
	uintptr_t vaddr;
	uint32_t origin_insn;
	uintptr_t insn_exec_vaddr;
	int insn_exec_type;
	enum process_model model;
	struct raw_refcnt refcnt;
	struct dlist_node node;
	struct dlist_node event_node;
	struct uprobe_trace_event *event;
};

#define __UPROBE_NOPROBE  __attribute__((section("__uprobe_noprobe_list")))

struct uprobe *create_uprobe(struct process_s *proc, struct vspace_s *vs, uintptr_t vaddr);
void destroy_uprobe(struct uprobe *uprobe);
#ifdef CONFIG_UPROBE
int uprobe_procmgr_fork(struct process_s *process, struct process_s *process_new);
void process_destroy_uprobe(struct process_s *process);
#else
static inline int uprobe_procmgr_fork(struct process_s *process, struct process_s *process_new)
{
	UNUSED(process, process_new);
	return E_HM_OK;
}
static inline void process_destroy_uprobe(struct process_s *process)
{
	UNUSED(process);
}
#endif

#endif
