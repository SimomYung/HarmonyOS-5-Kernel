/*
 * drivers/dma-buf/heaps/mm_cpa_heap.c
 *
 * Copyright(C) 2022 Hisilicon Technologies Co., Ltd. All rights reserved.
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
#define pr_fmt(fmt) "[cpa_heap]" fmt

#include <linux/device.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/freezer.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <linux/of_address.h>
#include <linux/highmem.h>
#include <linux/oom.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/sysctl.h>
#include <linux/workqueue.h>
#include <linux/compaction.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#include "deferred-free-helper.h"
#include "mm_tee_op.h"
#include "mm_sec_alloc.h"
#include "mm_heap_priv.h"
#include "mm_heap_helpers.h"

#include "page_pool.h"

#define CPA_SIZE_320M_PAGE		81920
#define CPA_MAX_KILL_NUM		20
#define CPA_WATER_MARK_RATIO	50
#define CPA_RK_RATIO			5
#define CPA_RK_RATIO_BY_ZRAM	5
#define CPA_ERR_LVL				0
#define CPA_DBG_LVL				1
#define CPA_KP_FREEMEM_MODEL	0
#define CPA_KP_ZRAM_MODEL		1
#define TOTAL_RAM_PAGES_1G	(1 << 18)

static unsigned int orders[] = {9, 0};

#define CPA_NUM_ORDERS    ARRAY_SIZE(orders)

struct cpa_page_info {
	struct list_head pages;
	unsigned long num;
};

struct cpa_heap {
	struct dma_heap heap;
	const char *name;
	phys_addr_t heap_base;
	size_t heap_size;
	atomic64_t alloc_size;
	struct dmabuf_page_pool *pools[CPA_NUM_ORDERS];
	/* heap mutex */
	struct mutex mutex;
	struct dma_heap_sec_tee tee;
	int ta_init;
};

struct cpa_work_data {
	struct work_struct work;
	struct cpa_heap *cpa_heap;
};

#define cpa_debug(level, x...)			\
	do {					\
		if (dma_cpa_debug_level >= (level))	\
			pr_info(x);		\
	} while (0)

#define to_cpa_heap(x) container_of(x, struct cpa_heap, heap)

static unsigned int zram_percentage = 100;
static short cpa_process_adj = 500;
static int dma_cpa_debug_level;
static gfp_t high_order_gfp_flags = (GFP_KERNEL | __GFP_ZERO | __GFP_NOWARN |
				     __GFP_NORETRY) & ~__GFP_DIRECT_RECLAIM;
static gfp_t low_order_gfp_flags = GFP_KERNEL | __GFP_NOWARN;
static struct cpa_work_data *cpa_mem_work_data;

#ifndef CONFIG_LIBLINUX
static void cpa_dump_tasks(void)
{
	struct task_struct *p = NULL;
	struct task_struct *task = NULL;
	short tsk_oom_adj = 0;
	unsigned long tsk_nr_ptes;
	int task_state = 0;
	char frozen_mark = ' ';

	cpa_debug(CPA_DBG_LVL,
		"[ pid ]  uid  tgid  total_vm  rss nptes  swap  adjs  name  frozen_status\n");

	rcu_read_lock();
	for_each_process(p) {
		task = find_lock_task_mm(p);
		if (!task)
			continue;

		tsk_oom_adj = task->signal->oom_score_adj;
		tsk_nr_ptes = (unsigned long)atomic_long_read(&task->mm->pgtables_bytes);
		task_state = (int)task_state_index(task);
		frozen_mark = frozen(task) ? '*' : ' ';

		cpa_debug(CPA_DBG_LVL,
			"[%5d] %5d %5d %8lu %6lu %5lu %5lu %5hd %c %s%c\n",
			task->pid, from_kuid(&init_user_ns, task_uid(task)),
			task->tgid, task->mm->total_vm, get_mm_rss(task->mm),
			tsk_nr_ptes,
			get_mm_counter(task->mm, MM_SWAPENTS),
			tsk_oom_adj,
			task_state,
			task->comm,
			frozen_mark);
		task_unlock(task);
	}
	rcu_read_unlock();
}

static void cpa_show_mem(int debug_lev, const char *show_msg)
{
	cpa_debug(debug_lev, "cpa show mem: %s\n", show_msg);
	if (debug_lev <= dma_cpa_debug_level)
		show_mem(0, NULL);
}

static unsigned long cpa_free_mem_page(void)
{
	unsigned long free_pages = global_zone_page_state(NR_FREE_PAGES);
	unsigned long free_cma_pages = global_zone_page_state(NR_FREE_CMA_PAGES);
	unsigned long max = totalreserve_pages > free_cma_pages ?
				totalreserve_pages : free_cma_pages;

	if (free_pages > max)
		free_pages -= max;
	else
		free_pages = 0;

	return free_pages;
}

static inline unsigned long get_free_page(void)
{
	return cpa_free_mem_page();
}

static inline unsigned long get_file_page(void)
{
	return global_node_page_state(NR_FILE_PAGES) -
		    global_node_page_state(NR_SHMEM) -
		    global_node_page_state(NR_UNEVICTABLE) -
		    total_swapcache_pages();
}

static inline unsigned long get_anon_page(void)
{
	return global_node_page_state(NR_ACTIVE_ANON) +
			global_node_page_state(NR_INACTIVE_ANON);
}

static inline unsigned long get_zram_page(void)
{
	return (unsigned long)(total_swap_pages - get_nr_swap_pages());
}

static void cpa_direct_dropcache(void)
{
	cpa_drop_pagecache();
	drop_slab();
}

