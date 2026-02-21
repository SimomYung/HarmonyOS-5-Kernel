/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : mac_vap.c 的头文件
 * 作者       : huxiaotong
 * 创建日期   : 2012年10月19日
 */

#ifndef MAC_MIB_STA_EHT_CFG_H
#define MAC_MIB_STA_EHT_CFG_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifdef _PRE_WLAN_FEATURE_11BE
/*****************************************************************************
 函 数 名  : mac_mib_set_EHTLinkReconfigurationOperationActivated
 功能描述  : 设置MIB项 dot11EHTLinkReconfigurationOperationActivated 的值
 修改历史      :
  1.日    期   : 2024年2月5日
    作    者   : w30008019
    修改内容   : 新生成函数

*****************************************************************************/
static inline void mac_mib_set_EHTLinkReconfigurationOperationActivated(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTLinkReconfigurationOperationActivated = val;
}

/*****************************************************************************
 函 数 名  : mac_mib_get_EHTLinkReconfigurationOperationActivated
 功能描述  : 获取MIB项 dot11EHTLinkReconfigurationOperationActivated 的值
 修改历史      :
  1.日    期   : 2024年2月5日
    作    者   : w30008019
    修改内容   : 新生成函数
*****************************************************************************/
static inline oal_bool_enum_uint8 mac_mib_get_EHTLinkReconfigurationOperationActivated(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTLinkReconfigurationOperationActivated;
}

static inline void mac_mib_set_EHTOptionImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTOptionImplemented = en_val;
}


static inline oal_bool_enum_uint8 mac_mib_get_EHTOptionImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTOptionImplemented;
}


static inline void mac_mib_set_EHTBaseLineFeaturesImplementedOnly(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTBaseLineFeaturesImplementedOnly = en_val;
}


static inline oal_bool_enum_uint8 mac_mib_get_EHTBaseLineFeaturesImplementedOnly(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTBaseLineFeaturesImplementedOnly;
}


static inline void mac_mib_set_EHTEPCSPriorityAccessActivated(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTEPCSPriorityAccessActivated = en_val;
}


static inline uint8_t mac_mib_get_EHTNSEPPriorityAccessActivated(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTEPCSPriorityAccessActivated;
}


static inline void mac_mib_set_RestrictedTWTOptionImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11RestrictedTWTOptionImplemented = en_val;
}


static inline uint8_t mac_mib_get_RestrictedTWTOptionImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11RestrictedTWTOptionImplemented;
}


static inline void mac_mib_set_EHTTXOPSharingTFOptionImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTTXOPSharingTFOptionImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTTXOPSharingTFOptionImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTTXOPSharingTFOptionImplemented;
}


static inline void mac_mib_set_WNMSleepModeImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11WNMSleepModeImplemented = en_val;
}


static inline uint8_t mac_mib_get_WNMSleepModeImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11WNMSleepModeImplemented;
}


static inline void mac_mib_set_SCSActivated(wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11SCSActivated = en_val;
}


static inline uint8_t mac_mib_get_SCSActivated(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11SCSActivated;
}

static inline void mac_mib_set_EHTMaxAmpduLengthExponent(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTMaxAmpduLengthExponent = val;
}

static inline uint8_t mac_mib_get_EHTMaxAmpduLengthExponent(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTMaxAmpduLengthExponent;
}

static inline void mac_mib_set_EHTTrsSupport(wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTTrsSupport = val;
}

static inline uint8_t mac_mib_get_EHTTrsSupport(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTTrsSupport;
}

static inline void mac_mib_set_TxopReturnSupportInTxopSharingMode2(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 val)
{
    mib_info->wlan_mib_eht_sta_config.dot11TxopReturnSupportInTxopSharingMode2 = val;
}

static inline uint8_t mac_mib_get_TxopReturnSupportInTxopSharingMode2(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11TxopReturnSupportInTxopSharingMode2;
}


static inline void mac_mib_set_EHTPPEThresholdsRequired(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTPPEThresholdsRequired = en_val;
}


static inline uint8_t mac_mib_get_EHTPPEThresholdsRequired(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTPPEThresholdsRequired;
}

static inline void mac_mib_set_EHTOmControlSupport(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11EHTOmControlSupport = en_val;
}

