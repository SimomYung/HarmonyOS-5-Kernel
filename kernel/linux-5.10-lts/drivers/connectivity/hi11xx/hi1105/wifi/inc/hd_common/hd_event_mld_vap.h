/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 定义HOST/DEVICE传递vap事件 的头文件
 * 创建日期   : 2022年9月16日
 */

#ifndef HD_EVENT_MLD_VAP_H
#define HD_EVENT_MLD_VAP_H

#include "mac_common.h"
#include "wlan_spec.h"

typedef struct {
    uint8_t                      mld_mode; /* 参见枚举 wlan_mld_mode_enum 类型定义 */
    uint8_t                      mld_vap_id;  /* 将mld hmac vap id传递到dmac */
    uint8_t                      link_id;
    uint8_t                      rsv;
    uint8_t                      mld_addr[WLAN_MAC_ADDR_LEN];
} mac_cfg_add_mld_vap_param_stru;
typedef mac_cfg_add_mld_vap_param_stru mac_cfg_del_mld_vap_param_stru;

typedef enum {
    MLD_CHANGE_EVENT_CHANGE_TO_LEGACY = 0,
    MLD_CHANGE_EVENT_CHANGE_TO_MLD = 1,
    MLD_CHANGE_EVENT_CHANGE_TO_LINK = 2,

    MLD_CHANGE_FSM_EVENT_BUTT
} mld_change_fsm_event_type_enum;

/* 切换vap的mld mode时，需同步至dmac的参数 */
typedef struct {
    uint8_t mld_vap_id;  /* master vap的vap id */
    uint8_t new_mld_mode;
    uint8_t mld_mode_change_event;
    uint8_t work_flag;   /* link是否工作 */
    uint8_t link_id;     /* 本MLD分配的link id */
} mac_vap_mld_mode_change_stru;

/* connect流程需同步至dmac的vap参数 */
typedef struct {
    uint8_t mld_addr[WLAN_MAC_ADDR_LEN];  /* 本端的MLD地址 */
    uint8_t mld_bssid[WLAN_MAC_ADDR_LEN]; /* 对端的MLD地址 */
    uint8_t ap_link_cnt;                  /* 对端支持的link个数 */
    uint8_t mld_assoc_mode;               /* 类型：mld_assoc_mode_enum */
    uint8_t reserved[2];
} h2d_mld_vap_param_stru;

/* MLD模式切换 */
/* 同device dmac_ml_state_info */
typedef enum {
    MLD_MODE_SWITCH_INIT = 0,
    MLD_MODE_SWITCH_MLSR = 1,
    MLD_MODE_SWITCH_EMLSR = 2,
    MLD_MODE_SWITCH_STR = 3,
} mac_mld_mode_switch_enum;

/* MLD模式切换 */
typedef struct {
    uint8_t vap_id;
    uint8_t next_vap_state; /* VAP的下一个工作状态 */
    uint8_t next_mld_mode; /* MLD的目的工作模式. 参考类型 mac_mld_mode_switch_enum */
} mac_mld_mode_switch_stru;

#endif /* end of hd_event_mld_vap.h */
