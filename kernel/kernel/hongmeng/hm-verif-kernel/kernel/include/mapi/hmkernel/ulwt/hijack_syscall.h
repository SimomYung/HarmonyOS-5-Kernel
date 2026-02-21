/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ULWT hijack syscall
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 1 18:02:27 2023
 */
#ifndef MAPI_HMKERNEL_ULWT_HIJACK_SYSCALL_H
#define MAPI_HMKERNEL_ULWT_HIJACK_SYSCALL_H

#include <hmkernel/types.h>
#include <hmasm/ucontext.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(SILK);

typedef enum ulwt_user_hook_nr {
	/* default true */
	ULWT_USER_HOOK_NR_GETTID = 0,
	ULWT_USER_HOOK_NR_EXIT,
	ULWT_USER_HOOK_NR_SET_TID_ADDRESS,
	ULWT_USER_HOOK_NR_RT_SIGSUSPEND,
	ULWT_USER_HOOK_NR_RT_SIGTIMEDWAIT,
	ULWT_USER_HOOK_NR_NANOSLEEP,
	ULWT_USER_HOOK_NR_SCHED_YIELD,
	ULWT_USER_HOOK_NR_EPOLL_PWAIT,
	ULWT_USER_HOOK_NR_EXECVE,
	/* need check paramter */
	ULWT_USER_HOOK_NR_CLONE,
	ULWT_USER_HOOK_NR_MUNMAP,
} ulwt_user_hook_nr_e;

#ifdef CONFIG_ULWT
int ulwt_user_hook(struct arch_uctx *uctx, ulwt_user_hook_nr_e nr, bool user_hook_bypass);
#else
static inline int ulwt_user_hook(struct arch_uctx *uctx, ulwt_user_hook_nr_e nr, bool user_hook_bypass)
{
	UNUSED(uctx, nr, user_hook_bypass);
	return E_HM_OK;
}
#endif
#endif