static inline uint8_t mac_mib_get_EHTOmControlSupport(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11EHTOmControlSupport;
}


static inline void mac_mib_set_TIDtoLinkMappingActivated(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11TIDtoLinkMappingActivated = en_val;
}


static inline uint8_t mac_mib_get_TIDtoLinkMappingActivated(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11TIDtoLinkMappingActivated;
}


static inline void mac_mib_set_MultiLinkActivated(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_sta_config.dot11MultiLinkActivated = en_val;
}


static inline uint8_t mac_mib_get_MultiLinkActivated(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_sta_config.dot11MultiLinkActivated;
}


static inline void mac_mib_set_EHTCurrentChannelWidth(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val)
{
    mib_info->wlan_mib_eht_oper_info_config.dot11EHTCurrentChannelWidth = val;
}


static inline uint8_t mac_mib_get_EHTCurrentChannelWidth(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_oper_info_config.dot11EHTCurrentChannelWidth;
}


static inline void mac_mib_set_EHTDisabledSubchannelBitmapPresent(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_oper_info_config.dot11EHTDisabledSubchannelBitmapPresent = en_val;
}


static inline uint8_t mac_mib_get_EHTDisabledSubchannelBitmapPresent(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_oper_info_config.dot11EHTDisabledSubchannelBitmapPresent;
}


static inline void mac_mib_set_EHTDisabledSubchannelBitmap(wlan_mib_ieee802dot11_stru *mib_info,
    uint16_t us_val)
{
    mib_info->wlan_mib_eht_oper_info_config.dot11EHTDisabledSubchannelBitmap = us_val;
}


static inline uint16_t mac_mib_get_EHTDisabledSubchannelBitmap(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_oper_info_config.dot11EHTDisabledSubchannelBitmap;
}


static inline void mac_mib_set_EHTSupportFor320MHzImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSupportFor320MHzImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTSupportFor320MHzImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSupportFor320MHzImplemented;
}


static inline void mac_mib_set_EHTNonOFDMAULMUMIMOLessThanOrEqualto80Implemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonOFDMAULMUMIMOLessThanOrEqualto80Implemented = en_val;
}


static inline uint8_t mac_mib_get_EHTNonOFDMAULMUMIMOLessThanOrEqualto80Implemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonOFDMAULMUMIMOLessThanOrEqualto80Implemented;
}


static inline void mac_mib_set_EHTNonOFDMAULMUMIMOEqualto160Implemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonOFDMAULMUMIMOEqualto160Implemented = en_val;
}


static inline uint8_t mac_mib_get_EHTNonOFDMAULMUMIMOEqualto160Implemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonOFDMAULMUMIMOEqualto160Implemented;
}


static inline void mac_mib_set_EHTNonOFDMAULMUMIMOEqualto320Implemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonOFDMAULMUMIMOEqualto320Implemented = en_val;
}


static inline uint8_t mac_mib_get_EHTNonOFDMAULMUMIMOEqualto320Implemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonOFDMAULMUMIMOEqualto320Implemented;
}


static inline void mac_mib_set_EHTPartialBWULMUMIMOImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPartialBWULMUMIMOImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTPartialBWULMUMIMOImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPartialBWULMUMIMOImplemented;
}


static inline void mac_mib_set_EHTMUPPDUwith4xEHTLTFand0point8usecGIImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUPPDUwith4xEHTLTFand0point8usecGIImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMUPPDUwith4xEHTLTFand0point8usecGIImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUPPDUwith4xEHTLTFand0point8usecGIImplemented;
}


static inline void mac_mib_set_EHTPSRBasedSRImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPSRBasedSRImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTPSRBasedSRImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPSRBasedSRImplemented;
}


static inline void mac_mib_set_EHTPowerBoostFactorImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPowerBoostFactorImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTPowerBoostFactorImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPowerBoostFactorImplemented;
}


static inline void mac_mib_set_EHTTx1024QAMand4096QAMLessThan242ToneRUImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTx1024QAMand4096QAMLessThan242ToneRUImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTTx1024QAMand4096QAMLessThan242ToneRUImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTx1024QAMand4096QAMLessThan242ToneRUImplemented;
}


