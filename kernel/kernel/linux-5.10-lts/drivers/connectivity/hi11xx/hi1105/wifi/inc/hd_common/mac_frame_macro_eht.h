/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 对应的帧的结构及操作接口定义的源文件(HAL模块不可以调用)
 * 创建日期 : 2023年9月18日
 */

#ifndef MAC_FRAME_MACRO_EHT_H
#define MAC_FRAME_MACRO_EHT_H

/* 1 其他头文件包含 */
#include "wlan_types.h"
#include "oal_util.h"
#include "securec.h"

/* 此文件中定义的结构体与协议相关，需要1字节对齐 */
/* 802.11be协议 EHT相关宏定义 */
#if defined(_PRE_WLAN_FEATURE_11BE) || defined(_PRE_WLAN_FEATURE_11BE_EXT)
#define MAC_EHT_MAC_CAP_LEN              2
#define MAC_EHT_PHY_CAP_LEN              9

#define MAC_EHT_CAP_MIN_LEN              12
#define MAC_EHT_OPER_MIN_LEN             6

#define MAC_EHT_MAX_PPET_NUM             8
#define MAC_EHT_PPE_THRESHOLDS_FIELD_LEN 8

/* eht capabilites的ppe threshold相关定义 */
#define MAC_EHT_PPET_NSS_FIELD_BIT 4             /* EHT PPE Threshold NSS subfield的bit数 */
#define MAC_EHT_PPET_RU_BITMASK_FIELD_BIT 5      /* EHT PPE Threshold RU Index Bitmap subfield的bit数 */
#define MAC_EHT_PPET_RU_BITMASK_FIELD_VALUE 0x0f
#define MAX_EHT_PPET_LEN 62

#define MAC_EHT_PPET_PPETX_FIELD_BIT     3     /* PPE Threshold PPET16、PPET8的bit数 */
#define MAC_EHT_PPET_PPET8_FIELD_BIT      3     /* PPE Threshold PPET16、PPET8的bit数 */

#define MAC_EHT_PPET_RU0_INDEX            0     /* PPE Threshold RU0的索引值 */
#define MAC_EHT_PPET_RU1_INDEX            1     /* PPE Threshold RU1的索引值 */
#define MAC_EHT_PPET_RU2_INDEX            2     /* PPE Threshold RU2的索引值 */
#define MAC_EHT_PPET_RU3_INDEX            3     /* PPE Threshold RU3的索引值 */
#define MAC_EHT_PPET_RU4_INDEX            4     /* PPE Threshold RU4的索引值 */

#define MAC_EHT_PPET_RU_BITMASK_VALID     1     /* PPE Threshold RU Index Bitmap对应的RU有效值 */
#define MAC_EHT_PPET_RU_BITMASK_INVALID   0     /* PPE Threshold RU Index Bitmap对应的RU无效值 */

#define MAC_EHT_PPET_RU_MASK              (0x1) /* PPE Threshold RU Index Bitmap对应每个RU的掩码 */
#define MAC_HE_PPET_VALUE_MASK           (0x7) /* PPE Threshold 的PPET8、PPET16的掩码 */

#define MAC_EHT_PPET_NSS1_INDEX          0     /* PPE Threshold NSTS的第1条NSTS流索引值 */
#define MAC_EHT_PPET_NSS2_INDEX          1     /* PPE Threshold NSTS的第2条NSTS流索引值 */

#define MAC_EHT_MAX_PPET_NSS        1     /* 5612支持的PPE Threshold NSS最大值 */

/* PPETx、PPET8门限值定义 */
#define MAC_EHT_PPE_THR_BPSK    0 /* BPSK */
#define MAC_EHT_PPE_THR_QPSK    1 /* QPSK */
#define MAC_EHT_PPE_THR_16QAM   2 /* 16-QAM */
#define MAC_EHT_PPE_THR_64QAM   3 /* 64-QAM */
#define MAC_EHT_PPE_THR_256QAM  4 /* 256-QAM */
#define MAC_EHT_PPE_THR_1024QAM 5 /* 1024-QAM */
#define MAC_EHT_PPE_THR_4096QAM 6 /* 4096-QAM */
#define MAC_EHT_PPE_THR_NONE     7 /* None */

/* EHT PHY CAP common nominal packet padding值定义 */
#define MAC_EHT_NOMINAL_PKT_PAD_0US  0      /* 0us */
#define MAC_EHT_NOMINAL_PKT_PAD_8US  1      /* 8us */
#define MAC_EHT_NOMINAL_PKT_PAD_16US 2      /* 16us */
#define MAC_EHT_NOMINAL_PKT_PAD_16US_20US 3 /* 16us+20us */

#define MAC_EHT_CHANGE (BIT9)

#define SUPPORT_EHT_LTFS_DISABLE     4 /* dot11EHTMaxNumberOfSupportedEHTLTFsForSU 不支持EHT-LTF */
#define SUPPORT_EHT_LTFS_ENABLE      8 /* dot11EHTMaxNumberOfSupportedEHTLTFsForSU 支持EHT-LTF */
#define MIN_EHT_SOUNDING_NDP_NSS     7 /* STA接收EHT sounding NDP最大空间流数减1，最小值为3 */
#define MAX_EHT_SOUNDING_NDP_NUM_STS 1 /* EHT sounding NDP的TXVECTOR参数NUM_STS的最大值，最大流数-1 */

/* EHT枚举类型 */
typedef enum {
    WLAN_EHT_MCS0,
    WLAN_EHT_MCS1,
    WLAN_EHT_MCS2,
    WLAN_EHT_MCS3,
    WLAN_EHT_MCS4,
    WLAN_EHT_MCS5,
    WLAN_EHT_MCS6,
    WLAN_EHT_MCS7,
    WLAN_EHT_MCS8,
    WLAN_EHT_MCS9,
    WLAN_EHT_MCS10,
    WLAN_EHT_MCS11,
    WLAN_EHT_MCS12,
    WLAN_EHT_MCS13,
    WLAN_EHT_MCS14,
    WLAN_EHT_MCS15,
    WLAN_EHT_MCS_BUTT,
} wlan_eht_mcs_enum;

#endif /* end of _PRE_WLAN_FEATURE_11BE */
#endif /* end of MAC_FRAME_MACRO_EHT_H */
