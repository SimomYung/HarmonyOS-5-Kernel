/*
 * Copyright (c) @CompanyNameMagicTag 2024-2025. All rights reserved.
 * Description: nbw frame encap and trx
 * Author: wifi
 * Create: 2024-12-26
 */
#ifndef HMAC_NBW_FRAME_H
#define HMAC_NBW_FRAME_H

#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "hmac_vap.h"
#include "hmac_nbw_timeslot_mgmt.h"
#include "mac_nbw_common.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_FRAME_H
#ifdef _PRE_WLAN_FEATURE_NBW

#define MAC_NBW_BEACON_BASIC_ATTR_LEN 3 /* 包含属性头 */
#define WAITTING_TWO_DURATION_MS (MAC_NBW_DEFALUT_PERIODS_MS * 2) /* 两个beacon调度周期 128*2 ms */
#define MASTER_BEACON_INDICATE 1

/* Timeslot neg ACTION帧subtype类型 */
typedef enum {
    MAC_NBW_TIMESLOT_NEG_REQUEST_ACTION = 0,
    MAC_NBW_TIMESLOT_NEG_RESPONSE_ACTION = 1,
    MAC_NBW_TIMESLOT_NEG_INDICATE_ACTION = 2,
    MAC_NBW_TIMESLOT_NEG_ACTION_SUBTYPE_BUTT,
} hmac_nbw_timeslot_neg_action_subtype_enum;

/* Timeslot adjust ACTION帧subtype类型 */
typedef enum {
    MAC_NBW_TIMESLOT_ADJUST_SLAVE_SLAVEAP_REQ_ACTION = 0,
    MAC_NBW_TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_ACTION = 1,
    MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_ACTION = 2,
    MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_ACTION = 3,
    MAC_NBW_TIMESLOT_ADJUST_MASTER_INDICATE_ACTION = 4,
    MAC_NBW_TIMESLOT_ADJUST_ACTION_SUBTYPE_BUTT,
} hmac_nbw_timeslot_adjust_action_subtype_enum;

void hmac_nbw_send_timeslot_frame(uint8_t *peer_addr, uint8_t *slave_addr, uint8_t addr_len,
    hmac_nbw_uslot_info_stru uslot_info, uint8_t action_type, uint8_t request_type);
void hmac_nbw_rx_timeslot_neg_handle(hmac_vap_stru *hmac_vap, uint8_t *peer_addr,
    uint8_t *payload, uint16_t payload_len);
void hmac_nbw_rx_timeslot_adjust_handle(hmac_vap_stru *hmac_vap, uint8_t *peer_addr,
    uint8_t *payload, uint16_t payload_len);
void hmac_nbw_encap_master_beacon(hmac_chba_vap_stru *chba_vap_info, uint8_t *beacon_buf,
    uint16_t *beacon_len, uint8_t beacon_type);
uint32_t hmac_nbw_sync_beacon_info(hmac_vap_stru *hmac_vap, hmac_chba_vap_stru *chba_vap_info);
uint32_t hmac_nbw_waiting_master_slot_time_response_proc(void *arg);
uint32_t hmac_nbw_rx_beacon(hmac_vap_stru *hmac_vap, mac_ieee80211_frame_stru *mac_hdr,
    uint8_t *payload, uint16_t payload_len);
oal_bool_enum_uint8 hmac_nbw_is_nbw_action(uint8_t *payload, uint32_t payload_len);
#endif
#endif
