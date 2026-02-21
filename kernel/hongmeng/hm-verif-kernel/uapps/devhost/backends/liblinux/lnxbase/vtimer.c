/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: vtimer pal interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 14 11:36:50 2023
 */
#include <limits.h>
#include <sched.h>
#include <hongmeng/errno.h>
#include <hmkernel/kvic_timer_module.h>
#include <hmkernel/cpuset.h>
#include <hmkernel/sched/sched.h>
#include <hmkernel/sched_module.h>
#include <hmkernel/sched/priority.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/raw_sem.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmsrv_sys/hm_proc_affinity.h>
#include <devhost/log.h>
#include <libpreempt/preempt.h>
#include "../../../utils/utils.h"
#include "../../../devhost_backend.h"

#include <lnxbase/lnxbase.h>

struct vtimer_handle_args {
	event_handler_p	vtimer_handler_func;
	thread_init_p	vtimer_thread_init_func;
	struct raw_sem *create_thread_sem_ptr;
	cref_t *cref_ptr;
	unsigned int	cpu;
};

static try_stop_handler_p vtimer_try_stop_fn = NULL;
void lnxbase_enable_stop_tick(try_stop_handler_p stop_func)
{
	vtimer_try_stop_fn = stop_func;
}

static rref_t module_rref;
struct raw_sem g_clock_register_sem_ptr;

static rref_t __kvic_timer_cref_init()
{
	int ret = E_HM_OK;
	rref_t init_rref;

	init_rref = hm_module_open(KVIC_TIMER_MODULE_NAME);
	if (IS_REF_ERR(init_rref)) {
		return ERR_TO_REF(E_HM_NODEV);
	}

	ret = hm_module_ioctl(init_rref, __KVIC_TIMER_ALLOC, NULL, 0, NULL, 0);
	if (ret < 0) {
		(void)hm_module_close(init_rref);
		return ERR_TO_REF(ret);
	}

	return init_rref;
}

static int clock_get_monotonic_time(struct timespec *ts)
{
	int ret = E_HM_OK;
	ret = hm_clock_gettime(CLOCK_MONOTONIC, ts);
	if (ret != E_HM_OK) {
		return ret;
	}
	ts->tv_sec += 1;
	return ret;
}

static int prepare_cref_and_post_sem(struct vtimer_handle_args *handler_args)
{
	int ret = E_HM_OK;
	char name[HMCAP_THREAD_NAME_SIZE];
	unsigned int cpu = handler_args->cpu;

	cpu_set_t cpuset = {0};
	cpuset.__bits[0] = (unsigned long)__cpuset_one(cpu).__bits;
	ret = hm_thread_sched_setaffinity(0, sizeof(cpuset), &cpuset);
	if (ret != E_HM_OK) {
		dh_warn("failed to setaffinity of cpu %u\n", cpu);
		*handler_args->cref_ptr = ERR_TO_REF(ret);
		(void)raw_sem_post(handler_args->create_thread_sem_ptr);
		return ret;
	}

	NOFAIL(!(sprintf_s(name, HMCAP_THREAD_NAME_SIZE, "kevtd/%u", cpu) > 0));
	(void)hm_thread_setname(0, name, NULL);

	*handler_args->cref_ptr = hm_thread_effective_cref();
	(void)raw_sem_post(handler_args->create_thread_sem_ptr);

	return ret;
}

static int vtimer_set_kevtd_sched(void)
{
	int ret = E_HM_OK;
#ifdef CONFIG_KEVTD_SCHED_RR
	struct sched_param param;

	mem_zero_s(param);
	ret = hm_thread_sched_getparam(0, &param);
	if (ret < 0) {
		dh_warn("hm_thread_sched_getparam failed: %s\n", hmstrerror(ret));
		return ret;
	}

	param.sched_priority = HM_PRIORITY_DH_KEVTD;
	ret = hm_thread_sched_setscheduler(0, SCHED_RR, &param);
	if (ret < 0) {
		dh_warn("hm_thread_sched_setscheduler failed: %s\n", hmstrerror(ret));
		return ret;
	}
#endif
	return ret;
}

