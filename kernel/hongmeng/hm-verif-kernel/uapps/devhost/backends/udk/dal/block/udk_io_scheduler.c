/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK io scheduler framework
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 12 11:43:30 2023
 */

#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_io_scheduler.h>
#include <udk/log.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>

struct uspinlock_s udk_sched_list_lock = USPINLOCK_INITIALIZER;
static DLIST_HEAD(udk_sched_list);

#define req_hash_key(req) (udk_req_pos(req) + udk_req_sectors(req))

bool udk_sched_bio_merge_ok(struct udk_request *req, struct udk_bio *bio)
{
	if (!udk_req_can_merge(req, bio)) {
		return false;
	}

	return true;
}

static bool
udk_io_scheduler_match(const struct udk_io_scheduler_type *sched_type,
		       const char *name)
{
	return strcmp(sched_type->name, name) == 0;
}

static struct udk_io_scheduler_type *udk_find_io_scheduler(const char *name)
{
	struct udk_io_scheduler_type *sched_type = NULL;

	dlist_for_each_entry(sched_type, &udk_sched_list,
			      struct udk_io_scheduler_type, list)
		if (udk_io_scheduler_match(sched_type, name)) {
			return sched_type;
		}

	return NULL;
}

static struct udk_io_scheduler_type *udk_io_scheduler_find(const char *name)
{
	struct udk_io_scheduler_type *sched_type = NULL;

	uspinlock_lock(&udk_sched_list_lock);
	sched_type = udk_find_io_scheduler(name);
	uspinlock_unlock(&udk_sched_list_lock);
	return sched_type;
}

#define MAX_BUCKET_SIZE 64
#define MAX_HASH_COUNT 4096
struct udk_io_scheduler_queue *
udk_alloc_io_scheduler(struct udk_io_scheduler_type *sched_type)
{
	struct udk_io_scheduler_queue *sched = NULL;

	sched = calloc(1, sizeof(*sched));
	if (sched == NULL) {
		return NULL;
	}

	sched->type = sched_type;
	(void)easy_htable_init(&sched->req_hash, malloc, MAX_BUCKET_SIZE,
			       MAX_HASH_COUNT,
			       offsetof(struct udk_request, hash_key),
			       offsetof(struct udk_request, hash));

	return sched;
}

void udk_sched_req_hash_del(struct udk_request *req)
{
	if (req->req_flags & UDK_RQF_HASHED) {
		easy_htable_remove_u64(&(req->queue->scheduler->req_hash),
				       req->hash_key, &(req->hash));
		req->req_flags &= ~UDK_RQF_HASHED;
	}
}

void udk_sched_req_hash_add(struct udk_request_queue *queue,
			    struct udk_request *req)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;

	req->hash_key = req_hash_key(req);
	easy_htable_insert_u64(&(sched->req_hash), req->hash_key, &(req->hash));
	req->req_flags |= UDK_RQF_HASHED;
}

void udk_sched_req_hash_reposition(struct udk_request_queue *queue,
				   struct udk_request *req)
{
	udk_sched_req_hash_del(req);
	udk_sched_req_hash_add(queue, req);
}

struct udk_request *udk_sched_req_hash_find(struct udk_request_queue *queue,
					    uint64_t offset)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;
	struct udk_request *req = NULL;

	req = easy_htable_lookup_u64(&sched->req_hash, offset, NULL);

	return req;
}

void udk_sched_rbtree_add(struct rbt_tree *root, struct udk_request *req)
{
	struct rbt_node **p = &root->root;
	struct rbt_node *parent = NULL;
	struct udk_request *__req = NULL;
	bool right = false;

	while (*p != NULL) {
		parent = *p;
		__req = rbt_entry(parent, struct udk_request, rbt_node);

		if (udk_req_pos(req) < udk_req_pos(__req)) {
			p = &(*p)->rbt_left;
			right = false;
		} else if (udk_req_pos(req) >= udk_req_pos(__req)) {
			p = &(*p)->rbt_right;
			right = true;
		}
	}

	rbt_insert(root, &req->rbt_node, parent, right);
}

void udk_sched_rbtree_del(struct rbt_tree *root, struct udk_request *req)
{
	rbt_remove(root, &req->rbt_node);
}

struct udk_request *udk_sched_rbtree_find(struct rbt_tree *root,
					  uint64_t sector)
{
	struct rbt_node *n = root->root;
	struct udk_request *req = NULL;

	while (n) {
		req = rbt_entry(n, struct udk_request, rbt_node);

		if (sector < udk_req_pos(req)) {
			n = n->rbt_left;
		} else if (sector > udk_req_pos(req)) {
			n = n->rbt_right;
		} else {
			return req;
		}
	}

	return NULL;
}

