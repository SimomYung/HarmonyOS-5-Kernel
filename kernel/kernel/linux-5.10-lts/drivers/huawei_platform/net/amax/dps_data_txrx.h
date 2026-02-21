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

#define BLURMAC2STR(a) (a)[0], (a)[1], (a)[4], (a)[5]
#define BLURMACSTR "%02x:%02x:xx:xx:%02x:%02x"
#define DPS_ANNOUNCEMENT_TOTAL_LEN 16
#define COMMON_PART_LEN 7
#define TOTAL_LEN_BYTE 2
#define SINGLE_TLV_BYTE 3
#define VER_11 0x11
#define LOW_POWER_TLV_TYPE 21

typedef enum {
	DPS_DEV_MAIN = 0,
	DPS_DEV_ASSIST,
	DPS_DEV_DOUBLE,
	DPS_DEV_BUTT
} dps_fn_type;

typedef enum {
    DPS_OPERATION_ANNOUNCEMENT = 0,
    DPS_OPERATION_REQUEST = 1,
    DPS_OPERATION_RESPONSE = 2,
    DPS_OPERATION_DATA = 3,
    DPS_OPERATION_ENDING = 4,
    DPS_OPERATION_ABORT = 5,
    DPS_OPERATION_AMAX_GET_REQ = 6,
    DPS_OPERATION_AMAX_GET_RSP = 7,
    DPS_OPERATION_AMAX_CONN_REQ = 8,
    DPS_OPERATION_AMAX_CONN_RSP = 9,
    DPS_OPERATION_AMAX_DISCONN_REQ = 10,
    DPS_OPERATION_AMAX_DISCONN_RSP = 11,
    DPS_OPERATION_ZERO_ROAM_START = 20,
    DPS_OPERATION_ZERO_ROAM_END = 21,
    DPS_OPERATION_AMAX_LINK_OPS = 22,
    DPS_OPERATION_DFT = 23,
    DPS_OPERATION_AMAX_LOW_POWER_REQ = 24,
    DPS_OPERATION_AMAX_LOW_POWER_RSP = 25
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

typedef struct {
    uint8_t version;
    uint16_t operation;
    uint16_t transactionId;
} __attribute__ ((packed)) dps_head;

typedef struct {
	uint8_t send_dev;
	uint8_t bssid[ETHER_ADDR_LEN];
} __attribute__ ((packed)) dps_common_part;

typedef struct {
	uint8_t type;
	uint8_t len;
	uint8_t value[0];
} dps_tlv;

typedef struct {
    uint16_t capsRsv;
    uint8_t capsRsv1;
    uint8_t supportDft : 1;
    uint8_t supportRoamOptimize : 1;
    uint8_t supportQuickSwitchCellular : 1;
    uint8_t supportAmax : 1;
    uint8_t supportLowPower : 1;
	uint8_t reserved : 3;
} dps_capabilities;

extern dps_action_info g_amax_roam_start_info;
extern dps_action_info g_amax_roam_end_info;

void dps_swap_uint16(uint8_t *dst, uint16_t value);
void dps_alloc_and_send_frame(dps_mgmt_msg *dps_data_info, unsigned int msg_len);
uint32_t check_pkt_is_dps_data(struct sk_buff *netbuf);
void dps_report_data_to_userspace(struct sk_buff *netbuf);
void dps_ioctl_msg_proc(uint8_t *msg_info, unsigned int msg_len);
void dps_report_amax_msg_to_userspace(uint8_t *msg_info, unsigned int msg_len, int32_t msg_type);
uint8_t *dps_get_sta_mac_addr(uint8_t dev);
uint8_t *dps_get_main_bssid(void);
bool is_bssid_valid(uint8_t *bssid, uint32_t len);
int32_t dps_build_common_part(uint8_t *buf, uint32_t len);
void dps_build_data_header(uint8_t *buf, uint16_t type, uint8_t version);
void devicepipe_rx_data_report(uint8_t *buf, size_t len);

#endif
