/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 用于定义11be相关内容
 * 创建日期   : 2023年9月4日
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#ifndef WLAN_MIB_EHT_H
#define WLAN_MIB_EHT_H

#include "oal_types.h"
#include "wlan_types.h"
#include "hd_event.h"

#if defined(_PRE_WLAN_FEATURE_11BE) || defined(_PRE_WLAN_FEATURE_11BE_EXT)
/*****************************************************************************
  2 枚举定义
*****************************************************************************/
/* EHT Operation 支持的带宽枚举 */
typedef enum {
    WLAN_EHT_CHANNEL_WIDTH_20  = 0,    /* 工作在20MHz */
    WLAN_EHT_CHANNEL_WIDTH_40  = 1,    /* 工作在40MHz */
    WLAN_EHT_CHANNEL_WIDTH_80  = 2,    /* 工作在80MHz */
    WLAN_EHT_CHANNEL_WIDTH_160 = 3,    /* 工作在160MHz */
    WLAN_EHT_CHANNEL_WIDTH_320 = 4,    /* 工作在320MHz */

    WLAN_EHT_CHANNEL_WIDTH_BUTT
} wlan_eht_oper_width_enum;

/*****************************************************************************
  3 MIB结构体定义
*****************************************************************************/
/* 参考IEEE P802.11be™/D4.0 附录Annex C部分 */
typedef struct {
    oal_bool_enum_uint8 dot11EHTOptionImplemented : 1;
    oal_bool_enum_uint8 dot11EHTTXOPSharingTFOptionImplemented : 1;
    oal_bool_enum_uint8 dot11EHTNSTRMobileAPMLDImplemented : 1;  /* 未使用 */
    oal_bool_enum_uint8 dot11RestrictedTWTOptionImplemented : 1;
    oal_bool_enum_uint8 en_dot11TwoBQRsOptionImplemented : 1;    /* 未使用 */
    oal_bool_enum_uint8 en_dot11SRSOptionImplemented : 1;        /* 未使用 */
    oal_bool_enum_uint8 en_dot11AAROptionImplemented : 1;        /* 未使用 */
    oal_bool_enum_uint8 en_dot11AlignedTWTOptionImplemented : 1; /* 未使用 */

    oal_bool_enum_uint8 dot11EHTPPEThresholdsRequired : 1;
    oal_bool_enum_uint8 dot11TIDtoLinkMappingActivated : 1;
    oal_bool_enum_uint8 dot11EHTEPCSPriorityAccessActivated : 1; /* 新增，需要对照nsep适配 */
    oal_bool_enum_uint8 dot11MultiLinkActivated : 1;             /* 只定义接口，未使用 */
    oal_bool_enum_uint8 dot11EHTEMLSROptionImplemented : 1;      /* 未使用 */
    oal_bool_enum_uint8 dot11EHTEMLSROptionActivated : 1;        /* 未使用 */
    oal_bool_enum_uint8 dot11EHTEMLMROptionImplemented : 1;      /* 未使用 */
    oal_bool_enum_uint8 dot11EHTEMLMROptionActivated : 1;        /* 未使用 */

    oal_bool_enum_uint8 dot11OperationParameterUpdateImplemented : 1;      /* 未使用 */
    oal_bool_enum_uint8 dot11EHTLinkReconfigurationOperationActivated : 1; /* 未使用 */
    oal_bool_enum_uint8 dot11MultiLinkTrafficIndicationActivated : 1;      /* 未使用 */
    oal_bool_enum_uint8 reserved : 5;

    oal_bool_enum_uint8 dot11EHTBaseLineFeaturesImplementedOnly : 1;   /* 初始化为flase，5x的算法会使用，业务封装解析没有使用 */
    oal_bool_enum_uint8 dot11WNMSleepModeImplemented : 1;              /* 初始化为flase，5x的算法会使用，业务封装解析没有使用 */
    oal_bool_enum_uint8 dot11SCSActivated : 1;                         /* 对应有具体的IE能力位，初始化+解析后设置user能力位 */
    oal_bool_enum_uint8 dot11EHTOmControlSupport : 1;                  /* 对应有具体的IE能力位，初始化+解析后设置user能力位 */
    oal_bool_enum_uint8 dot11EHTMaxAmpduLengthExponent : 1;
    oal_bool_enum_uint8 dot11EHTTrsSupport : 1;
    oal_bool_enum_uint8 dot11TxopReturnSupportInTxopSharingMode2 : 1;
    oal_bool_enum_uint8 reserved2 : 1;

    uint8_t dot11EHTMCSFeedbackOptionImplemented;
    uint32_t dot11MSDTimerDuration;
    uint32_t dot11MSDTXOPMax;
    uint32_t dot11MLDAssociationSAQueryMaximumTimeout;
} wlan_mib_Dot11EHTStationConfigEntry_stru;

