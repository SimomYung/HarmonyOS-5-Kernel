/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : hmac_mld_vap.c 的头文件
 * 创建日期   : 2023年10月13日
 */

#ifndef __HMAC_MLD_VAP_H__
#define __HMAC_MLD_VAP_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_ML
#include "mac_mld.h"

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define ML_SCAN_MAX_TIME 1
#define ML_LINK_SWITCH_WAIT_DEAUTH_TIME 90000 /* link切换后1.5s内收到ap deauth，则执行重关联 */
#define ML_LINK_SWITCH_TIMEOUT 150 /* 等待device回抛link切换时间 h2d+wait_null 10ms+d2h */
/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/*****************************************************************************
    初始化vap特性枚举
*****************************************************************************/
/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
/*****************************************************************************
  5 消息头定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
typedef struct emlsr_cap {
    uint8_t emlsr_support;
    uint8_t emlsr_padding_delay;
    uint8_t emlsr_transition_delay;
    uint8_t resv;
} hmac_mld_emlsr_cap_stru;
typedef struct {
    uint8_t link_id;
    uint8_t key_index;
    uint8_t rsv[2]; // 2：字节对齐
    uint8_t key[OAL_WPA_KEY_LEN];
} hmac_mld_link_key_info; /* 仅用于supplicant通过私有ioctl下发gtk、igtk、bigtk */

typedef struct {
    uint8_t key_len;
    uint8_t link_num;
    uint8_t mld_addr[ETH_ALEN];
    hmac_mld_link_key_info link_key_info[WLAN_MAX_ML_LINK_NUM];
} hmac_mld_key_info; /* 仅用于supplicant通过私有ioctl下发gtk、igtk、bigtk */

typedef struct {
    uint8_t                     mld_connect;
    uint8_t                     mld_mode; /* 记录漫游前的mld模式 */
    uint8_t                     mld_link_cnt; /* 当前自己的link cnt */
    uint8_t                     ap_link_cnt;   /* STA模式下，对端AP的link个数 */
    uint8_t                     switch_mac_addr;
    uint8_t                     mld_addr[WLAN_MAC_ADDR_LEN];
    uint8_t                     mld_bssid[WLAN_MAC_ADDR_LEN];
    uint8_t                     mac_addr[WLAN_MAC_ADDR_LEN];
    ml_link_info_stru           link_ap_info;
    hmac_mld_record_link_info_stru  record_link_info[WLAN_MAX_ML_LINK_NUM];
} hmac_mld_connect_info;

typedef struct {
    ml_link_switch_info_stru    link_switch_info;
    uint32_t                    link_switch_time_record; /* 最近一次link切换时间 */
    frw_timeout_stru            link_switch_wait_timer;  /* link切换等待定时器 */
    uint8_t                     link_switch_res; /* 当前是否为link切换后的状态，vap初始化去初始化流程需清零 */
    uint8_t                     is_link_switching; /* 当前正处于link切换流程中 标记 */
    uint8_t                     rsv[2];
} hmac_link_switch_record;

typedef struct hmac_mld_manage {
    oal_net_device_stru        *mld_net_device;
    uint8_t                     mld_addr[WLAN_MAC_ADDR_LEN];  // 本端的MLD地址
    uint8_t                     mld_bssid[WLAN_MAC_ADDR_LEN]; // 对端的MLD地址
    struct hmac_vap_tag        *link_vap[WLAN_MAX_ML_LINK_NUM];
    struct hmac_vap_tag        *mld_vap;
    hmac_mld_key_info          *mld_key_infos; /* 存放ioctl下发的秘钥信息，配完秘钥后置NULL */
    uint8_t                     mld_link_cnt;  /* 自身link个数 */
    uint8_t                     ap_link_cnt;   /* STA模式下，对端AP的link个数 */
    uint8_t                     mld_id;  /* 11BE draft 2.0 用于标识 AP MLD */
    uint8_t                     cur_oper_link;  /* 当前工作的link vap */
    uint8_t                     in_use;
    uint8_t                     mld_assoc_mode; /* 标记入网模式：MLSR or STR */
    hmac_link_switch_record     link_switch_record; /* link切换信息记录 */
    oal_dlist_head_stru         ml_user_list_head;
    oal_spin_lock_stru          ml_user_lock; /* ml_user lock */
} hmac_mld_manage_stru;

typedef struct hmac_mld_link_vap {
    uint8_t                     mld_mode;   /* 参见枚举 wlan_mld_mode_enum 类型定义 */
    uint8_t                     work_flag;  /* 该link vap是否工作 */
    uint8_t                     link_id;
    ml_link_info_stru           link_ap_info;  /* 对于STA mode, 保存对端link ap的信息 */
    hmac_mld_emlsr_cap_stru     emlsr_cap; // 关联时的emlsr能力宣称
    hmac_mld_manage_stru       *mld_vap_manage;
} hmac_mld_vap_info_stru;

