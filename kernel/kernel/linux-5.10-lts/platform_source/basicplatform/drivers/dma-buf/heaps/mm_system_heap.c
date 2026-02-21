/*
 * drivers/dma-buf/heaps/mm_system_heap.c
 *
 * Copyright (C) 2022 Hisilicon, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#define pr_fmt(fmt) "[system_heap]" fmt

#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/err.h>
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/mutex.h>
#include <securec.h>
#include <linux/vmalloc.h>
#include <linux/kthread.h>
#include <linux/dmabuf/mm_dma_heap.h>
#ifdef CONFIG_DMABUF_RECLAIM
#include <linux/dmabuf/dmabuf_map_info.h>
#include <linux/dmabuf/dmabuf_reclaim_fd.h>
#endif
#ifdef CONFIG_DMABUF_NPU_HUGE_POOL
#include <liblinux/pal.h>
#endif
#ifdef CONFIG_LIBLINUX
#include <trace/hooks/liblinux.h>
#endif
#include <linux/seq_file.h>
#include <linux/notifier.h>
#include <linux/bits.h>
#include <linux/miscdevice.h>

#include "mm_heap_helpers.h"
#include "mm_sec_alloc.h"
#include "page_pool.h"
#include "deferred-free-helper.h"
#include "mm_heap_priv.h"

#define KB_SHIFT 10
#define MAX_PAUSE_LIMIT 1000
#define HIGH_ORDER_GFP  (((GFP_HIGHUSER | __GFP_ZERO | __GFP_NOWARN \
				| __GFP_NORETRY) & ~__GFP_RECLAIM) \
				| __GFP_COMP)
#define LOW_ORDER_GFP (GFP_HIGHUSER | __GFP_ZERO | __GFP_COMP)
gfp_t order_flags[] = {HIGH_ORDER_GFP, HIGH_ORDER_GFP, LOW_ORDER_GFP, LOW_ORDER_GFP, LOW_ORDER_GFP};
/*
 * The selection of the orders used for allocation (1MB, 64K, 4K) is designed
 * to match with the sizes often found in IOMMUs. Using order 4 pages instead
 * of order 0 pages can significantly improve the performance of many IOMMUs
 * by reducing TLB pressure and time spent updating page tables.
 */
unsigned int orders[] = {9, 8, 4, 2, 0};
#define SYS_NUM_ORDERS	ARRAY_SIZE(orders)

struct system_heap {
	struct dma_heap heap;
	struct dmabuf_page_pool *pools[SYS_NUM_ORDERS];
#ifdef CONFIG_DMABUF_NPU_HUGE_POOL
	struct dmabuf_page_pool *huge_pools[SYS_NUM_ORDERS];
#endif
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	struct dmabuf_page_pool *async_pools[SYS_NUM_ORDERS];
#endif
	const char *name;
	phys_addr_t align;
	void *priv;
	unsigned long pool_watermark; /* wakeup thread to alloc memory from system */
	struct task_struct *sys_pool_thread;
	wait_queue_head_t sys_pool_wait;
	atomic_t wait_flag;
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	struct task_struct *sys_pool_alloc_task;
	wait_queue_head_t sys_pool_alloc_wait;
	atomic_t sys_pool_alloc_wait_flag;
	atomic_t sys_pool_alloc_prio;
	atomic_t sys_pool_wmark_min;
	atomic_t sys_pool_wmark_low;
	atomic_t sys_pool_wmark_high;
#endif
	struct mutex pool_lock;
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY
	struct blocking_notifier_head nhs[DMA_HEAP_FLAGS_USAGE_ID_MAX];
#endif /* CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY */
	unsigned long pause_limit; /* termination condition of page pool fill thread pause */
	unsigned long mem_count; /* allocation count since page pool fill thread pause */
};
static struct system_heap *sys_heap;

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY
int system_heap_evt_register_notify(struct notifier_block *n, int usage_id)
{
	int ret = 0;

	if (n == NULL) {
		pr_err("%s, NULL pointer of input parameter\n", __func__);
		return -EINVAL;
	}

	if ((usage_id >= DMA_HEAP_FLAGS_USAGE_ID_MAX) || (usage_id < 0)) {
		pr_err("%s, usage_id not supported, usage_id[%d] (DMA_HEAP_FLAGS_USAGE_ID_MAX: %d)\n",
			__func__, usage_id, DMA_HEAP_FLAGS_USAGE_ID_MAX);
		return -EINVAL;
	}

	if (sys_heap == NULL) {
		pr_err("%s, sys_heap not init yet\n", __func__);
		return -EPERM;
	}

	ret = blocking_notifier_chain_register(&sys_heap->nhs[usage_id], n);
	if (ret != 0)
		pr_err("%s, blocking_notifier_chain_unregister fail, ret[%d], usage_id[%d]\n", 
			__func__, ret, usage_id);

	return ret;
}

int system_heap_evt_unregister_notify(struct notifier_block *n, int usage_id)
{
	int ret = 0;

	if (n == NULL) {
		pr_err("%s, NULL pointer of input parameter\n", __func__);
		return -EINVAL;
	}

	if ((usage_id >= DMA_HEAP_FLAGS_USAGE_ID_MAX) || (usage_id < 0)) {
		pr_err("%s, usage_id not supported, usage_id[%d] (DMA_HEAP_FLAGS_USAGE_ID_MAX: %d)\n",
			__func__, usage_id, DMA_HEAP_FLAGS_USAGE_ID_MAX);
		return -EINVAL;
	}

	if (sys_heap == NULL) {
		pr_err("%s, sys_heap not init yet\n", __func__);
		return -EPERM;
	}

	ret = blocking_notifier_chain_unregister(&sys_heap->nhs[usage_id], n);
	if (ret != 0)
		pr_err("%s, blocking_notifier_chain_unregister fail, ret[%d], usage_id[%d]\n", 
			__func__, ret, usage_id);

	return ret;
}

static inline void system_heap_evt_notify(struct system_heap *system_heap, int evt, struct dma_buf *buffer, u64 heap_flags)
{
	int i, ret;
	u64 usage_flags = DMA_HEAP_FLAGS_2_USAGE_FLAGS(heap_flags);

	for (i = 0; i < DMA_HEAP_FLAGS_USAGE_ID_MAX; i++) {
		if (usage_flags & BIT(i)) {
			ret = blocking_notifier_call_chain(&system_heap->nhs[i], evt, buffer);
			if (notifier_to_errno(ret) != 0)
				pr_err("%s, blocking_notifier_call_chain fial ret[%d], event[%d], usage_id[%d], heap_flags[0x%x]\n", 
					__func__, notifier_to_errno(ret), evt, i, heap_flags);
		}
	}
}
#endif /* CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY */

#ifdef CONFIG_DMABUF_POOL_RESIZE
/* WQ to gather alloc/free notifications */
static DECLARE_WAIT_QUEUE_HEAD(system_heap_ctl_wq);
static atomic_t system_heap_resize_event = ATOMIC_INIT(0);

