/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Hmac tid调度模块 - 文件待重构
 * 作    者 : wifi
 * 创建日期 : 2020年7月28日
 */

#include "hmac_tid_sche.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_host_ring_tx.h"
#include "dpe_tx_data.h"
#include "oam_stat_wifi.h"
#include "hmac_tid_update.h"
#include "hmac_tx_stat.h"
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
#include "hmac_config.h"
#endif
#include "hmac_tx_switch.h"
#include "hmac_host_performance.h"
#include "host_hal_dma.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TID_SCHE_C

/* tid调度管理结构体, 主要用于保存待调度的user tid链表, 在调度线程中处理 */
hmac_tid_schedule_stru g_tid_schedule_list;
oal_atomic g_tid_sche_forbid;
#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
#define MAX_TID_DEPTH 10240
static uint32_t g_tid_queue_size = 0;
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static uint8_t hmac_tid_sche_list_for_each_rcu(hmac_tid_list_func func, void *param)
{
    hmac_tid_info_stru *tid_info = NULL;
    struct list_head *tid_list = &g_tid_schedule_list.tid_list.list;

    if (oal_unlikely(list_empty(tid_list))) {
        return OAL_FALSE;
    }

    list_for_each_entry_rcu(tid_info, tid_list, tid_sche_entry) {
        if (func(tid_info, param) == OAL_RETURN) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}
#endif

static inline hmac_tid_info_stru *hmac_sche_tid_info_getter(void *entry)
{
    return oal_container_of(entry, hmac_tid_info_stru, tid_sche_entry);
}

static inline void *hmac_sche_entry_getter(hmac_tid_info_stru *tid_info)
{
    return &tid_info->tid_sche_entry;
}

const hmac_tid_list_ops g_tid_sche_list_ops = {
    .tid_info_getter = hmac_sche_tid_info_getter,
    .entry_getter = hmac_sche_entry_getter,
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    .for_each_rcu = hmac_tid_sche_list_for_each_rcu,
#endif
};

static inline hmac_tid_info_stru *hmac_tx_tid_info_getter(void *entry)
{
    return oal_container_of(entry, hmac_tid_info_stru, tid_tx_entry);
}

static inline void *hmac_tx_entry_getter(hmac_tid_info_stru *tid_info)
{
    return &tid_info->tid_tx_entry;
}

const hmac_tid_list_ops g_tid_tx_list_ops = {
    .tid_info_getter = hmac_tx_tid_info_getter,
    .entry_getter = hmac_tx_entry_getter,
};

void hmac_tid_schedule_init(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    hmac_tid_list_init_rcu(&g_tid_schedule_list.tid_list, &g_tid_sche_list_ops);
#else
    hmac_tid_list_init(&g_tid_schedule_list.tid_list, &g_tid_sche_list_ops);
#endif
    hmac_tid_list_init(&g_tid_schedule_list.tid_tx_list, &g_tid_tx_list_ops);
    oal_atomic_set(&g_tid_schedule_list.ring_mpdu_num, 0);
    oal_atomic_set(&g_tid_sche_forbid, OAL_FALSE);
}

static inline oal_bool_enum_uint8 hmac_tid_push_ring_allowed(hmac_tid_info_stru *tid_info)
{
    return oal_atomic_read(&tid_info->tx_ring.inited);
}

static inline void hmac_tid_get_netbuf_list(
    oal_netbuf_head_stru *tid_queue, oal_netbuf_head_stru *netbuf_list, uint32_t delist_limit)
{
    oal_netbuf_head_init(netbuf_list);

    oal_spin_lock_head_bh(tid_queue);

    while (!oal_netbuf_list_empty(tid_queue) && oal_netbuf_list_len(netbuf_list) < delist_limit) {
        oal_netbuf_list_tail_nolock(netbuf_list, oal_netbuf_delist_nolock(tid_queue));
#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
        hmac_tid_queue_depth_decrease(1);
#endif
    }

    oal_spin_unlock_head_bh(tid_queue);
}

static void hmac_device_ring_tx_update_netbuf(oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (tx_ctl == NULL) {
        return;
    }
    /* 设置8023帧长, 用于设置device ring msdu info的msdu_len字段 */
    mac_get_cb_mpdu_len(tx_ctl) = oal_netbuf_len(netbuf);

#ifndef _PRE_WINDOWS_SUPPORT
    /* device ring tx某些情况下会切换为tid7 device tx发送, 此处统一计算csum */
    if (netbuf->ip_summed == CHECKSUM_PARTIAL) {
        oal_skb_checksum_help(netbuf);
    }
#endif
}

static uint32_t hmac_device_ring_tx_send_netbuf(dpe_wlan_vap_stru *dpe_vap, oal_netbuf_stru *netbuf)
{
    frw_event_stru *frw_event = NULL;
    frw_event_mem_stru *frw_event_mem = frw_event_alloc_m(sizeof(dmac_tx_event_stru));

    if (oal_unlikely(!frw_event_mem)) {
        oam_error_log0(0, 0, "{hmac_device_ring_tx_send_netbuf::frw_event_alloc_m failed}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    frw_event = frw_get_event_stru(frw_event_mem);
    frw_event_hdr_init(&frw_event->st_event_hdr, FRW_EVENT_TYPE_HOST_DRX, DMAC_TX_HOST_DTX, sizeof(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1, dpe_vap->uc_chip_id, dpe_vap->uc_device_id, dpe_vap->uc_vap_id);
    ((dmac_tx_event_stru *)frw_event->auc_event_data)->pst_netbuf = netbuf;

    /* h2d传输netbuf时, 传输失败的netbuf由hcc统一释放, hmac无需释放 */
    if (oal_unlikely(frw_event_dispatch_event(frw_event_mem) != OAL_SUCC)) {
        oam_warning_log0(0, 0, "{hmac_device_ring_tx_send_netbuf::frw_event_dispatch_event failed}");
        oam_stat_vap_incr(dpe_vap->uc_vap_id, tx_abnormal_msdu_dropped, 1);
    }

    frw_event_free_m(frw_event_mem);

    return OAL_SUCC;
}

/*
 * 功能描述 : host tx msdu软重传处理入口
 * 1.日    期 : 2020年7月24日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
uint32_t hmac_tx_soft_retry_process(hmac_tid_info_stru *tid_info, oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);

    if (tx_ctl == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (mac_get_cb_retried_num(tx_ctl) > MAC_KEY_FRAME_SOFT_RETRY_CNT) {
        return OAL_FAIL;
    }

    oam_warning_log4(0, OAM_SF_TX, "{hmac_tx_soft_retry_process::user[%d] tid[%d] retry frame type[%d] subtype[%d]",
        tid_info->user_index, tid_info->tid_no, mac_get_cb_frame_type(tx_ctl), mac_get_cb_frame_subtype(tx_ctl));

    return hmac_host_tx_tid_enqueue(tid_info, netbuf);
}

static inline void hmac_tid_push_ring_fail_proc(hmac_tid_info_stru *tid_info, oal_netbuf_stru *netbuf)
{
    if (hmac_tx_soft_retry_process(tid_info, netbuf) == OAL_SUCC) {
        return;
    }

    hmac_free_netbuf_list(netbuf);
}

static uint32_t hmac_device_ring_tx(dpe_wlan_vap_stru *dpe_vap, hmac_tid_info_stru *tid_info, oal_netbuf_stru *netbuf)
{
    uint32_t ret;

    oal_mutex_lock(&tid_info->tx_ring.tx_lock);

    if (oal_atomic_read(&tid_info->ring_tx_mode) != DEVICE_RING_TX_MODE) {
        oal_mutex_unlock(&tid_info->tx_ring.tx_lock);
        oam_warning_log2(0, 0, "hmac_device_ring_tx::usr[%d] tid[%d] mode err", tid_info->user_index, tid_info->tid_no);
        return OAL_FAIL;
    }

    hmac_device_ring_tx_update_netbuf(netbuf);
    ret = hmac_device_ring_tx_send_netbuf(dpe_vap, netbuf);

    oal_mutex_unlock(&tid_info->tx_ring.tx_lock);

    return ret;
}

static void hmac_tid_push_netbuf_list_to_device_ring(dpe_wlan_vap_stru *dpe_vap, hmac_tid_info_stru *tid_info,
    oal_netbuf_head_stru *netbuf_list)
{
    oal_netbuf_stru *netbuf = NULL;

    while ((netbuf = oal_netbuf_delist_nolock(netbuf_list)) != NULL) {
        if (hmac_device_ring_tx(dpe_vap, tid_info, netbuf) != OAL_SUCC) {
            /* 此处FAIL为hmac层异常, 需要处理对应netbuf */
            hmac_tid_push_ring_fail_proc(tid_info, netbuf);
        }
    }
}

OAL_STATIC uint32_t hmac_tid_push_netbuf_to_device_ring(hmac_tid_info_stru *tid_info, dpe_wlan_vap_stru *dpe_vap)
{
    oal_netbuf_head_stru netbuf_list;

    hmac_tid_get_netbuf_list(&tid_info->tid_queue, &netbuf_list, oal_netbuf_list_len(&tid_info->tid_queue));
    if (oal_unlikely(oal_netbuf_list_empty(&netbuf_list))) {
        return OAL_SUCC;
    }

    hmac_tid_push_netbuf_list_to_device_ring(dpe_vap, tid_info, &netbuf_list);

    return OAL_SUCC;
}

static inline uint32_t hmac_tid_get_ring_avail_size(hmac_msdu_info_ring_stru *tx_ring)
{
    return hal_host_tx_tid_ring_depth_get(tx_ring->base_ring_info.size) - (uint32_t)oal_atomic_read(&tx_ring->msdu_cnt);
}

static inline void hmac_tid_set_chr_report_param(hmac_tid_info_stru *tid_info)
{
    tid_info->tx_ring.tx_ring_stuck_check.last_push_ring_tsf = oal_time_get_stamp_ms(); /* 最后一次入ring的tsf */
    tid_info->tx_ring.tx_ring_stuck_check.chr_report_flag = OAL_FALSE;
}

OAL_STATIC uint32_t hmac_tid_push_netbuf_list_to_host_ring(dpe_wlan_vap_stru *dpe_vap, uint16_t user_idx,
    hmac_tid_info_stru *tid_info, oal_netbuf_head_stru *netbuf_list)
{
    oal_netbuf_stru *netbuf = NULL;
    hal_pm_try_wakeup_forbid_sleep(HAL_PM_FORBIDE_SLEEP_TID_PUSH_NETBUF_LIST);
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_TID_PUSH_NETBUF_LIST);
        return OAL_FAIL;
    }

    while ((netbuf = oal_netbuf_delist_nolock(netbuf_list)) != NULL) {
        if (hmac_host_ring_tx(dpe_vap, user_idx, netbuf) != OAL_SUCC) {
            /* 此处先要进行unmap操作 */
            hal_host_tx_unmap_single_netbuf(netbuf);
            hmac_tid_push_ring_fail_proc(tid_info, netbuf);
            continue;
        }

        if (++tid_info->tx_ring.ring_sync_cnt >= oal_atomic_read(&tid_info->tx_ring.ring_sync_th)) {
            tid_info->tx_ring.ring_sync_cnt = 0;
            hmac_tx_reg_write_ring_info(tid_info, TID_CMDID_ENQUE, OAL_FALSE);
            hmac_tx_sched_trigger(dpe_vap->uc_vap_id, tid_info);
        }
    }

    hmac_tx_reg_write_ring_info(tid_info, TID_CMDID_ENQUE, OAL_FALSE);
    hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_TID_PUSH_NETBUF_LIST);
    return OAL_SUCC;
}

