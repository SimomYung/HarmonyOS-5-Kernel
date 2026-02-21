/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  mas block flush reduce interface
 */

#ifndef __MAS_BLK_FLUSH_INTERFACE_H__
#define __MAS_BLK_FLUSH_INTERFACE_H__

#include <udk/notifier.h>
#include <udk/workqueue.h>
#include <udk/block/udk_disk.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>

#ifdef CONFIG_MAS_BLK
extern void __cfi_mas_blk_flush_work_fn(unsigned long val);
extern void mas_blk_flush_work_fn(unsigned long val);
extern int mas_blk_poweroff_flush_notifier_call(unsigned int cmd, unsigned long arg);
extern int __cfi_mas_blk_flush_init(void);
extern int mas_blk_flush_init(void);
extern void mas_blk_flush_reduced_queue_unregister(struct udk_request_queue *q);
extern void mas_blk_flush_list_register(struct dlist_node *lld_list);
extern void mas_blk_flush_list_unregister(struct dlist_node *lld_list);

bool mas_blk_flush_async_dispatch(struct udk_request_queue *q,
	const struct udk_bio *bio);
void mas_blk_flush_update(struct udk_request_queue *queue, int error);
void blk_power_off_flush(int emergency);
void mas_blk_panic_flush(void);
void blk_flush_set_async(struct udk_bio *bio);
void blk_mq_tagset_flush_reduce_config(
	struct udk_blk_mq_tag_set *tag_set, bool flush_reduce_enable);
void mas_blk_queue_async_flush_init(struct udk_request_queue *q);
int blk_flush_async_support(const struct udk_disk *disk);

extern int __cfi_mas_blk_poweroff_flush_notifier_call(unsigned int cmd, unsigned long arg);
#else
static inline int __cfi_mas_blk_poweroff_flush_notifier_call(unsigned int cmd, unsigned long arg)
{
	UNUSED(cmd);
	UNUSED(arg);
	return 0;
}

static inline void blk_flush_set_async(struct udk_bio *bio)
{
	UNUSED(bio);
}
#endif

#endif /* __MAS_BLK_FLUSH_INTERFACE_H__ */