static inline void mac_mib_set_EHTRx1024QAMand4096QAMLessThan242ToneRUImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTRx1024QAMand4096QAMLessThan242ToneRUImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTRx1024QAMand4096QAMLessThan242ToneRUImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTRx1024QAMand4096QAMLessThan242ToneRUImplemented;
}


static inline void mac_mib_set_EHTExtraLTFsImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTExtraLTFsImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTExtraLTFsImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTExtraLTFsImplemented;
}


static inline void mac_mib_set_EHTMCS15For52p26and106p26MRUImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For52p26and106p26MRUImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMCS15For52p26and106p26MRUImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For52p26and106p26MRUImplemented;
}


static inline void mac_mib_set_EHTMCS15For484p242MRUImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For484p242MRUImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMCS15For484p242MRUImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For484p242MRUImplemented;
}


static inline void mac_mib_set_EHTMCS15For996p484and996p484p242MRUImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For996p484and996p484p242MRUImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMCS15For996p484and996p484p242MRUImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For996p484and996p484p242MRUImplemented;
}


static inline void mac_mib_set_EHTMCS15For3x996MRUImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For3x996MRUImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMCS15For3x996MRUImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMCS15For3x996MRUImplemented;
}


static inline void mac_mib_set_EHTDupImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTDupImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTDupImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTDupImplemented;
}


static inline void mac_mib_set_EHTSupportFor242ToneRUInBWWiderThan20Implemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSupportFor242ToneRUInBWWiderThan20Implemented = en_val;
}


static inline uint8_t mac_mib_get_EHTSupportFor242ToneRUInBWWiderThan20Implemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSupportFor242ToneRUInBWWiderThan20Implemented;
}


static inline void mac_mib_set_EHT20MHzOperatingSTARxNDPwithWiderBWImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHT20MHzOperatingSTARxNDPwithWiderBWImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHT20MHzOperatingSTARxNDPwithWiderBWImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHT20MHzOperatingSTARxNDPwithWiderBWImplemented;
}


static inline void mac_mib_set_EHTSUBeamformerImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSUBeamformerImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTSUBeamformerImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSUBeamformerImplemented;
}


static inline void mac_mib_set_EHTSUBeamformeeImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSUBeamformeeImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTSUBeamformeeImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTSUBeamformeeImplemented;
}


static inline void mac_mib_set_EHTMUBeamformerLessThanOrEqualTo80Implemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUBeamformerLessThanOrEqualTo80Implemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMUBeamformerLessThanOrEqualTo80Implemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUBeamformerLessThanOrEqualTo80Implemented;
}


static inline void mac_mib_set_EHTMUBeamformerEqualTo160Implemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUBeamformerEqualTo160Implemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMUBeamformerEqualTo160Implemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUBeamformerEqualTo160Implemented;
}


static inline void mac_mib_set_EHTMUBeamformerEqualTo320Implemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUBeamformerEqualTo320Implemented = en_val;
}


static inline uint8_t mac_mib_get_EHTMUBeamformerEqualTo320Implemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMUBeamformerEqualTo320Implemented;
}


static inline void mac_mib_set_EHTPartialBWDLMUMIMOImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPartialBWDLMUMIMOImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTPartialBWDLMUMIMOImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTPartialBWDLMUMIMOImplemented;
}


static inline void mac_mib_set_EHTTriggeredSUBeamformingFeedbackImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTriggeredSUBeamformingFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTTriggeredSUBeamformingFeedbackImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTriggeredSUBeamformingFeedbackImplemented;
}


static inline void mac_mib_set_EHTTriggeredMUBeamformingPartialBWFeedbackImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTriggeredMUBeamformingPartialBWFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTTriggeredMUBeamformingPartialBWFeedbackImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTriggeredMUBeamformingPartialBWFeedbackImplemented;
}


static inline void mac_mib_set_EHTTriggeredCQIFeedbackImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTriggeredCQIFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTTriggeredCQIFeedbackImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTTriggeredCQIFeedbackImplemented;
}


static inline void mac_mib_set_EHTNonTriggeredCQIFeedbackImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonTriggeredCQIFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTNonTriggeredCQIFeedbackImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNonTriggeredCQIFeedbackImplemented;
}


