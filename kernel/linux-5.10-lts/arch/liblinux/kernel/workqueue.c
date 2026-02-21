// SPDX-License-Identifier: GPL-2.0-only

#include <linux/sched.h>
#include <linux/hashtable.h>
#include <linux/module.h>
#include <trace/hooks/liblinux.h>

#include "workqueue_internal.h"

static void hook_unbind_workers(void* __data, struct worker_pool *pool, int cpu, struct mutex *lock, struct list_head *workers)
{
	struct cpumask targets_mask;
	struct worker *worker;

	cpumask_copy(&targets_mask, cpu_online_mask);
	cpumask_clear_cpu(cpu, &targets_mask);
	list_for_each_entry((worker), workers, node)
		if (({ lockdep_assert_held(lock); false; })) { }
		else
			set_cpus_allowed_ptr(worker->task, &targets_mask);
}
INIT_VENDOR_HOOK(ldk_rvh_unbind_workers, hook_unbind_workers);

static unsigned int worker_pool_caching_nr = 10;
module_param(worker_pool_caching_nr, uint, S_IRUGO | S_IWUSR);

static void workers_need_caching(void *__data, unsigned int nr_workers, bool *need_caching)
{
	if (nr_workers <= worker_pool_caching_nr)
		*need_caching = true;
	else
		*need_caching = false;
}
INIT_VENDOR_HOOK(ldk_rvh_too_many_workers, workers_need_caching);
