// SPDX-License-Identifier: GPL-2.0
/*
 * Deferred dmabuf freeing helper
 *
 * Copyright (C) 2020 Linaro, Ltd.
 *
 * Based on the ION page pool code
 * Copyright (C) 2011 Google, Inc.
 */

#include <linux/freezer.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/sched/signal.h>
#ifdef CONFIG_LIBLINUX
#undef CREATE_TRACE_POINTS
#include <trace/hooks/liblinux.h>
#endif /* CONFIG_LIBLINUX */

#include "deferred-free-helper.h"

static LIST_HEAD(free_list);
static size_t list_nr_pages;
wait_queue_head_t freelist_waitqueue;
struct task_struct *freelist_task;
static DEFINE_SPINLOCK(free_list_lock);

#ifdef CONFIG_LIBLINUX
#define HIGH_WATERMARK 153600 /* 600M */
#define LOW_WATERMARK  51200 /* 200M */
#endif

void deferred_free(struct deferred_freelist_item *item,
		   void (*free)(struct deferred_freelist_item*,
				enum df_reason),
		   size_t nr_pages)
{
	unsigned long flags;

	INIT_LIST_HEAD(&item->list);
	item->nr_pages = nr_pages;
	item->free = free;

	spin_lock_irqsave(&free_list_lock, flags);
	list_add(&item->list, &free_list);
	list_nr_pages += nr_pages;
	spin_unlock_irqrestore(&free_list_lock, flags);
	wake_up(&freelist_waitqueue);
}
EXPORT_SYMBOL_GPL(deferred_free);

static size_t free_one_item(enum df_reason reason)
{
	unsigned long flags;
	size_t nr_pages;
	struct deferred_freelist_item *item;

	spin_lock_irqsave(&free_list_lock, flags);
	if (list_empty(&free_list)) {
		spin_unlock_irqrestore(&free_list_lock, flags);
		return 0;
	}
	item = list_first_entry(&free_list, struct deferred_freelist_item, list);
	list_del(&item->list);

	nr_pages = item->nr_pages;
	list_nr_pages -= nr_pages;
	spin_unlock_irqrestore(&free_list_lock, flags);

	item->free(item, reason);
	return nr_pages;
}

unsigned long get_freelist_nr_pages(void)
{
	unsigned long nr_pages;
	unsigned long flags;

	spin_lock_irqsave(&free_list_lock, flags);
	nr_pages = list_nr_pages;
	spin_unlock_irqrestore(&free_list_lock, flags);
	return nr_pages;
}
EXPORT_SYMBOL_GPL(get_freelist_nr_pages);

#ifdef CONFIG_LIBLINUX
unsigned long dmaheap_freelist_total(void)
{
	return get_freelist_nr_pages();
}
#endif

static unsigned long freelist_shrink_count(struct shrinker *shrinker,
					   struct shrink_control *sc)
{
	return get_freelist_nr_pages();
}

static unsigned long freelist_shrink_scan(struct shrinker *shrinker,
					  struct shrink_control *sc)
{
	unsigned long total_freed = 0;

	if (sc->nr_to_scan == 0)
		return 0;

	while (total_freed < sc->nr_to_scan) {
		size_t pages_freed = free_one_item(DF_UNDER_PRESSURE);
		if (!pages_freed)
			break;

		total_freed += pages_freed;
	}

	return total_freed;
}

static struct shrinker freelist_shrinker = {
	.count_objects = freelist_shrink_count,
	.scan_objects = freelist_shrink_scan,
	.seeks = DEFAULT_SEEKS,
	.batch = 0,
#ifdef CONFIG_LIBLINUX
	.is_stand_alone = true,
#endif /* CONFIG_LIBLINUX */
};

static int deferred_free_thread(void *data)
{
#ifdef CONFIG_LIBLINUX
	unsigned int prio_status = PRIO_NORMAL;
	unsigned long freelist_page;
#endif
	while (true) {
		wait_event_freezable(freelist_waitqueue,
				     get_freelist_nr_pages() > 0);

#ifdef CONFIG_LIBLINUX
		freelist_page = get_freelist_nr_pages();
		if (freelist_page > HIGH_WATERMARK) {
			if (prio_status == PRIO_NORMAL) {
				trace_ldk_rvh_set_vip_prio(freelist_task, PRIO_HIGH);
				prio_status = PRIO_HIGH;
				pr_err("%s: dma buf prio set prio\n", __func__);
			}
		} else if (freelist_page < LOW_WATERMARK) {
			if (prio_status == PRIO_HIGH) {
				trace_ldk_rvh_set_vip_prio(freelist_task, PRIO_NORMAL);
				prio_status = PRIO_NORMAL;
				pr_err("%s: dma buf prio clear prio\n", __func__);
			}
		}
#endif /* CONFIG_LIBLINUX */
		free_one_item(DF_NORMAL);
	}

	return 0;
}

static int deferred_freelist_init(void)
{
	list_nr_pages = 0;

	init_waitqueue_head(&freelist_waitqueue);
	freelist_task = kthread_run(deferred_free_thread, NULL,
				    "%s", "dmabuf-deferred-free-worker");
	if (IS_ERR(freelist_task)) {
		pr_err("Creating thread for deferred free failed\n");
		return -1;
	}
	sched_set_normal(freelist_task, 0);

	return register_shrinker(&freelist_shrinker);
}
module_init(deferred_freelist_init);
MODULE_LICENSE("GPL v2");
