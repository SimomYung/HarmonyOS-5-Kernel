/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for thread operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <sched.h>
#include <stdio.h>
#include <hmasm/types.h>
#include <hmsysmgr/dumpstack.h>
#include <hmkernel/sched_module.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libsysif/sysmgr/api.h>
#include <libhmactv/actv.h>
#include <libhmsync/thread.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_dump.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_proc_affinity.h>
#include <libhmucap/ucap.h>
#include <libmem/umem.h>
#include <libpreempt/preempt.h>
#include <libsysif/utils_common.h>
#include <hmkernel/sched/priority.h>

#include <devhost/backend.h>
#include <devhost/err.h>
#include <devhost/interrupt.h>
#include <devhost/log.h>
#include <devhost/pm.h>
#include <devhost/security.h>

static raw_thread_key_t thread_key;

int liblinux_pal_thread_create_ex(void* (*fun)(void *), void *arg,
				  liblinux_pal_thread_handle_t *handle)
{
	raw_thread_t td;
	int ret;

	ret = raw_thread_create(&td, NULL, fun, arg);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}

	handle->handle = (unsigned long long)ptr_to_ulong(td);

	return 0;
}

int liblinux_pal_thread_create(void* (*fun)(void *), void *arg,
			       liblinux_pal_thread_handle_t *handle)
{
	raw_thread_t td;
	int ret;

	ret = raw_thread_create(&td, NULL, fun, arg);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}

	handle->handle = (unsigned long long)ptr_to_ulong(td);
	handle->tid = raw_thread_tid_of(td);

	return 0;
}

int liblinux_pal_thread_create_task_init(void *(*func)(void *), void *arg,
					 liblinux_pal_thread_handle_t *handle,
					 struct task_struct *task,
					 void (*task_init)(struct task_struct *task,
							   liblinux_pal_thread_handle_t *handle))
{
	raw_thread_t td;
	raw_thread_attr_t attr;
	int ret;

	if ((task == NULL) || (task_init == NULL)) {
		return -EINVAL;
	}

	ret = raw_thread_attr_init(&attr);
	BUG_ON(ret != 0);

	ret = raw_thread_attr_setflags(&attr, THREAD_CONFIG_FLAGS_MASK_IS_SUSPEND);
	BUG_ON(ret != 0);

	ret = raw_thread_create(&td, &attr, func, arg);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}

	handle->handle = (unsigned long long)ptr_to_ulong(td);
	handle->tid = raw_thread_tid_of(td);
	handle->tcb_cref = raw_thread_cref_of(td);

	task_init(task, handle);

	ret = raw_thread_target_setspecific(td, thread_key, task);
	BUG_ON(ret != 0); /* fail only if thread_key is invalid */

	ret = hm_thread_resume(handle->tcb_cref);
	if (ret != 0) {
		dh_panic("Impossible thread resume failure, err=%s\n", hmstrerror(ret));
	}

	return 0;
}

void liblinux_pal_thread_exit(void)
{
	raw_thread_exit(NULL);
}

void liblinux_pal_thread_exit_ex(void *exitcode)
{
	raw_thread_exit(exitcode);
}

int liblinux_pal_thread_join_ex(const liblinux_pal_thread_handle_t *handle,
				void **exitcode)
{
	int ret;
	raw_thread_t td = ulong_to_ptr((unsigned long)handle->handle,
				       struct raw_thread);

	BUG_ON(td == NULL);

	ret = raw_thread_join(td, exitcode);

	return -hmerrno2posix(ret);
}

unsigned long long liblinux_pal_thread_actv_cref(void)
{
	return hm_actv_local_thread_cref_of();
}

unsigned long liblinux_pal_thread_stack_info(const liblinux_pal_thread_handle_t *handle, unsigned long *base)
{
	raw_thread_t td = ulong_to_ptr((unsigned long)handle->handle,
				       struct raw_thread);

	BUG_ON(td == NULL);

	*base = raw_thread_stack_base_of(td);
	return raw_thread_stack_size_of(td);
}

unsigned long long liblinux_pal_thread_sched_cref(void)
{
	return hm_actv_local_sched_thread_cref_of();
}