OAL_STATIC void hmac_tid_push_netbuf_list_to_host_ring_before_dma(oal_netbuf_head_stru *netbuf_list,
    oal_netbuf_head_stru *before)
{
    oal_netbuf_stru *netbuf = NULL;
    while ((netbuf = oal_netbuf_delist_nolock(netbuf_list)) != NULL) {
        // netbuf地址转换为devva供mac访问ring
        (void)hal_host_tx_netbuf_init_msdu_dscr(netbuf);
        oal_netbuf_list_tail(before, netbuf);
    }
}

static uint32_t hmac_tid_push_netbuf_list_to_host_ring_proc(
    dpe_wlan_vap_stru *dpe_vap, uint16_t user_idx, hmac_tid_info_stru *tid_info, oal_netbuf_head_stru *netbuf_list)
{
    uint32_t ret;
    oal_netbuf_head_stru before_map;
    oal_netbuf_head_stru after_map;

    oal_netbuf_list_head_init(&before_map);
    oal_netbuf_list_head_init(&after_map);

    hmac_tid_push_netbuf_list_to_host_ring_before_dma(netbuf_list, &before_map);
    hal_host_tx_dma_map_proc(&before_map, &after_map);
    ret = hmac_tid_push_netbuf_list_to_host_ring(dpe_vap, user_idx, tid_info, &after_map);
    /* after map netbuf list中的netbuf无法入队后，重新unmap后放回netbuf_list进行回收 */
    hal_host_tx_dma_unmap_step_proc(&after_map, netbuf_list);

    return ret;
}


