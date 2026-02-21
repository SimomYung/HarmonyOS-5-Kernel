/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: bkops core framework
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef MAS_ENHANCED_GC_CORE_H
#define MAS_ENHANCED_GC_CORE_H

#include <linux/workqueue.h>
#include <linux/blkdev.h>

enum enhanced_gc_operation {
	ENHANCED_GC_STOP = 0,
	ENHANCED_GC_START,
};

typedef int (bkops_enhanced_gc_start_stop_fn)(void *bkops_data, int start, u32 *target_size);
typedef int (bkops_enhanced_gc_status_query_fn)(void *bkops_data, u32 *status,
    u32 *device_target, u32 *now_free_cnt);
struct enhanced_ops {
    void *bkops_data;
	bkops_enhanced_gc_start_stop_fn *bkops_enhanced_gc_start_stop;
	bkops_enhanced_gc_status_query_fn *bkops_enhanced_gc_status_query;
};

int mas_enhanced_gc_enable(struct enhanced_ops *bkops);
#endif /* MAS_ENHANCED_GC_CORE_H */
