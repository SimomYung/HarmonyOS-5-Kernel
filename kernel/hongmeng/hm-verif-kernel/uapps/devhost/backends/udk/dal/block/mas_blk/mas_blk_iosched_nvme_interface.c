/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  MAS MQ ioscheduler interface
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <libstrict/strict.h>
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/block/mas_blk/mas_blk_iosched_nvme_interface.h>
#include <udk/block/mas_blk/mas_blk_nvme_mq_tag.h>

int __cfi_nvme_mq_sync_io_dispatch_work_fn(struct udk_work *work)
{
	nvme_mq_sync_io_dispatch_work_fn(work);
	return 0;
}

void __cfi_nvme_mq_sync_burst_check_timer_expire(unsigned long val)
{
	struct mas_nvme_sched_ds_lld *sched_ds_lld = (struct mas_nvme_sched_ds_lld *)val;
	nvme_mq_sync_burst_check_timer_expire(sched_ds_lld);
}

int __cfi_nvme_mq_async_io_dispatch_work_fn(struct udk_work *work)
{
	nvme_mq_async_io_dispatch_work_fn(work);
	return 0;
}

int __cfi_nvme_mq_io_guard_work_fn(struct udk_work *work)
{
	nvme_mq_io_guard_work_fn(work);
	return 0;
}

static inline struct udk_blk_mq_tags *__cfi_nvme_tagset_init_tags(struct udk_blk_mq_tag_set *set)
{
	return nvme_tagset_init_tags(set);
}

static inline int __cfi_nvme_mq_iosched_init(struct udk_request_queue *q)
{
	return nvme_mq_iosched_init(q);
}

static inline void __cfi_nvme_mq_iosched_exit(struct udk_request_queue *q)
{
	nvme_mq_iosched_exit(q);
}

static inline void __cfi_nvme_mq_req_complete(
	struct udk_request *rq, struct udk_request_queue *q)
{
	nvme_mq_req_complete(rq, q);
}

static inline int __cfi_nvme_mq_tag_get(struct udk_blk_mq_tags *tags,
	struct udk_bio *bio, struct udk_request_queue *queue)
{
	return nvme_mq_tag_get(tags, bio, queue);
}

static inline void __cfi_nvme_mq_tag_put(struct udk_blk_mq_tags *tags,
	unsigned int tag, struct udk_request_queue *queue)
{
	nvme_mq_tag_put(tags, tag, queue);
}

void __cfi_nvme_mq_write_throttle_check_timer_expire(unsigned long val)
{
	struct mas_nvme_sched_ds_lld *ds_lld = (struct mas_nvme_sched_ds_lld *)val;
	nvme_mq_write_throttle_check_timer_expire(ds_lld);
}

static inline void __cfi_nvme_mq_status_dump(
	struct udk_request_queue *q, enum blk_dump_scene s)
{
	nvme_mq_status_dump(q, s);
}

struct blk_queue_ops mas_nvme_blk_queue_ops = {
	.io_scheduler_strategy = IOSCHED_MAS_NVME_MQ,
	.mq_iosched_init_fn = __cfi_nvme_mq_iosched_init,
	.mq_iosched_exit_fn = __cfi_nvme_mq_iosched_exit,
	.mq_req_complete_fn = __cfi_nvme_mq_req_complete,
	.mq_tag_get_fn = __cfi_nvme_mq_tag_get,
	.mq_tag_put_fn = __cfi_nvme_mq_tag_put,
	.blk_status_dump_fn = __cfi_nvme_mq_status_dump,
	.scheduler_priv = &mas_nvme_mq,
	.iosched_name = "MAS MQ",
};

struct blk_tagset_ops mas_nvme_blk_tagset_ops = {
	.tagset_init_tags_fn = __cfi_nvme_tagset_init_tags,
	.queue_ops = &mas_nvme_blk_queue_ops,
};
