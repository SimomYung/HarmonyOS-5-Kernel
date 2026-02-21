/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 定义HOST/DEVICE传递psm事件 的头文件
 * 创建日期   : 2022年9月16日
 */

#ifndef HD_EVENT_PSM_H
#define HD_EVENT_PSM_H

#include "mac_common.h"
#include "wlan_spec.h"

/* STA PS 发送参数 */
typedef struct {
    uint8_t uc_vap_ps_mode;
} mac_cfg_ps_mode_param_stru; /* hd_event */

typedef struct {
    uint16_t us_beacon_timeout;
    uint16_t us_tbtt_offset;
    uint16_t us_ext_tbtt_offset;
    uint16_t us_dtim3_on;
} mac_cfg_ps_param_stru; /* hd_event */

typedef struct {
    uint8_t uc_psm_info_enable : 2;  // 开启psm的统计维测输出
    uint8_t uc_psm_debug_mode : 2;   // 开启psm的debug打印模式
    uint8_t uc_psm_resd : 4;
} mac_cfg_ps_info_stru; /* hd_event */

/* P2P NOA节能配置参数 */
typedef struct {
    uint32_t start_time;
    uint32_t duration;
    uint32_t interval;
    uint8_t uc_count;
    uint8_t auc_rsv[NUM_3_BYTES];
} mac_cfg_p2p_noa_param_stru; /* hd_event */

#ifdef _PRE_WLAN_FEATURE_TXOPPS
/* STA txopps aid同步 */
typedef struct {
    uint16_t us_partial_aid;
    uint8_t en_protocol;
    uint8_t uc_enable;
} mac_cfg_txop_sta_stru; /* hd_event */

/* 软件配置mac txopps使能寄存器需要的三个参数 */
typedef struct {
    oal_switch_enum_uint8 en_machw_txopps_en;         /* sta是否使能txopps */
    oal_switch_enum_uint8 en_machw_txopps_condition1; /* txopps条件1 */
    oal_switch_enum_uint8 en_machw_txopps_condition2; /* txopps条件2 */
    uint8_t auc_resv[1];
} mac_txopps_machw_param_stru; /* hd_event */
#endif

typedef enum {
    MAC_PM_DEBUG_SISO_RECV_BCN = 0,
    MAC_PM_DEBUG_DYN_TBTT_OFFSET = 1,
    MAC_PM_DEBUG_NO_PS_FRM_INT = 2,
    MAC_PM_DEBUG_PKT_FILTER = 3,
    MAC_PM_DEBUG_AO = 4,

    MAC_PM_DEBUG_CFG_BUTT
} mac_pm_debug_cfg_enum_uint8; /* hd_event */

typedef struct {
    uint32_t cmd_bit_map;
    uint8_t uc_srb_switch; /* siso收beacon开关 */
    uint8_t uc_dto_switch; /* 动态tbtt offset开关 */
    uint8_t uc_nfi_switch;
    uint8_t uc_pkt_filter_switch;
    uint8_t uc_ao_switch;
} mac_pm_debug_cfg_stru; /* hd_event */

typedef enum {
    MAC_PSM_QUERY_FLT_STAT = 0,
    MAC_PSM_QUERY_FASTSLEEP_STAT = 1,
#ifdef _PRE_WLAN_FEATURE_PSM_DFX_EXT
    MAC_PSM_QUERY_BEACON_CNT = 2,
#endif

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    MAC_PSM_QUERY_RX_LISTEN_STATE = 3,
#endif
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
    MAC_PSM_QUERY_ABNORMAL_PKTS_CNT = 4,
#endif

    MAC_PSM_QUERY_TYPE_BUTT
} mac_psm_query_type_enum;
typedef uint8_t mac_psm_query_type_enum_uint8;
#define MAC_PSM_QUERY_MSG_MAX_STAT_ITEM  10
typedef struct {
    uint32_t                          query_item;
    uint32_t                          aul_val[MAC_PSM_QUERY_MSG_MAX_STAT_ITEM];
} mac_psm_query_stat_stru;

typedef struct {
    mac_psm_query_type_enum_uint8 en_query_type;
    mac_psm_query_stat_stru st_stat;
} mac_psm_query_msg; /* hd_event */

typedef enum {
    MAC_STA_PM_SWITCH_OFF = 0,      /* 关闭低功耗 */
    MAC_STA_PM_SWITCH_ON = 1,       /* 打开低功耗 */
    MAC_STA_PM_MANUAL_MODE_ON = 2,  /* 开启手动sta pm mode */
    MAC_STA_PM_MANUAL_MODE_OFF = 3, /* 关闭手动sta pm mode */
    MAC_STA_PM_SWITCH_BUTT,         /* 最大类型 */
} mac_pm_switch_enum;
typedef uint8_t mac_pm_switch_enum_uint8; /* hd_event */