/* Notify WQ poller about system heap pool resize */
static void cond_notify_resize(bool do_notify)
{
	if (do_notify) {
		atomic_set(&system_heap_resize_event, 1);
		wake_up_interruptible(&system_heap_ctl_wq);
	}
}
#else /* !CONFIG_DMABUF_POOL_RESIZE */
static inline void cond_notify_resize(bool do_notify) { }
#endif /* !CONFIG_DMABUF_POOL_RESIZE */

static int dma_sys_pool_count(struct system_heap *system_heap)
{
	int i;
	int num_pages = 0;
	struct dmabuf_page_pool **pool = NULL;

	pool = system_heap->pools;
	for (i = 0; i < SYS_NUM_ORDERS; i++, pool++)
		num_pages += ((*pool)->count[POOL_LOWPAGE] +
			      (*pool)->count[POOL_HIGHPAGE]) << (*pool)->order;
	return num_pages;
}

static int fill_pool_once(struct dmabuf_page_pool *pool, unsigned long *count)
{
	struct page *rpage = NULL;

	rpage = dmabuf_page_pool_alloc_fill(pool, count);
	if (!rpage) 
		return -ENOMEM;

	dmabuf_page_pool_free(pool, rpage);

	return 0;
}

static void fill_pool_watermark(struct dmabuf_page_pool **pools,
				unsigned long watermark)
{
	unsigned int i;
	unsigned long count = 0;
	unsigned long fill_size;
	bool notify = false;

	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		while (watermark) {
			if (fill_pool_once(pools[i], &count) != 0) {
				pr_err("%s:fill_pool_once fail, pools[%u]\n", __func__, i);
				break;
			}
			notify = true;
			fill_size = (1UL << pools[i]->order) * count;
			if (watermark >= fill_size) {
				watermark -= fill_size;
			} else {
				watermark = 0;
			}
		}
	}
	cond_notify_resize(notify);
}

#ifdef CONFIG_DMABUF_POOL_RESIZE
/* Free up to @nr_pages from the pools */
static void shrink_pool_watermark(struct dmabuf_page_pool **pools,
				unsigned long nr_pages)
{
	bool notify = false;
	unsigned int i;
	int nr_freed;

	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		while (nr_pages) {
			nr_freed = dmabuf_page_pool_do_shrink(pools[i],
						0, nr_pages);
			if (!notify && nr_freed)
				notify = true;
			if (nr_pages <= nr_freed || !nr_freed)
				break;
			nr_pages -= nr_freed;
		}
	}
	cond_notify_resize(notify);
}
#endif

static void dma_pool_wakeup(void)
{
	 struct system_heap *heap = sys_heap;

	 atomic_set(&heap->wait_flag, 1);
	 wake_up_interruptible(&heap->sys_pool_wait);
}

static int dma_pool_kthread(void *p)
{
	struct system_heap *heap = NULL;
	int ret;
	long nr_fill_count = 0;

	heap = (struct system_heap *)p;
	while (!kthread_should_stop()) {
		ret = wait_event_interruptible(heap->sys_pool_wait,
						atomic_read(&heap->wait_flag));
		if (ret)
			continue;

		atomic_set(&heap->wait_flag, 0);

		mutex_lock(&heap->pool_lock);
		nr_fill_count = heap->pool_watermark -
				(unsigned long)dma_sys_pool_count(heap);
#ifdef CONFIG_DMABUF_POOL_RESIZE
		if (nr_fill_count < 0)
			shrink_pool_watermark(heap->pools, -nr_fill_count);
		else if (nr_fill_count > 0)
			fill_pool_watermark(heap->pools, nr_fill_count);
#else
		if (nr_fill_count <= 0){
			mutex_unlock(&heap->pool_lock);
			continue;
		}
		fill_pool_watermark(heap->pools, nr_fill_count);
#endif
		heap->pool_watermark = 0;
		mutex_unlock(&heap->pool_lock);
	}
	return 0;
}

#ifndef CONFIG_DMABUF_POOL_RESIZE
static bool dma_pool_watermark_check(struct system_heap *heap,
				     unsigned long nr_watermark)
{
	unsigned long nr_pool_count = 0;

	if (!nr_watermark)
		return false;

	if (heap->pool_watermark >= nr_watermark)
		return false;

	nr_pool_count = (unsigned long)dma_sys_pool_count(heap);
	if (nr_pool_count >= nr_watermark)
		return false;

	return true;
}
#endif

static unsigned long get_dma_pool_watermark(void)
{
	unsigned long watermark;
	struct system_heap *heap = sys_heap;

	mutex_lock(&heap->pool_lock);
	watermark = heap->pool_watermark * PAGE_SIZE;
	mutex_unlock(&heap->pool_lock);

	return watermark;
}

static void set_dma_pool_watermark(unsigned long watermark)
{
	struct system_heap *heap = sys_heap;
	unsigned long nr_watermark = watermark / PAGE_SIZE;
	bool pool_wakeup = true;
	if (!wq_has_sleeper(&heap->sys_pool_wait))
		return;

	mutex_lock(&heap->pool_lock);

#ifdef CONFIG_DMABUF_POOL_RESIZE
	heap->pool_watermark = nr_watermark;
	/* No need for wakeup since we don't change anything. */
	if (!nr_watermark)
		pool_wakeup = false;
#else
	pool_wakeup = dma_pool_watermark_check(heap, nr_watermark);
	/*
	 * Maximization principle.
	 */
	if (pool_wakeup && (heap->pool_watermark < nr_watermark)) {
		heap->pool_watermark = nr_watermark;
	} else if (!nr_watermark) {
		heap->pool_watermark = nr_watermark;
	} else {
		/* no need to set watermark because thread will not run */
	}
#endif
	mutex_unlock(&heap->pool_lock);

	if (pool_wakeup)
		dma_pool_wakeup();
}

static void set_dma_pool_fill_pause(unsigned long target_val)
{
	struct system_heap *heap = sys_heap;
	unsigned int i;

	if (target_val == 0)
		return;

	mutex_lock(&heap->pool_lock);
	heap->pause_limit = target_val;
	heap->mem_count = 0;
	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		heap->pools[i]->extend_flag |= ZONE_EXTEND_FROM_IOFAST_ONLY;
	}
	mutex_unlock(&heap->pool_lock);
}

static ssize_t dma_pool_watermark_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t size)
{
	unsigned long val;
	int ret;
	char *p = (char*)buf;
	char *token = strsep(&p, " ");

	if (token != NULL) {
		ret = kstrtoul(token, 0, &val);
		if (!ret) {
			if (val > MAX_WATER_MARK)
				val = MAX_WATER_MARK;

			set_dma_pool_watermark(val);
		}
		token = strsep(&p, " ");
		if (token != NULL) {
			ret = kstrtoul(skip_spaces(token), 0, &val);
			if (ret)
				return ret;
 
			if (val > MAX_PAUSE_LIMIT)
				val = MAX_PAUSE_LIMIT;
			set_dma_pool_fill_pause(val);
		}
	}
	return size;
}

static ssize_t dma_pool_watermark_show(struct device *dev,
				       struct device_attribute *attr,
				       char *buf)
{
	return sprintf_s(buf, sizeof(unsigned long), "%lu", get_dma_pool_watermark());
}

