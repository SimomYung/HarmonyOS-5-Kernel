/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of queue operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#ifndef __UDK_QUEUE_PRIV_H__
#define __UDK_QUEUE_PRIV_H__

#include <udk/net/netdev.h>

int tx_enqueue(struct pkt_buf *pbuf,
	       struct udk_netdev_queue *queue);
void tx_queue_xmit(struct udk_netdev_queue *queue);

#endif /* __UDK_QUEUE_PRIV_H__ */
