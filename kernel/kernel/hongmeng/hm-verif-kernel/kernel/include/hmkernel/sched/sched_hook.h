/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: General sched hook header.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 6 14:21:11 2024
 */
 
#ifndef HMKERNEL_SCHED_HOOK_H
#define HMKERNEL_SCHED_HOOK_H
 
#include <hmkernel/tcb.h>
#include <hmkernel/cpuset.h>
#include <hmkernel/sched_module.h>
#include <hmkernel/sched/lt.h>

#include <mapi/hmkernel/lite_hook.h>

/* sched.c */
LITE_HOOK_DECLARE_VOID(sched_tick,
	LH_PROTO(unsigned int cpu, struct tcb_s *tcb),
	LH_ARGS(cpu, tcb))

LITE_HOOK_DECLARE_VOID(sched_in_hook,
	LH_PROTO(struct tcb_s *tcb, unsigned int cpu),
	LH_ARGS(tcb, cpu))

LITE_HOOK_DECLARE_VOID(sched_out_hook,
	LH_PROTO(struct tcb_s *tcb, unsigned int cpu),
	LH_ARGS(tcb, cpu))

LITE_HOOK_DECLARE_VOID(sched_switch_hook,
	LH_PROTO(struct tcb_s *prev_tcb, struct tcb_s *next_tcb, unsigned int cpu),
	LH_ARGS(prev_tcb, next_tcb, cpu))

LITE_HOOK_DECLARE_VOID(sched_tcb_init,
	LH_PROTO(struct tcb_s *tcb, struct tcb_s *parent, const struct tcb_init_attr_s *attr),
	LH_ARGS(tcb, parent, attr))

LITE_HOOK_DECLARE_VOID(sched_tcb_exit,
	LH_PROTO(struct tcb_s *tcb),
	LH_ARGS(tcb))

LITE_HOOK_DECLARE_VOID(sched_init,
	LH_PROTO(bool *done),
	LH_ARGS(done))

LITE_HOOK_DECLARE_VOID(user_context_switch_hook,
	LH_PROTO(struct tcb_s *tcb),
	LH_ARGS(tcb))

LITE_HOOK_DECLARE_VOID(ffts_thd_sched_info_sync,
	LH_PROTO(struct capcall_info_s *callinfo, const void __user *args, size_t args_size,
	 __maybe_unused void __user *result, __maybe_unused size_t result_size, int *err),
	LH_ARGS(done))

LITE_HOOK_DECLARE_VOID(flo_save_hook,
	LH_PROTO(struct arch_uctx *uctx),
	LH_ARGS(uctx))

LITE_HOOK_DECLARE_VOID(flo_restore_hook,
	LH_PROTO(struct tcb_s *tcb, struct arch_uctx *uctx),
	LH_ARGS(tcb, uctx))

LITE_HOOK_DECLARE(lt_set_conf, int, E_HM_OK,
	LH_PROTO(struct tcb_s *tcb, struct sched_lt_args_s *lt_args),
	LH_ARGS(tcb, lt_args))

#endif
