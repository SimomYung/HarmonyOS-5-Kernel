/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_pool.h"

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <securec.h>
#include "hvgr_log_api.h"
#include "hvgr_dm_api.h"

#define HVGR_ALLOC_THRESHOLD 512u
#define WORKQ_NAME_MAX_LEN 64u

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static void hvgr_pool_grow_work(struct work_struct *data)
#else
static void hvgr_pool_grow_work(struct kthread_work *data)
#endif
{
	struct hvgr_device *gdev = hvgr_get_device();
	struct hvgr_pool *pool = NULL;
	struct page **pages = NULL;
	long ret;
	uint32_t alloc_pages;
	uint32_t i;
	uint32_t pool_size;
	uint32_t grow_pages;

	pool = container_of(data, struct hvgr_pool, grow_work);
	mutex_lock(&pool->pool_lock);
	pool_size = pool->cur_size;
	mutex_unlock(&pool->pool_lock);

	if (pool_size >= pool->threshold || pool->grow == NULL)
		return;

	alloc_pages = pool->water - pool_size;
	if (alloc_pages <= HVGR_ALLOC_THRESHOLD)
		pages = kcalloc(alloc_pages, sizeof(struct page *), GFP_KERNEL);
	else
		pages = vzalloc((alloc_pages * sizeof(struct page *)));
	if (pages == NULL) {
		hvgr_debug(gdev, HVGR_MEM, "%s:%d poor grow work malloc fail", __func__, __LINE__);
		return;
	}

	ret = pool->grow(pool->priv, alloc_pages, (void **)pages);
	if (ret != 0) {
		hvgr_debug(gdev, HVGR_MEM, "%s:%d poor grow work alloc page fail",
			__func__, __LINE__);
		kvfree(pages);
		return;
	}

	mutex_lock(&pool->pool_lock);
	if (pool->cur_size >= pool->water) {
		mutex_unlock(&pool->pool_lock);
		goto free_pages;
	}

	grow_pages = min((pool->max_size - pool->cur_size), alloc_pages);
	for (i = 0; i < grow_pages; i++) {
		list_add(&(pages[i]->lru), &pool->list_head);
		pool->cur_size++;
	}

	mutex_unlock(&pool->pool_lock);

	if (grow_pages < alloc_pages)
		pool->free(pool->priv, (alloc_pages - grow_pages), (void **)(pages + grow_pages),
			0);
	goto free_array;

free_pages:
	pool->free(pool->priv, alloc_pages, (void **)pages, 0);
free_array:
	kvfree(pages);
}

long hvgr_pool_init(struct hvgr_pool *pool, struct hvgr_pool_data *data)
{
	static uint32_t workq_id = 0;
	char workq_name[WORKQ_NAME_MAX_LEN];
	struct hvgr_device *gdev = hvgr_get_device();

	if (pool == NULL || data == NULL || data->alloc == NULL || data->free == NULL)
		return -EINVAL;

	pool->max_size = data->max_size;
	pool->water = (pool->max_size / 2U);
	pool->threshold = (pool->water * 3U / 4U);
	pool->priv = data->priv;
	pool->alloc = data->alloc;
	pool->free = data->free;
	pool->callback = data->callback;
	pool->grow = data->grow;
	pool->cur_size = 0;
	INIT_LIST_HEAD(&pool->list_head);
	mutex_init(&pool->pool_lock);

	if (pool->grow == NULL)
		goto init_done;

	(void)snprintf_s(workq_name, WORKQ_NAME_MAX_LEN, WORKQ_NAME_MAX_LEN - 1, "hvgr_grow_wq_%u",
		workq_id);
	workq_id++;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	pool->grow_wq = alloc_workqueue(workq_name,
		WQ_MEM_RECLAIM | WQ_HIGHPRI | WQ_UNBOUND, 1);
	if (pool->grow_wq == NULL) {
		hvgr_debug(gdev, HVGR_MEM, "%s:%d alloc workqueue %s fail",
			__func__, __LINE__, workq_name);
	} else {
		INIT_WORK(&pool->grow_work, hvgr_pool_grow_work);
		hvgr_debug(gdev, HVGR_MEM, "%s:%d create work %s (%u:%u:%u:%u)", __func__, __LINE__,
			workq_name, pool->cur_size, pool->threshold, pool->water, pool->max_size);
		if (data->prefill) {
			hvgr_debug(gdev, HVGR_MEM, "%s:%d queue work %s to prefill",
				__func__, __LINE__, workq_name);
			(void)queue_work(pool->grow_wq, &pool->grow_work);
		}
	}
#else
	pool->grow_wq = kthread_create_worker(0, workq_name);
	if (IS_ERR(pool->grow_wq)) {
		pool->grow_wq = NULL;
		hvgr_debug(gdev, HVGR_MEM, "%s:%d alloc workqueue %s fail",
			__func__, __LINE__, workq_name);
	} else {
		sched_set_fifo(pool->grow_wq->task);
		kthread_init_work(&pool->grow_work, hvgr_pool_grow_work);
		hvgr_debug(gdev, HVGR_MEM, "%s:%d create work %s (%u:%u:%u:%u)", __func__, __LINE__,
			workq_name, pool->cur_size, pool->threshold, pool->water, pool->max_size);
		if (data->prefill) {
			hvgr_debug(gdev, HVGR_MEM, "%s:%d queue work %s to prefill",
				__func__, __LINE__, workq_name);
			(void)kthread_queue_work(pool->grow_wq, &pool->grow_work);
		}
	}
#endif
init_done:
	pool->inited = true;
	return 0;
}

