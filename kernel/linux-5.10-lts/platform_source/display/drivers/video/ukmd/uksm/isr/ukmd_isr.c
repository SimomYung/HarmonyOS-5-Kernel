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
#include <linux/bitops.h>
#include "ukmd_log.h"
#include "ukmd_isr.h"

static void ukmd_isr_init_listener_list(struct ukmd_isr *isr_ctrl)
{
	int32_t i;

	for (i = 0; i < ISR_LISTENER_LIST_COUNT; ++i)
		INIT_LIST_HEAD(&isr_ctrl->isr_listener_list[i]);
}

static struct list_head* ukmd_isr_get_listener_list(struct ukmd_isr *isr_ctrl, uint32_t listen_bit)
{
	int index = get_count_order(listen_bit);
	if (index < 0 || index >= ISR_LISTENER_LIST_COUNT) {
		ukmd_pr_err("invalid index:%d", index);
		return NULL;
	}

	return &isr_ctrl->isr_listener_list[index];
}

static void ukmd_isr_request(struct ukmd_isr *isr_ctrl)
{
	int32_t ret;

	if (unlikely(!isr_ctrl)) {
		ukmd_pr_err("isr_ctrl is null ptr\n");
		return;
	}

	if (isr_ctrl->irq_no < 0) {
		ukmd_pr_warn("%s error irq_no: %d", isr_ctrl->irq_name, isr_ctrl->irq_no);
		return;
	}

	ret = request_irq((uint32_t)isr_ctrl->irq_no, isr_ctrl->isr_fnc, 0, isr_ctrl->irq_name, isr_ctrl);
	if (ret) {
		ukmd_pr_err("setup irq fail %s, isr_ctrl->irq_no: %d", isr_ctrl->irq_name, isr_ctrl->irq_no);
		return;
	}
	ukmd_pr_info("setup irq %s, isr_ctrl->irq_no: %d", isr_ctrl->irq_name, isr_ctrl->irq_no);
	/* default disable irq */
	disable_irq((uint32_t)isr_ctrl->irq_no);
}

static void ukmd_isr_release(struct ukmd_isr *isr_ctrl)
{
	if (atomic_read(&isr_ctrl->refcount) != 0)
		return;

	if (isr_ctrl->irq_no > 0)
		free_irq((uint32_t)isr_ctrl->irq_no, isr_ctrl);
}

static void ukmd_isr_enable(struct ukmd_isr *isr_ctrl)
{
	if (atomic_read(&isr_ctrl->refcount) == 0)
		enable_irq((uint32_t)isr_ctrl->irq_no);

	atomic_inc(&isr_ctrl->refcount);
}

static void ukmd_isr_disable(struct ukmd_isr *isr_ctrl)
{
	if (atomic_read(&isr_ctrl->refcount) == 0)
		return;

	if (atomic_dec_and_test(&isr_ctrl->refcount))
		disable_irq((uint32_t)isr_ctrl->irq_no);
}

static void ukmd_isr_enable_no_ref(struct ukmd_isr *isr_ctrl)
{
	if (atomic_read(&isr_ctrl->refcount) != 0)
		enable_irq((uint32_t)isr_ctrl->irq_no);
}

static void ukmd_isr_disable_no_ref(struct ukmd_isr *isr_ctrl)
{
	if (atomic_read(&isr_ctrl->refcount) != 0)
		disable_irq((uint32_t)isr_ctrl->irq_no);
}

static void ukmd_isr_handle_func(struct ukmd_isr *isr_ctrl, uint32_t handle_event)
{
	ukmd_pr_debug("irq_name:%s irq_no:%d refcount:%u handle_event:%u",
		isr_ctrl->irq_name, isr_ctrl->irq_no, atomic_read(&isr_ctrl->refcount), handle_event);
	switch (handle_event) {
	case UKMD_ISR_REQUEST:
		ukmd_isr_request(isr_ctrl);
		break;
	case UKMD_ISR_RELEASE:
		ukmd_isr_release(isr_ctrl);
		break;
	case UKMD_ISR_DISABLE:
		ukmd_isr_disable(isr_ctrl);
		break;
	case UKMD_ISR_ENABLE:
		ukmd_isr_enable(isr_ctrl);
		break;
	case UKMD_ISR_ENABLE_NO_REF:
		ukmd_isr_enable_no_ref(isr_ctrl);
		break;
	case UKMD_ISR_DISABLE_NO_REF:
		ukmd_isr_disable_no_ref(isr_ctrl);
		break;
	default:
		return;
	}
}

