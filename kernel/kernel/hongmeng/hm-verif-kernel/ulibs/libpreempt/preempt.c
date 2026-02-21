/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Interface for preempt disable/enable
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 13 15:12:28 2020
 */
#include <libpreempt/preempt.h>

#include <hmkernel/sched_module.h>
#include <hmkernel/activation/sched.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hmkernel/compiler.h>

#include <libhmactv/actv.h>
#include <libhmlog/hmlog.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhmsync/thread.h>
#include <libhmsync/uev/uev.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhwsecurec/securec.h>
#include <hmasm/current.h>

#ifdef CONFIG_LOCK_TRACE_UEV_LOGGING
#define uev_trace_preempt_log(event) (uev_trace_lock_event(event,	\
					 (uint64_t)(uintptr_t)__builtin_return_address(0), NULL))
#else
#define uev_trace_preempt_log(event) do {} while (0)
#endif

static bool disable_process_preempt_feature_flag = false;

void disable_process_preempt_feature(void)
{
	disable_process_preempt_feature_flag = true;
}

void enable_process_preempt_feature(void)
{
	disable_process_preempt_feature_flag = false;
}

_Bool is_process_preempt_feature_disabled(void)
{
	return disable_process_preempt_feature_flag;
}

void preempt_enable(void)
{
	int err;
	struct sched_preempt_args_s preempt_args;
	uev_trace_preempt_log(LOCKTRACE_EVENT_TYPE_PREEMPT_ENABLE);
	mem_zero_s(preempt_args);
	preempt_args.thread_cref = 0;
	preempt_args.type = PREEMPT_ENABLE;
	err = syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_CONF,
							 &preempt_args, sizeof(preempt_args), NULL, 0);
	if (err < 0) {
		hm_warn_ratelimited("enable preempt return : %s\n", hmstrerror(err));
	}
}

void preempt_disable(void)
{
	int err;
	struct sched_preempt_args_s preempt_args;
	mem_zero_s(preempt_args);
	preempt_args.thread_cref = 0;
	preempt_args.type = PREEMPT_DISABLE;
	err = syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_CONF,
							 &preempt_args, sizeof(preempt_args), NULL, 0);
	if (err < 0) {
		hm_warn_ratelimited("disable preempt return : %s\n", hmstrerror(err));
	}
	uev_trace_preempt_log(LOCKTRACE_EVENT_TYPE_PREEMPT_DISABLE);
}

#define PREEMPT_MAX_CPUS 64
#define PREEMPT_DISABLE_CNT_UNINIT (-128)

/* Per-thread local data for preempt control */
static __thread struct local_control_s local_ctrl = {PREEMPT_DISABLE_CNT_UNINIT, NULL};

static bool registered = false;
static int disabled[PREEMPT_MAX_CPUS] = {0};

struct local_control_s *preempt_process_get_lc(void)
{
	return &local_ctrl;
}

/*
 * Newly created thread will be assigned new tls, at this moment
 * we set its local_ctrl.disable_cnt to unintialized state.
 * */
void preempt_process_lc_reset(void)
{
	local_ctrl.disable_cnt = PREEMPT_DISABLE_CNT_UNINIT;
	local_ctrl.info = NULL;
}

static void __preempt_process_init(void)
{
	BUG_ON(local_ctrl.disable_cnt != PREEMPT_DISABLE_CNT_UNINIT,
	       "disable cnt :%d\n", local_ctrl.disable_cnt);
	local_ctrl.disable_cnt = 0;
	local_ctrl.info = hm_actv_local_sched_of();

	if (unlikely(sched_info_is_inited(local_ctrl.info))) {
		hm_panic("sched info should not be inited before init\n");
	}
	hm_actv_local_set_sched_inited();
}

void preempt_process_init(void)
{
	__preempt_process_init();
}

bool preempt_process_is_registered(void)
{
	return registered;
}

int __hot __weak
preempt_process_enable(void)
{
	int ret;

	if (unlikely(!registered) || disable_process_preempt_feature_flag) {
		return 0;
	}

	struct local_control_s *lc = &local_ctrl;
	OPTIMIZE(lc);

	lc->disable_cnt--;

	ret = (int)__arch_current_cpuid();

	uev_trace_preempt_log(LOCKTRACE_EVENT_TYPE_PREEMPT_PROCESS_ENABLE);
	if (lc->disable_cnt == 0) {
		disabled[ret]--;
		BUG_ON(disabled[ret] < 0, "cpu %d recursive disable cnt: %d\n", ret, disabled[ret]);
		sched_info_set_sched_state(lc->info, SCHED_STATE_PREEMPT_ENABLED);
		if (unlikely(sched_info_need_resched(lc->info))) {
			(void)syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_RESCHED, NULL, 0, NULL, 0);
			sched_info_clear_resched(lc->info);
		}
	} else {
		BUG_ON(lc->disable_cnt < 0, "disable cnt: %d\n", lc->disable_cnt);
	}
	return ret;
}

