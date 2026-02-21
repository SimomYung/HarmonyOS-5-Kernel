/*
 *  ISP driver, qos.c
 *
 * Copyright (c) 2023 ISP Technologies CO., Ltd.
 *
 */

#include <linux/err.h>
#include <linux/rpmsg.h>
#include <securec.h>
#include <rpmsg_internal.h>
#include "hisp_internel.h"
#include <linux/kthread.h>
#include <linux/wait.h>

#define ISP_CONNECT_ADDR    2048

struct hisp_msg_info_list_s {
	struct isp_msg_content msg;
	struct list_head list;
};

struct hisp_msg_service {
	unsigned int src;
	unsigned int dst;
	size_t max_len;
	unsigned int msg_task_status;
	struct rpmsg_endpoint *ept;
	wait_queue_head_t msg_task_wait;
	struct task_struct *msg_task;
	struct mutex hisp_msg_mutex;
	struct hisp_msg_info_list_s msg_node;
};

static DEFINE_MUTEX(g_hisp_msg_service_lock);
struct hisp_msg_service *g_hisp_msg_service;

/**
 * @brief hisp_msg_service_send - kernel rpmsg to isp
 *
 * @param data is msg content: struct isp_msg_content
 * @param len is msg content size
 *
 * @retval 0 send success, other send fail
*/
int hisp_msg_service_send(void *data, unsigned int len)
{
	struct hisp_msg_service *service = NULL;
	int ret = -EINVAL;
	int msg_type = ISPMSG_TYPE_OTHER;

	if (hisp_msg_type_check(data, len, &msg_type))
		return -EINVAL;

	if (msg_type != ISPMSG_TYPE_KERNL) {
		pr_err("hisp_msg is not kernel type\n");
		return -EINVAL;
	}

	mutex_lock(&g_hisp_msg_service_lock);
	if (g_hisp_msg_service == NULL) {
		pr_err("g_hisp_msg_service is NULL\n");
		goto ERROR;
	}
	service = g_hisp_msg_service;
	if ((data == NULL) || (len > service->max_len)) {
		pr_err("wrong param len.0x%x\n", len);
		goto ERROR;
	}

	if (service->ept) {
		ret = rpmsg_send(service->ept, data, (int)len);
		if (ret < 0)
			pr_err("%s: rpmsg_send fail ret is %d", __func__, ret);
	}

	mutex_unlock(&g_hisp_msg_service_lock);
	return ret;
ERROR:
	mutex_unlock(&g_hisp_msg_service_lock);
	return -EINVAL;
}

int __attribute__((weak)) hisp_msg_from_isp_req_handle(struct isp_msg_content *msg)
{
	(void)msg;
	return 0;
}

void __attribute__((weak)) hisp_msg_handle_init(void)
{
	return;
}

void __attribute__((weak)) hisp_msg_handle_deinit(void)
{
	return;
}

/**
 * @brief hisp_msg_type_check - rpmsg type check
 *
 * @param data is msg content: struct isp_msg_content
 * @param len is msg content size
 * @param type is msg type for output: 0 normal type, other kernel type
 *
 * @retval 0 check done, others check error
*/
int __attribute__((weak)) hisp_msg_type_check(void *data, unsigned int len, int *type)
{
	(void)data;
	(void)len;
	*type = 0;
	return 0;
}

/**
 * @brief hisp_msg_service_recv - recv one rpmsg from isp
 *
 * @param service is msg serivce form g_hisp_msg_service
 * @param data is msg content: struct isp_msg_content
 * @param len is msg content size
 *
 * @retval 0 add node state
*/
static int hisp_msg_recv_single(struct hisp_msg_service *service, void *data, unsigned int len)
{
	struct hisp_msg_info_list_s *node = NULL;
	struct isp_msg_content *msg;
	int ret;

	if (len > sizeof(struct isp_msg_content)) {
		pr_err("[%s] error len.0x%x\n", __func__, len);
		return -1;
	}

	msg = (struct isp_msg_content *)data;
	if (msg == NULL) {
		pr_err("[%s] msg is NULL\n", __func__);
		return -1;
	}

	node =
	    kzalloc(sizeof(struct hisp_msg_info_list_s), GFP_KERNEL);
	if (node == NULL) {
		pr_err("node malloc fail\n");
		return -EINVAL;
	}
	ret = memcpy_s(&node->msg, sizeof(struct isp_msg_content),
		       msg, sizeof(struct isp_msg_content));
	if (ret) {
		pr_err("memcpy data to msg failed: %d\n", ret);
		goto MEM_ERROR;
	}

	mutex_lock(&service->hisp_msg_mutex);
	list_add_tail(&node->list, &service->msg_node.list);
	mutex_unlock(&service->hisp_msg_mutex);

	return 0;

MEM_ERROR:
	kfree(node);
	return ret;
}
/**
 * @brief hisp_msg_service_recv - recv rpmsg from isp
 *
 * @param data is msg content: struct isp_msg_content
 * @param len is msg content size
 * @param src is addr of isp_ept
 *
 * @retval 0 ept->callback work, other kernel work
*/
int hisp_msg_service_recv(void *data, unsigned int len, unsigned int src)
{
	struct hisp_msg_service *service = NULL;
	int ret = -1;
	int msg_type = ISPMSG_TYPE_OTHER;

	if (hisp_msg_type_check(data, len, &msg_type))
		return -EINVAL;
	if (msg_type == ISPMSG_TYPE_OTHER)
		return ISPMSG_TYPE_OTHER; // normal msg

	mutex_lock(&g_hisp_msg_service_lock);
	if (g_hisp_msg_service == NULL) {
		goto ERROR;
	}
	service = g_hisp_msg_service;
	if ((data == NULL) || (len > service->max_len)) {
		pr_err("wrong param len.0x%x\n", len);
		goto ERROR;
	}
	if (src != service->dst) {
		pr_err("wrong param src.0x%x\n", src);
		goto ERROR;
	}

	ret = hisp_msg_recv_single(service, data, len);
	if (ret != 0) {
		pr_err("hisp_msg_recv_single failed\n");
		goto ERROR;
	}
	service->msg_task_status = 1;
	wake_up_process(service->msg_task);

ERROR:
	mutex_unlock(&g_hisp_msg_service_lock);
	return ISPMSG_TYPE_KERNL; // other msg
}

