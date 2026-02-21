/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: mas block io latency interface
 */

#include <udk/block/mas_blk/mas_blk_latency_interface.h>

void __mas_blk_latency_check_timer_expire(unsigned long val)
{
	struct udk_timer *timer = (struct udk_timer *)val;
	struct blk_bio_cust *mas_bio = (struct blk_bio_cust *)container_of(timer,
			struct blk_bio_cust, latency_expire_timer);
	struct udk_bio *bio = (struct udk_bio *)container_of(mas_bio, struct udk_bio, mas_bio);

	mas_blk_latency_check_timer_expire(bio);
}

int __mas_queue_io_latency_warning_threshold_store(void *ctx, void *src,
	size_t pos, size_t size, size_t *wsize)
{
	return mas_queue_io_latency_warning_threshold_store(ctx, src, pos, size, wsize);
}
