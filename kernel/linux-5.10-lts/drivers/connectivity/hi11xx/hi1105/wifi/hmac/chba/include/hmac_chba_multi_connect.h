/*
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 * 功能说明 : chba并行连接相关头文件
 * 作    者 : y30038709
 * 创建日期 : 2024年5月6日
 */
#ifndef HMAC_CHBA_MULTI_CONNECT_H
#define HMAC_CHBA_MULTI_CONNECT_H
#ifdef _PRE_WLAN_CHBA_MGMT
void hmac_chba_whitelist_clear(void);
void hmac_chba_whitelist_del_user(uint8_t *peer_addr, uint8_t addr_len);
uint32_t hmac_chba_whitelist_add_user(uint8_t *peer_addr, uint8_t addr_len);
uint32_t hmac_chba_whitelist_check(uint8_t *peer_addr, uint8_t addr_len);
void hmac_chba_whitelist_init(void);
void hmac_chba_assoc_priv_ie_list_init(void);
uint32_t hmac_chba_save_assoc_priv_ie(hmac_vap_stru *hmac_vap, hmac_chba_conn_param_stru *chba_conn_params);
uint8_t *hmac_chba_find_assoc_priv_ie(const unsigned char *user_mac_addr, uint32_t *app_ie_len);
void hmac_chba_del_assoc_priv_ie(uint8_t *user_mac_addr, uint8_t addr_len);
void hmac_chba_assoc_priv_ie_list_clear(void);
uint32_t hmac_chba_cancel_response_connect(mac_vap_stru *mac_vap, uint16_t len, uint8_t *params);
oal_bool_enum_uint8 hmac_chba_is_not_verified_user(hmac_chba_vap_stru *chba_vap_info, uint16_t user_idx);
void hmac_chba_save_not_verified_user_id(hmac_chba_vap_stru *chba_vap_info, uint16_t user_idx);
void hmac_chba_del_not_verified_user_id(hmac_chba_vap_stru *chba_vap_info, uint16_t user_idx);
void hmac_chba_clear_one_fast_conn_rsp_info(hmac_vap_stru *hmac_vap, hmac_chba_vap_stru *chba_vap_info,
    hmac_user_stru *hmac_user);
#endif
#endif /* end of HMAC_CHBA_MULTI_CONNECT_H */
