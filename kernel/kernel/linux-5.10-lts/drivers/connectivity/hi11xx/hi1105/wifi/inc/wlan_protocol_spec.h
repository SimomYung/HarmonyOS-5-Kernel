/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 对应所有公共的信息(HAL同时可以使用的)放到该文件中
 * 创建日期 : 2023年9月19日
 */

#ifndef WLAN_PROTOCOL_SPEC_H
#define WLAN_PROTOCOL_SPEC_H

/* 1 其他头文件包含 */
#include "oal_ext_if.h"

/* 2 枚举定义 */

/* WIFI协议类型定义 */
/* Note: 此定义若修改, 请知会 00330043, 需要相应更新g_auc_default_mac_to_phy_protocol_mapping数组值 */
typedef enum {
    WLAN_LEGACY_11A_MODE = 0,    /* 11a, 5G, OFDM */
    WLAN_LEGACY_11B_MODE = 1,    /* 11b, 2.4G */
    WLAN_LEGACY_11G_MODE = 2,    /* 旧的11g only已废弃, 2.4G, OFDM */
    WLAN_MIXED_ONE_11G_MODE = 3, /* 11bg, 2.4G */
    WLAN_MIXED_TWO_11G_MODE = 4, /* 11g only, 2.4G */
    WLAN_HT_MODE = 5,            /* 11n(11bgn或者11an，根据频段判断) */
    WLAN_VHT_MODE = 6,           /* 11ac */
    WLAN_HT_ONLY_MODE = 7,       /* 11n only mode,只有带HT的设备才可以接入 */
    WLAN_VHT_ONLY_MODE = 8,      /* 11ac only mode 只有带VHT的设备才可以接入 */
    WLAN_HT_11G_MODE = 9,        /* 11ng,不包括11b */
    WLAN_HE_MODE = 10,           /* 11ax */
    WLAN_HT_ONLY_MODE_2G = 11,
    WLAN_VHT_ONLY_MODE_2G = 12,
    WLAN_EHT_MODE = 13,          /* 11be */
    WLAN_11B_PLUS_MODE = 14,     /* 11b+ */
    WLAN_MIXED_11BBPLUS_MODE = 15,     /* 11b与11b+混合模式 */
    WLAN_MIXED_11ABPLUS_MODE = 16,     /* 11a与11b+混合模式 */
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV)
    WLAN_PROTOCOL_BUTT = 11,
#elif (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1105_DEV)
    WLAN_PROTOCOL_BUTT = 12,
#else
    WLAN_PROTOCOL_BUTT,
#endif
} wlan_protocol_enum;
typedef uint8_t wlan_protocol_enum_uint8;

#endif /* end of WLAN_PROTOCOL_SPEC_H */
