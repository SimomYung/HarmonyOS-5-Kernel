/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_multilink.c的头文件
 * 作    者 : wifi
 * 创建日期 : 2023年11月8日
 */

#ifndef HMAC_MULTILINK_H
#define HMAC_MULTILINK_H

#include "hmac_resource.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MULTILINK_H

#ifdef _PRE_WLAN_FEATURE_ML
#pragma pack(1)
typedef struct {
    mac_tbtt_infomation_header_stru tbtt_info_hdr;
    uint8_t operation_class;
    uint8_t channel_num;
} mac_neighbor_ap_info_stru;

typedef struct {
    uint8_t neigh_ap_tbtt_offset;
    uint8_t bssid[WLAN_MAC_ADDR_LEN];
    uint32_t short_ssid;
    mac_bss_parameters_subfield_stru bss_param;
    uint8_t psd_for_20m;
    mac_mld_paramters_stru mld_param;
} mac_mld_tbtt_info_stru;
#pragma pack()

uint8_t hmac_set_auth_req_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *mgmt_frame);
uint8_t hmac_set_auth_resp_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *mgmt_frame, uint8_t is_ml_auth);
uint8_t hmac_set_assoc_req_basic_ml_ie(hmac_vap_stru *hmac_sta, uint8_t *req_frame);
uint8_t hmac_set_assoc_rsp_basic_ml_ie(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *rsp_frame);
uint32_t hmac_scan_update_multi_link_cap(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr,
    uint8_t *frame_body, uint16_t frame_len);
uint32_t hmac_mld_process_asoc_rsp(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *mac_hdr, uint8_t *payload, uint16_t msg_len);
void hmac_mld_rx_mgmt_change_addr(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf);
void hmac_mld_tx_mgmt_change_addr(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf);
uint32_t hmac_sta_parse_basic_ml_common_info(uint8_t *common_info, mac_eht_ml_ie_header_stru *ml_ie_stru,
    ml_info_stru *ap_mld_info, uint8_t common_info_len);
uint8_t mac_count_basic_ml_common_info_length(mac_eht_ml_ie_header_stru *ml_ie_stru);
uint32_t hmac_ap_parse_assoc_req_basic_ml_ie(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len);
hmac_vap_stru *hmac_mld_get_sta_vap_by_ap_link_id(hmac_vap_stru *hmac_vap, int8_t ap_link_id);
ml_link_info_stru *hmac_get_ap_link_info(hmac_vap_stru *hmac_vap, ml_info_stru *ap_mld_info);
uint32_t hmac_sta_parse_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *ie, uint16_t ie_len,
    ml_info_stru *ap_mld_info, uint8_t frame_type);
#endif /* end of _PRE_WLAN_FEATURE_ML */
#endif /* end of HMAC_MULTILINK_H */