static inline void hmac_tid_recycle_netbuf_list(oal_netbuf_head_stru *tid_queue, oal_netbuf_head_stru *netbuf_list)
{
    oal_spin_lock_head_bh(tid_queue);
#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
    hmac_tid_queue_depth_increase(oal_netbuf_list_len(netbuf_list));
#endif
    oal_netbuf_queue_splice_tail_init(netbuf_list, tid_queue);
    oal_spin_unlock_head_bh(tid_queue);
}

OAL_STATIC uint32_t hmac_tid_push_netbuf_to_host_ring(hmac_tid_info_stru *tid_info, dpe_wlan_vap_stru *dpe_vap)
{
    uint32_t avail_size;
    oal_netbuf_head_stru netbuf_list;

    host_cnt_init_record_performance(TX_RING_PROC);
    host_start_record_performance(TX_RING_PROC);

    if (oal_unlikely(!hmac_tid_push_ring_allowed(tid_info))) {
        return OAL_FAIL;
    }

    avail_size = hmac_tid_get_ring_avail_size(&tid_info->tx_ring);
#ifdef _PRE_WLAN_FEATURE_TX_STAT
    hmac_stat_tx_ring(tid_info, avail_size);
#endif
    hmac_tid_get_netbuf_list(&tid_info->tid_queue, &netbuf_list, avail_size);
    if (oal_unlikely(oal_netbuf_list_empty(&netbuf_list) != 0)) {
        return OAL_SUCC;
    }
    if (hmac_tid_push_netbuf_list_to_host_ring_proc(dpe_vap, tid_info->user_index,
        tid_info, &netbuf_list) != OAL_SUCC) {
        hmac_tid_recycle_netbuf_list(&tid_info->tid_queue, &netbuf_list);
        return OAL_FAIL;
    }

    hmac_tid_set_chr_report_param(tid_info);
    host_end_record_performance(host_cnt_get_record_performance(TX_RING_PROC), TX_RING_PROC);

    return OAL_SUCC;
}

