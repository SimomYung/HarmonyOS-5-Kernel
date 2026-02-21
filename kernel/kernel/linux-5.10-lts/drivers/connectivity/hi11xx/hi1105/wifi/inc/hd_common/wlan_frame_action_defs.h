/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 协议帧相关定义的头文件
 * 创建日期   : 2023年2月3日
 */

#ifndef __WLAN_FRAME_ACTION_DEFS_H__
#define __WLAN_FRAME_ACTION_DEFS_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_util.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 宏定义
*****************************************************************************/
/* Action Frames: Category字段枚举 */
typedef enum {
    MAC_ACTION_CATEGORY_SPECMGMT = 0,
    MAC_ACTION_CATEGORY_QOS = 1,
    MAC_ACTION_CATEGORY_DLS = 2,
    MAC_ACTION_CATEGORY_BA = 3,
    MAC_ACTION_CATEGORY_PUBLIC = 4,
    MAC_ACTION_CATEGORY_RADIO_MEASURMENT = 5,
    MAC_ACTION_CATEGORY_FAST_BSS_TRANSITION = 6,
    MAC_ACTION_CATEGORY_HT = 7,
    MAC_ACTION_CATEGORY_SA_QUERY = 8,
    MAC_ACTION_CATEGORY_PROTECTED_DUAL_OF_ACTION = 9,
    MAC_ACTION_CATEGORY_WNM = 10,
    MAC_ACTION_CATEGORY_MESH = 13,
    MAC_ACTION_CATEGORY_MULTIHOP = 14,
#ifdef _PRE_WLAN_FEATURE_WMMAC
    MAC_ACTION_CATEGORY_WMMAC_QOS = 17,
#endif
    MAC_ACTION_CATEGORY_ROBUST_AV_STREAMING = 19,
    MAC_ACTION_CATEGORY_VHT = 21,
    MAC_ACTION_CATEGORY_S1G = 22, /* 802.11ah - 2016 */
    MAC_ACTION_CATEGORY_HE = 30,
    MAC_ACTION_CATEGORY_WUR = 32,
    MAC_ACTION_CATEGORY_PROTECTED_FINE_TIMING = 34,
    MAC_ACTION_CATEGORY_EHT = 36,
    MAC_ACTION_CATEGORY_PROTECTED_EHT = 37,
#ifdef _PRE_WLAN_FEATURE_SAP
    MAC_ACTION_CATEGORY_SAP = 125,
#endif
    MAC_ACTION_CATEGORY_VENDOR_SPECIFIC_PROTECTED = 126,
    MAC_ACTION_CATEGORY_VENDOR = 127
} mac_action_category_enum;
typedef uint8_t mac_category_enum_uint8;
/* 0 Spectrum Management Category下的Action枚举值 */
typedef enum {
    MAC_SPEC_MSMT_REQ = 0,
    MAC_SPEC_MSMT_RPT = 1,
    MAC_SPEC_TPC_REQUEST = 2,
    MAC_SPEC_TPC_RPT = 3,
    MAC_SPEC_CH_SWITCH_ANNOUNCE = 4 /* Channel Switch Announcement */
} mac_specmgmt_action_type_enum;
/* 3 BlockAck Category下的Action值的枚举 */
typedef enum {
    MAC_BA_ACTION_ADDBA_REQ = 0,
    MAC_BA_ACTION_ADDBA_RSP = 1,
    MAC_BA_ACTION_DELBA = 2,

    MAC_BA_ACTION_BUTT
} mac_ba_action_type_enum;
/* 4 Public Category下的Action枚举值 */
typedef enum {
    MAC_PUB_COEXT_MGMT = 0,            /* 20/40 BSS Coexistence Management */
    MAC_PUB_EX_CH_SWITCH_ANNOUNCE = 4, /* Extended Channel Switch Announcement */
    MAC_PUB_VENDOR_SPECIFIC = 9,
    MAC_PUB_GAS_INIT_REQ = 10,    /* public Action: GAS Initial Request(0x0a) */
    MAC_PUB_GAS_INIT_RESP = 11,   /* public Action: GAS Initial Response(0x0b) */
    MAC_PUB_GAS_COMBAK_REQ = 12,  /* public Action: GAS Comeback Request(0x0c) */
    MAC_PUB_GAS_COMBAK_RESP = 13, /* public Action: GAS Comeback Response(0x0d) */
    MAC_PUB_FTM_REQ = 32,
    MAC_PUB_FTM = 33,
    MAC_PUB_FILS_DISCOVERY = 34,
    MAC_PUB_LMR = 47
} mac_public_action_type_enum;
typedef uint8_t mac_public_action_type_enum_uint8;
/* 5 Radio Measurement下的Action枚举值 */
typedef enum {
    MAC_RM_ACTION_RADIO_MEASUREMENT_REQUEST = 0,
    MAC_RM_ACTION_RADIO_MEASUREMENT_REPORT = 1,
    MAC_RM_ACTION_LINK_MEASUREMENT_REQUEST = 2,
    MAC_RM_ACTION_LINK_MEASUREMENT_REPORT = 3,
    MAC_RM_ACTION_NEIGHBOR_REPORT_REQUEST = 4,
    MAC_RM_ACTION_NEIGHBOR_REPORT_RESPONSE = 5
} mac_rm_action_type_enum;
/* 6 FT Category下的Action值的枚举 */
typedef enum {
    MAC_FT_ACTION_REQUEST = 1,
    MAC_FT_ACTION_RESPONSE = 2,
    MAC_FT_ACTION_CONFIRM = 3,
    MAC_FT_ACTION_ACK = 4,
    MAC_FT_ACTION_PRE_AUTH = 100,
    MAC_FT_ACTION_PREAUTH_REQUEST  = 100,
    MAC_FT_ACTION_PREAUTH_RESPONSE = 101,
    MAC_FT_ACTION_BUTT = 200
} mac_ft_action_type_enum;
/* 7 HT Category下的Action值的枚举 */
typedef enum {
    MAC_HT_ACTION_NOTIFY_CHANNEL_WIDTH = 0,
    MAC_HT_ACTION_SMPS = 1,
    MAC_HT_ACTION_PSMP = 2,
    MAC_HT_ACTION_RESERVED = 3,
    MAC_HT_ACTION_CSI = 4,
    MAC_HT_ACTION_NON_COMPRESSED_BEAMFORMING = 5,
    MAC_HT_ACTION_COMPRESSED_BEAMFORMING = 6,
    MAC_HT_ACTION_ASEL_INDICES_FEEDBACK = 7,

    MAC_HT_ACTION_BUTT
} mac_ht_action_type_enum;
/* 8 SA QUERY Category下的Action值的枚举 */
typedef enum {
    MAC_SA_QUERY_ACTION_REQUEST = 0,
    MAC_SA_QUERY_ACTION_RESPONSE = 1
} mac_sa_query_action_type_enum;
#ifdef _PRE_WLAN_FEATURE_11V
/* 10 WNM Category下的Action值的枚举 */
typedef enum {
    MAC_WNM_ACTION_EVENT_REQUEST = 0,
    MAC_WNM_ACTION_EVENT_REPORT = 1,
    MAC_WNM_ACTION_DIALGNOSTIC_REQUEST = 2,
    MAC_WNM_ACTION_DIALGNOSTIC_REPORT = 3,
    MAC_WNM_ACTION_LOCATION_CONF_REQUEST = 4,
    MAC_WNM_ACTION_LOCATION_CONF_RESPONSE = 5,
    MAC_WNM_ACTION_BSS_TRANSITION_MGMT_QUERY = 6,
    MAC_WNM_ACTION_BSS_TRANSITION_MGMT_REQUEST = 7,
    MAC_WNM_ACTION_BSS_TRANSITION_MGMT_RESPONSE = 8,
    MAC_WNM_ACTION_FMS_REQUEST = 9,
    MAC_WNM_ACTION_FMS_RESPONSE = 10,
    MAC_WNM_ACTION_COLLOCATED_INTER_REQUEST = 11,
    MAC_WNM_ACTION_COLLOCATEC_INTER_REPORT = 12,
    MAC_WNM_ACTION_TFS_REQUEST = 13,
    MAC_WNM_ACTION_TFS_RESPONSE = 14,
    MAC_WNM_ACTION_TFS_NOTIFY = 15,
    MAC_WNM_ACTION_SLEEP_MODE_REQUEST = 16,
    MAC_WNM_ACTION_SLEEP_MODE_RESPONSE = 17,
    MAC_WNM_ACTION_TIM_BROADCAST_REQUEST = 18,
    MAC_WNM_ACTION_TIM_BROADCAST_RESPONSE = 19,
    MAC_WNM_ACTION_QOS_TRAFFIC_CAP_UPDATE = 20,
    MAC_WNM_ACTION_CHANNEL_USAGE_REQUEST = 21,
    MAC_WNM_ACTION_CHANNEL_USAGE_RESPONSE = 22,
    MAC_WNM_ACTION_DMS_REQUEST = 23,
    MAC_WNM_ACTION_DMS_RESPONSE = 24,
    MAC_WNM_ACTION_TIMING_MEAS_REQUEST = 25,
    MAC_WNM_ACTION_NOTIFICATION_REQUEST = 26,
    MAC_WNM_ACTION_NOTIFICATION_RESPONSE = 27,

    MAC_WNM_ACTION_BUTT
} mac_wnm_action_type_enum;
#endif  // _PRE_WLAN_FEATURE_11V

