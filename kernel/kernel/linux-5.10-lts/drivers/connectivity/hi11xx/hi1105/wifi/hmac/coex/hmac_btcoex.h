/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_btcoex.c 的头文件
 * 作    者 : wifi
 * 创建日期 : 2016年4月6日
 */

#ifndef HMAC_BTCOEX_H
#define HMAC_BTCOEX_H
/* 1 其他头文件包含 */
#include "frw_ext_if.h"
#include "oal_ext_if.h"
#include "dpe_wlan_tid.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_BTCOEX_H

/* 2 宏定义 */
#define BTCOEX_BSS_NUM_IN_BLACKLIST 16
#define BTCOEX_ARP_FAIL_DELBA_NUM   2
#define BTCOEX_ARP_FAIL_REASSOC_NUM 4
#define BTCOEX_ARP_PROBE_TIMEOUT    (300)  // ms

#define BTCOEX_BLACKLIST_BA_SIZE_LIMIT 0x0002

/* 3 枚举定义 */
typedef enum {
    BTCOEX_BLACKLIST_TPYE_FIX_BASIZE = 0,
    BTCOEX_BLACKLIST_TPYE_NOT_AGGR = 1,

    BTCOEX_BLACKLIST_TPYE_BUTT
} btcoex_blacklist_type_enum;

#define mac_btcoex_check_valid_sta(_mac_vap) \
    ((is_sta(_mac_vap) && (((_mac_vap)->st_cap_flag.bit_bt20dbm) || \
    (WLAN_BAND_2G == (_mac_vap)->st_channel.en_band))) ? OAL_TRUE : OAL_FALSE)

/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
typedef struct {
    uint16_t us_last_baw_start;                       /* 上一次接收到ADDBA REQ中的baw_start值 */
    uint16_t us_last_seq_num;                         /* 上一次接收到ADDBA REQ中的seq_num值 */
    uint8_t en_blacklist_tpye; /* 黑名单方案 */
    oal_bool_enum_uint8 en_ba_handle_allow;          /* 黑名单方案1:是否允许共存删建聚合 黑名单方案2:是否允许建立聚合 */
} hmac_btcoex_addba_req_stru;

typedef struct {
    uint8_t auc_user_mac_addr[WLAN_MAC_ADDR_LEN]; /* 黑名单 */
    uint8_t uc_type;                              /* 写入黑名单的类型 */
    uint8_t uc_used;                              /* 是否已经写过黑名单 */
} hmac_btcoex_delba_exception_stru;
typedef struct {
    hmac_btcoex_delba_exception_stru btcoex_delba_exception[BTCOEX_BSS_NUM_IN_BLACKLIST];
    uint8_t uc_exception_bss_index; /* 黑名单的数组下标 */
    uint8_t auc_resv[3]; /* 4字节对齐补3字节 */
} hmac_device_btcoex_stru;

typedef struct {
    hmac_btcoex_addba_req_stru btcoex_addba_req;
    uint16_t us_ba_size;
    oal_bool_enum_uint8 en_delba_btcoex_trigger; /* 是否btcoex触发删建BA */
    uint8_t res;
} hmac_user_btcoex_stru;

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
void hmac_btcoex_check_rx_same_baw_start_from_addba_req(void *arg1, void *arg2, mac_ieee80211_frame_stru *frame_hdr,
    uint8_t *action, uint32_t frame_body_len);
uint32_t hmac_btcoex_check_exception_in_list(void *arg, uint8_t *mac_addr);
void hmac_btcoex_blacklist_handle_init(void *arg);
uint8_t hmac_btcoex_check_addba_req(void *vap, void *user);
uint32_t hmac_btcoex_d2h_bt_state_syn(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param);
#endif /* end of HMAC_BTCOEX_H */

