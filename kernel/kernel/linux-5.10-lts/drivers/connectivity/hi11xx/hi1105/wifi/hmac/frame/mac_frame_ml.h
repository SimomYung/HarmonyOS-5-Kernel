/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : mac_frame_ml.c 的头文件
 * 创建日期   : 2023年9月18日
 */

#ifndef MAC_FRAME_ML_H
#define MAC_FRAME_ML_H

#include "wlan_types.h"
#include "oam_ext_if.h"
#include "securec.h"
#include "mac_user.h"
#include "mac_vap.h"
#include "hmac_encap_frame_ap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_ML

uint8_t mac_set_auth_basic_ml_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *addr, uint8_t addr_len);
void mac_free_sub_ie_defrag_buffer(uint8_t *defrag_sub_ie);
uint8_t mac_check_sub_ie_need_defragment(uint8_t *sub_ie, uint16_t max_len);
uint32_t mac_per_sta_profile_subelement_defragment(uint8_t **sub_ie, uint16_t *defrag_len, uint16_t *frag_len,
    uint8_t *defrag_sub_ie, uint16_t defrag_sub_ie_len);
uint8_t mac_count_basic_ml_sta_info_length(uint8_t *sta_control);
uint32_t mac_sta_parse_basic_ml_sta_info(uint8_t *sta_info, uint8_t *sta_control,
    ml_link_info_stru *ml_link_info);
void mac_set_basic_ml_ie_control(hmac_vap_stru *hmac_vap, mac_ml_control_union *ml_control);
uint8_t mac_set_eml_capabilities_field(hmac_mld_vap_info_stru *mld_vap_link_info, uint8_t *buffer);
uint8_t mac_set_ml_link_id_info_field(uint8_t link_id, uint8_t *buffer);
uint8_t mac_set_mld_capabilities_field(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t link_cnt);
uint8_t mac_set_mld_reconfig_cap_and_op_field(uint8_t *buffer, uint8_t link_cnt);
uint8_t mac_get_ml_reconfig_eml_capabilities(hmac_vap_stru *hmac_vap);
uint32_t mac_basic_ml_element_defragment(uint8_t **ml_ie, uint16_t *ret_len,
    uint8_t *defrag_ie, uint16_t defrag_ie_len);
void mac_free_ie_defrag_buffer(uint8_t *defrag_ie);
uint32_t mac_parse_auth_req_ml_info(uint8_t *payload, uint16_t payload_len,
    hmac_auth_req_info_stru *auth_req_info);
ml_link_info_stru *hmac_get_bss_mld_link_info(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr);
uint32_t hmac_sta_parse_basic_ml_sta_profile(hmac_vap_stru *hmac_vap, uint8_t *sta_profile,
    uint16_t sta_profile_len, ml_link_info_stru *ml_link_info, uint8_t frame_type);
uint32_t hmac_sta_parse_basic_ml_common_info_process(uint8_t  *ml_ie, uint16_t ie_len,
    ml_info_stru *ap_mld_info, uint8_t *ret_common_info_len);
uint8_t mac_check_ml_ie_need_defragment(uint8_t *ml_ie, uint16_t max_len);
#endif /* end of MAC_FRAME_ML_H */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
