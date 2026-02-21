/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : hmac_mld_user.c的头文件
 * 创建日期   : 2022年03月07日
 */

#ifndef __HMAC_MLD_USER_H__
#define __HMAC_MLD_USER_H__
#ifdef _PRE_WLAN_FEATURE_ML
/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "mac_frame.h"
typedef enum {
    WLAN_TTLM_INIT = 0,          /* ttlm状态初始值 */
    WLAN_TTLM_PROCESS = 1,       /* ttlm状态协商中 */
    WLAN_TTLM_COMPLETE = 2,      /* ttlm状态协商结束 */
    WLAN_TTLM_BUTT,
} wlan_ttlm_status_enum;

typedef struct {
    uint8_t user_index;
    uint32_t expected_duration;
    uint8_t diff_time;
} mac_ttlm_frw_timer_stru;

typedef struct {
    uint8_t link_bitmap[WLAN_TIDNO_BUTT];  /* 每个tid 的link bitmap */
    uint8_t default_mapping;               /* 标识是否是default mapping */
    uint8_t status;                        /* 标识当前ttlm状态 */
    uint8_t resv[2];
} mac_ttlm_stru;

typedef struct {
    frw_timeout_stru            ttlm_switch_timer; /* ttlm 开始生效定时器 */
    frw_timeout_stru            ttlm_duration_timer; /* ttlm 开始失效定时器 */
    frw_timeout_stru            ttlm_request_timer; /* ttlm 协商等待响应超时定时器 */
    mac_ttlm_stru               ttlm[WLAN_DOWNUPLINK];
    uint8_t                     ttlm_status[TTLM_TIDNO_BUTT][WLAN_MAX_ML_LINK_NUM]; /* 保留上一次的信息 */
    uint8_t                     is_advertised;                     /* 标识是否是通告的 */
    uint8_t                     direction;
    uint8_t                     dialog_token;
    uint8_t                     resv;
} mac_ttlm_info_stru;
typedef struct {
    uint8_t                    link_assoc_is_valid;
    uint8_t                    resv;
    uint16_t                   link_status_code;
} hmac_mld_link_assoc_info;

typedef struct {
    uint8_t        link_is_valid;
    uint8_t        link_vap_id;
    uint8_t        is_main_link_user;
    uint16_t       link_user_id;
} hmac_mld_record_link_info_stru;

typedef struct {
    uint8_t emlsr_state;  /* 记录emlsr当前工作状态 emlsr_state_enum */
    uint8_t emlsr_padding_delay    : 3, // 记录emlsr参数更新
            emlsr_transition_delay : 3, // 记录emlsr参数更新
            rsv : 2;
} mac_emlsr_record_info;

typedef struct {
    oal_dlist_head_stru             dlist_entry; /* 链表指针 */
    uint8_t                         in_use;
    uint8_t                         mld_user_man_id;
    uint8_t                         ml_vap_id;
    uint8_t                         mld_link_cnt;
    uint16_t                        ml_user_id;
    uint8_t                         mld_addr[WLAN_MAC_ADDR_LEN];            /* MLD的对端MAC地址 */
    hmac_mld_link_assoc_info        link_assoc_info[WLAN_MAX_ML_LINK_NUM];
    hmac_mld_record_link_info_stru  record_link_info[WLAN_MAX_ML_LINK_NUM];
    mac_eml_capabilities_stru       eml_cap;
    mac_mld_capabilities_stru       mld_cap;
    mac_medium_sync_info_stru       medium_cap;
    mac_ttlm_info_stru              ttlm_info;
    mac_emlsr_record_info           emlsr_record_info;
} hmac_mld_user_manage_stru;

typedef struct {
    /* 参见枚举 wlan_mld_mode_enum 类型定义 */
    uint8_t                         mld_mode;  /* MLD模式，表示是否为MLD设备、MLD链路设备 */
    uint8_t                         link_id;
    uint8_t                         bpcc;
    uint8_t                         ap_link_id;
    hmac_mld_user_manage_stru      *mld_user_manage;
} hmac_mld_user_link_info_stru;
#endif
#endif /* end of hmac_user.h */
