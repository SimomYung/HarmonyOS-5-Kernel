/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Hmac tid更新模块
 * 作    者 : wifi
 * 创建日期 : 2021年5月18日
 */

#ifndef HMAC_TID_UPDATE_H
#define HMAC_TID_UPDATE_H

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "hmac_tid_list_rcu.h"
#endif
#include "hmac_tid_list.h"
#include "frw_ext_if.h"

#define HMAC_TID_UPDATE_LVL_CNT 4

#ifdef _PRE_WLAN_FEATURE_SINGLE_CORE_PERFORMANCE
/* tid当前统计周期内发送的帧数 */
#define TID_UPDATE_TH_LEVEL0 200
#define TID_UPDATE_TH_LEVEL1 1000
#define TID_UPDATE_TH_LEVEL2 3000
#define TID_UPDATE_TH_LEVEL3 999999
/* 调度事件下发频率 */
#define RING_SYNC_TH_LEVEL0 9
#define RING_SYNC_TH_LEVEL1 7
#define RING_SYNC_TH_LEVEL2 5
#define RING_SYNC_TH_LEVEL3 3
/* tid调度频率 */
#define TID_SCHE_TH_LEVEL0 1
#define TID_SCHE_TH_LEVEL1 16
#define TID_SCHE_TH_LEVEL2 64
#define TID_SCHE_TH_LEVEL3 128
/* ring tx MIPS优化开关 */
#define RING_TX_OPT_LEVEL0 OAL_TRUE
#define RING_TX_OPT_LEVEL1 OAL_TRUE
#define RING_TX_OPT_LEVEL2 OAL_TRUE
#define RING_TX_OPT_LEVEL3 OAL_TRUE
#else
/* tid当前统计周期内发送的帧数 */
#define TID_UPDATE_TH_LEVEL0 500
#define TID_UPDATE_TH_LEVEL1 3000
#define TID_UPDATE_TH_LEVEL2 6000
#define TID_UPDATE_TH_LEVEL3 999999
/* 调度事件下发频率 */
#define RING_SYNC_TH_LEVEL0 9
#define RING_SYNC_TH_LEVEL1 7
#define RING_SYNC_TH_LEVEL2 5
#define RING_SYNC_TH_LEVEL3 3
/* tid调度频率 */
#define TID_SCHE_TH_LEVEL0 1
#define TID_SCHE_TH_LEVEL1 1
#define TID_SCHE_TH_LEVEL2 16
#define TID_SCHE_TH_LEVEL3 64
/* ring tx MIPS优化开关 */
#define RING_TX_OPT_LEVEL0 OAL_FALSE
#define RING_TX_OPT_LEVEL1 OAL_FALSE
#define RING_TX_OPT_LEVEL2 OAL_TRUE
#define RING_TX_OPT_LEVEL3 OAL_TRUE
#endif

#define HMAC_TID_UPDATE_TIMER_PERIOD 25

typedef enum {
    HMAC_TID_UPDATE_LIST_BEFORE_ADD_USER,
    HMAC_TID_UPDATE_LIST_AFTER_ADD_USER,
    HMAC_TID_UPDATE_LIST_BEFORE_DEL_USER,
    HMAC_TID_UPDATE_LIST_AFTER_DEL_USER,
    HMAC_TID_UPDATE_LIST_BEFORE_ADD_MCAST_USER,
    HMAC_TID_UPDATE_LIST_AFTER_ADD_MCAST_USER,
    HMAC_TID_UPDATE_LIST_WIFI_STATE_NOTIFY,
} hmac_tid_update_list_type_enum;

typedef struct {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    hmac_tid_rcu_list_stru tid_list;
#else
    hmac_tid_list_stru tid_list;
#endif
    uint32_t tid_update_th[HMAC_TID_UPDATE_LVL_CNT];
    uint32_t tid_sche_th[HMAC_TID_UPDATE_LVL_CNT];
    uint32_t ring_sync_th[HMAC_TID_UPDATE_LVL_CNT];
    uint32_t ring_tx_opt[HMAC_TID_UPDATE_LVL_CNT];
    frw_timeout_stru tid_update_timer;
} hmac_tid_update_stru;

extern hmac_tid_update_stru g_tid_update_list;

static inline hmac_tid_update_stru *hmac_get_tid_update_list(void)
{
    return &g_tid_update_list;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static inline uint32_t hmac_tid_update_list_enqueue(hmac_tid_info_stru *tid_info)
{
    return hmac_tid_list_enqueue_rcu(&g_tid_update_list.tid_list, tid_info);
}

static inline uint32_t hmac_tid_update_list_delete(hmac_tid_info_stru *tid_info)
{
    return hmac_tid_list_delete_rcu(&g_tid_update_list.tid_list, tid_info);
}

static inline uint8_t hmac_tid_update_list_for_each(hmac_tid_list_func func, void *param)
{
    return hmac_tid_list_for_each_rcu(&g_tid_update_list.tid_list, func, param);
}
#else
static inline uint32_t hmac_tid_update_list_enqueue(hmac_tid_info_stru *tid_info)
{
    return hmac_tid_list_enqueue(&g_tid_update_list.tid_list, tid_info);
}

static inline void hmac_tid_update_list_delete(hmac_tid_info_stru *tid_info)
{
    hmac_tid_list_delete(&g_tid_update_list.tid_list, tid_info);
}

static inline uint8_t hmac_tid_update_list_for_each(hmac_tid_list_func func, void *param)
{
    return hmac_tid_list_for_each(&g_tid_update_list.tid_list, func, param);
}
#endif
void hmac_tid_update_list_dump(uint8_t type);
void hmac_tid_update_init(void);
void hmac_tid_update_timer_init(void);
void hmac_tid_update_timer_deinit(void);
#endif