static struct hisp_msg_info_list_s *get_msg_node_from_list(
	struct hisp_msg_service *service)
{
	struct hisp_msg_info_list_s *node = NULL;
	struct hisp_msg_info_list_s *node_temp = NULL;

	mutex_lock(&service->hisp_msg_mutex);
	list_for_each_entry_safe(node, node_temp, &service->msg_node.list, list) {
		list_del(&node->list);
		mutex_unlock(&service->hisp_msg_mutex);
		return node;
	}
	mutex_unlock(&service->hisp_msg_mutex);

	return NULL;
}

static int hisp_recv_msg_work()
{
	struct hisp_msg_service *service = g_hisp_msg_service;
	struct hisp_msg_info_list_s *node = NULL;
	int ret = 0;

	if (service == NULL) {
		pr_err("g_hisp_msg_service is NULL\n");
		return -EINVAL;
	}

	while (!kthread_should_stop()) {
		ret = wait_event_interruptible(service->msg_task_wait,
				(service->msg_task_status == 1) ||
				kthread_should_stop());
		if (kthread_should_stop())
			break;
		if (ret != 0) {
			pr_err("wait event failed\n");
			continue;
		}
		service->msg_task_status = 0;

		while (1) {
			node = get_msg_node_from_list(service);
			if (node != NULL) {
				ret = hisp_msg_from_isp_req_handle(&node->msg);
				if (ret != 0)
					pr_err("isp msg.0x%x handle fail\n", node->msg.info.api_name);
				kfree(node);
			} else
				break;
		}
	}

	return 0;
}


void hisp_msg_service_init(struct rpmsg_endpoint *ept)
{
	struct hisp_msg_service *service;
	mutex_lock(&g_hisp_msg_service_lock);
	if (g_hisp_msg_service != NULL) {
		pr_err("g_hisp_msg_service already creat\n");
		goto ERROR;
	}
	service = kzalloc(sizeof(struct hisp_msg_service), GFP_KERNEL);
	if (service == NULL) {
		pr_err("hisp_rpmsg_service is NULL\n");
		goto ERROR;
	}

	service->src = ept->addr;
	service->dst = ISP_CONNECT_ADDR;
	service->ept = ept;
	service->max_len = ISP_MSG_MAX_PAYLOAD - sizeof(struct rpmsg_hdr);
	init_waitqueue_head(&service->msg_task_wait);
	service->msg_task_status = 0;
	service->msg_task = kthread_create(hisp_recv_msg_work,
				(void *)service, "recv_isp_msg");
	if (IS_ERR(service->msg_task)) {
		pr_err("[%s] Failed : kthread_create.%ld\n",
			__func__, PTR_ERR(service->msg_task));
		kfree(service);
		goto ERROR;
	}

	INIT_LIST_HEAD(&service->msg_node.list);
	mutex_init(&service->hisp_msg_mutex);

	g_hisp_msg_service = service;

	hisp_msg_handle_init();

	pr_info("hisp msg_service creat\n");

ERROR:
	mutex_unlock(&g_hisp_msg_service_lock);
}

void hisp_msg_service_exit(void)
{
	struct hisp_msg_service *service = g_hisp_msg_service;
	struct hisp_msg_info_list_s *node = NULL;
	struct hisp_msg_info_list_s *node_temp = NULL;

	mutex_lock(&g_hisp_msg_service_lock);

	hisp_msg_handle_deinit();

	if (service == NULL) {
		pr_err("service is NULL\n");
		goto Exit;
	}

	if (service->msg_task != NULL) {
		kthread_stop(service->msg_task);
		service->msg_task_status = 0;
		service->msg_task = NULL;
	}

	list_for_each_entry_safe(node, node_temp, &service->msg_node.list, list) {
		list_del(&node->list);
		kfree(node);
	}

	service->ept = NULL;
	mutex_destroy(&service->hisp_msg_mutex);

	kfree(service);
	g_hisp_msg_service = NULL;
Exit:
	pr_info("hisp msg_service destroy\n");
	mutex_unlock(&g_hisp_msg_service_lock);
	return;
}
