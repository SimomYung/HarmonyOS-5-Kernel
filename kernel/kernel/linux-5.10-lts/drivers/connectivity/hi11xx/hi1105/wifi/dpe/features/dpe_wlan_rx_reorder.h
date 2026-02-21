/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : 重排序头文件
 * 作    者 :
 * 创建日期 : 2024年12月25日
 */

#ifndef DPE_WLAN_RX_REORDER_H
#define DPE_WLAN_RX_REORDER_H

/* 1 头文件包含 */

/* 2 宏定义 */
/* 峰值吞吐测试时,为避免业务侧丢帧,需要将冲排序队列超时定时器调大 */
#define HMAC_BA_RX_VO_TIMEOUT 40  /* 40 milliseconds */
#define HMAC_BA_RX_VI_TIMEOUT 100 /* 100 milliseconds */
#define HMAC_BA_RX_BE_TIMEOUT 60  /* 100 milliseconds */
#define HMAC_BA_RX_BK_TIMEOUT 100 /* 100 milliseconds */
#define HMAC_BA_RX_DEFAULT_TIMEOUT 100 /* 100 milliseconds */

/* 为改善游戏体验,当接收吞吐量较低时,冲排序队列超时定时器周期调小,避免在业务侧堵塞游戏报文 */
#define HMAC_BA_RX_VO_TIMEOUT_MIN 20 /* 20 milliseconds */
#define HMAC_BA_RX_VI_TIMEOUT_MIN 20 /* 20 milliseconds */
#define HMAC_BA_RX_BE_TIMEOUT_MIN 20 /* 20 milliseconds */
#define HMAC_BA_RX_BK_TIMEOUT_MIN 20 /* 20 milliseconds */

#ifdef _PRE_WLAN_CHBA_MGMT
#define DPE_WLAN_CHBA_DBAC_BA_RX_DEFAULT_TIMEOUT 100 /* CHBA DBAC模式下rx重排序队列超时定时器默认周期100 milliseconds */
#endif  /* _PRE_WLAN_CHBA_MGMT */
#endif  /* DPE_WLAN_RX_REORDER_H */