/* 19 ROBUST_AV_STREAMING Category下的action的枚举 - 802.11be */
typedef enum {
    MAC_ROBUST_AV_STREAMING_ACTION_SCS_REQ = 0,
    MAC_ROBUST_AV_STREAMING_ACTION_SCS_RSP = 1,
    MAC_ROBUST_AV_STREAMING_ACTION_BUTT
} mac_robust_av_streaming_action_type_enum;

/* 21 VHT Category下的Action值的枚举 */
typedef enum {
    MAC_VHT_ACTION_COMPRESSED_BEAMFORMING = 0,
    MAC_VHT_ACTION_GROUPID_MANAGEMENT = 1,
    MAC_VHT_ACTION_OPREATING_MODE_NOTIFICATION = 2,

    MAC_VHT_ACTION_BUTT
} mac_vht_action_type_enum;

/* 23 S1G Category下的action的枚举 - 802.11ah */
typedef enum {
    MAC_S1G_ACTION_AID_SWITCH_REQ = 0,
    MAC_S1G_ACTION_AID_SWITCH_RESP = 1,
    MAC_S1G_ACTION_SYNC_CONTROL = 2,
    MAC_S1G_ACTION_STA_INFORMATION_ANNOUNCEMENT = 3,
    MAC_S1G_ACTION_EDCA_PARAMETER_SET = 4,
    MAC_S1G_ACTION_EL_OPERATION = 5,
    MAC_S1G_ACTION_TWT_SETUP = 6,
    MAC_S1G_ACTION_TWT_TEARDOWN = 7,
    MAC_S1G_ACTION_SECTORIZED_GROUP_ID_LIST = 8,
    MAC_S1G_ACTION_SECTOR_ID_FEEDBACK = 9,
    MAC_S1G_ACTION_RESERVE = 10,
    MAC_S1G_ACTION_TWT_INFORMATION = 11,

    MAC_SIG_ACTION_BUTT
} mac_s1g_action_type_enum;
/* 30 HE Category下的Action值的枚举 */
typedef enum {
    MAC_HE_ACTION_COMPRESSED_BEAMFORMING_AND_CQI = 0,
    MAC_HE_ACTION_QUICK_TIME_PERIOD = 1,
    MAC_HE_ACTION_OPS = 2,

    MAC_HE_ACTION_BUTT
} mac_he_action_type_enum;
#ifdef _PRE_WLAN_FEATURE_WUR_AP
/* 32 WUR Category下的Action值的枚举 */
typedef enum {
    MAC_WUR_ACTION_WUR_MODE_SETUP = 0,
    MAC_WUR_ACTION_WUR_MODE_TEARDOWN = 1,

    MAC_WUR_ACTION_BUTT
} mac_wur_action_type_enum;
#endif

