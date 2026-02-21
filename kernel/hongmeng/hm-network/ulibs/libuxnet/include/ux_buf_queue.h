/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for buf queue
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 19:55:26 2019
 */

#ifndef _UX_BUF_QUEUE_H_
#define _UX_BUF_QUEUE_H_

#include <lib/dlist.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread_cond.h>

#define UX_RCVBUF_LEN		0x7fffffff

typedef struct ux_buf_head {
	/* the head member must be first. */
	struct dlist_node head;
	struct raw_mutex lock;
	__u32 qlen;
	size_t len;
} ux_buf_head_t;

typedef struct ux_buf {
	/* the node member must be first. */
	struct dlist_node node;
	size_t data_len; /* length of data in receive buffer */
	size_t start;
	char data[];  /* receive buffer */
} ux_buf_t;

static inline void ux_buf_queue_init(ux_buf_head_t *buf_queue)
{
	dlist_init(&buf_queue->head);
	raw_mutex_init(&buf_queue->lock);
	buf_queue->qlen = 0;
	buf_queue->len = 0;
}

static inline void ux_buf_queue_lock(ux_buf_head_t *buf_queue)
{
	raw_mutex_lock(&buf_queue->lock);
}

static inline void ux_buf_queue_unlock(ux_buf_head_t *buf_queue)
{
	raw_mutex_unlock(&buf_queue->lock);
}

static inline void ux_buf_queue_insert_tail(ux_buf_head_t *buf_queue, ux_buf_t *ux_buf)
{
	dlist_insert_tail(&buf_queue->head, &ux_buf->node);
	buf_queue->qlen++;
	buf_queue->len += ux_buf->data_len;
}

static inline ux_buf_t *ux_buf_queue_dqueue_first(ux_buf_head_t *buf_queue)
{
	ux_buf_t *ux_buf = NULL;
	if (dlist_empty(&buf_queue->head)) {
		return NULL;
	}

	ux_buf = dlist_first_entry(&buf_queue->head, ux_buf_t, node);
	dlist_delete(&ux_buf->node);
	buf_queue->qlen--;
	buf_queue->len -= ux_buf->data_len;
	return ux_buf;
}

static inline ux_buf_t *ux_buf_queue_first(const ux_buf_head_t *buf_queue)
{
	ux_buf_t *ux_buf = NULL;

	if (dlist_empty(&buf_queue->head)) {
		return NULL;
	}

	ux_buf = dlist_first_entry(&buf_queue->head, ux_buf_t, node);
	return ux_buf;
}

/* return next entry. If buf is the last entry, return NULL. */
static inline ux_buf_t *ux_buf_queue_next(const ux_buf_head_t *buf_queue, const ux_buf_t *buf)
{
	/*
	 * To avoid returning the head node,
	 * we need to determine if this entry is the last entry.
	 */
	if (buf == dlist_last_entry(&buf_queue->head, ux_buf_t, node)) {
		return NULL;
	}

	return dlist_next_entry(buf, ux_buf_t, node);
}

static inline __u32 ux_buf_queue_len(const ux_buf_head_t *buf_queue)
{
	return buf_queue->qlen;
}

static inline size_t ux_buf_len(const ux_buf_head_t *buf_queue)
{
	return buf_queue->len;
}

static inline void ux_buf_len_desc(ux_buf_head_t *buf_queue, size_t len)
{
	buf_queue->len -= len;
}

static inline void ux_buf_len_reset(ux_buf_head_t *buf_queue)
{
	buf_queue->len = 0;
}

#endif
