/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_scan.c 的头文件
 * 作    者 :
 * 创建日期 : 2014年11月26日
 */

#ifndef HMAC_ML_SCAN_H
#define HMAC_ML_SCAN_H

#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hmac_vap.h"
#include "mac_device.h"
#include "hmac_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_ML

typedef struct {
    uint8_t eid;
    uint8_t ext_eid;
} mac_comm_ie_info;

typedef struct {
    uint32_t oui;
    uint8_t oui_type;
} mac_comm_vendor_ie_info;

OAL_STATIC OAL_INLINE uint32_t hmac_check_ml_scan_time(hmac_vap_stru *hmac_vap)
{
    return hmac_vap->ml_scan_param.scan_req_cnt < ML_SCAN_MAX_TIME;
}

uint32_t hmac_set_hipriv_ml_scan(mac_vap_stru *mac_vap, uint32_t *params);
uint32_t hmac_get_hipriv_ml_scan(void);
uint32_t hmac_get_hipriv_ml_scan_no_bss(void);
void hmac_ml_scan_comp_cb(void *record);
void hmac_prepare_ml_scan_req(hmac_vap_stru *hmac_vap, mac_scan_req_stru *scan_params);
void hmac_clear_ml_scan_params(hmac_vap_stru *hmac_vap);
uint8_t hmac_sta_check_start_ml_scan(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr);
uint32_t hmac_sta_start_ml_scan(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr, mac_vap_state_enum vap_state,
    hmac_conn_param_stru *connect_param);
uint32_t hmac_sta_parse_scan_result_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *ie, uint16_t ie_len,
    ml_info_stru *ap_mld_info, uint8_t frame_type);
uint32_t hmac_ml_scan_result_handle(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf, uint16_t buf_len);
void hmac_free_ml_defragment(void);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_scan.h */
