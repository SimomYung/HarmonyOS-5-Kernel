/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_sae.c的头文件
 * 作    者 : wifi3
 * 创建日期 : 2020年07月20日
 */

#ifndef HMAC_SAE_H
#define HMAC_SAE_H

#include "oal_ext_if.h"
#include "hmac_mgmt_join.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SAE_H

#define SAE_AUTH_COMMIT_LEN 104
#define SAE_AUTH_CONFIRM_LEN 40

#ifdef _PRE_WLAN_FEATURE_SAE
oal_bool_enum_uint8 hmac_key_mgmt_psk2(uint32_t akm);
oal_bool_enum_uint8 hmac_key_mgmt_sae(uint32_t akm);
oal_bool_enum_uint8 hmac_key_mgmt_sae8(uint32_t akm);
oal_bool_enum_uint8 hmac_key_mgmt_sae_ext(uint32_t akm);
oal_bool_enum_uint8 hmac_key_mgmt_psk(uint32_t akm);
oal_bool_enum_uint8 hmac_key_mgmt_ft_psk(uint32_t akm);
oal_bool_enum_uint8 hmac_key_mgmt_only_ft(uint32_t *akm, uint32_t len);
oal_bool_enum_uint8 hmac_key_mgmt_8021x(const uint32_t *akm, uint32_t len);
uint32_t hmac_sta_sae_connect_info_wpas(hmac_vap_stru *hmac_sta, hmac_auth_req_stru *auth_req);
uint32_t hmac_sta_process_sae_commit(hmac_vap_stru *sta_vap, oal_netbuf_stru *netbuf);
uint32_t hmac_sta_process_sae_confirm(hmac_vap_stru *sta_vap, oal_netbuf_stru *netbuf);
void hmac_report_ext_auth_worker(oal_work_stru *pst_sae_report_ext_auth_worker);
void hmac_update_sae_connect_param(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_ie, uint32_t ie_len);
uint32_t hmac_config_set_sae_pwe(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param);
uint32_t hmac_vap_set_sae_pwe(mac_vap_stru *mac_vap, uint32_t *params);
#endif
#endif /* end of hmac_sae.h */
