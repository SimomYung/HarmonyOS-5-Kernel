/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header file of signal
 * Author: Huawei OS Kernel Lab
 * Create: Thur Oct 12 11:43:34 2023
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_SIGNAL_H
#define H_SYSMGR_INCLUDE_THREAD_SIGNAL_H

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
