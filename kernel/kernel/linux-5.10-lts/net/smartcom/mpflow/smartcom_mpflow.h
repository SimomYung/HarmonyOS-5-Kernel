/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef _SMARTCOM_MPFLOW_H
#define _SMARTCOM_MPFLOW_H

#include <net/sock.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/netdevice.h>
#include <linux/spinlock.h>

#include "smartcom/smartcom.h"
#include "../smartcom_utils.h"

#define MPFLOW_ERROR (-1)
#define MPFLOW_OK 0

#define SKARR_SZ 200

#define SMARTCOM_MPFLOW_IP_BURST_OFF 0
#define SMARTCOM_MPFLOW_IP_BURST_AUTO 1
#define SMARTCOM_MPFLOW_IP_BURST_FIX 2

#define SMARTCOM_MPFLOW_AI_MAX_APP  10
#define SMARTCOM_MPFLOW_BIND_PORT_SIZE 16
#define SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE 8
#define SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX SMARTCOM_MPFLOW_BIND_PORT_SIZE

#define SMARTCOM_MPFLOW_AI_RESET_DURATION msecs_to_jiffies(500)

#define SMARTCOM_MPFLOW_FALLBACK_NOPAYLOAD_THRH 5
#define SMARTCOM_MPFLOW_FALLBACK_SYN_RST_THRH 3
#define SMARTCOM_MPFLOW_FALLBACK_SYN_TOUT_THRH 3
#define SMARTCOM_MPFLOW_FALLBACK_RETRANS_THRH 3
#define SMARTCOM_MPFLOW_FALLBACK_RETRANS_TIME 5
#define SMARTCOM_MPFLOW_FALLBACK_SYN_TOUT_TIME 3

#define SMARTCOM_MPFLOW_FALLBACK_CLR 0
#define SMARTCOM_MPFLOW_FALLBACK_SET 1
#define SMARTCOM_MPFLOW_FALLBACK_SYNCLR 2
#define SMARTCOM_MPFLOW_FALLBACK_NONE 3

#define SMARTCOM_MPFLOW_BINDMODE_ORI_DEV_OFFSET 8
#define SMARTCOM_MPFLOW_BINDMODE_ORI_DEV_MASK ((1 << SMARTCOM_MPFLOW_BINDMODE_ORI_DEV_OFFSET) - 1)

#define SMARTCOM_MPFLOW_SND_BYTE_THRESHOLD 8
#define SMARTCOM_MPFLOW_BINDMODE_MASK 0xFFFF
#define SMARTCOM_MPFLOW_BINDMODE_SHIFT 16
#define SMARTCOM_MPFLOW_FALLBACK_WAIT_TIME 10

#define SMARTCOM_MPFLOW_FLOW_BIND_BURST_TIME msecs_to_jiffies(500)

typedef enum {
	SMARTCOM_MPFLOW_BIND_NONE = 0,
	SMARTCOM_MPFLOW_BIND_WIFI0,
	SMARTCOM_MPFLOW_BIND_WIFI1,
	SMARTCOM_MPFLOW_BIND_CELL0,
	SMARTCOM_MPFLOW_BIND_CELL1,
	SMARTCOM_MPFLOW_BIND_MAX
} smartcom_mpflow_ai_bind_dev;

#define SMARTCOM_MPFLOW_DEV_NUM  SMARTCOM_NET_MAX_NUM
#define SMARTCOM_MPFLOW_AI_MAX_LIST_NUM (SMARTCOM_MPFLOW_DEV_NUM * SMARTCOM_MPFLOW_AI_MAX_APP)

struct smarmcom_mpflow_network_info {
	int32_t net_type;
	int32_t net_id;
	int32_t is_validated;
	int32_t is_default;
	char iface_name[IFNAMSIZ];
};

struct smartcom_mpflow_stat {
	struct list_head list;
	uid_t uid;
	int ifindex;
	char name[IFNAMSIZ];
	int16_t mpflow_estlink;          /* ESTABLISHED link */
	uint8_t mpflow_fallback;         /* fallback flag */
	uint8_t mpflow_fail_nopayload;   /* failure count from app server no payload */
	uint8_t mpflow_fail_syn_rst;     /* failure count from syn rst */
	uint8_t mpflow_fail_syn_timeout; /* failure count from syn timeout */
	unsigned long start_jiffies;
	unsigned long report_jiffies;
	uint16_t retrans_count[SMARTCOM_MPFLOW_FALLBACK_RETRANS_TIME + 1];
};

struct smartcom_mpflow_fallback_ver {
	uid_t uid;
	int8_t index;
};

struct smartcom_mpflow_fallback_info {
	uid_t uid;
	int32_t reason;
	int32_t net_type;
};

typedef enum {
	IMMEDIATE = 1,
	ON_SAME_TYPE_FIN,
	ON_OTH_DEV_SAME_TYPE_FIN
} smartcom_mpflow_reset_timing;

typedef enum {
	SK_ERROR = 1,
	INTF_CHANGE
} smartcom_mpflow_reset_method;

