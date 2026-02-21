/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network pktbuf
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#ifndef __UX_PKTBUF_H_
#define __UX_PKTBUF_H_

#include <libhmsync/atomic.h>
#include <lib/dlist.h>

struct ux_conntrack;
typedef struct ux_pktbuf {
	/* next pktbuf in single linked list */
	struct dlist_node head;

	/* cloned orig pktbuf */
	struct ux_pktbuf *clone;

	/* data buffer */
	void *payload;
	/*  */
	void *data;
	void *tail;

	size_t tot_len;
	size_t len;

	struct ux_conntrack *ctrk;
	raw_atomic_int_t refcnt;
} ux_pktbuf_t;

ux_pktbuf_t *ux_pktbuf_alloc(size_t size);
ux_pktbuf_t *ux_pktbuf_clone(ux_pktbuf_t *pkt);
void ux_pktbuf_free(ux_pktbuf_t *pkt);
void ux_pktbuf_ref(ux_pktbuf_t *pkt);

static inline void *ux_pktbuf_pull(ux_pktbuf_t *pkt, unsigned int len)
{
	if (pkt->len < len) {
		return NULL;
	}

	pkt->len -= len;
	pkt->data = (void *)((char *)pkt->data + len);

	return pkt->data;
}

static inline void *ux_pktbuf_push(ux_pktbuf_t *pkt, unsigned int len)
{
	void *ptr = pkt->tail;

	if (pkt->len + len > pkt->tot_len || len > pkt->tot_len) {
		return NULL;
	}

	pkt->len += len;
	pkt->tail = (void *)((char *)pkt->tail + len);

	return ptr;
}

#endif