/* Compact all nodes in the system */
static void cpa_direct_compact(int model, unsigned int order,
				unsigned long nrpages)
{
	cpa_compact_nodes(model, (int)order, nrpages);
}

static unsigned long cpa_reclaim_anon2zram(unsigned long nr_to_reclaim)
{
	return cpa_shrink_all_memory(nr_to_reclaim);
}

static void cpa_selected(struct task_struct *selected,
			unsigned long selected_task_size)
{
	unsigned long other_free = cpa_free_mem_page();
	unsigned long other_file = get_file_page();
	unsigned long other_anon = get_anon_page();

	task_lock(selected);
	send_sig(SIGKILL, selected, 0);
	task_unlock(selected);

	cpa_debug(CPA_DBG_LVL, "%s : Killing '%s' (%d), tgid=%d, adj %hd\n"
		  "   to free %ldkB on behalf of '%s' (%d) when\n"
		  "   free %lukB  file %lukB  anon %lukB\n",
		  __func__,
		  selected->comm, selected->pid, selected->tgid,
		  selected->signal->oom_score_adj,
		  selected_task_size * (long)(PAGE_SIZE / 1024),
		  current->comm, current->pid,
		  other_free * (PAGE_SIZE / 1024),
		  other_file * (PAGE_SIZE / 1024),
		  other_anon * (PAGE_SIZE / 1024));
}

static unsigned long cpa_direct_kill(int model, unsigned long reclaim_by_kill)
{
	struct task_struct *p = NULL;
	struct task_struct *tsk = NULL;
	struct task_struct *selected = NULL;
	short oom_score_adj = 0;
	unsigned long selected_task_size = 0;
	int nr_to_kill = CPA_MAX_KILL_NUM;
	unsigned long task_size = 0;
	unsigned long rem = 0;

	rcu_read_lock();
	do {
		for_each_process(tsk) {
			if (tsk->flags & PF_KTHREAD)
				continue;

			p = find_lock_task_mm(tsk);
			if (!p)
				continue;

			if (selected == p) {
				task_unlock(p);
				continue;
			}

			if (test_tsk_thread_flag(p, TIF_MEMDIE)) {
				task_unlock(p);
				continue;
			}

			if ((unsigned long)p->state & TASK_UNINTERRUPTIBLE) {
				task_unlock(p);
				cpa_debug(CPA_DBG_LVL, "[%s]filter D state process: %d (%s) state:0x%lx\n",
					__func__, p->pid, p->comm, p->state);
				continue;
			}

			oom_score_adj = p->signal->oom_score_adj;
			if (oom_score_adj < cpa_process_adj) {
				task_unlock(p);
				continue;
			}

			if (!model)
				task_size = get_mm_rss(p->mm);
			else
				task_size = get_mm_counter(p->mm, MM_SWAPENTS);

			task_unlock(p);

			if (task_size <= 0)
				continue;

			if (selected && (task_size < selected_task_size))
					continue;

			oom_score_adj = 0;
			selected = p;
			selected_task_size = task_size;
		}

		if (selected) {
			cpa_selected(selected, selected_task_size);
			rem += selected_task_size;
		}

		p = NULL;
		tsk = NULL;
		selected_task_size = 0;
	} while ((rem < reclaim_by_kill) && (--nr_to_kill));

	rcu_read_unlock();

	return rem;
}
#endif

static inline unsigned int order_to_size(unsigned int order)
{
	return (PAGE_SIZE << order);
}

static int order_to_index(unsigned int order)
{
	int i;

	for (i = 0; i < CPA_NUM_ORDERS; i++)
		if (order == orders[i])
			return i;

	return -EINVAL;
}

static void __cpa_heap_buf_free(struct deferred_freelist_item *item,
				 enum df_reason reason)
{
	struct mm_heap_helper_buffer *buffer = NULL;

	buffer = container_of(item, struct mm_heap_helper_buffer, deferred_free);

	if (buffer->vmap_cnt > 0) {
		pr_warn_once("%s: buffer still mapped in the kernel\n",
			     __func__);
		buffer->heap->ops->unmap_kernel(buffer->heap, buffer);
	}
	buffer->heap->ops->free(buffer);

	kfree(buffer);
}

static void cpa_heap_buf_free(struct mm_heap_helper_buffer *buffer)
{
	int npages = PAGE_ALIGN(buffer->size) / PAGE_SIZE;

	deferred_free(&buffer->deferred_free, __cpa_heap_buf_free, npages);
}

static struct page *alloc_buffer_page(struct cpa_heap *cpa_heap,
				unsigned int order, unsigned int is_nofail)
{
	struct dmabuf_page_pool *pool = NULL;
	struct page *page = NULL;
	int index = 0;

	index = order_to_index(order);
	if (index < 0)
		return NULL;

	pool = cpa_heap->pools[index];
	if (is_nofail && (order <= orders[1]))
		pool->gfp_mask = (pool->gfp_mask | __GFP_NOFAIL) &
							(~__GFP_NORETRY);

	/*
	 * if need to use page pool;
	 * now using the alloc func as below:
	 * page = dmabuf_page_pool_alloc(pool);
	 */
	page = dmabuf_page_pool_alloc(pool);

	return page;
}

static void free_buffer_page(struct cpa_heap *cpa_heap,
		struct mm_heap_helper_buffer *buffer, struct page *page)
{
	struct dmabuf_page_pool *pool = NULL;
	unsigned int order = compound_order(page);
	int index = 0;

	index = order_to_index(order);
	if (index < 0)
		return;

	pool = cpa_heap->pools[index];