typedef enum {
    MAC_STA_PM_CTRL_TYPE_HOST = 0, /* 低功耗控制类型 HOST  */
    MAC_STA_PM_CTRL_TYPE_DBAC = 1, /* 低功耗控制类型 DBAC  */
    MAC_STA_PM_CTRL_TYPE_ROAM = 2, /* 低功耗控制类型 ROAM  */
    MAC_STA_PM_CTRL_TYPE_CMD = 3,  /* 低功耗控制类型 CMD   */
    MAC_STA_PM_CTRL_TYPE_ERSRU = 4,  /* 低功耗控制类型 ERSRU     */
    MAC_STA_PM_CTRL_TYPE_CSI = 5,  /* 低功耗控制类型 CSI */
    MAC_STA_PM_CTRL_TYPE_EMLSR = 6, /* 低功耗控制类型 EMLSR  */
    MAC_STA_PM_CTRL_TYPE_SAME_CHAN_LISTEN = 7,
    MAC_STA_PM_CTRL_TYPE_BUTT,     /* 最大类型，应小于 8  */
} mac_pm_ctrl_type_enum;
typedef uint8_t mac_pm_ctrl_type_enum_uint8; /* hd_event */

typedef struct {
    mac_pm_ctrl_type_enum_uint8 uc_pm_ctrl_type; /* mac_pm_ctrl_type_enum */
    mac_pm_switch_enum_uint8 uc_pm_enable;       /* mac_pm_switch_enum */
} mac_cfg_ps_open_stru; /* hd_event */

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
typedef enum {
    MAC_RX_LISTEN_CFG80211_POWER_MGMT = 0,
    MAC_RX_LISTEN_IOCTL_GAME_CTRL = 1,
    MAC_RX_LISTEN_BUTT
} mac_rx_listen_ps_switch_enum;

typedef struct {
    uint8_t rx_listen_enable;
    uint8_t rx_listen_ctrl_type;
} mac_rx_listen_ps_switch_stru; /* hd_event */
#endif

#ifdef _PRE_WLAN_FEATURE_WUR
typedef enum {
    MAC_TX_WUR_WAKEUP = 0,
    MAC_TX_WUR_BEACON = 1,
    MAC_TX_WUR_SETUP = 2,
    MAC_TX_WUR_TEARDOWN = 3,
    MAC_WUR_FASTSLEEP_PARAM = 4,   /* 配置wur极致快睡参数 */
    MAC_WUR_SLEEP_MODE = 5,   /* 配置浅睡进入wur或者深睡进wur */

    MAC_WUR_DEBUG_CMD_BUTT
} mac_wur_debug_cmd_enum;

typedef struct {
    uint8_t mac_addr[WLAN_MAC_ADDR_LEN]; /* 用户的MAC ADDR */
    uint8_t is_always_on;               /* wur工作是否长醒模式 */
    uint8_t count;                      /* duty_cycle_period = beacon_period * count */
    uint8_t ratio;                      /* on_duration = duty_cycle_period / ratio */
    uint8_t rev[3];                     /* 保留3 */
} mac_cfg_wur_setup_param_stru;

typedef struct {
    uint8_t mac_addr[WLAN_MAC_ADDR_LEN]; /* 用户的MAC ADDR */
    uint8_t resv[2];
} mac_cfg_wur_teardown_param_stru;

typedef struct {
    uint8_t mac_addr[WLAN_MAC_ADDR_LEN]; /* 用户的MAC ADDR */
    uint8_t is_short_wake_up;
    uint8_t is_ml_wake_up  : 1,
            link_id_bitmap : 4,
            resv           : 3;
} mac_cfg_wur_wakeup_param_stru;
typedef struct {
    uint8_t pm_timeout; /* wur极致快睡，device睡眠定时器超时时间 */
    uint8_t psm_max_timer_restart_cnt; /* wur极致快睡，device睡眠定时器最大超时次数 */
    uint8_t host_pm_timeout; /* host睡眠定时器超时时间，1格是20ms */
    uint8_t resv;
} mac_cfg_wur_fastsleep_param_stru;

typedef struct {
    uint32_t cmd_bitmap;
    mac_cfg_wur_setup_param_stru setup_params;
    mac_cfg_wur_teardown_param_stru teardown_params;
    mac_cfg_wur_wakeup_param_stru wakeup_params;
    mac_cfg_wur_fastsleep_param_stru fastsleep_params;
    uint32_t sleep_mode;
} mac_cfg_wur_debug_param_stru;

typedef struct {
    mac_category_enum_uint8 action_category; /* ACTION帧的类型, wur=32 */
    uint8_t action_field;                   /* 不同ACTION类下的子帧类型, 0:setup, 1:teardown */
    uint16_t user_idx;
    mac_cfg_wur_stru cfg_info;
} dmac_ctx_update_wur_stru; /* hd_event */
#endif

#endif /* end of hd_event_psm.h */