static DEVICE_ATTR_RW(dma_pool_watermark);

static struct attribute *dma_pool_attrs[] = {
	&dev_attr_dma_pool_watermark.attr,
	NULL,
};

static const struct attribute_group dma_pool_attr_group = {
	.name = NULL,
	.attrs = dma_pool_attrs,
};

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
#define DMA_POOL_WMARK_MAX 786432 /* 3GB */

static atomic64_t dma_pool_wmark_min = ATOMIC64_INIT(0);
static atomic64_t dma_pool_wmark_low = ATOMIC64_INIT(0);
static atomic64_t dma_pool_realwake = ATOMIC64_INIT(0);
static int sys_pool_wmark_min_default = 0;
static int sys_pool_wmark_low_default = 0;
static int sys_pool_wmark_high_default = 0;

static ssize_t dma_pool_wmark_min_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	int val;
	int ret;

	ret = kstrtoint(buf, 0, &val);
	if (ret)
		return ret;

	if (val > DMA_POOL_WMARK_MAX || val < 0)
		return -EINVAL;

	atomic_set(&sys_heap->sys_pool_wmark_min, val);

	pr_err("%s: sys_pool_wmark_min %d\n",
		__func__, atomic_read(&sys_heap->sys_pool_wmark_min));

	return count;
}

static ssize_t dma_pool_wmark_min_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf_s(buf, PAGE_SIZE, "%d\n", atomic_read(&sys_heap->sys_pool_wmark_min));
}

static DEVICE_ATTR_RW(dma_pool_wmark_min);

static ssize_t dma_pool_wmark_low_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	int val;
	int ret;

	ret = kstrtoint(buf, 0, &val);
	if (ret)
		return ret;

	if (val > DMA_POOL_WMARK_MAX || val < 0)
		return -EINVAL;

	atomic_set(&sys_heap->sys_pool_wmark_low, val);

	pr_err("%s: sys_pool_wmark_low %d\n",
		__func__, atomic_read(&sys_heap->sys_pool_wmark_low));

	return count;
}

static ssize_t dma_pool_wmark_low_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf_s(buf, PAGE_SIZE, "%d\n", atomic_read(&sys_heap->sys_pool_wmark_low));
}

static DEVICE_ATTR_RW(dma_pool_wmark_low);

static ssize_t dma_pool_wmark_high_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	int val;
	int ret;

	ret = kstrtoint(buf, 0, &val);
	if (ret)
		return ret;

	if (val > DMA_POOL_WMARK_MAX || val < 0)
		return -EINVAL;

	atomic_set(&sys_heap->sys_pool_wmark_high, val);

	pr_err("%s: sys_pool_wmark_high %d\n",
		__func__, atomic_read(&sys_heap->sys_pool_wmark_high));

	return count;
}

static ssize_t dma_pool_wmark_high_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf_s(buf, PAGE_SIZE, "%d\n", atomic_read(&sys_heap->sys_pool_wmark_high));
}

static DEVICE_ATTR_RW(dma_pool_wmark_high);

static ssize_t dma_pool_wmark_default_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	int val;
	int ret;

	ret = kstrtoint(buf, 0, &val);
	if (ret)
		return ret;

	if (val)
		return -EINVAL;

	atomic_set(&sys_heap->sys_pool_wmark_min, sys_pool_wmark_min_default);
	atomic_set(&sys_heap->sys_pool_wmark_low, sys_pool_wmark_low_default);
	atomic_set(&sys_heap->sys_pool_wmark_high, sys_pool_wmark_high_default);

	pr_err("%s: sys_pool_wmark_min %d, sys_pool_wmark_low %d, sys_pool_wmark_high %d\n",
		__func__, atomic_read(&sys_heap->sys_pool_wmark_min),
		atomic_read(&sys_heap->sys_pool_wmark_low),
		atomic_read(&sys_heap->sys_pool_wmark_high));

	return count;
}

static ssize_t dma_pool_wmark_default_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	ssize_t size = 0;

	size += sprintf_s(buf, PAGE_SIZE, "sys_pool_wmark_min %d\n",
						atomic_read(&sys_heap->sys_pool_wmark_min));
	size += sprintf_s(buf + size, PAGE_SIZE - size, "sys_pool_wmark_low %d\n",
						atomic_read(&sys_heap->sys_pool_wmark_low));
	size += sprintf_s(buf + size, PAGE_SIZE - size, "sys_pool_wmark_high %d\n",
						atomic_read(&sys_heap->sys_pool_wmark_high));

	return size;
}

static DEVICE_ATTR_RW(dma_pool_wmark_default);

static struct attribute *dma_pool_wmark_attrs[] = {
	&dev_attr_dma_pool_wmark_min.attr,
	&dev_attr_dma_pool_wmark_low.attr,
	&dev_attr_dma_pool_wmark_high.attr,
	&dev_attr_dma_pool_wmark_default.attr,
	NULL,
};

static const struct attribute_group dma_pool_wmark_attr_group = {
	.name = NULL,
	.attrs = dma_pool_wmark_attrs,
};

int system_heap_sys_pool_wmark_high(void)
{
	return atomic_read(&sys_heap->sys_pool_wmark_high);
}

int system_heap_sys_pool_count(void)
{
	return dma_sys_pool_count(sys_heap);
}

static void system_heap_sys_pool_task_show(struct seq_file *s)
{
	seq_printf(s, "dma_pool_wmark_min %lld, dma_pool_wmark_low %lld, dma_pool_realwake %lld\n"
		"dma_pool_wmark_high %lld, dma_pool_empty %lld\n",
		atomic64_read(&dma_pool_wmark_min), atomic64_read(&dma_pool_wmark_low),
		atomic64_read(&dma_pool_realwake), get_dma_pool_wmark_high_value(),
		get_dma_pool_empty_value());
}

void dma_pool_alloc_wakeup(void)
{
	int nr_pools = 0;
	struct system_heap *heap = sys_heap;

	nr_pools = dma_sys_pool_count(heap);
	if (nr_pools >= atomic_read(&heap->sys_pool_wmark_low))
		return;

	atomic64_inc(&dma_pool_wmark_low);

	if (!wq_has_sleeper(&heap->sys_pool_alloc_wait))
		return;

	if (nr_pools < atomic_read(&heap->sys_pool_wmark_min)) {
		atomic64_inc(&dma_pool_wmark_min);
		if (atomic_read(&heap->sys_pool_alloc_prio) == DEFAULT_PRIO) {
			set_user_nice(heap->sys_pool_alloc_task, PRIO_TO_NICE(MAX_RT_PRIO));
			atomic_set(&heap->sys_pool_alloc_prio, MAX_RT_PRIO);
		}
	} else {
		if (atomic_read(&heap->sys_pool_alloc_prio) == MAX_RT_PRIO) {
			set_user_nice(heap->sys_pool_alloc_task, PRIO_TO_NICE(DEFAULT_PRIO));
			atomic_set(&heap->sys_pool_alloc_prio, DEFAULT_PRIO);
		}
	}

	atomic_set(&heap->sys_pool_alloc_wait_flag, 1);
	wake_up_interruptible(&heap->sys_pool_alloc_wait);
}