	/*
	 * if need to use page pool;
	 * now using the alloc func as below:
	 * dmabuf_page_pool_free(pool, page);
	 */
	__free_pages(page, pool->order);
}

static struct page *alloc_largest_available(struct cpa_heap *cpa_heap,
					    unsigned long ac_sz,
					    unsigned int max_order,
					    unsigned int is_nofail)
{
	int i;
	struct page *page = NULL;

	for (i = 0; i < CPA_NUM_ORDERS; i++) {
		if (ac_sz < order_to_size(orders[i]))
			continue;
		if (max_order < orders[i])
			continue;

		page = alloc_buffer_page(cpa_heap, orders[i], is_nofail);
		if (!page)
			continue;

		return page;
	}

	return NULL;
}

static unsigned long cpa_alloc_large(struct cpa_heap *cpa_heap,
				     struct cpa_page_info *info,
				     unsigned long size,
				     unsigned int is_nofail)
{
	struct page *page = NULL;
	unsigned int max_order = orders[0];
	unsigned long size_remaining = size;

	while (size_remaining > 0) {
		page = alloc_largest_available(cpa_heap,
					       size_remaining,
					       max_order,
					       is_nofail);
		if (!page)
			break;
		max_order = compound_order(page);
		list_add_tail(&page->lru, &info->pages);
		info->num++;
		size_remaining -= (PAGE_SIZE << max_order);
	}

	return (size - size_remaining);
}

static int __cpa_heap_input_check(struct cpa_heap *cpa_heap,
				  unsigned long size, unsigned long flags)
{
	long alloc_size;

	alloc_size = atomic64_read(&cpa_heap->alloc_size);
	if ((alloc_size + size) > cpa_heap->heap_size) {
		pr_err("alloc size = 0x%lx, size = 0x%lx, heap size = 0x%lx\n",
		       alloc_size, size,
		       cpa_heap->heap_size);
		return -EINVAL;
	}

	if (size > SZ_64M) {
		pr_err("size too large! size 0x%lx, per_alloc_sz 0x4000000(64M)\n",
		       size);
		return -EINVAL;
	}

	if (dma_buf_is_unsecure(flags)) {
		pr_err("invalid alloc flag in cpa heap flag(0x%lx)\n", flags);
		return -EINVAL;
	}

	return 0;
}

static int change_cpa_prop(struct cpa_heap *cpa_heap,
			   struct mm_heap_helper_buffer *buffer, u32 cmd)
{
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct tz_pageinfo *pageinfo = NULL;
	struct mem_chunk_list mcl;
	unsigned int nents = table->nents;
	unsigned int i = 0;
	int ret = 0;

	if (!cpa_heap->ta_init) {
		pr_err("[%s] TA not inited.\n", __func__);
		return -EINVAL;
	}

	if (cmd == DMA_HEAP_SEC_CMD_ALLOC) {
		pageinfo = kzalloc(nents * sizeof(*pageinfo), GFP_KERNEL);
		if (!pageinfo)
			return -ENOMEM;

		for_each_sg(table->sgl, sg, table->nents, i) {
			page = sg_page(sg);
			pageinfo[i].addr = page_to_phys(page);
			pageinfo[i].nr_pages = sg->length / PAGE_SIZE;
		}

		mcl.phys_addr = (void *)pageinfo;
		mcl.nents = nents;
		mcl.protect_id = SEC_TASK_DRM;
#ifdef CONFIG_MM_SECMEM_DRM_TEE_API_SUPPORT
		if (buffer->flags & DMA_HEAP_FLAGS_DRM_TEE_MASK)
			mcl.ta_rw_index = DMA_HEAP_FLAGS_2_TA_RW_INDEX(buffer->flags);
#endif
	} else if (cmd == DMA_HEAP_SEC_CMD_FREE) {
		mcl.protect_id = SEC_TASK_DRM;
		mcl.buff_id = buffer->id;
		mcl.phys_addr = NULL;
	} else {
		pr_err("[%s] Error cmd\n", __func__);
		return -EINVAL;
	}

	ret = secmem_tee_exec_cmd(cpa_heap->tee.session, &mcl, cmd);
	if (ret) {
		pr_err("[%s] Exec cmd[%d] fail\n", __func__, cmd);
		ret = -EINVAL;
	} else {
		if (cmd == DMA_HEAP_SEC_CMD_ALLOC)
			buffer->id = mcl.buff_id;
	}

	kfree(pageinfo);

	return ret;
}

static int cpa_prepare_session(struct cpa_heap *cpa_heap)
{
	int ret = 0;

	if (!cpa_heap->ta_init) {
		ret = sec_tee_init(&cpa_heap->tee, DMA_HEAP_SESSIONS_SECMEM);
		if (ret) {
			pr_err("[%s] TA init failed\n", __func__);
			goto err;
		}

		cpa_heap->ta_init = 1;
	}

err:
	return ret;
}

static inline unsigned long cpa_water_mark_page(struct cpa_heap *cpa_heap)
{
	unsigned long alloc_size = (unsigned long)atomic64_read(&cpa_heap->alloc_size);

	return (((cpa_heap->heap_size - alloc_size) * 100 / CPA_WATER_MARK_RATIO) >> PAGE_SHIFT);
}

#ifndef CONFIG_LIBLINUX
static unsigned long get_zram_water_mark_page(void)
{
	unsigned int cpa_zram_ratio;

	if (total_swap_pages > (totalram_pages() * zram_percentage / 100))
		cpa_zram_ratio = 20;
	else
		cpa_zram_ratio = 75;

	cpa_debug(CPA_DBG_LVL, "[%s] cpa_zram_ratio: %u\n",
			__func__, cpa_zram_ratio);

	return (total_swap_pages * cpa_zram_ratio / 100);
}

