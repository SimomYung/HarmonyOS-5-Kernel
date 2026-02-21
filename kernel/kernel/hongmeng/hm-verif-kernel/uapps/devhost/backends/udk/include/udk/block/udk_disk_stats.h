/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: UDK block operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 08 18:04:16 2023
 */
#ifndef __UDK_DISK_STATS_H__
#define __UDK_DISK_STATS_H__

#include <udk/block/udk_block.h>

struct udk_bio;

#define stat_atomic32_t vatomic32_t
#define stat_atomic64_t vatomic64_t
#define stat_atomic_set(ptr, val) atomic_set(ptr, val)
#define stat_atomic32_read(ptr) vatomic32_read_rlx(ptr)
#define stat_atomic64_read(ptr) vatomic64_read_rlx(ptr)
#define stat_atomic32_inc(ptr) vatomic32_inc_get_rlx(ptr)
#define stat_atomic32_dec(ptr) vatomic32_dec_get_rlx(ptr)
#define stat_atomic32_add(ptr, val) vatomic32_add_rlx(ptr, val)
#define stat_atomic32_sub(ptr, val) vatomic32_sub_rlx(ptr, val)
#define stat_atomic64_add(ptr, val) vatomic64_add_rlx(ptr, val)

enum stat_group {
	STAT_READ,
	STAT_WRITE,
	STAT_DISCARD,
	STAT_FLUSH,
	NR_STAT_GROUPS,
};

struct udk_diskstats {
	stat_atomic64_t nsecs[NR_STAT_GROUPS];
	stat_atomic64_t nsecs_margin[NR_STAT_GROUPS];
	stat_atomic64_t max_nsecs[NR_STAT_GROUPS];
	stat_atomic32_t sectors[NR_STAT_GROUPS];
	stat_atomic32_t ios[NR_STAT_GROUPS];
	stat_atomic32_t ios_margin[NR_STAT_GROUPS];
	stat_atomic32_t merges[NR_STAT_GROUPS];
	stat_atomic64_t io_ticks;
	stat_atomic32_t inflight;
};

void udk_diskstats_inc_bio_and_sectors(struct udk_bio *bio);
void udk_diskstats_dec_bio_and_sectors(struct udk_bio *bio);
void udk_diskstats_update_nsecs(struct udk_bio *bio);
void udk_diskstats_update_merges(struct udk_bio *bio);
#endif /* __UDK_DISK_STATS_H__ */
