/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: mas block MQ tag
 */

#ifndef __MAS_BLK_NVME_MQ_TAG_H__
#define __MAS_BLK_NVME_MQ_TAG_H__
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>

extern void nvme_tagset_free_tags(struct udk_blk_mq_tags *tags);
extern struct udk_blk_mq_tags *nvme_tagset_init_tags(struct udk_blk_mq_tag_set *set);

#endif
