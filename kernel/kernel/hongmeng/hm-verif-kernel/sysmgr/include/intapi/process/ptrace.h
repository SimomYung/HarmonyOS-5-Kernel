/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition for ptrace
 * Author: Huawei OS Kernel Lab
 * Create: Tues July 18 11:42:01 2023
 */
#ifndef SYSMGR_INTAPI_PROCESS_PTRACE_H
#define SYSMGR_INTAPI_PROCESS_PTRACE_H

#include <stdint.h>
#include <sys/ptrace.h>
#include <hmasm/registers.h>
#include <intapi/process/process.h>
#include <hmkernel/debug.h>

#define NORMAL_REGS_MAX 30
#define NORMAL_REGS_MAX_COMP 13

struct ptrace_arg_s {
	int request;
	pid_t pid;
	void *addr;
	void *data;
};

struct compat_iovec {
	__u32 iov_base;
	__u32 iov_len;
};

struct user_hwdebug_state {
	__u32 dbg_info;
	__u32 pad;
	struct {
		__u64 addr;
		__u32 ctrl;
		__u32 pad;
	} dbg_regs[16];
};

#define PTRACE_MODE_READ	0x01
#define PTRACE_MODE_ATTACH	0x02
#define PTRACE_MODE_FSCREDS	0x08

#define PTRACE_MODE_READ_FSCREDS (PTRACE_MODE_READ | PTRACE_MODE_FSCREDS)
#define PTRACE_MODE_ATTACH_FSCREDS (PTRACE_MODE_ATTACH | PTRACE_MODE_FSCREDS)

#ifdef CONFIG_PTRACE
int ptrace_traceme(struct thread_s *thread);
int ptrace_handle_request(struct thread_s *current_thread, struct ptrace_arg_s ptrace_arg);
void ptrace_syscall_trace(struct thread_s *thread, bool is_enter);
#else
static inline int ptrace_traceme(struct thread_s *thread)
{
	UNUSED(thread);
	return E_HM_NOSYS;
}

static inline int ptrace_handle_request(struct thread_s *current_thread, struct ptrace_arg_s ptrace_arg)
{
	UNUSED(current_thread, ptrace_arg);
	return E_HM_NOSYS;
}

static inline void ptrace_syscall_trace(struct thread_s *thread, bool is_enter)
{
	UNUSED(thread, is_enter);
}
#endif

#endif
