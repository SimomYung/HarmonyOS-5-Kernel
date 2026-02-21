/*
 * hw_astate_dispatch.h
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

#ifndef _HW_ASTATE_DISPATCH_H
#define _HW_ASTATE_DISPATCH_H
#include <linux/list.h>

#define SUBSCRIBER_NAME_LEN_MAX 20
#define REQUEST_ANCO_RUNNING 0
#define KEEP_ANCO_RUNNING 1
#define CANCEL_ANCO_RUNNING 2
#define ANCO_EXIT 3

struct anco_status_subscriber {
	struct list_head list;
	char name[SUBSCRIBER_NAME_LEN_MAX];
	void (*hook)(int status);
};
void subscribe_anco_status(struct anco_status_subscriber *subscriber);
void unsubscribe_anco_status(struct anco_status_subscriber *subscriber);
void dispatch_anco_status(int status);
bool is_anco_exit(void);
#endif