typedef struct {
    uint8_t addr[WLAN_MAC_ADDR_LEN];
    uint8_t bssid[WLAN_MAC_ADDR_LEN];
    uint8_t ap_link_id;
    uint8_t band;
    uint8_t assoc_succ;
    uint8_t resv;
} mld_link_info_rpt; /* 仅用于supplicant通过私有ioctl获取mld info */

typedef struct {
    uint8_t sta_mld_addr[WLAN_MAC_ADDR_LEN];
    uint8_t ap_mld_addr[WLAN_MAC_ADDR_LEN];
    mld_link_info_rpt link_info[WLAN_MAX_ML_LINK_NUM];
} mld_info_rpt; /* 仅用于supplicant通过私有ioctl获取mld info */

typedef enum {
    OAL_CHANNEL_SWITCH_ANNOUNCEMENT_ELEMENT             = 0,
    OAL_EXTENDED_CHANNEL_SWITCH_ANNOUNCEMENT_ELEMENT    = 1,
    OAL_EDCA_PARAMETERS_LELMENT                         = 2,
    OAL_QUIET_ELEMENT                                   = 3,
    OAL_DSSS_PARAMETER_SET                              = 4,
    OAL_HT_OPERATION_ELEMENT                            = 5,
    OAL_WIDE_BANDWIDTH_CHANNEL_SWITCH_ELEMENT           = 6,
    OAL_CHANNEL_SWITCH_WRAPPER_ELEMENT                  = 7,
    OAL_OPERATING_MODE_NOTIFICATION_ELEMENT             = 8,
    OAL_QUIET_CHANNEL_ELEMENT                           = 9,
    OAL_VHT_OPERATION_ELEMENT                           = 10,
    OAL_HE_OPERATION_ELEMENT                            = 11,
    OAL_BROADCAST_TWT_ELEMENT                           = 12,
    OAL_BROADCAST_TWT_PARAMETER                         = 13,
    OAL_BSS_COLOR_CHANGE_ANNOUNCEMENT_ELEMENT           = 14,
    OAL_SPATIAL_REUSE_PARAMETER_SET_ELEMENT             = 15,
    OAL_UORA_PARAMETER_SET_ELEMENT                      = 16,
    OAL_EHT_OPERATION_ELEMENT                           = 17,
    OAL_ML_RECONFIGURATION_EVENT                        = 18,
    OAL_CRITICAL_UPDATA_BUTT
} oal_critical_update_type_enum;

/* 参数更新状态 */
typedef enum {
    OAL_DTIM_BEACON_OUT = 0,            /* DTIM_BEACON外 */
    OAL_DTIM_BEACON_MID = 1,            /* 当前处于DTIM_BEACON */
    OAL_DTIM_BEACON_IN  = 2,            /* DTIM_BEACON内 */
    OAL_DTIM_BEACON_BUTT
} oal_critical_update_state_enum;

typedef struct mld_bss_critical_update {
    uint8_t bss_parameters_change_count;
    uint8_t all_update_include;
    uint8_t reserved[2]; // 对齐
    oal_critical_update_state_enum critical_update_state;
    oal_critical_update_type_enum curr_critical_updat_type;
} mld_bss_critical_update_stru;

typedef struct mld_emlsr_cap {
    uint8_t emlsr_support;
    uint8_t emlsr_padding_delay;
    uint8_t emlsr_transition_delay;
    uint8_t transition_timeout;
} mld_emlsr_cap_stru;

typedef struct ml_scan_param {
    mac_vap_state_enum_uint8 vap_state_bak; /* 记录vap的状态 后续还原使用 */
    uint8_t mld_id;                   /* 表示ML Probe Request所针对的AP MLD的标识 */
    uint8_t scan_req_cnt;             /* 单播扫描次数 */
    int8_t ssid[WLAN_SSID_MAX_LEN];   /* 网络ssid */
    uint8_t bssid[WLAN_MAC_ADDR_LEN]; /* 单播扫描指定bssid */
    uint8_t sta_mac_addr[WLAN_MAC_ADDR_LEN]; /* 另一个link的mac地址 */
    mac_channel_stru mac_channel;
    hmac_conn_param_stru connect_param; /* 保存关联参数 */
} hmac_ml_scan_param_stru;
/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  10 函数声明
*****************************************************************************/
void hmac_mld_change_vap_user_mld_param(struct hmac_vap_tag *cur_work_vap, struct hmac_vap_tag *next_work_vap,
    hmac_mld_manage_stru *mld_vap_manage);
void hmac_mlsr_change_netdev_vap_relation(struct hmac_vap_tag *cur_work_vap, struct hmac_vap_tag *next_work_vap);
void hmac_mld_set_mld_assoc_mode(struct hmac_vap_tag *hmac_vap, uint8_t new_mld_assoc_mode);
#endif
#endif /* end of hmac_mld_vap.h */
