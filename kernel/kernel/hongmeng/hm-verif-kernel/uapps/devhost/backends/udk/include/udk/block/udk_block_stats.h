/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block DFX definitions
 * Author: Huawei OS Kernel Lab
 * Create: Thr Dev 21 14:38:25 2023
 */
#ifndef __UDK_BLOCK_STATS_H__
#define __UDK_BLOCK_STATS_H__

#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <libhmsync/bitops/generic.h>

#define UDK_BIO_INFO_MAX_COUNT	CONFIG_UDK_BIO_INFO_MAX_COUNT

typedef int (*udk_hungtask_notifier)(void *args);

enum udk_bio_stage_enum {
	UDK_BIO_STAGE_UNINITIALIZED = 0,
	UDK_BIO_STAGE_SUBMIT,
	UDK_BIO_STAGE_REQUEUE,
	UDK_BIO_STAGE_REQUEUE_DISPATCH,
	UDK_BIO_STAGE_TIMEOUT,
	UDK_BIO_STAGE_EH,
	UDK_BIO_STAGE_ENDIO,
	UDK_BIO_STAGE_FINISH,
	UDK_BIO_STAGE_NUM,
};

void udk_bio_info_set_stage(struct udk_bio *bio, int stage);
void udk_request_info_set_stage(struct udk_request *req, int stage);
int udk_bio_register_hungtask_notifier(void);
int udk_blk_register_hungtask_notifier(udk_hungtask_notifier notify_func, void *args);

#endif /* __UDK_BLOCK_STATS_H__ */
