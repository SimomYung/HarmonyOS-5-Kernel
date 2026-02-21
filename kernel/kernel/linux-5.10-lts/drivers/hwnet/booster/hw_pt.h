/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This file is performance statistics for
 *              data transmission head file.
 * Author: lianchaofeng@huawei.com
 * Create: 2019-12-10
 */

#ifndef HW_PACKET_TRACKER_H
#define HW_PACKET_TRACKER_H

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/in6.h>
#include "netlink_handle.h"

#include <net/sock.h>
#include "../chr/wbc_hw_hook.h"
#define PROC_BITMASK_SETTED 0xa0
#define PROC_BITMASK_QUEUE_RECV 0xb0
#define PROC_BITMASK_APP_RECV 0xc0
#define PROC_BITMASK_SET_MASK 0xff
#define PROC_BITMASK_RNIC_UL_OUT (1 << 1)

#define INTERFACE_SIZE 2
#define QUEUE_REPORT_TIME (15 * 60 * HZ)
#define QUEUE_ERROR_REPORT_TIME (15 * 60 * HZ)
#define QUEUE_LARGE_DELAY_THRESHOLD 50
#define QUEUE_SLOW_DURATION_MIN_THRESHOLD 500
#define QUEUE_SLOW_DURATION_MAX_THRESHOLD (10 * 60 * 1000)
#define CONTINUE_LARGEDELAY_COUNT_THRESHOLD 10
#define CONTINUE_RECV_LARGEDELAY_COUNT_THRESHOLD 20
#define IP_ADDR_STR_MAX_LEN 48

struct sk_flow {
    u8 is_valid;
	u8 proto;
    u32 continue_slow_count;
    u64 continue_slow_first_time;
};

struct hw_kernel_delay_info {
	/* kernel send data to modem cost more time than threshold */
	u32 uplink_delay_count;
	u32 uplink_data_count;
	u32 uplink_time_cost_ave;

	u32 downlink_delay_cnt;
	u32 downlink_data_count;
	u32 downlink_time_cost_ave;

	/* downlink data stay too long in receive buffer */
	u32 downlink_out_wait_cnt;
	u32 uplink_abnormal_not_report_cnt;
	u32 downlink_abnormal_not_report_cnt;
};

struct hw_buffer_queue_info
{
    u64 app_recv_pkg_count;
    u64 queue_recv_pkg_count;
    u64 app_recv_large_delay_count;
    u64 queue_recv_large_delay_count;
    u64 app_recv_large_delay_total_delay;
    u64 queue_recv_large_delay_total_delay;
    u64 app_recv_total_delay;
    u64 queue_recv_total_delay;
    u32 app_longest_nurecv_delay;
    u32 app_recv_continuously_large_count;
    u32 app_recv_continuously_large_max_count;
};

struct hw_buffer_queue_info_lst
{
    struct hw_buffer_queue_info report_app_stat[CHR_MAX_REPORT_APP_COUNT];
};

struct tcp_zero_window_info {
    struct res_msg_head msg_head;
	u16 src_port;
	u16 dest_port;
    u32 ip_type;
    u32 uid;
	u32 interface;
	char src_ip[IP_ADDR_STR_MAX_LEN];
	char dest_ip[IP_ADDR_STR_MAX_LEN];
};

void init_stat(void);
void hw_pt_set_skb_stamp(struct sk_buff *skb);
void hw_pt_set_skb_long_stamp(struct sk_buff *skb);
void hw_pt_dev_uplink(struct sk_buff *skb, struct net_device *dev);
void hw_pt_l4_downlink_in(struct sock *skt, struct sk_buff *skb, int eaten);
void hw_pt_l4_downlink_out(struct sock *skt, struct sk_buff *skb);
void hw_pt_app_recv(struct sock *skt, struct sk_buff *skb);
void hw_pt_queue_recv(struct sock *skt, struct sk_buff *skb, int eaten);
msg_process* __init hw_pt_init(notify_event *notify);

#endif
