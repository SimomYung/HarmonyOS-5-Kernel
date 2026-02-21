/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network pktbuf
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#include <stdlib.h>

#include "ux_pktbuf.h"

ux_pktbuf_t *ux_pktbuf_alloc(size_t size)
{
	ux_pktbuf_t *pkt = NULL;

	if (size == 0) {
		return NULL;
	}

	pkt = malloc(sizeof(ux_pktbuf_t));
	if (pkt == NULL) {
		return NULL;
	}

	pkt->payload = malloc(size);
	if (pkt->payload == NULL) {
		free(pkt);
		return NULL;
	}

	dlist_init(&pkt->head);
	pkt->clone = NULL;
	pkt->tot_len = size;
	pkt->len = 0;
	pkt->data = pkt->payload;
	pkt->tail = pkt->payload;
	(void)raw_atomic_int_set(&pkt->refcnt, 1);

	return pkt;
}

ux_pktbuf_t *ux_pktbuf_clone(ux_pktbuf_t *pkt)
{
	ux_pktbuf_t *npkt = NULL;

	ux_pktbuf_ref(pkt);

	npkt = malloc(sizeof(ux_pktbuf_t));
	if (npkt == NULL) {
		ux_pktbuf_free(pkt);
		return NULL;
	}

	dlist_init(&npkt->head);
	npkt->clone = pkt;
	npkt->tot_len = pkt->tot_len;
	npkt->len = pkt->len;
	npkt->payload = pkt->payload;
	npkt->data = pkt->data;
	npkt->tail = pkt->tail;
	(void)raw_atomic_int_set(&npkt->refcnt, 1);

	return npkt;
}

static void __ux_pktbuf_free_data(ux_pktbuf_t *pkt)
{
	free(pkt->payload);
	pkt->payload = NULL;
}

static void __ux_pktbuf_free(ux_pktbuf_t *pkt)
{
	free(pkt);
}

void ux_pktbuf_free(ux_pktbuf_t *pkt)
{
	if (raw_atomic_int_dec(&pkt->refcnt) == 0) {
		if (pkt->clone != NULL) {
			ux_pktbuf_free(pkt->clone);
		} else {
			__ux_pktbuf_free_data(pkt);
		}
		__ux_pktbuf_free(pkt);
	}
}

void ux_pktbuf_ref(ux_pktbuf_t *pkt)
{
	(void)raw_atomic_int_inc(&pkt->refcnt);
}
