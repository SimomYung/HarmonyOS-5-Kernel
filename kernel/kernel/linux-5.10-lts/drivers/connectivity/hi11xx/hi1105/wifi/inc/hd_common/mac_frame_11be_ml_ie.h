/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 用于定义11be相关内容
 * 创建日期   : 2022年2月11日
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#ifndef __MAC_FRAME_11BE_ML_IE_H__
#define __MAC_FRAME_11BE_ML_IE_H__

#ifdef _PRE_WLAN_FEATURE_ML
#include "wlan_types.h"
#include "oam_ext_if.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

#define DELETE_TIMER_FIELD_LEN 2
#define STA_CONTROL_FIELD_LEN  2
#define MLD_CAPBILITIES_FIELD_LEN  2
#define MLD_ID_FIELD_LEN    1
#define EML_CAPBILITIES_FIELD_LEN  2
#define MEDIUM_SYNCHRONIZATION_DELAY_INFORMATION_LEN 2
#define ML_DTIM_INFO_LEN   2
#define ML_TSF_OFFSET      8
#define BSS_PARAMETERS_CHANGE_COUNT_LEN 1
#define ML_CONTROL_FIELD_LEN  2
#define ML_LINK_ID_INFO_FIELD_LEN  1
#define ML_STA_INFO_OFFSET   4
#define ML_STA_INFO_LENGTH   1
#define ML_STA_CONTROL_OFFSET   2
#define AUTH_MLD_IE_LEN         12
#define ML_COMMON_INFO_LEN      1

#define IE_FRAGMENT_THRESHOLD   255
#define EXT_IE_FRAGMENT_THRESHOLD   254
#define MAX_ML_IE_LEN   1024
#define MAX_COMMON_INFO_LEN 18
#define EMLSR_OMN_LINK_BITMAP_LEN 2
/* 不包含sta-profile时的ml ie最大长度 */
#define MAX_NON_PER_STA_PROFILE_ML_IE_LEN (MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + MAX_COMMON_INFO_LEN)

/* Reduced Neighbor Report element 相关宏定义 */
#define MIN_REDUCED_NEIGHBOR_REPORT_IE_LEN  20 /* 含有MLD Parameters字段的reduced neighbor report ie的最小长度 */
#define TBTT_INFO_TYPE0 0
#define TBTT_INFO_INCLUDE_MLD_LEN_TYPE0       16  /* TBTT Information字段中含有MLD Parameters时的长度 */
#define TBTT_INFO_MLD_PARAM_OFFSET 13
#define TBTT_INFO_TYPE1 1
#define TBTT_INFO_INCLUDE_MLD_LEN_TYPE1       3  /* TBTT Information字段中含有MLD Parameters时的长度 */
#define NEIGHBOR_AP_MLD_PARAMETERS_OFFSET       17  /* Neighbor AP Information字段中MLD Parameters字段的offset */
#define NEIGHBOR_AP_BSS_PARAM_OFFSET            15
#define NEIGHBOR_AP_BSSID_OFFSET    5
#define NEIGHBOR_AP_CHANNEL_NUM_OFFSET    3
#define NEIGHBOR_AP_TBTT_INFO_SET_OFFSET    4
#define EMLSR_ACTION_VERABLE_LEN 3

/* 此文件中定义的结构体与协议相关，需要1字节对齐 */
typedef enum {
    BASIC_MULTI_LINK            = 0,
    PROBE_REQUEST_MULTI_LINK    = 1,
    RECONFIGURATION_MULTI_LINK  = 2,
    MULTI_LINK_TYPE_BUTT
} mac_ml_type_enum;

typedef enum {
    ML_SUBEID_PER_STA_PROFILE    = 0,
    ML_SUBEID_VENDOR_SPECIFIC    = 221,
    ML_SUBDID_BUTT
} mac_ml_subelement_id_enum;

typedef enum {
    EMLSR_PADDING_DELAY_0_US = 0,
    EMLSR_PADDING_DELAY_32_US = 1,
    EMLSR_PADDING_DELAY_64_US = 2,
    EMLSR_PADDING_DELAY_128_US = 3,
    EMLSR_PADDING_DELAY_256_US = 4,
    EMLSR_PADDING_DELAY_BUTT
} mac_emlsr_padding_delay_value_enum;

typedef enum {
    EMLSR_TRANSITION_DELAY_0_US = 0,
    EMLSR_TRANSITION_DELAY_16_US = 1,
    EMLSR_TRANSITION_DELAY_32_US = 2,
    EMLSR_TRANSITION_DELAY_64_US = 3,
    EMLSR_TRANSITION_DELAY_128_US = 4,
    EMLSR_TRANSITION_DELAY_256_US = 5,
    EMLSR_TRANSITION_DELAY_BUTT
} mac_emlsr_transition_delay_value_enum;

typedef enum {
    EMLMR_PADDING_DELAY_0_US = 0,
    EMLMR_PADDING_DELAY_32_US = 1,
    EMLMR_PADDING_DELAY_64_US = 2,
    EMLMR_PADDING_DELAY_128_US = 3,
    EMLMR_PADDING_DELAY_256_US = 4,
    EMLMR_PADDING_DELAY_BUTT
} mac_emlmr_delay_value_enum;