typedef uint32_t (* hmac_tid_push_ring)(hmac_tid_info_stru *, dpe_wlan_vap_stru *);
OAL_STATIC hmac_tid_push_ring hmac_get_tid_push_ring_func(hmac_tid_info_stru *tid_info)
{
    uint8_t ring_tx_mode = (uint8_t)oal_atomic_read(&tid_info->ring_tx_mode);
    hmac_tid_push_ring func = NULL;

    if (oal_likely(ring_tx_mode == HOST_RING_TX_MODE)) {
        func = hmac_tid_push_netbuf_to_host_ring;
    } else if (ring_tx_mode == DEVICE_RING_TX_MODE) {
        func = hmac_tid_push_netbuf_to_device_ring;
    }

    return func;
}
#define HMAC_TID_PUSH_RING_FAIL_DUMP_CNT 30
#define HMAC_TID_PUSH_RING_FAIL_RECOVER_CNT 2100
static void hmac_tid_push_ring_fail_reset_msdu_cnt(hmac_msdu_info_ring_stru *tx_ring, hmac_tid_info_stru *tid_info)
{
    oal_mutex_lock(&tx_ring->tx_lock);
    oal_mutex_lock(&tx_ring->tx_comp_lock);
    if ((oal_atomic_read(&tx_ring->msdu_cnt) != 0)) {
        oam_error_log0(0, 0, "{hmac_tid_push_ring_fail_handle_ptr::msdu_cnt err!}");
        oal_atomic_set(&tx_ring->msdu_cnt, 0);
        hmac_tx_mode_h2d_switch_process(tid_info);
    }
    oal_mutex_unlock(&tx_ring->tx_comp_lock);
    oal_mutex_unlock(&tx_ring->tx_lock);
}