void hvgr_pool_term(struct hvgr_pool *pool)
{
	if (pool == NULL || !pool->inited)
		return;

	pool->cur_size = 0;
	pool->max_size = 0;

	if (pool->grow_wq != NULL)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		destroy_workqueue(pool->grow_wq);
#else
		kthread_destroy_worker(pool->grow_wq);
#endif
}

long hvgr_pool_alloc(struct hvgr_pool *pool, uint32_t size, void **array)
{
	uint32_t i;
	uint32_t pool_get_size;
	struct page *page = NULL;
	long ret = 0;

	if (pool == NULL || array == NULL || size == 0)
		return -EINVAL;

	hvgr_systrace_begin("%s", __func__);
	mutex_lock(&pool->pool_lock);
	hvgr_systrace_begin("%s get lock %u %u", __func__, pool->cur_size, size);
	if (pool->cur_size == 0) {
		mutex_unlock(&pool->pool_lock);
		hvgr_systrace_end();
		ret = pool->alloc(pool->priv, size, array);
		hvgr_systrace_end();
		goto exit;
	}

	pool_get_size = min(pool->cur_size, size);

	for (i = 0; i < pool_get_size; i++) {
		page = list_first_entry(&pool->list_head, struct page, lru);
		list_del_init(&page->lru);

		array[i] = (void *)page;
		pool->cur_size--;
	}

	mutex_unlock(&pool->pool_lock);
	hvgr_systrace_end();
	if (pool_get_size < size) {
		ret = pool->alloc(pool->priv, (size - pool_get_size), (array + pool_get_size));
		hvgr_systrace_end();
		goto exit;
	}
	hvgr_systrace_end();

exit:
	if (pool->grow_wq != NULL && pool->cur_size < pool->threshold)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		(void)queue_work(pool->grow_wq, &pool->grow_work);
#else
		(void)kthread_queue_work(pool->grow_wq, &pool->grow_work);
#endif

	return ret;
}

void hvgr_pool_free(struct hvgr_pool *pool, uint32_t size, void **array, uint32_t flag)
{
	uint32_t i;
	uint32_t pool_put_size;
	struct page *page = NULL;

	if (pool == NULL || array == NULL || size == 0)
		return;

	hvgr_systrace_begin("%s:%d", __func__, __LINE__);
	mutex_lock(&pool->pool_lock);
	hvgr_systrace_begin("%s get lock %u %u", __func__, pool->cur_size, size);
	if (pool->cur_size == pool->max_size) {
		mutex_unlock(&pool->pool_lock);
		hvgr_systrace_end();
		pool->free(pool->priv, size, array, flag);
		hvgr_systrace_end();
		return;
	}

	pool_put_size = pool->max_size - pool->cur_size;
	pool_put_size = min(pool_put_size, size);

	hvgr_systrace_begin("%s:%d", __func__, __LINE__);
	for (i = 0; i < pool_put_size; i++) {
		page = (struct page *)array[i];
		if (unlikely(page == NULL)) {
			pr_err("[HVGR_ERROR] free page is NULL!\n");
			continue;
		}
		list_add(&page->lru, &pool->list_head);
		pool->cur_size++;
	}
	hvgr_systrace_end();
	mutex_unlock(&pool->pool_lock);
	hvgr_systrace_end();
	if (pool_put_size < size)
		pool->free(pool->priv, (size - pool_put_size), (array + pool_put_size), flag);
	hvgr_systrace_end();
}

long hvgr_pool_alloc_direct(struct hvgr_pool *pool, uint32_t size, void **array)
{
	uint32_t i;
	struct page *page = NULL;

	if (pool == NULL || array == NULL || size == 0)
		return -EINVAL;
	hvgr_systrace_begin("%s", __func__);
	mutex_lock(&pool->pool_lock);
	hvgr_systrace_begin("%s get lock %u %u", __func__, pool->cur_size, size);
	if (pool->cur_size < size) {
		mutex_unlock(&pool->pool_lock);
		hvgr_systrace_end();
		hvgr_systrace_end();
		return -ENOMEM;
	}

	for (i = 0; i < size; i++) {
		page = list_first_entry(&pool->list_head, struct page, lru);
		list_del_init(&page->lru);

		array[i] = (void *)page;
		pool->cur_size--;
	}

	mutex_unlock(&pool->pool_lock);
	hvgr_systrace_end();
	hvgr_systrace_end();
	return 0;
}

uint32_t hvgr_pool_grow(struct hvgr_pool *pool, uint32_t size, void **array)
{
	uint32_t pool_grow_size;
	uint32_t i;
	struct page *page = NULL;

	if (pool == NULL || array == NULL || size == 0)
		return 0;

	hvgr_systrace_begin("%s", __func__);
	mutex_lock(&pool->pool_lock);
	hvgr_systrace_begin("%s get lock %u %u", __func__, pool->cur_size, size);
	if (pool->cur_size == pool->max_size) {
		mutex_unlock(&pool->pool_lock);
		hvgr_systrace_end();
		hvgr_systrace_end();
		return 0;
	}

	pool_grow_size = pool->max_size - pool->cur_size;
	pool_grow_size = min(pool_grow_size, size);

	for (i = 0; i < pool_grow_size; i++) {
		page = (struct page *)array[i];
		list_add(&page->lru, &pool->list_head);
		pool->cur_size++;
	}
	mutex_unlock(&pool->pool_lock);
	hvgr_systrace_end();
	hvgr_systrace_end();

	return pool_grow_size;
}