enum udk_sched_merge udk_sched_merge(struct udk_request_queue *queue,
				     struct udk_request **req,
				     struct udk_bio *bio)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;
	struct udk_request *__req = NULL;

	if (!udk_bio_mergeable(bio)) {
		return UDK_IO_SCHEDULER_NO_MERGE;
	}

	if (queue->last_merge && udk_sched_bio_merge_ok(queue->last_merge, bio)) {
		enum udk_sched_merge ret =
			udk_blk_try_merge(queue->last_merge, bio);
		if (ret != UDK_IO_SCHEDULER_NO_MERGE) {
			*req = queue->last_merge;
			return ret;
		}
	}

	__req = udk_sched_req_hash_find(queue, bio->bi_sector);
	if (__req && udk_sched_bio_merge_ok(__req, bio)) {
		*req = __req;

		if (udk_blk_discard_mergable(__req)) {
			return UDK_IO_SCHEDULER_DISCARD_MERGE;
		}
		return UDK_IO_SCHEDULER_BACK_MERGE;
	}

	if (sched->type->ops.request_merge) {
		return sched->type->ops.request_merge(queue, req, bio);
	}

	return UDK_IO_SCHEDULER_NO_MERGE;
}

bool udk_sched_attempt_insert_merge(struct udk_request_queue *queue,
				    struct udk_request *req,
				    struct dlist_node *free)
{
	struct udk_request *__req = NULL;
	bool ret;

	if (queue->last_merge &&
	    udk_blk_attempt_req_merge(queue, queue->last_merge, req)) {
		dlist_insert(free, &req->queuelist);
		return true;
	}

	ret = false;
	while (1) {
		__req = udk_sched_req_hash_find(queue, udk_req_pos(req));
		if (!__req || !udk_blk_attempt_req_merge(queue, __req, req)) {
			break;
		}

		dlist_insert(free, &req->queuelist);
		/* The merged request could be merged with others, try again */
		ret = true;
		req = __req;
	}

	return ret;
}

void udk_sched_merged_request(struct udk_request_queue *queue,
			      struct udk_request *req,
			      enum udk_sched_merge type)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;

	if (sched->type->ops.request_merged) {
		sched->type->ops.request_merged(queue, req, type);
	}

	if (type == UDK_IO_SCHEDULER_BACK_MERGE) {
		udk_sched_req_hash_reposition(queue, req);
	}

	queue->last_merge = req;
}

void udk_sched_merge_requests(struct udk_request_queue *queue,
			      struct udk_request *req, struct udk_request *next)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;

	if (sched->type->ops.requests_merged) {
		sched->type->ops.requests_merged(queue, req, next);
	}

	udk_sched_req_hash_reposition(queue, req);
	queue->last_merge = req;
}

struct udk_request *udk_sched_next_request(struct udk_request_queue *queue,
					   struct udk_request *req)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;

	if (sched->type->ops.next_request) {
		return sched->type->ops.next_request(queue, req);
	}

	return NULL;
}

struct udk_request *udk_sched_prev_request(struct udk_request_queue *queue,
					   struct udk_request *req)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;

	if (sched->type->ops.prev_request) {
		return sched->type->ops.prev_request(queue, req);
	}

	return NULL;
}

int udk_sched_register(struct udk_io_scheduler_type *sched_type)
{
	if ((!sched_type->ops.insert_requests ||
	     !sched_type->ops.dispatch_request)) {
		return -EINVAL;
	}

	uspinlock_lock(&udk_sched_list_lock);
	if (udk_find_io_scheduler(sched_type->name)) {
		uspinlock_unlock(&udk_sched_list_lock);
		return -EBUSY;
	}
	dlist_insert_tail(&udk_sched_list, &sched_type->list);
	uspinlock_unlock(&udk_sched_list_lock);

	return 0;
}

void udk_sched_unregister(struct udk_io_scheduler_type *sched_type)
{
	uspinlock_lock(&udk_sched_list_lock);
	dlist_delete(&sched_type->list);
	uspinlock_unlock(&udk_sched_list_lock);
}

static inline bool udk_sched_support_iosched(struct udk_request_queue *queue)
{
	if ((queue->tag_set &&
	     (queue->tag_set->flags & UDK_BLK_MQ_NO_SCHED))) {
		return false;
	}
	return true;
}

static struct udk_io_scheduler_type *udk_io_scheduler_get_default(void)
{
	return udk_io_scheduler_find("mq-deadline");
}

void udk_io_scheduler_init_mq(struct udk_request_queue *queue)
{
	struct udk_io_scheduler_type *sched_type = NULL;
	int err;

	if (!udk_sched_support_iosched(queue)) {
		return;
	}

	if (queue->scheduler != NULL) {
		return;
	}

	sched_type = udk_io_scheduler_get_default();
	if (sched_type == NULL) {
		return;
	}

	err = udk_blk_mq_init_sched(queue, sched_type);
	if (err < 0) {
		udk_warn("udk_io_scheduler:%s initialization failed! \n",
			 sched_type->name);
	}
}

struct udk_request *
udk_sched_rbtree_prev_request(struct udk_request_queue *queue,
			      struct udk_request *req)
{
	struct rbt_node *rbprev = rbt_prev(&req->rbt_node);
	UNUSED(queue);

	if (rbprev != NULL) {
		return rbt_entry_req(rbprev);
	}

	return NULL;
}

struct udk_request *
udk_sched_rbtree_next_request(struct udk_request_queue *queue,
			      struct udk_request *req)
{
	struct rbt_node *rbnext = rbt_next(&req->rbt_node);
	UNUSED(queue);

	if (rbnext != NULL) {
		return rbt_entry_req(rbnext);
	}

	return NULL;
}