static void hmac_tid_push_ring_fail_handle_ptr(hmac_tid_info_stru *tid_info)
{
    hmac_msdu_info_ring_stru *tx_ring = &tid_info->tx_ring;
    /* device侧ring硬件读写指针相等但卡在H2D_SWITCHING_MODE,怀疑host侧软件维护msdu_cnt异常或write_index异常，增加维测+自愈 */
    /* 一直处于H2D_SWITCHING_MODE,release指针和read指针、write指针相等的状态，怀疑msdu_cnt异常 */
    if ((oal_atomic_read(&tid_info->ring_tx_mode) == H2D_SWITCHING_MODE) &&
        (tx_ring->release_index == tx_ring->base_ring_info.read_index) &&
        (tx_ring->release_index == tx_ring->base_ring_info.write_index)) {
        hmac_tid_push_ring_fail_reset_msdu_cnt(tx_ring, tid_info);
        return;
    }

    /* 一直处于H2D_SWITCHING_MODE,release指针和read指针相等，但write指针不相等的状态，怀疑write指针未更新到硬件 */
    if ((oal_atomic_read(&tid_info->ring_tx_mode) == H2D_SWITCHING_MODE) &&
        (tx_ring->release_index == tx_ring->base_ring_info.read_index) &&
        (tx_ring->release_index != tx_ring->base_ring_info.write_index)) {
        /* 打印host侧tx_ring指针信息，与device侧tx_ring打印write指针对比 */
        oam_warning_log4(0, 0, "{hmac_tid_push_ring_fail_handle_ptr::read[%d] write[%d] release[%d] msdu_cnt[%d]!}",
            tx_ring->base_ring_info.read_index, tx_ring->base_ring_info.write_index, tx_ring->release_index,
            oal_atomic_read(&tx_ring->msdu_cnt));
    }
}

static void hmac_tid_push_ring_fail_handle(uint8_t vap_idx, uint16_t user_idx, hmac_tid_info_stru *tid_info)
{
    h2d_tid_debug_info_stru tid_debug_info = { 0 };

    if (++tid_info->push_fail_cnt % HMAC_TID_PUSH_RING_FAIL_DUMP_CNT == 0) {
        oam_warning_log4(0, 0, "{hmac_tid_push_ring_fail_handle::user[%d] tid[%d] push ring[%d] fail cnt[%d]!}",
            tid_info->user_index, tid_info->tid_no, oal_atomic_read(&tid_info->ring_tx_mode), tid_info->push_fail_cnt);
        /* chba ring switch持续失败问题抛事件至device添加维测, 问题定位后删除 */
        tid_debug_info.need_recover = (tid_info->push_fail_cnt % HMAC_TID_PUSH_RING_FAIL_RECOVER_CNT) == 0 ? 1 : 0;
        if (tid_info->push_fail_cnt == HMAC_TID_PUSH_RING_FAIL_DUMP_CNT || tid_debug_info.need_recover == 1) {
            /* 减少刷屏，初次和持续报错下发device处理 */
            tid_debug_info.user_idx = user_idx;
            tid_debug_info.tid_no = tid_info->tid_no;
            tid_debug_info.ring_tx_mode = (uint8_t)oal_atomic_read(&tid_info->ring_tx_mode);
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
            hmac_config_send_event_adapt_dpe(vap_idx, WLAN_CFGID_RING_SWITCH_FAIL_DEBUG,
                sizeof(h2d_tid_debug_info_stru), (uint8_t *)&tid_debug_info);
#endif
            hmac_tid_push_ring_fail_handle_ptr(tid_info);
        }
    }
}

static inline void hmac_tid_push_ring_fail_reset(hmac_tid_info_stru *tid_info)
{
    tid_info->push_fail_cnt = 0;
}

static uint8_t hmac_tid_push_ring_process(hmac_tid_info_stru *tid_info, void *param)
{
    dpe_wlan_user_stru *dpe_user = dpe_wlan_user_get(tid_info->user_index);
    dpe_wlan_vap_stru *dpe_vap = NULL;
    hmac_tid_push_ring func;

    if (oal_unlikely(dpe_user == NULL || dpe_user->en_user_asoc_state != MAC_USER_STATE_ASSOC)) {
        return OAL_CONTINUE;
    }

    dpe_vap = dpe_wlan_vap_get(dpe_user->uc_vap_id);
    if (oal_unlikely(dpe_vap == NULL || dpe_vap->en_vap_state != MAC_VAP_STATE_UP)) {
        return OAL_CONTINUE;
    }

    func = hmac_get_tid_push_ring_func(tid_info);
    if (oal_unlikely(func == NULL)) {
        hmac_tid_push_ring_fail_handle(dpe_vap->uc_vap_id, dpe_user->us_assoc_id, tid_info);
        return OAL_CONTINUE;
    }

    hmac_tid_push_ring_fail_reset(tid_info);
    func(tid_info, dpe_vap);

    g_pm_wlan_pkt_statis.ring_tx_pkt += (uint32_t)oal_atomic_read(&g_tid_schedule_list.ring_mpdu_num);

    return OAL_CONTINUE;
}

