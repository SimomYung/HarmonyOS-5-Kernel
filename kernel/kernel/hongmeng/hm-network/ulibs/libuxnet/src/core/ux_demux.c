/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network demux
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 19:41:36 2019
 */

#include <stdio.h>
#include <errno.h>
#include <net/if_arp.h>

#include <libhmsync/raw_rwlock.h>

#include "ux_demux.h"

STATIC_DEFINE_RAW_RWLOCK(ux_demux_lock);
static DLIST_HEAD(ux_demux_list);

int ux_register_packet_demux(ux_packet_demux_t *demux)
{
	(void)raw_rwlock_wrlock(&ux_demux_lock);
	dlist_insert_tail(&ux_demux_list, &demux->head);
	(void)raw_rwlock_unlock(&ux_demux_lock);

	return 0;
}

void ux_unregister_packet_demux(ux_packet_demux_t *demux)
{
	(void)raw_rwlock_wrlock(&ux_demux_lock);
	dlist_delete(&demux->head);
	(void)raw_rwlock_unlock(&ux_demux_lock);
}

static ux_packet_demux_t *__ux_packet_demux_lookup(unsigned short type)
{
	ux_packet_demux_t *demux = NULL;

	dlist_for_each_entry(demux, &ux_demux_list, ux_packet_demux_t, head) {
		if (demux->type == type) {
			return demux;
		}
	}

	return NULL;
}

ux_packet_demux_t *ux_packet_demux_lookup(unsigned short type)
{
	ux_packet_demux_t *demux = NULL;

	(void)raw_rwlock_rdlock(&ux_demux_lock);
	demux = __ux_packet_demux_lookup(type);
	(void)raw_rwlock_unlock(&ux_demux_lock);

	return demux;
}
