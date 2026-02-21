/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Hmac tid模块
 * 作    者 : wifi
 * 创建日期 : 2020年7月23日
 */

#ifndef DPE_WLAN_TID_H
#define DPE_WLAN_TID_H

#include "mac_common.h"
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "host_hal_ext_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    oal_bool_enum_uint8 in_use;          /* 缓存BUF是否被使用 */
    uint8_t uc_num_buf;                /* MPDU占用的netbuf(接收描述符)个数 */
    uint16_t us_seq_num;               /* MPDU对应的序列号 */
    oal_netbuf_head_stru st_netbuf_head; /* MPDU对应的描述符首地址 */
    uint32_t rx_time;               /* 报文被缓存的时间戳 */
} hmac_rx_buf_stru;

typedef struct {
    uint8_t uc_tid;
    mac_delba_initiator_enum_uint8 en_direction;
    uint8_t uc_resv[1];
    uint8_t uc_vap_id;
    uint16_t us_mac_user_idx;
    uint16_t us_timeout_times;
} hmac_ba_alarm_stru;

/* TID对应的发送BA会话的状态 */
typedef struct {
    uint16_t us_baw_start;       /* 第一个未收到的MPDU的序列号 */
    uint16_t us_baw_size;        /* Block_Ack会话的buffer size大小 */

    uint8_t uc_lut_index;                    /* LUT索引 */

    /* 以下action帧相关 */
    uint8_t extension_ie_received;         /* 接收到addba extension IE,则addba rsp中也要带此IE */
    uint8_t addba_para_set;                /* ADDBA Additional Parameter Set */
    mac_back_variant_enum_uint8 en_back_var; /* BA会话的变体 */
    oal_bool_enum_uint8 en_amsdu_supp;       /* BLOCK ACK支持AMSDU的标识 */
    uint8_t uc_dialog_token;               /* ADDBA交互帧的dialog token */
    uint8_t uc_ba_policy;                  /* Immediate=1 Delayed=0 */
    uint16_t us_ba_timeout;                /* BA会话交互超时时间 */
} hmac_ba_rx_para_stru;

/* Hmac侧接收侧BA会话句柄 */
typedef struct {
    uint16_t us_baw_start;       /* 第一个未收到的MPDU的序列号 */
    uint16_t us_baw_end;         /* 最后一个可以接收的MPDU的序列号 */
    uint16_t us_baw_head;        /* bitmap的起始序列号 */
    uint16_t us_baw_tail;        /* 目前Re-Order队列中，最大的序列号 */
    uint16_t us_baw_size;        /* Block_Ack会话的buffer size大小 */
    uint16_t us_last_relseq;     /* 目前Re-Order队列中，上一次release seqnum */

    uint8_t uc_lut_index;                  /* 接收端Session H/w LUT Index */
    oal_bool_enum_uint8 en_timer_triggered;      /* 上一次上报是否为定时器上报 */
    oal_bool_enum_uint8 en_is_ba;                /* Session Valid Flag */
    dmac_ba_conn_status_enum_uint8 en_ba_status; /* 该TID对应的BA会话的状态 */
    uint16_t uc_mpdu_cnt;                      /* 当前Re-Order队列中，MPDU的数目 td */

    /* 重排序队列锁, 用于hcc线程和事件线程并发 */
    oal_spin_lock_stru st_ba_lock;
    /* Re-Order队列 */
    uint32_t aul_rx_buf_bitmap[2];         /* rx buffer bitmap 2:共64位 */
    hmac_rx_buf_stru ast_re_order_list[WLAN_AMPDU_RX_HE_BUFFER_SIZE];
} hmac_ba_rx_stru;

/* TID对应的BA信息的保存结构 */
typedef struct {
    uint8_t uc_tid_no;
    uint16_t qos_last_seq_frag_num; /* 保存本地最后收到的seq_num值 */
    hmac_ba_rx_stru *pst_ba_rx_info;            /* 由于部分处理上移，这部分内存到LocalMem中申请 */
    oal_spin_lock_stru st_ba_timer_lock;        /* 用于hcc线程和事件线程并发 */
    frw_timeout_stru st_ba_timer;               /* 接收重排序缓冲超时 */
    hmac_ba_alarm_stru st_alarm_data;
} hmac_tid_ba_stru;

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
    hal_tx_ring_table_addr_val_stru tx_ring_table_addr;
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

typedef struct {
    uint8_t tid_res_id;
    uint8_t work_flag;
    hmac_tid_ba_stru tid_ba[WLAN_TID_MAX_NUM]; /* tid ba资源 */
    hmac_tid_info_stru tid_info[WLAN_TID_MAX_NUM]; /* tx tid info资源 */
} hmac_tid_stru;

extern uint32_t g_hmac_tid_res_bitmap;

void hmac_tid_res_init(void);
uint32_t *hmac_tid_get_res_bitmap(void);
void hmac_tid_res_init(void);
void hmac_user_tid_res_deinit(uint16_t user_idx);
hmac_tid_ba_stru *hmac_get_tid_ba_info(uint16_t user_idx, uint8_t tid);
hmac_tid_info_stru *hmac_get_tx_tid_info(uint16_t user_idx, uint8_t tid);
void dpe_wlan_rx_ba_init(hmac_ba_rx_para_stru *ba_rx, uint8_t vap_id, uint16_t assoc_id, uint8_t tid);
void hmac_clear_one_tid_queue(uint16_t us_assoc_id, uint8_t num);

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
void dpe_change_tid_ba_info(uint16_t cur_work_user_idx, uint16_t next_work_user_idx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
