/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : mac_frame_11be.c
 * 创建日期   : 2023年9月18日
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "mac_resource.h"
#include "mac_ie.h"
#include "securec.h"
#include "mac_frame_common.h"
#include "mac_mib.h"
#include "mac_user.h"
#include "hmac_11ax.h"
#include "hisi_customize_wifi.h"
#include "mac_frame_11be.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_FRAME_11BE_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

/*****************************************************************************
  3 函数实现
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_11BE

uint8_t mac_forbid_encap_eht_ie(mac_vap_stru *mac_vap)
{
    wlan_mib_ieee802dot11_stru *mib_info = NULL;

    if (mac_vap == NULL || mac_vap->pst_mib_info == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_forbid_encap_eht_ie:: mac_vap->pst_mib_info is NULL!}");
        return OAL_TRUE;
    }

    mib_info = mac_vap->pst_mib_info;
    if ((mac_mib_get_EHTOptionImplemented(mib_info) != OAL_TRUE) ||
        (mac_vap_is_work_eht_protocol(mac_vap) == OAL_FALSE)) {
        return OAL_TRUE;
    }

    /* PF认证要求对于WEP、TKIP 加密方式不能关联在EHT模式 */
    if ((mac_is_wep_enabled(mac_vap) == OAL_TRUE) || (mac_is_tkip_only(mac_vap) == OAL_TRUE)) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

/*
 * 函 数 名   : mac_set_eht_ppe_thresholds_field_handle
 * 功能描述   : 填写eht ppe thresholds域
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void mac_set_eht_ppe_thresholds_field_handle(mac_vap_stru *mac_vap,
    mac_frame_eht_ppe_thresholds_stru *ppe_thresholds, uint8_t *ppetx_value, uint8_t ppetx_len,
    uint8_t *ppet8_value, uint8_t ppet8_len, uint8_t *ppet_cnt)
{
    uint8_t ru_bitmask = mac_mib_get_eht_RuIndexMask(mac_vap->pst_mib_info);
    uint8_t nss_idx;
    uint8_t ru_idx;

    /* 双流 */
    ppe_thresholds->nss = mac_mib_get_eht_Nss(mac_vap->pst_mib_info);
    /* 支持RU-242 */
    ppe_thresholds->ru_index0_mask = (ru_bitmask & MAC_EHT_PPET_RU_MASK);
    /* 支持RU-484 */
    ppe_thresholds->ru_index1_mask =  ((ru_bitmask >> MAC_EHT_PPET_RU1_INDEX) & MAC_EHT_PPET_RU_MASK);
    /* 支持RU-996 */
    ppe_thresholds->ru_index2_mask = ((ru_bitmask >> MAC_EHT_PPET_RU2_INDEX) & MAC_EHT_PPET_RU_MASK);
    /* 支持RU-2x996 */
    ppe_thresholds->ru_index3_mask = ((ru_bitmask >> MAC_EHT_PPET_RU3_INDEX) & MAC_EHT_PPET_RU_MASK);
    /* 支持2x996+484, 3x996, 3x996+484, 4x996 */
    ppe_thresholds->ru_index4_mask = ((ru_bitmask >> MAC_EHT_PPET_RU4_INDEX) & MAC_EHT_PPET_RU_MASK);
    if (ppetx_len != MAC_EHT_MAX_PPET_NUM || ppet8_len != MAC_EHT_MAX_PPET_NUM) {
        return;
    }

    for (nss_idx = 0; nss_idx < MAC_EHT_MAX_PPET_NSS + 1; nss_idx++) {
        for (ru_idx = 0; ru_idx < MAC_EHT_PPET_RU_BITMASK_FIELD_BIT; ru_idx++) {
            if (((ru_bitmask >> ru_idx) & MAC_EHT_PPET_RU_MASK) == 0) {
                continue;
            }

            if (*ppet_cnt >= MAC_EHT_MAX_PPET_NUM) {
                return;
            }

            ppetx_value[*ppet_cnt] =  mac_mib_get_eht_NssRuPpetx(mac_vap->pst_mib_info, nss_idx, ru_idx);
            ppet8_value[*ppet_cnt] =  mac_mib_get_eht_NssRuPpet8(mac_vap->pst_mib_info, nss_idx, ru_idx);
            *ppet_cnt = *ppet_cnt + 1;
        }
    }
}

/*
 * 函 数 名   : mac_set_eht_ppe_thresholds_field
 * 功能描述   : 填写eht ppe thresholds域
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC uint8_t mac_set_eht_ppe_thresholds_field(mac_vap_stru *mac_vap, uint8_t *buffer)
{
    mac_frame_eht_ppe_thresholds_stru *ppe_thresholds = NULL;
    uint8_t ppetx_value[MAC_EHT_MAX_PPET_NUM] = { 0 };
    uint8_t ppet8_value[MAC_EHT_MAX_PPET_NUM] = { 0 };
    uint8_t ppe_threshold_field_len = 0;
    uint8_t ppet_cnt = 0;

    ppe_thresholds = (mac_frame_eht_ppe_thresholds_stru *)buffer;
    memset_s(ppe_thresholds, MAC_EHT_PPE_THRESHOLDS_FIELD_LEN, 0, MAC_EHT_PPE_THRESHOLDS_FIELD_LEN);

    mac_set_eht_ppe_thresholds_field_handle(mac_vap, ppe_thresholds, ppetx_value,
        MAC_EHT_MAX_PPET_NUM, ppet8_value, MAC_EHT_MAX_PPET_NUM, &ppet_cnt);

    if (ppet_cnt >= 1) { /* 如果PPET数大于1，设置HE CAP->PPE Threshold field的第0个PPET值 */
        ppe_thresholds->ppetx_value0 = ppetx_value[0];  /* 第0个PPET值 */
        ppe_thresholds->ppet8_value0 = ppet8_value[0];
    }

    if (ppet_cnt >= 2) { /* 如果PPET数大于2，设置HE CAP->PPE Threshold field的第1个PPET值 */
        ppe_thresholds->ppetx_value1 = ppetx_value[1];    /* 第1个PPET值 */
        ppe_thresholds->ppet8_value1 = ppet8_value[1];
    }

    if (ppet_cnt >= 3) { /* 如果PPET数大于3，设置HE CAP->PPE Threshold field的第2个PPET值 */
        ppe_thresholds->ppetx_value2 = ppetx_value[2];    /* 第2个PPET值 */
        ppe_thresholds->ppet8_value2 = ppet8_value[2];
    }

    if (ppet_cnt >= 4) { /* 如果PPET数大于4，设置HE CAP->PPE Threshold field的第3个PPET值 */
        ppe_thresholds->ppetx_value3 = ppetx_value[3];    /* 第3个PPET值 */
        ppe_thresholds->ppet8_value3 = ppet8_value[3];
    }

    if (ppet_cnt >= 5) { /* 如果PPET数大于5，设置HE CAP->PPE Threshold field的第4个PPET值 */
        ppe_thresholds->ppetx_value4 = ppetx_value[4];    /* 第4个PPET值 */
        ppe_thresholds->ppet8_value4 = ppet8_value[4];
    }

    if (ppet_cnt >= 6) { /* 如果PPET数大于6，设置HE CAP->PPE Threshold field的第5个PPET值 */
        ppe_thresholds->ppetx_value5 = ppetx_value[5];    /* 第5个PPET值 */
        ppe_thresholds->ppet8_value5 = ppet8_value[5];
    }

    if (ppet_cnt >= 7) { /* 如果PPET数大于7，设置HE CAP->PPE Threshold field的第6个PPET值 */
        ppe_thresholds->ppetx_value6 = ppetx_value[6];    /* 第6个PPET值 */
        ppe_thresholds->ppet8_value6 = ppet8_value[6];
    }

    if (ppet_cnt >= 8) { /* 如果PPET数大于8，设置HE CAP->PPE Threshold field的第7个PPET值 */
        ppe_thresholds->ppetx_value7 = ppetx_value[7];    /* 第7个PPET值 */
        ppe_thresholds->ppet8_value7 = ppet8_value[7];
    }

    /*
    NSS subfield: 4 bit
    Ru Index Bitmask subfield: 5bit
    PPET16 + PPET8 subfield: 6bit
    NSS num * valid Ru num: ppet_cnt
    */
    ppe_threshold_field_len = (MAC_EHT_PPET_NSS_FIELD_BIT + MAC_EHT_PPET_RU_BITMASK_FIELD_BIT +
        (MAC_EHT_PPET_PPETX_FIELD_BIT + MAC_EHT_PPET_PPET8_FIELD_BIT) * ppet_cnt + 7) >> BIT_OFFSET_3; // 7: 字节对齐

    return ppe_threshold_field_len;
}

OAL_STATIC void mac_set_eht_mcs_20mhz_nss(uint8_t *buffer, uint8_t nss)
{
    mac_frame_eht_basic_mcs_map_20mhz_stru *mcs_20_only = (mac_frame_eht_basic_mcs_map_20mhz_stru *)buffer;
    memset_s(mcs_20_only, sizeof(mac_frame_eht_basic_mcs_map_20mhz_stru),
        0, sizeof(mac_frame_eht_basic_mcs_map_20mhz_stru));
    mcs_20_only->bit_rx_max_nss_eht_mcs0_7 = nss;
    mcs_20_only->bit_tx_max_nss_eht_mcs0_7 = nss;

    mcs_20_only->bit_rx_max_nss_eht_mcs8_9 = nss;
    mcs_20_only->bit_tx_max_nss_eht_mcs8_9 = nss;

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_1024qam_support == OAL_TRUE) {
        mcs_20_only->bit_rx_max_nss_eht_mcs10_11 = nss;
        mcs_20_only->bit_tx_max_nss_eht_mcs10_11 = nss;
    }

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_4096qam_support == OAL_TRUE) {
        mcs_20_only->bit_rx_max_nss_eht_mcs12_13 = nss;
        mcs_20_only->bit_tx_max_nss_eht_mcs12_13 = nss;
    }
}

OAL_STATIC void mac_set_eht_mcs_below_80mhz_nss(uint8_t *buffer, uint8_t nss)
{
    mac_frame_eht_basic_mcs_map_except_20mhz_stru *mcs_bw_below_80 =
        (mac_frame_eht_basic_mcs_map_except_20mhz_stru *)buffer;
    memset_s(mcs_bw_below_80, sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru),
        0, sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru));
    mcs_bw_below_80->bit_rx_max_nss_eht_mcs0_9 = nss;
    mcs_bw_below_80->bit_tx_max_nss_eht_mcs0_9 = nss;

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_1024qam_support == OAL_TRUE) {
        mcs_bw_below_80->bit_rx_max_nss_eht_mcs10_11 = nss;
        mcs_bw_below_80->bit_tx_max_nss_eht_mcs10_11 = nss;
    }

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_4096qam_support == OAL_TRUE) {
        mcs_bw_below_80->bit_rx_max_nss_eht_mcs12_13 = nss;
        mcs_bw_below_80->bit_tx_max_nss_eht_mcs12_13 = nss;
    }
}

OAL_STATIC void mac_set_eht_mcs_160mhz_nss(uint8_t *buffer, uint8_t nss)
{
    mac_frame_eht_basic_mcs_map_except_20mhz_stru *mcs_bw_equal_160 =
        (mac_frame_eht_basic_mcs_map_except_20mhz_stru *)buffer;
    memset_s(mcs_bw_equal_160, sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru),
        0, sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru));
    mcs_bw_equal_160->bit_rx_max_nss_eht_mcs0_9 = nss;
    mcs_bw_equal_160->bit_tx_max_nss_eht_mcs0_9 = nss;

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_1024qam_support == OAL_TRUE) {
        mcs_bw_equal_160->bit_rx_max_nss_eht_mcs10_11 = nss;
        mcs_bw_equal_160->bit_tx_max_nss_eht_mcs10_11 = nss;
    }

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_4096qam_support == OAL_TRUE) {
        mcs_bw_equal_160->bit_rx_max_nss_eht_mcs12_13 = nss;
        mcs_bw_equal_160->bit_tx_max_nss_eht_mcs12_13 = nss;
    }
}

