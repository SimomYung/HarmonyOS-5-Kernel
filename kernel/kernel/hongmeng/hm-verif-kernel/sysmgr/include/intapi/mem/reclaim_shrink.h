/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Memory shrink reclaim framework
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 13:06:14 2023
 */

#ifndef SYSMGR_INTAPI_MEM_RECLAIM_SHRINK_H
#define SYSMGR_INTAPI_MEM_RECLAIM_SHRINK_H

#include <hongmeng/types.h>
#include <lib/dlist.h>

#define RECLAIM_SHRINK_STOP     (-1)
#define RECLAIM_SHRINK_EMPTY    (-2)
#define RECLAIM_SHRINK_SEEKS    (2)     /* A good number if you don't know better. */
#define RECLAIM_SHRINK_BATCH    (128)
#define RECLAIM_SHRINK_MIN      (10)    /* Minimum objects of reclaims at a time */

/*
 * The "priority" of shrink mem scanning is how much of the objects we will scan in one go.
 * A value of 12 for RECLAIM_SHRINK_PRIORITY implies that we will scan 1/4096th of the
 * objects ("nr_freeable_objects >> 12") during an aging round.
 */
#define RECLAIM_SHRINK_PRIORITY (12)

struct reclaim_shrink_control {
	/*
	 * How many objects reclaim_objects should scan and try to reclaim.
	 */
	unsigned long nr_to_scan;

	/*
	 * How many objects did reclaim_objects process.
	 */
	unsigned long nr_scanned;

	int priority;
};

struct reclaim_shrinker_stat {
	unsigned long long ts_count_call;
	unsigned long long ts_reclaim_call;

	unsigned long nr_count_call;
	unsigned long nr_reclaim_call;

	unsigned long nr_scanned;
	unsigned long nr_freed;
};

struct reclaim_shrinker {
	const char *name;
	int batch; /* reclaim batch size, 0 = default */
	int seeks; /* seeks to recreate an obj */
	int flags; /* reserved for furture */
	struct dlist_node list;
	struct reclaim_shrinker_stat stat;

	/*
	 * How many objects reclaim_objects can be reclaim.
	 */
	unsigned long (*count_objects)(struct reclaim_shrinker *, struct reclaim_shrink_control *);

	/*
	 * Do the objects reclaiming process, with the goal of rsc->nr_to_scan objects be reclaimed.
	 */
	unsigned long (*reclaim_objects)(struct reclaim_shrinker *, struct reclaim_shrink_control *);
};

int  reclaim_shrinker_register(struct reclaim_shrinker *shrinker);
void reclaim_shrinker_unregister(struct reclaim_shrinker *shrinker);
unsigned long reclaim_shrink(int priority, bool fast);
int  reclaim_shrinker_init(void);
void reclaim_shrinker_dump(void);
int reclaim_shrinker_read(char *data, size_t data_len);
int reclaim_shrinker_write(uint64_t pos, const char *src, size_t size, size_t *wsize);
uint64_t reclaim_shrinker_count_free_objects(void);

#endif /* SYSMGR_INTAPI_MEM_RECLAIM_SHRINK_H */