static inline void mac_mib_set_EHTNG16SUFeedbackImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNG16SUFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTNG16SUFeedbackImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNG16SUFeedbackImplemented;
}


static inline void mac_mib_set_EHTNG16MUFeedbackImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNG16MUFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTNG16MUFeedbackImplemented(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNG16MUFeedbackImplemented;
}


static inline void mac_mib_set_EHTCodebookSizePhi4Psi2SUFeedbackImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTCodebookSizePhi4Psi2SUFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTCodebookSizePhi4Psi2SUFeedbackImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTCodebookSizePhi4Psi2SUFeedbackImplemented;
}


static inline void mac_mib_set_EHTCodebookSizePhi7Psi5MUFeedbackImplemented(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTCodebookSizePhi7Psi5MUFeedbackImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTCodebookSizePhi7Psi5MUFeedbackImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTCodebookSizePhi7Psi5MUFeedbackImplemented;
}


static inline void mac_mib_set_EHTNDPwith4xEHTLTFand3point2GIImplemented(wlan_mib_ieee802dot11_stru *mib_info,
    oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNDPwith4xEHTLTFand3point2GIImplemented = en_val;
}


static inline uint8_t mac_mib_get_EHTNDPwith4xEHTLTFand3point2GIImplemented(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNDPwith4xEHTLTFand3point2GIImplemented;
}


static inline void mac_mib_set_EHTMaxNumberOfSupportedEHTLTFsForSU(wlan_mib_ieee802dot11_stru *mib_info,
    uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMaxNumberOfSupportedEHTLTFsForSU = val;
}


static inline uint8_t mac_mib_get_EHTMaxNumberOfSupportedEHTLTFsForSU(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMaxNumberOfSupportedEHTLTFsForSU;
}


static inline void mac_mib_set_EHTMaxNumberOfSupportedEHTLTFsForMUandNDP(wlan_mib_ieee802dot11_stru *mib_info,
    uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMaxNumberOfSupportedEHTLTFsForMUandNDP = val;
}


static inline uint8_t mac_mib_get_EHTMaxNumberOfSupportedEHTLTFsForMUandNDP(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMaxNumberOfSupportedEHTLTFsForMUandNDP;
}


static inline void mac_mib_set_EHTBeamformeeSSLessThanOrEqualTo80(wlan_mib_ieee802dot11_stru *mib_info,
    uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTBeamformeeSSLessThanOrEqualTo80 = val;
}


static inline uint8_t mac_mib_get_EHTBeamformeeSSLessThanOrEqualTo80(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTBeamformeeSSLessThanOrEqualTo80;
}


static inline void mac_mib_set_EHTBeamformeeSSEqualTo160(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTBeamformeeSSEqualTo160 = val;
}


static inline uint8_t mac_mib_get_EHTBeamformeeSSEqualTo160(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTBeamformeeSSEqualTo160;
}


static inline void mac_mib_set_EHTBeamformeeSSEqualTo320(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTBeamformeeSSEqualTo320 = val;
}


static inline uint8_t mac_mib_get_EHTBeamformeeSSEqualTo320(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTBeamformeeSSEqualTo320;
}


static inline void mac_mib_set_EHTNumberSoundingDimensionsLessThanOrEqualTo80(
    wlan_mib_ieee802dot11_stru *mib_info, uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNumberSoundingDimensionsLessThanOrEqualTo80 = val;
}


static inline uint8_t mac_mib_get_EHTNumberSoundingDimensionsLessThanOrEqualTo80(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNumberSoundingDimensionsLessThanOrEqualTo80;
}


static inline void mac_mib_set_EHTNumberSoundingDimensionsEqualTo160(wlan_mib_ieee802dot11_stru *mib_info,
    uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNumberSoundingDimensionsEqualTo160 = val;
}


static inline uint8_t mac_mib_get_EHTNumberSoundingDimensionsEqualTo160(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNumberSoundingDimensionsEqualTo160;
}


static inline void mac_mib_set_EHTNumberSoundingDimensionsEqualTo320(wlan_mib_ieee802dot11_stru *mib_info,
    uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNumberSoundingDimensionsEqualTo320 = val;
}


static inline uint8_t mac_mib_get_EHTNumberSoundingDimensionsEqualTo320(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTNumberSoundingDimensionsEqualTo320;
}

