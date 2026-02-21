/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#ifndef _FI_UTILS_H
#define _FI_UTILS_H

#include <linux/netlink.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <linux/spinlock_types.h>
#include <linux/types.h>
#include "huawei_platform/log/hw_log.h"
#include "securec.h"

#undef HWLOG_TAG
#define HWLOG_TAG SMARTCOM_FI_KERNEL

HWLOG_REGIST();

#define FI_NL_MSG_TYPE_MASK 0x0100
#define FI_NL_MSG_TYPE_MASK_LEN 8

#define fi_logd(fmt, ...) do \
		hwlog_debug("%s "fmt"\n", __func__, ##__VA_ARGS__); \
	while (0)

#define fi_logi(fmt, ...) do \
		hwlog_info("%s "fmt"\n", __func__, ##__VA_ARGS__); \
	while (0)

#define fi_loge(fmt, ...) do \
		hwlog_err("%s "fmt"\n", __func__, ##__VA_ARGS__); \
	while (0)

struct sk_buff* fi_get_netlink_skb(int type, int length, char **data);
void fi_enqueue_netlink_skb(struct sk_buff *pskb);
int fi_netlink_thread(void* data);
void fi_empty_netlink_skb_queue(void);

#endif // _FI_UTILS_H
