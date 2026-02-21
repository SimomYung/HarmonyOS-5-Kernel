/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes for devhost shrinker APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 10 03:01:34 2023
 */

#ifndef __DEVHOST_API_SHRINKER_H__
#define __DEVHOST_API_SHRINKER_H__

#include <lib/dlist.h>

#define SHRINK_SIZE_ALL (-1)

enum devhost_shrinker_mode {
	DEVHOST_SHRINKER_FAST,
	DEVHOST_SHRINKER_SLOW,
	MAX_SHRINKER_MODE,
};

struct devhost_shrinker {
	struct dlist_node dnode;
	unsigned long (*query)(struct devhost_shrinker *shrinker, enum devhost_shrinker_mode type);
	unsigned long (*shrink)(struct devhost_shrinker *shrinker, enum devhost_shrinker_mode type, int max_nr);
};

void devhost_shrinker_register(struct devhost_shrinker *shrinker);
int devhost_shrinker_do_shrink(enum devhost_shrinker_mode type, int max_nr);
int devhost_shrinker_query(int type);
int devhost_shrinker_shrink(int max_nr);

#endif /* __DEVHOST_API_SHRINKER_H__ */
