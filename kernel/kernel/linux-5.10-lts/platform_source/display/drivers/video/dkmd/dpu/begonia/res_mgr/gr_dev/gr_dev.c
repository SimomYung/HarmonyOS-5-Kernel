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

#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/iommu/mm_iommu.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/notifier.h>
#include "ukmd_listener.h"
#include "gr_dev.h"
#include "res_mgr.h"
#include "dkmd_log.h"
#include "dksm_dmd.h"

#define IOVA_ASYNC_MAP_WORK_MAX_NUM 50
#define IOVA_NOTIFY_INIT_TRYLOCK_WAITTIME 1000
#define IOVA_NOTIFY_INIT_TRYLOCK_TIMES 100
#define IOVA_ALGINED_BOUNDARY 0x100000000UL // 4GB

static struct platform_device *g_rm_device;
static struct dpu_gr_dev *g_gr_dev;

// work function for MapIovaAsync: run iova map in sub-thread; called when buffer was allocated.
static void gr_dev_map_iova_buffer_notify_work(struct work_struct *work)
{
	struct dpu_gr_dev *work_dev = container_of(work, struct dpu_gr_dev, iova_map_worker);
	struct dma_buf *dmabuf = NULL;
	unsigned long buf_size = 0;
	uint64_t iova = 0;
	struct work_input_node *input_node = NULL;

	if (unlikely(!work_dev)) {
		dpu_pr_err("gr_dev not inited");
		return;
	}

	while (true) {
		mutex_lock(&work_dev->input_list_mutex);
		if (work_dev->input_list_size == 0) {
			mutex_unlock(&work_dev->input_list_mutex);
			break;
		}
		input_node = list_entry(work_dev->input_list_head.next, struct work_input_node, list_node);
		if (unlikely(!input_node)) {
			dpu_pr_err("input is null");
			mutex_unlock(&work_dev->input_list_mutex);
			return;
		}

		dmabuf = input_node->work_dmabuf;
		list_del(&input_node->list_node);
		kfree(input_node);
		work_dev->input_list_size--;
		mutex_unlock(&work_dev->input_list_mutex);
		if (IS_ERR_OR_NULL(dmabuf)) {
			dpu_pr_err("get dma buf fail, ret = %ld", PTR_ERR((dmabuf)));
			continue;
		}

		mutex_lock(&work_dev->ctrl_mutex);
		iova = kernel_iommu_map_dmabuf(dpu_res_get_device(), dmabuf, 0, &buf_size);
		if ((iova == 0) || (buf_size == 0))
			dpu_pr_err("iommu_map error: iova(0x%llx), size(0x%llx)", iova, buf_size);
		mutex_unlock(&work_dev->ctrl_mutex);

		dma_buf_put(dmabuf);
	}
	return;
}

#ifdef CONFIG_DKMD_DPU_OHOS
static int gr_dev_map_iova_buffer_notify(struct notifier_block *self, unsigned long action, void *data)
{
	struct work_input_node *input_node = NULL;
	struct dma_buf *work_dmabuf = (struct dma_buf *)data;

	if (IS_ERR_OR_NULL(work_dmabuf)) {
		dpu_pr_err("input dmabuf is null");
		return notifier_from_errno(-1);
	}

	if (likely(action == DMA_HEAP_NOTIFY_EVENT_ALLOC && g_gr_dev && g_gr_dev->iova_map_workqueue)) {
		input_node = kzalloc(sizeof(struct work_input_node), GFP_KERNEL);
		if (unlikely(!input_node)) {
			dpu_pr_err("alloc work_input_node fail");
			return notifier_from_errno(-1);
		}

		mutex_lock(&g_gr_dev->input_list_mutex);
		if (unlikely(g_gr_dev->input_list_size > IOVA_ASYNC_MAP_WORK_MAX_NUM)) {
			dpu_pr_warn("too many work to do: %u", g_gr_dev->input_list_size);
			kfree(input_node);
			mutex_unlock(&g_gr_dev->input_list_mutex);
			return notifier_from_errno(-1);
		}

		get_file(work_dmabuf->file); // dma_buf_put() in iova_map_worker
		input_node->work_dmabuf = work_dmabuf;
		list_add_tail(&input_node->list_node, &g_gr_dev->input_list_head);
		g_gr_dev->input_list_size++;
		mutex_unlock(&g_gr_dev->input_list_mutex);
		queue_work(g_gr_dev->iova_map_workqueue, &g_gr_dev->iova_map_worker);
		return notifier_from_errno(0);
	}

	return notifier_from_errno(-1);
}

