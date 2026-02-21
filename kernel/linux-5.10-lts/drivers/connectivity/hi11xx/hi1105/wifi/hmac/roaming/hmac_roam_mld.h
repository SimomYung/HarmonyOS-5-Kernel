/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : hmac_roam_mld.c的头文件
 * 作    者 : wifi
 * 创建日期 : 2024年05月09日
 */

#ifndef HMAC_ROAM_MLD_H
#define HMAC_ROAM_MLD_H

#include "hmac_resource.h"
#include "hmac_roam_alg.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_MLD_H

#ifdef _PRE_WLAN_FEATURE_ML
typedef enum {
    ROAM_MLD_CHANGE_TO_LEGACY = 0,
    ROAM_LEGACY_CHANGE_TO_MLD = 1,
    ROAM_MLD_CHANGE_TO_SINGLE_LINK = 2,
    ROAM_SINGLE_LINK_CHANGE_TO_MLD = 3,
    ROAM_SINGLE_LINK_CHANGE_TO_LEGACY = 4,
    ROAM_LEGACY_CHANGE_TO_SINGLE_LINK = 5,
    ROAM_MLD_NO_CHANGE = 6,

    ROAM_MLD_CHANGE_BUTT
} mld_roam_change_type_enum;

uint8_t hmac_roam_mld_check_target_bss_is_link_vap(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr);
uint8_t hmac_roam_mld_bssid_check(hmac_roam_info_stru *roam_info, hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr);
uint32_t hmac_mld_start_roam_connect(hmac_roam_info_stru  *roam_info, uint8_t is_roam_mld_connect,
    uint8_t is_cur_mld_connect);
void hmac_mld_roam_to_old_bss(hmac_roam_info_stru *roam_info, hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user);
void hmac_roam_connect_comp_postdo_notify_mld(hmac_vap_stru *hmac_vap,
    hmac_roam_info_stru *roam_info, uint32_t result);
void hmac_link_sta_wait_asoc_rx_succ_handle(hmac_vap_stru *hmac_sta);
uint32_t hmac_proc_mld_roam_to_old_bss(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info);
void hmac_roam_connect_comp_predo_notify_mld(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint32_t result);
uint8_t hmac_roam_check_need_link_switch(hmac_roam_info_stru *roam_info, hmac_vap_stru *hmac_vap,
    mac_bss_dscr_stru *bss_dscr, uint8_t is_roam_mld_connect, uint8_t is_cur_mld_connect);
void hmac_roam_start_clear_old_mld_connet_info(hmac_roam_info_stru *roam_info);
uint32_t hmac_mld_link_switch_deauth_force_reassoc(hmac_vap_stru *hmac_vap);
void hmac_roam_mld_recover_mlsr_relationship(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info);
#endif /* end of _PRE_WLAN_FEATURE_ML */
#endif /* end of OAM_FILE_ID_HMAC_ROAM_MLD_H */
