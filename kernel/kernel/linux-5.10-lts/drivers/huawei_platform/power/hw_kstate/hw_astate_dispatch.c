/*
 * hw_astate_dispatch.c
 *
 * This file use to dispatch anco status
 *
 * Copyright (c) 2014-2020 Huawei Technologies Co., Ltd.
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
#include <huawei_platform/power/hw_astate_dispatch.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <securec.h>

DEFINE_MUTEX(anco_state_hook_lock);
static struct list_head anco_state_hooks;
static bool g_is_anco_exit = true;

bool is_anco_exit(void)
{
	return g_is_anco_exit;
}

void subscribe_anco_status(struct anco_status_subscriber *subscriber)
{
	mutex_lock(&anco_state_hook_lock);
	list_add_tail(&subscriber->list, &anco_state_hooks);
	mutex_unlock(&anco_state_hook_lock);
}

void unsubscribe_anco_status(struct anco_status_subscriber *subscriber)
{
	struct anco_status_subscriber *elem = NULL;
	struct anco_status_subscriber *n = NULL;
	mutex_lock(&anco_state_hook_lock);
	list_for_each_entry_safe(elem, n, &anco_state_hooks, list) {
		if (subscriber == elem) {
			list_del(&elem->list);
			break;
		}
	}
	mutex_unlock(&anco_state_hook_lock);
}

void dispatch_anco_status(int status)
{
	struct anco_status_subscriber *elem = NULL;
	g_is_anco_exit = (status == ANCO_EXIT) ? true: false;
	pr_info("dispatch anco status changed, status: %d", status);
	mutex_lock(&anco_state_hook_lock);
	list_for_each_entry(elem, &anco_state_hooks, list) {
		elem->hook(status);
	}
	mutex_unlock(&anco_state_hook_lock);
}

static int __init astate_init(void)
{
	INIT_LIST_HEAD(&anco_state_hooks);
	return 0;
}

module_init(astate_init);

