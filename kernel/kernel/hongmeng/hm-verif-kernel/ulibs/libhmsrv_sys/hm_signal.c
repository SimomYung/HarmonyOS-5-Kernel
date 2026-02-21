/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2023. All rights reserved.
 * Description: Implementation of sysif server signal
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 15:02:00 2018
 */

/* include this header first to use hm_signal_defs.h instead of signal.h */
#include <libhmsrv_sys/hm_signal.h>

#include <libmem/utils.h>
#include <hongmeng/types.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/bitops/generic.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#include <unistd.h>

#include "../libhmsync/raw_tls.h"

#include <libhmsrv_sys/hm_hmsignal.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_futex.h>

#ifndef __WORDSIZE
# if SIZE_MAX == UINT_MAX
#  define __WORDSIZE	32
# else
#  define __WORDSIZE	64
# endif
#endif
#define __NSIG_WORDS	(_NSIG / __WORDSIZE)

#if ((__NSIG_WORDS == 2) || (SIZE_MAX == UINT_MAX))
#define __SS_SHIFT	32U
#endif

void __sigset2mask(const sigset_t *set, uint64_t *sigmask)
{
#if __NSIG_WORDS == 2
	/* __bits[1] stands for higher 32 bit of sigmask */
	*sigmask = set->__bits[0] | ((uint64_t)set->__bits[1] << __SS_SHIFT);
#elif __NSIG_WORDS == 1
	*sigmask = set->__bits[0];
#else
# error "unsupported _NSIG"
#endif
}

void __mask2sigset(uint64_t sigmask, sigset_t *set)
{
#if __NSIG_WORDS == 2
	set->__bits[0] = (unsigned long)sigmask;
	set->__bits[1] = (unsigned long)(sigmask >> __SS_SHIFT);
#elif __NSIG_WORDS == 1
	set->__bits[0] = (unsigned long)sigmask;
#else
# error "unsupported _NSIG"
#endif
}

/* Keep in sync with vdso_convert_to_siginfo_t */
static void __siginfo_k_to_u(siginfo_t *u, const struct __siginfo *k)
{
	u->si_signo	= (int)k->signum;
	u->si_code	= (int)k->si_code;
	u->si_errno	= (int)k->si_errno;

	/* Apply other siginfo later */
	if (k->si_type == (__u32)__SI_RT) {
		u->si_value.sival_ptr = u64_to_ptr(k->__sigrt.sig_value.sival_ptr, void);
		u->si_pid = k->__sigrt.pid;
		u->si_uid = k->__sigrt.uid;
	} else if (k->si_type == (__u32)__SI_CHLD) {
		u->si_pid = k->__sigchld.pid;
		u->si_uid = k->__sigchld.uid;
		u->si_status = k->__sigchld.status;
	} else if (k->si_type == (__u32)__SI_FAULT) {
		u->si_addr = u64_to_ptr(k->__fault.fault_addr, void);
	} else if (k->si_type == (__u32)__SI_KILL) {
		u->si_pid = k->__sigkill.pid;
		u->si_uid = k->__sigkill.uid;
	} else if (k->si_type == (__u32)__SI_SYS) {
		u->si_call_addr = u64_to_ptr(k->__sigsys.call_addr, void *);
		u->si_syscall = k->__sigsys.syscall;
		u->si_arch = k->__sigsys.arch;
	} else {
		hm_warn("unknown si_type=%u\n", k->si_type);
	}
}

static inline void __siginfo_u_to_k(struct __siginfo *k, const siginfo_t *u)
{
	k->signum	= (unsigned int)u->si_signo;
	k->si_code	= (unsigned int)u->si_code;
	k->si_type	= __SI_RT;
	k->__sigrt.pid	= u->si_pid;
	k->__sigrt.uid	= u->si_uid;
	k->si_errno	= (unsigned int)u->si_errno;
	k->__sigrt.sig_value.sival_ptr = ptr_to_u64(u->si_value.sival_ptr);
}

void hm_fill_posix_siginfo(siginfo_t *psiginfo,
			       const struct __siginfo *siginfo)
{
	__siginfo_k_to_u(psiginfo, siginfo);
}

void hm_fill_posix_siginfo_compat(compat_siginfo_t *u, const struct __siginfo *k)
{
	u->si_signo	= (int)k->signum;
	u->si_code	= (int)k->si_code;
	u->si_errno	= (int)k->si_errno;

	if (k->si_type == (__u32)__SI_RT) {
		u->si_value = (__u32)k->__sigrt.sig_value.sival_ptr;
		u->si_pid = k->__sigrt.pid;
		u->si_uid = k->__sigrt.uid;
	} else if (k->si_type == (__u32)__SI_CHLD) {
		u->si_pid = k->__sigchld.pid;
		u->si_uid = k->__sigchld.uid;
		u->si_status = k->__sigchld.status;
	} else if (k->si_type == (__u32)__SI_FAULT) {
		u->si_addr = (__u32)k->__fault.fault_addr;
	} else if (k->si_type == (__u32)__SI_KILL) {
		u->si_pid = k->__sigkill.pid;
		u->si_uid = k->__sigkill.uid;
	} else if (k->si_type == (__u32)__SI_SYS) {
		u->si_call_addr = (__u32)k->__sigsys.call_addr;
		u->si_syscall = k->__sigsys.syscall;
		u->si_arch = k->__sigsys.arch;
	} else {
		hm_warn("unknown si_type=%u\n", k->si_type);
	}
}

