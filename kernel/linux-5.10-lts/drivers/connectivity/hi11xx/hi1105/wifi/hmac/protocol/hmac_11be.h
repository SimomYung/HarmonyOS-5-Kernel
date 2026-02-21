/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_11ax.c的头文件
 * 作    者 : wifi3
 * 创建日期 : 2020年07月21日
 */

#ifndef HMAC_11BE_H
#define HMAC_11BE_H

#include "hmac_resource.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_11BE_H

#ifdef _PRE_WLAN_FEATURE_11BE

void mac_vap_init_11be_rates(mac_vap_stru *mac_vap, mac_device_stru *mac_dev);
void hmac_scan_update_11be_ie(hmac_vap_stru *hmac_vap,
    mac_bss_dscr_stru *bss_dscr, uint8_t *frame_body, uint16_t frame_len);
uint32_t hmac_process_eht_ie(mac_vap_stru *mac_vap,
    hmac_user_stru *hmac_user, mac_ies_info *ies_info);
uint8_t mac_user_get_eht_capable(mac_user_stru *mac_user);
void mac_vap_11be_cap_init(mac_vap_stru *mac_vap);
void hmac_roam_update_11be_protocol_mib(hmac_vap_stru *hmac_vap, uint8_t *bssid);

#endif /* end of _PRE_WLAN_FEATURE_11BE */
#endif /* end of OAM_FILE_ID_HMAC_11BE_H */