static void cpa_prepare_memory(struct cpa_heap *cpa_heap,
				int compact_model,
				unsigned long water_mark_page,
				unsigned long tokill_rem_page,
				unsigned long toreclaim_page,
				unsigned long *kill_rem_page,
				unsigned long *reclaimed_page)
{
	unsigned long free_page;
	unsigned long file_page;
	unsigned long file_water_mark_page;
	unsigned long zram_page;
	unsigned long zram_water_mark_page;
	unsigned long nr_expect;

	cpa_show_mem(CPA_DBG_LVL, "start cpa prepare memory\n");

	/**
	 * 1.dropcache
	 */
	file_water_mark_page = CPA_SIZE_320M_PAGE;
	free_page = get_free_page();
	file_page = get_file_page();

	cpa_debug(CPA_DBG_LVL, "[%s] step dropcache before, free_page: %lu,"
		  " water_mark_page: %lu, file_page: %lu, file_page_water_mark: %lu\n",
		  __func__, free_page, water_mark_page,
		  file_page, file_water_mark_page);

	if ((free_page < water_mark_page) &&
	    (file_page > file_water_mark_page)) {
		cpa_direct_dropcache();
		cpa_show_mem(CPA_DBG_LVL, "after dropcache\n");
	}

	/**
	 * 2.kill process
	 */
	zram_page = get_zram_page();
	zram_water_mark_page = get_zram_water_mark_page();

	cpa_debug(CPA_DBG_LVL, "[%s] step kill proc before, zram_page: %lu, zram_water_mark_page: %lu\n",
		  __func__, zram_page, zram_water_mark_page);

	if (zram_page > zram_water_mark_page) {
		*kill_rem_page = cpa_direct_kill(CPA_KP_ZRAM_MODEL,
						 tokill_rem_page);
		if (!*kill_rem_page) {
			cpa_show_mem(CPA_ERR_LVL,
				     "prepare memory kill process fail\n");
			cpa_dump_tasks();
		}

		cpa_show_mem(CPA_DBG_LVL, "after kill process\n");
	}

	/**
	 * 3.reclaim memory
	 *
	 */
	free_page = get_free_page();
	cpa_debug(CPA_DBG_LVL, "[%s] step reclaim before, free_page: %lu, water_mark_page: %lu\n",
		  __func__, free_page, water_mark_page);

	if (free_page < water_mark_page) {
		*reclaimed_page = cpa_reclaim_anon2zram(toreclaim_page);
		if (!*reclaimed_page)
			cpa_show_mem(CPA_ERR_LVL,
				     "prepare memory reclaim memory fail\n");

		cpa_show_mem(CPA_DBG_LVL, "after reclaim\n");
	}

	/*
	 * 4.compact nodes
	 */
	if (*kill_rem_page || *reclaimed_page) {
		nr_expect = toreclaim_page >> orders[1];
		cpa_debug(CPA_DBG_LVL, "[%s] compact nr_expect page: %lu\n",
			__func__, nr_expect);

		cpa_direct_compact(compact_model, orders[1], nr_expect);
	}

	cpa_show_mem(CPA_DBG_LVL, "finish cpa prepare memory\n");
}

static void cpa_prepare_memory_for_work(struct work_struct *work)
{
	unsigned long water_mark_page = 0;
	unsigned long free_page = 0;
	unsigned long tokill_rem_page = 0;
	unsigned long toreclaim_page = 0;
	unsigned long kill_rem_page = 0;
	unsigned long reclaimed_page = 0;
	struct cpa_work_data *work_data = container_of(work, struct cpa_work_data, work);

	water_mark_page = cpa_water_mark_page(work_data->cpa_heap);

	free_page = get_free_page();
	if (free_page < water_mark_page)
		toreclaim_page = water_mark_page - free_page;

	tokill_rem_page = CPA_RK_RATIO_BY_ZRAM * toreclaim_page;

	cpa_prepare_memory(work_data->cpa_heap, 1, water_mark_page,
			    tokill_rem_page, toreclaim_page,
			   &kill_rem_page, &reclaimed_page);

	cpa_debug(CPA_DBG_LVL, "[%s] kill process free mempage: %lu, reclaimed mempage: %lu\n",
		  __func__, kill_rem_page, reclaimed_page);
}

static void cpa_prepare_memory_for_alloc(struct cpa_heap *cpa_heap,
					unsigned long size,
					unsigned long *kill_rem_page,
					unsigned long *reclaimed_page)
{
	unsigned long water_mark_page;
	unsigned long tokill_rem_page;
	unsigned long toreclaim_page;

	water_mark_page = cpa_water_mark_page(cpa_heap);
	tokill_rem_page = (size / PAGE_SIZE);
	toreclaim_page = tokill_rem_page * 100 / CPA_WATER_MARK_RATIO;

	cpa_prepare_memory(cpa_heap, 0, water_mark_page,
			   tokill_rem_page, toreclaim_page,
			   kill_rem_page, reclaimed_page);

	cpa_debug(CPA_DBG_LVL, "[%s] kill process free mempage:%lu, reclaimed mempage:%lu\n",
			__func__, *kill_rem_page, *reclaimed_page);
}

