// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_module.c for kernel sg module init and deinit
 * Create: 2022-07-23
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include "hw_kernel_sg_utils.h"
#include "hw_kernel_sg_proc.h"
#include "hw_kernel_sg_scanner.h"
#include "hw_kernel_sg_uploader.h"

#define KSG_LOG_TAG "kernel_sg_module"

struct kernel_sg_module_work {
	struct workqueue_struct *kernel_sg_wq;
	struct work_struct kernel_sg_work;
};

static struct kernel_sg_module_work g_kernel_sg_work_data;

static void kernel_sg_work_init(struct work_struct *data)
{
	int result;
	(void)data;
	ksg_log_trace(KSG_LOG_TAG, "kernel sg work init");

	do {
		/* init kernel sg proc */
		result = kernel_sg_proc_init();
		if (result != KSG_SUCCESS) {
			ksg_log_error(KSG_LOG_TAG, "kernel sg proc init failed");
			break;
		}

		/* init kernel sg scanner */
		result = kernel_sg_scanner_init();
		if (result != KSG_SUCCESS) {
			ksg_log_error(KSG_LOG_TAG, "kernel sg scanner init failed");
			break;
		}

		/* init kernel sg uploader */
		result = kernel_sg_uploader_init();
		if (result != KSG_SUCCESS) {
			ksg_log_error(KSG_LOG_TAG, "kernel sg uploader init failed");
			break;
		}
	} while (0);

	if (result != KSG_SUCCESS) {
		if (g_kernel_sg_work_data.kernel_sg_wq != NULL) {
			destroy_workqueue(g_kernel_sg_work_data.kernel_sg_wq);
			g_kernel_sg_work_data.kernel_sg_wq = NULL;
		}

		kernel_sg_proc_exit();
		kernel_sg_scanner_exit();
		kernel_sg_uploader_exit();
		ksg_log_error(KSG_LOG_TAG, "kernel sg module init failed");
		return;
	}

	ksg_log_trace(KSG_LOG_TAG, "kernel sg module init success");
}

static int __init kernel_sg_module_init(void)
{
	g_kernel_sg_work_data.kernel_sg_wq =
		create_singlethread_workqueue("HW_KERNEL_SG_MODULE");

	if (g_kernel_sg_work_data.kernel_sg_wq == NULL) {
		ksg_log_error(KSG_LOG_TAG, "kernel sg module wq error, no mem");
		return -ENOMEM;
	}

	INIT_WORK(&(g_kernel_sg_work_data.kernel_sg_work), kernel_sg_work_init);
	queue_work(g_kernel_sg_work_data.kernel_sg_wq, &(g_kernel_sg_work_data.kernel_sg_work));

	return KSG_SUCCESS;
}

static void __exit kernel_sg_module_exit(void)
{
	if (g_kernel_sg_work_data.kernel_sg_wq != NULL) {
		destroy_workqueue(g_kernel_sg_work_data.kernel_sg_wq);
		g_kernel_sg_work_data.kernel_sg_wq = NULL;
	}

	kernel_sg_proc_exit();
	kernel_sg_scanner_exit();
	kernel_sg_uploader_exit();
	ksg_log_trace(KSG_LOG_TAG, "kernel sg module exit success");

	return;
}

module_init(kernel_sg_module_init);
module_exit(kernel_sg_module_exit);

MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
MODULE_DESCRIPTION("Huawei kernel sg");
