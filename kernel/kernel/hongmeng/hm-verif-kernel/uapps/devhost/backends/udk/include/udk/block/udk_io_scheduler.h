/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK io scheduler definitions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 12 11:43:30 2023
 */
#ifndef _UDK_IO_SCHEDULER_H
#define _UDK_IO_SCHEDULER_H

#include <udk/module.h>
#include <libalgo/rb_htable.h>

struct udk_io_scheduler_type;

enum udk_sched_merge {
	UDK_IO_SCHEDULER_NO_MERGE = 0,
	UDK_IO_SCHEDULER_FRONT_MERGE = 1,
	UDK_IO_SCHEDULER_BACK_MERGE = 2,
	UDK_IO_SCHEDULER_DISCARD_MERGE = 3,
};

struct udk_blk_mq_hw_ctx;
struct udk_request_queue;
struct udk_request;
struct udk_io_scheduler_queue;

struct udk_io_scheduler_mq_ops {
	int (*init_sched)(struct udk_request_queue *,
			  struct udk_io_scheduler_type *);
	void (*exit_sched)(struct udk_io_scheduler_queue *);
	int (*init_hctx)(struct udk_blk_mq_hw_ctx *, unsigned int);
	void (*exit_hctx)(struct udk_blk_mq_hw_ctx *, unsigned int);
	bool (*bio_merge)(struct udk_request_queue *, struct udk_bio *,
			  unsigned int);
	int (*request_merge)(struct udk_request_queue *q, struct udk_request **,
			     struct udk_bio *);
	void (*request_merged)(struct udk_request_queue *, struct udk_request *,
			       enum udk_sched_merge);
	void (*requests_merged)(struct udk_request_queue *,
				struct udk_request *, struct udk_request *);
	void (*prepare_request)(struct udk_request *);
	void (*finish_request)(struct udk_request *);
	void (*insert_requests)(struct udk_blk_mq_hw_ctx *, struct dlist_node *,
				bool);
	struct udk_request *(*dispatch_request)(struct udk_blk_mq_hw_ctx *);
	bool (*has_work)(struct udk_blk_mq_hw_ctx *);
	void (*completed_request)(struct udk_request *, uint64_t);
	void (*requeue_request)(struct udk_request *);
	struct udk_request *(*prev_request)(struct udk_request_queue *,
					    struct udk_request *);
	struct udk_request *(*next_request)(struct udk_request_queue *,
					    struct udk_request *);
};

struct udk_io_scheduler_type {
	struct udk_io_scheduler_mq_ops ops;
	const char *name;
	struct dlist_node list;
};

void udk_sched_req_hash_del(struct udk_request *req);
void udk_sched_req_hash_add(struct udk_request_queue *q,
			    struct udk_request *req);
void udk_sched_req_hash_reposition(struct udk_request_queue *q,
				   struct udk_request *req);
struct udk_request *udk_sched_req_hash_find(struct udk_request_queue *q,
					    uint64_t offset);

struct udk_io_scheduler_queue {
	struct udk_io_scheduler_type *type;
	void *scheduler_data;
	unsigned long flags;
	struct easy_htable req_hash;
};

extern enum udk_sched_merge udk_sched_merge(struct udk_request_queue *,
					    struct udk_request **,
					    struct udk_bio *);
extern void udk_sched_merge_requests(struct udk_request_queue *,
				     struct udk_request *,
				     struct udk_request *);
extern void udk_sched_merged_request(struct udk_request_queue *,
				     struct udk_request *,
				     enum udk_sched_merge);
extern bool udk_sched_attempt_insert_merge(struct udk_request_queue *,
					   struct udk_request *,
					   struct dlist_node *);
extern struct udk_request *udk_sched_prev_request(struct udk_request_queue *,
						  struct udk_request *);
extern struct udk_request *udk_sched_next_request(struct udk_request_queue *,
						  struct udk_request *);
void udk_io_scheduler_init_mq(struct udk_request_queue *);

extern int udk_sched_register(struct udk_io_scheduler_type *);
extern void udk_sched_unregister(struct udk_io_scheduler_type *);

extern bool udk_sched_bio_merge_ok(struct udk_request *, struct udk_bio *);
extern struct udk_io_scheduler_queue *
udk_alloc_io_scheduler(struct udk_io_scheduler_type *);

extern struct udk_request *
udk_sched_rbtree_prev_request(struct udk_request_queue *, struct udk_request *);
extern struct udk_request *
udk_sched_rbtree_next_request(struct udk_request_queue *, struct udk_request *);

extern void udk_sched_rbtree_add(struct rbt_tree *, struct udk_request *);
extern void udk_sched_rbtree_del(struct rbt_tree *, struct udk_request *);
extern struct udk_request *udk_sched_rbtree_find(struct rbt_tree *, uint64_t);

#define RB_EMPTY_NODE(p) ((p)->rbt_parent_color == 0)
#define RB_CLEAR_NODE(p) ((p)->rbt_parent_color = 0)
#define rbt_entry_req(node) rbt_entry((node), struct udk_request, rbt_node)

#endif /* _UDK_IO_SCHEDULER_H */