static void *__vtimer_thread_fn(void *args)
{
	int ret = E_HM_OK;
	struct vtimer_handle_args *handler_args = (struct vtimer_handle_args *)args;
	unsigned int cpu = handler_args->cpu;
	event_handler_p handler_fn = handler_args->vtimer_handler_func;
	thread_init_p thread_init_fn = handler_args->vtimer_thread_init_func;
	struct timespec timeout;

	backend_ctx_setup();
	ret = prepare_cref_and_post_sem(handler_args);
	if (ret != E_HM_OK) {
		return NULL;
	}

	ret = clock_get_monotonic_time(&timeout);
	BUG_ON(ret != E_HM_OK);
	ret = raw_sem_timedwait(&g_clock_register_sem_ptr, &timeout);
	BUG_ON(ret != E_HM_OK);

	ret = thread_init_fn(cpu);
	BUG_ON(ret != 0);

	ret = vtimer_set_kevtd_sched();
	BUG_ON(ret != E_HM_OK);

	/*
	 * Mark kevtd as an irq tcb for schedule, this will make scheduler try to run kevtd first.
	 * Specially, kevtd will access devhost resources with preempt disabled within it's init_fn.
	 * If we mark kevtd before init_fn, it will always do retry since resources is not ready yet,
	 * and this will trigger devhost hungry finally. So we mark kevtd after it's inited.
	 */
	hmsrv_update_sched_class_module_rref();
	(void)hm_module_ioctl(thread_module_sched_ref(), __SCHED_MARK_IRQ, NULL, 0, NULL, 0);

	while (1) {
		ret = hm_trigger_wait(1, NULL);
		if (ret < 0) {
			continue;
		}
		handler_fn(cpu);
		if (vtimer_try_stop_fn != NULL) {
			vtimer_try_stop_fn(cpu);
		}
	}
	return NULL;
}

static void __async_create_thread_work(void *data)
{
	int ret = E_HM_OK;
	raw_thread_t thread;
	raw_thread_attr_t attr;
	struct vtimer_handle_args *_data = (struct vtimer_handle_args *)data;
	unsigned int cpu = _data->cpu;

	ret = raw_thread_attr_init(&attr);
	if (ret == E_HM_OK) {
		int err;
		int tid;

		ret = raw_thread_create(&thread, &attr, __vtimer_thread_fn, data);
		tid = raw_thread_tid_of(thread);
		err = hm_thread_set_lt_group(tid, SCHED_LT_POWER_GROUP);
		if (err != 0) {
			dh_error("hm_thread_set_lt_group failed, tid=%d, ret=%s\n", tid, hmstrerror(ret));
		}
	}
	if (ret != E_HM_OK) {
		*_data->cref_ptr = ERR_TO_REF(ret);
		(void)raw_sem_post(_data->create_thread_sem_ptr);
		dh_error("create raw thread %u failed, err=%s\n", cpu, hmstrerror(ret));
	}
}

static int semaphores_init(struct raw_sem *create_thread_sem_p)
{
	int ret = E_HM_OK;

	ret = raw_sem_init(&g_clock_register_sem_ptr, 0);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = raw_sem_init(create_thread_sem_p, 0);
	if (ret != E_HM_OK) {
		return ret;
	}

	return ret;
}

static int semaphore_timedwait_and_check(struct raw_sem *wait_sem_p, cref_t *crefs,
					 struct vtimer_handle_args *args, unsigned int cpu_nr)
{
	int ret = E_HM_OK;
	struct timespec timeout = { 0 };
	unsigned int idx;
	bool is_wait_err = false;

	ret = clock_get_monotonic_time(&timeout);
	if (ret != E_HM_OK) {
		return ret;
	}

	for (idx = 0; idx < cpu_nr; idx++) {
		ret = raw_sem_timedwait(wait_sem_p, &timeout);
		is_wait_err = is_wait_err || ((ret != E_HM_OK) ? true : false);
	}
	if (is_wait_err) {
		return E_HM_TIMEDOUT;
	}

	for (idx = 0; idx < cpu_nr; idx++) {
		if (IS_REF_ERR(crefs[args[idx].cpu])) {
			ret = REF_TO_ERR(crefs[args[idx].cpu]);
			return ret;
		}
	}

	return ret;
}

static int get_cpu_info_and_init_sems(unsigned int *config_cpu_nr_ptr,
				      struct raw_sem *create_thread_sem_p)
{
	int ret = E_HM_OK;
	unsigned long config_cpu_nr = 0UL;

