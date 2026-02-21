/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Actv threadpool function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 20 10:34:42 2020
 */
#include <libhmactv/actv.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmlog/hmlog.h>

#include "../libhmsync/raw_thread_impl.h"
#include "actv_threadpool.h"

static struct actv_threadpool {
	struct rbt_tree root;
	struct raw_rwlock lock;
} actv_threadpool;

void actv_threadpool_init(void)
{
	struct actv_threadpool *pool = &actv_threadpool;
	mem_zero_s(*pool);

	raw_rwlock_init(&pool->lock, NULL);
}

static int actv_threadpool_insert_nolock(struct actv_threadpool *pool,
					 struct raw_thread *thread)
{
	struct raw_thread *cur = NULL;
	bool is_rchild = false;
	struct rbt_node *p = NULL, *parent = NULL;
	struct rbt_tree *tree = &pool->root;
	int rc = E_HM_OK;

	p = tree->root;
	while (p != NULL) {
		parent = p;
		cur = rbt_entry(p, struct raw_thread, node);
		if (CMPT_THREAD_REG(thread, tid) > CMPT_THREAD_REG(cur, tid)) {
			p = p->rbt_right;
			is_rchild = true;
		} else if (CMPT_THREAD_REG(thread, tid) < CMPT_THREAD_REG(cur, tid)) {
			p = p->rbt_left;
			is_rchild = false;
		} else {
			hm_error("CMPT_THREAD_REG(thread, tid) = %d,  CMPT_THREAD_REG(cur, tid)) = %d\n",
				 CMPT_THREAD_REG(thread, tid), CMPT_THREAD_REG(cur, tid));
			rc = -1;
			break;
		}
	}
	if (rc == E_HM_OK) {
		rbt_insert(tree, &thread->node, parent, is_rchild);
	}
	return rc;
}

int actv_threadpool_insert(struct raw_thread *thread)
{
	struct actv_threadpool *pool = &actv_threadpool;
	int rc = E_HM_OK;

	if (thread == NULL) {
		rc = E_HM_INVAL;
	} else {
		RAW_RWLOCK_WR_GUARD(_, &pool->lock);
		rc = actv_threadpool_insert_nolock(pool, thread);
	}
	return rc;
}

void actv_threadpool_remove(struct raw_thread *thread)
{
	struct actv_threadpool *pool = &actv_threadpool;

	if (thread != NULL) {
		RAW_RWLOCK_WR_GUARD(_, &pool->lock);
		rbt_remove(&pool->root, &thread->node);
	}
}

__pure struct raw_thread *actv_threadpool_search_nolock(unsigned int actv_id)
{
	struct actv_threadpool *pool = &actv_threadpool;
	struct raw_thread *cur = NULL;
	struct rbt_tree *tree = &pool->root;
	struct rbt_node *p = tree->root;

	while (p != NULL) {
		cur = rbt_entry(p, struct raw_thread, node);

		if (actv_id == (unsigned int)CMPT_THREAD_REG(cur, tid)) {
			return (struct raw_thread *)cur;
		}
		if (actv_id > (unsigned int)CMPT_THREAD_REG(cur, tid)) {
			p = p->rbt_right;
		} else {
			p = p->rbt_left;
		}
	}
	return NULL;
}

struct raw_thread *actv_threadpool_search(unsigned int actv_id)
{
	struct actv_threadpool *pool = &actv_threadpool;

	RAW_RWLOCK_RD_GUARD(_, &pool->lock);
	return actv_threadpool_search_nolock(actv_id);
}

struct raw_rwlock *actv_threadpool_lock(void)
{
	struct actv_threadpool *pool = &actv_threadpool;
	return &pool->lock;
}
