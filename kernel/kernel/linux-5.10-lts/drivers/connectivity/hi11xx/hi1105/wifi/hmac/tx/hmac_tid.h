/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Hmac tid模块
 * 作    者 : wifi
 * 创建日期 : 2020年7月23日
 */

#ifndef HMAC_TID_H
#define HMAC_TID_H

#include "mac_common.h"
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "host_hal_ext_if.h"

typedef struct {
    uint8_t chr_report_flag;
    uint32_t last_push_ring_tsf;
} hmac_tx_ring_stuck_check_stru;

typedef struct {
    msdu_info_ring_stru base_ring_info;
    msdu_info_ring_ml_stru   ml_ring_info;
    hal_host_tx_msdu_ring_stru ring_buf;
    uint16_t release_index;
    oal_atomic last_period_tx_msdu;
    oal_atomic msdu_cnt;             /* 当前ring中的msdu数 */
    oal_atomic inited;
    oal_atomic ring_sync_th;
    uint32_t ring_sync_cnt;
    oal_atomic ring_tx_opt;
    uint64_t tx_ring_table_addr;
    uint8_t host_tx_flag;
    uint16_t push_ring_full_cnt;
    uint32_t tx_msdu_cnt;            /* 当前ring已发送的msdu个数 */
    oal_netbuf_stru **netbuf_list;
    oal_mutex_stru tx_lock;
    oal_mutex_stru tx_comp_lock;
    hmac_tx_ring_stuck_check_stru tx_ring_stuck_check;
} hmac_msdu_info_ring_stru;

#ifdef _PRE_WLAN_FEATURE_TX_STAT
typedef struct {
    uint32_t enqueue_msdu_cnt;     /* 入缓存队列的msdu统计 */
    uint32_t tx_comp_msdu_cnt;     /* 发送完成msdu个数 */
    uint32_t tx_ring_full_cnt;     /* ring满无法入队计数 */
    uint32_t max_queue_len;        /* 缓存队列的最大深度 */
} hmac_tx_stat_stru;
#endif

typedef struct {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct list_head tid_sche_entry;
    struct list_head tid_update_entry;
#else
    oal_dlist_head_stru tid_sche_entry;
    oal_dlist_head_stru tid_update_entry;
#endif
    oal_dlist_head_stru tid_tx_entry;
    oal_dlist_head_stru tid_tx_comp_entry;
    oal_dlist_head_stru tid_ring_switch_entry;
    uint8_t tid_no;
    uint8_t is_in_ring_list;        /* 该tid是否已存在tx ring切换链表 */
    uint16_t user_index;            // mld模式下表示ml user id，非mld模式则为user id;切换link或切至非mld时需更新 */
    uint16_t lut_index;             /* mld模式下表示mld_lut_index */
    uint8_t ring_switch_fail_cnt;
    uint8_t rsv;
    uint32_t push_fail_cnt;
    oal_atomic inited;
    oal_atomic ring_tx_mode;        /* 发送模式host ring tx/device ring tx/device tx */
    oal_atomic tid_sche_th;
    oal_spin_lock_stru lock;
    oal_netbuf_head_stru tid_queue;
    hmac_msdu_info_ring_stru tx_ring;
#ifdef _PRE_WLAN_FEATURE_TX_STAT
    hmac_tx_stat_stru tx_stat;
#endif
} hmac_tid_info_stru;

void hmac_tid_res_init(void);
uint32_t hmac_tid_get_res_bitmap(void);
typedef uint8_t (* hmac_tid_list_func)(hmac_tid_info_stru *, void *);

typedef struct {
    hmac_tid_info_stru* (*tid_info_getter)(void *);
    void* (*entry_getter)(hmac_tid_info_stru *);
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    uint8_t (*for_each_rcu)(hmac_tid_list_func, void *);
#endif
} hmac_tid_list_ops;

static inline uint8_t hmac_tid_info_inited(hmac_tid_info_stru *tid_info)
{
    return oal_atomic_read(&tid_info->inited);
}

static inline uint8_t hmac_tx_ring_switching(hmac_tid_info_stru *tid_info)
{
    return oal_atomic_read(&tid_info->ring_tx_mode) != HOST_RING_TX_MODE &&
           oal_atomic_read(&tid_info->ring_tx_mode) != DEVICE_RING_TX_MODE;
}

static inline bool hmac_tid_ring_tx_opt(hmac_tid_info_stru *tid_info)
{
    return oal_atomic_read(&tid_info->tx_ring.ring_tx_opt);
}

static inline void hmac_tid_netbuf_enqueue(oal_netbuf_head_stru *tid_queue, oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (tx_ctl == NULL) {
        return;
    }
    oal_spin_lock_head_bh(tid_queue);
    mac_get_cb_retried_num(tx_ctl)++;
    oal_netbuf_list_tail_nolock(tid_queue, netbuf);
    oal_spin_unlock_head_bh(tid_queue);
}

static inline oal_netbuf_stru *hmac_tid_netbuf_dequeue(oal_netbuf_head_stru *tid_queue)
{
    if (oal_netbuf_list_empty(tid_queue)) {
        return NULL;
    }

    return oal_netbuf_delist(tid_queue);
}

#endif