int hm_sigaction(int signum, const struct k_sigaction *act, struct k_sigaction *oldact)
{
	int rc = E_HM_OK;

	if (signum == 0 || !__is_signum_valid((unsigned int)signum) ||
	    ((act != NULL) && !__is_sig_action_settable((unsigned int)signum))) {
		rc = E_HM_INVAL;
	} else {
		bool set_new = false;
		struct uni_sigaction sa;
		struct __actvret_hmcall_signal_set_signal_handler ret;

		mem_zero_s(sa);
		mem_zero_s(ret);

		if (act != NULL) {
			set_new = true;
			sa.handler = (uintptr_t)act->ksa_handler_type;
			sa.flags = act->ksa_flags;
			sa.restorer = (uintptr_t)act->ksa_restorer;
#if SIZE_MAX == UINT_MAX
			sa.mask = act->ksa_mask[0];
			sa.mask |= (uint64_t)act->ksa_mask[1] << __SS_SHIFT;
#else
			sa.mask = *(ptr_cast(act->ksa_mask, uint64_t));
#endif
		}

		rc = actvcall_hmcall_signal_set_signal_handler((unsigned int)signum, set_new, sa, &ret);
		if (rc == E_HM_OK && oldact != NULL) {
			oldact->ksa_handler_type = (void *)(uintptr_t)ret.old_act.handler;
			oldact->ksa_restorer = (void (*)(void))(uintptr_t)ret.old_act.restorer;
			oldact->ksa_flags = (unsigned long)ret.old_act.flags;
#if SIZE_MAX == UINT_MAX
			oldact->ksa_mask[0] = (unsigned long)ret.old_act.mask;
			oldact->ksa_mask[1] = (unsigned long)(ret.old_act.mask >> __SS_SHIFT);
#else
			*(uint64_t *)oldact->ksa_mask = ret.old_act.mask;
#endif
		}
	}

	return rc;
}

int hm_rt_sigprocmask(int how, const sigset_t *nset, sigset_t *oset, int sigsize)
{
	int rc;
	uint64_t old_mask = 0ULL;
	uint64_t new_mask = 0ULL;
	struct __actvret_hmcall_signal_set_sigmask ret;
	mem_zero_s(ret);

	if ((size_t)(unsigned int)sigsize != SIGSET_SIZE) {
		rc = E_HM_INVAL;
	} else {
		rc = actvcall_hmcall_signal_set_sigmask(SIGNAL_GET_SIGMASK, 0, &ret);
		if (rc == E_HM_OK) {
			old_mask = ret.old_mask;
		}
	}

	if (rc == E_HM_OK) {
		if (nset != NULL) {
			__sigset2mask(nset, &new_mask);

			/* Simply ignore SIGKILL and SIGSTOP. */
			new_mask &= ~((__u64)1U << ((__u64)SIGKILL - 1U));
			new_mask &= ~((__u64)1U << ((__u64)SIGSTOP - 1U));

			switch (how) {
			case SIG_BLOCK:
				new_mask |= old_mask;
				break;
			case SIG_UNBLOCK:
				new_mask = (~new_mask) & old_mask;
				break;
			case SIG_SETMASK:
				break;
			default:
				hm_debug("unsupported, how=%d\n", how);
				rc = E_HM_INVAL;
				break;
			}

			if (rc == E_HM_OK) {
				rc = actvcall_hmcall_signal_set_sigmask(SIGNAL_SET_SIGMASK, new_mask, &ret);
			}
		}

		if (oset != NULL) {
			__mask2sigset(old_mask, oset);
		}
	}

	return rc;
}

int hm_rt_sigpending(sigset_t *set)
{
	int rc;
	struct __actvret_hmcall_signal_get_pending_signal ret;
	mem_zero_s(ret);

	rc = actvcall_hmcall_signal_get_pending_signal(&ret);
	if (rc < 0) {
		hm_debug("get pending signals failed, rc=%s\n", hmstrerror(rc));
	} else {
		__mask2sigset((uint64_t)ret.pending_signal, set);
	}

	return rc;
}

