/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_quick_report.h"

void report_gpu_quick_leak_event(unsigned long used_pages, struct hvgr_ctx *ctx)
{
	char msg[MAX_MSG_LEN] = { 0 };
	char pid_str[16] = { 0 };
	struct hiview_hisysevent *hievent = NULL;
	int ret = 0;

	if (likely(used_pages < HVGR_MEM_POOL_REPORT_PAGE_THRESHOLD)) {
		return;
	}

	static DEFINE_RATELIMIT_STATE(ratelimit_gpu_quick_leak_report, 30 * 60 * HZ, 1); // 30 mins
	if (!__ratelimit(&ratelimit_gpu_quick_leak_report))
		return;

	dev_err(ctx->gdev->dev,
		"stringid: %s, pid:%d, process_name:%s, group_name:%s, used_pages:%lu\n",
		GPU_LEAK_REASON, (int)ctx->tgid, ctx->process_name, ctx->group_name, used_pages);
	ret = snprintf_s(msg, MAX_MSG_LEN, MAX_MSG_LEN - 1,
		"pid:%d, process_name:%s, group_name:%s, used_pages:%lukB",
		(int)ctx->tgid, ctx->process_name,
		ctx->group_name, used_pages << (PAGE_SHIFT - 10)); // 1kB = 1024B
	if (ret < 0) {
		dev_err(ctx->gdev->dev, "snprintf_s failed for msg\n");
		return;
	}
	ret = snprintf_s(pid_str, sizeof(pid_str), sizeof(pid_str) - 1, "%d", (int)ctx->tgid);
	if (ret < 0) {
		dev_err(ctx->gdev->dev, "snprintf_s failed for pid_str\n");
		return;
	}
	hievent = hisysevent_create(KERNEL_MEMORY_DOMAIN, GPU_LEAK_STR, FAULT);
	if (hievent == NULL) {
		dev_err(ctx->gdev->dev, "create hisysevent fail, stringid: %s\n", GPU_LEAK_REASON);
		return;
	}

	hisysevent_put_string(hievent, "REASON", GPU_LEAK_REASON);
	hisysevent_put_string(hievent, "PID", pid_str);
	hisysevent_put_string(hievent, "PROCESS_NAME", ctx->process_name);
	hisysevent_put_string(hievent, "MSG", msg);
	if (hisysevent_write(hievent) < 0) {
		dev_err(ctx->gdev->dev, "send hisysevent fail, stringid: %s\n", GPU_LEAK_REASON);
		hisysevent_destroy(&hievent);
		return;
	}
	dev_err(ctx->gdev->dev, "send hisysevent success\n");
	hisysevent_destroy(&hievent);
}