OAL_STATIC void mac_set_eht_mcs_320mhz_nss(uint8_t *buffer, uint8_t nss)
{
    mac_frame_eht_basic_mcs_map_except_20mhz_stru *mcs_bw_equal_320 =
        (mac_frame_eht_basic_mcs_map_except_20mhz_stru *)buffer;
    memset_s(mcs_bw_equal_320, sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru),
        0, sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru));
    mcs_bw_equal_320->bit_rx_max_nss_eht_mcs0_9 = nss;
    mcs_bw_equal_320->bit_tx_max_nss_eht_mcs0_9 = nss;

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_1024qam_support == OAL_TRUE) {
        mcs_bw_equal_320->bit_rx_max_nss_eht_mcs10_11 = nss;
        mcs_bw_equal_320->bit_tx_max_nss_eht_mcs10_11 = nss;
    }

    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_4096qam_support == OAL_TRUE) {
        mcs_bw_equal_320->bit_rx_max_nss_eht_mcs12_13 = nss;
        mcs_bw_equal_320->bit_tx_max_nss_eht_mcs12_13 = nss;
    }
}

/*
 * 函 数 名   : mac_set_support_eht_mcs_and_nss_set_field
 * 功能描述   : 填写Supported EHT-MCS And NSS Set域
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC uint8_t mac_set_support_eht_mcs_and_nss_set_field(mac_vap_stru *mac_vap, uint8_t *buffer)
{
    uint8_t band = mac_vap->st_channel.en_band;
    uint8_t vap_mode = mac_vap->en_vap_mode;
    uint8_t channel_width = 0;
    uint8_t len = 0;
    uint8_t nss = 0;
    mac_device_stru *mac_dev = mac_res_get_dev(mac_vap->uc_device_id);
    wlan_nss_enum_uint8 nss_num_rx;
    wlan_nss_enum_uint8 nss_num_tx;
#ifdef _PRE_WLAN_FEATURE_11AX
    /* 获取HE PHY Capabilities Information的Supported Channel Width字段值 */
    channel_width = mac_get_he_channel_width(mac_vap);
#endif
    if (mac_dev == NULL) {
        return len;
    }
    /* EHT-MCS Map中nss为0表示不支持对应的MCS, 有效值从1开始 */
    mac_vap_ini_get_nss_num(mac_dev, &nss_num_rx, &nss_num_tx);
    nss = nss_num_tx + 1;

    /*
    5G频段：如果HE PHY Capabilities Information的Supported Channel Width的B1 B2 B3全为0
            则包含EHT-MCS Map (20 MHz-Only Non-AP STA)字段
    2G频段：如果HE PHY Capabilities Information的Supported Channel Width的B0为0
            则包含EHT-MCS Map (20 MHz-Only Non-AP STA)字段
    */
    if (vap_mode == WLAN_VAP_MODE_BSS_STA &&
        ((band == WLAN_BAND_2G &&  ((channel_width & BIT0) == 0)) ||
         (band == WLAN_BAND_5G && ((channel_width & (BIT1 | BIT2 | BIT3)) == 0)))) {
        mac_set_eht_mcs_20mhz_nss(buffer, nss);
        len += sizeof(mac_frame_eht_basic_mcs_map_20mhz_stru);
        buffer += len;
    }

    /*
    5G/6G频段：如果HE PHY Capabilities Information的Supported Channel Width的B1为1
               则包含EHT-MCS Map (BW≤80 MHz, Except 20 MHz-Only Non-AP STA)字段
    2G 频段  ：如果HE PHY Capabilities Information的Supported Channel Width的B0为1
               则包含EHT-MCS Map (BW≤80 MHz, Except 20 MHz-Only Non-AP STA)字段
    */
    if ((vap_mode == WLAN_VAP_MODE_BSS_AP) ||
        ((band == WLAN_BAND_2G && ((channel_width & BIT0) != 0)) ||
        ((band == WLAN_BAND_5G) && ((channel_width & BIT1) != 0)))) {
        mac_set_eht_mcs_below_80mhz_nss(buffer, nss);
        len += sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru);
        buffer += len;
    }

    /* 如果HE PHY Capabilities Information的Supported Channel Width的B2为1
       则包含EHT-MCS Map (BW = 160 MHz)字段 */
    if ((channel_width & BIT2) != 0) {
        mac_set_eht_mcs_160mhz_nss(buffer, nss);
        len += sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru);
        buffer += len;
    }

    /* 如果EHT PHY Capabilities Information中Support For 320MHz In 6GHz为1
       则包含EHT-MCS Map (BW = 320 MHz)字段 */
    if (mac_mib_get_EHTSupportFor320MHzImplemented((mac_vap->pst_mib_info))) {
        mac_set_eht_mcs_320mhz_nss(buffer, nss);
        len += sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru);
        buffer += len;
    }

    return len;
}

OAL_STATIC void mac_set_eht_phy_mcs15(
    mac_vap_stru *mac_vap, mac_frame_eht_phy_capabilities_stru *eht_phy_capinfo)
{
    uint8_t mcs15 = 0;

    /* B51-B54: Support Of MCS 15 */
    if (mac_mib_get_EHTMCS15For52p26and106p26MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE) {
        return;
    }

    /***************************************************************************
      1 支持， 0 不支持
      B51:                    是否支持MCS15在52+26-tone和106+26-tone的MRU
      B52: 支持80 MHz  带宽时，是否支持MCS15在484+242-tone的MRU
      B53: 支持160 MHz 带宽时，是否支持MCS15在996+484-tone和996+484+242-tone的MRU
      B54: 支持320 MHz 带宽时，是否支持MCS15在3x996-tone的MRU
    ****************************************************************************/
    mcs15 = 1;
    eht_phy_capinfo->bit_support_of_mcs15 = mcs15;
    if (mac_mib_get_EHTMCS15For484p242MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE) {
        return;
    }

    mcs15 |= BIT1;
    eht_phy_capinfo->bit_support_of_mcs15 = mcs15;
    if (mac_mib_get_EHTMCS15For996p484and996p484p242MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE) {
        return;
    }

    mcs15 |= BIT2;
    eht_phy_capinfo->bit_support_of_mcs15 = mcs15;
    if (mac_mib_get_EHTMCS15For3x996MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE) {
        return;
    }

    mcs15 |= BIT3;
    eht_phy_capinfo->bit_support_of_mcs15 = mcs15;
}

OAL_STATIC void mac_set_eht_phy_capabilities_info_part2(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *eht_phy_capinfo)
{
    uint8_t num_ltf_mu = 0;
    uint8_t max_num_ltf = 0;
    uint8_t num_ltf_su = 0;

    /* B44-B45: Common Nominal Packet Padding */
    if (eht_phy_capinfo->ppe_thresholds_present == 0) {
        eht_phy_capinfo->common_nominal_pkt_padding = MAC_EHT_NOMINAL_PKT_PAD_16US_20US;
    }

    /* B46-B50: Maximum Number Of Supported EHT-LTFs */
    if (mac_mib_get_EHTExtraLTFsImplemented(mac_vap->pst_mib_info)) {
        /* B46设置为1表示支持，0表示不支持 */
        max_num_ltf = 1;
        num_ltf_su = mac_mib_get_EHTMaxNumberOfSupportedEHTLTFsForSU(mac_vap->pst_mib_info);
        if (num_ltf_su == BYTE_OFFSET_8) {
            /* B47-B48的值设置为0表示最多支持4个EHT-LTF，设置为1表示最多支持8个EHT-LTF，其他值保留 */
            max_num_ltf |= BIT1;
        }

        num_ltf_mu = mac_mib_get_EHTMaxNumberOfSupportedEHTLTFsForMUandNDP(mac_vap->pst_mib_info);
        if (num_ltf_mu == BYTE_OFFSET_8) {
            /* B49-B50的值设置为0表示最多支持4个EHT-LTF，设置为1表示最多支持8个EHT-LTF，其他值保留 */
            max_num_ltf |= BIT3;
        }

        eht_phy_capinfo->supported_eht_ltfs_max_number = max_num_ltf;
    }

    mac_set_eht_phy_mcs15(mac_vap, eht_phy_capinfo);
}

