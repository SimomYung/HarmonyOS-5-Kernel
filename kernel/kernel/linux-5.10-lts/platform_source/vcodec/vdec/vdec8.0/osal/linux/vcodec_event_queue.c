/*
 * drv_venc_osal.c
 *
 * This is for venc drv.
 *
 * Copyright (c) 2009-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "vcodec_event_queue.h"
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/sched/clock.h>


int32_t vcodec_lock_create(spinlock_t **phlock)
{
	spinlock_t *plock = NULL;

	plock = vmalloc(sizeof(spinlock_t));
	if (!plock) {
		dprint(PRN_ERROR, "vmalloc failed");
		return -ENOMEM;
	}

	spin_lock_init(plock);
	*phlock = plock;

	return 0;
}

void vcodec_lock_destroy(spinlock_t *hlock)
{
	if (hlock)
		vfree((void *)hlock);
}

int32_t vcodec_init_event(vcodec_event *event)
{
	event->is_done = false;
	init_waitqueue_head(&(event->queue_head));
	return 0;
}

int32_t vcodec_give_event(vcodec_event *event)
{
	event->is_done = true;
	wake_up_all(&(event->queue_head));
	return 0;
}

