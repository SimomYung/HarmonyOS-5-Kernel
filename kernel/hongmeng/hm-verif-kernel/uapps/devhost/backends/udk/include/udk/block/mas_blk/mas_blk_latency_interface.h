/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: mas block io latency interface
 */

#ifndef __MAS_BLK_LATENCY_INTERFACE_H__
#define __MAS_BLK_LATENCY_INTERFACE_H__

#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/mas_blk/mas_blk.h>

extern const struct bio_delay_stage_config bio_stage_cfg[BIO_PROC_STAGE_MAX];
extern const struct req_delay_stage_config req_stage_cfg[REQ_PROC_STAGE_MAX];

extern void mas_blk_latency_check_timer_expire(struct udk_bio *bio);
extern void __mas_blk_latency_check_timer_expire(unsigned long data);

extern int mas_queue_io_latency_warning_threshold_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
extern int __mas_queue_io_latency_warning_threshold_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);

void mas_blk_queue_latency_init(struct udk_request_queue *q);
void blk_mq_tagset_latency_warning_set(
	struct udk_blk_mq_tag_set *tag_set, unsigned int warning_threshold_ms);
void blk_queue_latency_warning_set(
	struct udk_request_queue *q, unsigned int warning_threshold_ms);

#endif /* __MAS_BLK_LATENCY_INTERFACE_H__ */
