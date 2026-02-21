/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MEM_QUICK_REPORT_H
#define HVGR_MEM_QUICK_REPORT_H

#include <dfx/hiview_hisysevent.h>
#include <securec.h>
#include <linux/ratelimit.h>
#include "hvgr_defs.h"
#include "hvgr_log_api.h"

#define HVGR_MEM_POOL_REPORT_PAGE_THRESHOLD (((totalram_pages() / 2) * 8) / 10)
#define MAX_MSG_LEN 512UL
#define KERNEL_MEMORY_DOMAIN "KERNEL_VENDOR"
#define GPU_LEAK_STR "LOWMEM"
#define GPU_LEAK_REASON "GPU_OVER_LIMIT"

void report_gpu_quick_leak_event(unsigned long used_pages, struct hvgr_ctx *ctx);

#endif