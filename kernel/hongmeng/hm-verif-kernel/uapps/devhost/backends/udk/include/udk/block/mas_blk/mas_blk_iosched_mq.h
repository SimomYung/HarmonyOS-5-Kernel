/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  MAS MQ ioscheduler interface
 */

#ifndef __MAS_BLK_IOSCHED_MQ_H__
#define __MAS_BLK_IOSCHED_MQ_H__

#ifdef CONFIG_MAS_BLK
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/block/mas_blk/mas_blk_iosched_interface.h>
#ifdef CONFIG_MAS_BLK_NVME
#include <udk/block/mas_blk/mas_blk_iosched_nvme_interface.h>
#endif

static inline int udk_mas_mq_make_async_request(struct udk_blk_mq_hw_ctx *hctx, struct udk_request *req)
{
	int ret = 0;
#ifdef CONFIG_MAS_BLK_NVME
	ret = nvme_mq_make_async_request(hctx, req);
#else
	ret = ufs_mq_make_async_request(hctx, req);
#endif
	return ret;
}

static inline void udk_mas_mq_bio_is_sync(struct udk_request_queue *queue, struct udk_bio *bio)
{
#ifdef CONFIG_MAS_BLK_NVME
	nvme_mq_bio_is_sync(queue, bio);
#else
	ufs_mq_bio_is_sync(queue, bio);
#endif
}

static inline int udk_mas_mq_sched_request(struct udk_request *req, struct udk_request_queue *queue)
{
#ifdef CONFIG_MAS_BLK_NVME
	return nvme_mq_sched_request(req, queue);
#else
	return ufs_mq_sched_request(req, queue);
#endif
}

static inline void udk_mas_tag_tags_init(struct udk_blk_mq_tag_set *set)
{
#ifdef CONFIG_MAS_BLK_NVME
	mas_tag_nvme_tags_init(set->nvme_flag, set);
#else
	mas_tag_tags_init(set->ufs_flag, set);
#endif
}

#endif /* CONFIG_MAS_BLK */
#endif /* __MAS_BLK_IOSCHED_MQ_H__ */
