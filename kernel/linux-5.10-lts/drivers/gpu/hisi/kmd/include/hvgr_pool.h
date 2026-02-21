/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_POOL_H
#define HVGR_POOL_H

#include <linux/version.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>

struct hvgr_pool;

typedef long (*pool_alloc)(void *target, uint32_t size, void **array);
typedef void (*pool_free)(void *target, uint32_t size, void **array, uint32_t flag);
typedef void (*pool_callback)(struct hvgr_pool *pool, void *element, uint32_t flag);
typedef long (*pool_grow)(void *target, uint32_t size, void **array);

struct hvgr_pool {
	bool inited;
	uint32_t cur_size;
	uint32_t max_size;
	uint32_t water;
	uint32_t threshold;
	struct mutex pool_lock;
	struct list_head list_head;

	pool_alloc alloc;
	pool_free free;
	pool_callback callback;
	pool_grow grow;
	void *priv;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	struct workqueue_struct *grow_wq;
	struct work_struct grow_work;
#else
	struct kthread_worker *grow_wq;
	struct kthread_work grow_work;
#endif
};

struct hvgr_pool_data {
	uint32_t max_size;
	uint32_t water;

	pool_alloc alloc;
	pool_free free;
	pool_callback callback;
	pool_grow grow;
	void *priv;
	bool prefill;
};

long hvgr_pool_init(struct hvgr_pool *pool, struct hvgr_pool_data *data);

void hvgr_pool_term(struct hvgr_pool *pool);

long hvgr_pool_alloc(struct hvgr_pool *pool, uint32_t size, void **array);

void hvgr_pool_free(struct hvgr_pool *pool, uint32_t size, void **array, uint32_t flag);

long hvgr_pool_alloc_direct(struct hvgr_pool *pool, uint32_t size, void **array);

uint32_t hvgr_pool_grow(struct hvgr_pool *pool, uint32_t size, void **array);

#endif
