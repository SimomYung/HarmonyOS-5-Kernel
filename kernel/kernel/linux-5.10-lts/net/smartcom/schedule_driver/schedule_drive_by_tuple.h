/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#ifndef _SCHEDULE_DRIVE_BY_TUPLE_H
#define _SCHEDULE_DRIVE_BY_TUPLE_H

#include <linux/spinlock_types.h>
#include <linux/in6.h>

#include "../smartcom_utils.h"
#include "smartcom/smartcom_schedule_driver.h"

#define UID_EMPTY_APP 0
#define TUPLE_EMPTY_APP 0
#define TUPLE_FILL_APP 1
#define SMARTCOM_MAX_ACC_APP_UID 8
#define SMARTCOM_MAX_ACC_APP_TUPLE 8

struct flow_msg_tuple {
	int32_t			uid;
	__be16			l3proto; /* ipv4 or ipv6 */
	__u8			proto; /* udp or tcp */
	union {
		struct {
			uint32_t	ipv4_sip;
			uint32_t	ipv4_dip;
		};
#if IS_ENABLED(CONFIG_IPV6)
		struct {
			struct in6_addr	ipv6_sip;
			struct in6_addr	ipv6_dip;
		};
#endif
	};
	__be16			sport;
	__be16			dport;
};

struct smartcom_schedule_driver_acc_app_info_tuple {
	struct flow_msg_tuple  flow_msg; /* The tuple of accelerate Application */
	int32_t tuple;
	uint16_t  age;
	uint16_t  reverse;
	spinlock_t     tuple_lock;
};

struct smartcom_schedule_driver_acc_app_info_tuple_list {
	struct smartcom_schedule_driver_acc_app_info_tuple  flow_msg_list[SMARTCOM_MAX_ACC_APP_TUPLE];
	uint16_t  age;
	int32_t uid;
	spinlock_t     list_lock;
};

int smartcom_schedule_driver_start_acc_tuple(const void *pdata, uint16_t len);
int smartcom_schedule_driver_stop_acc_tuple(const void *pdata, uint16_t len);
void smartcom_schedule_driver_set_acc_state_tuple(struct sk_buff *skb);
bool tuple_is_equal(struct flow_msg_tuple *left, struct flow_msg_tuple *right);
bool tuple_copy(struct flow_msg_tuple *src, struct flow_msg_tuple *dst);

#endif /* _SCHEDULE_DRIVE_BY_TUPLE_H */