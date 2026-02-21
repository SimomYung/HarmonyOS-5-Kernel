/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Apis for sigset.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 23 21:59:26 2020
 */
#ifndef SYSMGR_INTAPI_SIGNAL_SIGSET_H
#define SYSMGR_INTAPI_SIGNAL_SIGSET_H

#include <stdbool.h>
#include <stdint.h>
#include <libhmsrv_sys/hm_signal.h>

/*
 * For the signums used in sysmgr, they must have been checked and belong to [1, 64].
 * So we can use __SIGIDX_OF to switch signum to idx.
 * 1. for the signum from kernel, it must be check in the hmsignal handler when find on the timer list;
 * 2. for the signum from client, we have checked in signal_process by __is_rt_signum_valid and
 *    __is_st_signum_valid function.
 */
#define __SIGIDX_OF(nr)		((nr) - 1U)
#define SIGSET_BIT(nr)		(1ULL << ((unsigned int)(nr) - 1U))

static inline uint64_t
sigset_set(uint64_t sigset, unsigned int signum)
{
	uint64_t sigmask = sigset;

	if (signum > 0U && signum < (unsigned int)_NSIG) {
		sigmask |= SIGSET_BIT(signum);
	}
	return sigmask;
}

static inline bool
sigset_test(uint64_t sigset, unsigned int signum)
{
	bool set = false;

	if (signum > 0U && signum < (unsigned int)_NSIG &&
	    (sigset & SIGSET_BIT(signum)) != 0ULL) {
		set = true;
	}
	return set;
}

static inline uint64_t
sigset_clr(uint64_t sigset, unsigned int signum)
{
	uint64_t sigmask = sigset;

	if (signum > 0U && signum < (unsigned int)_NSIG) {
		sigmask &= ~(SIGSET_BIT(signum));
	}
	return sigmask;
}

/*
 * Because __builtin_ffsll returns the position of the first 1
 * in an unsigned long long. For example, if the 2 bit (0-63) is 1,
 * it returns 3. Signal encoding is from 1-64, which is exactly
 * what we expected.
 */
static inline unsigned int
sigset_first_signum(uint64_t sigset)
{
	return (unsigned int)__builtin_ffsll((long long)sigset);
}

/* Clear the unmaskable signal bit in the given sigset */
static inline uint64_t
sigset_ign_unmaskable(uint64_t sigset)
{
	return (sigset & (~(SIGSET_BIT(SIGKILL) | SIGSET_BIT(SIGSTOP))));
}

static inline uint64_t
sigset_and(uint64_t sigset, uint64_t sigmask)
{
	return (sigset & sigmask);
}

static inline uint64_t
sigset_or(uint64_t sigset, uint64_t sigmask)
{
	return (sigset | sigmask);
}

#define SIGSET_FULL	((uint64_t)(-1))
#define SIGSET_EMPTY	((uint64_t)(0))
#define SIGSET_MASKABLE	(sigset_ign_unmaskable(SIGSET_FULL))

#endif