OAL_STATIC void mac_set_eht_phy_capabilities_info_part3(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *eht_phy_capinfo)
{
    /* B55: Support Of EHT DUP (MCS 14) In 6 GHz: 是否支持6 GHz频段下的EHT DUP */
    eht_phy_capinfo->bit_eht_dup_in_6ghz = mac_mib_get_EHTDupImplemented(mac_vap->pst_mib_info);

    /* B56: Support For 20 MHz Operating STA Receiving NDP With Wider Bandwidth: 工作在20MHz下，
       是否能接收带宽大于20MHz的NDP PPDU */
    eht_phy_capinfo->support_of_20mhz_sta_recv_ndp_over_20mhz =
        mac_mib_get_EHT20MHzOperatingSTARxNDPwithWiderBWImplemented(mac_vap->pst_mib_info);

    /* B57: Non-OFDMA UL MU-MIMO (BW≤80 MHz): 是否支持在带宽小于或等于80MHz时，
       使用non-OFDMA UL MU-MIMO接收或发送的EHT TB PPDU */
    eht_phy_capinfo->non_ofdma_ul_mu_mimo_below_80mhz =
        mac_mib_get_EHTNonOFDMAULMUMIMOLessThanOrEqualto80Implemented(mac_vap->pst_mib_info);

    /* B58: Non-OFDMA UL MU-MIMO (BW=160 MHz): 是否支持在带宽等于160MHz时，
       使用non-OFDMA UL MU-MIMO 接收或发送EHT TB PPDU */
    eht_phy_capinfo->non_ofdma_ul_mu_mimo_equal_160mhz =
        mac_mib_get_EHTNonOFDMAULMUMIMOEqualto160Implemented(mac_vap->pst_mib_info);

    /* B59: Non-OFDMA UL MU-MIMO (BW=320 MHz): 是否支持在带宽等于320MHz时，
       使用non-OFDMA UL MU-MIMO 接收或发送EHT TB PPDU */
    eht_phy_capinfo->non_ofdma_ul_mu_mimo_equal_320mhz =
        mac_mib_get_EHTNonOFDMAULMUMIMOEqualto320Implemented(mac_vap->pst_mib_info);

    /* B60: MU Beamformer (BW≤80 MHz): 在带宽小于等于80MHz时，是否支持non-OFDMA DL MU-MIMO */
    eht_phy_capinfo->mu_beamformer_below_80mhz =
        mac_mib_get_EHTMUBeamformerLessThanOrEqualTo80Implemented(mac_vap->pst_mib_info);

    /* B61: MU Beamformer (BW=160 MHz): 在带宽等于160MHz时，是否支持non-OFDMA DL MU-MIMO */
    eht_phy_capinfo->mu_beamformer_equal_160mhz =
        mac_mib_get_EHTMUBeamformerEqualTo160Implemented(mac_vap->pst_mib_info);

    /* B62: MU Beamformer (BW=320 MHz): 在带宽等于320MHz时，是否支持non-OFDMA DL MU-MIMO */
    eht_phy_capinfo->mu_beamformer_equal_320mhz = OAL_FALSE;

    /* B63: TB Sounding Feedback Rate Limit: EHT TB PPDU是否有最大速率限制 */
    eht_phy_capinfo->tb_sounding_feadback_rate_limit = 0;

    eht_phy_capinfo->rx_1024qam_in_wider_bandwidth_dl_ofdma_support =
        mac_mib_get_Rx1024QamInWiderBandwidthDlOfdmaSupport(mac_vap->pst_mib_info);

    eht_phy_capinfo->rx_4096qam_in_wider_bandwidth_dl_ofdma_support =
        mac_mib_get_Rx4096QamInWiderBandwidthDlOfdmaSupport(mac_vap->pst_mib_info);
}
#ifdef _PRE_WLAN_FEATURE_160M
static void mac_set_eht_phy_cap_4hltf_08gi_160m(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *eht_phy_cap)
{
    if (((g_cust_cap.enhanced_phy_cap_mask & CUSTOM_HAL_PHY_CAP_RX_DELAY_OPT_MASK) != 0) &&
        ((mac_vap->st_channel.en_bandwidth >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) &&
        (mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS))) {
        eht_phy_cap->bit_eht_mu_ppdu_4x_eht_ltf_08us_gi = 0;
    }
}
#endif
OAL_STATIC void mac_set_eht_phy_capabilities_info_part1(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *eht_phy_capinfo)
{
    /* B25: Ng = 16 SU Feedback: 是否支持SU反馈的EHT Compressed Beamforming Report field中的16个子载波分组 */
    eht_phy_capinfo->ng16_su_feedback = mac_mib_get_EHTNG16SUFeedbackImplemented(mac_vap->pst_mib_info);

    /* B26: Ng = 16 MU Feedback: 是否支持MU反馈的EHT Compressed Beamforming Report field中的16个子载波分组 */
    eht_phy_capinfo->ng16_mu_feedback = mac_mib_get_EHTNG16MUFeedbackImplemented(mac_vap->pst_mib_info);

    /* B27: Codebook Size (Φ, ψ) = {4, 2} SU Feedback: 是否支持codebook size (Φ, ψ) = {4, 2} */
    eht_phy_capinfo->codebook_42_su_feedback =
        mac_mib_get_EHTCodebookSizePhi4Psi2SUFeedbackImplemented(mac_vap->pst_mib_info);

    /* B28: Codebook Size (Φ, ψ) = {7, 5} MU Feedback: 是否支持codebook size (Φ, ψ) = {7, 5} */
    eht_phy_capinfo->codebook_75_mu_feedback =
        mac_mib_get_EHTCodebookSizePhi7Psi5MUFeedbackImplemented(mac_vap->pst_mib_info);

    /* B29: Triggered SU Beamforming Feedback: 是否支持接收或发送部分和全部带宽的SU beamforming反馈 */
    eht_phy_capinfo->triggered_su_beamforming_feedback =
        mac_mib_get_EHTTriggeredSUBeamformingFeedbackImplemented(mac_vap->pst_mib_info);

    /* B30: Triggered MU Beamforming Partial BW Feedback: 是否支持接收或发送部分的MU beamforming反馈 */
    eht_phy_capinfo->triggered_mu_beamforming_partial_bw_feedback =
        mac_mib_get_EHTTriggeredMUBeamformingPartialBWFeedbackImplemented(mac_vap->pst_mib_info);

    /* B31: Triggered CQI Feedback: 是否支持部分和全部带宽的CQI反馈 */
    eht_phy_capinfo->triggered_cqi_feedback =
        mac_mib_get_EHTTriggeredCQIFeedbackImplemented(mac_vap->pst_mib_info);

    /* B32: Partial Bandwidth DL MU-MIMO: 是否支持DL MU-MIMO和OFDMA组合模式 */
    eht_phy_capinfo->partial_bandwidth_dl_mu_mimo =
        mac_mib_get_EHTPartialBWDLMUMIMOImplemented(mac_vap->pst_mib_info);

    /* B33: EHT PSR-based SR Support: 是否支持EHT PSR-based SR操作 */
    eht_phy_capinfo->eht_psr_based_sr_support = mac_mib_get_EHTPSRBasedSRImplemented(mac_vap->pst_mib_info);

    /* B34: Power Boost Factor Support: 是否支持接收RU的功率提升系数在[0.5, 2]范围内的EHT MU PPDU */
    eht_phy_capinfo->power_boost_factor_support =
        mac_mib_get_EHTPowerBoostFactorImplemented(mac_vap->pst_mib_info);

    /* B35: EHT MU PPDU With 4x EHT-LTF And 0.8 μs GI：是否支持接收4x EHT-LTF和0.8 μs GI的EHT MU PPDU */
    eht_phy_capinfo->bit_eht_mu_ppdu_4x_eht_ltf_08us_gi =
        mac_mib_get_EHTMUPPDUwith4xEHTLTFand0point8usecGIImplemented(mac_vap->pst_mib_info);
        // 1107c 160M需要关闭4*hltf+0.8gi
#ifdef _PRE_WLAN_FEATURE_160M
    mac_set_eht_phy_cap_4hltf_08gi_160m(mac_vap, eht_phy_capinfo);
#endif

    /* B36-B39: Max Nc：表示EHT compressed beamforming/CQI report支持的最大Nc值 */
    if (eht_phy_capinfo->su_beamformee) {
        eht_phy_capinfo->max_nc = mac_mib_get_EHTMaxNc(mac_vap->pst_mib_info);
    }

    /* B40: Non-Triggered CQI Feedback：是否支持接收/发送全带宽的non-triggered CQI反馈 */
    eht_phy_capinfo->non_triggered_cqi_feedback =
        mac_mib_get_EHTNonTriggeredCQIFeedbackImplemented(mac_vap->pst_mib_info);

    /* B41: Tx 1024-QAM And 4096-QAM < 242-tone RU Support：是否支持使用1024-QAM 和
       4096-QAM并且小于242-tone RU发送EHT TB PPDU */
    eht_phy_capinfo->tx_1024qam_4096qam_below_242_tone_ru_support =
        mac_mib_get_EHTTx1024QAMand4096QAMLessThan242ToneRUImplemented(mac_vap->pst_mib_info);

    /* B42: Rx 1024-QAM And 4096-QAM < 242-tone RU Support：是否支持接收使用1024-QAM 和
       4096-QAM并且小于242-tone RU发送EHT TB PPDU */
    eht_phy_capinfo->rx_1024qam_4096qam_below_242_tone_ru_support =
        mac_mib_get_EHTRx1024QAMand4096QAMLessThan242ToneRUImplemented(mac_vap->pst_mib_info);

    /* B43: PPE Thresholds Present：PPE Thresholds是否存在 */
    eht_phy_capinfo->ppe_thresholds_present = mac_mib_get_EHTPPEThresholdsRequired(mac_vap->pst_mib_info);
}

/*
 * 函 数 名   : mac_set_eht_phy_capabilities_info
 * 功能描述   : 填写eht phy capabilities info域
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void mac_set_eht_phy_capabilities_info(mac_vap_stru *mac_vap, uint8_t *buffer)
{
    mac_frame_eht_phy_capabilities_stru *eht_phy_capinfo = (mac_frame_eht_phy_capabilities_stru *)buffer;

    memset_s(eht_phy_capinfo, sizeof(mac_frame_eht_phy_capabilities_stru), 0,
        sizeof(mac_frame_eht_phy_capabilities_stru));

    /* B1: Support For 320 MHz In 6 GHz: 是否支持6 GHz频段的320 MHz的带宽 */
    eht_phy_capinfo->bit_320mhz_6ghz_support = mac_mib_get_EHTSupportFor320MHzImplemented(mac_vap->pst_mib_info);

    /* B2: Support for 242-tone RU In BW Wider Than 20 MHz: 是否支持6 GHz频段的320 MHz的带宽 */
    eht_phy_capinfo->support_of_242_tone_ru_over_20mhz =
        mac_mib_get_EHTSupportFor242ToneRUInBWWiderThan20Implemented(mac_vap->pst_mib_info);

    /* B3: NDP With 4x EHT-LTF And 3.2 μs GI: 对于beamformee，是否支持接收4x EHT-LTF and 3.2 μs GI的EHT sounding NDP */
    eht_phy_capinfo->bit_ndp_4x_eht_ltf_3p2us_gi =
        mac_mib_get_EHTNDPwith4xEHTLTFand3point2GIImplemented(mac_vap->pst_mib_info);

    /* B4: Partial Bandwidth UL MU-MIMO: 是否支持UL MU-MIMO和OFDMA组合模式 */
    eht_phy_capinfo->partial_bandwidth_ul_mu_mimo =
        mac_mib_get_EHTPartialBWULMUMIMOImplemented(mac_vap->pst_mib_info);

    /* B5: SU Beamformer: 是否支持SU Beamformer */
    eht_phy_capinfo->su_beamformer = mac_mib_get_EHTSUBeamformerImplemented(mac_vap->pst_mib_info);

    /* B6: SU Beamformee: 是否支持SU Beamformee */
    eht_phy_capinfo->su_beamformee = mac_mib_get_EHTSUBeamformeeImplemented(mac_vap->pst_mib_info);

    if (eht_phy_capinfo->su_beamformee) {
        /* B7-B9: Beamformee SS (≤ 80 MHz): 当带宽小于或等于80 MHz时，表示STA能接收EHT sounding NDP的最大空间流数 */
        eht_phy_capinfo->beamformee_ss_below_80mhz =
            oal_max(mac_mib_get_EHTBeamformeeSSLessThanOrEqualTo80(mac_vap->pst_mib_info), 3); /* 3：认证最低要求 */
        if (mac_vap->st_channel.en_band != WLAN_BAND_2G) {
            /* B10-B12: Beamformee SS (= 160 MHz): 当带宽等于160 MHz时，表示STA能接收EHT sounding NDP的最大空间流数 */
            if (g_wlan_spec_cfg->max_5g_bw >= WLAN_BANDWIDTH_160) {
                eht_phy_capinfo->beamformee_ss_equal_160mhz =
                    oal_max(mac_mib_get_EHTBeamformeeSSEqualTo160(mac_vap->pst_mib_info), 3); /* 3：认证最低要求 */
            } else {
                eht_phy_capinfo->beamformee_ss_equal_160mhz = 0;
            }
            /* B13-B15: Beamformee SS (= 320 MHz): 当带宽等于320 MHz时，表示STA能接收EHT sounding NDP的最大空间流数 */
            eht_phy_capinfo->beamformee_ss_equal_320mhz = 0; /* 07C不支持320M */
        }
    }

    if (eht_phy_capinfo->su_beamformer) {
        /* B16-B18: Number Of Sounding Dimensions (≤ 80 MHz): 当带宽小于或等于80 MHz时，
           表示EHT sounding NDP的TXVECTOR参数NUM_STS的最大值 */
        eht_phy_capinfo->sounding_dimensions_num_below_80mhz =
            mac_mib_get_EHTNumberSoundingDimensionsLessThanOrEqualTo80(mac_vap->pst_mib_info);

        if (mac_vap->st_channel.en_band != WLAN_BAND_2G) {
            /* B19-B21: Number Of Sounding Dimensions (= 160 MHz): 当带宽等于160 MHz时，
                表示EHT sounding NDP的TXVECTOR参数NUM_STS的最大值 */
            eht_phy_capinfo->sounding_dimensions_num_equal_160mhz =
                mac_mib_get_EHTNumberSoundingDimensionsEqualTo160(mac_vap->pst_mib_info);

            /* B22-B24: Number Of Sounding Dimensions (= 320 MHz): 当带宽等于320 MHz时，
                表示EHT sounding NDP的TXVECTOR参数NUM_STS的最大值 */
            eht_phy_capinfo->sounding_dimensions_num_equal_320mhz =
                mac_mib_get_EHTNumberSoundingDimensionsEqualTo320(mac_vap->pst_mib_info);
        }
    }

    /* eht cap info字段能力位较多，拆分为多个part */
    mac_set_eht_phy_capabilities_info_part1(mac_vap, eht_phy_capinfo);
    mac_set_eht_phy_capabilities_info_part2(mac_vap, eht_phy_capinfo);
    mac_set_eht_phy_capabilities_info_part3(mac_vap, eht_phy_capinfo);
}

