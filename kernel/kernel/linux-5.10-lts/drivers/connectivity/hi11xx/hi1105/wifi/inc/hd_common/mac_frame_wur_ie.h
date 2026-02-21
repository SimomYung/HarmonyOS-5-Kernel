/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 用于定义wur相关内容
 * 创建日期   : 2022年2月11日
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#ifndef __MAC_FRAME_WUR_IE_H__
#define __MAC_FRAME_WUR_IE_H__

#ifdef _PRE_WLAN_FEATURE_WUR
#include "wlan_types.h"
#include "oam_ext_if.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

#define WUR_GROUP_ID_OFFSET 32
#define MAC_WUR_CAP_IE_LEN  4           /* 1字节EXT_EID + 1字节support bands + 2字节wur cap info */
#define MAC_WUR_OPER_IE_LEN 11          /* 1字节EXT_EID + 10字节wur operation parameters */

/* 1字节EXT_EID + 1字节aciton type + 1字节resp status + 1字节params control + 最少6字节wur params */
#define MAC_WUR_MODE_STA_MIN_IE_LEN 10
/* 1字节EXT_EID + 1字节aciton type + 1字节resp status + 1字节params control + 最少10字节wur params */
#define MAC_WUR_MODE_AP_MIN_IE_LEN 14
/* 1字节EXT_EID + 1字节aciton type + 1字节resp status + 1字节params control + 2字节wur params */
#define MAC_WUR_MODE_AP_REJECT_IE_LEN 6

#define MAC_WUR_SETUP_FIXED_LEN 3 /* setup帧的fixed字段 category + wur action + dialog token */
#define MAC_WUR_MAC_HEADER_LEN 4
#define MAC_SHORT_WUR_WAKEUP_FRAME_LEN 2 /* short wake up 帧长，不包含fcs */

typedef enum {
    ENTER_WUR_MODE_REQ           = 0,
    ENTER_WUR_MODE_RESP          = 1,
    ENTER_WUR_MODE_SUSPEND_REQ   = 2,
    ENTER_WUR_MODE_SUSPEND_RESP  = 3,
    ENTER_WUR_MODE_SUSPEND       = 4,
    ENTER_WUR_MODE               = 5,
    WUR_SETUP_TYPE_BUTT
} wur_setup_type_enum;

typedef enum {
    WUR_ACCEPT                                         = 0,
    WUR_DENIED_UNSPECAFIED_REASON                      = 1,
    WUR_DENIED_PREFERRED_DUTY_CYCLE_PERIOD_TOO_LARGE   = 2,
    WUR_MODE_RESP_STATUS_BUTT
} wur_mode_resp_status_enum;

typedef enum {
    WUR_GROUP_ID_BITMAP_SIZE_0    = 0, /* 表示wur group id bitmap字段不存在 */
    WUR_GROUP_ID_BITMAP_SIZE_16   = 1, /* 表示wur group id bitmap field包含一个16-bit bitmap */
    WUR_GROUP_ID_BITMAP_SIZE_32   = 2, /* 表示wur group id bitmap field包含一个32-bit bitmap */
    WUR_GROUP_ID_BITMAP_SIZE_64   = 3, /* 表示wur group id bitmap field包含一个64-bit bitmap */
    WUR_GROUP_ID_BITMAP_BUTT
} wur_group_id_bitmap_size_enum;

typedef enum {
    WUR_BEACON_FRAME           = 0,
    WUR_WAKE_UP_FRAME          = 1,
    WUR_VENDOR_SPECIFIC_FRAME  = 2,
    WUR_DISCOVERY_FRAME        = 3,
    WUR_FRAME_TYPE_BUTT
} wur_frame_type_enum;

typedef enum {
    WUR_WAKE_UP_FRAME_RATE_NO_RECOMMENDED  = 0, /* 没有推荐的wake up帧速率 */
    WUR_WAKE_UP_FRAME_RATE_LDR             = 1, /* sta推荐ap发的wake up帧速率为LOW DATA RATE */
    WUR_WAKE_UP_FRAME_RATE_HDR             = 2, /* sta推荐ap发的wake up帧速率为HIGH DATA RATE */
    WUR_WAKE_UP_FRAME_RATE_BUTT
} wur_recommended_wake_up_frame_rate_enum; /* 仅sta使用 */

