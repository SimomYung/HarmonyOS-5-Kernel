/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 14:47:19 2019
 */
#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "pthread_impl.h"

int pthread_sigqueue(pthread_t t, int sig, const union sigval value)
{
	siginfo_t si;
	sigset_t set;
	int ret;

	if (!is_pthread_valid(t)) {
		ret = ESRCH;
	} else {
		(void)memset(&si, 0, sizeof(siginfo_t));
		si.si_signo = sig;
		si.si_code = SI_QUEUE;
		si.si_value = value;
		si.si_uid = getuid();
		si.si_pid = getpid();

		__block_app_sigs(&set);
		ret = -(int)__syscall(SYS_rt_tgsigqueueinfo, si.si_pid, t->tid,
				      sig, (long)(uintptr_t)&si);
		__restore_sigs(&set);
	}

	return ret;
}