static int do_size_remaining(unsigned long size_remaining, unsigned long size,
	struct cpa_heap *cpa_heap, struct cpa_page_info *info, bool flag,
	unsigned long kill_rem_page, unsigned long reclaimed_page)
{
	unsigned long retry_to_rk_page = 0;
	unsigned long retry_kill_rem_page = 0;
	unsigned long retry_reclaimed_page = 0;
	unsigned long nr_expect = 0;

	if (size_remaining) {
		retry_to_rk_page = size / PAGE_SIZE;

		retry_kill_rem_page = cpa_direct_kill(CPA_KP_FREEMEM_MODEL,
						      retry_to_rk_page);
		if (!retry_kill_rem_page)
			cpa_debug(CPA_ERR_LVL, "[%s]retry kill process but no free\n",
				  __func__);

		retry_reclaimed_page = cpa_reclaim_anon2zram(retry_to_rk_page);
		if (!retry_reclaimed_page)
			cpa_debug(CPA_ERR_LVL, "[%s]retry alloc to reclaim page: no mempage to reclaim\n",
				  __func__);
		nr_expect = retry_to_rk_page >> orders[1];
		cpa_direct_compact(1, orders[1], nr_expect);

		size_remaining -= cpa_alloc_large(cpa_heap, info,
						  size_remaining, flag);
	}

	if (size_remaining) {
		pr_err("[%s] No enough memory to alloc! size_remaining: %lx Byte\n",
		       __func__, size_remaining);

		pr_err("[%s] prepare memory argument show kill freedpage: %lu, reclaimed mempage: %lu\n",
		       __func__, kill_rem_page, reclaimed_page);

		pr_err("[%s] retry alloc -- need free mempage:%lu; kill proc freepage: %lu, reclaimed mempage: %lu\n",
		       __func__, retry_to_rk_page,
		       retry_kill_rem_page, retry_reclaimed_page);

		cpa_show_mem(CPA_ERR_LVL, "cpa alloc fail\n");

		retry_kill_rem_page = cpa_direct_kill(CPA_KP_FREEMEM_MODEL,
					CPA_RK_RATIO * retry_to_rk_page);
		cpa_debug(CPA_ERR_LVL, "[%s]No enough mem kill process to free mempage: %lu, free mempage: %lu\n",
			__func__, (CPA_RK_RATIO * retry_to_rk_page),
			  retry_kill_rem_page);

		queue_work(system_power_efficient_wq, &cpa_mem_work_data->work);

		return -EINVAL;
	}

	return 0;
}
#else /* CONFIG_LBLINUX */
static void cpa_prepare_memory_for_work(struct work_struct *work)
{
	struct cpa_work_data *work_data = container_of(work, struct cpa_work_data, work);
	unsigned long water_mark_page = cpa_water_mark_page(work_data->cpa_heap);
	struct liblinux_pal_cpa_prepare_memory_args args = {
		.water_mark_page = water_mark_page,
		.heap_size = work_data->cpa_heap->heap_size,
		.allocated_size = atomic64_read(&work_data->cpa_heap->alloc_size),
		.order = orders[1],
	};
	/*
	 * compat_mode = 1
	 * water_mark_page
	 * tokill_rem_page = f1(CPA_RK_RATIO_BY_ZRAM, water_mark_page, free_page)
	 * toreclaim_page = f2(water_mark_page, free_page)
	 *
	 * free_page is provide by sysmgr itself
	 *
	 * cpa_prepare_memory = drop + kill(CPA_KP_ZRAM_MODEL) + reclaim + compat
	 * */
	liblinux_cpa_prepare_memory_for_work(&args);
}

static void cpa_prepare_memory_for_alloc(struct cpa_heap *cpa_heap,
					unsigned long size,
					unsigned long *kill_rem_page,
					unsigned long *reclaimed_page)
{
	unsigned long water_mark_page = cpa_water_mark_page(cpa_heap);
	struct liblinux_pal_cpa_prepare_memory_args args = {
		.water_mark_page = water_mark_page,
		.heap_size = cpa_heap->heap_size,
		.allocated_size = atomic64_read(&cpa_heap->alloc_size),

		.alloc_size = size,
		.order = orders[1],
	};

	/*
	 * output parameters are only used for logging
	 *
	 * compat_mode = 0
	 * water_mark_page
	 * tokill_rem_page = f3(size, PAGE_SIZE)
	 * toreclaim_page = f4(size, PAGE_SIZE, 100, CPA_WATER_MARK_RATIO)
	 *
	 * cpa_prepare_memory = drop + kill(CPA_KP_ZRAM_MODEL) + reclaim + compat
	 */
	liblinux_cpa_prepare_memory_for_alloc(&args);
}

static int do_size_remaining(unsigned long size_remaining, unsigned long size,
	struct cpa_heap *cpa_heap, struct cpa_page_info *info, bool flag,
	unsigned long kill_rem_page, unsigned long reclaimed_page)
{
	if (size_remaining) {
		struct liblinux_pal_cpa_prepare_memory_args args = {
			.heap_size = cpa_heap->heap_size,
			.allocated_size = atomic64_read(&cpa_heap->alloc_size),

			.alloc_size = size,
			.order = orders[1],
		};
		/*
		 * retry_to_rk_page = f5(size, PAGE_SIZE)
		 * kill(CPA_KP_FREEMEM_MODEL, retry_to_rk_page) + reclaim + compact(1, orders[1], retry_to_rk_page)
		 */
		liblinux_cpa_prepare_memory_for_remaining(&args);
		size_remaining -= cpa_alloc_large(cpa_heap, info,
						  size_remaining, flag);
	}

	if (size_remaining) {
		pr_err("[%s] No enough memory to alloc! size_remaining: %lx Byte\n",
		       __func__, size_remaining);

		queue_work(system_power_efficient_wq, &cpa_mem_work_data->work);

		return -EINVAL;
	}

	return 0;
}
#endif /* CONFIG_LIBLINUX */

