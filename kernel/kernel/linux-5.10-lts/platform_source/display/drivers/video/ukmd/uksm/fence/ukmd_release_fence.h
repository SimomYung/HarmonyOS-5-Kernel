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

#ifndef UKMD_FENCE_H
#define UKMD_FENCE_H

#include <linux/dma-fence.h>

#include "ukmd_timeline.h"

struct ukmd_release_fence {
	struct dma_fence base;
	struct ukmd_timeline *timeline;
	char name[UKMD_SYNC_NAME_SIZE];
};

static inline struct ukmd_timeline *release_fence_get_timeline(struct dma_fence *fence)
{
	struct ukmd_release_fence *ukmd_fence = container_of(fence, struct ukmd_release_fence, base);

	return ukmd_fence->timeline;
}

/**
 * @brief
 *  return fence fd, if create fence succ, will return >= 0,
 *  else return < 0.
 */
int32_t ukmd_release_fence_create(struct ukmd_timeline *timeline, uint64_t *fence_pt);


#endif