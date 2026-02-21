/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: mas block core interface
 */

#ifndef __MAS_BLK_CORE_INTERFACE_H__
#define __MAS_BLK_CORE_INTERFACE_H__

#include <libstrict/strict.h>

#include <udk/device.h>
#include <udk/block/udk_disk.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>

#ifdef CONFIG_MAS_BLK
extern int __cfi_mas_queue_status_show(void *ctx, void *dst,
	size_t pos, size_t size, size_t *rsize);
extern int mas_queue_status_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize);

void mas_blk_mq_init_allocated_queue(struct udk_request_queue *q);
void mas_blk_mq_free_queue(const struct udk_request_queue *q);
void mas_blk_sq_init_allocated_queue(struct udk_request_queue *q);
void mas_blk_allocated_queue_init(struct udk_request_queue *q);
bool mas_blk_generic_make_request_check(struct udk_bio *bio);
void mas_blk_check_partition_done(struct udk_disk *disk, bool has_part_tbl);
void mas_blk_request_update(const struct udk_request *req, int error,
	unsigned int nr_bytes);
void mas_blk_queue_register(struct udk_request_queue *q, const struct udk_disk *disk);
#endif

#ifdef CONFIG_MAS_MQ_USING_CP
bool mas_blk_get_cp_flag(void);
void mas_blk_set_cp_flag(bool val);
int mas_blk_cp_enable(unsigned int cmd, unsigned long arg);
#else
static inline bool mas_blk_get_cp_flag(void)
{
	return false;
}

static inline void mas_blk_set_cp_flag(bool val)
{
	UNUSED(val);
}

static inline int mas_blk_cp_enable(unsigned int cmd, unsigned long arg)
{
	UNUSED(cmd);
	UNUSED(arg);
	return 0;
}
#endif /* CONFIG_MAS_MQ_USING_CP */
#endif
