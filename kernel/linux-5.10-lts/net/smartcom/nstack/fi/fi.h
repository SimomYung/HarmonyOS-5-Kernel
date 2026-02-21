/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#ifndef _FI_H
#define _FI_H

#include <linux/if.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/spinlock_types.h>
#include <linux/types.h>
#include "fi_flow.h"
#include "../../smartcom_utils.h"

#define FI_LAUNCH_APP_MAX		8
#define FI_TIMER_INTERVAL		2 /* 500ms timer */
#define FI_NL_SKB_QUEUE_MAXLEN	128
#define FI_DEV_NUM			SMARTCOM_NET_MAX_NUM

#define FI_MIN_DELTA_US 5000u
#define FI_MIN_BURST_DURATION 2000u
#define FI_MIN_BURST_SIZE (10 * 1024)
#define FI_RTT_SHIFT 3

#define TCPHDR_OFFSET_FIN 0
#define TCPHDR_OFFSET_SYN 1
#define TCPHDR_OFFSET_RST 2
#define TCPHDR_OFFSET_PSH 3
#define TCPHDR_OFFSET_ACK 4
#define TCPHDR_OFFSET_URG 5
#define TCPHDR_OFFSET_ECE 6
#define TCPHDR_OFFSET_CWR 7

#define FI_MIN_HTTP_LEN  100
#define FI_HTTP_RSP_CODE_MAX_LEN 8
#define FI_HTTP_VERSION_STR "HTTP/1.1 "
#define FI_HTTP_RESPONSE_OK 200
#define FI_HTTP_RESPONSE_NO_CONTENT 204
#define FI_HTTP_RESPONSE_PARTIAL_CONTENT 206
#define FI_HTTP_RESPONSE_MOVED 301
#define FI_HTTP_RESPONSE_REDIRECT 302 // client may retry on the flow
#define FI_HTTP_RESPONSE_OTHER_LOCATION 303
#define FI_HTTP_RESPONSE_USE_GATEWAY 305
#define FI_HTTP_RESPONSE_BAD_REQUEST 400
#define FI_HTTP_RESPONSE_FORBIDDEN 403 // client may retry on the flow
#define FI_HTTP_RESPONSE_NOT_FOUND 404
#define FI_HTTP_RESPONSE_SERVER_INTERNAL_ERROR 500
#define F_HTTP_RESPONSE_GATEWAY_ERROR 502

#define FI_DECIMAL_BASE 10

#define FI_SKB_MAX_FRAG_LIST_NUM 64

#define FI_VER "V4.0 20211222"

typedef enum {
	FI_MPFLOW_SERVICE,
	FI_MPROUTE_SERVICE,
}fi_service_type;

typedef enum {
	FI_SET_NL_PID_CMD,
	FI_COLLECT_START_CMD,
	FI_COLLECT_STATUS_UPDATE_CMD,
	FI_COLLECT_INFO_UPDATE_CMD,
	FI_STOP_FLOW_PKT_REPORT_CMD,
	CMD_NUM,
} fi_cfg_type;

typedef enum {
	FI_PKT_MSG_RPT = 1,
	FI_PERIODIC_MSG_RPT,
	FI_QOS_MSG_RPT,
	FI_IFACE_MSG_RPT,
	FI_MPROUTE_KD_RPT,
} fi_msg_type;

struct fi_app_info_table {
	struct fi_app_info_node	app[FI_LAUNCH_APP_MAX];
	atomic_t			free_cnt;
};

struct fi_ctx {
	struct mutex		nf_mutex;
	bool				nf_exist;
	struct timer_list	tm;
	bool				running;
	bool	iface_qos_rpt_enable;
	struct sock *nlfd;
	uint32_t nl_pid;
	uint32_t mproute_pid;
	struct sk_buff_head skb_queue;
	struct semaphore netlink_sync_sema;
	struct task_struct *netlink_task;
	uint8_t mpflow_app_num;
	uint8_t mproute_app_num;
	bool is_tm_start;
};

struct fi_iface_msg {
	char dev[IFNAMSIZ];
	uint32_t rcv_bytes;
};

struct fi_cfg_head {
	unsigned short		type;
	unsigned short		len;
};

extern struct fi_ctx g_fi_context;
extern void fi_cfg_process(const struct fi_cfg_head *cfg_head);
extern void fi_init(struct sock *nlfd);
extern void fi_deinit(void);
void fi_bw_calc(struct fi_flow_node *node);
void fi_rtt_update(struct fi_flow_node *flow);
int fi_parse_http_rsp_code(const struct sk_buff *skb, char *payload, uint32_t parse_len);

#endif /* _FI_H */