/*
 * 函 数 名   : mac_set_eht_mac_capabilities_info
 * 功能描述   : 填写eht mac capabilities info域
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void mac_set_eht_mac_capabilities_info(mac_vap_stru *mac_vap, uint8_t *buffer)
{
    mac_frame_eht_mac_capabilities_stru *eht_mac_capinfo = (mac_frame_eht_mac_capabilities_stru *)buffer;

    memset_s(eht_mac_capinfo, sizeof(mac_frame_eht_mac_capabilities_stru), 0,
        sizeof(mac_frame_eht_mac_capabilities_stru));

    /*********************************** EHT MAC Capabilities info**********************************
     |---------------------------------------------------------------------------------------------|
     | NSEP      | EHT OM   | Triggered | Triggered | Restricted | SCS Traffic | Maximum |         |
     | Priority  | Control  | TXOP      | TXOP      | TWT        | Description | MPDU    |         |
     | Access    | Support  | Sharing   | Sharing   | Support    | Support     | Length  | Reserved|
     | Supported |          | Mode 1    | Mode 2    |            |             |         |         |
     |           |          | Support   | Support   |            |             |         |         |
     |---------------------------------------------------------------------------------------------|
     | B0        | B1       | B2        | B3        | B4         | B5          | B6 B7   | 8 BIT   |
     |---------------------------------------------------------------------------------------------|

    ************************************************************************************************/
    /* B0: NSEP Priority Access Supported: 是否支持NSEP优先接入 */
    eht_mac_capinfo->epcs_priority_access_sup = mac_mib_get_EHTNSEPPriorityAccessActivated(mac_vap->pst_mib_info);

    /* B1: EHT OM Control Support: 是否支持接收携带EHT OM Control子字段的帧 */
    eht_mac_capinfo->eht_om_control_support = mac_mib_get_EHTOmControlSupport(mac_vap->pst_mib_info);

    /* B2: Triggered TXOP Sharing Mode 1 Support: 是否支持传输或回应携带Triggered TXOP Sharing Mode
       为1的TXOP sharing trigger帧 */
    eht_mac_capinfo->txop_sharing_mode1_support =
        mac_mib_get_EHTTXOPSharingTFOptionImplemented(mac_vap->pst_mib_info);

    /* B3: Triggered TXOP Sharing Mode 2 Support: 是否支持传输或回应携带Triggered TXOP Sharing Mode
       为2的TXOP sharing trigger帧 */
    eht_mac_capinfo->txop_sharing_mode2_support =
        mac_mib_get_EHTTXOPSharingTFOptionImplemented(mac_vap->pst_mib_info);

    /* B4: Restricted TWT Support: 是否支持restricted TWT */
    eht_mac_capinfo->restricted_twt_support = mac_mib_get_RestrictedTWTOptionImplemented(mac_vap->pst_mib_info);

    /* B5: SCS Traffic Description Support: 是否支持传输和接收包含QoS Characteristics子元素的SCS帧 */
    eht_mac_capinfo->scs_traffic_dscr_support = mac_mib_get_SCSActivated(mac_vap->pst_mib_info);

    /* B6: Maximum MPDU Length: STA能接收的最大MPDU长度。0代表3895字节，1代表7991字节，
       2代表11454字节。5G和6G频段该字段为缺省字段值为3 */
    if (mac_vap->st_channel.en_band == WLAN_BAND_2G) {
        eht_mac_capinfo->max_mpdu_length = mac_mib_get_maxmpdu_length(mac_vap);
    } else {
        eht_mac_capinfo->max_mpdu_length = WLAN_MIB_VHT_MPDU_LEN_BUTT;
    }

    /* B8: Maximum A-MPDU Length Exponent Extension */
    eht_mac_capinfo->bit_max_ampdu_length_exponent = mac_mib_get_EHTMaxAmpduLengthExponent(mac_vap->pst_mib_info);

    /* B9: EHT TRS Support */
    eht_mac_capinfo->eht_trs_support = mac_mib_get_EHTTrsSupport(mac_vap->pst_mib_info);

    /* B10: TXOP Return Support In TXOP Sharing Mode 2 */
    eht_mac_capinfo->txop_return_support_in_txop_sharing_mode2 =
        mac_mib_get_TxopReturnSupportInTxopSharingMode2(mac_vap->pst_mib_info);
    /* b11:HE CAP支持htc时，sta是否支持发送/AP是否支持结束HTC域中携带两个BQR Control */
    eht_mac_capinfo->support_of_2bqrs = OAL_FALSE;
    /* b12-b13 EHT MFB */
    eht_mac_capinfo->eht_link_adaption_support = OAL_FALSE;
}

/*
 * 函 数 名   : mac_fill_eht_capabilities_ie
 * 功能描述   : 填充eht capabilities 信息
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void mac_fill_eht_capabilities_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len)
{
    uint8_t support_mcs_nss_len = 0;
    uint8_t ppe_thresholds_len = 0;
    mac_frame_eht_phy_capabilities_stru *eht_phy_capinfo = NULL;
    mac_frame_eht_capabilities_ie_stru *eht_cap_ie = NULL;

    if (oal_unlikely(oal_any_null_ptr2(mac_vap, mac_vap->pst_mib_info))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_fill_eht_capabilities_ie:: mac_vap or pst_mib_info is NULL!}");
        return;
    }

    /* Draft P802.11be_D1.3 */
    /* 9.4.2.313 EHT Capabilities element */
    /****************************************************************
    -----------------------------------------------------------------
    |EID |Length |EID Extension|EHT MAC Capa. Info |EHT PHY Capa. Info|
    -----------------------------------------------------------------
    |1   |1      |1            |2                 |8               |
    -----------------------------------------------------------------
    |Supported EHT-MCS And NSS Set|EHT PPE Thresholds(Optional)          |
    -----------------------------------------------------------------
    |Variable                 |Variable                          |
    -----------------------------------------------------------------
    ****************************************************************/

    eht_cap_ie = (mac_frame_eht_capabilities_ie_stru *)buffer;

    eht_cap_ie->eht_cap_ie_hdr.element_id = MAC_EID_EHT;
    eht_cap_ie->eht_cap_ie_hdr.element_id_ext = MAC_EID_EXT_EHT_CAP;

    *ie_len += MAC_IE_EXT_HDR_LEN;
    buffer += MAC_IE_EXT_HDR_LEN;

    /* 填充EHT mac capabilities information域信息 */
    mac_set_eht_mac_capabilities_info(mac_vap, buffer);
    *ie_len += MAC_EHT_MAC_CAP_LEN;
    buffer += MAC_EHT_MAC_CAP_LEN;

    /* 填充EHT PHY Capabilities Information域信息 */
    eht_phy_capinfo = (mac_frame_eht_phy_capabilities_stru *)buffer;
    mac_set_eht_phy_capabilities_info(mac_vap, buffer);
    buffer += MAC_EHT_PHY_CAP_LEN;
    *ie_len += MAC_EHT_PHY_CAP_LEN;

    /* 填充Supported EHT-MCS And NSS Set域信息 */
    support_mcs_nss_len = mac_set_support_eht_mcs_and_nss_set_field(mac_vap, buffer);
    buffer += support_mcs_nss_len;
    *ie_len += support_mcs_nss_len;

    /* 填充EHT PPE Thresholds field域信息 */
    if (eht_phy_capinfo->ppe_thresholds_present == 1) {
        ppe_thresholds_len = mac_set_eht_ppe_thresholds_field(mac_vap, buffer);
        *ie_len += ppe_thresholds_len;
    }

    eht_cap_ie->eht_cap_ie_hdr.length = *ie_len - MAC_IE_HDR_LEN;
}


/*
 * 函 数 名   : mac_set_eht_capabilities_ie
 * 功能描述   : 填充eht capabilities信息
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
void mac_set_eht_capabilities_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len)
{
    if (oal_unlikely(oal_any_null_ptr3(mac_vap, buffer, ie_len))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_he_capabilities_ie::input NULL.}");
        return;
    }

    if (mac_forbid_encap_eht_ie(mac_vap) == OAL_TRUE) {
        return;
    }

    *ie_len = 0;
    mac_fill_eht_capabilities_ie(mac_vap, buffer, ie_len);
}


OAL_STATIC uint8_t mac_set_eht_disabled_subchannel_bitmap(mac_vap_stru *mac_vap, uint8_t *buffer)
{
    memset_s(buffer, DISABLED_SUBCHANNEL_BITMAP_LEN, 0, DISABLED_SUBCHANNEL_BITMAP_LEN);
    return DISABLED_SUBCHANNEL_BITMAP_LEN;
}

OAL_STATIC uint8_t mac_set_eht_oper_info(mac_vap_stru *mac_vap, uint8_t *buffer,
    eht_operation_parameters_stru *eht_oper_parameters)
{
    eht_operation_information_stru *eht_oper_info = (eht_operation_information_stru *)buffer;
    uint8_t band_width = mac_vap->st_channel.en_bandwidth;
    uint8_t len = EHT_OPERATION_INFO_MIN_LEN;

    memset_s(eht_oper_info, sizeof(eht_operation_information_stru), 0, sizeof(eht_operation_information_stru));
    eht_oper_info->channel_width = WLAN_EHT_CHANNEL_WIDTH_20;

    if ((band_width >= WLAN_BAND_WIDTH_40PLUS) && (band_width <= WLAN_BAND_WIDTH_40MINUS)) {
        eht_oper_info->channel_width = WLAN_EHT_CHANNEL_WIDTH_40;
    } else if ((band_width >= WLAN_BAND_WIDTH_80PLUSPLUS) && (band_width <= WLAN_BAND_WIDTH_80MINUSMINUS)) {
        eht_oper_info->channel_width = WLAN_EHT_CHANNEL_WIDTH_80;
#ifdef _PRE_WLAN_FEATURE_160M
    } else if ((band_width >= WLAN_BAND_WIDTH_160PLUSPLUSPLUS) && (band_width >= WLAN_BAND_WIDTH_160MINUSMINUSMINUS)) {
        eht_oper_info->channel_width = WLAN_EHT_CHANNEL_WIDTH_160;
#endif
    }

    if (band_width <= WLAN_BAND_WIDTH_80MINUSMINUS) {
        mac_set_ccfs_below_80m(&mac_vap->st_channel, band_width, &eht_oper_info->ccfs0);
#ifdef _PRE_WLAN_FEATURE_160M
    } else if (band_width <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS) {
        mac_set_ccfs_160m(&mac_vap->st_channel, band_width, &eht_oper_info->ccfs0, &eht_oper_info->ccfs1);
#endif
    }

    if (eht_oper_parameters->disabled_subchannel_bitmap_present) {
        buffer += EHT_OPERATION_INFO_MIN_LEN;
        mac_set_eht_disabled_subchannel_bitmap(mac_vap, buffer);
        len += DISABLED_SUBCHANNEL_BITMAP_LEN;
    }

    return len;
}

OAL_STATIC uint8_t mac_set_eht_operation_params(mac_vap_stru *mac_vap, uint8_t *buffer)
{
    eht_operation_parameters_stru *eht_oper_parameters = (eht_operation_parameters_stru *)buffer;

    memset_s(eht_oper_parameters, EHT_OPERATION_PARAMETERS_LEN, 0, EHT_OPERATION_PARAMETERS_LEN);
    eht_oper_parameters->eht_operation_information_present = 0;
    eht_oper_parameters->disabled_subchannel_bitmap_present = 0;
    eht_oper_parameters->eht_default_pe_duration = 0;
    eht_oper_parameters->group_addressed_bu_indication_limit = 0;
    eht_oper_parameters->group_addressed_bu_indication_exponent = 0;

    return EHT_OPERATION_PARAMETERS_LEN;
}

OAL_STATIC uint8_t mac_set_basic_eht_mcs_and_nss_set(mac_vap_stru *mac_vap, uint8_t *buffer)
{
    mac_frame_eht_basic_mcs_map_20mhz_stru *basic_eht_mcs_nss_set = (mac_frame_eht_basic_mcs_map_20mhz_stru *)buffer;
    memset_s(basic_eht_mcs_nss_set, BASIC_EHT_MCS_AND_NSS_SET_LEN, 0, BASIC_EHT_MCS_AND_NSS_SET_LEN);
    basic_eht_mcs_nss_set->bit_rx_max_nss_eht_mcs0_7 = 1;
    basic_eht_mcs_nss_set->bit_tx_max_nss_eht_mcs0_7 = 1;

    return BASIC_EHT_MCS_AND_NSS_SET_LEN;
}

/*
 * 函 数 名   : mac_fill_eht_operation_ie
 * 功能描述   : 填充eht operation 信息
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void mac_fill_eht_operation_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len)
{
    uint8_t                         eht_oper_info_len;
    uint8_t                         *orig_buffer = buffer;
    eht_operation_parameters_stru   *eht_oper_parameters = NULL;
    mac_frame_eht_operation_ie_stru *eht_oper_ie = NULL;

    if (oal_unlikely(oal_any_null_ptr2(mac_vap, mac_vap->pst_mib_info))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_fill_eht_capabilities_ie:: mac_vap or pst_mib_info is NULL!}");
        return;
    }

    /* Draft P802.11be_D2.0 */
    /* 9.4.2.311 EHT Operation element */
    /**************************************************************************
    ---------------------------------------------------------------------------
    |EID |Length |EID Extension|EHT Operation |Basic EHT-MCS |EHT Operation   |
    |    |       |             |Parameters    |And Nss Set   |Information     |
    ---------------------------------------------------------------------------
    |1   |1      |1            |1             |4             |0 or 3 or 5     |
    --------------------------------------------------------------------------|
    ***************************************************************************/

    eht_oper_ie = (mac_frame_eht_operation_ie_stru *)buffer;

    eht_oper_ie->eht_oper_ie_hdr.element_id = MAC_EID_EHT;
    eht_oper_ie->eht_oper_ie_hdr.element_id_ext = MAC_EID_EXT_EHT_OPER;
    buffer += MAC_IE_EXT_HDR_LEN;

    /* 填充EHT Operation Parameters */
    eht_oper_parameters = (eht_operation_parameters_stru *)buffer;
    mac_set_eht_operation_params(mac_vap, buffer);
    buffer += EHT_OPERATION_PARAMETERS_LEN;

    /* 填充Basic EHT-MCS And Nss Set */
    mac_set_basic_eht_mcs_and_nss_set(mac_vap, buffer);
    buffer += BASIC_EHT_MCS_AND_NSS_SET_LEN;

    /* 填充EHT Operation Information */
    if (eht_oper_parameters->eht_operation_information_present) {
        eht_oper_info_len = mac_set_eht_oper_info(mac_vap, buffer, eht_oper_parameters);
        buffer += eht_oper_info_len;
    }

    *ie_len = buffer - orig_buffer;
    eht_oper_ie->eht_oper_ie_hdr.length = *ie_len - MAC_IE_HDR_LEN;
}

