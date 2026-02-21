/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  MAS MQ ioscheduler interface
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <libstrict/strict.h>
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/block/mas_blk/mas_blk_iosched_interface.h>
#include <udk/block/mas_blk/mas_blk_mq_tag.h>

int __cfi_ufs_mq_sync_io_dispatch_work_fn(struct udk_work *work)
{
	ufs_mq_sync_io_dispatch_work_fn(work);
	return 0;
}

void __cfi_ufs_mq_sync_burst_check_timer_expire(unsigned long val)
{
	struct mas_ufs_sched_ds_lld *sched_ds_lld = (struct mas_ufs_sched_ds_lld *)val;
	ufs_mq_sync_burst_check_timer_expire(sched_ds_lld);
}

int __cfi_ufs_mq_async_io_dispatch_work_fn(struct udk_work *work)
{
	ufs_mq_async_io_dispatch_work_fn(work);
	return 0;
}

int __cfi_ufs_mq_io_guard_work_fn(struct udk_work *work)
{
	ufs_mq_io_guard_work_fn(work);
	return 0;
}

int __cfi_ufs_mq_make_request(struct udk_bio *bio)
{
	return ufs_mq_make_request(bio);
}

static inline struct udk_blk_mq_tags *__cfi_ufs_tagset_init_tags(struct udk_blk_mq_tag_set *set)
{
	return ufs_tagset_init_tags(set);
}

static inline void __cfi_ufs_tagset_free_tags(struct udk_blk_mq_tags *tags)
{
	ufs_tagset_free_tags(tags);
}

static inline int __cfi_ufs_mq_iosched_init(struct udk_request_queue *q)
{
	return ufs_mq_iosched_init(q);
}

static inline void __cfi_ufs_mq_iosched_exit(struct udk_request_queue *q)
{
	ufs_mq_iosched_exit(q);
}

static inline void __cfi_ufs_mq_req_init(
	struct udk_request_queue *q, struct udk_blk_mq_ctx *ctx, struct udk_request *rq)
{
	UNUSED(q);
	ufs_mq_req_init(ctx, rq);
}

static inline void __cfi_ufs_mq_req_complete(
	struct udk_request *rq, struct udk_request_queue *q)
{
	ufs_mq_req_complete(rq, q);
}

static inline void __cfi_ufs_mq_req_deinit(struct udk_request *rq)
{
	ufs_mq_req_deinit(rq);
}

static inline void __cfi_ufs_mq_req_insert(
	struct udk_request *req, struct udk_request_queue *q)
{
	ufs_mq_req_insert(req, q);
}

static inline void __cfi_ufs_mq_req_requeue(
	struct udk_request *req, struct udk_request_queue *q)
{
	ufs_mq_req_requeue(req, q);
}

static inline void __cfi_ufs_mq_req_timeout_handler(struct udk_request *req)
{
	UNUSED(req);
}

static inline int __cfi_ufs_mq_tag_get(struct udk_blk_mq_tags *tags,
	struct udk_bio *bio, struct udk_request_queue *queue)
{
	return ufs_mq_tag_get(tags, bio, queue);
}

static inline void __cfi_ufs_mq_tag_put(struct udk_blk_mq_tags *tags,
	unsigned int tag, struct udk_request_queue *queue)
{
	ufs_mq_tag_put(tags, tag, queue);
}

static inline int __cfi_ufs_mq_tag_wakeup_all(struct udk_blk_mq_tags *tags)
{
	UNUSED(tags);
	return 0;
}

static inline void __cfi_ufs_mq_exec_queue(struct udk_request_queue *q)
{
	ufs_mq_exec_queue(q);
}

static inline void __cfi_ufs_mq_run_hw_queue(struct udk_request_queue *q)
{
	ufs_mq_run_hw_queue(q);
}

static inline void __cfi_ufs_mq_run_requeue(struct udk_request_queue *q)
{
	ufs_mq_run_requeue(q);
}

void __cfi_ufs_mq_write_throttle_check_timer_expire(unsigned long val)
{
	struct mas_ufs_sched_ds_lld *ds_lld = (struct mas_ufs_sched_ds_lld *)val;
	ufs_mq_write_throttle_check_timer_expire(ds_lld);
}

static inline void __cfi_ufs_mq_write_throttle_handler(
	struct udk_request_queue *q, bool level)
{
	ufs_mq_write_throttle_handler(q, level);
}

static inline void __cfi_ufs_mq_status_dump(
	struct udk_request_queue *q, enum blk_dump_scene s)
{
	ufs_mq_status_dump(q, s);
}

static inline void __cfi_ufs_tagset_power_off_proc(struct blk_dev_lld *lld)
{
	ufs_tagset_power_off_proc(lld);
}

struct blk_queue_ops mas_ufs_blk_queue_ops = {
	.io_scheduler_strategy = IOSCHED_MAS_UFS_MQ,
	.mq_iosched_init_fn = __cfi_ufs_mq_iosched_init,
	.mq_iosched_exit_fn = __cfi_ufs_mq_iosched_exit,
	.mq_req_init_fn = __cfi_ufs_mq_req_init,
	.mq_req_complete_fn = __cfi_ufs_mq_req_complete,
	.mq_req_deinit_fn = __cfi_ufs_mq_req_deinit,
	.mq_req_insert_fn = __cfi_ufs_mq_req_insert,
	.mq_req_requeue_fn = __cfi_ufs_mq_req_requeue,
	.mq_req_timeout_fn = __cfi_ufs_mq_req_timeout_handler,
	.mq_tag_get_fn = __cfi_ufs_mq_tag_get,
	.mq_tag_put_fn = __cfi_ufs_mq_tag_put,
	.mq_tag_wakeup_all_fn = __cfi_ufs_mq_tag_wakeup_all,
	.mq_exec_queue_fn = __cfi_ufs_mq_exec_queue,
	.mq_run_hw_queue_fn = __cfi_ufs_mq_run_hw_queue,
	.mq_run_requeue_fn = __cfi_ufs_mq_run_requeue,
	.blk_write_throttle_fn = __cfi_ufs_mq_write_throttle_handler,
	.blk_status_dump_fn = __cfi_ufs_mq_status_dump,
	.scheduler_priv = &mas_ufs_mq,
	.iosched_name = "MAS MQ",
};

struct blk_tagset_ops mas_ufs_blk_tagset_ops = {
	.tagset_init_tags_fn = __cfi_ufs_tagset_init_tags,
	.tagset_free_tags_fn = __cfi_ufs_tagset_free_tags,
	.tagset_power_off_proc_fn = __cfi_ufs_tagset_power_off_proc,
	.queue_ops = &mas_ufs_blk_queue_ops,
};