int liblinux_pal_thread_set_my_data(const void *data)
{
	int ret;

	ret = raw_thread_setspecific(thread_key, data);
	return -hmerrno2posix(ret);
}

void *liblinux_pal_thread_get_my_data(void)
{
	return raw_thread_getspecific(thread_key);
}

int liblinux_pal_thread_init_ex(void (*dtor)(void *))
{
	int ret;

	ret = raw_thread_key_create(&thread_key, dtor);
	return -hmerrno2posix(ret);
}

void liblinux_pal_thread_init(void)
{
	(void)liblinux_pal_thread_init_ex(NULL);
}

void liblinux_pal_thread_specific_init(unsigned int *offset, unsigned int *key)
{
	*key = thread_key;
	raw_thread_specific_init(offset);
}

void liblinux_pal_thread_yield(void)
{
	(void)sched_yield();
}

int liblinux_pal_thread_setname(const liblinux_pal_thread_handle_t *handle,
				const char *thread_name)
{
	int ret;
	raw_thread_t td = ulong_to_ptr((unsigned long)handle->handle,
				       struct raw_thread);

	BUG_ON(td == NULL);

	ret = hm_thread_setname(raw_thread_cref_of(td), thread_name, NULL);

	return -hmerrno2posix(ret);
}

int liblinux_pal_cpu_config_ex(unsigned long *online_mask, unsigned long size,
			       int preempt_flag)
{
	UNUSED(size);
	int ret;
	unsigned long cpu_nr;
	struct proc_affinity_info_s proc_affinity_info;

	if (online_mask == NULL) {
		return -EINVAL;
	}

	ret = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, &cpu_nr, sizeof(cpu_nr));
	if (ret < 0) {
		dh_error("devhost: get config cpu num failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	mem_zero_s(proc_affinity_info);
	ret = hm_proc_affinity_get(0, &proc_affinity_info);
	if (ret < 0) {
		dh_error("devhost: get process affinity failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = hm_sysctrl_read_conf(__SYSCONF_ONLINE_CPU_MASK, (void *)online_mask,
				   sizeof(*online_mask));
	if (ret < 0) {
		dh_error("devhost: get online cpu mask failed, ret = %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	if (proc_affinity_info.privileged == 0) {
		*online_mask &=  proc_affinity_info.default_affinity.__bits;
	}

	dh_info("LDK: config: cpu_nr=%lu, online_mask=%#lx\n", cpu_nr, *online_mask);
	if (____NOT(preempt_flag)) {
		dh_info("LDK: config_liblinux_preempt is disabled\n");
		return (int)cpu_nr;
	}

	hmsrv_update_sched_class_module_rref();
	ret = preempt_process_register();
	if (ret < 0) {
		dh_error("devhost: preempt register failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return (int)cpu_nr;
}

static void cpu_logical_map_read(unsigned long *cpu_map, unsigned long cpu_nr)
{
	UNUSED(cpu_nr);
	int ret;

	if (cpu_map == NULL) {
		/* means we don't need to get the cpu map info. */
		return;
	}

	ret = hm_sysctrl_read_conf(__SYSCONF_CPU_LOGICAL_MAP, (void *)cpu_map, sizeof(*cpu_map) * cpu_nr);
	if (ret < 0) {
		panic("devhost: get cpu topology failed, ret=%s\n", hmstrerror(ret));
	}

	return;
}

int liblinux_pal_cpu_config(unsigned long *online_mask,
			     unsigned long *cpu_map,
			     unsigned long cpu_nr,
			     int preempt_flag)
{
	cpu_logical_map_read(cpu_map, cpu_nr);
	return liblinux_pal_cpu_config_ex(online_mask, sizeof(*online_mask), preempt_flag);
}

int liblinux_pal_cpu_topology(unsigned int *topo, unsigned int size)
{
	int ret;

	if (topo == NULL) {
		return -EINVAL;
	}

	ret = hm_sysctrl_read_conf(__SYSCONF_CPU_TOPOLOGY, (void *)topo, size);
	if (ret < 0) {
		panic("devhost: get cpu topology failed, ret=%s\n", hmstrerror(ret));
	}

	return 0;
}

int liblinux_pal_thread_setaffinity(const liblinux_pal_thread_handle_t *handle,
				    unsigned long mask)
{
	int ret;
	cpu_set_t cpuset = {0};
	raw_thread_t td;
	int tid = 0;

	cpuset.__bits[0] = mask;
	if (handle != NULL) {
		td = ulong_to_ptr((unsigned long)handle->handle,
				  struct raw_thread);
		tid = raw_thread_tid_of(td);
	}

	do {
		ret = hm_thread_sched_setaffinity(tid, sizeof(cpuset), &cpuset);
	} while (ret == E_HM_AGAIN);

	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int liblinux_pal_thread_getaffinity(const liblinux_pal_thread_handle_t *handle,
				    unsigned long *mask)
{
	int ret = E_HM_OK;
	cpu_set_t cpuset;
	raw_thread_t td;

	if (mask == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (handle == NULL) {
			/* get current tcb's affinity */
			ret = hm_thread_sched_getaffinity(0, sizeof(cpuset), &cpuset);
		} else {
			td = ulong_to_ptr((unsigned long)handle->handle,
					  struct raw_thread);
			ret = hm_thread_sched_getaffinity(raw_thread_tid_of(td),
							  sizeof(cpuset), &cpuset);
		}
	}

	if (ret < 0) {
		return -hmerrno2posix(ret);
	} else {
		*mask = cpuset.__bits[0];
	}

	return 0;
}

int liblinux_pal_sched_setaffinity(int tid, unsigned long mask)
{
	int ret;
	cpu_set_t cpuset = {0};

	cpuset.__bits[0] = mask;
	do {
		ret = hm_thread_sched_setaffinity(tid, sizeof(cpuset), &cpuset);
	} while (ret == E_HM_AGAIN);

	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int liblinux_pal_sched_getaffinity(int tid, unsigned long *mask)
{
	int ret = E_HM_OK;
	cpu_set_t cpuset;

	if (mask == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_thread_sched_getaffinity(tid, sizeof(cpuset), &cpuset);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	} else {
		*mask = cpuset.__bits[0];
	}

	return 0;
}

int liblinux_pal_thread_setscheduler(const liblinux_pal_thread_handle_t *handle,
				     int prio, int policy)
{
	int ret;
	int tid;
	struct sched_param param_user;

	if (handle == NULL) {
		tid = 0;
	} else {
		tid = raw_thread_tid_of(ulong_to_ptr((unsigned long)handle->handle,
						     struct raw_thread));
	}

	if (prio > HM_PRIORITY_SCHED_USER_RT_MAX) {
		hm_info("prio(%d) of tid=%d reach limit\n", prio, tid);
		prio = HM_PRIORITY_SCHED_USER_RT_MAX;
	}

	param_user.sched_priority = prio;
	ret = hm_thread_sched_setscheduler(tid, policy, &param_user);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

void *liblinux_pal_preempt_init(void)
{
	return preempt_process_info();
}

void liblinux_pal_preempt_resched(void)
{
	preempt_module_resched();
}

void liblinux_pal_preempt_enable(void)
{
	int ret;

	if (devhost_preempt_bypassed()) {
		return;
	}
	ret = preempt_process_enable();
	BUG_ON((ret < 0) && (ret != E_HM_OBJEXIST));
}

void liblinux_pal_preempt_disable(void)
{
	int ret;

	if (devhost_preempt_bypassed()) {
		return;
	}
	ret = preempt_process_disable();
	BUG_ON(ret < 0);
}

unsigned int liblinux_pal_processor_id(void)
{
	int cpu;

	cpu = preempt_process_cpu_get();
	BUG_ON(cpu < 0);

	return (unsigned int)cpu;
}

static __thread struct irq_stat_s {
	int cpu;
	int disabled;
} irq_stat = {.cpu = -1, .disabled = 0};

static __thread int preempt_cpu = -1;

void liblinux_pal_local_irq_enable(void)
{
	int cpu;

	if (devhost_preempt_bypassed()) {
		return;
	}

	struct irq_stat_s *irq = &irq_stat;
	if (irq->disabled) {
		cpu = preempt_process_enable();
		BUG_ON(irq->cpu < 0 || cpu != irq->cpu);
		*irq = (struct irq_stat_s){.disabled = 0, .cpu = -1};
		preempt_cpu = -1;
	}
}

void liblinux_pal_local_irq_disable(void)
{
	(void)liblinux_pal_local_irq_save();
}

int liblinux_pal_local_irq_save(void)
{
	int cpu;
	int old_state;

	if (devhost_preempt_bypassed()) {
		return 0;
	}

	struct irq_stat_s *irq = &irq_stat;
	old_state = (int)irq->disabled;
	if (!old_state) {
		cpu = preempt_process_disable();
		BUG_ON(preempt_cpu != -1, "cpu:%d, preempt_cpu:%d, irq.cpu:%d, irq.disabled:%d\n",
				cpu, preempt_cpu, irq->cpu, irq->disabled);
		*irq = (struct irq_stat_s){.disabled = 1, .cpu = cpu};
		preempt_cpu = cpu;
	}

	return old_state;
}

void liblinux_pal_local_irq_restore(int disabled)
{
	int cpu;

	if (devhost_preempt_bypassed()) {
		return;
	}

	struct irq_stat_s *irq = &irq_stat;
	BUG_ON(!irq->disabled);

	if (disabled == 0) {
		cpu = preempt_process_enable();
		BUG_ON(cpu != irq->cpu, "cpu:%d, preempt_cpu:%d, irq.cpu:%d, irq.disabled:%d\n",
				cpu, preempt_cpu, irq->cpu, irq->disabled);
		*irq = (struct irq_stat_s){.disabled = 0, .cpu = -1};
		preempt_cpu = -1;
	}
}

int liblinux_pal_local_save_flags(void)
{
	if (devhost_preempt_bypassed()) {
		return 1;
	}

	return irq_stat.disabled;
}

int liblinux_pal_irq_set_affinity_hint(unsigned long linux_irq, unsigned long mask)
{
	return devhost_irq_setaffinity_hint((unsigned int)linux_irq, mask);
}

int liblinux_pal_is_thread_ctx(void)
{
	int ret;
	struct sysarg_thread_stat stat = {0};

	ret = hm_thread_stat(0, &stat, THREAD_STAT_MIGRATING);
	BUG_ON(ret < 0);

	return !((bool)stat.tcb_is_migrating);
}

void liblinux_pal_process_set_slv(unsigned int cpu, unsigned int slv)
{
	int ret = preempt_process_set_slv(cpu, slv);
	if (ret < 0) {
		dh_error("devhost: preempt process set slv failed, cpu: %u, slv: %u, ret=%s\n", cpu, slv, hmstrerror(ret));
	}
}

void liblinux_pal_thread_set_slv(const liblinux_pal_thread_handle_t *handle, unsigned int slv)
{
	int ret;
	pid_t tid = 0;

	if (handle != NULL) {
		raw_thread_t td = ulong_to_ptr((unsigned long)handle->handle, struct raw_thread);
		tid = raw_thread_tid_of(td);
	}

	ret = preempt_thread_set_slv(tid, slv);
	if (ret < 0) {
		dh_error("devhost: preempt thread set slv failed, tid:%d, slv: %u, ret=%s\n", tid, slv, hmstrerror(ret));
	}
}

void liblinux_pal_thread_dump(void)
{
	int ret;
	unsigned int flags = 0;

	dumpstack_set_flag(flags, DUMPSTACK_FLAGS_ELF_LOAD_INFO);
	dumpstack_set_flag(flags, DUMPSTACK_FLAGS_STACK_BACKTRACE);
	dumpstack_set_flag(flags, DUMPSTACK_FLAGS_SYMBOL_NO_SYMBOL);

	ret = hm_dump_thread_to_klog(0, flags);
	if (ret < 0) {
		dh_error("dump thread failed, ret=%s\n", hmstrerror(ret));
	}
}

/* Check pid and sid of terminal caller process and return sid */
static int terminal_caller_process_check(void)
{
	int pid;
	int sid;

	sid = devhost_mysid();
	if (sid < 0) {
		dh_error("terminal acquire sid failed, err=%d\n", sid);
		return sid;
	}

	pid = devhost_current_pid();
	if (pid < 0) {
		dh_error("terminal acquire pid failed, err=%d\n", pid);
		return pid;
	}
	/* not session leader, no permission to control terminal */
	if (pid != sid) {
		dh_debug("terminal caller not session leader, sid: %d, pid: %d\n", sid, pid);
		return -EPERM;
	}
	return sid;
}

#define TERMINAL_PREEMPTED_BY_ROOT 1
static int terminal_preempt_check(struct tty_info *info, int preempt)
{
	/* Current tty has not been controled */
	if (info->tty_sid == 0) {
		return 0;
	}
	/*
	 * If tty has already been controled, only root process can preempt
	 * it when arg equals to 1
	 */
	if ((preempt == TERMINAL_PREEMPTED_BY_ROOT)) {
		if (devhost_caller_has_ab_and_cap((int)DH_ABILITY_TTY, (int)DH_CAP_ADMIN) == 0) {
			return 0;
		}
	}
	dh_warn("tty has already been controled by other process, and "
		 "caller is not root, other sid = %d, preempt = %d\n", info->tty_sid, preempt);
	return -EPERM;
}

int liblinux_pal_set_control_terminal(struct tty_info *info, int preempt)
{
	int pid;
	int sid;
	int ret;

	BUG_ON(info->tty_name == NULL);

	sid = terminal_caller_process_check();
	if (sid < 0) {
		return sid;
	}
	pid = sid;

	if (sid == info->tty_sid) {
		return 0;
	}

	ret = terminal_preempt_check(info, preempt);
	if (ret < 0) {
		return ret;
	}

	if (info->f_mode == 0 && (devhost_caller_has_ab_and_cap((int)DH_ABILITY_TTY, (int)DH_CAP_ADMIN) < 0)) {
		dh_debug("process only has write access and no capbility\n");
		return -EPERM;
	}

	ret = devhost_setfgpgrp(sid, pid);
	if (ret != 0) {
		dh_error("terminal set pgrp failed, sid: %d, pid: %d, err=%d\n", sid, pid, ret);
		return ret;
	}

	ret = devhost_session_set_ttyinfo(sid, info->tty_name, info->tty_dev_no);
	if (ret != 0) {
		dh_error("terminal set session tty name failed, sid: %d, err=%d\n", sid, ret);
		return ret;
	}

	return 0;
}

int liblinux_pal_get_sid(void)
{
	int sid;

	sid = devhost_mysid();
	if (sid < 0) {
		dh_error("get sid failed, err=%d\n", sid);
	}
	return sid;
}

int liblinux_pal_get_pgid(void)
{
	int pgid;

	pgid = devhost_current_pgid();
	if (pgid < 0) {
		dh_error("get pgid failed, err=%d\n", pgid);
	}
	return pgid;
}

int liblinux_pal_set_pgrp(int tty_sid, int pgrp)
{
	int ret;

	if (tty_sid < 0 || pgrp < 0) {
		dh_error("set pgrp arg invalid, tty sid: %d, pgrp: %d\n", tty_sid, pgrp);
		return -EINVAL;
	}

	ret = devhost_setfgpgrp(tty_sid, pgrp);
	if (ret != 0) {
		dh_error("set pgrp failed, tty sid: %d, pgrp: %d, err=%d\n", tty_sid, pgrp, ret);
		return ret;
	}
	return 0;
}

int liblinux_pal_get_pgrp(int tty_sid)
{
	int pgrp;

	if (tty_sid < 0) {
		dh_error("get pgrp arg invalid, tty sid: %d\n", tty_sid);
		return -EINVAL;
	}

	pgrp = devhost_getfgpgrp(tty_sid);
	if (pgrp < 0) {
		dh_error("get pgrp failed, tty sid: %d, err=%d\n", tty_sid, pgrp);
	}
	return pgrp;
}

void liblinux_pal_process_signal(int sid, int sig)
{
	if (sig <= 0 || sig > SIGRTMAX) {
		dh_error("signal %d exceeds expected value\n", sig);
		return;
	}

	devhost_kill_process_group(sid, sig);
}

void liblinux_pal_process_signal_with_pid(int pid, int sig)
{
	devhost_kill_process(pid, sig);
}

int liblinux_pal_signal_isignored(int pid, int sig)
{
	int ret = 0;
	ret = hm_signal_check_ignore(pid, sig);
	if (ret < 0) {
		return 0;
	}

	return ret;
}

int liblinux_pal_current_pgrp_isorphaned(void)
{
	int ret = 0;
	ret = hm_proc_current_pgrp_isorphaned();
	if (ret < 0) {
		ret = 1;
	}

	return ret;
}

void liblinux_pal_sys_shutdown(void)
{
	dh_info("liblinux will shutdown system\n");
	(void)devhost_shutdown();
}

void liblinux_pal_sys_snapshot(void)
{
	dh_info("liblinux will trigger snapshot\n");
	(void)devhost_snapshot();
}

int liblinux_pal_thread_inactive(const liblinux_pal_thread_handle_t *handle)
{
	int ret = 0;
	raw_thread_t td;
	struct sysarg_thread_stat stat;

	td = ulong_to_ptr((unsigned long)handle->handle, struct raw_thread);
	BUG_ON(td == NULL);
	mem_zero_s(stat);

	if (hm_thread_stat(raw_thread_cref_of(td), &stat, THREAD_STAT_STATE) == E_HM_OK) {
		switch (stat.tcb_state) {
		case __TCB_STATE_READY:
		case __TCB_STATE_RUNNING:
			break;
		case __TCB_STATE_BLOCKED:
		case __TCB_STATE_DEAD:
		case __TCB_STATE_UNKNOWN:
		default:
			ret = 1;
			break;
		}
	}
	return ret;
}

void liblinux_pal_fpu_save(void)
{
	hm_actv_fpu_begin();
}

void liblinux_pal_fpu_restore(void)
{
	hm_actv_fpu_end();
}

int liblinux_pal_get_user_pages(unsigned long vaddr, size_t len, unsigned long long *ret_pa_array,
				size_t array_size)
{
	return hm_umem_get_user_pages(true, vaddr, len, ret_pa_array, array_size);
}

int liblinux_pal_put_user_pages(unsigned long vaddr, size_t len, unsigned long long *ret_pa_array,
				size_t array_size)
{
	return hm_umem_put_user_pages(true, vaddr, len, ret_pa_array, array_size);
}

int liblinux_pal_getpriority_by_tid(int tid)
{
	int ret = E_HM_OK;
	struct sched_param param = {0};

	ret = hm_thread_sched_getparam(tid, &param);
	if (ret < 0) {
		dh_info("acquire param of priority failed\n");
		return -hmerrno2posix(ret);
	}

	/* ret must be a legal priority here, so forced it to int */
	return (int)param.sched_priority;
}

int liblinux_pal_set_user_nice(int tid, int nice)
{
	return -hmerrno2posix(hm_thread_setpriority(PRIO_PROCESS, tid, nice));
}

int liblinux_pal_query_main_thread(int tid, int *main_thread_tid, char *main_thread_name, unsigned int name_len)
{
	struct __actvret_hmcall_proc_query_main_thread actv_ret;
	int ret;

	if (main_thread_tid == NULL || main_thread_name == NULL)
		return -hmerrno2posix(E_HM_INVAL);

	mem_zero_s(actv_ret);
	ret = actvcall_hmcall_proc_query_main_thread(tid, main_thread_name, name_len, &actv_ret);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}
	*main_thread_tid = actv_ret.main_tid;
	return 0;
}

uint64_t liblinux_pal_get_current_signals(void)
{
	return hm_actv_local_get_raising_hmsignals();
}

uint64_t liblinux_pal_get_pending_signals(void)
{
	int rc;
	struct __actvret_hmcall_signal_get_pending_signal pret;

	rc = actvxactcall_hmcall_signal_get_pending_signal(true, true, &pret);
	if (rc != E_HM_OK) {
		dh_error("get pending signals failed, rc=%s\n", hmstrerror(rc));
		return 0;
	}
	return (uint64_t)pret.pending_signal;
}

int liblinux_pal_thread_set_vip_prio(const liblinux_pal_thread_handle_t *handle,
		int prio)
{
	int ret;
	int tid;

	if (handle == NULL) {
		tid = 0;
	} else {
		tid = raw_thread_tid_of(ulong_to_ptr((unsigned long)handle->handle,
					struct raw_thread));
	}
	ret = actvcall_hmcall_thread_set_vip_prio(tid, prio);
	if (ret != E_HM_OK) {
		dh_error("devhost set_vip_prio failed, err=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

void liblinux_pal_thread_set_power_group(const liblinux_pal_thread_handle_t *handle)
{
	int tid = raw_thread_tid_of(ulong_to_ptr((unsigned long)handle->handle,
						 struct raw_thread));
	int ret = hm_thread_set_lt_group(tid, SCHED_LT_POWER_GROUP);
	if (ret != 0) {
		dh_error("hm_thread_set_lt_group failed, tid=%d, ret=%s\n", tid, hmstrerror(ret));
	}
}

/*
 * default boost value(0) does not trigger frequency
 * valid boost range is (0, 100]
 */
void liblinux_pal_thread_set_task_boost(const liblinux_pal_thread_handle_t *handle, int boost)
{
	int tid = raw_thread_tid_of(ulong_to_ptr((unsigned long)handle->handle,
						 struct raw_thread));
	int ret = hm_thread_set_lt_boost(tid, boost);
	if (ret != 0) {
		dh_error("hm_thread_set_lt_boost failed, tid=%d, ret=%s\n", tid, hmstrerror(ret));
	}
}

void liblinux_pal_thread_set_pin_preempted(void)
{
	int ret = hm_thread_set_pin_preempted();
	if (ret != 0) {
		dh_error("hm_thread_set_pin_preempted, ret=%s\n", hmstrerror(ret));
	}
}

void liblinux_pal_set_iowait(void)
{
	hm_actv_local_set_iowait();
	hm_actv_local_set_block_info((unsigned long)__builtin_return_address(0));
}

void liblinux_pal_clr_iowait(void)
{
	hm_actv_local_clr_iowait();
}

int liblinux_pal_proc_lock_pid(int pid)
{
	int rc = E_HM_OK;

	rc = actvxactcall_hmcall_proc_lock_pid(true, true, (pid_t)pid, false /* get */);
	if (rc != E_HM_OK) {
		dh_error("lock pid %d failed: %s\n", pid, hmstrerror(rc));
		return -hmerrno2posix(rc);
	}
	return 0;
}

int liblinux_pal_proc_unlock_pid(int pid)
{
	int rc = E_HM_OK;

	rc = actvxactcall_hmcall_proc_lock_pid(true, true, (pid_t)pid, true /* put */);
	if (rc != E_HM_OK) {
		dh_error("unlock pid %d failed: %s\n", pid, hmstrerror(rc));
		return -hmerrno2posix(rc);
	}
	return 0;
}

int liblinux_pal_thread_set_freezable(void)
{
	return hm_thread_set_freezable();
}

bool liblinux_pal_thread_freezing(void)
{
	return hm_thread_freezing();
}

int liblinux_pal_thread_try_to_freeze(void)
{
	return hm_thread_try_to_freeze();
}

void liblinux_pal_thread_get_priority(int *rt_prio, int *nice)
{
	unsigned int prio = hm_actv_local_thread_priority_of();

	if (!rt_prio || !nice) {
		return;
	}

	if (prio <= HM_PRIORITY_SCHED_RT_OFFSET) {
		*rt_prio = 0;
		*nice = ices_priority_to_nice((int)prio);
	} else {
		*rt_prio = prio - HM_PRIORITY_SCHED_RT_OFFSET;
	}
}

int liblinux_pal_get_current_exec_path(char *buf, size_t len)
{
	int ret = hm_proc_current_exec_path(buf, len);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

pid_t liblinux_pal_thread_vtid_to_root_tid(pid_t tid)
{
	return hm_proc_vtid_to_root_tid(tid);
}
