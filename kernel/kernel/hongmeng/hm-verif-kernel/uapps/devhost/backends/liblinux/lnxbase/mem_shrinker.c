/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LDK memory shrinker
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 29 10:09:19 2021
 */
#include "mem_shrinker.h"

#include <libhmsync/raw_mutex_guard.h>
#include <hongmeng/panic.h>
#include <devhost/log.h>
#include <lnxbase/lnxbase.h>

#define MEM_SHRINKER_FLAGS_STOP_REG		(0x1U)

struct shrinker_group_s {
	struct raw_mutex mutex;
	struct dlist_node head;
	unsigned long flags;
};

static struct shrinker_group_s shrinker_group = {
	.mutex = RAW_MUTEX_INITIALIZER,
	.head = DLIST_HEAD_INIT(shrinker_group.head),
	.flags = 0,
};

void lnxbase_register_mem_shrinker(struct lnxbase_mem_shrinker *shrinker)
{
	BUG_ON(shrinker == NULL);
	BUG_ON(shrinker->shrink == NULL);
	RAW_MUTEX_GUARD(_, &shrinker_group.mutex);
	if ((shrinker_group.flags & MEM_SHRINKER_FLAGS_STOP_REG) != 0) {
		dh_warn("register mem shrinker failed, register is stopped\n");
		return;
	}
	dlist_insert_tail(&shrinker_group.head, &shrinker->dnode);
}

unsigned long lnxbase_shrink_mem(int size)
{
	struct lnxbase_mem_shrinker *shrinker_node;
	unsigned long cnt;
	unsigned long cnt_sum = 0;

	if (size < -1) {
		dh_warn("mem shrinker failed, size=%d\n", size);
		return 0;
	}

	raw_mutex_lock(&shrinker_group.mutex);
	if ((shrinker_group.flags & MEM_SHRINKER_FLAGS_STOP_REG) == 0) {
		shrinker_group.flags |= MEM_SHRINKER_FLAGS_STOP_REG;
	}
	raw_mutex_unlock(&shrinker_group.mutex);

	dlist_for_each_entry(shrinker_node, &shrinker_group.head, struct lnxbase_mem_shrinker, dnode) {
		if (size == 0) {
			break;
		}
		cnt = shrinker_node->shrink(shrinker_node, size);
		cnt_sum += cnt;
		if (size == SHRINK_SIZE_ALL) {
			continue;
		}
		size = (unsigned long)(long)size > cnt ? size - (int)(signed long)cnt : 0;
	}
	return cnt_sum;
}