static uint8_t hmac_get_tx_sche_tid_info(hmac_tid_info_stru *tid_info, void *param)
{
    if (oal_unlikely(tid_info == NULL)) {
        oam_error_log0(0, 0, "{hmac_get_tx_sche_tid_info::tid_info NULL}");
        return OAL_RETURN;
    }

    if (oal_netbuf_list_len(&tid_info->tid_queue) == 0) {
        return OAL_CONTINUE;
    }

    hmac_tid_tx_list_enqueue(tid_info);

    return OAL_CONTINUE;
}

static inline void hmac_tid_schedule_thread_process(void)
{
    /* rcu protected */
    hmac_tid_schedule_list_for_each(hmac_get_tx_sche_tid_info, NULL);
    /* local */
    hmac_tid_tx_list_for_each(hmac_tid_push_ring_process, NULL);
    hmac_tid_tx_list_clear();
}

#ifdef _PRE_WLAN_WAIT_QUEUE_THREAD_SCHDULE
int32_t hmac_tid_schedule_thread(void *data)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    int32_t ret;
    hmac_tid_schedule_stru *tid_schedule = (hmac_tid_schedule_stru *)data;

    allow_signal(SIGTERM);
    while (!oal_kthread_should_stop()) {
        ret = oal_wait_event_interruptible_m(tid_schedule->tx_sch_wq,
            (oal_atomic_read(&tid_schedule->tx_sch_flag) == OAL_TRUE));
        if (oal_unlikely(ret == -ERESTARTSYS)) {
            oam_error_log1(0, 0, "{hmac_tid_schedule_thread::interrupt by a signal, ret %d}", ret);
            break;
        }
        hmac_tid_schedule_thread_process();

        if (oal_dlist_is_empty(&g_tid_schedule_list.tid_tx_list.list) == OAL_TRUE) {
            oal_atomic_set(&tid_schedule->tx_sch_flag, OAL_FALSE);
        }
    }
#endif

    return OAL_SUCC;
}
#else
int32_t hmac_tid_schedule_thread(void *data)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct sched_param param;

    param.sched_priority = 97; /* 调度优先级97 */
    oal_set_thread_property(current, SCHED_FIFO, &param, -10); /* set nice -10 */
    allow_signal(SIGTERM);

    while (oal_likely(down_interruptible(&g_tid_schedule_list.tid_sche_sema) == 0)) {
#ifdef _PRE_WINDOWS_SUPPORT
        if (oal_kthread_should_stop((PRT_THREAD)data)) {
#else
        if (oal_kthread_should_stop()) {
#endif
            break;
        }

        hmac_tid_schedule_thread_process();
    }
#endif

    return OAL_SUCC;
}
#endif

static inline uint8_t hmac_tid_queue_empty(hmac_tid_info_stru *tid_info, void *param)
{
    if (oal_unlikely(tid_info == NULL)) {
        oam_error_log0(0, 0, "{hmac_tid_queue_empty::tid_info NULL}");
        return OAL_RETURN;
    }

    if (oal_netbuf_list_len(&tid_info->tid_queue)) {
        *((uint8_t *)param) = OAL_FALSE;
        return OAL_RETURN;
    }

    return OAL_CONTINUE;
}

uint8_t hmac_is_tid_empty(void)
{
    uint8_t tid_empty = OAL_TRUE;

    if (oal_unlikely(oal_atomic_read(&g_tid_sche_forbid) == OAL_TRUE)) {
        return OAL_TRUE;
    }

    hmac_tid_schedule_list_for_each(hmac_tid_queue_empty, &tid_empty);

    return tid_empty;
}