static int do_preempt_process_disable(struct __sched_info_s *info)
{
	unsigned int cpu;
	/*
	 * If another thread had disabled preemption in this cpu, try again.
	 * If this thread keep running in this cpu, we can know preemption state through
	 * disabled[cpu]. Otherwise, it may be interrupted and loaded to another cpu,
	 * so we need judge sched state again. In this case, kernel will set
	 * SCHED_STATE_PREEMPT_BUSY if someone had disabled preemption in the cpu.
	 */
	do {
		sched_info_set_sched_state(info, SCHED_STATE_PREEMPT_DISABLED);
		barrier();
		cpu = __arch_current_cpuid();
		BUG_ON(cpu >= PREEMPT_MAX_CPUS, "preempt cpu id abnormal: %u\n", cpu);
		if (disabled[cpu] > 0 || sched_info_sched_state_of(info) == SCHED_STATE_PREEMPT_BUSY) {
			(void)syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_RESCHED, NULL, 0, NULL, 0);
			barrier();
			if (sched_info_sched_state_of(info) == SCHED_STATE_SYNCED) {
				break;
			}
		} else {
			/*
			 * If state is not SCHED_STATE_PREEMPT_BUSY here, it means that
			 * SCHED_STATE_PREEMPT_DISABLED flag has been synced to kernel or
			 * this thread keep running in this cpu. In first case, no need to
			 * retry. And in second case, disabled[cpu] is false also means no
			 * need to retry.
			 */
			break;
		}
	} while (true);
	return (int)cpu;
}

int __hot __weak
preempt_process_disable(void)
{
	int cpu;

	if (unlikely(!registered) || disable_process_preempt_feature_flag) {
		return 0;
	}

	struct local_control_s *lc = &local_ctrl;
	OPTIMIZE(lc);

	if (unlikely(lc->info == NULL)) {
		__preempt_process_init();
	}

	if (lc->disable_cnt++ == 0) {
		cpu = do_preempt_process_disable(lc->info);
		disabled[cpu]++;
	} else {
		cpu = (int)__arch_current_cpuid();
	}

	uev_trace_preempt_log(LOCKTRACE_EVENT_TYPE_PREEMPT_PROCESS_DISABLE);
	return cpu;
}

int preempt_process_register(void)
{
	int err = E_HM_OBJEXIST;
	if (!registered) {
		struct sched_preempt_args_s preempt_args;
		mem_zero_s(preempt_args);
		preempt_args.thread_cref = 0;
		preempt_args.type = PREEMPT_PROCESS_REGISTER;
		err = syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_CONF,
								 &preempt_args, sizeof(preempt_args), NULL, 0);
	}
	if (err >= 0) {
		registered = true;
	} else {
		hm_error("register preempt failed: %s\n", hmstrerror(err));
	}
	return err;
}

int preempt_process_unregister(void)
{
	int err = E_HM_OBJEXIST;
	if (registered) {
		struct sched_preempt_args_s preempt_args;
		mem_zero_s(preempt_args);
		preempt_args.thread_cref = 0;
		preempt_args.type = PREEMPT_PROCESS_UNREGISTER;
		err = syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_CONF,
								 &preempt_args, sizeof(preempt_args), NULL, 0);
	}
	if (err >= 0) {
		registered = false;
	} else {
		hm_error("unregister preempt failed: %s\n", hmstrerror(err));
	}
	return err;
}

void preempt_process_reset(void)
{
	registered = false;
	mem_zero_a(disabled);
	hmsrv_update_sched_class_module_rref();
}

int __weak preempt_process_cpu_get(void)
{
	return __arch_current_cpuid();
}

bool preempt_process_feature_enabled(void)
{
	int err;
	struct sched_preempt_args_s preempt_args;
	mem_zero_s(preempt_args);
	preempt_args.thread_cref = 0;
	preempt_args.type = PREEMPT_PROCESS_STAT_QUERY;
	err = syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_CONF,
							 &preempt_args, sizeof(preempt_args), NULL, 0);
	return err == E_HM_OK;
}

/*
 * set current process's sched level at @cpu to @slv
 * if success, only threads with slv >= @slv can run on @cpu afterwards
 * */
int preempt_process_set_slv(unsigned int cpu, unsigned int slv)
{
	int err = E_HM_PERM;
	struct sched_preempt_slv_args_s slv_args;

	if (registered) {
		mem_zero_s(slv_args);
		slv_args.cmd = PREEMPT_SLV_SET_PROCESS;
		slv_args.cpu = cpu;
		slv_args.slv = slv;

		err = syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_SET_SLV,
					 &slv_args, sizeof(slv_args), NULL, 0);
	}

	uev_trace_preempt_log(LOCKTRACE_EVENT_TYPE_PREEMPT_PROCESS_SET_SLV);
	return err;
}

int preempt_thread_set_slv(int tid, unsigned int slv)
{
	int err = E_HM_PERM;
	struct sched_preempt_slv_args_s slv_args;

	if (registered) {
		mem_zero_s(slv_args);
		slv_args.cmd = PREEMPT_SLV_SET_THREAD;
		slv_args.slv = slv;
		slv_args.thread_cref = tid == 0 ? 0ULL : hm_thread_tid2cref(tid);

		err = syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_SET_SLV,
					 &slv_args, sizeof(slv_args), NULL, 0);
	}

	uev_trace_preempt_log(LOCKTRACE_EVENT_TYPE_PREEMPT_THREAD_SET_SLV);
	return err;
}

void *preempt_process_info(void)
{
	void *info = hm_actv_local_sched_of();

	if (unlikely(sched_info_is_inited(info))) {
		hm_info("sched info should not be inited before init\n");
	} else {
		hm_actv_local_set_sched_inited();
	}

	return info;
}

void preempt_module_resched(void)
{
	(void)syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PREEMPT_RESCHED,
				 NULL, 0, NULL, 0);
}

int __weak hm_user_preempt_disable(void)
{
	return preempt_process_disable();
}
 
int __weak hm_user_preempt_enable(void)
{
	return preempt_process_enable();
}
