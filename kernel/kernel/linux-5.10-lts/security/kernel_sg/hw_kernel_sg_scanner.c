// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_scanner.c for scanner init and work.
 * Create: 2022-07-23
 */

#include "hw_kernel_sg_scanner.h"

#define KSG_LOG_TAG "kernel_sg_scanner"

struct kernel_sg_scanner_node g_sg_scanner_node;

struct workqueue_struct *g_kernel_sg_scan_wq;

static DEFINE_MUTEX(sg_scanner_list_lock);

static int g_scanner_list_init = KSG_LIST_UNINIT;

int kernel_sg_scanner_init(void)
{
	g_kernel_sg_scan_wq = create_singlethread_workqueue("HW_KERNEL_SG_SCANNER");
	if (g_kernel_sg_scan_wq == NULL) {
		ksg_log_error(KSG_LOG_TAG, "kernel sg scanner wq error, no mem");
		return -ENOMEM;
	}

	if (g_scanner_list_init != KSG_LIST_UNINIT) {
		ksg_log_trace(KSG_LOG_TAG, "kernel sg scanner has init success at register");
		return KSG_SUCCESS;
	}
	INIT_LIST_HEAD(&g_sg_scanner_node.sg_scanner_list);
	g_scanner_list_init = KSG_LIST_INIT;
	ksg_log_trace(KSG_LOG_TAG, "kernel sg scanner init success");
	return KSG_SUCCESS;
}

static void kernel_sg_do_callback(struct work_struct *data)
{
	struct kernel_sg_scan_work *work_node = NULL;

	if (data == NULL) {
		ksg_log_error(KSG_LOG_TAG, "kernel sg do callbackfunc param invalid");
		return;
	}
	work_node = container_of(data, struct kernel_sg_scan_work, work);
	if (work_node == NULL) {
		ksg_log_error(KSG_LOG_TAG, "kernel sg work_node null");
		return;
	}

	if (work_node->callbackfunc != NULL) {
		work_node->callbackfunc();
		ksg_log_trace(KSG_LOG_TAG, "kernel sg do callbackfunc done");
	}
	kfree(work_node);
}

void kernel_sg_scanner(void)
{
	struct kernel_sg_scanner_node *tmp = NULL;

	if (g_scanner_list_init != KSG_LIST_INIT) {
		ksg_log_error(KSG_LOG_TAG, "scanner_list not init");
		return;
	}

	mutex_lock(&sg_scanner_list_lock);
	list_for_each_entry(tmp, &g_sg_scanner_node.sg_scanner_list, sg_scanner_list) {
		struct kernel_sg_scan_work *work_node = NULL;

		work_node = kzalloc(sizeof(struct kernel_sg_scan_work), GFP_KERNEL);
		if (work_node == NULL) {
			ksg_log_error(KSG_LOG_TAG, "work_node init failed");
			break;
		}

		work_node->callbackfunc = tmp->callbackfunc;

		INIT_WORK(&work_node->work, kernel_sg_do_callback);
		queue_work(g_kernel_sg_scan_wq, &work_node->work);
	}
	mutex_unlock(&sg_scanner_list_lock);
}

void kernel_sg_scanner_exit(void)
{
	struct kernel_sg_scanner_node *pos = NULL;
	struct kernel_sg_scanner_node *next = NULL;

	if (g_kernel_sg_scan_wq != NULL) {
		destroy_workqueue(g_kernel_sg_scan_wq);
		g_kernel_sg_scan_wq = NULL;
	}

	if (g_scanner_list_init == KSG_LIST_INIT) {
		mutex_lock(&sg_scanner_list_lock);
		list_for_each_entry_safe(pos, next, &g_sg_scanner_node.sg_scanner_list, sg_scanner_list) {
			list_del(&pos->sg_scanner_list);
			kfree(pos);
			pos = NULL;
		}
		mutex_unlock(&sg_scanner_list_lock);
	}

	ksg_log_trace(KSG_LOG_TAG, "scanner list cleanup success");
}

unsigned int register_report_security_callback(const sg_callback callback)
{
	struct kernel_sg_scanner_node *listnode = NULL;
	struct kernel_sg_scanner_node *tmp = NULL;
	int repeat_count = 0;
	int list_size = 0;
	if (callback == NULL) {
		ksg_log_error(KSG_LOG_TAG, "callback null");
		return KSG_ERRCODE;
	}

	if (g_scanner_list_init == KSG_LIST_UNINIT) {
		INIT_LIST_HEAD(&g_sg_scanner_node.sg_scanner_list);
		g_scanner_list_init = KSG_LIST_INIT;
		ksg_log_trace(KSG_LOG_TAG, "scanner init success at register");
	}

	listnode = kzalloc(sizeof(struct kernel_sg_scanner_node), GFP_KERNEL);
	if (listnode == NULL) {
		ksg_log_error(KSG_LOG_TAG, "listnode init failed");
		return KSG_ERRCODE;
	}
	listnode->callbackfunc = callback;

	mutex_lock(&sg_scanner_list_lock);
	list_for_each_entry(tmp, &g_sg_scanner_node.sg_scanner_list, sg_scanner_list) {
		list_size++;
		if (listnode->callbackfunc == tmp->callbackfunc) {
			repeat_count++;
			ksg_log_debug(KSG_LOG_TAG, "callback had in list, no need add again");
			break;
		}
	}

	if ((list_size >= KSG_SCAN_LIST_MAX_SIZE) && (repeat_count == 0)) {
		ksg_log_error(KSG_LOG_TAG, "scaner too many %d, can not add any", list_size);
		kfree(listnode);
		mutex_unlock(&sg_scanner_list_lock);
		return KSG_ERRCODE;
	}

	if (repeat_count == 0) {
		list_add_tail(&listnode->sg_scanner_list, &g_sg_scanner_node.sg_scanner_list);
		ksg_log_trace(KSG_LOG_TAG, "scanner list add node success");
	} else {
		kfree(listnode);
	}
	mutex_unlock(&sg_scanner_list_lock);

	return KSG_SUCCESS;
}

