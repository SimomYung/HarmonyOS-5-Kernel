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

#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/iommu/mm_iommu.h>
#include <linux/delay.h>
#include "gr_dev.h"
#include "res_mgr.h"
#include "dkmd_log.h"
#include "dksm_dmd.h"
#define IOVA_ALGINED_BOUNDARY 0x100000000UL // 4GB
static struct platform_device *g_rm_device;

static void *gr_dev_init(struct dpu_res_data *rm_data)
{
	struct dpu_res *rm = NULL;
	struct dpu_gr_dev *gr_dev = kzalloc(sizeof(struct dpu_gr_dev), GFP_KERNEL);
	if (!gr_dev)
		return NULL;

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
	kfree(gr_dev);
	gr_dev = NULL;
}

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
    else if ((*iova / IOVA_ALGINED_BOUNDARY) != ((*iova + buf_size) / IOVA_ALGINED_BOUNDARY))
		dsm_client_record(dsm_lcd_client, "iommu map not aligned 4G");

	dsm_client_notify(dsm_lcd_client, DSM_LCD_MDSS_IOMMU_ERROR_NO);
}

static struct dma_buf *gr_dev_map_dmabuf_by_sharefd(
	uint64_t *iova, int32_t fd, uint32_t size, uint32_t is_protect_layer)
{
	unsigned long buf_size = 0;
	struct dma_buf *buf = dma_buf_get(fd);
	int ebit_used_flag = 0;

	if (IS_ERR_OR_NULL(buf)) {
		dpu_pr_err("get dma buf fail, ret = %ld sharefd = %d", PTR_ERR(buf), fd);
		return NULL;
	}

	/*
	  CPro DRM layer: kernel_iommu_map_dmabuf para ebit_used_flag is IOMMU_PROTECT(use ebit).
	  non CPro or CPro non DRM layer: kernel_iommu_map_dmabuf para ebit_used_flag is 0(not use ebit).
	  dpu_is_support_ebit:return is support ebit in different pltform。
	*/
	if ((is_protect_layer == 1) && (dpu_is_support_ebit() == 1))
		ebit_used_flag = IOMMU_PROTECT;

	*iova = kernel_iommu_map_dmabuf(dpu_res_get_device(), buf, ebit_used_flag, &buf_size);
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
#if defined (CONFIG_HUAWEI_DSM) || defined (CONFIG_HUAWEI_OHOS_DSM)
		gr_dev_map_dmabuf_dsm(iova, buf_size, size);
#endif
		return NULL;
	}

	dma_buf_put(buf);
	return buf;
}

static int32_t gr_dev_map_iova_buffer(void *data, void __user *argp)
{
	struct dpu_gr_dev *gr_dev = (struct dpu_gr_dev *)data;
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

	dmabuf =
		gr_dev_map_dmabuf_by_sharefd(&buf_data.iova, buf_data.share_fd, (uint32_t)buf_data.size, buf_data.is_protect_layer);
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
