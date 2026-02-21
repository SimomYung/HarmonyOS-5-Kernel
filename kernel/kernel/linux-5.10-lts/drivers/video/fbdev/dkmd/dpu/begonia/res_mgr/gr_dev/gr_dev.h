/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef MEM_MGR_H
#define MEM_MGR_H

#include <linux/list.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/dma-buf.h>

#include "dkmd_res_mgr.h"

#define IOMMU_PROTECT (1 << 20)

struct work_input_node {
	struct list_head list_node;
	struct dma_buf *work_dmabuf;
};

struct dpu_gr_dev {
	struct platform_device *pdev;

	// iova map ctrl
	struct workqueue_struct *iova_map_workqueue;
	struct work_struct iova_map_worker;
	struct list_head input_list_head;
	struct mutex input_list_mutex;
	uint32_t input_list_size;

	struct mutex ctrl_mutex;
	bool is_iova_notify_init;
};

void map_iova_buffer_notify_init(void);
void dpu_res_register_gr_dev(struct list_head *resource_head);

#endif