typedef enum {
    WUR_WAKE_UP_FRAME_IN_PRIMARY_CHANNEL = 0,                 /* wake up帧和wur beacon在同一个主信道 */
    WUR_WAKE_UP_FRAME_IN_FH_RELATIVE_TO_PRIMARY_CHANNEL = 1, /* fisrt higher 20MHZ  */
    WUR_WAKE_UP_FRAME_IN_FL_RELATIVE_TO_PRIMARY_CHANNEL = 2, /* fisrt lower 20MHZ  */
    WUR_WAKE_UP_FRAME_IN_SH_RELATIVE_TO_PRIMARY_CHANNEL = 3, /* second higher 20MHZ  */
    WUR_WAKE_UP_FRAME_IN_SL_RELATIVE_TO_PRIMARY_CHANNEL = 4, /* second lower 20MHZ  */
    WUR_WAKE_UP_FRAME_IN_TH_RELATIVE_TO_PRIMARY_CHANNEL = 5, /* third higher 20MHZ  */
    WUR_WAKE_UP_FRAME_IN_TL_RELATIVE_TO_PRIMARY_CHANNEL = 6,  /* third lower 20MHZ  */

    WUR_WAKE_UP_FRAME_CHANNEL_OFFSET_BUTT
} wur_channel_offset_enum;

typedef enum {
    MAC_WUR_GROUP_ID_BITMAP_SIZE_0BIT   = 0,
    MAC_WUR_GROUP_ID_BITMAP_SIZE_16BIT  = 1,
    MAC_WUR_GROUP_ID_BITMAP_SIZE_32BIT  = 2,
    MAC_WUR_GROUP_ID_BITMAP_SIZE_64BIT  = 3,
    MAC_WUR_GROUP_ID_BITMAP_SIZE_BUTT
} wur_group_bitmap_size_enum;

/*****************************************************************************
  STRUCT定义
*****************************************************************************/
/* 用#pragma pack(1)/#pragma pack()方式达到一字节对齐 */
#pragma pack(1)
/* wur beacon帧头结构 */
typedef struct {
    uint32_t type                   : 3,  /* wur帧类型，参见wur_frame_type_enum */
             is_protected           : 1,  /* 是否mic保护帧 */
             length_present         : 1,  /* 是否包含length of frame body field */
             resv                   : 3,
             transmitter_id         : 12, /* transmitter ID */
             partial_tsf            : 12; /* 部分tsf，用于sta同步tsf，达到对齐on duration的窗口的目的 */
} wur_beacon_mac_header_stru; /* wur beacon没有frame body */

/* wur wake up帧头结构 */
typedef struct {
    uint32_t type                   : 3,  /* wur帧类型，参见wur_frame_type_enum */
             is_protected           : 1,  /* 是否mic保护帧 */
             length_present         : 1,  /* 是否包含length of frame body field */
             group_addressed_bu     : 1,
             rsv                    : 2,
             id                     : 12,
             counter                : 4,
             sequence_num           : 8;
} wur_wake_up_mac_header_stru;

/* wur 通用mac帧头结构，适用于帧类型wur_frame_type_enum */
typedef struct {
    uint32_t type                   : 3,  /* wur帧类型，参见wur_frame_type_enum */
             is_protected           : 1,  /* 是否mic保护帧 */
             length_present         : 1,  /* 是否包含length of frame body field */
             length_or_misc         : 3,
             id                     : 12, /* 各种id，不同的wur帧类型，使用不同的id */
             type_dependent_control : 12; /* 不同的wur帧类型，含义不同 */
} wur_mac_header_stru;

typedef struct {
    uint16_t fcs;
} wur_fcs_stru;

typedef struct {
    uint16_t type                   : 3,  /* wur帧类型，参见wur_frame_type_enum */
             is_protected           : 1,  /* 是否mic保护帧 */
             wur_id                 : 12; /* wur id */
} wur_short_wake_up_frame_stru;