/*
 * 函 数 名   : mac_set_eht_operation_ie
 * 功能描述   : 填充eht operation 信息
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
void mac_set_eht_operation_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len)
{
    if (oal_unlikely(oal_any_null_ptr3(mac_vap, buffer, ie_len))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_eht_operation_ie::input NULL.}");
        return;
    }

    if (mac_forbid_encap_eht_ie(mac_vap) == OAL_TRUE) {
        return;
    }

    *ie_len = 0;
    mac_fill_eht_operation_ie(mac_vap, buffer, ie_len);
}

/*
 * 函 数 名   : mac_set_eht_ie
 * 功能描述   : 封装BE相关IE
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
uint16_t mac_set_eht_ie(mac_vap_stru *mac_vap, uint8_t *origin_buf, wlan_frame_mgmt_subtype_enum mgmt_subtype)
{
    uint8_t  ie_len = 0;
    uint16_t total_ie_len;
    uint8_t  *beacon_buf = NULL;

    beacon_buf = origin_buf;
    switch (mgmt_subtype) {
        case WLAN_BEACON:
        case WLAN_PROBE_RSP:
        case WLAN_ASSOC_RSP:
        case WLAN_REASSOC_RSP:
            /* 填充eht capabilities信息 */
            mac_set_eht_capabilities_ie(mac_vap, beacon_buf, &ie_len);
            beacon_buf += ie_len;

            /* 填充eht operation信息 */
            mac_set_eht_operation_ie(mac_vap, beacon_buf, &ie_len);
            beacon_buf += ie_len;
            break;

        case WLAN_PROBE_REQ:
        case WLAN_ASSOC_REQ:
        case WLAN_REASSOC_REQ:
             /* 填充eht capabilities信息 */
            mac_set_eht_capabilities_ie(mac_vap, beacon_buf, &ie_len);
            beacon_buf += ie_len;
            break;

        default:
            break;
        }

    total_ie_len = (uint16_t)(beacon_buf - origin_buf);
    return total_ie_len;
}

OAL_STATIC void mac_eht_mcs_info_print(mac_frame_eht_basic_mcs_map_except_20mhz_stru *eht_mcs_nss_map)
{
    oam_warning_log4(0, OAM_SF_ANY,
        "{mac_eht_mcs_info_print::rx_mcs0_9[0x%x] tx_mcs0_9[0x%x] rx_mcs10_11[0x%x] tx_mcs0_9[0x%x]}",
        eht_mcs_nss_map->bit_rx_max_nss_eht_mcs0_9, eht_mcs_nss_map->bit_tx_max_nss_eht_mcs0_9,
        eht_mcs_nss_map->bit_rx_max_nss_eht_mcs10_11, eht_mcs_nss_map->bit_tx_max_nss_eht_mcs10_11);
    oam_warning_log2(0, OAM_SF_ANY,
        "{mac_eht_mcs_info_print::rx_mcs12_13[0x%x] tx_mcs12_13[0x%x]}",
        eht_mcs_nss_map->bit_rx_max_nss_eht_mcs12_13, eht_mcs_nss_map->bit_tx_max_nss_eht_mcs12_13);
}

OAL_STATIC uint32_t mac_parse_eht_mcs_20mhz_nss(uint8_t band, uint8_t he_channel_width_set,
    mac_frame_eht_capabilities_ie_stru *eht_cap, uint8_t *mcs_nss, uint8_t *remain_len, uint8_t *size)
{
    uint8_t size_20mhz = sizeof(mac_frame_eht_basic_mcs_map_20mhz_stru);

    if ((band == WLAN_BAND_2G && ((he_channel_width_set & BIT0) == 0)) ||
        (band == WLAN_BAND_5G && ((he_channel_width_set & (BIT1 | BIT2 | BIT3)) == 0))) {
        if (*remain_len < size_20mhz) {
            return OAL_FAIL;
        }
        if (memcpy_s((uint8_t *)(&eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz), size_20mhz,
            mcs_nss, size_20mhz) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_20mhz_nss::memory copy failed.}");
            return OAL_FAIL;
        }
        *size += size_20mhz;
        *remain_len -= size_20mhz;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t mac_parse_eht_mcs_below_80mhz_nss(uint8_t band, uint8_t vap_mode,
    uint8_t he_channel_width_set, mac_frame_eht_capabilities_ie_stru *eht_cap,
    uint8_t *mcs_nss, uint8_t *remain_len, uint8_t *size)
{
    uint8_t size_except_20mhz = sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru);

    /* 自身为STA 对端为AP时，对端一定会携带<=80MHz EHT MCS map */
    if ((vap_mode == WLAN_VAP_MODE_BSS_STA) ||
        ((band == WLAN_BAND_2G && ((he_channel_width_set & BIT0) != 0)) ||
        ((band == WLAN_BAND_5G) && ((he_channel_width_set & BIT1) != 0)))) {
        if (*remain_len < size_except_20mhz) {
            return OAL_FAIL;
        }
        if (memcpy_s((uint8_t *)(&(eht_cap->st_eht_mcs_nss.st_eht_mcs_map_below_80mhz)),
            size_except_20mhz, mcs_nss, size_except_20mhz) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_below_80mhz_nss::memory copy failed.}");
            return OAL_FAIL;
        }
        mac_eht_mcs_info_print(&(eht_cap->st_eht_mcs_nss.st_eht_mcs_map_below_80mhz));
        *size += size_except_20mhz;
        *remain_len -= size_except_20mhz;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t mac_parse_eht_mcs_160mhz_nss(uint8_t he_channel_width_set,
    mac_frame_eht_capabilities_ie_stru *eht_cap, uint8_t *mcs_nss,
    uint8_t *remain_len, uint8_t *size)
{
    uint8_t size_except_20mhz = sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru);

    if ((he_channel_width_set & BIT2) != 0) {
        if (*remain_len < size_except_20mhz) {
            return OAL_FAIL;
        }
        if (memcpy_s((uint8_t *)(&eht_cap->st_eht_mcs_nss.st_eht_mcs_map_160mhz),
            size_except_20mhz, mcs_nss, size_except_20mhz) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_160mhz_nss::memory copy failed.}");
            return OAL_FAIL;
        }
        mac_eht_mcs_info_print(&(eht_cap->st_eht_mcs_nss.st_eht_mcs_map_160mhz));
        *size += size_except_20mhz;
        *remain_len -= size_except_20mhz;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t mac_parse_eht_mcs_320mhz_nss(uint8_t support_320mhz,
    mac_frame_eht_capabilities_ie_stru *eht_cap, uint8_t *mcs_nss,
    uint8_t *remain_len, uint8_t *size)
{
    uint8_t size_except_20mhz = sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru);

    if (support_320mhz) {
        if (*remain_len < size_except_20mhz) {
            return OAL_FAIL;
        }
        if (memcpy_s((uint8_t *)(&eht_cap->st_eht_mcs_nss.st_eht_mcs_map_320mhz),
            size_except_20mhz, mcs_nss, size_except_20mhz) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_320mhz_nss::memory copy failed.}");
            return OAL_FAIL;
        }
        *size += size_except_20mhz;
        *remain_len -= size_except_20mhz;
    }
    return OAL_SUCC;
}

/*
 * 函 数 名 : mac_parse_eht_mcs_nss
 * 功能描述 : 解析 EHT MCS NSS
 * 修改历史
* 日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void mac_eht_mcs_cust_update(mac_frame_eht_capabilities_ie_stru *eht_cap)
{
    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_1024qam_support == OAL_FALSE) {
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_tx_max_nss_eht_mcs10_11 = 0;
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_below_80mhz.bit_tx_max_nss_eht_mcs10_11 = 0;
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_160mhz.bit_tx_max_nss_eht_mcs10_11 = 0;
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_320mhz.bit_tx_max_nss_eht_mcs10_11 = 0;
    }
    if (g_st_mac_device_custom_cfg.custom_cfg_11be.eht_4096qam_support == OAL_FALSE) {
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_tx_max_nss_eht_mcs12_13 = 0;
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_below_80mhz.bit_tx_max_nss_eht_mcs12_13 = 0;
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_160mhz.bit_tx_max_nss_eht_mcs12_13 = 0;
        eht_cap->st_eht_mcs_nss.st_eht_mcs_map_320mhz.bit_tx_max_nss_eht_mcs12_13 = 0;
    }
}
static void mac_sync_eht_mcs_nss(mac_frame_eht_capabilities_ie_stru *eht_cap, const uint8_t *mcs_nss,
    uint8_t remain_len, uint8_t size)
{
    mac_frame_eht_basic_mcs_map_except_20mhz_stru *eht_mcs_map = NULL;

    if (remain_len < sizeof(mac_frame_eht_basic_mcs_map_except_20mhz_stru)) {
        return;
    }
    eht_mcs_map = (mac_frame_eht_basic_mcs_map_except_20mhz_stru *)mcs_nss;

    // 本身携带20M对应的nss信息不进行同步
    if (size == sizeof(mac_frame_eht_basic_mcs_map_20mhz_stru)) {
        return;
    }

    /* 高带宽nss能力同步到低带宽20Mhz */
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_rx_max_nss_eht_mcs0_7 =  eht_mcs_map->bit_rx_max_nss_eht_mcs0_9;
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_rx_max_nss_eht_mcs8_9 = eht_mcs_map->bit_rx_max_nss_eht_mcs0_9;
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_tx_max_nss_eht_mcs0_7 = eht_mcs_map->bit_tx_max_nss_eht_mcs0_9;
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_tx_max_nss_eht_mcs8_9 = eht_mcs_map->bit_tx_max_nss_eht_mcs0_9;
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_rx_max_nss_eht_mcs10_11 = eht_mcs_map->bit_rx_max_nss_eht_mcs10_11;
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_rx_max_nss_eht_mcs12_13 = eht_mcs_map->bit_rx_max_nss_eht_mcs12_13;
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_tx_max_nss_eht_mcs10_11 = eht_mcs_map->bit_tx_max_nss_eht_mcs10_11;
    eht_cap->st_eht_mcs_nss.st_eht_mcs_map_20mhz.bit_tx_max_nss_eht_mcs12_13 = eht_mcs_map->bit_tx_max_nss_eht_mcs12_13;
}

