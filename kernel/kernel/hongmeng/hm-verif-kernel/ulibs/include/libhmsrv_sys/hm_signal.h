/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2024. All rights reserved.
 * Description: Definition of sysif server signal
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 15:02:00 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SIGNAL_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SIGNAL_H

#include <stdint.h>
#include <stdbool.h>
#include <hmasm/types.h>
#include <hmasm/compat.h>
#include <hmasm/ucontext.h>
#include <hmasm/sigcontext.h>
#include <libhmlog/hmlog.h>
#include <hmkernel/capability.h>
#include <hmkernel/ipc/signal.h>
#include <libhmsrv_sys/hm_signal_abi.h>
#include <libhmsrv_sys/hm_signal_defs.h>
#ifndef __KERNEL__
/* VDSO can not include this header */
#include <libstrict/strict.h>
#endif

/* We only support 65 POSIX signals */
#ifdef _NSIG
#undef _NSIG
#define _NSIG    65U
#endif

#define KSA_MASK_SIZE 2

#define SIGSET_SIZE   (size_t)(_NSIG / 8UL)
#define COMPAT_MINSIGSTKSZ 2048

#define hm_sigmask(signum) (1ULL << ((unsigned int)(signum) - 1U))
#define hm_siginmask(signum, mask) \
	(((signum) > 0) && __is_st_signum_valid(signum) && ((hm_sigmask(signum) & (mask)) != 0))

/*
 * See table in:
 * pubs.opengroup.org/onlinepubs/009604599/basedefs/signal.h.html
 */
#define SIG_COREDUMP_MASK  (\
	hm_sigmask(SIGABRT) | hm_sigmask(SIGBUS)  | \
	hm_sigmask(SIGFPE)  | hm_sigmask(SIGILL)  | \
	hm_sigmask(SIGQUIT) | hm_sigmask(SIGSEGV) | \
	hm_sigmask(SIGSYS)  | hm_sigmask(SIGTRAP) | \
	hm_sigmask(SIGXCPU) | hm_sigmask(SIGXFSZ))

#define SIG_TERMINATE_MASK (\
	hm_sigmask(SIGALRM) | hm_sigmask(SIGHUP)  | \
	hm_sigmask(SIGINT)  | hm_sigmask(SIGKILL) | \
	hm_sigmask(SIGPIPE) | hm_sigmask(SIGPOLL) | \
	hm_sigmask(SIGPROF) | hm_sigmask(SIGTERM) | \
	hm_sigmask(SIGUSR1) | hm_sigmask(SIGUSR2) | \
	hm_sigmask(SIGVTALRM) | hm_sigmask(SIGPWR) | \
	hm_sigmask(SIGSTKFLT))

#define SIG_IGNORE_MASK (\
	hm_sigmask(SIGCHLD) | hm_sigmask(SIGURG) | \
	hm_sigmask(SIGWINCH) | hm_sigmask(SIGCONT))

#define SIGNAL_MIN_STANDARD	1U
#define SIGNAL_NR_STANDARD	34U
#define SIGNAL_MAX_STANDARD	(SIGNAL_MIN_STANDARD + SIGNAL_NR_STANDARD - 1U)
#define SIGNAL_MASK_STANDARD	(((1ULL << SIGNAL_MAX_STANDARD) - 1ULL) - \
				((1ULL << (SIGNAL_MIN_STANDARD - 1U)) - 1ULL))

#define SIGNAL_UNDEFINED_MIN	32U
#define SIGNAL_UNDEFINED_MAX	34U

#define SIGNAL_MIN_RT		(SIGNAL_NR_STANDARD + 1U)
#define SIGNAL_MAX_RT		64U
#define SIGNAL_NR_RT		(SIGNAL_MAX_RT - SIGNAL_MIN_RT + 1U)

#define SIGNAL_RT		SIGNAL_MIN_RT

#define SIGNAL_FREEZE_USER_HANDLE	43

#define HM_SIGNALFD_EPOLL_ADD    1
#define HM_SIGNALFD_EPOLL_DEL    2
#define HM_SIGNALFD_EPOLL_CLOSE  3
#define HM_SIGNALFD_EPOLL_UPDATE 4

/* Used in SYS_rt_sigaction */
struct k_sigaction {
	union {
		void (*ksa_handler)(int signum);
		void (*ksa_sigaction)(int signum, siginfo_t *siginfo, void *param);
		void *ksa_handler_type;
	};
	unsigned long ksa_flags;
	union {
		void (*ksa_restorer)(void);
		unsigned long ksa_restorer_addr;
	};
	unsigned ksa_mask[KSA_MASK_SIZE];
};

struct compat_sigaction {
	unsigned int ksa_handler;
	unsigned int ksa_flags;
	unsigned int ksa_restorer;
	unsigned int ksa_mask[KSA_MASK_SIZE];
};

#define UNI_SA_NOFPSIMD (1ULL << 63)

/* Used in sysif and sigframe */
struct uni_sigaction {
	uint64_t handler;
	uint64_t flags;
	uint64_t restorer;
	uint64_t mask;
};

#define HM_SIG_DFL 0UL
#define HM_SIG_IGN 1UL

#define SIGSTACK_FLAG_SS_AUTODISARM	(1U << 31)

/* Information saved by the sysmgr on the destination thread stack */
struct sigframe {
	struct arch_uctx uctx;
	struct __siginfo siginfo;
	struct uni_sigaction action;
	uint64_t sigprocmask;
	uint64_t wu_desc_addr;
	uint64_t direct_signal;
};

