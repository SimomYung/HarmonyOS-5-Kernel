/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : MP13 wlan产品规格宏定义
 * 作    者 :
 * 创建日期 : 2014年10月10日
 */

#ifndef WLAN_SPEC_MP13_H
#define WLAN_SPEC_MP13_H

#include "wlan_spec_host.h"

/* 作为P2P GO 允许关联最大用户数 */
#ifdef _PRE_WINDOWS_SUPPORT
#define WLAN_P2P_GO_ASSOC_USER_MAX_NUM_MP13 8
#else
#define WLAN_P2P_GO_ASSOC_USER_MAX_NUM_MP13 4
#endif

/* 支持的建立rx ba 的最大个数 */
#define WLAN_MAX_RX_BA_MP13 32

/* 支持的建立tx ba 的最大个数 */
#define WLAN_MAX_TX_BA_MP13 32

/* 其他BSS的广播帧上报hal vap ID */
#define WLAN_HAL_OHTER_BSS_ID_MP13 14

#define WLAN_AMPDU_TX_MAX_NUM_MP13 64 /* AMPDU发送端最大聚合子MPDU个数 */

/* WLAN芯片对应的hal device spec */
#define WLAN_DEVICE_MAX_NUM_PER_CHIP_MP13 2 /* 每个chip支持hal device的最大个数 */

#define WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE_MP13 4 /* mp13 WIFI业务VAP个数 */

/* mp13 整个BOARD支持的最大的WIFI VAP数目 5个业务VAP + 1个配置vap */
#define WLAN_VAP_SUPPORT_MAX_NUM_LIMIT_MP13 (WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC * \
    (WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE + WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE_MP13))

/* board组播用户数 */
#define WLAN_MULTI_USER_MAX_NUM_LIMIT_MP13 (WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC * \
    WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE_MP13)

#define HAL_MAX_VAP_NUM_MP13 4 /* 0305 HAL VAP最大数量 */

#endif /* #ifndef WLAN_SPEC_MP13_H */