static int clear_cpa_prop(struct mm_heap_helper_buffer *buffer,
				struct cpa_heap *cpa_heap)
{
	int ret;

	ret = change_cpa_prop(cpa_heap, buffer, DMA_HEAP_SEC_CMD_FREE);
	if (ret)
		pr_err("%s, unconfig cpa_prop fail!\n", __func__);

	return ret;
}

static int do_cpa_prop(struct mm_heap_helper_buffer *buffer,
				struct cpa_heap *cpa_heap)
{
	int ret;

#ifndef CONFIG_MM_LB_FULL_COHERENCY
#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
	dma_heap_flush_all_cpus_caches_raw();
#else
	dma_heap_flush_all_cpus_caches();
#endif
#else
	dma_ci_sgt(buffer->sg_table);
#endif

	ret = change_cpa_prop(cpa_heap, buffer, DMA_HEAP_SEC_CMD_ALLOC);
	if (ret)
		pr_err("%s, config cpa_prop fail!\n", __func__);

	return ret;
}

static struct dma_buf *cpa_heap_allocate(struct dma_heap *heap,
			unsigned long size, unsigned long fd_flags,
			unsigned long heap_flags)
{
	struct page *page = NULL;
	struct page *tmp_page = NULL;
	struct scatterlist *sg = NULL;
	struct sg_table *table = NULL;
	struct cpa_heap *cpa_heap = to_cpa_heap(heap);
	struct mm_heap_helper_buffer *buffer = NULL;
	struct dma_buf *dmabuf = NULL;
	struct cpa_page_info info;
	unsigned long kill_rem_page = 0;
	unsigned long reclaimed_page = 0;
	/*lint -e666 */
	unsigned long size_remaining = ALIGN(size, order_to_size(orders[1]));
	/*lint +e666 */
	unsigned int i = 0;
	int ret = 0;

	cpa_debug(CPA_DBG_LVL, "%s, [%s]alloc: 0x%lx, already alloc_size: 0x%lx\n",
		  __func__, cpa_heap->name, size, atomic64_read(&cpa_heap->alloc_size));

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

	init_mm_heap_helper_buffer(buffer, cpa_heap_buf_free);
	buffer->heap = heap;
	buffer->size = size;
	buffer->flags = heap_flags;
	buffer->magic = atomic64_inc_return(&dmaheap_magic);

	mutex_lock(&cpa_heap->mutex);
	ret = __cpa_heap_input_check(cpa_heap, size, heap_flags);
	if (ret) {
		pr_err("cpa alloc input check: params err!\n");
		goto out;
	}

	info.num = 0;
	INIT_LIST_HEAD(&info.pages);

	queue_work(system_power_efficient_wq, &cpa_mem_work_data->work);

	/*
	 * 1.cpa prepare session for the first time
	 */
	ret = cpa_prepare_session(cpa_heap);
	if (ret)
		goto out;

	/*
	 * 2.cpa prepare memory for remain size alloc
	 */
	cpa_prepare_memory_for_alloc(cpa_heap, size_remaining,
				     &kill_rem_page, &reclaimed_page);

	size_remaining -= cpa_alloc_large(cpa_heap,
					  &info, size_remaining, false);
	ret = do_size_remaining(size_remaining, size, cpa_heap, &info, false,
					kill_rem_page, reclaimed_page);
	if (ret)
		goto free_pages;

	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		goto free_pages;

	ret = sg_alloc_table(table, info.num, GFP_KERNEL);
	if (ret)
		goto free_table;

	sg = table->sgl;
	list_for_each_entry_safe(page, tmp_page, &info.pages, lru) {
#ifdef CONFIG_NEED_CHANGE_MAPPING
		change_secpage_range(page_to_phys(page),
				     (unsigned long)page_address(page),
				     PAGE_SIZE << compound_order(page),
				     __pgprot(PROT_DEVICE_nGnRE));
#endif
		sg_set_page(sg, page, PAGE_SIZE << compound_order(page), 0);
		sg_dma_address(sg) = sg_phys(sg);
		sg_dma_len(sg) = sg->length;
		sg = sg_next(sg);
		list_del(&page->lru);
	}

#ifdef CONFIG_NEED_CHANGE_MAPPING
	flush_tlb_all();
#endif
	buffer->sg_table = table;

	ret = do_cpa_prop(buffer, cpa_heap);
	if (ret)
		goto free_sg_table;

	atomic64_add(size, &cpa_heap->alloc_size);
	mutex_unlock(&cpa_heap->mutex);

	/* create the dmabuf */
	dmabuf = mm_heap_helper_export_dmabuf(buffer, fd_flags);
	if (IS_ERR(dmabuf))
		goto unconfig_prop;
	buffer->dmabuf = dmabuf;

	if (dma_buf_is_unsecure(heap_flags))
		dma_heap_flush_buffer_cache(buffer, heap_flags);

	mm_dmaheap_init_dump(buffer);
	mm_dmaheap_buffer_add(buffer);
	cpa_debug(CPA_DBG_LVL, "%s, succ -- alloc: 0x%lx\n", __func__, size);

	return dmabuf;

unconfig_prop:
	mutex_lock(&cpa_heap->mutex);
	ret = clear_cpa_prop(buffer, cpa_heap);
	if (ret) {
		mutex_unlock(&cpa_heap->mutex);
		pr_err("%s, [%s]alloc fail, unconfig prop fail!\n",
					__func__, cpa_heap->name);
		return ERR_PTR(-EINVAL);
	}
free_sg_table:
	for_each_sg(table->sgl, sg, table->nents, i) {
#ifdef CONFIG_NEED_CHANGE_MAPPING
		change_secpage_range(page_to_phys(sg_page(sg)),
				     (unsigned long)page_address(sg_page(sg)),
				     sg->length, PAGE_KERNEL);
		flush_tlb_all();
#endif
		free_buffer_page(cpa_heap, buffer, sg_page(sg));
	}
#ifdef CONFIG_NEED_CHANGE_MAPPING
		flush_tlb_all();
#endif
	sg_free_table(table);
free_table:
	kfree(table);
free_pages:
	list_for_each_entry_safe(page, tmp_page, &info.pages, lru)
		free_buffer_page(cpa_heap, buffer, page);
out:
	mutex_unlock(&cpa_heap->mutex);
	kfree(buffer);
	pr_err("%s: [%s]alloc fail, size[0x%lx], heap all alloc_size[0x%lx], heap_size[0x%lx]\n",
			__func__, cpa_heap->name, size,
			atomic64_read(&cpa_heap->alloc_size), cpa_heap->heap_size);

	return ERR_PTR(-EINVAL);
}

