/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  MAS MQ ioscheduler interface
 */

#ifndef __MAS_BLK_IOSCHED_NVME_INTERFACE_H__
#define __MAS_BLK_IOSCHED_NVME_INTERFACE_H__

#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>

extern struct blk_tagset_ops mas_nvme_blk_tagset_ops;

extern struct mas_nvme_mq_priv mas_nvme_mq;

extern int nvme_mq_iosched_init(struct udk_request_queue *q);
extern void nvme_mq_iosched_exit(struct udk_request_queue *q);
extern void nvme_mq_req_complete(
	struct udk_request *rq, const struct udk_request_queue *q);
extern int nvme_mq_tag_get(struct udk_blk_mq_tags *tags,
	struct udk_bio *bio, struct udk_request_queue *queue);
extern void nvme_mq_tag_put(struct udk_blk_mq_tags *tags,
	unsigned int tag, struct udk_request_queue *queue);
extern void nvme_mq_status_dump(
	const struct udk_request_queue *q, enum blk_dump_scene s);
extern void nvme_mq_async_io_dispatch_work_fn(struct udk_work *work_p);
struct mas_nvme_sched_ds_lld;
extern void nvme_mq_sync_burst_check_timer_expire(struct mas_nvme_sched_ds_lld *sched_ds_lld);
extern void nvme_mq_sync_io_dispatch_work_fn(const struct udk_work *work);

extern void nvme_mq_write_throttle_check_timer_expire(struct mas_nvme_sched_ds_lld *sched_ds_lld);
extern void nvme_mq_write_throttle_handler(struct udk_request_queue *q, bool level);

extern void __cfi_nvme_mq_write_throttle_check_timer_expire(unsigned long data);

extern int __cfi_nvme_mq_async_io_dispatch_work_fn(struct udk_work *work);

extern void __cfi_nvme_mq_sync_burst_check_timer_expire(unsigned long data);
extern int __cfi_nvme_mq_sync_io_dispatch_work_fn(struct udk_work *work);
extern int __cfi_nvme_mq_io_guard_work_fn(struct udk_work *work);
extern void nvme_mq_io_guard_work_fn(struct udk_work *work);
extern void nvme_tagset_power_off_proc(struct blk_dev_lld *lld);

void blk_mq_tagset_nvme_mq_iosched_enable(
	struct udk_blk_mq_tag_set *tag_set, int enable);
struct udk_request *nvme_mq_seek_request(const struct udk_request_queue *q);
void nvme_mq_async_requeue(
	const struct udk_request *rq, const struct udk_request_queue *q);
void nvme_mq_bio_is_sync(struct udk_request_queue *q, struct udk_bio *bio);
void nvme_mq_requeue_sync_list(
	struct udk_request *rq, struct udk_request_queue *q);
void nvme_mq_rq_inflt_update(
	const struct udk_request *rq,
	struct mas_nvme_sched_ds_lld *ds_lld, bool update_complete_time);
int nvme_mq_fs_io_limit(
	struct udk_request *rq, struct mas_nvme_sched_ds_lld *ds_lld);
void nvme_mq_sync_burst_dispatch_check(struct mas_nvme_sched_ds_lld *ds_lld);

int nvme_mq_sched_request(struct udk_request *rq, struct udk_request_queue *queue);
int nvme_mq_make_async_request(struct udk_blk_mq_hw_ctx *hctx, struct udk_request *rq);

#endif /* __MAS_BLK_IOSCHED_NVME_INTERFACE_H__ */
