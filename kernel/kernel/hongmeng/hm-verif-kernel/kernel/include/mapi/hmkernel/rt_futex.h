/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Rt futex function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 14 15:16:20 2020
 */
#ifndef MAPI_HMKERNEL_RT_FUTEX_H
#define MAPI_HMKERNEL_RT_FUTEX_H

#include <hmkernel/rt_futex_key.h>
#include <uapi/hmkernel/rt_futex.h>
#include <hmkernel/capability/cap_thread.h>

struct tcb_s;

#define RT_FUTEX_HASH_TBL_SIZE 127U

#ifdef CONFIG_RT_FUTEX
int rt_futex_wait_hook(struct tcb_s *caller, struct tcb_s *owner,
		       struct rt_futex_key_s *key);
int rt_futex_wake_hook(struct tcb_s *caller, const struct rt_futex_key_s *key,
		       struct tcb_s **next_owner, int __user *uaddr);
bool rt_futex_intr_hook(struct tcb_s *tcb);
int rt_futex_exit(struct tcb_s *tcb, struct sysarg_thread_rt_futex_exit_info *exit_info);
void rt_futex_enable(void);
int rt_futex_exit_transfer(const struct tcb_s *tcb, struct tcb_s *next);
void rt_futex_hb_acq_hook(struct rt_futex_key_s *key);
void rt_futex_hb_rel_hook(const struct rt_futex_key_s *key);
int rt_futex_exit_set_uvalue(struct tcb_s *next, struct capability_s *vspace, int __user *futex_vaddr, u32 flags);
int rt_futex_update_uvalue(struct tcb_s *next_owner, int __user *uaddr);
#else
static inline int rt_futex_wait_hook(struct tcb_s *caller, struct tcb_s *owner,
				     struct rt_futex_key_s *key)
{
	UNUSED(caller, owner, key);
	return 0;
}
static inline int rt_futex_wake_hook(struct tcb_s *caller,
				     const struct rt_futex_key_s *key,
				     struct tcb_s **next_owner, int __user *uaddr)
{
	UNUSED(caller, key, next_owner, uaddr);
	return 0;
}
static inline bool rt_futex_intr_hook(struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}
static inline int rt_futex_exit(const struct tcb_s *tcb, struct sysarg_thread_rt_futex_exit_info *exit_info)
{
	UNUSED(tcb, exit_info);
	return 0;
}
static inline void rt_futex_enable(void) {};
static inline int rt_futex_exit_transfer(const struct tcb_s *tcb,
					 struct tcb_s *next)
{
	return 0;
}
static void rt_futex_hb_acq_hook(struct rt_futex_key_s *key)
{
	UNUSED(key);
}
static void rt_futex_hb_rel_hook(const struct rt_futex_key_s *key)
{
	UNUSED(key);
}
static inline int rt_futex_exit_set_uvalue(struct tcb_s *next, struct capability_s *vspace,
					   int __user *futex_vaddr, u32 flags)
{
	UNUSED(next, vspace, futex_vaddr, flags);
	return 0;
}
static inline int rt_futex_update_uvalue(struct tcb_s *next_owner, int __user *uaddr)
{
	UNUSED(next_owner, uaddr);
	return 0;
}
#endif

#endif
