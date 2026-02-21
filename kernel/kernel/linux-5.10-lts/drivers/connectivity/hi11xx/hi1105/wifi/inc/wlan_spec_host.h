 /*
  * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
  * 功能说明 : wlan产品规格宏定义，里面划分各个模块的spec的定义
  * 作    者 :
  * 创建日期 : 2012年9月26日
  */

#ifndef WLAN_SPEC_HOST_H
#define WLAN_SPEC_HOST_H

// 此处不加extern "C" UT编译不过
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1. 整个BOARD支持的最大的hal device数目, HOST按最大规格定义,HOST看不见DBDC规格, 用于资源定义 */
#define WLAN_DEVICE_MAX_NUM_PER_CHIP  2

/* hal device id 枚举 */
typedef enum {
    HAL_DEVICE_ID_MASTER        = 0,    /* master的hal device id */
    HAL_DEVICE_ID_SLAVE         = 1,    /* slave的hal device id */
} hal_device_id_enum;

/* 2. WLAN芯片对应的mac device spec */
#define WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP 1 /* 每个chip支持业务device的最大个数 */
/* 整个BOARD支持的最大的业务mac device数目 */
#define WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC (WLAN_CHIP_MAX_NUM_PER_BOARD * WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP)

/* 3. 每个device支持vap的最大个数=最大业务VAP数目+配置VAP数量 */
/* 由于最大业务VAP个数需要按照设备同时创建的最大规格初始化，目前按照最大的规格设置即可 */
#define WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE  1 /* 配置VAP个数,一个业务device一个 */
#define WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE 5 /* WIFI业务VAP个数(原本应该用AP个数+STA个数（这个表示芯片最大支持能力 */
/*
 * 软件规格:P2P_dev/CL以STA模式存在，P2P_GO以AP模式存在
 * 1)AP 模式: 1个ap + 1个配置vap
 * 2)STA+P2P共存模式: 1个sta + 1个P2P_dev + 1个P2P_GO/Client + 1个配置vap
 * 3)STA+NAN: 1个sta + 1个P2P_dev + 1个配置vap（NAN使用wlan0 VAP, 不使能P2P 业务功能）
 * 4)双接口：2个sta+1个P2P_dev+1个配置VAP, 不使能P2P 业务功能
 * 5)2投屏：1个STA + 1个P2P_DEV + 2个GO 或 1个STA + 1个P2P_DEV + 2个GC 或1个STA + 1个P2P_DEV + 1个GO + 1个GC
 */
#ifdef _PRE_DUAL_CHIP
/* 不修改device，保持以下设置与单芯片时一致 */
/* 整个BOARD支持的最大的WIFI VAP数目 */
#define WLAN_VAP_MAX_NUM_LIMIT (WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE + WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE)
/* 整个BOARD支持的最大WIFI业务VAP的数目不包括配置vap */
#define WLAN_SERVICE_VAP_SUPPORT_MAX_NUM_LIMIT WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE
#else
/* 整个BOARD支持的最大的WIFI VAP数目 */
#define WLAN_VAP_MAX_NUM_LIMIT (WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC * \
    (WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE + WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE))
/* 整个BOARD支持的最大WIFI业务VAP的数目不包括配置vap */
#define WLAN_SERVICE_VAP_SUPPORT_MAX_NUM_LIMIT (WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC * \
    WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE)
#endif

#ifdef _PRE_PRODUCT_ID_E5
#define WLAN_SERVICE_AP_MAX_NUM_PER_DEVICE  3 /* 业务AP的规格，将之前的WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE修改 */
#else
#define WLAN_SERVICE_AP_MAX_NUM_PER_DEVICE  2 /* 业务AP的规格，将之前的WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE修改 */
#endif
#define WLAN_SERVICE_STA_MAX_NUM_PER_DEVICE 3 /* 业务STA的规格 */

#define HAL_MAX_VAP_NUM                     5 /* HAL VAP最大数量 */
#define WLAN_AP_STA_COEXIST_VAP_NUM         0 /* 业务ap sta共存时vap数目,mp13后续有STA+AP共存的情况 */

/* 4. WLAN_ASSOC_USER_MAX_NUM对应g_us_assoc_max_user WLAN_ASSOC_USER_MAX_NUM对应g_us_active_max_user */
#ifdef _PRE_PRODUCT_ID_E5
#define WLAN_ASSOC_USER_MAX_NUM 32
/* 支持32个单播用户, 组播用户暂定4个，待VAP资源修改后再刷新 */
#define WLAN_USER_MAX_USER_LIMIT 36
#else
#define WLAN_ASSOC_USER_MAX_NUM 16 /* mp16关联用户的最大个数 */
/* mp16软件规格:P2P_dev/CL以STA模式存在，P2P_GO以AP模式存在，用户数量(组播+单播用户)最大17个,多预留一个备份
        1)AP 模式:  1个组播用户 + 16个单播用户
        2)STA+P2P CL 共存模式:  2个组播用户 + 2个单播用户
        3)STA+P2P GO 共存模式+P2P0:  3个组播用户 + 9个单播用户(STA 1 + GO 8) WLAN_P2P_GO_ASSOC_USER_MAX_NUM_MP16
        4)STA+2 P2P GO+P2P0:4个组播用户(STA+P2P0+P2P_GC+P2P_GO)+16个单播用户 */
#define WLAN_USER_MAX_USER_LIMIT 20
#endif

/* 活跃用户索引位图长度 */
#define WLAN_ACTIVE_USER_IDX_BMAP_LEN ((WLAN_ASSOC_USER_MAX_NUM + 7) >> 3)
/* 关联用户索引位图长度 */
#define WLAN_ASSOC_USER_IDX_BMAP_LEN ((WLAN_ASSOC_USER_MAX_NUM + 7) >> 3)

#define WLAN_USER_MAX_TID_NUM (WLAN_USER_MAX_USER_LIMIT * WLAN_TID_MAX_NUM) /* mp16最大tid num */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_spec.h */