static struct notifier_block map_iova_buffer_notifier = {
	.notifier_call = gr_dev_map_iova_buffer_notify,
};

void map_iova_buffer_notify_init(void)
{
	uint32_t try_times = 0;

	if (unlikely(!g_gr_dev)) {
		dpu_pr_err("gr_dev is null");
		return;
	}

	mutex_lock(&g_gr_dev->ctrl_mutex);
	while (!g_gr_dev->is_iova_notify_init) {
		if (system_heap_evt_register_notify(&map_iova_buffer_notifier, DMA_HEAP_FLAGS_USAGE_ID_DSS) == 0) {
			dpu_pr_info("map_iova_buffer_notify init success");
			g_gr_dev->is_iova_notify_init = true;
			break;
		}

		usleep_range(IOVA_NOTIFY_INIT_TRYLOCK_WAITTIME, IOVA_NOTIFY_INIT_TRYLOCK_WAITTIME + 1);
		if (++try_times > IOVA_NOTIFY_INIT_TRYLOCK_TIMES) {
			dpu_pr_err("map_iova_buffer_notify init fail!");
			break;
		}
	}
	mutex_unlock(&g_gr_dev->ctrl_mutex);
	return;
}
void map_iova_buffer_notify_deinit(void)
{
	if (system_heap_evt_unregister_notify(&map_iova_buffer_notifier, DMA_HEAP_FLAGS_USAGE_ID_DSS) == 0)
		dpu_pr_info("map_iova_buffer_notify deinit success");

	if (g_gr_dev)
		g_gr_dev->is_iova_notify_init = false;
}
#else
void map_iova_buffer_notify_init(void)
{
	// only enable for OHOS
}
void map_iova_buffer_notify_deinit(void)
{
	// only enable for OHOS
}
#endif

static void map_iova_resource_init(struct dpu_gr_dev *gr_dev)
{
	if (!gr_dev) {
		dpu_pr_err("gr_dev is null");
		return;
	}

	dpu_pr_info("map_iova_resource_init +");

	gr_dev->iova_map_workqueue =
		alloc_ordered_workqueue("iova_map_workqueue", WQ_HIGHPRI | WQ_MEM_RECLAIM); // need high priority
	if (!gr_dev->iova_map_workqueue) {
		dpu_pr_err("create iova_map_workqueue failed");
		return;
	}
	INIT_WORK(&gr_dev->iova_map_worker, gr_dev_map_iova_buffer_notify_work);

	INIT_LIST_HEAD(&gr_dev->input_list_head);
	gr_dev->input_list_size = 0;
	mutex_init(&gr_dev->input_list_mutex);
	mutex_init(&gr_dev->ctrl_mutex);
	gr_dev->is_iova_notify_init = false;
	dpu_pr_info("map_iova_resource_init -");
}

static void map_iova_resource_deinit(struct dpu_gr_dev *gr_dev)
{
	if (!gr_dev) {
		dpu_pr_err("gr_dev is null");
		return;
	}

	dpu_pr_info("map_iova_resource_deinit +");
	map_iova_buffer_notify_deinit(); // stop callback first

	if (gr_dev->iova_map_workqueue) {
		destroy_workqueue(gr_dev->iova_map_workqueue);
		gr_dev->iova_map_workqueue = NULL;
	}

	dpu_pr_info("map_iova_resource_deinit -");
}

static void *gr_dev_init(struct dpu_res_data *rm_data)
{
	struct dpu_res *rm = NULL;
	struct dpu_gr_dev *gr_dev = kzalloc(sizeof(struct dpu_gr_dev), GFP_KERNEL);
	if (!gr_dev)
		return NULL;

	map_iova_resource_init(gr_dev);
	g_gr_dev = gr_dev;

	rm = container_of(rm_data, struct dpu_res, data);

	g_rm_device = rm->pdev;

	if (dma_set_mask_and_coherent(&rm->pdev->dev, DMA_BIT_MASK(64)) != 0)
		dpu_pr_err("dma set mask and coherent failed\n");

	return gr_dev;
}

