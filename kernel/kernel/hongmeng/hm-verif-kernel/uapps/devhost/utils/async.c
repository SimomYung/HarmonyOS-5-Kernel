/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Utils for async workqeue
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 17 15:01:02 2019
 */
#include "utils.h"

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_workqueue.h>
#include <libhmsrv_sys/hm_procmgr.h> /* for 'hm_getpid()' */
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_thread.h>

#include <libdevhost/devhost.h>
#include <libpreempt/preempt.h>

#include "devhost.h"
#include <devhost/log.h>
#ifdef CONFIG_NET_FUSION
#include <devhost/workqueue.h>
#endif

#define DEVHOST_WQ_THREAD_NR	1
/* Reserve 2 bytes for adding serial number for workq thread name */
#define DH_WQ_TNAME_MAX_LEN	(RAW_THREADPOOL_NAME_LEN - 2)

static struct raw_workqueue g_default_wq;
static struct raw_workqueue g_smpcall_wq;

struct invoke_args {
	void (*callback)(void *);
	void *data;
	unsigned int cpu;
};

struct async_work {
	struct raw_work work;
	devhost_async_func_t func;
	void *data;
};

struct call_smpcall_wq_data {
	struct invoke_args args;
	struct async_work awork;
	bool use;
};

static struct call_smpcall_wq_data *g_smpcall_wq_data = NULL;
static unsigned long config_cpu_nr = 0UL;

static inline bool smpcall_wq_in_use(unsigned int cpu)
{
	return g_smpcall_wq_data[cpu].use;
}

static inline void smpcall_wq_invoke_begin(unsigned int cpu)
{
	g_smpcall_wq_data[cpu].use = true;
}

static inline void smpcall_wq_invoke_end(unsigned int cpu)
{
	g_smpcall_wq_data[cpu].use = false;
}

#ifdef CONFIG_NET_FUSION
struct raw_workqueue *devhost_get_actvpool_wq(void)
{
	return &g_default_wq;
}
#endif

int devhost_workq_attr_set_name(struct raw_workqueue_attr *attr, const char *name)
{
	int ret = E_HM_OK;
	int hm_pid;
	char new_name[DH_WQ_TNAME_MAX_LEN] = {0};

	hm_pid = hm_getpid();
	if (hm_pid < 0) {
		dh_debug("get pid failed, err=%d\n", hm_pid);
		return hm_pid;
	}

	ret = snprintf_s(new_name, DH_WQ_TNAME_MAX_LEN,
			 DH_WQ_TNAME_MAX_LEN - 1, "%s/%d",
			 name, hm_pid);
	if (ret < 0) {
		return E_HM_INVAL;
	}

	ret = raw_workqueue_attr_set_name(attr, new_name);
	if (ret < 0) {
		return ret;
	}

	return E_HM_OK;
}

static int devhost_raw_workqueue_init(struct raw_workqueue *wq, const char *name)
{
	struct raw_workqueue_attr attr;
	int ret = E_HM_OK;

	raw_workqueue_attr_init(&attr, DEVHOST_WQ_THREAD_NR, 1); /* nr_queues = 1 */

	ret = devhost_workq_attr_set_name(&attr, name);
	if (ret < 0) {
		dh_warn("set workq thread %s failed, ret=%s\n",
			name, hmstrerror(ret));
		return ret;
	}

	return raw_workqueue_init(wq, &attr);
}

static int default_workqueue_init(void)
{
	return devhost_raw_workqueue_init(&g_default_wq, DH_DEFAULT_WORKQ_TNAME);
}

static int __handle_async_work(struct raw_work *work)
{
	struct async_work *awork = container_of(work, struct async_work, work);

	(*awork->func)(awork->data);

	raw_work_destroy(&awork->work);
	free(awork);

	return 0;
}

static void __smpcall_schedule_helper(void *data)
{
	UNUSED(data);
	struct sched_param param_user = {0};
	int ret = E_HM_OK;

	/*
	 * Simulate the smpcall thread to do __idle_cpu_die asynchronously.
	 * It needs to run even after sched_cpu_dying().
	 */
	ret = preempt_thread_set_slv(0, DH_THREAD_SCHED_LEVEL_MAX);
	if (ret < 0) {
		dh_error("devhost: smpcall wq set slv failed, ret=%s\n", hmstrerror(ret));
	}

	param_user.sched_priority = DH_SMPCALL_PRIO;
	ret = hm_thread_sched_setscheduler(0, SCHED_RR, &param_user);
	if (ret < 0) {
		dh_error("devhost: smpcall wq set schedule failed, ret=%s\n", hmstrerror(ret));
	}
}

/*
 * Use queue_work to ensure that doing schedule_helper in its own thread context,
 * not in the devhost main thread.
 */
