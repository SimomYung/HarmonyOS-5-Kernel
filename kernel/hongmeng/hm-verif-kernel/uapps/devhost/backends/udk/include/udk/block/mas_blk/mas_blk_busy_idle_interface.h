/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: block busy idle interface
 */

#ifndef __MAS_BLK_BUSY_IDLE_INTERFACE_H__
#define __MAS_BLK_BUSY_IDLE_INTERFACE_H__

#include <udk/timer.h>
#include <udk/notifier.h>
#include <udk/workqueue.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>

extern void __cfi_mas_blk_busyidle_handler_latency_check_timer_expire(
	unsigned long timer);
extern void mas_blk_busyidle_handler_latency_check_timer_expire(
	struct udk_timer *timer);
extern int __cfi_mas_blk_busyidle_notify_handler(
	struct udk_notifier *nb, unsigned long val, void *v);
extern int mas_blk_busyidle_notify_handler(
	const struct udk_notifier *nb, unsigned long val, const void *v);
extern void __cfi_mas_blk_idle_notify_work(unsigned long val);
extern void mas_blk_idle_notify_work(unsigned long val);
extern void __cfi_mas_blk_busyidle_end_rq(
	struct udk_request *rq, int error);
extern void mas_blk_busyidle_end_rq(
	const struct udk_request *rq, int error);
extern void blk_queue_busyidle_enable(const struct udk_request_queue *q,
	int enable);

#if defined(CONFIG_MAS_DEBUG_FS)
extern int __cfi_mas_queue_busyidle_enable_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
extern int __cfi_mas_queue_busyidle_statistic_reset_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
extern int __cfi_mas_queue_busyidle_statistic_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize);
extern int __cfi_mas_queue_idle_state_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize);

extern int mas_queue_busyidle_enable_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
extern int mas_queue_busyidle_statistic_reset_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize);
extern int mas_queue_busyidle_statistic_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize);
extern int mas_queue_idle_state_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize);
#endif /* CONFIG_MAS_DEBUG_FS */
#endif /* __MAS_BLK_BUSY_IDLE_INTERFACE_H__ */