/* Compatible sigframe structure for 32-bit */
struct compat_sigframe {
	struct aarch32_uctx uctx;
	struct __siginfo siginfo;
	struct uni_sigaction action;
	uint64_t sigprocmask;
	uint64_t wu_desc_addr;
	uint64_t direct_signal;
};

/* used for SA_RESTART */
#ifndef __HOST_LLT__
static inline bool hm_signal_restart_vsyscall_of(uintptr_t tls)
{
	uintptr_t cur_tls = tls;
	cur_tls -= HM_SIGNAL_ABI_BASE_OFFSET;
	cur_tls += offsetof(union hm_signal_abi_u, signal_restart);
	return (*(char *)cur_tls != (char)0);
}

static inline void hm_set_signal_restart_vsyscall(uintptr_t tls, bool restart)
{
	uintptr_t cur_tls = tls;
	cur_tls -= HM_SIGNAL_ABI_BASE_OFFSET;
	cur_tls += offsetof(union hm_signal_abi_u, signal_restart);
	*(char *)cur_tls = restart ? (char)1 : (char)0;
}

static inline bool hm_signal_handled(uintptr_t tls)
{
	uintptr_t cur_tls = tls;
	cur_tls -= HM_SIGNAL_ABI_BASE_OFFSET;
	cur_tls += offsetof(union hm_signal_abi_u, signal_handled);
	return (*(char *)cur_tls != (char)0);
}

static inline void hm_set_signal_handled(uintptr_t tls, bool handled)
{
	uintptr_t cur_tls = tls;
	cur_tls -= HM_SIGNAL_ABI_BASE_OFFSET;
	cur_tls += offsetof(union hm_signal_abi_u, signal_handled);
	*(char *)cur_tls = handled ? (char)1 : (char)0;
}
#else
static inline bool hm_signal_restart_vsyscall_of(uintptr_t tls)
{
	UNUSED(tls);
	return false;
}

static inline void hm_set_signal_restart_vsyscall(uintptr_t tls, bool restart)
{
	UNUSED(tls, restart);
}

static inline bool hm_signal_handled(uintptr_t tls)
{
	UNUSED(tls);
	return true;
}

static inline void hm_set_signal_handled(uintptr_t tls, bool handled)
{
	UNUSED(tls, handled);
}
#endif

void hm_fill_posix_siginfo(siginfo_t *psiginfo, const struct __siginfo *siginfo);
void __sigset2mask(const sigset_t *set, uint64_t *sigmask);
void __mask2sigset(uint64_t sigmask, sigset_t *set);

int hm_sigaction(int signum, const struct k_sigaction *act, struct k_sigaction *oldact);
int hm_rt_sigprocmask(int how, const sigset_t *nset, sigset_t *oset, int sigsize);
int __hm_rt_sigprocmask(int how, const sigset_t *nset, sigset_t *oset, int sigsize, bool restart);
int hm_rt_sigpending(sigset_t *set);
int hm_rt_sigtimedwait(const sigset_t *set, siginfo_t *psiginfo, const struct timespec *ptimeout);
int hm_rt_sigqueueinfo(pid_t tgid, int signum, siginfo_t *psiginfo);
int hm_rt_tgsigqueueinfo(pid_t tgid, pid_t tid, int signum, siginfo_t *psiginfo);
int hm_kill(pid_t pid, int signum);
int hm_tkill(pid_t tid, int signum);
int hm_tgkill(pid_t tgid, pid_t tid, int signum);
int hm_sigaltstack(const stack_t *ss, stack_t *old_ss);
/* used in signalfd.c */
int hm_read_signalevents(__u64 sigmask, __u32 *revents);
int hm_signal_epoll_ctrl(int fd, int op, __u64 sigmask);

static inline bool
__is_rt_signum_valid(unsigned int num)
{
	return num >= __SIGNAL_MIN_RT && num <= __SIGNAL_MAX_RT;
}

static inline bool
__is_st_signum_valid(unsigned int num)
{
	/* signum 0 is valid signum */
	return num <= __SIGNAL_MAX_STANDARD;
}

static inline bool
__is_signum_valid(unsigned int num)
{
	/* signum 0 is valid signum */
	return num <= __SIGNAL_MAX_RT;
}

static inline bool
__is_sig_action_settable(unsigned int num)
{
	return (num != SIGKILL) && (num != SIGSTOP);
}

static inline bool
__is_undefined_signum_valid(unsigned int num)
{
	return num >= __SIGNAL_UNDEFINED_MIN && num <= __SIGNAL_UNDEFINED_MAX;
}

static inline bool
__is_signum_coredump(unsigned int num)
{
	return hm_siginmask(num, SIG_COREDUMP_MASK);
}

/*
 * Undefined signal(32~34) and rt-signal(35~64) should alse be terminated
 * by default.
 */
static inline bool
__is_signum_terminate(unsigned int num)
{
	return hm_siginmask(num, SIG_TERMINATE_MASK) ||
	       __is_undefined_signum_valid(num) || __is_rt_signum_valid(num);
}

static inline bool
__is_signum_ignored(unsigned int num)
{
	return hm_siginmask(num, SIG_IGNORE_MASK);
}

void vdso_sigentry(struct sigframe *frame);
void vdso_sigtramp1(uintptr_t arg, struct sigframe *frame);
void vdso_sigtramp2(uintptr_t arg, struct sigframe *frame);

#endif /* ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SIGNAL_H */
