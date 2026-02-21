/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Functions for queue management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 16 10:45:12 2019
 */
#include "queue_priv.h"
#include <lib/utils.h>
#include <udk/log.h>
#include <udk/device.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/bitops/generic.h>
#include <libhwsecurec/securec.h>

#include "pbuf_priv.h"

static struct pkt_buf *tx_dequeue(struct udk_netdev_queue *queue)
{
	struct pkt_buf *pbuf = NULL;
	struct udk_mutex *q_lock = &queue->queue_lock;

	udk_mutex_lock(q_lock);
	if (queue->state != UDK_QUEUE_STATE_ON) {
		udk_mutex_unlock(q_lock);
		return NULL;
	}

	if (dlist_empty(&queue->pbuf_list)) {
		if (queue->pbuf_num != 0) {
			udk_error("pbuf_num do not match with pbuf_list. pbuf_num=%u\n",
				  queue->pbuf_num);
			queue->pbuf_num = 0;
		}
		udk_mutex_unlock(q_lock);
		return NULL;
	}
	pbuf = dlist_first_entry(&queue->pbuf_list, struct pkt_buf, list);
	dlist_delete(&pbuf->list);
	queue->pbuf_num--;

	udk_mutex_unlock(q_lock);

	return pbuf;
}

int tx_enqueue(struct pkt_buf *pbuf,
	       struct udk_netdev_queue *queue)
{
	struct udk_net_device *dev = NULL;
	struct udk_mutex *q_lock = NULL;

	if (pbuf == NULL || pbuf->dev == NULL || queue == NULL) {
		return E_HM_INVAL;
	}
	dev = pbuf->dev;
	q_lock = &queue->queue_lock;

	udk_mutex_lock(q_lock);
	if (queue->state != UDK_QUEUE_STATE_ON) {
		dev->dev_stats.deact_drop++;
		udk_mutex_unlock(q_lock);
		return E_HM_INVAL;
	}

	if (queue->pbuf_num < queue->pbuf_max) {
		queue->pbuf_num++;
		pbuf_enqueue(&queue->pbuf_list, pbuf);
	} else {
		pbuf->dev->dev_stats.full_drop++;
		udk_mutex_unlock(q_lock);
		return E_HM_INVAL;
	}
	udk_mutex_unlock(q_lock);

	return E_HM_OK;
}

void tx_queue_xmit(struct udk_netdev_queue *queue)
{
	int ret;
	unsigned int cnt = 0;
	struct pkt_buf *pbuf = NULL;
	struct udk_mutex *xmit_lock = NULL;

	if (queue == NULL) {
		return;
	}
	xmit_lock = &queue->xmit_lock;
	ret = udk_mutex_trylock(xmit_lock);
	if (ret != 0) {
		return;
	}

	while (((pbuf = tx_dequeue(queue)) != NULL) &&
	      (cnt < queue->pbuf_cnt_once)) {
		pbuf_start_xmit(pbuf);
		cnt++;
	}
	udk_mutex_unlock(xmit_lock);
}