static int sys_pool_alloc_kthread(void *p)
{
	int i, j, ret;
	int count;
	int nr_order;
	int waterline;
	int count_above_waterline;
	int nr_to_alloc = 0;
	struct dmabuf_page_pool *pool;
	struct dmabuf_page_pool *async_pool;
	struct system_heap *heap = (struct system_heap *)p;

	while (!kthread_should_stop()) {
		ret = wait_event_interruptible(heap->sys_pool_alloc_wait,
					atomic_read(&heap->sys_pool_alloc_wait_flag));
		if (ret)
			continue;

		atomic64_inc(&dma_pool_realwake);
		atomic_set(&heap->sys_pool_alloc_wait_flag, 0);
		nr_to_alloc = atomic_read(&heap->sys_pool_wmark_high) - dma_sys_pool_count(heap);
		if (nr_to_alloc <= 0) {
			continue;
		}

		fill_pool_watermark(heap->async_pools, nr_to_alloc);
		for (i = 0; i < SYS_NUM_ORDERS; i++) {
			async_pool = heap->async_pools[i];
			pool = heap->pools[i];
			nr_order = 1 << pool->order;
			waterline = dmabuf_page_pool_get_shrink_waterline(pool);

			mutex_lock(&pool->mutex);
			for (j = 0; j < POOL_TYPE_SIZE; j++) {
				count = async_pool->count[j];
				if (!count) {
					continue;
				}
				trace_ldk_rvh_track_dmabuf_free_pages_mod(async_pool, j, -(nr_order * count));
				list_splice_tail_init(&async_pool->items[j], &pool->items[j]);
				pool->count[j] += count;
				count_above_waterline = (pool->count[j] > waterline) ? min(count, (pool->count[j] - waterline)) : 0;
				trace_ldk_rvh_track_dmabuf_free_pages_mod(pool, j, nr_order * count_above_waterline);
				async_pool->count[j] = 0;
			}
			mutex_unlock(&pool->mutex);
		}
	}

	return 0;
}
#endif

static void system_heap_show_pool(struct seq_file *s, struct dmabuf_page_pool **pools)
{
	int i;
	long num_pages = 0;

	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		struct dmabuf_page_pool *pool = pools[i];
		if (pool->count[POOL_HIGHPAGE] > 0)
			seq_printf(s, "order %u highmem pages %8d total %8d kB\n",
					pool->order, pool->count[POOL_HIGHPAGE],
					pool->count[POOL_HIGHPAGE] << pool->order << (PAGE_SHIFT - KB_SHIFT));

		seq_printf(s, "order %u lowmem pages %8d total %8d kB\n",
				pool->order, pool->count[POOL_LOWPAGE],
				pool->count[POOL_LOWPAGE] << pool->order << (PAGE_SHIFT - KB_SHIFT));
		num_pages += (pool->count[POOL_HIGHPAGE] + pool->count[POOL_LOWPAGE]) << pool->order;
	}
	seq_printf(s, "sum: %8lu kB  %8lu MB\n",
			num_pages << (PAGE_SHIFT - KB_SHIFT),
			num_pages << (PAGE_SHIFT - KB_SHIFT) >> KB_SHIFT);
}

void mm_dmaheap_system_heap_info(struct seq_file *s, struct dma_heap *heap)
{
	struct system_heap *system_heap = container_of(heap,
						struct system_heap, heap);

	system_heap_show_pool(s, system_heap->pools);

#ifdef CONFIG_DMABUF_NPU_HUGE_POOL
	seq_puts(s, "------------------------------------------------------------------------\n");
	system_heap_show_pool(s, system_heap->huge_pools);
#endif

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	system_heap_show_pool(s, system_heap->async_pools);

	system_heap_sys_pool_task_show(s);
#endif
}

#if defined(CONFIG_LIBLINUX) && defined(CONFIG_HISI_GPU)
static atomic64_t hvgr_dmabuf_page_num = ATOMIC64_INIT(0);
/* 512M */
#define HVGR_DMABUF_PAGE_LIMIT 131072

static bool is_hvgr_dmabuf_page(struct page *page)
{
	if (unlikely(test_bit(PG_hvgr, &page->flags))) {
		atomic64_inc(&hvgr_dmabuf_page_num);
		pr_err("%s hvgr dmabuf page cnts %lld\r\n", __func__, atomic64_read(&hvgr_dmabuf_page_num));
		WARN_ON_ONCE(true);

		if (atomic64_read(&hvgr_dmabuf_page_num) > HVGR_DMABUF_PAGE_LIMIT)
			return false;
		return true;
	}

	return false;
}
#endif

static void __system_heap_buf_free(struct deferred_freelist_item *item,
				 enum df_reason reason)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	struct system_heap *system_heap = NULL;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	bool notify = false;
	int i, j;

	buffer = container_of(item, struct mm_heap_helper_buffer, deferred_free);
	/* Zero the buffer pages before adding back to the pool */
	if (reason == DF_NORMAL)
#ifdef CONFIG_DMABUF_RECLAIM
		if (!buffer_reclaim_status_get_lock(buffer) && mm_dma_heap_buffer_zero(buffer))
			reason = DF_UNDER_PRESSURE; // On failure, just free
	if (buffer_reclaim_status_get_lock(buffer))
		goto done;
#else
		if (mm_dma_heap_buffer_zero(buffer))
			reason = DF_UNDER_PRESSURE; // On failure, just free
#endif
	system_heap = container_of(buffer->heap, struct system_heap, heap);

	table = buffer->sg_table;
	for_each_sg(table->sgl, sg, table->nents, i) {
		struct page *page = sg_page(sg);
		pgprot_t pgprot = PAGE_KERNEL;
		unsigned int order = compound_order(page);

#ifdef CONFIG_MM_LB
		if (PageLB(page) || PageLB(page + ((1 << order) - 1)))
			pr_err("%s: page is lb, buf magic:%lu, bufsize:0x%lx, lb_size:0x%lx, plc_id:%d, page order:%u\n",
				__func__, buffer->magic, buffer->size, buffer->lb_size, buffer->plc_id, order);
#endif

#if defined(CONFIG_LIBLINUX) && defined(CONFIG_HISI_GPU)
		if (is_hvgr_dmabuf_page(page))
			continue;
#endif
		if (reason == DF_UNDER_PRESSURE) {
			__free_pages(page, compound_order(page));
		} else {
			for (j = 0; j < SYS_NUM_ORDERS; j++) {
				if (compound_order(page) == orders[j])
					break;
			}

			if (dma_buf_is_uncached(buffer->flags))
				pgprot = pgprot_writecombine(PAGE_KERNEL);

			if (j >= SYS_NUM_ORDERS) {
				pr_err("%s: page order-%d not match system_heap pool order\n",
					__func__, compound_order(page));
				continue;
			}
			notify = true;
#ifndef CONFIG_DMABUF_NPU_HUGE_POOL
			dmabuf_page_pool_free(system_heap->pools[j], page);
#else  /* CONFIG_DMABUF_NPU_HUGE_POOL */
			if (!dma_buf_is_npuhuge(buffer->flags)) {
				dmabuf_page_pool_free(system_heap->pools[j], page);
			} else {
				unsigned long curr_avail_buf = liblinux_get_sys_curr_avail_buf();
				unsigned long avail_high = liblinux_get_sys_avail_buf_wmark_high();
				if (curr_avail_buf > avail_high) {
					dmabuf_page_pool_free(system_heap->huge_pools[j], page);
				} else {
					dmabuf_page_pool_free(system_heap->pools[j], page);
				}
			}
#endif /* CONFIG_DMABUF_NPU_HUGE_POOL */
		}
	}
