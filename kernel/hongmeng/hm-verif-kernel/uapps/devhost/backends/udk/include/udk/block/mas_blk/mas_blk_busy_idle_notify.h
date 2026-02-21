/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: mas block busy idle framework
 */

#ifndef __MAS_BLK_BUSY_IDLE_NOTIFY__
#define __MAS_BLK_BUSY_IDLE_NOTIFY__

#include <udk/lib/dlist.h>
#include <udk/timer.h>
#include <udk/notifier.h>

#if defined(CONFIG_MAS_DEBUG_FS)
#define BLK_IDLE_100MS 100
#define BLK_IDLE_500MS 500
#define BLK_IDLE_1000MS 1000
#define BLK_IDLE_2000MS 2000
#define BLK_IDLE_4000MS 4000
#define BLK_IDLE_6000MS 6000
#define BLK_IDLE_8000MS 8000
#define BLK_IDLE_10000MS 10000
#endif

struct mas_udk_notifier {
	udk_notifier_func func;
	int priority;
	struct dlist_node node;
};

/* Internal structure! */
struct blk_busyidle_nb {
	/* list into subscribed_event_list */
	struct dlist_node subscribed_node;
	struct udk_timer handler_dur_check_timer;
	struct mas_udk_notifier busyidle_nb;
	/* Copy of subscriber's event_node */
	struct blk_busyidle_event_node event_node;
	/* provided by subscriber module */
	/* for internal maintenance */
	enum blk_io_state last_state;
};

int blk_busyidle_event_subscribe(
	const struct udk_request_queue *q,
	const struct blk_busyidle_event_node *event_node);

void blk_mq_tagset_busyidle_enable(struct udk_blk_mq_tag_set *tag_set, int enable);
int blk_queue_busyidle_event_unsubscribe(
	const struct blk_busyidle_event_node *event_node);
int blk_busyidle_event_unsubscribe(
	const struct blk_busyidle_event_node *event_node);
void mas_blk_add_rq_to_counted_list(
	struct blk_dev_lld *lld, struct udk_request *rq);
int hmfs_blk_busyidle_event_subscribe(struct hmsrv_io_ctx *ctx, char *name,
		int namelen, void *priv, void **res);
int hmfs_blk_busyidle_event_unsubscribe(void *pri);

#endif /* __MAS_BLK_BUSY_IDLE_NOTIFY__ */