typedef struct {
    uint8_t support_2g : 1,
            support_5g : 1,
            resv       : 6;
} wur_support_band_stru;

typedef struct {
    uint8_t transition_delay;                /* sta从wur doze到awake需要的最大时间，等于256*（本字段 + 1）us */
    uint8_t vl_wur_frame_support        : 1, /* 是否支持可变长的wur帧 */
            wur_group_ids_support       : 2, /* 0:not support, 1:最大16, 2:最大32, 3:最大64 */
            resv                        : 1,
            hdr_support                 : 1, /* 支持接收20M wur ppdu with hdr（high data rate） */
            fdma_chan_switch_support    : 1, /* 支持wur beaocn和wur wake-up帧在不同的信道上 */
            short_wake_up_support       : 1, /* 是否支持wur short wake up帧 */
            rsv                         : 1;
} wur_cap_info_stru;

typedef struct {
    wur_support_band_stru supported_bands;
    wur_cap_info_stru wur_cap_info;
} mac_frame_wur_cap_ie_stru;

typedef struct {
    uint8_t min_wake_up_duration; /* 单位：256us */
    uint16_t duty_cycle_period_units; /* 单位：4us */
    uint8_t wur_operating_class;
    uint8_t wur_channel;
    uint16_t wur_beacon_period; /* 单位：TU */
    uint16_t offset_of_twbtt;
    uint8_t counter    : 4, /* 表示广播wur wake up帧中包含的counter字段的值 */
            common_ipn : 1, /* BSS内所有受保护的WUR帧是否使用common PN。0:not support, 1:最大16, 2:最大32, 3:最大64 */
            rsv        : 3;
} mac_frame_wur_oper_ie_stru;

typedef struct {
    uint8_t wur_duty_cycle_start_timer_present : 1, /* 仅ap用，sta发送时固定为0 */
            wur_group_id_list_present          : 1, /* 仅ap用，sta发送时固定为0 */
            recommended_wur_params_present     : 1, /* 仅sta用，ap发送时固定为0 */
            resv                               : 5;
} wur_params_control_stru;

typedef struct {
    uint16_t wur_group_id_bitmap_size : 4,
             starting_wur_group_id    : 12;
} wur_group_id_list_stru; /* 后面可带wur group id bitmap */

typedef struct {
    uint16_t wur_id             : 12,
             wur_channel_offset : 3,
             rsv                : 1;
    uint64_t wur_duty_cycle_start_time; /* 单位：us */
} wur_ap_params_stru; /* 后面可带子字段wur_group_id_list_stru */

typedef struct {
    uint8_t recommended_wur_wake_up_frame_rate : 2, /* sta推荐ap发的wake up帧速率 */
            recommended_wur_channel_offset     : 3, /* sta推荐ap发的wake up帧的信道相对于primary channel的offset */
            requested_keep_alive_wur_frame     : 1, /* 1：sta需要ap在duty cycle service periods发保活wur帧，0：不需要 */
            rsv                                : 2;
} recommended_wur_params_stru;

typedef struct {
    uint32_t on_duration; /* 单位：4us */
    uint16_t duty_cycle_period; /* 本字段*duty_cycle_period_units = 实际的duty_cycle_period */
} wur_sta_params_stru; /* 后面可带子字段recommended_wur_params_stru, 取决于wur_params_control.recommended_wur_params_present */

/* sta后面带wur_sta_params_stru，ap后面带wur_ap_params_stru */
typedef struct {
    uint8_t action_type; /* wur_setup_type_enum  */
    uint8_t wur_mode_resp_status;
    wur_params_control_stru wur_params_control;
} mac_frame_wur_mode_fixed_ie_stru;

typedef struct {
    uint8_t action_type; /* wur_setup_type_enum  */
    uint8_t wur_mode_resp_status;
    wur_params_control_stru wur_params_control;
    wur_ap_params_stru wur_params;
    wur_group_id_list_stru group_id_list;
} mac_frame_wur_mode_ie_ap_stru;

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

#endif
