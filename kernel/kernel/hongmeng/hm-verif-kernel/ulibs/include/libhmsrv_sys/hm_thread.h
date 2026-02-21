/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Thread function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 24 15:32:39 2019
 */
#ifndef ULIBS_INCLUDE_HM_THREAD_H
#define ULIBS_INCLUDE_HM_THREAD_H

#include <hmkernel/capability.h>
#include <hmkernel/mm/kexec.h>
#include <hmkernel/tcb.h>
#include <stdint.h>
#include <stdbool.h>
#include <hmsched.h>
#include <time.h>
#include <sched.h>

#define HM_THREAD_TRY_FREEZE_SUCC	1U
#define HM_THREAD_NEED_FREEZING		2U

void hm_thread_exit(int exitcode);
int hm_thread_setname(cref_t thread_cref, const char *new_name, char *old_name);
int hm_thread_set_pdeath_signal(int signum);
int hm_thread_get_pdeath_signal(int *signum);
cref_t hm_thread_self_cref(void);
cref_t hm_thread_effective_cref(void);
cref_t hm_thread_tid2cref(int tid);
int hm_thread_cref2tid(cref_t cref);
cref_t hm_thread_vspace_cref(cref_t thread_cref);
int hm_thread_nstid_of_root(int root_tid, int *tid_array, int len);
int hm_thread_gettid(void);
int hm_thread_main_thread_name_of_cnode_idx(uint32_t cnode_idx, char *name, uint32_t len);
int hm_thread_get_name_by_root_ns_tid(int root_ns_tid, char *name, uint32_t len);
int hm_thread_stat(cref_t tcb_cref, struct sysarg_thread_stat *stat, unsigned long flag);
int __hm_thread_stat(cref_t tcb_cref, struct sysarg_thread_stat *stat,
		     struct __arch_fpsimd_state *fpsimd, unsigned long flag);
int hm_thread_set_runqueue(int tid, unsigned int newrq);
int hm_thread_kexec_load_files(struct softreboot_info files);
int hm_thread_set_tickless(int tid, bool tickless);
int hm_thread_yield(void);
int hm_thread_suspend(void);
int hm_thread_suspend_ex(cref_t cref);
int hm_thread_resume(cref_t cref);
int hm_thread_freeze(int tid, int flag);
int hm_thread_thaw(int tid);
int hm_thread_set_freezable(void);
bool hm_thread_freezing(void);
int hm_thread_try_to_freeze(void);
int hm_thread_sched_setparam(int tid, struct sched_param *param_user);
int hm_thread_sched_getparam(int tid, struct sched_param *param_user);
int hm_thread_sched_setscheduler(int tid, int policy, struct sched_param *param_user);
int hm_thread_sched_getscheduler(int tid);
int hm_thread_sched_setattr(int tid, struct sched_attr *uattr, unsigned int flags);
int hm_thread_sched_getattr(int tid, struct sched_attr *uattr, unsigned int size,
			    unsigned int flags);
int hm_thread_sched_setattr_native(int tid, struct sched_attr *uattr, unsigned int flags);
int hm_thread_sched_getattr_native(int tid, struct sched_attr *uattr, unsigned int size,
				   unsigned int flags);
int hm_thread_sched_getdefaultscheduler(void);
int hm_thread_sched_getdefaultpriority(void);
int hm_thread_get_seq(int tid, uint64_t *seq);
int hm_thread_setpriority(int which, int tid, int nice);
int hm_thread_sched_set_max_util(int tid, unsigned int max_util);
int hm_thread_sched_setaffinity(int tid, size_t size, void *cpuset);
int hm_thread_sched_getaffinity(int tid, size_t size, void *cpuset);
int hm_thread_sched_setaffinity_direct(int tid, size_t size, void *cpuset);
int hm_thread_sched_getaffinity_direct(int tid, size_t size, void *cpuset);
int hm_thread_kvic_source_bind(cref_t tcb_cref, unsigned int source_id, unsigned int flags);
int hm_thread_kvic_source_unbind(cref_t tcb_cref, unsigned int source_id);
int hm_thread_kvic_kev_source_bind(cref_t tcb_cref, unsigned int source_id, unsigned int flags);
int hm_thread_kvic_kev_source_unbind(cref_t tcb_cref, unsigned int source_id);
int hm_thread_attach_cpu_context(cref_t thread, cref_t context,
				 enum __tcb_attach_type type);
int hm_thread_detach_cpu_context(cref_t thread, cref_t *pcontext,
				 enum __tcb_attach_type type);
int hm_thread_sched_trans(cref_t src_cref, cref_t dst_cref, bool restore);
int hm_thread_deadline_ctrl(unsigned long cmd, int state);
int hm_thread_get_tokens(cref_t cref, __u64 *token, __u64 *ftoken);
int hm_thread_bind_cpu(void);
int hm_thread_unbind_cpu(void);
int hm_thread_set_pin_preempted(void);
#ifdef CONFIG_SECCOMP
void hm_thread_set_kill_signal(int tid);
#endif
#endif
