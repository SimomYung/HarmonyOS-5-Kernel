/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O multiplexing of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 15:14:11 2019
 */
#include <udk/log.h>
#include <udk/poll.h>
#include <udk/iolib.h>
#include <udk/device.h>
#include <udk/lib/string.h>

#include <devhost/poll.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#include <internal/poll.h>
#include <internal/device.h>

int udk_epoll_wakeup(struct udk_device *udkdev, uint32_t revents,
		     uint64_t key)
{
	UNUSED(udkdev, revents, key);
	return 0;
}

int udk_epoll_clean_revents(struct udk_device *udkdev, uint32_t revents,
			    uint64_t key)
{
	UNUSED(udkdev, revents, key);
	return 0;
}

struct poll_wevent_entry {
	struct udk_poll_wqueue *pwq;
	struct udk_poll_wevent *event;
	/* node of udk_poll_wqueue */
	struct dlist_node queue_node;
	/* node of udk_poll_wevent */
	struct dlist_node event_node;
};

struct udk_poll_wevent {
	void *target;
	int (*wakeup)(void *target, unsigned int revents);
	/* head of poll_wevent_entry */
	struct dlist_node head;
};

static void poll_wqueue_insert(struct udk_poll_wqueue *wq, struct poll_wevent_entry *entry)
{
	udk_mutex_lock(&wq->lock);
	dlist_insert_tail(&wq->head, &entry->queue_node);
	udk_mutex_unlock(&wq->lock);
}

static void poll_wqueue_delete(struct udk_poll_wqueue *wq, struct poll_wevent_entry *entry)
{
	udk_mutex_lock(&wq->lock);
	dlist_delete(&entry->queue_node);
	udk_mutex_unlock(&wq->lock);
}

void udk_poll_wqueue_init(struct udk_poll_wqueue *wq)
{
	if (wq != NULL) {
		udk_mutex_init(&(wq->lock));
		dlist_init(&(wq->head));
	}
}

int udk_poll_wqueue_add(struct udk_poll_wqueue *wq, struct udk_poll_wevent *event)
{
	struct poll_wevent_entry *entry = NULL;

	/* no need wait, return OK */
	if (event == NULL) {
		return 0;
	}
	if (wq == NULL) {
		return -EINVAL;
	}
	entry = (struct poll_wevent_entry *)udk_malloc(sizeof(struct poll_wevent_entry));
	if (entry == NULL) {
		udk_warn("malloc failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*entry);
	dlist_init(&(entry->queue_node));
	dlist_init(&(entry->event_node));
	entry->event = event;
	entry->pwq = wq;
	dlist_insert(&(event->head), &(entry->event_node));
	poll_wqueue_insert(wq, entry);

	return 0;
}

#define POLL_RECHECK	0x800U
void udk_poll_wqueue_wakeup(struct udk_poll_wqueue *wq, unsigned int revents)
{
	struct poll_wevent_entry *pos = NULL;
	struct udk_poll_wevent *event = NULL;
	unsigned int __revents = revents | POLL_RECHECK;
	int ret;

	if (wq == NULL) {
		return;
	}
	udk_mutex_lock(&(wq->lock));
	dlist_for_each_entry(pos, &(wq->head), struct poll_wevent_entry, queue_node) {
		event = pos->event;
		ret = event->wakeup(event->target, __revents);
		if (ret < 0) {
			udk_debug("poll wakeup failed, revent=%u, err=%s\n", __revents, hmstrerror(ret));
		}
	}
	udk_mutex_unlock(&(wq->lock));
}

static void __udk_poll_wevent_release(void *pinfo)
{
	struct udk_poll_wevent *event = NULL;
	struct poll_wevent_entry *pos = NULL;
	struct poll_wevent_entry *next = NULL;

	if (pinfo == NULL) {
		return;
	}
	event = (struct udk_poll_wevent *)pinfo;
	dlist_for_each_entry_safe(pos, next, &(event->head), struct poll_wevent_entry, event_node) {
		poll_wqueue_delete(pos->pwq, pos);
		dlist_delete(&pos->event_node);
		udk_free(pos);
	}
	udk_free(event);
}

int udk_poll_wevent_init(void *poll_info, struct udk_poll_wevent **out_event)
{
	struct udk_poll_wevent *event = NULL;
	struct devhost_poll_wqueues_info *pwq_info = NULL;
	struct devhost_poll_info *pinfo = (struct devhost_poll_info *)poll_info;

	if (unlikely((pinfo == NULL) ||
		     (pinfo->pwq_info == NULL) || (pinfo->target == NULL) ||
		     (pinfo->wakeup == NULL) || (out_event == NULL))) {
		udk_warn("udk_pollwait_init invalid\n");
		return -EINVAL;
	}
	*out_event = NULL;

	event = (struct udk_poll_wevent *)udk_malloc(sizeof(struct udk_poll_wevent));
	if (event == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*event);
	event->target = pinfo->target;
	event->wakeup = pinfo->wakeup;
	dlist_init(&(event->head));

	pwq_info = pinfo->pwq_info;
	pwq_info->pwq = (void *)event;
	pwq_info->release = __udk_poll_wevent_release;
	*out_event = event;

	return 0;
}

void udk_poll_wevent_deinit(void *poll_info, struct udk_poll_wevent **event)
{
	struct devhost_poll_wqueues_info *pwq_info = NULL;
	struct devhost_poll_info *pinfo = (struct devhost_poll_info *)poll_info;

	if (unlikely((pinfo == NULL) ||
		     (pinfo->pwq_info == NULL) || (event == NULL))) {
		udk_warn("udk_pollwait_deinit invalid\n");
		return;
	}

	pwq_info = pinfo->pwq_info;
	pwq_info->pwq = NULL;
	pwq_info->release = NULL;
	udk_free(*event);
	*event = NULL;
}