static inline void mac_mib_set_Rx1024QamInWiderBandwidthDlOfdmaSupport(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.en_dot11Rx1024QamInWiderBandwidthDlOfdmaSupport = en_val;
}

static inline uint8_t mac_mib_get_Rx1024QamInWiderBandwidthDlOfdmaSupport(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.en_dot11Rx1024QamInWiderBandwidthDlOfdmaSupport;
}

static inline void mac_mib_set_Rx4096QamInWiderBandwidthDlOfdmaSupport(
    wlan_mib_ieee802dot11_stru *mib_info, oal_bool_enum_uint8 en_val)
{
    mib_info->wlan_mib_eht_phy_cap_config.en_dot11Rx4096QamInWiderBandwidthDlOfdmaSupport = en_val;
}

static inline uint8_t mac_mib_get_Rx4096QamInWiderBandwidthDlOfdmaSupport(
    const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.en_dot11Rx4096QamInWiderBandwidthDlOfdmaSupport;
}


static inline void mac_mib_set_EHTMaxNc(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val)
{
    mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMaxNc = val;
}


static inline uint8_t mac_mib_get_EHTMaxNc(const wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_phy_cap_config.dot11EHTMaxNc;
}


static inline void mac_mib_set_eht_Nss(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val)
{
    mib_info->wlan_mib_eht_ppe_thresholds_config.dot11Nss = val;
}


static inline uint8_t mac_mib_get_eht_Nss(wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_ppe_thresholds_config.dot11Nss;
}


static inline void mac_mib_set_eht_RuIndexMask(wlan_mib_ieee802dot11_stru *mib_info, uint8_t en_val)
{
    mib_info->wlan_mib_eht_ppe_thresholds_config.dot11RuIndexMask = en_val;
}


static inline uint8_t mac_mib_get_eht_RuIndexMask(wlan_mib_ieee802dot11_stru *mib_info)
{
    return mib_info->wlan_mib_eht_ppe_thresholds_config.dot11RuIndexMask;
}


static inline void mac_mib_set_eht_NssRuPpetx(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val,
    uint8_t nss_idx, uint8_t ru_idx)
{
    if (nss_idx > MAC_EHT_MAX_PPET_NSS || ru_idx >=  MAC_EHT_PPET_RU_BITMASK_FIELD_BIT) {
        return;
    }
    mib_info->wlan_mib_eht_ppe_thresholds_config.dot11NssRuPpetx[nss_idx][ru_idx] = val;
}


static inline uint8_t mac_mib_get_eht_NssRuPpetx(wlan_mib_ieee802dot11_stru *mib_info, uint8_t nss_idx,
    uint8_t ru_idx)
{
    if (nss_idx > MAC_EHT_MAX_PPET_NSS || ru_idx >=  MAC_EHT_PPET_RU_BITMASK_FIELD_BIT) {
        return 0;
    }
    return mib_info->wlan_mib_eht_ppe_thresholds_config.dot11NssRuPpetx[nss_idx][ru_idx];
}



static inline void mac_mib_set_eht_NssRuPpet8(wlan_mib_ieee802dot11_stru *mib_info, uint8_t val,
    uint8_t nss_idx, uint8_t ru_idx)
{
    if (nss_idx > MAC_EHT_MAX_PPET_NSS || ru_idx >=  MAC_EHT_PPET_RU_BITMASK_FIELD_BIT) {
        return;
    }
    mib_info->wlan_mib_eht_ppe_thresholds_config.dot11NssRuPpet8[nss_idx][ru_idx] = val;
}


static inline uint8_t mac_mib_get_eht_NssRuPpet8(wlan_mib_ieee802dot11_stru *mib_info, uint8_t nss_idx,
    uint8_t ru_idx)
{
    if (nss_idx > MAC_EHT_MAX_PPET_NSS || ru_idx >=  MAC_EHT_PPET_RU_BITMASK_FIELD_BIT) {
        return 0;
    }
    return mib_info->wlan_mib_eht_ppe_thresholds_config.dot11NssRuPpet8[nss_idx][ru_idx];
}

#endif /* end of _PRE_WLAN_FEATURE_11BE */
#endif /* end of mac_mib_sta_eht_cfg.h */
