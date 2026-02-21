/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 ：shenkuo wlan产品规格宏定义
 * 作    者 :
 * 创建日期 : 2020年06月15日
 */

#ifndef WLAN_SPEC_MP12_H
#define WLAN_SPEC_MP12_H

/* MP12 规格待确认 */
/* 作为P2P GO 允许关联最大用户数 */
#ifdef _PRE_WINDOWS_SUPPORT
#define WLAN_P2P_GO_ASSOC_USER_MAX_NUM_MP12 8
#else
#define WLAN_P2P_GO_ASSOC_USER_MAX_NUM_MP12 4
#endif

/* 支持的建立rx ba 的最大个数 */
#define WLAN_MAX_RX_BA_MP12 32

/* 支持的建立tx ba 的最大个数 */
#define WLAN_MAX_TX_BA_MP12 1024

/* rx cb vap id 5bit 全1表示其他BSS */
#define WLAN_HAL_OHTER_BSS_ID_MP12 0x1F

/* tx最大ampdu聚合规格 */
#define WLAN_AMPDU_TX_MAX_NUM_MP12 1024

/* WLAN芯片对应的hal device spec */
#define WLAN_DEVICE_MAX_NUM_PER_CHIP_MP12 1 /* 每个chip支持hal device的最大个数 */

#define WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE_MP12 5 /* mp12 WIFI业务VAP个数 */

/* mp12 整个BOARD支持的最大的WIFI VAP数目 5个业务VAP + 1个配置vap */
#define WLAN_VAP_SUPPORT_MAX_NUM_LIMIT_MP12 (WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC * \
    (WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE + WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE_MP12))

#define HAL_MAX_VAP_NUM_MP12 5 /* mp12 HAL VAP最大数量 */

#endif /* #ifndef WLAN_SPEC_MP12_H */