OAL_STATIC uint32_t mac_parse_eht_mcs_nss(mac_vap_stru *mac_vap, mac_user_stru *mac_user,
    uint8_t *mcs_nss, uint8_t remain_len, mac_frame_eht_phy_capabilities_stru *eht_phy_cap,
    mac_frame_eht_capabilities_ie_stru *eht_cap, uint8_t *mcs_nss_len)
{
    uint8_t band = mac_vap->st_channel.en_band;
    uint8_t vap_mode = mac_vap->en_vap_mode;
    uint8_t support_320mhz = eht_phy_cap->bit_320mhz_6ghz_support;
    uint8_t he_width_set = 0;
    uint8_t size = 0;
    uint32_t ret;

#ifdef _PRE_WLAN_FEATURE_11AX
    if (mac_user->st_he_hdl.en_he_capable == OAL_TRUE) {
        he_width_set = mac_user->st_he_hdl.st_he_cap_ie.st_he_phy_cap.bit_channel_width_set;
    }
#endif

    if (vap_mode == WLAN_VAP_MODE_BSS_AP) {
        ret = mac_parse_eht_mcs_20mhz_nss(band, he_width_set, eht_cap, mcs_nss, &remain_len, &size);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_20mhz_nss: fail.}");
            return OAL_FAIL;
        }
    }
    // 先校验20M对应流数是否未声明,未声明时高带宽流数同步到低带宽20M
    mac_sync_eht_mcs_nss(eht_cap, mcs_nss, remain_len, size);
    ret = mac_parse_eht_mcs_below_80mhz_nss(band, vap_mode, he_width_set, eht_cap, mcs_nss + size, &remain_len, &size);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_below_80mhz_nss: fail.}");
        return OAL_FAIL;
    }

    ret = mac_parse_eht_mcs_160mhz_nss(he_width_set, eht_cap, mcs_nss + size, &remain_len, &size);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_160mhz_nss: fail.}");
        return OAL_FAIL;
    }

    ret = mac_parse_eht_mcs_320mhz_nss(support_320mhz, eht_cap, mcs_nss + size, &remain_len, &size);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mcs_320mhz_nss: fail.}");
        return OAL_FAIL;
    }

    /* 定制化不支持eht 1024qam/4096qam 则清空对应速率集 */
    mac_eht_mcs_cust_update(eht_cap);

    *mcs_nss_len = size;
    return OAL_SUCC;
}

OAL_STATIC void mac_parse_eht_mcs15(mac_vap_stru *mac_vap, mac_frame_eht_phy_capabilities_stru *phy_cap)
{
    uint8_t mcs15 = 0;

    if (mac_mib_get_EHTMCS15For52p26and106p26MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE) {
        return;
    }

    mcs15 = 1;
    phy_cap->bit_support_of_mcs15 &= mcs15;

    if (mac_mib_get_EHTMCS15For484p242MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE) {
        return;
    }

    mcs15 |= BIT1;
    phy_cap->bit_support_of_mcs15 &= mcs15;

    if (mac_mib_get_EHTMCS15For996p484and996p484p242MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE)    {
        return;
    }

    mcs15 |= BIT2;
    phy_cap->bit_support_of_mcs15 &= mcs15;

    if (mac_mib_get_EHTMCS15For3x996MRUImplemented(mac_vap->pst_mib_info) == OAL_FALSE) {
        return;
    }

    mcs15 |= BIT3;
    phy_cap->bit_support_of_mcs15 &= mcs15;
}

OAL_STATIC void mac_parse_eht_ltfs_max_number(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *phy_cap)
{
    uint8_t max_num_ltf = 0;
    uint8_t num_ltf_su = 0;
    uint8_t num_ltf_mu = 0;
    /* B46-B50: Maximum Number Of Supported EHT-LTFs */
    if (mac_mib_get_EHTExtraLTFsImplemented(mac_vap->pst_mib_info)) {
        max_num_ltf = 1;
        num_ltf_su = mac_mib_get_EHTMaxNumberOfSupportedEHTLTFsForSU(mac_vap->pst_mib_info);
        if (num_ltf_su == BYTE_OFFSET_8) {
            max_num_ltf |= BIT1;
        }

        num_ltf_mu = mac_mib_get_EHTMaxNumberOfSupportedEHTLTFsForMUandNDP(mac_vap->pst_mib_info);
        if (num_ltf_mu == BYTE_OFFSET_8) {
            max_num_ltf |= BIT3;
        }

        if (phy_cap->supported_eht_ltfs_max_number > max_num_ltf) {
            phy_cap->supported_eht_ltfs_max_number = max_num_ltf;
        }
    } else {
        phy_cap->supported_eht_ltfs_max_number = 0;
    }
}


OAL_STATIC void mac_parse_eht_phy_capabilities_info1(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *phy_cap)
{
    /* B32: Partial Bandwidth DL MU-MIMO: 是否支持DL MU-MIMO和OFDMA组合模式 */
    phy_cap->partial_bandwidth_dl_mu_mimo &= mac_mib_get_EHTPartialBWDLMUMIMOImplemented(mac_vap->pst_mib_info);

    /* B33: EHT PSR-based SR Support: 是否支持EHT PSR-based SR操作 */
    phy_cap->eht_psr_based_sr_support &= mac_mib_get_EHTPSRBasedSRImplemented(mac_vap->pst_mib_info);

    /* B34: Power Boost Factor Support: 是否支持接收RU的功率提升系数在[0.5, 2]范围内的EHT MU PPDU */
    phy_cap->power_boost_factor_support &= mac_mib_get_EHTPowerBoostFactorImplemented(mac_vap->pst_mib_info);

    /* B36-B39: Max Nc：表示EHT compressed beamforming/CQI report支持的最大Nc值 */
    if (phy_cap->max_nc > mac_mib_get_EHTMaxNc(mac_vap->pst_mib_info)) {
        phy_cap->max_nc = mac_mib_get_EHTMaxNc(mac_vap->pst_mib_info);
    }

    /* B41: Tx 1024-QAM And 4096-QAM < 242-tone RU Support：是否支持使用1024-QAM 和
       4096-QAM并且小于242-tone RU发送EHT TB PPDU */
    phy_cap->tx_1024qam_4096qam_below_242_tone_ru_support &=
        mac_mib_get_EHTRx1024QAMand4096QAMLessThan242ToneRUImplemented(mac_vap->pst_mib_info);

    /* B42: Rx 1024-QAM And 4096-QAM < 242-tone RU Support：是否支持接收使用1024-QAM 和
       4096-QAM并且小于242-tone RU发送EHT TB PPDU */
    phy_cap->rx_1024qam_4096qam_below_242_tone_ru_support &=
        mac_mib_get_EHTTx1024QAMand4096QAMLessThan242ToneRUImplemented(mac_vap->pst_mib_info);

    /* B46-B50: Maximum Number Of Supported EHT-LTFs */
    mac_parse_eht_ltfs_max_number(mac_vap, phy_cap);

    /* B51-B54: Support Of MCS 15 */
    mac_parse_eht_mcs15(mac_vap, phy_cap);

    /* B55: Support Of EHT DUP (MCS 14) In 6 GHz: 是否支持6 GHz频段下的EHT DUP */
    phy_cap->bit_eht_dup_in_6ghz &= mac_mib_get_EHTDupImplemented(mac_vap->pst_mib_info);

    /* B56: Support For 20 MHz Operating STA Receiving NDP With Wider Bandwidth: 工作在20MHz下，
       是否能接收带宽大于20MHz的NDP PPDU */
    phy_cap->support_of_20mhz_sta_recv_ndp_over_20mhz &=
        mac_mib_get_EHT20MHzOperatingSTARxNDPwithWiderBWImplemented(mac_vap->pst_mib_info);

    /* B57: Non-OFDMA UL MU-MIMO (BW≤80 MHz): 是否支持在带宽小于或等于80MHz时，使用
       non-OFDMA UL MU-MIMO接收或发送的EHT TB PPDU */
    phy_cap->non_ofdma_ul_mu_mimo_below_80mhz &=
        mac_mib_get_EHTNonOFDMAULMUMIMOLessThanOrEqualto80Implemented(mac_vap->pst_mib_info);

    if (mac_vap->st_channel.en_band != WLAN_BAND_2G) {
        /* B58: Non-OFDMA UL MU-MIMO (BW=160 MHz): 是否支持在带宽等于160MHz时，使用
        non-OFDMA UL MU-MIMO 接收或发送EHT TB PPDU */
        phy_cap->non_ofdma_ul_mu_mimo_equal_160mhz &=
            mac_mib_get_EHTNonOFDMAULMUMIMOEqualto160Implemented(mac_vap->pst_mib_info);

        /* B59: Non-OFDMA UL MU-MIMO (BW=320 MHz): 是否支持在带宽等于320MHz时，使用
        non-OFDMA UL MU-MIMO 接收或发送EHT TB PPDU */
        phy_cap->non_ofdma_ul_mu_mimo_equal_320mhz &=
            mac_mib_get_EHTNonOFDMAULMUMIMOEqualto320Implemented(mac_vap->pst_mib_info);
    }
}

/*
 * 函 数 名 : mac_parse_eht_phy_capabilities_info
 * 功能描述 : 解析 EHT PHY Capabilities 信息
 * 修改历史
* 日    期 : 2023年9月18日
* 修改内容 : 新生成函数数
 */
OAL_STATIC void mac_parse_eht_phy_capabilities_info(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *phy_cap)
{
    uint8_t ret_val;
    /* B1: Support For 320 MHz In 6 GHz: 是否支持6 GHz频段的320 MHz的带宽 */
    phy_cap->bit_320mhz_6ghz_support &= mac_mib_get_EHTSupportFor320MHzImplemented(mac_vap->pst_mib_info);

    /* B2: Support for 242-tone RU In BW Wider Than 20 MHz: 是否支持6 GHz频段的320 MHz的带宽 */
    phy_cap->support_of_242_tone_ru_over_20mhz &=
        mac_mib_get_EHTSupportFor242ToneRUInBWWiderThan20Implemented(mac_vap->pst_mib_info);

    /* B3: NDP With 4x EHT-LTF And 3.2 μs GI: 对于beamformee，
       是否支持接收4x EHT-LTF and 3.2 μs GI的EHT sounding NDP */
    phy_cap->bit_ndp_4x_eht_ltf_3p2us_gi &=
        mac_mib_get_EHTNDPwith4xEHTLTFand3point2GIImplemented(mac_vap->pst_mib_info);

    /* B4: Partial Bandwidth UL MU-MIMO: 是否支持UL MU-MIMO和OFDMA组合模式 */
    phy_cap->partial_bandwidth_ul_mu_mimo &= mac_mib_get_EHTPartialBWULMUMIMOImplemented(mac_vap->pst_mib_info);

    /* B7-B9: Beamformee SS (≤ 80 MHz): 当带宽小于或等于80 MHz时，表示STA能接收EHT sounding NDP的最大空间流数 */
    ret_val = mac_mib_get_EHTBeamformeeSSLessThanOrEqualTo80(mac_vap->pst_mib_info);
    if (phy_cap->beamformee_ss_below_80mhz > ret_val) {
        phy_cap->beamformee_ss_below_80mhz = ret_val;
    }

    /* B10-B12: Beamformee SS (= 160 MHz): 当带宽等于160 MHz时，表示STA能接收EHT sounding NDP的最大空间流数 */
    if (phy_cap->beamformee_ss_equal_160mhz > mac_mib_get_EHTBeamformeeSSEqualTo160(mac_vap->pst_mib_info)) {
        phy_cap->beamformee_ss_equal_160mhz = mac_mib_get_EHTBeamformeeSSEqualTo160(mac_vap->pst_mib_info);
    }

    /* B13-B15: Beamformee SS (= 320 MHz): 当带宽等于320 MHz时，表示STA能接收EHT sounding NDP的最大空间流数 */
    if (phy_cap->beamformee_ss_equal_320mhz > mac_mib_get_EHTBeamformeeSSEqualTo320(mac_vap->pst_mib_info)) {
        phy_cap->beamformee_ss_equal_320mhz = mac_mib_get_EHTBeamformeeSSEqualTo320(mac_vap->pst_mib_info);
    }

    /* B16-B18: Number Of Sounding Dimensions (≤ 80 MHz): 当带宽小于或等于80 MHz时，
       表示EHT sounding NDP的TXVECTOR参数NUM_STS的最大值 */
    ret_val = mac_mib_get_EHTNumberSoundingDimensionsLessThanOrEqualTo80(mac_vap->pst_mib_info);
    if (phy_cap->sounding_dimensions_num_below_80mhz > ret_val) {
        phy_cap->sounding_dimensions_num_below_80mhz = ret_val;
    }

    /* B19-B21: Number Of Sounding Dimensions (= 160 MHz): 当带宽等于160 MHz时，
       表示EHT sounding NDP的TXVECTOR参数NUM_STS的最大值 */
    ret_val = mac_mib_get_EHTNumberSoundingDimensionsEqualTo160(mac_vap->pst_mib_info);
    if (phy_cap->sounding_dimensions_num_equal_160mhz > ret_val) {
        phy_cap->sounding_dimensions_num_equal_160mhz = ret_val;
    }

    /* B22-B24: Number Of Sounding Dimensions (= 320 MHz): 当带宽等于320 MHz时，
       表示EHT sounding NDP的TXVECTOR参数NUM_STS的最大值 */
    ret_val = mac_mib_get_EHTNumberSoundingDimensionsEqualTo320(mac_vap->pst_mib_info);
    if (phy_cap->sounding_dimensions_num_equal_320mhz > ret_val) {
        phy_cap->sounding_dimensions_num_equal_320mhz = ret_val;
    }

    /* B25: Ng = 16 SU Feedback: 是否支持SU反馈的EHT Compressed Beamforming Report field中的16个子载波分组 */
    phy_cap->ng16_su_feedback &= mac_mib_get_EHTNG16SUFeedbackImplemented(mac_vap->pst_mib_info);

    /* B26: Ng = 16 MU Feedback: 是否支持MU反馈的EHT Compressed Beamforming Report field中的16个子载波分组 */
    phy_cap->ng16_mu_feedback &= mac_mib_get_EHTNG16MUFeedbackImplemented(mac_vap->pst_mib_info);

    mac_parse_eht_phy_capabilities_info1(mac_vap, phy_cap);
}

