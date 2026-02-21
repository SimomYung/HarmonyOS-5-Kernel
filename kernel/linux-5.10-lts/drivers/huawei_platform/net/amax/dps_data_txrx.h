/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: This module is to support device-pipe synergy data frame sending and reveiving, framwork communication.
 * Create: 2023-11-23
 */
#ifndef DPS_DATA_TXRX_H
#define DPS_DATA_TXRX_H

#include "amax_multilink_main.h"

#define IP_UDP_LEN 28
#define DPS_VERSION_TYPE 17
#define DPS_BUFFER_MAX_SIZE 1400
#define MAX_LINK_NUM 2
#define OPS_GET_LINK_INFO 6
#define TLV_TYPE_MULTILINK_BASIC_V2 20
#define DPS_MSG_TWO_BYTE_SIZE 2
#define DPS_MSG_GET_INFO_HEADER_LENGTH 14
#define DPS_MSG_GET_INFO_TOTAL_LENGTH 30
#define DPS_MSG_GET_INFO_LENGTH 28
#define DPS_MSG_GET_INFO_TLV_LENGTH 26
#define TRANSITION_DEFAULT_ID 1

typedef enum {
	DPS_DEV_MAIN = 0,
	DPS_DEV_ASSIST,
	DPS_DEV_DOUBLE,
	DPS_DEV_BUTT
} dps_fn_type;

typedef enum {
	OPERATION_AMAX_ROAM_START = 20,
	OPERATION_AMAX_ROAM_END,
	OPERATION_AMAX_LINK_OPS,
} dps_ops_type;

typedef enum {
	TRANSITION_AMAX_ROAM_SUCC = 0,
	TRANSITION_AMAX_ROAM_FAIL,
	TRANSITION_AMAX_MAIN_LINK_INFO_REQ,
	TRANSITION_AMAX_ASSIST_LINK_INFO_REQ,
	TRANSITION_AMAX_ROAM_ENABLE,
	TRANSITION_AMAX_ROAM_DISABLE
} dps_trans_type;

typedef struct {
	unsigned char send_dev;
	unsigned char bssid[ETHER_ADDR_LEN];
	unsigned char data[0];
} dps_mgmt_msg;

typedef struct {
	uint8_t version;
	uint8_t operation[DPS_MSG_TWO_BYTE_SIZE];
	uint8_t transactionID[DPS_MSG_TWO_BYTE_SIZE];
} dps_action_header;

typedef struct {
	dps_action_header header;
	uint8_t tlv_length[DPS_MSG_TWO_BYTE_SIZE];
	unsigned char link_bssid[ETHER_ADDR_LEN];
} dps_action_frame;

typedef struct {
	uint8_t link_type;
	uint8_t link_name;
	uint8_t link_capa;
	uint8_t link_id;
	unsigned char link_bssid[ETHER_ADDR_LEN];
	uint8_t link_channel_num;
	uint8_t link_auth_type;
} dps_action_link_info;

typedef struct {
	uint8_t tlv_length[DPS_MSG_TWO_BYTE_SIZE];
	uint8_t type;
	uint8_t length;
	uint8_t apMLD_id;
	uint8_t link_num;
	dps_action_link_info link_info[MAX_LINK_NUM];
} dps_action_tlv;

typedef struct {
	char param_buf[DPS_BUFFER_MAX_SIZE];
	unsigned int param_size;
} dps_action_info;

extern dps_action_info g_amax_roam_start_info;
extern dps_action_info g_amax_roam_end_info;

void dps_swap_uint16(uint8_t *dst, uint16_t value);
void dps_alloc_and_send_frame(dps_mgmt_msg *dps_data_info, unsigned int msg_len);
uint32_t check_pkt_is_dps_data(struct sk_buff *netbuf);
void dps_report_data_to_userspace(struct sk_buff *netbuf);
void dps_ioctl_msg_proc(uint8_t *msg_info, unsigned int msg_len);
void dps_report_amax_msg_to_userspace(uint8_t *msg_info, unsigned int msg_len, int32_t msg_type);
uint8_t *dps_get_sta_mac_addr(uint8_t dev);
#endif