typedef enum {
    TRANSITION_TIMEOUT_0_TUS = 0,
    TRANSITION_TIMEOUT_128_US = 1,
    TRANSITION_TIMEOUT_256_US = 2,
    TRANSITION_TIMEOUT_512_US = 3,
    TRANSITION_TIMEOUT_1_TUS = 4,
    TRANSITION_TIMEOUT_2_TUS = 5,
    TRANSITION_TIMEOUT_4_TUS = 6,
    TRANSITION_TIMEOUT_8_TUS = 7,
    TRANSITION_TIMEOUT_16_TUS = 8,
    TRANSITION_TIMEOUT_32_TUS = 9,
    TRANSITION_TIMEOUT_64_TUS = 10,
    TRANSITION_TIMEOUT_BUTT
} mac_transition_timeout_enum;

typedef enum {
    MLD_ASSOC_MODE_MLSR  = 0,
    MLD_ASSOC_MODE_STR   = 1,

    MLD_ASSOC_MODE_BUTT
} mld_assoc_mode_enum; /* MLD关联入网模式枚举类型 */

typedef enum {
    LINK_SWITCH_FOR_HIPRIV    = 0,
    LINK_SWITCH_FOR_IOCTL     = 1,
    LINK_SWITCH_FOR_ALG       = 2,
    LINK_SWITCH_FOR_RECONFIG  = 3,
    LINK_SWITCH_FOR_TTLM      = 4,
    LINK_SWITCH_FOR_ROAM      = 5,
    LINK_SWITCH_TIMEOUT       = 100,    /* host未收到device上报NULL发送事件 */
    LINK_SWITCH_NO_SWITCH     = 101,    /* host判断此时不允许切换 */

    LINK_SWITCH_REASON_BUTT
} mlsr_link_switch_reason_enum; /* MLSR link切换触发源 枚举类型 */

typedef enum {
    EMLSR_OPEN_EVENT    = 0,
    EMLSR_CLOSE_EVENT    = 1,

    EMLSR_EVENT_BUTT
} emlsr_switch_event_enum; /* emlsr事件 枚举类型 */
typedef enum {
    EMLSR_ENTER_HIPRIV      = 0,
    EMLSR_ENTER_IOCTRL      = 1,

    EMLSR_ENTER_REASON_BUTT
} emlsr_enter_reason_enum;

typedef enum {
    EMLSR_CLOSE_INVALID      = 0,
    EMLSR_CLOSE_FOR_HIPRIV   = 1,
    EMLSR_CLOSE_FOR_IOCTL    = 2,
    EMLSR_CLOSE_FOR_MVAP     = 3,
    EMLSR_CLOSE_FOR_ROAM     = 4,
    EMLSR_CLOSE_FOR_LINKLOSS = 5,
    EMLSR_CLOSE_FOR_BT       = 6,
    EMLSR_CLOSE_FOR_RECONFIG = 7,
    EMLSR_CLOSE_FOR_TTLM     = 8,
    EMLSR_CLOSE_FOR_RECONNECT = 9,
    EMLSR_CLOSE_FOR_DEVICE_REJECT = 10,

    EMLSR_CLOSE_REASON_BUTT
} emlsr_close_reason_enum; /* emlsr退出事件原因枚举类型 */

typedef enum {
    EMLSR_CLOSED       = 0,
    EMLSR_OPENED       = 1,
    EMLSR_WAIT_OPEN    = 2,
    EMLSR_PARAM_UPDATE = 3,
    EMLSR_WAIT_CLOSE   = 4,

    EMLSR_STATE_BUTT
} emlsr_state_enum; /* MLSR link切换触发源 枚举类型 */

/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/* 用#pragma pack(1)/#pragma pack()方式达到一字节对齐 */
#pragma pack(1)
typedef union {
    struct {
        uint16_t    ml_control_type             : 3,
                    ml_control_resv             : 1,
                    ml_link_id_info_present     : 1,
                    ml_bss_parameters_change_count_present              : 1,
                    ml_medium_synchronization_delay_information_present : 1,
                    ml_eml_capabilities_present : 1,
                    ml_mld_capabilities_and_operations_present : 1,
                    ml_mld_id_present           : 1,
                    reserved                    : 6;
    } basic_ml_control;
    struct {
        uint16_t    ml_control_type             : 3,
                    ml_control_resv             : 1,
                    ml_mld_id_present           : 1,
                    reserved                    : 11;
    } probe_request_ml_control;
    struct {
        uint16_t    ml_control_type             : 3,
                    ml_control_resv             : 1,
                    ml_mld_mac_address_present  : 1,
                    ml_eml_capabilities_present  : 1,
                    ml_mld_capabilities_and_operations_present : 1,
                    reserved                    : 9;
    } reconfiguration_ml_control;
} mac_ml_control_union;

typedef struct {
    uint8_t                 eid;  /* element ID */
    uint8_t                 ie_len; /* length in bytes */
    uint8_t                 eid_extension;
    mac_ml_control_union    ml_control;   /* 不同类型的bitmap不同，单独定义ML Control */
} mac_eht_ml_ie_header_stru;

