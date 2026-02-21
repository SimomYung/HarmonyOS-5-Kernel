/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: This module is is the internal header file for the
 * 				model parameters
 * Author: hongyuping1@huawei.com
 * Create: 2024-02-26
 */

#ifndef _MODEL_PARA_COLLECT_H
#define _MODEL_PARA_COLLECT_H

#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/net.h>
#include <net/sock.h>

#define PAYLOAD_HEADER_ADDR 8
#define WECHAT_VIDEO_LAG 0x77
#define WECHAT_VOICE_LAG 0x76
#define BITMAP_INDEX_LEN 4
#define BYTE_LEN 8
#define SHORT_MAX_HEX 0xFFFF
#define TIME_ONESEC_TO_MS 1000
#define BITMAP_MAX_NUM 8
#define BYTE_MAX_HEX 0xFF
#define FOUR_BIT_MAX_HEX 0x0F
#define BITMAP_INCLUDE 16
#define SKB_MAX_FRAG_LIST_NUM 64
#define MAX_PARALLEL_FLOWS_NUM 4
#define VOIP_TYPE_NUMS 2
#define VIDEO_PARA_INDEX 0
#define VOICE_PARA_INDEX 1
#define VIDEO_FI_TYPE 8
#define PTP_PTS_ALL_EXIST_NUM 2
#define PAYLOAD_HEAD_TYPE_9711 0x9711
#define PKTTYPE_OFFSET_9711 14
#define SN_HIGH_OFFSET_9711 15
#define SN_LOW_OFFSET_9711 16
#define PAYLOAD_HEAD_TYPE_A401 0xA401
#define PKTTYPE_OFFSET_A401 7
#define SN_HIGH_OFFSET_A401 2
#define SN_LOW_OFFSET_A401 3
#define PAYLOAD_HEAD_TYPE_9613 0x9613
#define PKTTYPE_OFFSET_9613 14
#define SN_HIGH_OFFSET_9613 16
#define SN_LOW_OFFSET_9613 15
#define VIDEO_TYPE_VALUE_9613 0x21
#define PAYLOAD_HEAD_TYPE_9815 0x9815
#define PKTTYPE_OFFSET_9815 14
#define SN_HIGH_OFFSET_9815 16
#define SN_LOW_OFFSET_9815 15
#define VOICE_TYPE_VALUE_9815 0x10
#define EXCEPTION_OFFSET_9815 19
#define EXCEPTION_VALUE_9815 0x0b
#define PAYLOAD_HEAD_TYPE_MAX_NUM 4
#define OFFSET_MAX_NUM 4
#define PKT_TYPE_OFFSET_INDEX 1
#define SN_HIGH_OFFSET_INDEX 2
#define SN_LOW_OFFSET_INDEX 3
#define SN_HIGH_MAX_DIV 4


enum model_status {
	MODEL_OK,
	MODEL_FAIL,
};

struct payload_info_offset {
	u16 pkt_type_offset;
	u16 sn_high_offset;
	u16 sn_low_offset;
	u16 exception_byte_offset;
};

/* The structure of key-flow-notify data */
struct ip_info_tuple {
	u32 uid;
	u32 dest_port;
	u16 l3proto;
	u16 fi_type;
	union {
		struct in6_addr ip6_addr;
		u32 ip4_addr;
	} dest_addr;
};

struct model_pkt_parse {
	u16 time_stamp;
	u16 raw_frag_num;
	u16 frag_payload_len;
	unsigned char *frag_payload;
};

struct extracted_payload_para {
	u16 timestamp;
	u16 pkt_type;
	u16 pkt_len;
	u16 seq_num;
};

struct udp_para_rpt {
	int total_pkt_len;
	int pktnum;
	int loss_num;
	int max_con_loss_num;
	int total_time_interval;
	int max_time_interval;
	int square_sum_time_interval;
};

struct extra_para_rpt {
	u8 is_avaliable;
	u8 is_ptp;
	u16 fi_type;
};

struct  udp_info_msg {
	u16 type;
	u16 len;
	u32 is_need_combination;
	struct extra_para_rpt para_ext[MAX_PARALLEL_FLOWS_NUM];
	struct udp_para_rpt para_rpt[MAX_PARALLEL_FLOWS_NUM][VOIP_TYPE_NUMS]; // video,voice
};

struct temp_calc_para {
	bool is_record_start_sn;
	u8 header_number;
	u16 sn_start;
	u16 sn_end;
	short ts_last;
};

struct seqnum_bit_map {
	u32 header;
	u16 map[BITMAP_INCLUDE];
};

struct calc_para {
	struct udp_para_rpt calc_udp_para;
	struct seqnum_bit_map bit_map[BITMAP_MAX_NUM];
	struct temp_calc_para temp_para;
};

struct model_ctx {
	u16 payload_type;
	u16 fi_type;
	struct payload_info_offset offset;
	struct calc_para calc_para[VOIP_TYPE_NUMS]; // video、voice
	struct calc_para ptp_extra;
};

struct model_ctx_list {
	struct ip_info_tuple ip_info;
	struct model_ctx ctx;
};

struct model_key_flow_req {
	u16 type;
	u16 len;
	u32 app_type;
	struct ip_info_tuple ip_info;
};


void model_get_udp_para(struct sock *sk, struct sk_buff *skb, u8 direction);
void model_info_report(void);
void model_data_collect_start(struct model_key_flow_req *msg);
void model_data_collect_stop(struct model_key_flow_req *msg);

#endif // _MODEL_PARA_COLLECT_H
