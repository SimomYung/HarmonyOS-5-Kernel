// SPDX-License-Identifier: GPL-2.0-only

#include <linux/sched.h>
#include <linux/hashtable.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <trace/hooks/liblinux.h>
#include <liblinux/pal.h>

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

static unsigned int kworker_thread_caching_thresh = 20;
module_param(kworker_thread_caching_thresh, uint, S_IRUGO | S_IWUSR);

struct cached_kworker {
	bool fresh_worker;
	struct list_head list;
	struct worker kworker;
};

static DEFINE_SPINLOCK(cached_kworker_lock);
static LIST_HEAD(cached_kworker_list);
static unsigned int cached_kworker_nr = 0;

#ifdef CONFIG_KWORKER_CACHE_DEBUG
static unsigned int kworker_create = 0;
static unsigned int kworker_reuse = 0;
static unsigned int kworker_destroy = 0;
static struct proc_dir_entry *g_stat_proc = NULL;

static int kworker_cache_stat_show(struct seq_file *m, void *v)
{
	unsigned int create, reuse, caching, destroyed;
	spin_lock(&cached_kworker_lock);
	create = kworker_create;
	reuse = kworker_reuse;
	caching = cached_kworker_nr;
	destroyed = kworker_destroy;
	spin_unlock(&cached_kworker_lock);

	seq_printf(m, "Kworker cache threshold:\t%u\n", kworker_thread_caching_thresh);
	seq_printf(m, "Kworker created:\t\t%u\n", create);
	seq_printf(m, "Kworker in cache:\t\t%u\n", caching);
	seq_printf(m, "Kworker destoryed:\t\t%u\n", destroyed);
	seq_printf(m, "Kworker reuse times:\t\t%u\n", reuse);

	return 0;
}

static int kworker_cache_state_open(struct inode *inode, struct file *file)
{
	return single_open(file, kworker_cache_stat_show, NULL);
}

static ssize_t kworker_cache_stat_clear(struct file *file, const char __user *buf,
			size_t len, loff_t *pos)
{
	spin_lock(&cached_kworker_lock);
	kworker_create = 0;
	kworker_reuse = 0;
	kworker_destroy = 0;
	spin_unlock(&cached_kworker_lock);

	return len;
}

static const struct proc_ops kworker_cache_ops = {
	.proc_open = kworker_cache_state_open,
	.proc_write = kworker_cache_stat_clear,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static void kworker_cache_stat_procfs_init(void)
{
	g_stat_proc = proc_create("kworker_cache_stat", S_IRUGO | S_IWUSR, NULL, &kworker_cache_ops);
	if (g_stat_proc == NULL) {
		pr_warn_once("Failed to create procfs for kworker cache stat\n");
	}
}
#endif

static void hook_get_cached_kworker(void *data, struct worker **worker, int node, int prep_flag)
{
	struct cached_kworker *c_kworker;

#ifdef CONFIG_KWORKER_CACHE_DEBUG
	if (unlikely(g_stat_proc == NULL)) {
		kworker_cache_stat_procfs_init();
	}
#endif

	spin_lock(&cached_kworker_lock);
	if (!list_empty(&cached_kworker_list)) {
		c_kworker = list_entry(cached_kworker_list.next, struct cached_kworker, list);
		list_del_init(&c_kworker->list);
		cached_kworker_nr -= 1;
#ifdef CONFIG_KWORKER_CACHE_DEBUG
		kworker_reuse += 1;
#endif
		spin_unlock(&cached_kworker_lock);

		c_kworker->fresh_worker = false;
		*worker = &c_kworker->kworker;
	} else {
		spin_unlock(&cached_kworker_lock);

		c_kworker = kzalloc_node(sizeof(struct cached_kworker), GFP_KERNEL, node);
		if (c_kworker) {
			*worker = &c_kworker->kworker;
			c_kworker->kworker.task = NULL;
			c_kworker->fresh_worker = true;
#ifdef CONFIG_KWORKER_CACHE_DEBUG
			spin_lock(&cached_kworker_lock);
			kworker_create += 1;
			spin_unlock(&cached_kworker_lock);
#endif
		} else {
			*worker = NULL;
			return;
		}
	}

	INIT_LIST_HEAD(&c_kworker->kworker.entry);
	INIT_LIST_HEAD(&c_kworker->kworker.scheduled);
	INIT_LIST_HEAD(&c_kworker->kworker.node);
	c_kworker->kworker.flags = prep_flag;
}
INIT_VENDOR_HOOK(ldk_rvh_get_kworker, hook_get_cached_kworker);

static void hook_kworker_exit(void *data, struct worker *worker, int *cached,
			struct worker_pool **pool, void (set_pf_hndlr)(bool))
{
	struct cached_kworker *c_kworker;
	c_kworker = container_of(worker, struct cached_kworker, kworker);

	spin_lock(&cached_kworker_lock);
	if (cached_kworker_nr >= kworker_thread_caching_thresh) {
#ifdef CONFIG_KWORKER_CACHE_DEBUG
		kworker_destroy += 1;
#endif
		spin_unlock(&cached_kworker_lock);

		*cached = 0;
		kfree(c_kworker);
	} else {
		list_add_tail(&c_kworker->list, &cached_kworker_list);
		cached_kworker_nr += 1;
		set_task_comm(worker->task, "kworker/cached");
		kthread_park(worker->task);
		spin_unlock(&cached_kworker_lock);

		*cached = 1;
		/*
		 * Reset the priority of a cached kworker to avoid offering a high priority
		 * kworker thread to a pool with normal priority
		 */
		 liblinux_pal_thread_set_vip_prio(&task_thread_info(worker->task)->thread_handle, 0);
		 worker->task->prio = NICE_TO_PRIO(0);
		 worker->task->rt_priority = 0;
		 worker->task->static_prio = worker->task->prio;
		 worker->task->normal_prio = worker->task->prio;
		 worker->task->policy = SCHED_NORMAL;

		kthread_parkme();
		set_pf_hndlr(true);
		*pool = READ_ONCE(worker->pool);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_kworker_exit, hook_kworker_exit);

static void hook_free_kworker(void *data, struct worker *worker)
{
	struct cached_kworker *c_kworker;

	if (worker != NULL) {
		c_kworker = container_of(worker, struct cached_kworker, kworker);
		kfree(c_kworker);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_free_kworker, hook_free_kworker);

static void hook_wake_cached_kworker(void *data, struct worker *worker)
{
	struct cached_kworker *c_kworker;

	if (worker != NULL) {
		c_kworker = container_of(worker, struct cached_kworker, kworker);
		if (c_kworker->fresh_worker) {
			wake_up_process(worker->task);
		} else {
			kthread_unpark(worker->task);
		}
	}
}
INIT_VENDOR_HOOK(ldk_rvh_wake_cached_kworker, hook_wake_cached_kworker);