	ret = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, &config_cpu_nr, sizeof(config_cpu_nr));
	if ((ret < 0) || (config_cpu_nr >= UINT_MAX)) {
		return E_HM_INVAL;
	}
	*config_cpu_nr_ptr = (unsigned int)config_cpu_nr;

	ret = semaphores_init(create_thread_sem_p);
	if (ret != E_HM_OK) {
		return ret;
	}

	return ret;
}

static int __lnxbase_vtimer_init(event_handler_p handler_func, thread_init_p init_func,
				 rref_t init_rref, uint32_t cpu_mask)
{
	int ret = 0;
	unsigned int idx;
	struct raw_sem create_thread_sem;
	struct vtimer_handle_args *args = NULL;
	cref_t *crefs = NULL;
	__cpuset_t cpuset;
	unsigned int cpu_nr;
	unsigned int config_cpu_nr;

	ret = get_cpu_info_and_init_sems(&config_cpu_nr, &create_thread_sem);
	if (ret < 0) {
		return ret;
	}
	cpuset.__bits = (__u32)cpu_mask;
	cpu_nr = __cpuset_weight(cpuset);
	if (cpu_nr > config_cpu_nr) {
		return E_HM_INVAL;
	}

	args = (struct vtimer_handle_args *)malloc(cpu_nr * sizeof(struct vtimer_handle_args));
	if (args == NULL) {
		return E_HM_NOMEM;
	}

	crefs = (cref_t *)calloc(config_cpu_nr, sizeof(cref_t));
	if (crefs == NULL) {
		ret = E_HM_NOMEM;
		goto err;
	}

	for (idx = 0; idx < cpu_nr; idx++) {
		args[idx].vtimer_handler_func = handler_func;
		args[idx].vtimer_thread_init_func = init_func;
		args[idx].create_thread_sem_ptr = &create_thread_sem;
		args[idx].cpu = (unsigned int)__cpuset_first_cpu(cpuset);
		cpuset = __cpuset_del(cpuset, args[idx].cpu);
		args[idx].cref_ptr = &crefs[args[idx].cpu];

		/* cannot create child thread in actv context */
		devhost_async_schedule(__async_create_thread_work, &args[idx]);
	}

	ret = semaphore_timedwait_and_check(&create_thread_sem, crefs, args, cpu_nr);
	if (ret != E_HM_OK) {
		goto err;
	}

	ret = hm_module_ioctl(init_rref, __KVIC_TIMER_BIND, crefs, (size_t)(config_cpu_nr * sizeof(cref_t)), NULL, 0);
	if (ret == E_HM_OK) {
		module_rref = init_rref;
	}

	for (idx = 0; idx < cpu_nr; idx++) {
		(void)raw_sem_post(&g_clock_register_sem_ptr);
	}

err:
	free(crefs);
	free(args);
	return ret;
}

int lnxbase_vtimer_init(event_handler_p handler_func, thread_init_p init_func,
			uint32_t cpu_mask)
{
	int ret = E_HM_OK;
	rref_t init_rref;

	if ((handler_func == NULL) || (init_func == NULL) || (cpu_mask == 0U)) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	init_rref = __kvic_timer_cref_init();
	if (IS_REF_ERR(init_rref)) {
		return -hmerrno2posix(REF_TO_ERR(init_rref));
	}

	ret = __lnxbase_vtimer_init(handler_func, init_func, init_rref, cpu_mask);
	if (ret != E_HM_OK) {
		(void)hm_module_close(init_rref);
	}

	return -hmerrno2posix(ret);
}

static long long expire_ns_to_ts(int expires_ns)
{
	unsigned long long tock;
	long long timeout = (long long)expires_ns;
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 0;

	if (expires_ns >= 0) {
		ts.tv_sec += (long)(expires_ns / NSEC_PER_SEC);
		ts.tv_nsec += (long)(expires_ns % NSEC_PER_SEC);

		(void)timespec_to_values(&ts, &tock);
		timeout = (long long)tock;
	}

	return timeout;
}

int lnxbase_vtimer_set_next_event(int expires_ns)
{
	int ret = E_HM_OK;
	long long timeout = -1;

	timeout = expire_ns_to_ts(expires_ns);

	if (IS_REF_ERR(module_rref)) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	ret = hm_module_ioctl(module_rref, __KVIC_TIMER_SET_NEXT_EVENT,
			      &timeout, sizeof(timeout), NULL, 0);

	return -hmerrno2posix(ret);
}
