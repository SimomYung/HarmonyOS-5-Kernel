/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_HTS_DEFS_H
#define HVGR_HTS_DEFS_H

#include <linux/types.h>
#include <linux/hrtimer.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include "hvgr_mem_api.h"
#include "hvgr_cq_data.h"

#define HTS_EVENT_MAX_NUM          1024
#define HTS_WAIT_TIMEOUT           5 /* 5s */


struct hvgr_ctx;
struct hvgr_hts_dev {
	struct hvgr_mem_area *area;
	struct hvgr_ctx *ctx;
	struct list_head hts_list_head;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	struct workqueue_struct *hts_wq;
	struct work_struct no_consumer_wq_work;
#else
	struct kthread_worker *hts_wq;
	struct kthread_work no_consumer_wq_work;
#endif
	spinlock_t hts_lock;
	struct hvgr_cq_ctx *hts_timeout_cq_ctx;
	atomic_t hts_timeout_flag;
};

#endif
