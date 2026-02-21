/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: mas block busy idle interface
 */

#include <udk/block/mas_blk/mas_blk_busy_idle_interface.h>

void __cfi_mas_blk_busyidle_handler_latency_check_timer_expire(
	unsigned long timer)
{
	mas_blk_busyidle_handler_latency_check_timer_expire((struct udk_timer *)timer);
}

int __cfi_mas_blk_busyidle_notify_handler(
	struct udk_notifier *nb, unsigned long val, void *v)
{
	return mas_blk_busyidle_notify_handler(nb, val, v);
}

void __cfi_mas_blk_idle_notify_work(unsigned long val)
{
	mas_blk_idle_notify_work(val);
}

void __cfi_mas_blk_busyidle_end_rq(struct udk_request *rq, int error)
{
	mas_blk_busyidle_end_rq(rq, error);
}

#ifdef CONFIG_MAS_DEBUG_FS
int __cfi_mas_queue_busyidle_enable_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize)
{
	return mas_queue_busyidle_enable_store(ctx, src, pos, size, wsize);
}

int __cfi_mas_queue_busyidle_statistic_reset_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize)
{
	return mas_queue_busyidle_statistic_reset_store(ctx, src, pos, size, wsize);
}

int __cfi_mas_queue_busyidle_statistic_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize)
{
	return mas_queue_busyidle_statistic_show(ctx, dst, pos, size, rsize);
}

int __cfi_mas_queue_idle_state_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize)
{
	return mas_queue_idle_state_show(ctx, dst, pos, size, rsize);
}
#endif /* CONFIG_MAS_DEBUG_FS */