#ifdef CONFIG_DMABUF_RECLAIM
done:
	if (dma_buf_is_direct_reclaim(buffer->flags))
		kvfree(buffer->extid_table);

	if (table) {
		sg_free_table(table);
		kfree(table);
	}
#else
	sg_free_table(table);
	kfree(table);
#endif
	kfree(buffer);
	cond_notify_resize(notify);
}

static void system_heap_buf_free(struct mm_heap_helper_buffer *buffer)
{
	int npages = PAGE_ALIGN(buffer->size) / PAGE_SIZE;

	deferred_free(&buffer->deferred_free, __system_heap_buf_free, npages);
}

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_BATCH
#define BATCH_SIZE SZ_128M

static unsigned long alloc_largest_available_batch(struct system_heap *system_heap,
	unsigned long size, unsigned int max_order, unsigned long heap_flags, struct list_head *batch_pages)
{
	int i;
	unsigned long nr_pages;
	unsigned long page_size;
	unsigned long nr_allocated = 0;
	unsigned long batch_size = min(BATCH_SIZE, size);

	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		page_size = PAGE_SIZE << orders[i];
		if (size < page_size)
			continue;
		if (max_order < orders[i])
			continue;

		nr_pages = (batch_size > page_size) ? (batch_size / page_size) : 1;
#ifndef CONFIG_DMABUF_NPU_HUGE_POOL
		nr_allocated = dmabuf_page_pool_alloc_batch(system_heap->pools[i], nr_pages, batch_pages);
#else
		if (!dma_buf_is_npuhuge(heap_flags)) {
			nr_allocated = dmabuf_page_pool_alloc_batch(system_heap->pools[i], nr_pages, batch_pages);
		} else {
			nr_allocated = dmabuf_page_pool_npu_huge_alloc_batch(
				system_heap->huge_pools[i], system_heap->pools[i], nr_pages, batch_pages);
		}
#endif
		if (!nr_allocated)
			continue;
		return nr_allocated;
	}

	return nr_allocated;
}
#endif

static struct page *alloc_largest_available(struct system_heap *system_heap,
						unsigned long size, unsigned int max_order, unsigned long heap_flags)
{
	struct page *page = NULL;
	int i;

	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		if (size <  (PAGE_SIZE << orders[i]))
			continue;
		if (max_order < orders[i])
			continue;

#ifndef CONFIG_DMABUF_NPU_HUGE_POOL
		page = dmabuf_page_pool_alloc(system_heap->pools[i]);
#else  /* CONFIG_DMABUF_NPU_HUGE_POOL */
		if (!dma_buf_is_npuhuge(heap_flags)) {
			page = dmabuf_page_pool_alloc(system_heap->pools[i]);
		} else {
			page = dmabuf_page_npu_huge_pool_alloc(system_heap->huge_pools[i], system_heap->pools[i]);
		}
#endif /* CONFIG_DMABUF_NPU_HUGE_POOL */
		if (!page)
			continue;
		return page;
	}

	return NULL;
}

static int system_heap_input_check(unsigned long size, unsigned long flag)
{
	if (size / PAGE_SIZE > totalram_pages() / 2) {
		pr_err("%s: size[0x%lx] too large\n", __func__, size);
		return -ENOMEM;
	}

	return 0;
}

void dma_pool_alloc_wakeup_check(struct system_heap *system_heap)
{
	/*
	 * if camera touch down, do not wakeup dma_pool_alloc thread
	 */
	unsigned int i;
	if (system_heap->pause_limit == 0) {
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
		dma_pool_alloc_wakeup();
#endif
	} else {
		system_heap->mem_count++;
		if (system_heap->mem_count >= system_heap->pause_limit) {
			mutex_lock(&system_heap->pool_lock);
			system_heap->pause_limit = 0;
			system_heap->mem_count = 0;
			for (i = 0; i < SYS_NUM_ORDERS; i++)
				system_heap->pools[i]->extend_flag &= ~ZONE_EXTEND_FROM_IOFAST_ONLY;
			mutex_unlock(&system_heap->pool_lock);
		}
	}
}

