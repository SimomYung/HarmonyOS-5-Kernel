/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Devhost file_info kernel module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 09:48:47 2023
 */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/printk.h>
#include <linux/fs.h>

#include "file_info_api.h"
#include "file_info.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("file_info kernel module");
MODULE_VERSION("0.01");

static int fill_dma_buf_info(void *file, struct dma_buf_info *file_info)
{
	int ret = dmaheap_debug_dma_buf_info((struct file *)file, file_info);
	return ret;
}

static int fill_fence_info(void *file, unsigned int offset, unsigned int max_fences,
			   struct dma_fence_info *file_info, struct fences_info *fences_array)
{
	int ret = memcheck_debug_fence_info((struct file *)file, offset, max_fences, file_info, fences_array);
	return ret;
}

static int fill_ion_info(void *file, struct ion_info *file_info)
{
	int ret = memcheck_debug_ion_info((struct file *)file, file_info);
	return ret;
}

static int fill_mem_info(struct devhost_mem_info *mem_info)
{
	int ret = liblinux_fill_mem_info((struct liblinux_mem_info *)mem_info);
	return ret;
}

static int fill_gpu_info(uint32_t pid, unsigned long *gpu_info)
{
	liblinux_get_gpu_mem_usage(pid, gpu_info);
	return 1;
}

static int signal_lowmem_debug(short oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	liblinux_lowmem_debug(oom_score_adj, mem_total, is_emergency);
	return 0;
}

static struct devhost_file_info_ops g_file_info_ops = {
	.fill_dma_buf_info = fill_dma_buf_info,
	.fill_fence_info = fill_fence_info,
	.fill_ion_info = fill_ion_info,
	.fill_mem_info = fill_mem_info,
	.signal_lowmem_debug = signal_lowmem_debug,
	.fill_gpu_info = fill_gpu_info,
};

static int __init devhost_file_info_init(void)
{
	int ret;

	/* register ops */
	ret = devhost_file_info_ops_init(&g_file_info_ops);
	if (ret < 0) {
		pr_err("devhost_file_info_ops init failed, err=%d\n", ret);
	}
	return ret;
}

module_init(devhost_file_info_init);