/* EHT Opern IE的eht opern info子字段 */
typedef struct {
    oal_bool_enum_uint8 dot11EHTDisabledSubchannelBitmapPresent : 1; /* 0x初始化，未使用 */
    oal_bool_enum_uint8 reserved : 7;
    uint8_t dot11EHTCurrentChannelWidth;
    uint16_t dot11EHTDisabledSubchannelBitmap;
} wlan_mib_Dot11EHTOperationInfo_stru;


typedef struct {
    /* Dot11EHTPhyEntry */
    oal_bool_enum_uint8 dot11EHTSupportFor320MHzImplemented : 1;
    oal_bool_enum_uint8 dot11EHTNonOFDMAULMUMIMOLessThanOrEqualto80Implemented : 1;
    oal_bool_enum_uint8 dot11EHTNonOFDMAULMUMIMOEqualto160Implemented : 1;
    oal_bool_enum_uint8 dot11EHTNonOFDMAULMUMIMOEqualto320Implemented : 1;
    oal_bool_enum_uint8 dot11EHTPartialBWULMUMIMOImplemented : 1;
    oal_bool_enum_uint8 dot11EHTMUPPDUwith4xEHTLTFand0point8usecGIImplemented : 1;
    oal_bool_enum_uint8 dot11EHTPSRBasedSRImplemented : 1;
    oal_bool_enum_uint8 dot11EHTPowerBoostFactorImplemented : 1;

    /* 是否支持采用1024或4096 qam并且小于242 tone ru发送 */
    oal_bool_enum_uint8 dot11EHTTx1024QAMand4096QAMLessThan242ToneRUImplemented : 1;
    /* 是否支持采用1024或4096 qam并且小于242 tone ru接收 */
    oal_bool_enum_uint8 dot11EHTRx1024QAMand4096QAMLessThan242ToneRUImplemented : 1;
    oal_bool_enum_uint8 dot11EHTExtraLTFsImplemented : 1;
    /* 是否支持52+26-tone和106+26-tone MRU中的EHT-MCS 15 */
    oal_bool_enum_uint8 dot11EHTMCS15For52p26and106p26MRUImplemented : 1;
    oal_bool_enum_uint8 dot11EHTMCS15For484p242MRUImplemented : 1;
    oal_bool_enum_uint8 dot11EHTMCS15For996p484and996p484p242MRUImplemented : 1;
    oal_bool_enum_uint8 dot11EHTMCS15For3x996MRUImplemented : 1;
    oal_bool_enum_uint8 dot11EHTDupImplemented : 1;

    oal_bool_enum_uint8 dot11EHTSupportFor242ToneRUInBWWiderThan20Implemented : 1;
    oal_bool_enum_uint8 dot11EHT20MHzOperatingSTARxNDPwithWiderBWImplemented : 1;
    oal_bool_enum_uint8 dot11EHT20MhzOnlyLimitedCapabiltiesSupportImplemented : 1; /* 对比561x，新增mib项 */
    oal_bool_enum_uint8 reserved1 : 5;

    /* Dot11HTTransmitBeamformingConfigEntry */
    oal_bool_enum_uint8 dot11EHTSUBeamformerImplemented : 1;
    oal_bool_enum_uint8 dot11EHTSUBeamformeeImplemented : 1;
    oal_bool_enum_uint8 dot11EHTMUBeamformerLessThanOrEqualTo80Implemented : 1;
    oal_bool_enum_uint8 dot11EHTMUBeamformerEqualTo160Implemented : 1;
    oal_bool_enum_uint8 dot11EHTMUBeamformerEqualTo320Implemented : 1;
    oal_bool_enum_uint8 dot11EHTPartialBWDLMUMIMOImplemented : 1;
    oal_bool_enum_uint8 dot11EHTTriggeredSUBeamformingFeedbackImplemented : 1;
    oal_bool_enum_uint8 dot11EHTTriggeredMUBeamformingPartialBWFeedbackImplemented : 1;

    oal_bool_enum_uint8 dot11EHTTriggeredCQIFeedbackImplemented : 1;
    oal_bool_enum_uint8 dot11EHTNonTriggeredCQIFeedbackImplemented : 1;
    oal_bool_enum_uint8 dot11EHTNG16SUFeedbackImplemented : 1;
    oal_bool_enum_uint8 dot11EHTNG16MUFeedbackImplemented : 1;
    oal_bool_enum_uint8 dot11EHTCodebookSizePhi4Psi2SUFeedbackImplemented : 1;
    oal_bool_enum_uint8 dot11EHTCodebookSizePhi7Psi5MUFeedbackImplemented : 1;
    oal_bool_enum_uint8 dot11EHTNDPwith4xEHTLTFand3point2GIImplemented : 1;
    oal_bool_enum_uint8 reserved2 : 1;

    oal_bool_enum_uint8 en_dot11Rx1024QamInWiderBandwidthDlOfdmaSupport : 1;
    oal_bool_enum_uint8 en_dot11Rx4096QamInWiderBandwidthDlOfdmaSupport : 1;
    oal_bool_enum_uint8 reserved3 : 6;

    /* Dot11EHTPhyEntry */
    uint8_t dot11EHTCurrentChannelWidth;
    uint8_t dot11EHTMaxNumberOfSupportedEHTLTFsForSU;
    uint8_t dot11EHTMaxNumberOfSupportedEHTLTFsForMUandNDP;

    uint32_t dot11MSOFDMEDthreshold;
    uint32_t dot11EHTCurrentChannelCenterFrequencyIndex0;
    uint16_t dot11EHTSupportedEhtMcsAndNssSet20MhzOnlyStaImplemented;
    uint16_t dot11EHTSupportedEhtMcsAndNssSetImplemented;
    uint32_t dot11EHTDisabledSubchannelBitmap;

    /* Dot11HTTransmitBeamformingConfigEntry */
    uint32_t dot11EHTBeamformeeSSLessThanOrEqualTo80;
    uint32_t dot11EHTBeamformeeSSEqualTo160;
    uint32_t dot11EHTBeamformeeSSEqualTo320;

    uint32_t dot11EHTNumberSoundingDimensionsLessThanOrEqualTo80;
    uint32_t dot11EHTNumberSoundingDimensionsEqualTo160;
    uint32_t dot11EHTNumberSoundingDimensionsEqualTo320;

    uint32_t dot11EHTMaxNc;
} wlan_mib_Dot11EHTPhyCapabilities_stru;

typedef struct {
    uint8_t dot11Nss;
    uint8_t dot11RuIndexMask;
    uint8_t dot11NssRuPpetx[MAC_EHT_MAX_PPET_NSS + 1][MAC_EHT_PPET_RU_BITMASK_FIELD_BIT];
    uint8_t dot11NssRuPpet8[MAC_EHT_MAX_PPET_NSS + 1][MAC_EHT_PPET_RU_BITMASK_FIELD_BIT];
} wlan_mib_Dot11EHTPPEThresholds_stru;

#endif /* end of _PRE_WLAN_FEATURE_11BE */
#endif /* end of wlan_mib_eht.h */
