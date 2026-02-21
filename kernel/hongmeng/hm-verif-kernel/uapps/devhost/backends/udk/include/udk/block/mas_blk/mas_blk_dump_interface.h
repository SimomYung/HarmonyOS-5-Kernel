/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  mas block dump interface
 */

#ifndef __MAS_BLK_DUMP_INTERFACE_H__
#define __MAS_BLK_DUMP_INTERFACE_H__

#include <udk/notifier.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>

#ifdef CONFIG_MAS_BLK
void mas_blk_dump_register_queue(struct udk_request_queue *q);
void mas_blk_dump_unregister_queue(struct udk_request_queue *q);
void blk_mq_tagset_dump_register(
	struct udk_blk_mq_tag_set *tag_set, lld_dump_status_fn func);
void blk_queue_dump_register(struct udk_request_queue *q, lld_dump_status_fn func);
int mas_blk_panic_dump_notify(void *args);

extern int mas_blk_panic_notify(unsigned int cmd, unsigned long arg);
extern int __cfi_mas_blk_panic_notify(unsigned int cmd, unsigned long arg);
#else
static inline int __cfi_mas_blk_panic_notify(unsigned int cmd, unsigned long arg)
{
	UNUSED(cmd);
	UNUSED(arg);
	return 0;
}
#endif

#endif /* __MAS_BLK_DUMP_INTERFACE_H__ */