static int queue_smpcall_work(struct raw_workqueue *wq)
{
	struct async_work *awork = NULL;
	int ret = E_HM_OK;

	awork = (struct async_work *)malloc(sizeof(*awork));
	if (awork == NULL) {
		dh_error("devhost: alloc memory for smpcall wq failed\n");
		return E_HM_NOMEM;
	}

	(void)raw_work_init(&awork->work, __handle_async_work, 0);
	awork->func = __smpcall_schedule_helper;
	awork->data = NULL;

	ret = raw_workqueue_add_work(wq, &awork->work);
	if (ret < 0) {
		raw_work_destroy(&awork->work);
		free(awork);
	}

	return ret;
}

static int smpcall_workqueue_init(void)
{
	struct raw_workqueue *wq = &g_smpcall_wq;
	unsigned long call_data_size = 0;
	int ret = E_HM_OK;

	ret = devhost_raw_workqueue_init(wq, DH_SMPCALL_WORKQ_TNAME);
	if (ret < 0) {
		return ret;
	}

	ret = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, &config_cpu_nr,
				   sizeof(config_cpu_nr));
	if (ret < 0) {
		dh_debug("get cpu info failed: %s\n", hmstrerror(ret));
		return ret;
	}

	call_data_size = config_cpu_nr * sizeof(struct call_smpcall_wq_data);
	g_smpcall_wq_data = (struct call_smpcall_wq_data *)malloc(call_data_size);
	if (g_smpcall_wq_data == NULL) {
		dh_error("alloc memory for smpcall wq failed, "
			 "config_cpu_nr:%lu, size:%lu\n", config_cpu_nr, call_data_size);
		return E_HM_NOMEM;
	}

	mem_zero_b(g_smpcall_wq_data, call_data_size);

	return queue_smpcall_work(wq);
}

int devhost_workq_init(void)
{
	int ret = E_HM_OK;

	ret = default_workqueue_init();
	if (ret < 0) {
		return ret;
	}

	return smpcall_workqueue_init();
}

void devhost_async_schedule(devhost_async_func_t func, void *data)
{
	struct async_work *awork = NULL;
	int ret = E_HM_OK;

	BUG_ON(func == NULL);

	awork = (struct async_work *)malloc(sizeof(*awork));
	if (awork == NULL) {
		goto try_sync_call;
	}

	(void)raw_work_init(&awork->work, __handle_async_work, 0);
	awork->func = func;
	awork->data = data;

	ret = raw_workqueue_add_work(&g_default_wq, &awork->work);
	if (ret < 0) {
		raw_work_destroy(&awork->work);
		free(awork);
		goto try_sync_call;
	}

	return;

try_sync_call:
	func(data);
}

static int __handle_smpcall_wq_work(struct raw_work *work)
{
	struct async_work *awork = container_of(work, struct async_work, work);
	(*awork->func)(awork->data);
	raw_work_destroy(&awork->work);
	return 0;
}

static void __invoke_helper(void *data)
{
	unsigned long default_mask = (unsigned long)__cpuset_full.__bits;
	struct invoke_args *args = (struct invoke_args *)data;
	cpu_set_t cpuset = {0};
	int ret = 0;

	cpuset.__bits[0] = 1UL << args->cpu;
	ret = hm_thread_sched_setaffinity(0, sizeof(cpuset), &cpuset);
	if (ret == 0) {
		args->callback(args->data);
	}
	if (ret == 0) {
		cpuset.__bits[0] = default_mask;
		ret = hm_thread_sched_setaffinity(0, sizeof(cpuset), &cpuset);
	}
	if (ret < 0) {
		dh_error("invoke on cpu#%u failed, ret=%d\n", args->cpu, ret);
	}

	smpcall_wq_invoke_end(args->cpu);
}

/*
 * The dh-smpcall-wq is used exclusively for cpu-hotplug.
 * It is unnecessary to consider concurrent calls on the same CPU.
 */
void devhost_smpcall_wq_schedule(unsigned int cpu, void (*callback)(void *), void *data)
{
	struct invoke_args *args = NULL;
	struct async_work *awork = NULL;
	int ret = E_HM_OK;

	if (cpu >= config_cpu_nr || g_smpcall_wq_data == NULL) {
		return;
	}

	if (smpcall_wq_in_use(cpu)) {
		dh_error("devhost: last smpcall invoke is not over, cpu=%d\n", cpu);
		return;
	}

	smpcall_wq_invoke_begin(cpu);
	awork = &g_smpcall_wq_data[cpu].awork;
	args = &g_smpcall_wq_data[cpu].args;
	args->callback = callback,
	args->data = data,
	args->cpu = cpu,

	(void)raw_work_init(&awork->work, __handle_smpcall_wq_work, 0);
	awork->func = __invoke_helper;
	awork->data = (void *)args;

	ret = raw_workqueue_add_work(&g_smpcall_wq, &awork->work);
	if (ret < 0) {
		dh_error("devhost: add smpcall wq failed, ret=%s\n", hmstrerror(ret));
		raw_work_destroy(&awork->work);
		smpcall_wq_invoke_end(cpu);
	}
}
