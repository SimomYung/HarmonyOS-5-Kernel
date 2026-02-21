/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_ml_ttlm.c的头文件
 * 作    者 : wifi
 * 创建日期 : 2024年2月2日
 */

#ifndef HMAC_ML_TTLM_H
#define HMAC_ML_TTLM_H

#include "hmac_user.h"
#include "mac_vap.h"
#include "hmac_vap.h"
#include "mac_mld.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ML_TTLM_H

#ifdef _PRE_WLAN_FEATURE_ML
#define MAC_CB_DEFULT                   0xff
#define TTLM_MAPPING_SWITCH_TIME_LEN    2    // TTLM element mapping switch time filed length
#define TTLM_EXPECTED_DURATION_LEN      3    // TTLM element expected duration filed length
#define TTLM_IE_MIN_LEN                 2    // TTLM element min length
#define TTLM_IE_MAX_LEN                 24   // TTLM element max length
#define TTLM_REQUEST_TIME               500  // 500ms
#define TTLM_EXPECTED_DURATION_TIME_LEN 3    // ttlm ie中expected duration time字段长度
uint32_t hmac_mld_user_ttlm_init(hmac_user_stru *hmac_user);
oal_bool_enum_uint8 hmac_vap_check_ttlm_is_support(mac_vap_stru *mac_vap);
oal_bool_enum_uint8 hmac_user_check_ttlm_is_support(hmac_user_stru *hmac_user);
uint32_t hmac_ttlm_req_param_check(hmac_user_stru *hmac_user, mac_cfg_ttlm_frame_param *ttlm_req_param);
void hmac_save_ttlm_req_param(hmac_user_stru *hmac_user, mac_cfg_ttlm_frame_param *ttlm_req_param);
uint32_t hmac_send_ttlm_request(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    mac_cfg_ttlm_frame_param *ttlm_req_param);
uint32_t hmac_mld_ttlm_request_process(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
                                       mac_cfg_ttlm_frame_param *ttlm_req_param);
uint32_t hmac_send_ttlm_teardown(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *param);
uint32_t hmac_sta_rx_beacon_ttlm_process(hmac_vap_stru *hmac_vap, uint8_t *payload, mac_ies_info *ies_info);
uint32_t hmac_rx_ttlm_request_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf);
uint32_t hmac_rx_ttlm_response_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf);
uint32_t hmac_rx_ttlm_teardown_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);
uint32_t hmac_ttlm_request_time_timeout(void *arg);
void hmac_set_mld_user_default_ttlm_info(hmac_user_stru *hmac_user);
uint32_t hmac_ttlm_update_status(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user);
void hmac_set_ttlm_status(hmac_mld_user_manage_stru *mld_user_manage, wlan_ttlm_status_enum status);
void hmac_ttlm_destory_timers(hmac_mld_user_manage_stru  *mld_user_manage);
#endif
#endif