/* 34 Protected Fine Timing Category下的Action枚举值 */
typedef enum {
    MAC_PFTM_FTMR = 1,            /* protected FTMR */
    MAC_PFTM_FTM = 2,             /* protected FTM  */
    MAC_PFTM_LMR = 3,             /* protected LMR  */
} mac_protected_ftm_action_type_enum;

/* 36 EHT Category下的Action值的枚举 */
typedef enum {
    MAC_EHT_ACTION_COMPRESSED_BEAMFORMING_AND_CQI = 0,

    MAC_EHT_ACTION_BUTT
} mac_eht_action_type_enum;

/* 37 Protected EHT Category下的Action值的枚举 */
typedef enum {
    MAC_PROTECTED_EHT_ACTION_TID_TO_LINK_MAPPING_REQUEST = 0,
    MAC_PROTECTED_EHT_ACTION_TID_TO_LINK_MAPPING_RESPONSE = 1,
    MAC_PROTECTED_EHT_ACTION_TID_TO_LINK_MAPPING_TEARDOWN = 2,
    MAC_PROTECTED_EHT_ACTION_EPCS_PRIORITY_ACCESS_ENABLE_REQUEST = 3,
    MAC_PROTECTED_EHT_ACTION_EPCS_PRIORITY_ACCESS_ENABLE_RESPONSE = 4,
    MAC_PROTECTED_EHT_ACTION_EPCS_PRIORITY_ACCESS_TEARDOWN = 5,
    MAC_PROTECTED_EHT_ACTION_EML_OPERATING_MODE_NOTIFICATION = 6,
    MAC_PROTECTED_EHT_ACTION_LINK_RECOMMENDATION = 7,
    MAC_PROTECTED_EHT_ACTION_MULTI_LINK_OPERATION_UPDATE_REQUEST = 8,
    MAC_PROTECTED_EHT_ACTION_MULTI_LINK_OPERATION_UPDATE_RESPONSE = 9,
    MAC_PROTECTED_EHT_ACTION_LINK_RECONFIGURATION_NOTIFY = 10,
    MAC_PROTECTED_EHT_ACTION_LINK_RECONFIGURATION_REQUEST = 11,
    MAC_PROTECTED_EHT_ACTION_LINK_RECONFIGURATION_RESPONSE = 12,

    MAC_PROTECTED_EHT_ACTION_BUTT
} mac_protected_eht_action_type_enum;

/* 126 vendor protect Category下的Action值的枚举, Hishell(_PRE_WLAN_FEATURE_TBPEKE)使用 */
typedef enum {
    MAC_VENDOR_PROTECT_ACTION_PW_CHANGE_RESP = 0x66,
    MAC_VENDOR_PROTECT_ACTION_PW_SPREAD      = 0x67,

    MAC_VENDOR_PROTECT_ACTION_BUTT
} mac_vendor_protect_action_type_enum;

#ifdef _PRE_WLAN_FEATURE_SAP
typedef enum {
    MAC_SAP_ACTION_INFO_SYNC = 0,
    MAC_SAP_ACTION_DATA_SAVE = 1,
    MAC_SAP_ACTION_DATA_READ_UPLINK = 2,
    MAC_SAP_ACTION_STA_ROAM  = 3,
    MAC_SAP_ACTION_DATA_READ_DOWNLINK = 4,

    MAC_SAP_ACTION_BUTT
} mac_sap_action_type_enum;
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* end of wlan_types_macro.h */