/*
 * 函 数 名   : mac_parse_eht_mac_capabilities_info
 * 功能描述   : 解析 EHT MAC Capabilities 信息
 * 修改历史
* 日    期 : 2023年9月18日
* 修改内容 : 新生成函数
 */
OAL_STATIC void mac_parse_eht_mac_capabilities_info(mac_vap_stru *mac_vap,
    mac_frame_eht_mac_capabilities_stru *mac_cap)
{
    /* B0: NSEP Priority Access Supported: 是否支持NSEP优先接入 */
    mac_cap->epcs_priority_access_sup &= mac_mib_get_EHTNSEPPriorityAccessActivated(mac_vap->pst_mib_info);

    /* B1: EHT OM Control Support: 是否支持接收携带EHT OM Control子字段的帧 */
    mac_cap->eht_om_control_support &= mac_mib_get_EHTOmControlSupport(mac_vap->pst_mib_info);

    /* B2: Triggered TXOP Sharing Mode 1 Support: 是否支持传输或回应携带Triggered TXOP Sharing Mode
       为1的TXOP sharing trigger帧 */
    mac_cap->txop_sharing_mode1_support &= mac_mib_get_EHTTXOPSharingTFOptionImplemented(mac_vap->pst_mib_info);

    /* B3: Triggered TXOP Sharing Mode 2 Support: 是否支持传输或回应携带Triggered TXOP Sharing Mode
       为2的TXOP sharing trigger帧 */
    mac_cap->txop_sharing_mode2_support &= mac_mib_get_EHTTXOPSharingTFOptionImplemented(mac_vap->pst_mib_info);

    /* B4: Restricted TWT Support: 是否支持restricted TWT */
    mac_cap->restricted_twt_support &= mac_mib_get_RestrictedTWTOptionImplemented(mac_vap->pst_mib_info);

    /* B5: SCS Traffic Description Support: 是否支持传输和接收包含QoS Characteristics子元素的SCS帧 */
    mac_cap->scs_traffic_dscr_support &= mac_mib_get_SCSActivated(mac_vap->pst_mib_info);

    /* B6: Maximum MPDU Length: STA能接收的最大MPDU长度。0代表3895字节，1代表7991字节，2代表11454字节。
       5G和6G频段该字段为缺省字段 */
    if ((mac_vap->st_channel.en_band == WLAN_BAND_2G) &&
        (mac_cap->max_mpdu_length > mac_mib_get_maxmpdu_length(mac_vap))) {
        mac_cap->max_mpdu_length = mac_mib_get_maxmpdu_length(mac_vap);
    }

    /* B8: Maximum A-MPDU Length Exponent Extension */
    mac_cap->bit_max_ampdu_length_exponent &= mac_mib_get_EHTMaxAmpduLengthExponent(mac_vap->pst_mib_info);
}

OAL_STATIC uint32_t mac_parse_eht_mac_capabilities_info_handle(mac_vap_stru *mac_vap,
    mac_frame_eht_mac_capabilities_stru *mac_cap, mac_frame_eht_capabilities_ie_stru *eht_cap)
{
    uint8_t mac_cap_len = sizeof(mac_frame_eht_mac_capabilities_stru);
    /* 解析 EHT MAC Capabilities Info */
    mac_parse_eht_mac_capabilities_info(mac_vap, mac_cap);
    if (memcpy_s((uint8_t *)(&eht_cap->eht_mac_cap), mac_cap_len, mac_cap, mac_cap_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_mac_capabilities_info_handle::memory copy failed.}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t mac_parse_eht_phy_capabilities_info_handle(mac_vap_stru *mac_vap,
    mac_frame_eht_phy_capabilities_stru *phy_cap, mac_frame_eht_capabilities_ie_stru *eht_cap)
{
    uint8_t phy_cap_len = sizeof(mac_frame_eht_phy_capabilities_stru);

    /* 解析   EHT PHY Capabilities Info */
    mac_parse_eht_phy_capabilities_info(mac_vap, phy_cap);
    if (memcpy_s((uint8_t *)(&eht_cap->st_eht_phy_cap), phy_cap_len, phy_cap, phy_cap_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_phy_capabilities_info_handle::memory copy failed.}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 函 数 名   : mac_parse_eht_capabilities_ie
 * 功能描述   : 解析eht capabilities 信息
 * 修改历史
* 日    期 : 2023年9月18日
* 修改内容 : 新生成函数
 */
OAL_STATIC uint32_t mac_parse_eht_capabilities_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user,
    uint8_t *ie, mac_frame_eht_capabilities_ie_stru *eht_cap)
{
    mac_frame_eht_mac_capabilities_stru mac_cap = { 0 };
    mac_frame_eht_phy_capabilities_stru phy_cap = { 0 };
    uint8_t mac_cap_len = sizeof(mac_frame_eht_mac_capabilities_stru);
    uint8_t phy_cap_len = sizeof(mac_frame_eht_phy_capabilities_stru);
    uint8_t len = 0;
    uint8_t remain_len;
    uint8_t mcs_nss_set_len = 0;

    len = ie[1];
    if (len < MAC_EHT_CAP_MIN_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_parse_eht_capabilities_ie::invalid he cap ie len[%d].}", len);
        return OAL_FAIL;
    }

    ie += MAC_IE_EXT_HDR_LEN;

    /* 获取 EHT MAC Capabilities Info */
    if (memcpy_s((uint8_t *)(&mac_cap), mac_cap_len, ie, mac_cap_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_capabilities_ie::memory copy failed.}");
        return OAL_FAIL;
    }
    ie += MAC_EHT_MAC_CAP_LEN;

    /* 获取   EHT PHY Capabilities Info */
    if (memcpy_s((uint8_t *)(&phy_cap), phy_cap_len, ie, phy_cap_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_capabilities_ie::memory copy failed.}");
        return OAL_FAIL;
    }
    ie += MAC_EHT_PHY_CAP_LEN;

    if (mac_parse_eht_mac_capabilities_info_handle(mac_vap, &mac_cap, eht_cap) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if (mac_parse_eht_phy_capabilities_info_handle(mac_vap, &phy_cap, eht_cap) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 最小包含eht mac cap和eht phy cap */
    if (len == MAC_EHT_CAP_MIN_LEN) {
        return OAL_SUCC;
    }

    remain_len = len - MAC_EHT_CAP_MIN_LEN;
    /* 解析Support EHT-MCS NSS Set */
    if (mac_parse_eht_mcs_nss(mac_vap, mac_user, ie, remain_len,
        &phy_cap, eht_cap, &mcs_nss_set_len) != OAL_SUCC) {
        return OAL_FAIL;
    }
    ie += mcs_nss_set_len;

    /* 暂不解析EHT PPE Thresholds field */
    return OAL_SUCC;
}

/*
 * 函 数 名   : mac_parse_eht_operation_ie
 * 功能描述   : 解析eht operation 信息
 * 修改历史
* 日    期 : 2023年9月18日
* 修改内容 : 新生成函数
 */
uint32_t mac_parse_eht_operation_ie(mac_vap_stru *mac_vap, uint8_t *oper_ie,
    mac_frame_eht_operation_ie_stru *eht_oper)
{
    eht_operation_parameters_stru *eht_oper_parameters = NULL;
    uint8_t len = 0;

    if (oal_unlikely((oper_ie == NULL) || (eht_oper == NULL))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_operation_ie::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (oper_ie[1] < MAC_EHT_OPER_MIN_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_parse_eht_operation_ie::invalid eht oper ie len[%d].}", oper_ie[1]);
        return OAL_FAIL;
    }
    eht_oper_parameters = (eht_operation_parameters_stru *)(oper_ie + MAC_IE_EXT_HDR_LEN);
    len += (MAC_IE_EXT_HDR_LEN + EHT_OPERATION_PARAMETERS_LEN + BASIC_EHT_MCS_AND_NSS_SET_LEN);

    if (eht_oper_parameters->eht_operation_information_present == 1) {
        len += EHT_OPERATION_INFO_MIN_LEN;
        if (eht_oper_parameters->disabled_subchannel_bitmap_present == 1) {
            len += DISABLED_SUBCHANNEL_BITMAP_LEN;
        }
    }

    if (oper_ie[1] != len - MAC_IE_HDR_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_parse_eht_operation_ie::invalid eht oper ie len[%d].}", oper_ie[1]);
        return OAL_FAIL;
    }

    if (memcpy_s((uint8_t *)eht_oper, sizeof(mac_frame_eht_operation_ie_stru), oper_ie, len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_eht_operation_ie::memory copy failed.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
* 函 数 名: mac_process_eht_capabilities_ie
* 功能描述   : 处理EHT capabilities IE信息
* 修改历史
* 日    期 : 2023年9月18日
* 修改内容 : 新生成函数
*/
uint32_t mac_process_eht_capabilities_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user,
    uint8_t *eht_cap_ie)
{
    mac_eht_hdl_stru mac_eht_hdl;
    mac_frame_eht_capabilities_ie_stru eht_cap_value;

    /* 解析EHT cap IE */
    if (oal_unlikely(oal_any_null_ptr3(mac_vap, mac_user, eht_cap_ie))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_eht_capabilities_ie::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(&mac_eht_hdl, sizeof(mac_eht_hdl_stru), 0, sizeof(mac_eht_hdl_stru));
    memset_s(&eht_cap_value, sizeof(mac_frame_eht_capabilities_ie_stru),
        0, sizeof(mac_frame_eht_capabilities_ie_stru));

    /* 解析 EHT Capabilities Info */
    if (mac_parse_eht_capabilities_ie(mac_vap, mac_user, eht_cap_ie, &eht_cap_value) != OAL_SUCC) {
        return OAL_FAIL;
    }

    mac_user_get_eht_hdl(mac_user, &mac_eht_hdl);

    if (memcpy_s(&mac_eht_hdl.st_eht_cap_ie, sizeof(eht_cap_value), &eht_cap_value,
        sizeof(eht_cap_value)) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_eht_capabilities_ie::memory copy failed.}");
        return OAL_FAIL;
    }

    mac_user_set_eht_hdl(mac_user, &mac_eht_hdl);

    mac_user_set_eht_capable(mac_user, OAL_TRUE);

    return OAL_SUCC;
}

/*
* 函 数 名 : mac_process_eht_operation_ie
* 功能描述 : 处理EHT operation IE信息
* 修改历史
* 日    期 : 2023年9月18日
* 修改内容 : 新生成函数
*/
uint32_t mac_process_eht_operation_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user, uint8_t *oper_ie)
{
    mac_frame_eht_operation_ie_stru eht_oper;
    mac_eht_hdl_stru                eht_hdl;
    eht_operation_information_stru *new_eht_oper_info = &eht_oper.eht_oper_info;
    eht_operation_information_stru *old_eht_oper_info = &eht_hdl.eht_oper_ie.eht_oper_info;
    uint32_t ret = MAC_NO_CHANGE;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, mac_user, oper_ie))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_eht_operation_ie::param null}");
        return ret;
    }

    memset_s(&eht_oper, sizeof(mac_frame_eht_operation_ie_stru), 0, sizeof(mac_frame_eht_operation_ie_stru));
    memset_s(&eht_hdl, sizeof(mac_eht_hdl_stru), 0, sizeof(mac_eht_hdl_stru));

    /* 解析 EHT Operation Info */
    if (mac_parse_eht_operation_ie(mac_vap, oper_ie, &eht_oper) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_eht_operation_ie::parse eht opern ie FAIL!}");
        return ret;
    }

    mac_user_get_eht_hdl(mac_user, &eht_hdl);

    if (eht_oper.eht_oper_parameters.eht_operation_information_present) {
        if (new_eht_oper_info->channel_width > WLAN_EHT_CHANNEL_WIDTH_320) {
            oam_warning_log1(0, OAM_SF_ANY, "{mac_process_eht_operation_ie::invalid channel width[%d], force 20M.}",
                new_eht_oper_info->channel_width);
            new_eht_oper_info->channel_width = WLAN_EHT_CHANNEL_WIDTH_20;
        }

        if (old_eht_oper_info->channel_width != new_eht_oper_info->channel_width ||
            old_eht_oper_info->ccfs0 != new_eht_oper_info->ccfs0 ||
            old_eht_oper_info->ccfs1 != new_eht_oper_info->ccfs1) {
            ret = MAC_EHT_CHANGE;
        }
    }

    memcpy_s((uint8_t *)(&eht_hdl.eht_oper_ie), sizeof(mac_frame_eht_operation_ie_stru),
             &eht_oper, sizeof(mac_frame_eht_operation_ie_stru));

    mac_user_set_eht_hdl(mac_user, &eht_hdl);

    mac_user_set_eht_operation(mac_user, OAL_TRUE);

    return ret;
}