void ukmd_isr_setup(struct ukmd_isr *isr_ctrl)
{
	if (unlikely(!isr_ctrl)) {
		ukmd_pr_err("isr_ctrl is null ptr\n");
		return;
	}

	ukmd_isr_init_listener_list(isr_ctrl);
	isr_ctrl->handle_func = ukmd_isr_handle_func;
}

int32_t ukmd_isr_register_listener(struct ukmd_isr *isr_ctrl,
	struct notifier_block *nb, uint32_t listen_bit, void *listener_data)
{
	struct ukmd_isr_listener_node *listener_node = NULL;
	struct list_head *listener_list = NULL;

	if (!isr_ctrl) {
		ukmd_pr_err("isr_ctrl is null ptr");
		return -1;
	}

	listener_list = ukmd_isr_get_listener_list(isr_ctrl, listen_bit);
	if (!listener_list) {
		ukmd_pr_err("get listener_list fail, listen_bit=0x%x", listen_bit);
		return -1;
	}

	listener_node = kzalloc(sizeof(*listener_node), GFP_KERNEL);
	if (!listener_node) {
		ukmd_pr_err("alloc listener node fail, listen_bit=0x%x", listen_bit);
		return -ENOMEM;
	}

	listener_node->listen_bit = listen_bit;
	listener_node->listener_data.data = listener_data;

	list_add_tail(&listener_node->list_node, listener_list);

	return raw_notifier_chain_register(&listener_node->irq_nofitier, nb);
}

int32_t ukmd_isr_unregister_listener(struct ukmd_isr *isr_ctrl, struct notifier_block *nb, uint32_t listen_bit)
{
	struct list_head *listener_list = NULL;
	struct ukmd_isr_listener_node *listener_node = NULL;
	struct ukmd_isr_listener_node *_node_ = NULL;

	if (!isr_ctrl || !nb) {
		ukmd_pr_err("isr_ctrl or nb is null ptr");
		return -1;
	}

	listener_list = ukmd_isr_get_listener_list(isr_ctrl, listen_bit);
	if (!listener_list) {
		ukmd_pr_err("get listener_list fail, listen_bit=0x%x", listen_bit);
		return -1;
	}

	list_for_each_entry_safe(listener_node, _node_, listener_list, list_node) {
		if (listener_node->listen_bit != listen_bit)
			continue;
		if (listener_node->irq_nofitier.head == nb) {
			ukmd_pr_debug("listen bit 0x%x, del node\n", listen_bit);
			raw_notifier_chain_unregister(&listener_node->irq_nofitier, nb);

			list_del(&listener_node->list_node);
			kfree(listener_node);
			listener_node = NULL;
		}
	}

	return 0;
}

int32_t ukmd_isr_notify_listener(struct ukmd_isr *isr_ctrl, uint32_t listen_bit)
{
	struct list_head *listener_list = NULL;
	struct ukmd_isr_listener_node *listener_node = NULL;
	struct ukmd_isr_listener_node *_node_ = NULL;
	ktime_t notify_timestamp;

	if (!isr_ctrl) {
		ukmd_pr_err("isr_ctrl is null ptr");
		return -1;
	}

	/**
	 * @brief listen_bit MUST be single bit!!!
	 *
	 */
	listener_list = ukmd_isr_get_listener_list(isr_ctrl, listen_bit);
	if (!listener_list) {
		ukmd_pr_err("get listener_list fail, listen_bit=0x%x", listen_bit);
		return -1;
	}

	notify_timestamp = ktime_get();

	list_for_each_entry_safe(listener_node, _node_, listener_list, list_node) {
		if ((listener_node->listen_bit & listen_bit) == listen_bit) {
			listener_node->listener_data.notify_timestamp = notify_timestamp;
			raw_notifier_call_chain(&listener_node->irq_nofitier, listen_bit, &listener_node->listener_data);
		}
	}

	return 0;
}
