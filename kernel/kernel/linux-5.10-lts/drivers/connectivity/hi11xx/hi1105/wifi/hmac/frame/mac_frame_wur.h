/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : mac_frame_wur.c 的头文件
 * 创建日期   : 2023年9月18日
 */

#ifndef MAC_FRAME_WUR_H
#define MAC_FRAME_WUR_H

#include "wlan_types.h"
#include "oam_ext_if.h"
#include "securec.h"
#include "mac_user.h"
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_FRAME_WUR_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_WUR
void mac_set_wur_capabilities_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len);
void mac_set_wur_operation_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len);
uint16_t mac_set_wur_mode_ie(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *buffer,
    uint8_t action_type, uint8_t resp_status);
uint16_t mac_encap_wur_ie(mac_vap_stru *mac_vap, uint8_t *origin_buf, wlan_frame_mgmt_subtype_enum mgmt_subtype);
uint32_t hmac_process_wur_ie(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *payload, uint32_t msg_len);
uint32_t mac_process_wur_operation_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user, uint8_t *wur_oper_ie);
uint32_t mac_process_wur_mode_ie(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *wur_mode_ie);
uint16_t mac_set_wur_mode_ie_sta(mac_vap_stru *mac_vap, uint8_t *buffer,
    uint8_t action_type, uint8_t resp_status);
uint16_t mac_set_wur_mode_ie_ap(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *buffer,
    uint8_t action_type, uint8_t resp_status);
uint16_t hmac_mgmt_encap_asoc_rsp_ap_set_wur_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user,
    uint8_t *buffer);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of MAC_FRAME_11BE_H */
