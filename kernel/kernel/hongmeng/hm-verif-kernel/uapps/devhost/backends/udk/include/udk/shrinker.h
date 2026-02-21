/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Udk shrinker api
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 30 10:43:58 2024
 */

#ifndef __UDK_SHRINKER_H__
#define __UDK_SHRINKER_H__

#include <devhost/shrinker.h>

#define SHRINK_STOP	(~0UL)
#define SHRINK_EMPTY	(~0UL - 1)
#define GFP_KERNEL	0

#define DEFAULT_SEEKS 2 /* A good number if you don't know better. */

typedef unsigned gfp_t;

/* udk shrinker */
struct udk_shrink_control {
	gfp_t gfp_mask;
	int nid;
	unsigned long nr_to_scan;
	unsigned long nr_scanned;
};

struct udk_shrinker {
	unsigned long (*count_objects)(struct udk_shrinker *shrinker,
				       struct udk_shrink_control *sc);
	unsigned long (*scan_objects)(struct udk_shrinker *shrinker,
				      struct udk_shrink_control *sc);
	long batch;
	int seeks;
};

int udk_register_shrinker(struct udk_shrinker *shrinker);
int udk_shrinker_query(int flag);
int udk_shrinker_shrink(int max_nr, int *ret_nr);
#endif /* __UDK_SHRINKER_H__ */
