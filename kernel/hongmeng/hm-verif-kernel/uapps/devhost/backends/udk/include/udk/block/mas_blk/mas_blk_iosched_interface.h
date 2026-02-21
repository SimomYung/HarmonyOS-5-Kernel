/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  MAS MQ ioscheduler interface
 */

#ifndef __MAS_BLK_IOSCHED_INTERFACE_H__
#define __MAS_BLK_IOSCHED_INTERFACE_H__

#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>

extern int request_to_qc_t(struct udk_blk_mq_hw_ctx *hctx, struct udk_request *rq);
extern struct blk_tagset_ops mas_ufs_blk_tagset_ops;

extern struct mas_ufs_mq_priv mas_ufs_mq;

extern int ufs_mq_iosched_init(struct udk_request_queue *q);
extern void ufs_mq_iosched_exit(struct udk_request_queue *q);
extern void ufs_mq_req_init(const struct udk_blk_mq_ctx *ctx, struct udk_request *rq);
extern void ufs_mq_req_complete(
	struct udk_request *rq, const struct udk_request_queue *q);
extern void ufs_mq_req_deinit(struct udk_request *rq);
extern void ufs_mq_req_insert(
	struct udk_request *req, struct udk_request_queue *q);
extern void ufs_mq_req_requeue(
	struct udk_request *req, const struct udk_request_queue *q);
extern int ufs_mq_tag_get(struct udk_blk_mq_tags *tags,
	struct udk_bio *bio, struct udk_request_queue *queue);
extern void ufs_mq_tag_put(struct udk_blk_mq_tags *tags,
	unsigned int tag, struct udk_request_queue *queue);
extern int ufs_mq_tag_update_depth(void);
extern void ufs_mq_exec_queue(struct udk_request_queue *q);
extern void ufs_mq_run_hw_queue(struct udk_request_queue *q);
extern void ufs_mq_run_requeue(struct udk_request_queue *q);
extern void ufs_mq_poll_enable(bool *enable);
extern void ufs_mq_status_dump(
	const struct udk_request_queue *q, enum blk_dump_scene s);
extern void ufs_mq_async_io_dispatch_work_fn(struct udk_work *work_p);
struct mas_ufs_sched_ds_lld;
extern void ufs_mq_sync_burst_check_timer_expire(struct mas_ufs_sched_ds_lld *sched_ds_lld);
extern void ufs_mq_sync_io_dispatch_work_fn(const struct udk_work *work);

extern void ufs_mq_write_throttle_check_timer_expire(struct mas_ufs_sched_ds_lld *sched_ds_lld);
extern void ufs_mq_write_throttle_handler(struct udk_request_queue *q, bool level);

extern void __cfi_ufs_mq_write_throttle_check_timer_expire(unsigned long data);

extern int __cfi_ufs_mq_async_io_dispatch_work_fn(struct udk_work *work);

extern void __cfi_ufs_mq_sync_burst_check_timer_expire(unsigned long data);
extern int __cfi_ufs_mq_sync_io_dispatch_work_fn(struct udk_work *work);
extern int __cfi_ufs_mq_io_guard_work_fn(struct udk_work *work);
extern void ufs_mq_io_guard_work_fn(struct udk_work *work);
extern void ufs_tagset_power_off_proc(struct blk_dev_lld *lld);

int __cfi_ufs_mq_make_request(struct udk_bio *bio);
int ufs_mq_make_request(struct udk_bio *bio);
int get_mq_all_tag_used(struct udk_request_queue *q);
int get_mq_prio_tag_used(struct udk_request_queue *q);
bool ufs_order_panic_wait_datasync_handle(struct blk_dev_lld *blk_lld);
void blk_mq_tagset_ufs_mq_iosched_enable(
	struct udk_blk_mq_tag_set *tag_set, int enable);
struct udk_request *ufs_mq_seek_request(const struct udk_request_queue *q);
void ufs_mq_async_requeue(
	const struct udk_request *rq, const struct udk_request_queue *q);
struct udk_request *ufs_mq_pick_sync_rq(
	const struct mas_ufs_sched_ds_lld *ds_lld);
void ufs_mq_bio_is_sync(struct udk_request_queue *q, struct udk_bio *bio);
void ufs_mq_requeue_sync_list(
	struct udk_request *rq, struct udk_request_queue *q);
void ufs_mq_rq_inflt_update(
	const struct udk_request *rq,
	struct mas_ufs_sched_ds_lld *ds_lld, bool update_complete_time);
int ufs_mq_fs_io_limit(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld);
struct mas_ufs_sched_ds_lld *get_sched_ds_lld(
	const struct udk_request_queue *q);
void ufs_mq_merge_queue_io(struct udk_blk_mq_hw_ctx *hctx,
	struct udk_blk_mq_ctx *ctx, struct udk_request *rq,
	struct udk_bio *bio);
void ufs_mq_sync_burst_dispatch_check(struct mas_ufs_sched_ds_lld *ds_lld);

int ufs_mq_sched_request(struct udk_request *rq, struct udk_request_queue *queue);
int ufs_mq_make_async_request(struct udk_blk_mq_hw_ctx *hctx, struct udk_request *rq);

#ifdef CONFIG_MAS_DEBUG_FS
#ifdef CONFIG_MAS_MQ_USING_CP
int mas_queue_cp_enabled_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize);
int mas_queue_cp_enabled_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
int mas_queue_cp_debug_en_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize);
int mas_queue_cp_debug_en_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
int mas_queue_cp_limit_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize);
int mas_queue_cp_limit_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
#endif
#endif
#endif /* __MAS_BLK_IOSCHED_INTERFACE_H__ */
