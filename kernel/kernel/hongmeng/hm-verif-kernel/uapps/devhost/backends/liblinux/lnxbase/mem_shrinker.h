/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Mem shrinker internal APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 7 16:40:43 2022
 */
#ifndef __LNXBASE_MEM_SHRINKER_INTERNAL_H__
#define __LNXBASE_MEM_SHRINKER_INTERNAL_H__

#include <lib/dlist.h>

#define SHRINK_SIZE_ALL (-1)

struct lnxbase_mem_shrinker {
	unsigned long (*shrink)(struct lnxbase_mem_shrinker *_shrinker, int size);
	struct dlist_node dnode;
};
void lnxbase_register_mem_shrinker(struct lnxbase_mem_shrinker *shrinker);

#endif /* __LNXBASE_MEM_SHRINKER_INTERNAL_H__ */
