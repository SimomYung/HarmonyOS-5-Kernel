/*
 * dsp_extend_misc.c
 *
 * dsp extend misc driver.
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#include "dsp_extend_misc.h"

#include <linux/errno.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include "audio_log.h"
#include "audio_common_msg.h"
#include "audio_pcm_dma_buf_layout.h"
#include "dsp_mmap.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "dsp_extend_misc"

static int dsp_extend_misc_open(struct inode *finode, struct file *fd)
{
	AUDIO_LOGI("open device\n");
	return 0;
}

static int dsp_extend_misc_release(struct inode *finode, struct file *fd)
{
	AUDIO_LOGI("close device\n");
	return 0;
}

bool is_mmap_size(unsigned long size)
{
	return (size == AAUDIO_MMAP_SIZE || size == AAUDIO_MMAP_VOIP_SIZE_16K || size == AAUDIO_MMAP_MAX_SIZE);
}

static int dsp_extend_check_para(struct file *file, struct vm_area_struct *vma)
{
	unsigned long size;
	unsigned long offset;

	if (!vma) {
		AUDIO_LOGI("input error: vma is NULL\n");
		return -1;
	}

	offset = vma->vm_pgoff << PAGE_SHIFT;
	size = ((unsigned long)vma->vm_end - (unsigned long)vma->vm_start);
	AUDIO_LOGI("dsp_extend_misc_mmap map size %lu offset %lu\n", size, offset);

	if (offset != 0 || !is_mmap_size(size)) {
		AUDIO_LOGE("size or offset is error \n");
		return -1;
	}

	return 0;
}

static int dsp_extend_misc_mmap(struct file *file, struct vm_area_struct *vma)
{
	if (dsp_extend_check_para(file, vma) != 0)
		return -1;

	// set offset to the index of mmap misc
	unsigned long offset = 0;
	return dsp_misc_mmap_common(file, vma, offset);
}

static const struct file_operations g_dsp_extend_misc_fops = {
	.owner = THIS_MODULE,
	.open = dsp_extend_misc_open,
	.release = dsp_extend_misc_release,
	.mmap = dsp_extend_misc_mmap,
};

static struct miscdevice g_dsp_extend_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dsp_extend_misc",
	.fops = &g_dsp_extend_misc_fops,
};

static int dsp_extend_misc_mmap_1(struct file *file, struct vm_area_struct *vma)
{
	if (dsp_extend_check_para(file, vma) != 0)
		return -1;

	// set offset to the index of mmap misc
	unsigned long offset = 1;
	return dsp_misc_mmap_common(file, vma, offset);
}

static const struct file_operations g_dsp_extend_misc_fops_1 = {
	.owner = THIS_MODULE,
	.open = dsp_extend_misc_open,
	.release = dsp_extend_misc_release,
	.mmap = dsp_extend_misc_mmap_1,
};

static struct miscdevice g_dsp_extend_misc_device_1 = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dsp_extend_misc_1",
	.fops = &g_dsp_extend_misc_fops_1,
};

static int dsp_extend_misc_mmap_2(struct file *file, struct vm_area_struct *vma)
{
	if (dsp_extend_check_para(file, vma) != 0)
		return -1;

	// set offset to the index of mmap misc
	unsigned long offset = 2;
	return dsp_misc_mmap_common(file, vma, offset);
}

static const struct file_operations g_dsp_extend_misc_fops_2 = {
	.owner = THIS_MODULE,
	.open = dsp_extend_misc_open,
	.release = dsp_extend_misc_release,
	.mmap = dsp_extend_misc_mmap_2,
};

static struct miscdevice g_dsp_extend_misc_device_2 = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dsp_extend_misc_2",
	.fops = &g_dsp_extend_misc_fops_2,
};

static int dsp_extend_misc_mmap_3(struct file *file, struct vm_area_struct *vma)
{
	if (dsp_extend_check_para(file, vma) != 0)
		return -1;

	// set offset to the index of mmap misc
	unsigned long offset = 3;
	return dsp_misc_mmap_common(file, vma, offset);
}

static const struct file_operations g_dsp_extend_misc_fops_3 = {
	.owner = THIS_MODULE,
	.open = dsp_extend_misc_open,
	.release = dsp_extend_misc_release,
	.mmap = dsp_extend_misc_mmap_3,
};

static struct miscdevice g_dsp_extend_misc_device_3 = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dsp_extend_misc_3",
	.fops = &g_dsp_extend_misc_fops_3,
};

static int dsp_extend_misc_mmap_4(struct file *file, struct vm_area_struct *vma)
{
	if (dsp_extend_check_para(file, vma) != 0)
		return -1;

	// set offset to the index of mmap misc
	unsigned long offset = 4;
	return dsp_misc_mmap_common(file, vma, offset);
}

static const struct file_operations g_dsp_extend_misc_fops_4 = {
	.owner = THIS_MODULE,
	.open = dsp_extend_misc_open,
	.release = dsp_extend_misc_release,
	.mmap = dsp_extend_misc_mmap_4,
};

static struct miscdevice g_dsp_extend_misc_device_4 = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dsp_extend_misc_4",
	.fops = &g_dsp_extend_misc_fops_4,
};

static int dsp_extend_misc_mmap_5(struct file *file, struct vm_area_struct *vma)
{
	if (dsp_extend_check_para(file, vma) != 0)
		return -1;

	// set offset to the index of mmap misc
	unsigned long offset = 5;
	return dsp_misc_mmap_common(file, vma, offset);
}

static const struct file_operations g_dsp_extend_misc_fops_5 = {
	.owner = THIS_MODULE,
	.open = dsp_extend_misc_open,
	.release = dsp_extend_misc_release,
	.mmap = dsp_extend_misc_mmap_5,
};

static struct miscdevice g_dsp_extend_misc_device_5 = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dsp_extend_misc_5",
	.fops = &g_dsp_extend_misc_fops_5,
};

static int dsp_extend_misc_mmap_6(struct file *file, struct vm_area_struct *vma)
{
	if (dsp_extend_check_para(file, vma) != 0)
		return -1;

	// set offset to the index of mmap misc
	unsigned long offset = 6;
	return dsp_misc_mmap_common(file, vma, offset);
}

static const struct file_operations g_dsp_extend_misc_fops_6 = {
	.owner = THIS_MODULE,
	.open = dsp_extend_misc_open,
	.release = dsp_extend_misc_release,
	.mmap = dsp_extend_misc_mmap_6,
};

static struct miscdevice g_dsp_extend_misc_device_6 = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dsp_extend_misc_6",
	.fops = &g_dsp_extend_misc_fops_6,
};

static int dsp_extend_misc_probe(struct platform_device *pdev)
{
	int32_t ret = 0;
	IN_FUNCTION;
	AUDIO_LOGI("dsp_extend_misc_probe enter (%s)", pdev->name);

	ret = misc_register(&g_dsp_extend_misc_device);
	if (ret != 0) {
		AUDIO_LOGE("misc register failed, ret %d", ret);
		return ret;
	}

	ret = misc_register(&g_dsp_extend_misc_device_1);
	if (ret != 0) {
		AUDIO_LOGE("misc register 1 failed, ret %d", ret);
		goto error_probe_1;
	}

	ret = misc_register(&g_dsp_extend_misc_device_2);
	if (ret != 0) {
		AUDIO_LOGE("misc register 2 failed, ret %d", ret);
		goto error_probe_2;
	}

	ret = misc_register(&g_dsp_extend_misc_device_3);
	if (ret != 0) {
		AUDIO_LOGE("misc register 3 failed, ret %d", ret);
		goto error_probe_3;
	}

	ret = misc_register(&g_dsp_extend_misc_device_4);
	if (ret != 0) {
		AUDIO_LOGE("misc register 4 failed, ret %d", ret);
		goto error_probe_4;
	}

	ret = misc_register(&g_dsp_extend_misc_device_5);
	if (ret != 0) {
		AUDIO_LOGE("misc register 5 failed, ret %d", ret);
		goto error_probe_5;
	}

	ret = misc_register(&g_dsp_extend_misc_device_6);
	if (ret != 0) {
		AUDIO_LOGE("misc register 6 failed, ret %d", ret);
		goto error_probe_6;
	}

	OUT_FUNCTION;
	return 0;

error_probe_6:
	(void)misc_deregister(&g_dsp_extend_misc_device_5);
error_probe_5:
	(void)misc_deregister(&g_dsp_extend_misc_device_4);
error_probe_4:
	(void)misc_deregister(&g_dsp_extend_misc_device_3);
error_probe_3:
	(void)misc_deregister(&g_dsp_extend_misc_device_2);
error_probe_2:
	(void)misc_deregister(&g_dsp_extend_misc_device_1);
error_probe_1:
	(void)misc_deregister(&g_dsp_extend_misc_device);
	return ret;
}

static int dsp_extend_misc_remove(struct platform_device *pdev)
{
	IN_FUNCTION;
	AUDIO_LOGI("dsp_extend_misc_remove enter");

	(void)misc_deregister(&g_dsp_extend_misc_device);
	(void)misc_deregister(&g_dsp_extend_misc_device_1);
	(void)misc_deregister(&g_dsp_extend_misc_device_2);
	(void)misc_deregister(&g_dsp_extend_misc_device_3);
	(void)misc_deregister(&g_dsp_extend_misc_device_4);
	(void)misc_deregister(&g_dsp_extend_misc_device_5);
	(void)misc_deregister(&g_dsp_extend_misc_device_6);

	OUT_FUNCTION;
	return 0;
}

static const struct of_device_id dsp_extend_match_table[] = {
	{
		.compatible = DTS_COMP_HIFIDSP_EXTEND_NAME,
		.data = NULL,
	},
	{}
};
MODULE_DEVICE_TABLE(of, dsp_extend_match_table);

static struct platform_driver dsp_extend_misc_driver = {
	.driver = {
		.name = "hifi_dsp_extend_misc",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(dsp_extend_match_table),
	},
	.probe = dsp_extend_misc_probe,
	.remove = dsp_extend_misc_remove,
};

static int __init dsp_extend_misc_init(void)
{
	return platform_driver_register(&dsp_extend_misc_driver);
}

static void __exit dsp_extend_misc_exit(void)
{
	platform_driver_unregister(&dsp_extend_misc_driver);
}

fs_initcall(dsp_extend_misc_init);
module_exit(dsp_extend_misc_exit);

MODULE_DESCRIPTION("dsp extend driver");
MODULE_LICENSE("GPL v2");
