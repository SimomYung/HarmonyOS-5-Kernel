/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : dmac/host共用结构体
 * 作    者 : w30072618
 * 创建日期 : 2025年2月14日
 */
#ifndef HD_RX_ACTION_FILTER_H
#define HD_RX_ACTION_FILTER_H

#define ACTION_BLACKLIST_MAX_SIZE 30
#define IGNORE_FLAG 255
typedef struct {
    uint8_t start;
    uint8_t end;
} vap_state_range_stru;
/* action帧黑名单结构体 */
typedef struct {
    uint8_t action_type;                    /* action帧主类型 */
    uint8_t action_sub_type;                /* action帧子类型 */
    wlan_vap_mode_enum_uint8 vap_mode;      /* vap模式 */
    vap_state_range_stru vap_state;                   /* vap状态:up/非up,非up态有多种状态，用范围表示 */
    oal_bool_enum_uint8 user_idx_isvalid;   /* user_idx是否合法 */
} mac_rx_action_blacklist_stru;

#endif