static void gr_dev_deinit(void *data)
{
	struct dpu_gr_dev *gr_dev = NULL;

	if (!data)
		return;

	gr_dev = (struct dpu_gr_dev *)data;
	map_iova_resource_deinit(gr_dev);
	kfree(gr_dev);
	gr_dev = NULL;
	g_gr_dev = NULL;
}

#if defined (CONFIG_HUAWEI_DSM) || defined (CONFIG_HUAWEI_OHOS_DSM)
static void gr_dev_map_dmabuf_dsm(uint64_t *iova, unsigned long buf_size, uint32_t size)
{
	uint32_t cnt = 0;

	while((dsm_client_ocuppy(dsm_lcd_client) != 0) && (cnt < DSM_OCCUPY_RETRY_TIMES)) {
		dpu_pr_warn("dsm_client_ocuppy failed, retry %d times", ++cnt);
		usleep_range(500, 600);
	}

	if (cnt == DSM_OCCUPY_RETRY_TIMES) {
		dpu_pr_warn("dsm_client_ocuppy failed");
		return;
	}

	if (*iova == 0)
		dsm_client_record(dsm_lcd_client, "iommu map fail, iova null");
	else if (buf_size < size)
		dsm_client_record(dsm_lcd_client, "iommu map fail, iova_size(0x%llx) smaller than size(0x%x)", buf_size, size);

	dsm_client_notify(dsm_lcd_client, DSM_LCD_MDSS_IOMMU_ERROR_NO);
}
#endif

static struct dma_buf *gr_dev_map_dmabuf_by_sharefd(
	void *data, uint64_t *iova, int32_t fd, uint32_t size, uint32_t is_protect_layer)
{
	unsigned long buf_size = 0;
	struct dma_buf *buf = dma_buf_get(fd);
	int ebit_used_flag = 0;
	struct dpu_gr_dev * gr_dev = (struct dpu_gr_dev *)data;

	if (IS_ERR_OR_NULL(buf)) {
		dpu_pr_err("get dma buf fail, ret = %ld sharefd = %d", PTR_ERR(buf), fd);
		return NULL;
	}

	if (unlikely(!gr_dev)) {
		dpu_pr_err("gr_dev is null");
		return NULL;
	}

	/*
	  740 DRM layer: kernel_iommu_map_dmabuf para ebit_used_flag is IOMMU_PROTECT(use ebit).
	  non 740 or 740 non DRM layer: kernel_iommu_map_dmabuf para ebit_used_flag is 0(not use ebit).
	  dpu_is_support_ebit:return is support ebit in different pltform。
	*/
	if ((is_protect_layer == 1) && (dpu_is_support_ebit() == 1))
		ebit_used_flag = IOMMU_PROTECT;

	mutex_lock(&gr_dev->ctrl_mutex);
	*iova = kernel_iommu_map_dmabuf(dpu_res_get_device(), buf, ebit_used_flag, &buf_size);
	mutex_unlock(&gr_dev->ctrl_mutex);
	if ((*iova == 0) || (buf_size < size)) {
		dpu_pr_err("get iova_size(0x%llx) smaller than size(0x%x)", buf_size, size);
#if defined (CONFIG_HUAWEI_DSM) || defined (CONFIG_HUAWEI_OHOS_DSM)
		gr_dev_map_dmabuf_dsm(iova, buf_size, size);
#endif
		if (*iova != 0) {
			(void)kernel_iommu_unmap_dmabuf(dpu_res_get_device(), buf, *iova);
			*iova = 0;
		}
		dma_buf_put(buf);
		return NULL;
	}

	if ((*iova / IOVA_ALGINED_BOUNDARY) != ((*iova + buf_size) / IOVA_ALGINED_BOUNDARY)) {
		dpu_pr_err("iova addr is not aligned by 4G");
		return NULL;
	}

	dpu_pr_info("iova addr:%lx, request iova_size:0x%x, really iova_size:0x%llx", *iova, size, buf_size);

	dma_buf_put(buf);
	return buf;
}

