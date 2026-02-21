/*
 * heca_worker.h
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
#ifndef _HECA_WORKER_H_
#define _HECA_WORKER_H_

#include <linux/kthread.h>

void heca_worker_init(void);

bool heca_worker_queue_work(struct kthread_work *work);

#endif