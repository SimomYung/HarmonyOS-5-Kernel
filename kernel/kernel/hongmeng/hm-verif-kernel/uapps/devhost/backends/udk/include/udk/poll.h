/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define udk poll headfile
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 16:28:40 2019
 */

#ifndef __UDK_POLL_H__
#define __UDK_POLL_H__

#include <udk/mutex.h>
#include <udk/device.h>
#include <udk/lib/dlist.h>

int udk_epoll_wakeup(struct udk_device *udkdev, uint32_t revents,
		     uint64_t key);

int udk_epoll_clean_revents(struct udk_device *udkdev, uint32_t revents,
			    uint64_t key);

struct udk_poll_wqueue {
	struct udk_mutex lock;
	struct dlist_node head;
};

struct udk_poll_wevent;

void udk_poll_wqueue_init(struct udk_poll_wqueue *queue);
int udk_poll_wqueue_add(struct udk_poll_wqueue *queue, struct udk_poll_wevent *event);
void udk_poll_wqueue_wakeup(struct udk_poll_wqueue *queue, unsigned int revents);

#endif /* __DEVHOST_UDK_POLL_H__ */