int hm_rt_sigtimedwait(const sigset_t *set, siginfo_t *psiginfo,
		       const struct timespec *ptimeout)
{
	int rc;
	bool set_timeout = false;
	uint64_t sigmask;
	struct timespec timeout;
	struct __actvret_hmcall_signal_timedwait ret;

	if (set == NULL) {
		rc = E_HM_INVAL;
	} else {
		sigset_t sigset = *set;
		(void)sigdelset(&sigset, SIGKILL);
		(void)sigdelset(&sigset, SIGSTOP);
		__sigset2mask(&sigset, &sigmask);

		mem_zero_s(timeout);
		if (ptimeout != NULL) {
			set_timeout = true;
			mem_assign_s(timeout, *ptimeout);
		}

		mem_zero_s(ret);
		rc = actvcall_hmcall_signal_timedwait(sigmask, set_timeout, timeout.tv_sec, timeout.tv_nsec, &ret);
		if (rc == E_HM_OK) {
			if (psiginfo != NULL) {
				hm_fill_posix_siginfo(psiginfo, &ret.siginfo);
			}
			rc = (int)ret.siginfo.signum;
		}
	}

	return rc;
}

int hm_sigaltstack(const stack_t *ss, stack_t *old_ss)
{
	int rc;
	stack_t sas;
	bool set_new = false;
	struct __actvret_hmcall_signal_set_sigstack ret;

	mem_zero_s(ret);

	if (ss != NULL) {
		set_new = true;
		sas = *ss;
	} else {
		mem_zero_s(sas);
	}

	rc = actvcall_hmcall_signal_set_sigstack(set_new, sas, &ret);
	if (rc == E_HM_OK && old_ss != NULL) {
		*old_ss = ret.old_ss;
	}

	return rc;
}

int hm_kill(pid_t pid, int signum)
{
	int rc;
	struct siginfo_set sig_set;
	mem_zero_s(sig_set);

	if (!__is_signum_valid((unsigned int)signum)) {
		rc = E_HM_INVAL;
	} else {
		rc = actvcall_hmcall_signal_kill(pid, signum, sig_set);
		if (rc < 0) {
			hm_debug("send signal failed, rc=%s\n", hmstrerror(rc));
		}
	}

	return rc;
}


int hm_tkill(pid_t tid, int signum)
{
	int rc;
	struct siginfo_set sig_set;
	mem_zero_s(sig_set);

	if (tid < 0) {
		rc = E_HM_INVAL;
	} else if (!__is_signum_valid((unsigned int)signum)) {
		rc = E_HM_INVAL;
	} else {
		rc = actvcall_hmcall_signal_tkill(tid, signum, sig_set);
		if (rc < 0) {
			hm_debug("send signal failed, rc=%s\n", hmstrerror(rc));
		}
	}

	return rc;
}

int hm_tgkill(pid_t tgid, pid_t tid, int signum)
{
	int rc;

	if ((tgid <= 0) || (tid <= 0)) {
		rc = E_HM_INVAL;
	} else if (!__is_signum_valid((unsigned int)signum)) {
		rc = E_HM_INVAL;
	} else {
		rc = actvcall_hmcall_signal_tgkill(tgid, tid, signum);
		if (rc < 0) {
			hm_debug("send signal failed, rc=%s\n", hmstrerror(rc));
		}
	}

	return rc;
}

int hm_rt_sigqueueinfo(pid_t tgid, int signum, siginfo_t *psiginfo)
{
	int rc;

	if (!__is_signum_valid((unsigned int)signum)) {
		rc = E_HM_INVAL;
	} else if (psiginfo == NULL) {
		rc = E_HM_INVAL;
	} else {
		struct __siginfo siginfo;
		mem_zero_s(siginfo);

		__siginfo_u_to_k(&siginfo, psiginfo);
		rc = actvcall_hmcall_signal_sigqueue(tgid, siginfo, signum);
		if (rc < 0) {
			hm_debug("send signal failed, rc=%s\n", hmstrerror(rc));
		}
	}

	return rc;
}

int hm_rt_tgsigqueueinfo(pid_t tgid, pid_t tid, int signum, siginfo_t *psiginfo)
{
	int rc;

	if ((tgid <= 0) || (tid <= 0)) {
		rc = E_HM_INVAL;
	} else if (!__is_signum_valid((unsigned int)signum)) {
		rc = E_HM_INVAL;
	} else if (psiginfo == NULL) {
		rc = E_HM_INVAL;
	} else {
		struct __siginfo siginfo;
		mem_zero_s(siginfo);

		__siginfo_u_to_k(&siginfo, psiginfo);
		rc = actvcall_hmcall_signal_pthread_sigqueue(tgid, tid, signum, siginfo);
		if (rc < 0) {
			hm_debug("send signal failed, rc=%s\n", hmstrerror(rc));
		}
	}

	return rc;
}

int hm_read_signalevents(__u64 sigmask, __u32 *revents)
{
	int rc;

	if (revents == NULL) {
		rc = E_HM_INVAL;
	} else {
		struct __actvret_hmcall_signal_read_signalevents ret;
		mem_zero_s(ret);

		*revents = 0;
		rc = actvxactcall_hmcall_signal_read_signalevents(true, true, sigmask, &ret);
		if (rc == E_HM_OK) {
			*revents = ret.revents;
		}
	}

	return rc;
}

int hm_signal_epoll_ctrl(int fd, int op, __u64 sigmask)
{
	return actvxactcall_hmcall_signal_epoll_ctrl(true, true, fd, sigmask, op);
}
