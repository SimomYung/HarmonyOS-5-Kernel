/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Hmac tid调度模块
 * 作    者 : wifi
 * 创建日期 : 2020年7月28日
 */

#ifndef HMAC_TID_SCHE_H
#define HMAC_TID_SCHE_H

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "hmac_tid_list_rcu.h"
#endif
#include "hmac_tid_list.h"
#include "mac_device.h"

typedef enum {
    HMAC_TID_SCHE_LIST_BEFORE_ADD_USER,
    HMAC_TID_SCHE_LIST_AFTER_ADD_USER,
    HMAC_TID_SCHE_LIST_BEFORE_DEL_USER,
    HMAC_TID_SCHE_LIST_AFTER_DEL_USER,
    HMAC_TID_SCHE_LIST_BEFORE_ADD_MCAST_USER,
    HMAC_TID_SCHE_LIST_AFTER_ADD_MCAST_USER,
    HMAC_TID_SCHE_LIST_WIFI_STATE_NOTIFY,
} hmac_tid_sche_list_type_enum;

typedef struct {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    hmac_tid_rcu_list_stru tid_list;
#else
    hmac_tid_list_stru tid_list;
#endif
    hmac_tid_list_stru tid_tx_list; /* 本轮调度需要发帧的tid */
#ifdef _PRE_WLAN_WAIT_QUEUE_THREAD_SCHDULE
    oal_wait_queue_head_stru tx_sch_wq;
    oal_atomic tx_sch_flag;
#else
    struct semaphore tid_sche_sema;
#endif
    oal_task_stru *tid_schedule_thread;
    oal_atomic ring_mpdu_num;
    uint32_t schdule_cnt;
} hmac_tid_schedule_stru;

extern hmac_tid_schedule_stru g_tid_schedule_list;
extern oal_atomic g_tid_sche_forbid;
static inline hmac_tid_schedule_stru *hmac_get_tid_schedule_list(void)
{
    return &g_tid_schedule_list;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static inline uint32_t hmac_tid_schedule_list_enqueue(hmac_tid_info_stru *tid_info)
{
    return hmac_tid_list_enqueue_rcu(&g_tid_schedule_list.tid_list, tid_info);
}

static inline uint32_t hmac_tid_schedule_list_delete(hmac_tid_info_stru *tid_info)
{
    return hmac_tid_list_delete_rcu(&g_tid_schedule_list.tid_list, tid_info);
}

static inline uint8_t hmac_tid_schedule_list_for_each(hmac_tid_list_func func, void *param)
{
    return hmac_tid_list_for_each_rcu(&g_tid_schedule_list.tid_list, func, param);
}
#else
static inline uint32_t hmac_tid_schedule_list_enqueue(hmac_tid_info_stru *tid_info)
{
    return hmac_tid_list_enqueue(&g_tid_schedule_list.tid_list, tid_info);
}

static inline void hmac_tid_schedule_list_delete(hmac_tid_info_stru *tid_info)
{
    hmac_tid_list_delete(&g_tid_schedule_list.tid_list, tid_info);
}

static inline uint8_t hmac_tid_schedule_list_for_each(hmac_tid_list_func func, void *param)
{
    return hmac_tid_list_for_each(&g_tid_schedule_list.tid_list, func, param);
}
#endif

static inline uint32_t hmac_tid_tx_list_enqueue(hmac_tid_info_stru *tid_info)
{
    return hmac_tid_list_enqueue_nolock(&g_tid_schedule_list.tid_tx_list, tid_info);
}

static inline void hmac_tid_tx_list_clear(void)
{
    hmac_tid_list_clear_nolock(&g_tid_schedule_list.tid_tx_list);
}

static inline uint8_t hmac_tid_tx_list_for_each(hmac_tid_list_func func, void *param)
{
    return hmac_tid_list_for_each_nolock(&g_tid_schedule_list.tid_tx_list, func, param);
}

static inline void hmac_tid_schedule(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (oal_unlikely(oal_atomic_read(&g_tid_sche_forbid) == OAL_TRUE)) {
        return;
    }
    g_tid_schedule_list.schdule_cnt++;
#ifdef _PRE_WLAN_WAIT_QUEUE_THREAD_SCHDULE
    oal_atomic_set(&g_tid_schedule_list.tx_sch_flag, OAL_TRUE);
    oal_wait_queue_wake_up_interrupt(&g_tid_schedule_list.tx_sch_wq);
#else
    up(&g_tid_schedule_list.tid_sche_sema);
#endif
#endif
}

static inline uint32_t hmac_get_tid_schedule_cnt(void)
{
    return g_tid_schedule_list.schdule_cnt;
}

static inline oal_bool_enum_uint8 hmac_tid_sche_allowed(hmac_tid_info_stru *tid_info)
{
    return (oal_netbuf_list_len(&tid_info->tid_queue) >=
        (uint32_t)oal_atomic_read(&tid_info->tid_sche_th)) ? OAL_TRUE : OAL_FALSE;
}

void hmac_tid_schedule_init(void);
int32_t hmac_tid_schedule_thread(void *data);
uint8_t hmac_is_tid_empty(void);
uint8_t hmac_is_tid_device_ring(void);
void hmac_tx_ring_stuck_check(void);
void hmac_tid_sche_list_dump(uint8_t type);
void hmac_dump_tid_info(void);

#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
oal_bool_enum_uint8 hmac_tid_check_queue_overflow(void);
void hmac_tid_queue_depth_increase(uint32_t num);
void hmac_tid_queue_depth_decrease(uint32_t num);
#endif

#endif