static struct dma_buf *system_heap_do_allocate(struct dma_heap *heap,
					       unsigned long len,
					       unsigned long fd_flags,
					       unsigned long heap_flags)
{
	struct system_heap *system_heap = container_of(heap, struct system_heap, heap);
	struct mm_heap_helper_buffer *buffer = NULL;
	unsigned long size_remaining = len;
	unsigned int max_order = orders[0];
	struct dma_buf *dmabuf = NULL;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL, *tmp_page = NULL;
	struct list_head pages;
	int i = 0, ret = -ENOMEM;
	bool notify = false;

	if (system_heap_input_check(len, heap_flags))
		return ERR_PTR(-ENOMEM);

	if (dma_buf_is_secure(heap_flags)) {
		pr_err("%s: flag[0x%lx] err, sec_flags was set\n", __func__, heap_flags);
		return ERR_PTR(-EINVAL);
	}

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

	init_mm_heap_helper_buffer(buffer, system_heap_buf_free);
	buffer->heap = heap;
	buffer->size = len;
	buffer->flags = heap_flags;
	buffer->magic = atomic64_inc_return(&dmaheap_magic);

	dma_pool_alloc_wakeup_check(system_heap);

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_BATCH
	unsigned long nr_allocated = 0;
	LIST_HEAD(batch_pages);
#endif
	INIT_LIST_HEAD(&pages);
	i = 0;
	while (size_remaining > 0) {
		/*
		 * Avoid trying to allocate memory if the process
		 * has been killed by SIGKILL
		 */
		if (fatal_signal_pending(current))
			goto free_buffer;

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_BATCH
		nr_allocated = alloc_largest_available_batch(
			system_heap, size_remaining, max_order, buffer->flags, &batch_pages);
		if (!nr_allocated)
			goto free_buffer;

		page = list_first_entry_or_null(&batch_pages, struct page, lru);
		if (!page)
			goto free_buffer;

		notify = true;
		list_splice_tail_init(&batch_pages, &pages);
		size_remaining -= page_size(page) * nr_allocated;
		max_order = compound_order(page);
		i += nr_allocated;
#else
		page = alloc_largest_available(system_heap, size_remaining, max_order, buffer->flags);
		if (!page)
			goto free_buffer;
		notify = true;
		list_add_tail(&page->lru, &pages);
		size_remaining -= page_size(page);
		max_order = compound_order(page);
		i++;
#endif
	}
	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		goto free_buffer;

	if (sg_alloc_table(table, i, GFP_KERNEL))
		goto free_table;

	sg = table->sgl;
	list_for_each_entry_safe(page, tmp_page, &pages, lru) {
		sg_set_page(sg, page, page_size(page), 0);
		sg_dma_address(sg) = sg_phys(sg);
		sg_dma_len(sg) = sg->length;
		sg = sg_next(sg);
		list_del(&page->lru);
	}
	buffer->sg_table = table;
#ifdef CONFIG_DMABUF_RECLAIM
	if (dma_buf_is_direct_reclaim(buffer->flags)) {
		buffer->extid_num = buffer->size >> PAGE_SHIFT;
		buffer->extid_table = vzalloc(sizeof(int) * buffer->extid_num);

		if (!buffer->extid_table)
			goto free_pages;

		if (alloc_buffer_map_info((void *)buffer))
			goto free_extid_table;

		atomic_set(&buffer->attach_num, 0);
		buffer_reclaim_status_set(buffer, false);
		init_rwsem(&buffer->rwsem);
	}
#endif
	/* create dmabuf */
	dmabuf = mm_heap_helper_export_dmabuf(buffer, fd_flags);
	if (IS_ERR(dmabuf)) {
		ret = PTR_ERR(dmabuf);
#ifdef CONFIG_DMABUF_RECLAIM
		goto free_map_info;
#else
		goto free_pages;
#endif
	}
	buffer->dmabuf = dmabuf;
	/*
	 * For uncached buffers, we need to initially flush cpu cache, since
	 * the __GFP_ZERO on the allocation means the zeroing was done by the
	 * cpu and thus it is likely cached. Map (and implicitly flush) and
	 * unmap it now so we don't get corruption later on.
	 */
	dma_heap_flush_buffer_cache(buffer, heap_flags);

	mm_dmaheap_init_dump(buffer);
	mm_dmaheap_buffer_add(buffer);

	if (len > DMABUF_LARGE_BUF_SIZE) 
		pr_err("%s: alloc and export finish\n", __func__);
	cond_notify_resize(notify);
	return dmabuf;
#ifdef CONFIG_DMABUF_RECLAIM
free_map_info:
	if (dma_buf_is_direct_reclaim(buffer->flags))
		free_buffer_map_info((void *)buffer);
free_extid_table:
	if (dma_buf_is_direct_reclaim(buffer->flags))
		kvfree(buffer->extid_table);
#endif
free_pages:
	for_each_sgtable_sg(table, sg, i) {
		struct page *p = sg_page(sg);

		__free_pages(p, compound_order(p));
	}
	sg_free_table(table);
free_table:
	kfree(table);
free_buffer:
	list_for_each_entry_safe(page, tmp_page, &pages, lru)
		__free_pages(page, compound_order(page));
	kfree(buffer);
	/* In case of error handling the pool may be resized too */
	cond_notify_resize(notify);

	pr_err("%s: [%s]alloc fail, size[0x%lx]\n", __func__, system_heap->name, len);
	return ERR_PTR(ret);
}

static struct dma_buf *system_heap_allocate(struct dma_heap *heap,
					    unsigned long len,
					    unsigned long fd_flags,
					    unsigned long heap_flags)
{
	struct dma_buf *dmabuf = NULL;

	dmabuf = system_heap_do_allocate(heap, len, fd_flags, heap_flags);
	if (IS_ERR(dmabuf))
		return dmabuf;

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY
	system_heap_evt_notify(sys_heap, DMA_HEAP_NOTIFY_EVENT_ALLOC, dmabuf, heap_flags);
#endif /* CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY */

	return dmabuf;
}

#ifdef CONFIG_DMABUF_RECLAIM
int buffer_alloc_sgtable(void *buf)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	struct system_heap *system_heap = NULL;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL, *tmp_page = NULL;
	struct list_head pages;
	unsigned long size_remaining;
	unsigned int max_order = orders[0];
	bool notify = false;
	int i = 0;

	if (!buf)
		return -EINVAL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	system_heap = container_of(buffer->heap, struct system_heap, heap);
	size_remaining = buffer->size;

	INIT_LIST_HEAD(&pages);
	while (size_remaining > 0) {
		/*
		 * Avoid trying to allocate memory if the process
		 * has been killed by SIGKILL
		 */
		if (fatal_signal_pending(current))
			goto free_pages;

		page = alloc_largest_available(system_heap, size_remaining, max_order, buffer->flags);
		if (!page)
			goto free_pages;
		notify = true;
		list_add_tail(&page->lru, &pages);
		size_remaining -= page_size(page);
		max_order = compound_order(page);
		i++;
	}
	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		goto free_pages;

	if (sg_alloc_table(table, i, GFP_KERNEL))
		goto free_table;

	sg = table->sgl;
	list_for_each_entry_safe(page, tmp_page, &pages, lru) {
		sg_set_page(sg, page, page_size(page), 0);
		sg_dma_address(sg) = sg_phys(sg);
		sg_dma_len(sg) = sg->length;
		sg = sg_next(sg);
		list_del(&page->lru);
	}
	buffer->sg_table = table;

	atomic_long_add(PAGE_ALIGN(buffer->size), &dmaheap_alloc_total_size);
	dma_heap_direct_flush_buffer_cache(buffer);
	cond_notify_resize(notify);
	return 0;
free_table:
	kfree(table);
free_pages:
	list_for_each_entry_safe(page, tmp_page, &pages, lru)
		__free_pages(page, compound_order(page));
	cond_notify_resize(notify);
	return -EINVAL;
}

int buffer_free_sgtable(void *buf)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	int i = 0;

	if (!buf)
		return -EINVAL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	table = buffer->sg_table;
	if (!table)
		return 0;

	atomic_long_sub(buffer->size, &dmaheap_alloc_total_size);
	for_each_sg(table->sgl, sg, table->nents, i) {
		page = sg_page(sg);
		__free_pages(page, compound_order(page));
	}

	buffer->sg_table = NULL;
	sg_free_table(table);
	kfree(table);

	return 0;
}

bool is_system_heap(struct dma_heap *heap)
{
	if (!heap || !heap->name) {
		return false;
	}
	return !strcmp(heap->name, "system_heap");
}
#endif /* CONFIG_DMABUF_RECLAIM */

static long system_get_pool_size(struct dma_heap *heap)
{
	int i;
	int num_pages = 0;
	struct dmabuf_page_pool **pool = NULL;
	struct system_heap *system_heap = container_of(heap,
							struct system_heap, heap);

	num_pages = dma_sys_pool_count(system_heap);

#ifdef CONFIG_DMABUF_NPU_HUGE_POOL
	pool = system_heap->huge_pools;
	for (i = 0; i < SYS_NUM_ORDERS; i++, pool++)
		num_pages += ((*pool)->count[POOL_LOWPAGE] +
			      (*pool)->count[POOL_HIGHPAGE]) << (*pool)->order;
#endif
	return ((long)num_pages) << PAGE_SHIFT;
}

