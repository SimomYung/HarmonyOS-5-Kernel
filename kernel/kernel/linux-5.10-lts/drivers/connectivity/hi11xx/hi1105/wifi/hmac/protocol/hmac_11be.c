/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 11AX特性处理
 * 作    者 : wifi3
 * 创建日期 : 2020年07月21日
 */

#include "oal_ext_if.h"
#include "securec.h"
#include "mac_mib.h"
#include "mac_ie.h"
#include "mac_frame_11be.h"
#include "hmac_wifi7_self_cure.h"
#include "hmac_scan.h"
#include "hmac_11be.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_11BE_C

#ifdef _PRE_WLAN_FEATURE_11BE

/*****************************************************************************
功能描述  : 初始化11be速率
  1.日    期   : 2023年9月7日
    作    者   : wifi
    修改内容   : 新生成函数

*****************************************************************************/
void mac_vap_init_11be_rates(mac_vap_stru *mac_vap, mac_device_stru *mac_dev)
{
    return;
}

/*
 * 函 数 名   : hmac_scan_update_bss_list_11be
 * 功能描述   : 更新11be相关信息
 */
void hmac_scan_update_11be_ie(hmac_vap_stru *hmac_vap,
    mac_bss_dscr_stru *bss_dscr, uint8_t *frame_body, uint16_t frame_len)
{
    uint8_t *ie = NULL;
    uint8_t support_320mhz = 0;
    uint8_t channel_width = 0;
    mac_frame_eht_capabilities_ie_stru *eht_cap = NULL;
    mac_frame_eht_operation_ie_stru *eht_oper = NULL;
    mac_ies_info ies_info = {0};

    ies_info.frame_ies = frame_body;
    ies_info.frame_len = frame_len;

    if (is_custom_open_11be_switch(&hmac_vap->st_vap_base_info) == OAL_FALSE) {
        return;
    }

    /* EHT Capabilities IE */
    ie = mac_find_ie_ext_ie(MAC_EID_EHT, MAC_EID_EXT_EHT_CAP, &ies_info);
    if ((ie != NULL) && (ie[1] >= MAC_EHT_CAP_MIN_LEN)) {
        eht_cap = (mac_frame_eht_capabilities_ie_stru *)ie;

        bss_dscr->eht_capable = OAL_TRUE; /* 支持EHT */
        support_320mhz = eht_cap->st_eht_phy_cap.bit_320mhz_6ghz_support;
    }

    /* EHT Operation IE */
    ie = mac_find_ie_ext_ie(MAC_EID_EHT, MAC_EID_EXT_EHT_OPER, &ies_info);
    if ((ie != NULL) && (ie[1] >= MAC_EHT_OPER_MIN_LEN)) {
        eht_oper = (mac_frame_eht_operation_ie_stru *)ie;
        if (eht_oper->eht_oper_parameters.eht_operation_information_present) {
            channel_width = eht_oper->eht_oper_info.channel_width;
            if (support_320mhz && (channel_width & BIT2)) {
                bss_dscr->en_bw_cap = WLAN_BW_CAP_320M;
            }
            /* 更新带宽信息 */
            bss_dscr->en_channel_bandwidth = mac_get_eht_bandwith_from_center_freq_seg0_seg1(
                eht_oper->eht_oper_info.channel_width, bss_dscr->st_channel.uc_chan_number,
                eht_oper->eht_oper_info.ccfs0, eht_oper->eht_oper_info.ccfs1);
        }
    }
}