static int32_t gr_dev_map_iova_buffer(void *data, void __user *argp)
{
	struct res_dma_buf buf_data = {0};
	struct dma_buf *dmabuf;

	if (!argp) {
		dpu_pr_err("argp is nullptr");
		return -1;
	}

	if (copy_from_user(&buf_data, argp, sizeof(buf_data))) {
		dpu_pr_err("iova_info copy_from_user fail");
		return -EINVAL;
	}

	if (unlikely(buf_data.size == 0)) {
		dpu_pr_err("size is 0!");
		return -EINVAL;
	}

	dmabuf = gr_dev_map_dmabuf_by_sharefd(
		data, &buf_data.iova, buf_data.share_fd, (uint32_t)buf_data.size, buf_data.is_protect_layer);
	if (!dmabuf) {
		dpu_pr_err("dma buf map share_fd(%d) failed", buf_data.share_fd);
		return -EFAULT;
	}

	if (copy_to_user(argp, &buf_data, sizeof(buf_data)) != 0) {
		dpu_pr_err("copy_to_user failed");
		(void)kernel_iommu_unmap_dmabuf(dpu_res_get_device(), dmabuf, buf_data.iova);
		return -EFAULT;
	}

	dpu_pr_debug("share_fd = %d map size = %llu",
		buf_data.share_fd, buf_data.size);

	return 0;
}

static int32_t gr_dev_unmap_iova_buffer(void *data, const void __user *argp)
{
	void_unused(data);
	void_unused(argp);

	// unmap will be called on buf release.

	return 0;
}

static int32_t gr_dev_get_vscreen_info(void __user *argp)
{
	struct screen_info info = {0};

	if (!argp) {
		dpu_pr_err("argp is nullptr");
		return -1;
	}
	dpu_config_get_screen_info(&info.xres, &info.yres);
	dpu_pr_info("get panel_id 0 screen xres = %u, yres = %u", info.xres, info.yres);

	if (copy_to_user(argp, &info, sizeof(info)) != 0) {
		dpu_pr_debug("copy to user failed!");
		return -EFAULT;
	}

	return 0;
}

static int32_t gr_dev_get_dpu_version(void __user *argp)
{
	uint64_t value = dpu_config_get_version_value();

	if (!argp) {
		dpu_pr_debug("NULL Pointer!\n");
		return -EINVAL;
	}

	if (copy_to_user(argp, &value, sizeof(value)) != 0) {
		dpu_pr_debug("copy to user failed!");
		return -EFAULT;
	}

	return 0;
}

static uint32_t gr_dev_get_product_type(void __user *argp)
{
	uint32_t value = dpu_config_get_product_type();

	if (!argp) {
		dpu_pr_err("argp is nullptr");
		return -EINVAL;
	}

	if (copy_to_user(argp, &value, sizeof(value)) != 0) {
		dpu_pr_err("copy to user failed!");
		return -EFAULT;
	}

	return 0;
}

static long gr_dev_ioctl(struct dpu_res_resouce_node *res_node, uint32_t cmd, void __user *argp)
{
	if (!res_node || !res_node->data || !argp) {
		dpu_pr_err("res_node or node data or argp is NULL");
		return -EINVAL;
	}

	switch (cmd) {
	case RES_MAP_IOVA:
		return gr_dev_map_iova_buffer(res_node->data, argp);
	case RES_UNMAP_IOVA:
		return gr_dev_unmap_iova_buffer(res_node->data, argp);
	case RES_GET_VSCREEN_INFO:
		return gr_dev_get_vscreen_info(argp);
	case RES_GET_DISP_VERSION:
		return gr_dev_get_dpu_version(argp);
	case RES_GET_PRODUCT_TYPE:
		return gr_dev_get_product_type(argp);
	default:
		dpu_pr_debug("mem mgr unsupport cmd, need processed by other module");
		return 1;
	}

	return 0;
}

void dpu_res_register_gr_dev(struct list_head *resource_head)
{
	struct dpu_res_resouce_node *gr_dev_node = NULL;
	if (!resource_head)
		return;

	gr_dev_node = kzalloc(sizeof(struct dpu_res_resouce_node), GFP_KERNEL);
	if (!gr_dev_node)
		return;

	gr_dev_node->res_type = RES_GR_DEV;
	atomic_set(&gr_dev_node->res_ref_cnt, 0);

	gr_dev_node->init = gr_dev_init;
	gr_dev_node->deinit = gr_dev_deinit;
	gr_dev_node->ioctl = gr_dev_ioctl;

	list_add_tail(&gr_dev_node->list_node, resource_head);
	dpu_pr_debug("dpu_res_register_gr_dev success!");
}