#ifdef CONFIG_DMABUF_RECLAIM
static void system_heap_vm_open(struct vm_area_struct *vma)
{
	pr_err("%s, attantion!!!\n", __func__);
	/* vma flags should contain "VM_DONTCOPY" */
}

static vm_fault_t system_heap_vm_fault(struct vm_fault *vmf)
{
#ifdef CONFIG_DMABUF_RECLAIM
	int ret;
	struct vm_area_struct *vma;
	struct file *dma_file;
	struct inode *inode;
	unsigned long ino;

	if (!vmf) {
		goto fault;
	}
	vma = vmf->vma;
	if (!vma) {
		goto fault;
	}
	dma_file = vma->vm_file;
	if (!dma_file) {
		pr_err("%s: vm_file null\n", __func__);
		goto fault;
	}
	if (!is_dma_buf_file(dma_file)) {
		goto fault;
	}
	inode = dma_file->f_inode;
	if (!inode) {
		pr_err("%s: dma_file inode null\n", __func__);
		goto fault;
	}
	ino = inode->i_ino;
	ret = cpu_pf_dmabuf_resume_ino(ino);
	if (ret) {
		pr_err("%s: dmabuf[%lu] resume failed\n", __func__, ino);
	} else {
		pr_info("%s: dmabuf[%lu] resume succeed\n", __func__, ino);
		return VM_FAULT_NOPAGE;
	}
#endif
fault:
	pr_err("%s, dmabuf cpu fault unsupport!!!\n", __func__);
	return VM_FAULT_SIGBUS;
}

static void system_heap_vm_close(struct vm_area_struct *vma)
{
	struct dma_buf *dmabuf = NULL;
	struct mm_heap_helper_buffer *buffer = NULL;
	struct file *file = vma->vm_file;

	if (!file || !is_dma_buf_file(file)) {
		pr_err("%s, buffer file err\n", __func__);
		return;
	}

	dmabuf = (struct dma_buf *)file->private_data;
	if (IS_ERR_OR_NULL(dmabuf)) {
		pr_err("%s, file get dmabuf err\n", __func__);
		return;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s, buffer is NULL\n", __func__);
		return;
	}

	delete_buffer_uva_info(buffer, vma);
}

static const struct vm_operations_struct system_heap_vm_ops = {
	.fault = system_heap_vm_fault,
	.close = system_heap_vm_close,
};

static bool is_dmabuf_reclaimable_assign_va(unsigned long addr)
{
	struct vm_area_struct *vma = NULL;
	struct file *file = NULL;
	struct dma_buf *dmabuf = NULL;
	struct mm_heap_helper_buffer *buffer = NULL;

	vma = find_vma(current->mm, addr);
	if (!vma) {
		pr_err("%s, vma NULL\n", __func__);
		return false;
	}

	file = vma->vm_file;
	if (!file) {
		pr_err("%s, vma file NULL\n", __func__);
		return false;
	}

	dmabuf = (struct dma_buf *)file->private_data;
	if (IS_ERR_OR_NULL(dmabuf)) {
		pr_err("%s, file dmabuf err\n", __func__);
		return false;
	}

	return buffer_reclaim_status_get_lock(dmabuf->priv);
}

static int system_heap_map_user(struct dma_heap *heap, struct mm_heap_helper_buffer *buffer,
				    struct vm_area_struct *vma)
{
	int ret;

	if (dma_buf_is_direct_reclaim(buffer->flags)) {
		if (buffer->is_reclaim) {
			pr_err("%s, buf has already released\n", __func__);
			return -EINVAL;
		}
		/* init vma ops */
		vma->vm_ops = &system_heap_vm_ops;
		vma->vm_private_data = buffer;
	}

	ret = mm_dma_heap_map_user(heap, buffer, vma);
	if (!ret)
		(void)record_buffer_uva_info(buffer, vma, vma->vm_end - vma->vm_start);

	return ret;
}
#endif /* CONFIG_DMABUF_RECLAIM */

static const struct dma_heap_ops mm_system_heap_ops = {
	.allocate = system_heap_allocate,
	.get_pool_size = system_get_pool_size,
#ifdef CONFIG_DMABUF_RECLAIM
	.map_user = system_heap_map_user,
#else
	.map_user = mm_dma_heap_map_user,
#endif
	.map_kernel = mm_dma_heap_map_kernel,
	.unmap_kernel = mm_dma_heap_unmap_kernel,
};

#ifdef CONFIG_DMABUF_POOL_RESIZE
static __poll_t system_heap_poll(struct file *filep, poll_table *wait)
{
	poll_wait(filep, &system_heap_ctl_wq, wait);

	if (atomic_cmpxchg(&system_heap_resize_event, 1, 0))
		return EPOLLIN;
	return 0;
}

static ssize_t system_heap_read(struct file *filep, char __user *ubuf,
				size_t usize, loff_t *off)
{
	long pool_size;
	ssize_t count;
	char buf[32];

	if (*off != 0)
		return 0;

	/*
	 * In case if heap was not registered for
	 * any sake lets simply report an error.
	 */
	if (!sys_heap)
		return -ENODEV;

	mutex_lock(&sys_heap->pool_lock);
	pool_size = system_get_pool_size(&sys_heap->heap);
	mutex_unlock(&sys_heap->pool_lock);

	count = sprintf_s(buf, sizeof(buf), "%ld", pool_size);
	if (count < 0)
		return -EINVAL;

	if (usize < count)
		return -EINVAL;

	if (copy_to_user(ubuf, buf, count))
		return -EFAULT;

	*off = count;
	return count;
}

static const struct file_operations system_heap_ctl_fops = {
	.owner		= THIS_MODULE,
	.read		= system_heap_read,
	.poll		= system_heap_poll,
	.llseek		= default_llseek,
};

/* The device allows to poll from userspace for buffer resize notifications */
struct miscdevice system_heap_ctl_dev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "system_heap_ctl",
	.fops		= &system_heap_ctl_fops,
};

/* Create system heap device */
static void system_heap_create_ctl_dev(void)
{
	int ret = misc_register(&system_heap_ctl_dev);
	if (ret != 0) {
		pr_err("%s device registration error %d\n",
			system_heap_ctl_dev.name, ret);
	}
}
#else /* !CONFIG_DMABUF_POOL_RESIZE */
static inline void system_heap_create_ctl_dev(void) { }
#endif /* !CONFIG_DMABUF_POOL_RESIZE */

static int system_heap_parse_dt(struct system_heap *system_heap,
							struct device_node *np)
{
	u32 heap_align = 0;
	int ret;
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	int wmark_min = 0;
	int wmark_low = 0;
	int wmark_high = 0;
#endif