typedef struct {
    uint8_t common_info_len;
    uint8_t mld_id;
} mac_probe_req_common_info_stru;

typedef struct {
    uint8_t     link_id             : 4,
                reserved            : 4;
} mac_ml_link_id_info_stru;

typedef struct {
    uint16_t     medium_sync_duration           : 8,
                 medium_sync_ofdm_ed_threshold  : 4,
                 medium_sync_max_num_of_txops   : 4;
} mac_ml_medium_sync_delay_info_stru;

typedef struct {
    uint16_t     emlsr_support                  : 1, // 协商时确定，无法改变
                 emlsr_padding_delay            : 3, // STA关联及OMN宣称
                 emlsr_transition_delay         : 3, // STA关联及OMN宣称
                 emlmr_support                  : 1,
                 emlmr_delay                    : 3,
                 transition_timeout             : 4, // STA关联时AP宣称
                 reserved                       : 1;
} mac_eml_capabilities_stru;

typedef struct {
    uint16_t     maximum_number_of_simultaneous_links          : 4,
                 srs_support                                   : 1,
                 tid_to_link_mapping_negotiation_supported     : 2,
                 frequency_separation_for_str                  : 5,
                 aar_supplort                                  : 1,
                 link_reconfiguration_operation_support        : 1,
                 reserved                                      : 2;
} mac_mld_capabilities_stru;

typedef struct {
    uint8_t     dtim_count;
    uint8_t     dtim_period;
} mac_ml_dtim_info_stru;

typedef union {
    struct {
        uint16_t    link_id                    : 4,
                    complete_profile           : 1,
                    sta_mac_address_present    : 1,
                    beacon_interval_present    : 1,
                    tsf_offset_present         : 1,
                    dtim_info_present          : 1,
                    nstr_link_pair_present     : 1,
                    nstr_bitmap_size_present            : 1,
                    bss_parameters_change_count_present : 1,
                    reserved                            : 4;
    } basic_ml_sta_control;
    struct {
        uint16_t    link_id                    : 4,
                    complete_profile           : 1,
                    reserved                   : 11;
    } probe_request_ml_sta_control;
    struct {
        uint16_t    link_id                    : 4,
                    complete_profile           : 1,
                    mac_address_present        : 1,
                    delete_timer_present       : 1,
                    reconfiguration_operation_type     : 4,
                    operation_parameters_present       : 1,
                    nstr_bitmap_size                   : 1,
                    nstr_indication_bitmap_present     : 1,
                    reserved                   : 2;
    } reconfiguration_ml_sta_control;
} mac_ml_sta_control_union;

typedef struct {
    uint8_t                     sub_eid;  /* subelement ID */
    uint8_t                     ie_len; /* length in bytes */
    mac_ml_sta_control_union    ml_control;   /* 不同类型的bitmap不同，单独定义ML Control */
} mac_ml_per_sta_profiler_stru;

typedef struct {
    uint8_t         mld_id;
    uint16_t        link_id                        : 4,
                    bss_parameters_changel_count   : 8,
                    all_updates_included           : 1,
                    reserved                       : 3;
} mac_mld_paramters_stru;

typedef struct {
    uint8_t         emlsr_mode                     : 1,
                    emlmr_mode                     : 1,
                    emlsr_param_update_control     : 1,
                    indevice_coex_activities       : 1,
                    reserved                       : 4;
    uint16_t        emlsr_emlmr_link_bitmap;
    uint8_t         emlsr_padding_delay            : 3,
                    emlsr_transition_delay         : 3,
                    reserved1                      : 2;
} mac_emlsr_omn_stru;

/* Neighbor Report element的TBTT Information字段 */
typedef struct {
    uint16_t tbtt_infomation_field_type : 2,
             filtered_neighbor_ap       : 1,
             resevered                  : 1,
             tbtt_infomation_count      : 4,
             tbtt_infomation_length     : 8;
} mac_tbtt_infomation_header_stru;

/* Neighbor Report element的BSS Parameter字段 */
typedef struct {
    uint8_t oct_recommended                       : 1,
            same_ssid                             : 1,
            multiple_bssid                        : 1,
            transmitted_bssid                     : 1,
            member_of_ess_with_2g_5g_colocated_ap : 1,
            unsolicited_probe_responses_active    : 1,
            colocated_ap                          : 1,
            resevered                             : 1;
} mac_bss_parameters_subfield_stru;

typedef struct {
    uint8_t     direction                    : 2,
                default_link_mapping         : 1,
                mapping_switch_time_present  : 1,
                expected_duration_present    : 1,
                link_mapping_size            : 1,
                reserved                     : 2;
} fixed_ttlm_control;

typedef struct {
    uint16_t    direction                    : 2,
                default_link_mapping         : 1,
                mapping_switch_time_present  : 1,
                expected_duration_present    : 1,
                link_mapping_size            : 1,
                reserved                     : 2,
                link_mapping_presence_bitmap : 8;
} ttlm_control;
#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

#endif
