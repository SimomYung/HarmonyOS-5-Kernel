/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_multilink_reconfig.c的头文件
 * 作    者 : wifi
 * 创建日期 : 2024年2月8日
 */

#ifndef HMAC_MULTILINK_RECONFIG_H
#define HMAC_MULTILINK_RECONFIG_H

#include "hmac_resource.h"
#include "hmac_device.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MULTILINK_RECONFIF_H

#ifdef _PRE_WLAN_FEATURE_ML

#define MLD_RECFG_COMMON_INFO_LENGTH_FIELD_LEN 1
#define MLD_RECFG_COMMON_INFO_EML_CAP_FIELD_LEN 2
#define MLD_RECFG_COMMON_INFO_ML_CAP_OP_FIELD_LEN 2
#define MLD_RECFG_PER_STA_SUBELEMENT_FIELD_LEN 1
#define MLD_RECFG_PER_STA_LENGTH_FIELD_OFFSET MLD_RECFG_PER_STA_SUBELEMENT_FIELD_LEN
#define MLD_RECFG_PER_STA_LENGTH_FIELD_LEN 1
#define MLD_RECFG_PER_STA_STA_INFO_LEN_FIELD_LEN 1
#define MLD_RECFG_PER_STA_STA_INFO_OP_PARA_LEN 3
#define MLD_RECFG_REQ_TIMEOUT 500

typedef enum {
    ML_RECONFIG_AP_REMOVAL                    = 0,
    ML_RECONFIG_OPERATION_PARAMETER_UPDATE    = 1,
    ML_RECONFIG_ADD_LINK                      = 2,
    ML_RECONFIG_DELETE_LINK                   = 3,
    ML_RECONFIG_NSTR_STATUS_UPDATE            = 4,

    ML_RECONFIG_OP_TYPE_BUTT
} mac_ml_reconfig_operation_type_enum;

typedef struct {
    hmac_vap_stru *hmac_vap;
    mac_vap_stru *mac_vap;
    uint8_t user_assoc_id;
    uint8_t *user_mac_addr;
    mac_cfg_tx_ml_reconfig_param_stru *param;
    oal_netbuf_stru *netbuf;
    uint8_t cb_type;
    uint8_t cb_subtype;
    uint8_t need_ml_reconfig_action_payload;
    uint8_t rev[1];
    uint16_t payload_len;       // netbuf payload长度, 若netbuf携带tx_action, 则包含ctx_action长度
} hmac_ml_reconfig_gen_netbuf_ctx_stru;

uint32_t hmac_parse_reconfiguration_ml_ie(hmac_vap_stru *hmac_vap, mac_ies_info *ie_info);
uint32_t hmac_tx_action_ml_reconfig_request_proc(mac_vap_stru *mac_vap,
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_para);
void hmac_rx_ml_reconfig_rsp_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *frame_hdr, uint8_t *payload, uint32_t payload_len);
uint16_t hmac_set_reconfiguration_ml_ie_sta_complete_ie(hmac_vap_stru *hmac_sta, mac_vap_stru *mac_vap,
    uint8_t *frame, hmac_scanned_bss_info *scaned_bss);
uint32_t hmac_parse_reconfig_rsp_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *payload, uint16_t msg_len,
    ml_info_stru *ap_mld_info);
uint32_t hmac_ml_reconfig_remove_link(mac_vap_stru *mac_vap, uint16_t tbtt_num, hmac_vap_stru *delete_hmac_vap);

#endif /* end of _PRE_WLAN_FEATURE_ML */
#endif /* end of HMAC_MULTILINK_RECONFIG_H */