	ret = of_property_read_string(np, "heap-name", &system_heap->name);
	if (ret < 0) {
		pr_err("%s: can not find heap-name\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "heap-align", &heap_align);
	if (ret < 0) {
		pr_err("%s: can not find heap-align\n", __func__);
		return ret;
	}
	system_heap->align = heap_align;

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	ret = of_property_read_s32(np, "wmark-min", &wmark_min);
	if (ret < 0) {
		pr_info("%s: can not find wmark-min\n", __func__);
		wmark_min = DMA_POOL_WMARK_MIN;
	}
	atomic_set(&system_heap->sys_pool_wmark_min, wmark_min);
	sys_pool_wmark_min_default = wmark_min;

	ret = of_property_read_s32(np, "wmark-low", &wmark_low);
	if (ret < 0) {
		pr_info("%s: can not find wmark-low\n", __func__);
		wmark_low = DMA_POOL_WMARK_LOW;
	}
	atomic_set(&system_heap->sys_pool_wmark_low, wmark_low);
	sys_pool_wmark_low_default = wmark_low;

	ret = of_property_read_s32(np, "wmark-high", &wmark_high);
	if (ret < 0) {
		pr_info("%s: can not find wmark-high\n", __func__);
		wmark_high = DMA_POOL_WMARK_HIGH;
	}
	atomic_set(&system_heap->sys_pool_wmark_high, wmark_high);
	sys_pool_wmark_high_default = wmark_high;
#endif

	return 0;
}

static int system_heap_create(struct device_node *np)
{
	struct system_heap *system_heap = NULL;
	struct kobject *dma_kobj;
	int i, ret, err;

	system_heap = kzalloc(sizeof(*system_heap), GFP_KERNEL);
	if (!system_heap)
		return -ENOMEM;
	/* dts init */
	ret = system_heap_parse_dt(system_heap, np);
	if (ret) {
		pr_err("%s: dts init failed!\n", __func__);
		goto free_heap;
	}

	system_heap->heap.ops = &mm_system_heap_ops;
	system_heap->heap.name = system_heap->name;
	system_heap->heap.priv = NULL;
	system_heap->pause_limit = 0; /* initialize system_heap->pause_limit */
	system_heap->mem_count = 0;
	kref_init(&system_heap->heap.refcount);
	atomic_set(&system_heap->wait_flag, 0);

	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		system_heap->pools[i] = dmabuf_page_pool_create(order_flags[i], orders[i]);
		if (!system_heap->pools[i]) {
			pr_err("%s: page pool creation failed!\n", __func__);
			goto destroy_pools;
		}
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
		dmabuf_page_pool_list_del(system_heap->pools[i]);
		dmabuf_page_pool_list_add_sys(system_heap->pools[i]);
#endif
	}

#ifdef CONFIG_DMABUF_NPU_HUGE_POOL
	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		system_heap->huge_pools[i] = dmabuf_page_pool_create(order_flags[i], orders[i]);
		if (!system_heap->huge_pools[i]) {
			pr_err("%s: page huge pool creation failed!\n", __func__);
			goto destroy_pools;
		}
		system_heap->huge_pools[i]->reclaim_flag = DMABUF_PAGE_POOL_RECLAME_ALL;
	}
#endif

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		system_heap->async_pools[i] = dmabuf_page_pool_create(order_flags[i], orders[i]);
		if (!system_heap->async_pools[i]) {
			pr_err("%s: async_pools create failed!\n", __func__);
			goto destroy_pools;
		}
		system_heap->async_pools[i]->reclaim_flag = DMABUF_PAGE_POOL_RECLAME_ALL;
		dmabuf_page_pool_list_del(system_heap->async_pools[i]);
	}
#endif

	ret = dma_heap_add(&system_heap->heap);
	if (ret) {
		pr_err("%s: dma_heap_add[%s] failed!\n", __func__, system_heap->name);
		goto destroy_pools;
	}
	dma_coerce_mask_and_coherent(dma_heap_get_dev(&system_heap->heap),
						DMA_BIT_MASK(64));

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY
	for (i = 0; i < ARRAY_SIZE(system_heap->nhs); i++)
		BLOCKING_INIT_NOTIFIER_HEAD(&system_heap->nhs[i]);
#endif /* CONFIG_DMABUF_HEAPS_SYSTEM_NOTIFY */

	pr_err("system_heap [%s] info: heap create succ\n", system_heap->name);

	init_waitqueue_head(&system_heap->sys_pool_wait);
	system_heap->sys_pool_thread = kthread_run(dma_pool_kthread, system_heap,
						"%s", "dma_pool");
	if (IS_ERR(system_heap->sys_pool_thread)) {
		pr_err("%s: kthread_create failed!\n", __func__);
		goto destroy_pools;
	}

	dma_kobj = kobject_create_and_add("dma_pool", kernel_kobj);
	if (!dma_kobj)
		pr_err("kobject_create_and_add fail!\n");

	err = sysfs_create_group(dma_kobj, &dma_pool_attr_group);
	if (err)
		pr_err("create dma_pool_attr_group fail!\n");

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	atomic_set(&system_heap->sys_pool_alloc_wait_flag, 0);
	init_waitqueue_head(&system_heap->sys_pool_alloc_wait);
	system_heap->sys_pool_alloc_task = kthread_run(
		sys_pool_alloc_kthread, system_heap, "%s", "sys_pool_alloc_task");
	if (IS_ERR(system_heap->sys_pool_alloc_task)) {
		pr_err("%s: create sys_pool_alloc_task failed!\n", __func__);
		system_heap->sys_pool_alloc_task = NULL;
		goto destroy_pools;
	}
	set_user_nice(system_heap->sys_pool_alloc_task, PRIO_TO_NICE(DEFAULT_PRIO));
	atomic_set(&system_heap->sys_pool_alloc_prio, DEFAULT_PRIO);

	err = sysfs_create_group(dma_kobj, &dma_pool_wmark_attr_group);
	if (err)
		pr_err("create dma_pool_wmark_attr_group failed!\n");
#endif

	mutex_init(&system_heap->pool_lock);

	sys_heap = system_heap;

	system_heap_create_ctl_dev();
	return 0;

destroy_pools:
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		if (!system_heap->async_pools[i])
			continue;
		dmabuf_page_pool_destroy(system_heap->async_pools[i]);
	}
#endif

#ifdef CONFIG_DMABUF_NPU_HUGE_POOL
	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		if (!system_heap->huge_pools[i])
			continue;
		dmabuf_page_pool_destroy(system_heap->huge_pools[i]);
	}
#endif

	for (i = 0; i < SYS_NUM_ORDERS; i++) {
		if (!system_heap->pools[i])
			continue;
		dmabuf_page_pool_destroy(system_heap->pools[i]);
	}

free_heap:
	kfree(system_heap);

	return -EINVAL;
}

static int system_heap_init(void)
{
	int ret = 0;
	struct device_node *sys_np = NULL, *child_np = NULL;

	if (!dma_heap_node) {
		pr_err("%s, can not find dma_heap node\n", __func__);
		return -1;
	}

	sys_np = of_get_child_by_name(dma_heap_node, "system_heap");
	if (!sys_np) {
		pr_err("%s, can not find system_heap node\n", __func__);
		return -1;
	}

	for_each_available_child_of_node(sys_np, child_np) {
		ret = system_heap_create(child_np);
		if (ret)
			pr_err("%s, create[%s] fail\n", __func__, child_np->name);
	}

	return 0;
}

module_init(system_heap_init);
MODULE_LICENSE("GPL v2");
