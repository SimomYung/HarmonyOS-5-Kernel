/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: UDK block operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 08 18:04:16 2023
 */
#include <stdio.h>
#include <udk/log.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_disk_stats.h>
#include <udk/block/udk_partition.h>
#include "udk_block_internal.h"


#define BIO_SECTORS 9

static int udk_diskstats_get_type(struct udk_bio *bio)
{
	int type = udk_bio_req_op(bio);

	switch (type) {
	case UDK_REQ_WRITE:
		return STAT_WRITE;
	case UDK_REQ_READ:
		return STAT_READ;
	case UDK_REQ_DISCARD:
		return STAT_DISCARD;
	case UDK_REQ_FLUSH:
		return STAT_FLUSH;
	default:
		udk_warn("wrong udk req type:%d\n", type);
		return -1;
	}
}

static inline bool has_parent_dev(struct udk_bio *bio)
{
	return (bio->bi_part_dev->parent != NULL && bio->bi_part_dev->parent != bio->bi_part_dev);
}

static void udk_diskstats_inc_bio(struct udk_bio *bio, int stat)
{
	struct udk_partition *part = udkdev_to_part(bio->bi_part_dev);
	struct udk_partition *parent_part = NULL;

	stat_atomic32_inc(&part->diskstats.inflight);
	stat_atomic32_inc(&part->diskstats.ios[stat]);
	if (has_parent_dev(bio)) {
		parent_part = udkdev_to_part(bio->bi_part_dev->parent);
		stat_atomic32_inc(&parent_part->diskstats.inflight);
		stat_atomic32_inc(&parent_part->diskstats.ios[stat]);
	}
}

static void udk_diskstats_dec_bio(struct udk_bio *bio, int stat)
{
	struct udk_partition *part = udkdev_to_part(bio->bi_part_dev);
	struct udk_partition *parent_part = NULL;

	stat_atomic32_dec(&part->diskstats.inflight);
	stat_atomic32_dec(&part->diskstats.ios[stat]);
	if (has_parent_dev(bio)) {
		parent_part = udkdev_to_part(bio->bi_part_dev->parent);
		stat_atomic32_dec(&parent_part->diskstats.inflight);
		stat_atomic32_dec(&parent_part->diskstats.ios[stat]);
	}
}

static void udk_diskstats_inc_sectors(struct udk_bio *bio, int stat)
{
	struct udk_partition *part = udkdev_to_part(bio->bi_part_dev);
	struct udk_partition *parent_part = NULL;

	stat_atomic32_add(&part->diskstats.sectors[stat], (bio->bi_size >> BIO_SECTORS));
	if (has_parent_dev(bio)) {
		parent_part = udkdev_to_part(bio->bi_part_dev->parent);
		stat_atomic32_add(&parent_part->diskstats.sectors[stat], (bio->bi_size >> BIO_SECTORS));
	}
}

static void udk_diskstats_dec_sectors(struct udk_bio *bio, int stat)
{
	struct udk_partition *part = udkdev_to_part(bio->bi_part_dev);
	struct udk_partition *parent_part = NULL;

	stat_atomic32_sub(&part->diskstats.sectors[stat], (bio->bi_size >> BIO_SECTORS));
	if (has_parent_dev(bio)) {
		parent_part = udkdev_to_part(bio->bi_part_dev->parent);
		stat_atomic32_sub(&parent_part->diskstats.sectors[stat], (bio->bi_size >> BIO_SECTORS));
	}
}

void udk_diskstats_inc_bio_and_sectors(struct udk_bio *bio)
{
	int stat = udk_diskstats_get_type(bio);
	if (stat >= 0 && bio->bi_dev != NULL) {
		/* bi_part_dev should be set before any return, cause for diskstats use. */
		bio->bi_part_dev = bio->bi_dev;
		(void)udk_clock_gettime(CLOCK_MONOTONIC, &bio->start);
		udk_diskstats_inc_bio(bio, stat);
		udk_diskstats_inc_sectors(bio, stat);
	}
}

void udk_diskstats_dec_bio_and_sectors(struct udk_bio *bio)
{
	int stat = udk_diskstats_get_type(bio);
	if (stat >= 0) {
		udk_diskstats_dec_sectors(bio, stat);
		udk_diskstats_dec_bio(bio, stat);
	}
}

void udk_diskstats_update_merges(struct udk_bio *bio)
{
	int stat = udk_diskstats_get_type(bio);
	struct udk_partition *part = udkdev_to_part(bio->bi_part_dev);
	struct udk_partition *parent_part = NULL;

	if (stat >= 0) {
		stat_atomic32_inc(&part->diskstats.merges[stat]);
		if (has_parent_dev(bio)) {
			parent_part = udkdev_to_part(bio->bi_part_dev->parent);
			stat_atomic32_inc(&parent_part->diskstats.merges[stat]);
		}
	}
}

void udk_diskstats_update_nsecs(struct udk_bio *bio)
{
	uint64_t duration;
	int stat = udk_diskstats_get_type(bio);
	struct udk_partition *part = udkdev_to_part(bio->bi_part_dev);
	struct udk_partition *parent_part = NULL;
	uint64_t cur_max_nsec;

	if (stat >= 0) {
		(void)udk_clock_gettime(CLOCK_MONOTONIC, &bio->finish);
		duration = (uint64_t)((bio->finish.tv_sec - bio->start.tv_sec) * __NSEC_PER_SEC +
					(bio->finish.tv_nsec - bio->start.tv_nsec));

		stat_atomic32_dec(&part->diskstats.inflight);
		stat_atomic64_add(&part->diskstats.io_ticks, duration);
		stat_atomic64_add(&part->diskstats.nsecs[stat], duration);
		cur_max_nsec = stat_atomic64_read(&part->diskstats.max_nsecs[stat]);
		if (cur_max_nsec < duration) {
			vatomic64_init(&part->diskstats.max_nsecs[stat], duration);
		}
		if (has_parent_dev(bio)) {
			parent_part = udkdev_to_part(bio->bi_part_dev->parent);
			stat_atomic32_dec(&parent_part->diskstats.inflight);
			stat_atomic64_add(&parent_part->diskstats.io_ticks, duration);
			stat_atomic64_add(&parent_part->diskstats.nsecs[stat], duration);
		}
	}
}

void udk_disk_stat_init(struct udk_partition *part)
{
	vatomic32_init(&part->diskstats.inflight, 0);
	vatomic64_init(&part->diskstats.io_ticks, 0);
	for (int group = 0; group < NR_STAT_GROUPS; group++) {
		vatomic32_init(&part->diskstats.ios[group], 0);
		vatomic32_init(&part->diskstats.ios_margin[group], 0);
		vatomic32_init(&part->diskstats.merges[group], 0);
		vatomic32_init(&part->diskstats.sectors[group], 0);
		vatomic64_init(&part->diskstats.nsecs[group], 0);
		vatomic64_init(&part->diskstats.nsecs_margin[group], 0);
		vatomic64_init(&part->diskstats.max_nsecs[group], 0);
	}
}
