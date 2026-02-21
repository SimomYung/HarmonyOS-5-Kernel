/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 * Description: POSIX-compatible signal definitions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 22 10:31:39 2021
 */
#ifndef LIBHMSRV_SYS_HM_SIGNAL_DEFS_H
#define LIBHMSRV_SYS_HM_SIGNAL_DEFS_H

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SIGNAL_H
#error "hm_signal_defs.h should not be directly used, use hm_signal.h instead"
#endif

#ifndef _SIGNAL_H

/* Avoid clashes with libc's signal.h */
#define _SIGNAL_H

/* For necessary data types */
#define __NEED_size_t
#define __NEED_uintptr_t
#define __NEED_pid_t
#define __NEED_uid_t
#define __NEED_sigset_t
#define __NEED_struct_timespec
#define __NEED_time_t
#include <bits/alltypes.h>

typedef struct sigaltstack stack_t;

struct compat_sigaltstack {
	unsigned int ss_sp;
	int ss_flags;
	unsigned int ss_size;
};
typedef struct compat_sigaltstack compat_stack_t;

/* For signal number definitions */
#include <bits/signal.h>

/* Following are compatible signal-related definitions */
#define SIG_BLOCK   0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2

#define SI_TIMER (-2)
#define SI_TKILL (-6)

#define ILL_ILLOPC 1

#define SEGV_MAPERR 1
#define SEGV_ACCERR 2

#define BUS_ADRALN 1
#define BUS_OBJERR 3

#define CLD_EXITED    1
#define CLD_KILLED    2
#define CLD_DUMPED    3
#define CLD_STOPPED   5
#define CLD_CONTINUED 6

#define SS_ONSTACK 1
#define SS_DISABLE 2

#define SIG_ERR       ((void *)(uintptr_t)(-1))
#define SIG_DFL       ((void *)(uintptr_t)(0))
#define SIG_IGN       ((void *)(uintptr_t)(1))

union sigval {
	int sival_int;
	void *sival_ptr;
};

#define __SI_PAD_SIZE (128 - 2 * sizeof(int) - sizeof(long))

typedef struct {
	int si_signo;
	int si_errno;
	int si_code;
	union {
		char __pad[__SI_PAD_SIZE];
		struct {
			pid_t si_pid;
			uid_t si_uid;
			union {
				union sigval si_value;
				int si_status;
			};
		};
		void *si_addr;
		struct {
			void *si_call_addr;
			int si_syscall;
			unsigned int si_arch;
		};
	};
} siginfo_t;

#define __COMPAT_SI_PAD_SIZE (128 - 3 * sizeof(int))

typedef struct {
	int si_signo;
	int si_errno;
	int si_code;
	union {
		char __pad[__COMPAT_SI_PAD_SIZE];
		struct {
			pid_t si_pid;
			uid_t si_uid;
			union {
				unsigned int si_value;
				int si_status;
			};
		};
		unsigned int si_addr;
		struct {
			unsigned int si_call_addr;
			int si_syscall;
			unsigned int si_arch;
		};
	};
} compat_siginfo_t;

extern int sigdelset(sigset_t *set, int sig);
void (*signal(int, void (*)(int)))(int);

void hm_fill_posix_siginfo_compat(compat_siginfo_t *u, const struct __siginfo *k);

#endif /* _SIGNAL_H */

#endif /* LIBHMSRV_SYS_HM_SIGNAL_DEFS_H */