uint8_t hmac_dump_tid_queue(hmac_tid_info_stru *tid_info, void *param)
{
    uint32_t *bitmap = hmac_tid_get_res_bitmap();
    if (oal_unlikely(tid_info == NULL)) {
        oam_error_log0(0, 0, "{hmac_dump_tid_queue::tid_info NULL}");
        return OAL_RETURN;
    }

    if (oal_netbuf_list_len(&tid_info->tid_queue)) {
        oam_warning_log3(0, 0, "{hmac_dump_tid_queue::tid_no[%d] user_index[%d] tid_res_bitmap[%d]}",
            tid_info->tid_no, tid_info->user_index, *bitmap);
        return OAL_RETURN;
    }

    return OAL_CONTINUE;
}

void hmac_dump_tid_info(void)
{
    if (oal_unlikely(oal_atomic_read(&g_tid_sche_forbid) == OAL_TRUE)) {
        return;
    }

    hmac_tid_schedule_list_for_each(hmac_dump_tid_queue, NULL);
}

static inline uint8_t hmac_tid_queue_device_ring(hmac_tid_info_stru *tid_info, void *param)
{
    if (oal_unlikely(tid_info == NULL)) {
        oam_error_log0(0, 0, "{hmac_tid_queue_device_ring::tid_info NULL}");
        return OAL_RETURN;
    }

    if (oal_atomic_read(&tid_info->ring_tx_mode) != DEVICE_RING_TX_MODE) {
        *((uint8_t *)param) = OAL_FALSE;
        return OAL_RETURN;
    }

    return OAL_CONTINUE;
}

uint8_t hmac_is_tid_device_ring(void)
{
    uint8_t tid_device_ring = OAL_TRUE;

    hmac_tid_update_list_for_each(hmac_tid_queue_device_ring, &tid_device_ring);

    return tid_device_ring;
}

#define TX_RING_STUCK_CHECK_TIMEOUT_MS 1000
static uint8_t hmac_tx_ring_stuck_check_for_each(hmac_tid_info_stru *tid_info, void *param)
{
    uint32_t cur_time;
    uint32_t delta_time;

    if (oal_unlikely(tid_info == NULL)) {
        oam_error_log0(0, 0, "{hmac_tx_ring_stuck_check_for_each::tid_info NULL}");
        return OAL_RETURN;
    }

    if (tid_info->tx_ring.tx_ring_stuck_check.chr_report_flag == OAL_TRUE) {
        return OAL_CONTINUE;
    }

    cur_time = (uint32_t)oal_time_get_stamp_ms();
    delta_time = (uint32_t)oal_time_get_runtime(tid_info->tx_ring.tx_ring_stuck_check.last_push_ring_tsf, cur_time);
    /* 堵塞触发条件:上次入ring时间超过1s，且ring中的msdu个数不为0 */
    if (delta_time > TX_RING_STUCK_CHECK_TIMEOUT_MS && (oal_atomic_read(&tid_info->tx_ring.msdu_cnt) != 0)) {
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_RING_TX, CHR_WIFI_DRV_ERROR_TX_RING_STUCK);
        tid_info->tx_ring.tx_ring_stuck_check.chr_report_flag = OAL_TRUE;
    }

    return OAL_CONTINUE;
}

void hmac_tx_ring_stuck_check(void)
{
    hmac_tid_schedule_list_for_each(hmac_tx_ring_stuck_check_for_each, NULL);
}

static uint8_t hmac_tid_sche_list_tid_info_dump(hmac_tid_info_stru *tid_info, void *param)
{
    if (oal_unlikely(tid_info == NULL)) {
        oam_error_log0(0, 0, "{hmac_tid_sche_list_tid_info_dump::tid_info NULL}");
        return OAL_RETURN;
    }

    oam_warning_log2(0, 0, "{hmac_tid_sche_list_dump::user[%d] tid[%d]}", tid_info->user_index, tid_info->tid_no);

    return OAL_CONTINUE;
}

void hmac_tid_sche_list_dump(uint8_t type)
{
    oam_warning_log1(0, 0, "{hmac_tid_sche_list_dump::type[%d]}", type);
    hmac_tid_schedule_list_for_each(hmac_tid_sche_list_tid_info_dump, NULL);
}

#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
oal_bool_enum_uint8 hmac_tid_check_queue_overflow(void)
{
    return g_tid_queue_size > MAX_TID_DEPTH ? OAL_TRUE : OAL_FALSE;
}

void hmac_tid_queue_depth_increase(uint32_t num)
{
    g_tid_queue_size += num;
}

void hmac_tid_queue_depth_decrease(uint32_t num)
{
    g_tid_queue_size -= num;
}
#endif