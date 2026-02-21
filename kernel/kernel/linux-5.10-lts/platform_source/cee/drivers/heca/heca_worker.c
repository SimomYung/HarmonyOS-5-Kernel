/*
 * heca_worker.c
 *
 * hisilicon efficinecy control algorithm common worker.
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "heca_worker.h"

#include <uapi/linux/sched/types.h>

static struct task_struct *g_heca_worker_thread;
static struct kthread_worker g_heca_worker;

void heca_worker_init(void)
{
	int ret;
	struct sched_param param = { .sched_priority = MAX_USER_RT_PRIO / 2 };

	kthread_init_worker(&g_heca_worker);
	g_heca_worker_thread = kthread_create(kthread_worker_fn, &g_heca_worker, "heca_worker");
	if (IS_ERR(g_heca_worker_thread)) {
		pr_err("heca worker: failed to create heca worker thread\n");
		return;
	}

	ret = sched_setscheduler_nocheck(g_heca_worker_thread, SCHED_FIFO, &param);
	if (ret) {
		pr_err("heca worker: failed to set heca worker SCHED_FIFO\n");
		return;
	}

	wake_up_process(g_heca_worker_thread);
}

bool heca_worker_queue_work(struct kthread_work *work)
{
	return kthread_queue_work(&g_heca_worker, work);
}