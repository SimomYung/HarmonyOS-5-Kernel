/*
 * drv_venc_thread.h
 *
 * This is for wireless low latency solution.
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
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

#ifndef __DRV_VENC_THREAD_H__
#define __DRV_VENC_THREAD_H__

#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>

typedef int32_t (*thread_process_fun)(void*);

struct venc_kthread {
	int32_t condition;
	spinlock_t lock;
	wait_queue_head_t wait_queue;
	struct task_struct *task;
	thread_process_fun handler;
	void *data;
};

#define VENC_SET_EVENT(lock, action) \
	do { \
		unsigned long flag;	\
		spin_lock_irqsave((&(lock)), flag); \
		(action); \
		spin_unlock_irqrestore((&(lock)), flag); \
	} while (0)

int32_t venc_drv_create_thread(struct venc_kthread *thread, const char *name);
void venc_drv_destroy_thread(struct venc_kthread *thread);

#endif