uint32_t mac_get_avail_nss_eht(mac_frame_eht_basic_mcs_map_except_20mhz_stru *eht_mcs_map, uint8_t *nss)
{
    uint32_t    ret = OAL_SUCC;
    uint8_t     max_nss = 0;
    if (eht_mcs_map->bit_tx_max_nss_eht_mcs12_13 > max_nss) {
        max_nss = eht_mcs_map->bit_tx_max_nss_eht_mcs12_13;
    }
    if (eht_mcs_map->bit_rx_max_nss_eht_mcs12_13 > max_nss) {
        max_nss = eht_mcs_map->bit_rx_max_nss_eht_mcs12_13;
    }
    if (eht_mcs_map->bit_tx_max_nss_eht_mcs10_11 > max_nss) {
        max_nss = eht_mcs_map->bit_tx_max_nss_eht_mcs10_11;
    }
    if (eht_mcs_map->bit_rx_max_nss_eht_mcs10_11 > max_nss) {
        max_nss = eht_mcs_map->bit_rx_max_nss_eht_mcs10_11;
    }
    if (eht_mcs_map->bit_tx_max_nss_eht_mcs0_9 > max_nss) {
        max_nss = eht_mcs_map->bit_tx_max_nss_eht_mcs0_9;
    }
    if (eht_mcs_map->bit_rx_max_nss_eht_mcs0_9 > max_nss) {
        max_nss = eht_mcs_map->bit_rx_max_nss_eht_mcs0_9;
    }

    if (max_nss == 0) {  // nss为0表示不支持对应的mcs
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_get_avail_nss_eht::invalid nss.}");
        eht_mcs_map->bit_tx_max_nss_eht_mcs0_9 = 1;
        eht_mcs_map->bit_rx_max_nss_eht_mcs0_9 = 1;
        max_nss = 1;
    }
    *nss = max_nss - 1;
    return ret;
}

uint32_t mac_get_avail_20mhz_nss_eht(mac_frame_eht_basic_mcs_map_20mhz_stru *eht_mcs_map, uint8_t *nss)
{
    uint32_t    ret = OAL_SUCC;
    uint8_t     max_nss = 0;
    if (eht_mcs_map->bit_tx_max_nss_eht_mcs12_13 > max_nss) {
        max_nss = eht_mcs_map->bit_tx_max_nss_eht_mcs12_13;
    }
    if (eht_mcs_map->bit_rx_max_nss_eht_mcs12_13 > max_nss) {
        max_nss = eht_mcs_map->bit_rx_max_nss_eht_mcs12_13;
    }
    if (eht_mcs_map->bit_tx_max_nss_eht_mcs10_11 > max_nss) {
        max_nss = eht_mcs_map->bit_tx_max_nss_eht_mcs10_11;
    }
    if (eht_mcs_map->bit_rx_max_nss_eht_mcs10_11 > max_nss) {
        max_nss = eht_mcs_map->bit_rx_max_nss_eht_mcs10_11;
    }
    if (eht_mcs_map->bit_tx_max_nss_eht_mcs8_9 > max_nss) {
        max_nss = eht_mcs_map->bit_tx_max_nss_eht_mcs8_9;
    }
    if (eht_mcs_map->bit_rx_max_nss_eht_mcs8_9 > max_nss) {
        max_nss = eht_mcs_map->bit_rx_max_nss_eht_mcs8_9;
    }
    if (eht_mcs_map->bit_tx_max_nss_eht_mcs0_7 > max_nss) {
        max_nss = eht_mcs_map->bit_tx_max_nss_eht_mcs0_7;
    }
    if (eht_mcs_map->bit_rx_max_nss_eht_mcs0_7 > max_nss) {
        max_nss = eht_mcs_map->bit_rx_max_nss_eht_mcs0_7;
    }
    if (max_nss == 0) {  // nss为0表示不支持对应的mcs
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_get_avail_20mhz_nss_eht::invalid nss.}");
        eht_mcs_map->bit_tx_max_nss_eht_mcs0_7 = 1;
        eht_mcs_map->bit_rx_max_nss_eht_mcs0_7 = 1;
        max_nss = 1;
    }
    *nss = max_nss - 1;
    return ret;
}

uint32_t mac_user_set_avail_nss_eht(mac_user_stru *mac_user, mac_eht_hdl_stru *mac_eht_hdl,
    uint8_t *user_num_spatial_stream)
{
    mac_vap_stru *mac_vap = NULL;
    mac_frame_eht_basic_mcs_map_20mhz_stru *eht_mcs_map_20mhz = NULL;
    mac_frame_eht_basic_mcs_map_except_20mhz_stru *eht_mcs_map = NULL;
    uint32_t ret;

    mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(mac_user->uc_vap_id);
    if (oal_unlikely(mac_vap == NULL)) {
        oam_error_log0(mac_user->uc_vap_id, OAM_SF_ANY, "{hmac_user_set_avail_nss_eht::get mac vap fail.}");
        return OAL_FAIL;
    }

    if (mac_user->en_bandwidth_cap == WLAN_BW_CAP_320M) {
        eht_mcs_map = &(mac_eht_hdl->st_eht_cap_ie.st_eht_mcs_nss.st_eht_mcs_map_320mhz);
        ret = mac_get_avail_nss_eht(eht_mcs_map, user_num_spatial_stream);
    } else if (mac_user->en_bandwidth_cap == WLAN_BW_CAP_160M) {
        eht_mcs_map = &(mac_eht_hdl->st_eht_cap_ie.st_eht_mcs_nss.st_eht_mcs_map_160mhz);
        mac_eht_mcs_info_print(eht_mcs_map);
        ret = mac_get_avail_nss_eht(eht_mcs_map, user_num_spatial_stream);
    } else if ((mac_user->en_bandwidth_cap >= WLAN_BW_CAP_40M && mac_user->en_bandwidth_cap <= WLAN_BW_CAP_80M) ||
        (mac_user->en_bandwidth_cap == WLAN_BW_CAP_20M && mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA)) {
        eht_mcs_map = &(mac_eht_hdl->st_eht_cap_ie.st_eht_mcs_nss.st_eht_mcs_map_below_80mhz);
        mac_eht_mcs_info_print(eht_mcs_map);
        ret = mac_get_avail_nss_eht(eht_mcs_map, user_num_spatial_stream);
    } else if (mac_user->en_bandwidth_cap == WLAN_BW_CAP_20M && mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        eht_mcs_map_20mhz = &(mac_eht_hdl->st_eht_cap_ie.st_eht_mcs_nss.st_eht_mcs_map_20mhz);
        ret = mac_get_avail_20mhz_nss_eht(eht_mcs_map_20mhz, user_num_spatial_stream);
    } else {
        oam_warning_log0(mac_user->uc_vap_id, OAM_SF_ANY, "{hmac_user_set_avail_nss_eht::invalid eht bandwidth.}");
        return OAL_FAIL;
    }

    if (ret != OAL_SUCC) {
        oam_warning_log0(mac_user->uc_vap_id, OAM_SF_ANY, "{hmac_user_set_avail_nss_eht::invalid eht nss.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 功能描述   : 计算EHT协议下的带宽能力
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
uint8_t mac_get_eht_bandwith_from_center_freq_seg0_seg1(
    uint8_t chan_width, uint8_t channel, uint8_t ccfs0, uint8_t ccfs1)
{
    uint8_t ret = WLAN_BAND_WIDTH_20M;
    /*********** 不同带宽下，segment0 与 segment1 的意义 ***************
     当带宽为 20M/40M/80M 时，segment0表示信道中心频率索引
                             segment1设置为0
     当带宽为     160M    时，segment0表示 主80M  信道的 中心频率索引
                             segment1表示 160M   信道的 中心频率索引
     当带宽为     320M    时，segment0表示 主160M 信道的 中心频率索引
                             segment1表示 320M   信道的 中心频率索引
    ******************************************************************/
    if (chan_width == WLAN_EHT_CHANNEL_WIDTH_320) {
        /* 当前设备不支持320MHz带宽，降为20M */
#ifdef _PRE_WLAN_FEATURE_160M
    } else if (chan_width == WLAN_EHT_CHANNEL_WIDTH_160) {
        if (ccfs1 - ccfs0 == CHAN_OFFSET_PLUS_8 ||
            ccfs0 - ccfs1 == CHAN_OFFSET_PLUS_8) {
            ret = mac_get_160mbandwith_from_offset_channel((int8_t)(ccfs1 - channel));
        } else {
            ret = mac_get_160mbandwith_from_offset_channel((int8_t)(ccfs0 - channel));
        }
#endif
    } else if (chan_width == WLAN_EHT_CHANNEL_WIDTH_80) {
        ret = mac_get_80mbandwith_from_offset_channel((int8_t)(ccfs0 - channel));
    } else if (chan_width == WLAN_EHT_CHANNEL_WIDTH_40) {
        ret = mac_get_40mbandwith_from_offset_channel((int8_t)(ccfs0 - channel));
    }

    return ret;
}

/*
 * 函 数 名   : mac_set_addba_extension_ie
 * 功能描述   : 组ADDBA Extension IE
 */
uint32_t mac_set_addba_extension_ie(uint8_t *payload_addr, uint8_t *ie_len, uint8_t para)
{
    if (oal_unlikely(oal_any_null_ptr2(payload_addr, ie_len))) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ADDBA Extension Element Format */
    /* --------------------------------------------------------------------- */
    /* | Element ID | Length | ADDBA Additional Parameter Set | */
    /* --------------------------------------------------------------------- */
    /* | 1          | 1      | 1                              | */
    /* bit0:No-Fragmentation bit1-bit2:HE Fragmentation Operation bit3-bit4:Reserved bit5-bit7:Extended Buffer Size */
    /* --------------------------------------------------------------------- */
    /* 设置 ADDBA Extension Element */
    payload_addr[0] = MAC_EID_ADDBA_EXTENSION;
    payload_addr[1] = MAC_ADDBA_EXTENSION_IE_LEN;
    payload_addr[MAC_IE_HDR_LEN] = para;

    *ie_len = MAC_IE_HDR_LEN + MAC_ADDBA_EXTENSION_IE_LEN;

    return OAL_SUCC;
}

#endif /* end of _PRE_WLAN_FEATURE_11BE */
