/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "ukmd_log.h"

#include <linux/file.h>
#include <linux/sync_file.h>

#include "ukmd_release_fence.h"
#include "ukmd_fence_utils.h"

static const char *release_fence_get_driver_name(struct dma_fence *fence)
{
	struct ukmd_release_fence *ukmd_fence = container_of(fence, struct ukmd_release_fence, base);

	return ukmd_fence->name;
}

static const char *release_fence_get_timeline_name(struct dma_fence *fence)
{
	struct ukmd_timeline *timeline = release_fence_get_timeline(fence);

	return timeline->name;
}

static bool release_fence_enable_signaling(struct dma_fence *fence)
{
	return true;
}

static bool release_fence_is_signaled_gt(struct dma_fence *fence)
{
	struct ukmd_timeline *timeline = release_fence_get_timeline(fence);
	bool ret = ((s32)(timeline->pt_value - fence->seqno)) > 0;

	if (g_debug_ukmd_fence)
		ukmd_pr_info("fence signal=%d at timeline:%s timeline_value=%llu fence->seqno:%llu",
					ret, timeline->name, timeline->pt_value, fence->seqno);

	return ret;
}

static void release_fence_release(struct dma_fence *fence)
{
	if (g_debug_ukmd_fence)
		ukmd_pr_info("timeline %s release for fence %s\n",
			fence->ops->get_timeline_name(fence),
			fence->ops->get_driver_name(fence));

	kfree_rcu(fence, rcu);
}

static void release_fence_value_str(struct dma_fence *fence, char *str, int32_t size)
{
	int ret;
	ret = snprintf(str, (size_t)size, "%llu", fence->seqno);
	if (ret < 0)
		ukmd_pr_err("snprintf of fence->seqno failed!\n");
}

static void release_fence_timeline_value_str(struct dma_fence *fence, char *str, int32_t size)
{
	int ret;
	struct ukmd_timeline *timeline = release_fence_get_timeline(fence);

	ret = snprintf(str, (size_t)size, "%llu", timeline->pt_value);
	if (ret < 0)
		ukmd_pr_err("snprintf of timeline->pt_value failed!\n");
}

static struct dma_fence_ops g_release_fence_ops = {
	.get_driver_name = release_fence_get_driver_name,
	.get_timeline_name = release_fence_get_timeline_name,
	.enable_signaling = release_fence_enable_signaling,
	.signaled = release_fence_is_signaled_gt,
	.wait = dma_fence_default_wait,
	.release = release_fence_release,
	.fence_value_str = release_fence_value_str,
	.timeline_value_str = release_fence_timeline_value_str,
};

static void release_fence_init(struct ukmd_release_fence *fence,
	struct ukmd_timeline *timeline, uint64_t pt_value)
{
	int ret;
	fence->timeline = timeline;

	dma_fence_init(&fence->base, &g_release_fence_ops,
		 &timeline->value_lock, timeline->context, pt_value);

	ret = snprintf(fence->name, UKMD_SYNC_NAME_SIZE, "ukmd_release_fence_%llu", pt_value);
	if (ret < 0)
		ukmd_pr_err("snprintf of ukmd_release_fence failed!\n");
}

/* release fence listener begin */
static const char *release_fence_get_name(struct ukmd_timeline_listener *listener)
{
	struct ukmd_release_fence *fence = (struct ukmd_release_fence *)listener->listener_data;
	return release_fence_get_driver_name(&fence->base);
}

static bool release_fence_is_signaled(struct ukmd_timeline_listener *listener, uint64_t tl_val)
{
	struct ukmd_release_fence *release_fence = (struct ukmd_release_fence *)listener->listener_data;
	struct dma_fence *fence = &release_fence->base;

	if (test_bit(DMA_FENCE_FLAG_SIGNALED_BIT, &fence->flags))
		return true;

	if (fence->ops->signaled && fence->ops->signaled(fence)) {
		dma_fence_signal_timestamp(fence, listener->notify_timestamp);
		return true;
	}

	return false;
}

static int32_t release_fence_handle_signal(struct ukmd_timeline_listener *listener)
{
	struct ukmd_release_fence *fence = (struct ukmd_release_fence *)listener->listener_data;
	dma_fence_put(&fence->base);
	return 0;
}

static struct ukmd_timeline_listener_ops g_release_fence_listener_ops = {
	.get_listener_name = release_fence_get_name,
	.is_signaled = release_fence_is_signaled,
	.handle_signal = release_fence_handle_signal,
	.release = NULL,
};
/* release fence listener end */

int32_t ukmd_release_fence_create(struct ukmd_timeline *timeline, uint64_t *fence_pt)
{
	int32_t fd;
	uint64_t next_value;
	struct ukmd_release_fence *fence = NULL;
	struct ukmd_timeline_listener *listener = NULL;

	if (!timeline || !fence_pt) {
		ukmd_pr_err("timeline input error is null");
		return -1;
	}

	fence = kzalloc(sizeof(*fence), GFP_KERNEL);
	if (!fence) {
		ukmd_pr_err("kzalloc fence fail");
		return -ENOMEM;
	}
	next_value = ukmd_timeline_get_next_value(timeline);
	release_fence_init(fence, timeline, next_value);

	fd = ukmd_fence_get_fence_fd(&fence->base);
	if (fd < 0) {
		ukmd_timeline_dec_next_value(timeline);
		kfree(fence);
		fence = NULL;
		return -1;
	}

	listener = ukmd_timeline_alloc_listener(&g_release_fence_listener_ops, fence, next_value);
	if (!listener) {
		ukmd_pr_err("alloc fence listener fail, fence_pt=%llu", next_value);
		ukmd_timeline_dec_next_value(timeline);
		put_unused_fd((uint32_t)fd);
		kfree(fence);
		fence = NULL;
		return -1;
	}
	ukmd_timeline_add_listener(timeline, listener);

	if (g_debug_ukmd_fence)
		ukmd_pr_info("fence created at val=%llu timeline:%s timeline_value=%llu, next_value=%u",
			next_value, timeline->name, timeline->pt_value, timeline->next_value);

	*fence_pt = next_value;
	return fd;
}
