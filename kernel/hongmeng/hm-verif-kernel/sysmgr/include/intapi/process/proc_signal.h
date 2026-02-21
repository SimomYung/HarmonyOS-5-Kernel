/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Process signal
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 18 11:19:27 2020
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROC_SIGNAL_H
#define SYSMGR_INTAPI_PROCESS_PROC_SIGNAL_H

#include <intapi/process/process.h>

/*
 * Caller should hold process->signal_lock before calling the following
 * signal-related functions with underscore prefix.
 */
void process_sigpool_init(struct process_s *p);
struct rt_siginfo_s *__process_sigpool_alloc(struct process_s *process, unsigned int signum);
void __process_signal_free(struct process_s *process, struct rt_siginfo_s *rt_siginfo);

int __process_shared_pending_insert(struct process_s *process,
				    unsigned int signum,
				    struct __siginfo *siginfo);
void process_shared_pending_destroy(struct process_s *process);
void __process_shared_pending_destroy(struct process_s *process);
void __thread_pending_destroy(struct thread_s *thread);
int process_timer_siginfo_of(struct process_s *process, cref_t thread_cref,
			     cref_t timer_cref, struct __siginfo *psiginfo);
struct posix_timer_s *process_timer_of(struct process_s *process, cref_t thread_cref, cref_t timer_cref);

static inline int
__process_test_signal_handler(const struct process_s *process, unsigned int signum)
{
	return (signum > 0 && signum < _NSIG &&
		process->signal->actions[__SIGIDX_OF(signum)].handler > HM_SIG_IGN);
}

static inline int
process_test_signal_handler(struct process_s *process, unsigned int signum)
{
	int ret;

	raw_mutex_lock(&process->signal_lock);
	ret = __process_test_signal_handler(process, signum);
	raw_mutex_unlock(&process->signal_lock);

	return ret;
}

static inline bool
__process_test_signal_ignore(const struct process_s *process, unsigned int signum)
{
	return (signum > 0 && signum < _NSIG &&
		process->signal->actions[__SIGIDX_OF(signum)].handler == HM_SIG_IGN);
}

static inline bool
process_test_signal_ignore(struct process_s *process, unsigned int signum)
{
	bool ret = false;

	raw_mutex_lock(&process->signal_lock);
	ret = __process_test_signal_ignore(process, signum);
	raw_mutex_unlock(&process->signal_lock);

	return ret;
}

/*
 * The following interfaces that use signum will check the signum legality
 * before being called, so it can be guaranteed that signum > 0.
 */
static inline unsigned long
__process_signal_sighdlr_entry_of(struct process_s *process)
{
	return process->signal->handler_entry;
}

static inline uint64_t
__process_signal_mask_of(struct process_s *process, unsigned int signum)
{
	struct int_sigaction *act = &process->signal->actions[__SIGIDX_OF(signum)];
	uint64_t mask = act->mask;
	if ((act->flags & SA_NODEFER) == 0) {
		mask |= SIGSET_BIT(signum);
	}
	return mask;
}

static inline bool
__process_is_signal_onstack(const struct process_s *process, unsigned int signum)
{
	return (process->signal->actions[__SIGIDX_OF(signum)].flags & SA_ONSTACK) != 0;
}

int load_siginfo(struct process_s *p, siginfo_t *uaddr, struct __siginfo *info);

#endif