static void __cpa_heap_free(struct mm_heap_helper_buffer *buffer)
{
	struct cpa_heap *cpa_heap = to_cpa_heap(buffer->heap);
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	unsigned long size = 0;
	int i = 0;

	/* add to cpa pool */
	for_each_sg(table->sgl, sg, table->nents, i) {
#ifdef CONFIG_NEED_CHANGE_MAPPING
		change_secpage_range(page_to_phys(sg_page(sg)),
				     (unsigned long)page_address(sg_page(sg)),
				     sg->length, PAGE_KERNEL);
		flush_tlb_all();
#endif
		free_buffer_page(cpa_heap, buffer, sg_page(sg));
	}

	/* shrink to buddy */
	size = buffer->size;
	atomic64_sub(size, &cpa_heap->alloc_size);
	if (!atomic64_read(&cpa_heap->alloc_size)) {
		for (i = 0; i < CPA_NUM_ORDERS; i++)
			(void)dmabuf_page_pool_do_shrink(cpa_heap->pools[i],
				0, (int)(cpa_heap->heap_size >> PAGE_SHIFT));
	}
}

static void cpa_heap_free(struct mm_heap_helper_buffer *buffer)
{
	struct cpa_heap *cpa_heap = to_cpa_heap(buffer->heap);
	struct sg_table *table = buffer->sg_table;
	unsigned long size = buffer->size;
	int ret = 0;
	int is_lock_recursive;

	is_lock_recursive = mm_heap_mutex_lock_recursive(&cpa_heap->mutex);
	ret = clear_cpa_prop(buffer, cpa_heap);
	if (ret) {
		mutex_unlock(&cpa_heap->mutex);
		pr_err("cpa release MPU protect fail! Need check DRM runtime\n");
		return;
	}

	__cpa_heap_free(buffer);
#ifdef CONFIG_NEED_CHANGE_MAPPING
	flush_tlb_all();
#endif
	sg_free_table(table);
	kfree(table);

	mm_heap_mutex_unlock_recursive(&cpa_heap->mutex,
			is_lock_recursive);

	cpa_debug(CPA_DBG_LVL, "%s, succ -- free size: 0x%lx, cpa heap size: 0x%lx, already alloc size: 0x%lx\n",
		  __func__, size, cpa_heap->heap_size,
		  atomic64_read(&cpa_heap->alloc_size));
}

static int cpa_heap_phys(struct dma_heap *heap, struct dma_buf *dmabuf,
				phys_addr_t *addr, size_t *len)
{
	struct mm_heap_helper_buffer *buffer = NULL;

	if (IS_ERR_OR_NULL(heap) || IS_ERR_OR_NULL(dmabuf) ||
		IS_ERR_OR_NULL(addr) || IS_ERR_OR_NULL(len)) {
		pr_err("%s: invalid input para!\n", __func__);
		return -EINVAL;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s: get heap_helper buffer fail!\n", __func__);
		return -EINVAL;
	}

	*addr = buffer->id;
	*len = buffer->size;

	return 0;
}

static struct dma_heap_ops cpa_heap_ops = {
	.allocate = cpa_heap_allocate,
	.free = cpa_heap_free,
	/*
	 * if need to use page pool;
	 * now using the alloc func as below:
	 *
	 * .shrink = cpa_heap_shrink,
	 */
	 .get_heap_phys = cpa_heap_phys,
};

