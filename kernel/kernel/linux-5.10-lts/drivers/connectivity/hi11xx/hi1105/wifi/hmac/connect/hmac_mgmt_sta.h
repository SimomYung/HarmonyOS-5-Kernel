/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_mgmt_sta.c 的头文件
 * 作    者 :
 * 创建日期 : 2013年6月18日
 */

#ifndef HMAC_MGMT_STA_H
#define HMAC_MGMT_STA_H

#include "oal_ext_if.h"
#include "hmac_vap.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_mgmt_join.h"
#include "hmac_cali_mgmt.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_STA_H

#define HMAC_WMM_QOS_PARAMS_HDR_LEN   8
#define HMAC_WMM_QOSINFO_AND_RESV_LEN 2
#define HMAC_WMM_AC_PARAMS_RECORD_LEN 4
#define DATARATES_80211G_NUM          12

/* 去关联原因 */
typedef struct {
    hmac_mgmt_status_enum_uint8 en_disasoc_reason_code;
    uint8_t auc_resv[3];
} hmac_disasoc_rsp_stru;

typedef struct {
    hmac_mgmt_status_enum_uint8 en_result_code;
    uint8_t auc_resv[3];
} hmac_ap_start_rsp_stru;

/* 与dmac层的mpxx_date_rate_stru g_ast_mpxx_rates_11g同步修改 */
typedef struct {
    uint8_t uc_expand_rate;         /* 扩展速率集 */
    uint8_t uc_mac_rate;            /* MAC对应速率 */
    uint8_t uc_hal_wlan_rate_index; /* 速率index */
    uint8_t uc_rsv;
} hmac_data_rate_stru;

uint32_t hmac_sta_wait_asoc_rx(hmac_vap_stru *hmac_sta, void *msg);
uint32_t hmac_sta_auth_timeout(hmac_vap_stru *pst_hmac_sta, void *p_param);
uint32_t hmac_sta_wait_asoc_timeout(hmac_vap_stru *pst_hmac_sta, void *p_param);
uint32_t hmac_sta_not_up_rx_mgmt(hmac_vap_stru *hmac_vap, void *param);
void hmac_sta_set_txopps_partial_aid(mac_vap_stru *pst_mac_vap);
uint32_t hmac_process_assoc_rsp(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *mac_hdr, uint16_t hdr_len, uint8_t *payload, uint16_t msg_len_input);
uint8_t *hmac_sta_find_ie_in_probe_rsp(mac_vap_stru *pst_mac_vap, uint8_t uc_eid, uint16_t *pus_index);
void hmac_mgmt_sta_roam_rx_action(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf,
    oal_bool_enum_uint8 is_protected);
void hmac_process_assoc_rsp_user_sync_cap(hmac_vap_stru *hmac_sta,
    hmac_user_stru *hmac_user, uint8_t *mac_hdr, mac_ies_info *ies_info);
void hmac_sta_wait_asoc_rx_cfg_rsp_para(hmac_asoc_rsp_stru *asoc_rsp, uint8_t *mac_hdr,
    uint16_t msg_len, hmac_user_stru *hmac_user_ap);
void hmac_sta_wait_asoc_rx_rpt_user_chan_check(mac_vap_stru *mac_vap, uint32_t *ret,
    hmac_vap_stru *hmac_sta, uint16_t user_idx);
void hmac_process_assoc_rsp_set_user_para(hmac_user_stru *hmac_user,
    hmac_vap_stru *hmac_sta, uint32_t *change);
uint32_t hmac_update_vht_opern_ie_sta(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user,
    uint8_t *puc_payload);
void hmac_process_assoc_rsp_set_aid(mac_vap_stru *mac_vap, uint8_t *payload);
void hmac_process_assoc_rsp_parse_ie(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *mac_hdr, mac_ies_info *ies_info);
void hmac_process_assoc_rsp_set_feature(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *mac_hdr, mac_ies_info *ies_info);
void hmac_sta_wait_asoc_rx_succ_handle_single_link(hmac_user_stru *hmac_user_ap,
    hmac_vap_stru *hmac_sta, mac_vap_state_enum_uint8 en_new_vap_state, uint8_t *addr_sa, uint16_t user_idx);
void hmac_sta_update_h2d_join_req_params(hmac_vap_stru *hmac_vap,
    dmac_ctx_join_req_set_reg_stru *reg_params, mac_bss_dscr_stru *join_bss_dscr);
uint32_t hmac_config_set_channel_check_param(mac_vap_stru *pst_mac_vap, mac_cfg_channel_param_stru *pst_prot_param);
void hmac_config_chan_bw_exceed(mac_vap_stru *pst_mac_vap, mac_cfg_channel_param_stru *pst_channel_param);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_mgmt_sta.h */
