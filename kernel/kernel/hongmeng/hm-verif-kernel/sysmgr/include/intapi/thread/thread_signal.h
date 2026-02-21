/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header file of signal
 * Author: Huawei OS Kernel Lab
 * Create: Thur Oct 12 11:43:34 2023
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_SIGNAL_H
#define H_SYSMGR_INCLUDE_THREAD_SIGNAL_H

#include <intapi/process/process.h>
#include <intapi/signal/signal.h>
#include <intapi/thread/thread.h>
#include <hmkernel/ipc/signal.h>

static inline void thread_set_pdeath_signal(struct thread_s *thread, unsigned int signum)
{
	thread->pdeath_signal = (int)signum;
}

static inline int thread_pdeath_signal_of(struct thread_s *thread)
{
	return thread->pdeath_signal;
}

int thread_hmsignal(const struct thread_s *thread, unsigned int signum);

#ifdef CONFIG_FREEZER_USER_HANDLER
static inline void thread_exit_user_handler(struct thread_s *thread)
{
	struct process_s *process = thread_process_of(thread);
	raw_mutex_lock(&thread->mutex);
	if (thread->flags & THREAD_REIN_USERHANDLER) {
		thread->flags &= ~THREAD_IN_USERHANDLER;
		thread->flags &= ~THREAD_REIN_USERHANDLER;
	} else if (thread->flags & THREAD_IN_USERHANDLER) {
		thread->flags &= ~THREAD_IN_USERHANDLER;
		process->ext_flags &= ~PROCESS_IN_USERHANDLER;
		process->ext_flags &= ~PROCESS_REIN_USERHANDLER_FLAG;
		(void)signalmgr_ops_deliver_hmsignal(thread, true, HMSIGNAL_FREEZE);
	}
	raw_mutex_unlock(&thread->mutex);
}

static inline void thread_reset_userhandler_flag(struct thread_s *thread)
{
	struct process_s *process = thread_process_of(thread);
	if ((process->ext_flags & PROCESS_IN_USERHANDLER_FLAG) != 0) {
		process->ext_flags &= ~PROCESS_IN_USERHANDLER_FLAG;
	}
}
#else
static inline void thread_exit_user_handler(const struct thread_s *thread)
{}

static inline void thread_reset_userhandler_flag(const struct thread_s *thread)
{}
#endif

#ifdef CONFIG_FREEZER
int freeze_handle_hmsignal(struct thread_s *thread);
#else
static inline int freeze_handle_hmsignal(struct thread_s *thread)
{
	UNUSED(thread);
	return E_HM_NOSYS;
}
#endif

/*
 * Caller should hold process->signal_lock before calling the following
 * signal-related functions with underscore prefix.
 */
int __thread_fill_siginfo_and_sigpending(struct thread_s *thread, const struct __siginfo *siginfo);

uint64_t thread_signal_mask_of(const struct thread_s *thread);

int __thread_set_signal_mask(struct thread_s *thread, uint64_t mask, bool has_thread_alive);

int thread_set_signal_mask(struct thread_s *thread, uint64_t mask, bool has_thread_alive);
bool __thread_is_signum_masked(const struct thread_s *thread, unsigned int signum);

void __thread_clr_signum_pending(struct thread_s *thread, unsigned int signum);
bool __thread_is_signum_pending(const struct thread_s *thread, unsigned int signum);
void thread_sigtimedwait_begin(struct thread_s *thread, uint64_t sigset);
void thread_sigtimedwait_end(struct thread_s *thread);
bool __thread_is_signum_timedwait_masked(const struct thread_s *thread, unsigned int signum);
bool thread_is_signum_timedwait_masked(const struct thread_s *thread, unsigned int signum);
unsigned int __thread_sigpending_of(const struct thread_s *thread);
void handle_thread_exit(pid_t tid_nr);
void thread_handle_hmsignal_kill(struct thread_s *thread);
#endif