static int  cpa_parse_dt(struct cpa_heap *cpa_heap, struct device_node *np)
{
	unsigned int heap_base = 0;
	unsigned int heap_size = 0;
	int ret = 0;

	ret = of_property_read_string(np, "heap-name", &cpa_heap->name);
	if (ret < 0) {
		pr_err("%s, can not find heap-name\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	ret = of_property_read_u32(np, "heap-base", &heap_base);
	if (ret < 0) {
		pr_err("%s, can not find [%s] heap-base\n",
				__func__, cpa_heap->name);
		ret = -EINVAL;
		goto out;
	}
	cpa_heap->heap_base = heap_base;

	ret = of_property_read_u32(np, "heap-size", &heap_size);
	if (ret < 0) {
		pr_err("%s, can not find [%s] heap-size\n",
				__func__, cpa_heap->name);
		ret = -EINVAL;
		goto out;
	}
	cpa_heap->heap_size = heap_size;

	atomic64_set(&cpa_heap->alloc_size, 0);

out:
	return ret;
}

static void cpa_heap_destroy_pools(struct dmabuf_page_pool **pools)
{
	int i;

	for (i = 0; i < CPA_NUM_ORDERS; i++)
		if (pools[i])
			dmabuf_page_pool_destroy(pools[i]);
}

static int cpa_heap_create_pools(struct dmabuf_page_pool **pools)
{
	int i;

	for (i = 0; i < CPA_NUM_ORDERS; i++) {
		struct dmabuf_page_pool *pool = NULL;
		gfp_t gfp_flags = low_order_gfp_flags;

		if (orders[i] >= 8)
			gfp_flags = high_order_gfp_flags & ~__GFP_RECLAIMABLE;

		pool = dmabuf_page_pool_create(gfp_flags, orders[i]);
		if (!pool)
			goto err_create_pool;
		pools[i] = pool;
	}

	return 0;

err_create_pool:
	cpa_heap_destroy_pools(pools);

	return -ENOMEM;
}

static int cpa_heap_init_properties(struct cpa_heap *cpa_heap, struct device_node *np)
{
	int ret;

	ret = cpa_parse_dt(cpa_heap, np);
	if (ret < 0)
		return -EINVAL;

	cpa_heap->heap.name = cpa_heap->name;
	cpa_heap->heap.ops = &cpa_heap_ops;
	cpa_heap->heap.priv = NULL;
	kref_init(&cpa_heap->heap.refcount);

	return ret;
}

static void cpa_init_pool_order(void)
{
	/*
	 * TOTAL_RAM_PAGES_1G means the RAM SIZE
	 * is aligned according to 1G size
	 *
	 * 8 * TOTAL_RAM_PAGES_1G means 8G DDR size
	 * 4 * TOTAL_RAM_PAGES_1G means 4G DDR size
	 *
	 * And order[1] indicate that we may alloc 2^N(6\5\4)
	 * continuous pages luckily
	 */
	if (totalram_pages() > 8 * TOTAL_RAM_PAGES_1G)
#ifdef CONFIG_DMABUF_HEAPS_CPA_SIZE_OPTIMIZE
		orders[1] = 5;
#else
		orders[1] = 6;
#endif
	else if (totalram_pages() > 4 * TOTAL_RAM_PAGES_1G)
		orders[1] = 5;
	else
		orders[1] = 4;
}

static void cpa_init_zram_percentage(void)
{
	/*
	 * zram_percentage, a water_mark for cpa_zram_ratio,
	 * was used to judge whether hyperhold is enable or not.
	 * when hyperhold is enable, zram increased 4G for all platform.
	 * when hyperhold enable, zram was reconfiged as follow:
	 * DDR 6G: 6G zram
	 * DDR 8G: 8G zram
	 * DDR 12G: 8G zram
	 * accroding to orders[1], considering some reasons, we use this
	 * way to adapt to cpa_kill_process_water_mark.
	 */
	switch (orders[1]) {
	case 4:
	case 5:
		zram_percentage = 75;
		break;
	case 6:
		zram_percentage = 50;
		break;
	default:
		break;
	}
}

static int cpa_heap_create(struct device_node *np)
{
	struct cpa_heap *cpa_heap = NULL;
	struct cpa_work_data *cpa_work_data = NULL;
	int ret;

	cpa_heap = kzalloc(sizeof(*cpa_heap), GFP_KERNEL);
	if (!cpa_heap)
		return -ENOMEM;

	cpa_work_data = kzalloc(sizeof(*cpa_work_data), GFP_KERNEL);
	if (!cpa_work_data)
		goto free_cpa_heap;

	INIT_WORK(&(cpa_work_data->work), cpa_prepare_memory_for_work);

	mutex_init(&cpa_heap->mutex);

	ret = cpa_heap_init_properties(cpa_heap, np);
	if (ret)
		goto free_work_data;

	cpa_init_pool_order();
	cpa_init_zram_percentage();

	if (cpa_heap_create_pools(cpa_heap->pools))
		goto free_work_data;

	pr_err("cpa_heap [%s] info:\n"
		  "\t\t\t\t heap base: 0x%lx\n"
		  "\t\t\t\t heap size: %lu MB\n",
		  cpa_heap->name,
		  cpa_heap->heap_base,
		  cpa_heap->heap_size / SZ_1M);

	cpa_work_data->cpa_heap = cpa_heap;
	cpa_mem_work_data = cpa_work_data;

	ret = dma_heap_add(&cpa_heap->heap);
	if (ret) {
		pr_err("%s: dma_heap_add[%s] failed!\n", __func__, cpa_heap->name);
		goto destroy_pools;
	}
	dma_coerce_mask_and_coherent(dma_heap_get_dev(&cpa_heap->heap),
						DMA_BIT_MASK(64));

	return 0;

destroy_pools:
	cpa_heap_destroy_pools(cpa_heap->pools);
free_work_data:
	mutex_destroy(&cpa_heap->mutex);
	kfree(cpa_work_data);
free_cpa_heap:
	kfree(cpa_heap);

	return -EINVAL;
}

static int cpa_heap_init(void)
{
	int ret = 0;
	struct device_node *sg_np = NULL, *child_np = NULL;

	if (!dma_heap_node) {
		pr_err("%s, can not find dam_heap node\n", __func__);
		return -EINVAL;
	}

	sg_np = of_get_child_by_name(dma_heap_node, "cpa_heap");
	if (!sg_np) {
		pr_err("%s, can not find cpa_heap node\n", __func__);
		return -EINVAL;
	}

	for_each_available_child_of_node(sg_np, child_np) {
		ret = cpa_heap_create(child_np);
		if (ret)
			pr_err("%s, create[%s] fail\n", __func__, child_np->name);
	}

	return 0;
}

module_init(cpa_heap_init);
module_param_named(dma_cpa_debug_level, dma_cpa_debug_level, int, 0640);