uint32_t hmac_process_eht_ie(mac_vap_stru *mac_vap,
    hmac_user_stru *hmac_user, mac_ies_info *ies_info)
{
    uint8_t *eht_cap_ie = NULL;
    uint8_t *eht_opern_ie = NULL;
    uint32_t change = MAC_NO_CHANGE;

    memset_s(&(hmac_user->st_user_base_info.st_eht_hdl), sizeof(mac_eht_hdl_stru), 0, sizeof(mac_eht_hdl_stru));

     /* 支持11be，才进行后续的处理 */
    if (g_wlan_spec_cfg->feature_11be_is_open != OAL_TRUE ||
        mac_mib_get_EHTOptionImplemented(mac_vap->pst_mib_info) != OAL_TRUE) {
        return change;
    }

    /* 更新EHT Capabilities ie */
    eht_cap_ie = mac_find_ie_ext_ie(MAC_EID_EHT, MAC_EID_EXT_EHT_CAP, ies_info);
    if (eht_cap_ie != NULL) {
        /* 如果对端(STA)支持11BE，那么协商11BE的能力位 */
        mac_process_eht_capabilities_ie(mac_vap, &hmac_user->st_user_base_info, eht_cap_ie);
    }

    /* 更新EHT Operation ie */
    eht_opern_ie = mac_find_ie_ext_ie(MAC_EID_EHT, MAC_EID_EXT_EHT_OPER, ies_info);
    if (eht_opern_ie != NULL) {
        /* 如果对端(STA)支持11BE，那么协商11BE的能力位 */
        change |= mac_process_eht_operation_ie(mac_vap, &hmac_user->st_user_base_info, eht_opern_ie);
    }
    return change;
}

/*
 * 函 数 名  : mac_user_get_he_capable
 * 功能描述  : 获取he cap能力
 * 1.日    期  : 2017年5月7日
 *   作    者  : 无
 *   修改内容  : 新生成函数
 */
uint8_t mac_user_get_eht_capable(mac_user_stru *mac_user)
{
    if (mac_user != NULL) {
        return mac_user->st_eht_hdl.en_eht_capable;
    }

    return OAL_FALSE;
}

void mac_vap_11be_cap_init(mac_vap_stru *mac_vap)
{
    mac_vap->custom_switch_11be = g_pst_mac_device_capability[0].switch_11be;
    mac_vap->custom_switch_mlo = g_pst_mac_device_capability[0].switch_mlo;
    mac_vap->custom_switch_emlsr = g_pst_mac_device_capability[0].switch_emlsr;
}

void hmac_roam_update_11be_protocol_mib(hmac_vap_stru *hmac_vap, uint8_t *bssid)
{
    mac_vap_stru *mac_vap = &(hmac_vap->st_vap_base_info);
    mac_bss_dscr_stru *bss_dscr = NULL;
    if (hmac_wifi7_self_cure_mac_is_wifi7_blacklist_type(bssid)) {
        mac_mib_set_EHTOptionImplemented(mac_vap->pst_mib_info, OAL_FALSE);
        mac_mib_set_MultiLinkActivated(mac_vap->pst_mib_info, OAL_FALSE);
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_roam_update_11be_protocol_mib::wifi7 connect disable}");
        return;
    }

    if (hmac_wifi7_self_cure_mac_is_mld_blacklist_type(bssid)) {
        mac_mib_set_MultiLinkActivated(mac_vap->pst_mib_info, OAL_FALSE);
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_roam_update_11be_protocol_mib::mld connect disable}");
        return;
    }
    bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(mac_vap, bssid);
    if (bss_dscr == NULL) {
        oam_warning_log3(mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_roam_update_11be_protocol_mib::find bss failed by bssid:%x:XX:XX:XX:%x:%x}",
            bssid[MAC_ADDR_0], bssid[MAC_ADDR_4], bssid[MAC_ADDR_5]);
    } else {
        if (bss_dscr->supp_eht_double_wifi == OAL_TRUE) {
            mac_mib_set_MultiLinkActivated(mac_vap->pst_mib_info, OAL_FALSE);
            oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG,
                "{hmac_roam_update_11be_protocol_mib::supp_eht_double_wifi mld connect disable}");
            return;
        }
    }
    mac_vap_init_mib_mlo(mac_vap);

    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_roam_update_11be_protocol_mib::eht_mib=%u, mld_mib=%u",
        mac_mib_get_EHTOptionImplemented(mac_vap->pst_mib_info),
        mac_mib_get_MultiLinkActivated(mac_vap->pst_mib_info));
}

#endif /* end of _PRE_WLAN_FEATURE_11BE */
