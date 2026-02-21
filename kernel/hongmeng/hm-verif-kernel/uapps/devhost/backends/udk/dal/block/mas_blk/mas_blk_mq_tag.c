/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: mas block MQ tag
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_block_stats.h>
#include <udk/block/mas_blk/mas_blk_mq_tag.h>
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/block/mas_blk/mas_blk_iosched_interface.h>
#include <udk/block/mas_blk/mas_blk_dump_interface.h>
#include <udk/block/mas_blk/mas_blk_flush_interface.h>
#include <udk/block/mas_blk/mas_blk_latency_interface.h>
#include <udk/block/mas_blk/mas_blk_busy_idle_notify.h>

void ufs_tagset_free_tags(struct udk_blk_mq_tags *tags)
{
	udk_blk_mq_free_tags(tags);
}

struct udk_blk_mq_tags *ufs_tagset_init_tags(struct udk_blk_mq_tag_set *set)
{
	struct udk_blk_mq_tags *tags = NULL;
	int ret = 0;

	tags = udk_blk_mq_alloc_tags(set->queue_depth);
	if (!tags)
		return NULL;

	ret = memcpy_s(tags->prio_info, sizeof(struct mas_prio_queue_info) * MAS_MAX_PRIO_TYPES,
		set->prio_info, sizeof(struct mas_prio_queue_info) * MAS_MAX_PRIO_TYPES);
	if (ret != 0) {
		udk_blk_mq_free_tags(tags);
		return NULL;
	}

	return tags;
}

void mas_tag_tags_init(bool ufs_flag, struct udk_blk_mq_tag_set *tag_set)
{
	unsigned int prio_type;
	unsigned int tag_offset = 0;

	if (!ufs_flag)
		return;

	tag_set->flags &= ~UDK_BLK_MQ_TAG_HCTX_SHARED;
	for (prio_type = MAS_NORMAL_PRIO; prio_type < MAS_MAX_PRIO_TYPES; prio_type++) {
		tag_set->prio_info[prio_type].tag_offset = tag_offset;
		tag_set->prio_info[prio_type].tag_num = tag_set->queue_depth;
		tag_offset += tag_set->queue_depth;
	}

	tag_set->queue_depth = MAS_MAX_PRIO_TYPES * tag_set->queue_depth;
	blk_mq_tagset_direct_flush_register(tag_set, mas_blk_direct_flush_fn);
	blk_mq_tagset_dump_register(tag_set, (lld_dump_status_fn)NULL);
	blk_mq_tagset_ufs_mq_iosched_enable(tag_set, true);
	blk_mq_tagset_latency_warning_set(tag_set, 2000);
	(void)udk_blk_register_hungtask_notifier(mas_blk_panic_dump_notify, NULL);
	blk_mq_tagset_busyidle_enable(tag_set, true);
	blk_mq_tagset_flush_reduce_config(tag_set, true);
}