/* Reset related definition: FlowInfo */
struct smartcom_mpflow_flow_info {
	uint16_t l3proto;
	uint8_t l4proto;
	union {
		struct {
			uint32_t v4_sip;
			uint32_t v4_dip;
		};
#if defined(CONFIG_IPV6)
		struct {
			struct in6_addr v6_sip;
			struct in6_addr v6_dip;
		};
#endif
	};
	uint16_t src_port;
	uint16_t dst_port;
	int sk_dev_itf;
};

/* policy to kernel: RstPolicy2K */
struct smartcom_mpflow_reset_policy {
	smartcom_mpflow_reset_timing    timing;
	smartcom_mpflow_reset_method    method;
	uint32_t      blink_time;
	uint16_t      rst_bind_dev_or_mode;
	int32_t       err_num;
	char reset_ifname[IFNAMSIZ];
};

struct smartcom_mpflow_reset_flow_policy_info {
	uid_t uid; /* The uid of application */
	struct smartcom_mpflow_flow_info flow;
	struct smartcom_mpflow_reset_policy policy;
};

struct smartcom_mpflow_rst_all_flow_info {
	uid_t uid; /* The uid of application */
	struct smartcom_mpflow_reset_policy policy;
};

struct smartcom_mpflow_parse_stop_info {
	uid_t uid;              /* The uid of Acc Application */
	int32_t stop_reason;    /* The reason of mpflow_stop */
};

struct smartcom_mpflow_dport_range {
	uint16_t start_port;    /* start port */
	uint16_t end_port;      /* end port */
};

struct smartcom_mpflow_burst_ratio {
	uid_t uid; /* The uid of application */
	uint8_t ratio[SMARTCOM_NET_MAX_NUM];
};

// interface daemon to kernel: InitBindConfig app start
struct smartcom_mpflow_ai_port_cfg {
	uid_t   uid;            /* The uid of application */
	uint8_t l4_protocol;
	uint8_t port_num;
	uint8_t ip_burst[SMARTCOM_NET_MAX_NUM];    /* wifi vs. lte */
	struct smartcom_mpflow_dport_range port_range[SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE];
};

struct smartcom_mpflow_ai_init_bind_policy {
	uint8_t l4_protocol;
	uint8_t divide_mode; /* fix auto off */
	uint8_t ratio[SMARTCOM_NET_MAX_NUM]; /* wifi vs. lte */
	uint8_t port_num;
	struct smartcom_mpflow_dport_range port_range[SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE];
};

struct smartcom_mpflow_ai_init_bind_cfg {
	uid_t   uid;            /* The uid of application */
	uint8_t policy_num;
	int32_t excluded_tcp_ports[SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE];
	struct smartcom_mpflow_ai_init_bind_policy burst_bind;
	struct smartcom_mpflow_ai_init_bind_policy scatter_bind[SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE];
};

// interface daemon to kernel: port bind
struct smartcom_mpflow_port_bind_cfg {
	uid_t    uid;            /* The uid of application */
	uint32_t bind_mode;
	uint8_t l4protocol;
	struct smartcom_mpflow_dport_range port_range;
};

struct smartcom_mpflow_ai_bind_pattern {
	uint8_t select_mode;
	uint8_t ratio[SMARTCOM_NET_MAX_NUM];
	uint16_t last_bind_device;
};

struct smartcom_mpflow_single_port_bind_policy {
	uint32_t bind_mode;
	uint16_t protocol;      /* tcp or udp */
	struct smartcom_mpflow_ai_bind_pattern pattern;
	struct smartcom_mpflow_dport_range range;
};

struct smartcom_mpflow_ai_burst_port {
	uint8_t burst_port_num;
	uint8_t burst_protocol; /* tcp udp */
	uint8_t burst_divide_mode; /* off auto fix */
	uint8_t burst_ratio[SMARTCOM_NET_MAX_NUM];
	uint16_t launch_bind_device;
	unsigned long jiffies;
	unsigned long burst_cnt;
	struct smartcom_mpflow_dport_range burst_ports[SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE];
};

struct smartcom_mpflow_ai_info {
	uid_t uid; /* The uid of Acc Application */
	uint8_t port_num; /* config port number */
	uint16_t rst_bind_dev;
	uint16_t rst_all_flow_bind_dev;
	unsigned long rst_duration; /* ms */
	unsigned long rst_jiffies;
	unsigned long rst_all_flow_duration; /* ms */
	unsigned long rst_all_flow_jiffies;
	bool rst_all_flow;

	struct smartcom_mpflow_single_port_bind_policy port_bind_policies[SMARTCOM_MPFLOW_BIND_PORT_SIZE];
	struct smartcom_mpflow_ai_burst_port burst_info;
	int32_t excluded_tcp_ports[SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE];
};

void smartcom_mpflow_init(void);

void smartcom_mpflow_deinit(void);

int smartcom_mpflow_get_best_device(uid_t uid, struct sock *sk, struct sockaddr *uaddr, char *ifname, int len);

void smartcom_mpflow_event_process(int32_t event, uint8_t *pdata, uint16_t len);

void smartcom_mpflow_tcp_fallback(struct sock *sk, int reason, int state);

#endif  /* _SMARTCOM_MPFLOW_H */