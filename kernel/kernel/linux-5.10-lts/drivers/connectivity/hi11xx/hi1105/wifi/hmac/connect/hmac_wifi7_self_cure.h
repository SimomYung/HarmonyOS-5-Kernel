/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac arp probe功能函数定义
 * 作    者 : wifi
 * 创建日期 : 2024年7月25日
 */

#ifndef HMAC_WIFI7_SELF_CURE_H
#define HMAC_WIFI7_SELF_CURE_H

#ifdef _PRE_WLAN_FEATURE_11BE
/* 1 其他头文件包含 */
#include "frw_ext_if.h"
#include "oal_ext_if.h"
#include "mac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WIFI7_SELF_CURE_H

/* 2 宏定义 */
#define HMAC_WIFI7_SELF_CURE_BLACK_LIST_MAX_NUM 16

typedef enum {
    HMAC_WIFI7_SELF_CURE_BLACKLIST_TYPE_MLD   = 0,
    HMAC_WIFI7_SELF_CURE_BLACKLIST_TYPE_WIFI7 = 1,
} hmac_wifi7_self_cure_blacklist_type_enum;

typedef struct {
    uint8_t user_mac_addr[WLAN_MAC_ADDR_LEN];
    uint8_t blacklist_type; /* 枚举定义 hmac_wifi7_self_cure_blacklist_type_enum */
    uint8_t rsv;
} hmac_wifi7_self_cure_blacklist_stru;

typedef struct {
    hmac_wifi7_self_cure_blacklist_stru wifi7_blacklist[HMAC_WIFI7_SELF_CURE_BLACK_LIST_MAX_NUM];
    uint8_t blaclist_num;
} hmac_wifi7_self_cure_info_stru;

uint32_t hmac_wifi7_self_cure_black_list_set(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param);
oal_bool_enum_uint8 hmac_wifi7_self_cure_mac_is_wifi7_blacklist_type(uint8_t *mac);
oal_bool_enum_uint8 hmac_wifi7_self_cure_mac_is_mld_blacklist_type(uint8_t *mac);
#endif
#endif /* end of HMAC_WIFI7_SELF_CURE_H */