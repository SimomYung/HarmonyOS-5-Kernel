/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : vsp发送流程
 * 作者       : wifi
 * 创建日期   : 2020年12月24日
 */
#include "hmac_vsp_source.h"
#include "hmac_vsp_buf.h"
#include "hmac_vsp_flow_ctrl.h"
#include "hmac_tx_data.h"
#include "hmac_tx_ring_alloc.h"
#include "hmac_host_ring_tx.h"
#include "hmac_tx_data.h"
#include "hmac_vsp_if.h"
#include "alg_cfg.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_VSP_SOURCE_C

#ifdef _PRE_WLAN_FEATURE_VSP
hmac_vsp_source_stru g_vsp_source = {
    .vsp_base.timer.timer_id = 16,
};

static bool hmac_vsp_set_evt_flag(hmac_vsp_source_stru *source, enum vsp_source_event evt)
{
#if _PRE_OS_VERSION_LINUX == _PRE_OS_VERSION
    bool chg = !test_and_set_bit(evt, &source->evt_flags);
    oal_wait_queue_wake_up_interrupt(&source->evt_wq);
    return chg;
#else
    return OAL_TRUE;
#endif
}

static void hmac_vsp_source_timeout_update_rptr(hmac_tid_info_stru *tid_info)
{
    uint32_t rptr_val;

    if (hmac_tx_reg_read_rptr(tid_info, &rptr_val) != OAL_SUCC) {
        return;
    }

    tid_info->tx_ring.base_ring_info.read_index = (uint16_t)rptr_val;
}

OAL_STATIC void hmac_vsp_source_timeout_handler(hmac_vsp_source_stru *source)
{
    hmac_tid_info_stru *tid_info = source->vsp_base.tid_info;

    hmac_vsp_watchdog_cnt_update(source);
    oam_warning_log0(0, 0, "hmac_vsp_source_timeout_handler::timeout!");
    source->vsp_base.timer_running = OAL_FALSE;

    hal_pm_try_wakeup_forbid_sleep(HAL_PM_FORBIDE_SLEEP_VSP_SOURCE_TIMEOUT);
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_VSP_SOURCE_TIMEOUT);
        return;
    }

    oal_mutex_lock(&tid_info->tx_ring.tx_comp_lock);
    hmac_vsp_source_timeout_update_rptr(tid_info);
    oal_mutex_unlock(&tid_info->tx_ring.tx_comp_lock);

    hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_VSP_SOURCE_TIMEOUT);

    hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_TX_COMP);

    hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_RX_FEEDBACK);
}

static struct {
    uintptr_t va;
    hmac_vsp_tx_slice_stru *slice;
} g_vsp_tx_pkt_info[4096]; /* max host ring size 4096 */

static uintptr_t hmac_vsp_get_packet_va(uint16_t idx)
{
    return g_vsp_tx_pkt_info[idx].va;
}

static hmac_vsp_tx_slice_stru *hmac_vsp_get_slice_of_packet(uint16_t idx)
{
    return g_vsp_tx_pkt_info[idx].slice;
}

static void hmac_vsp_record_packet_info(uint16_t idx, uintptr_t va, hmac_vsp_tx_slice_stru *s)
{
    g_vsp_tx_pkt_info[idx].va = va;
    g_vsp_tx_pkt_info[idx].slice = s;
    s->pkts_in_ring++;
}

static inline uint8_t hmac_vsp_source_pkt_tx_succ(uint8_t *msdu_dscr_addr)
{
    hal_tx_msdu_dscr_info_stru msdu_dscr = { 0 };

    hal_vsp_msdu_dscr_info_get(msdu_dscr_addr, &msdu_dscr);

    return msdu_dscr.tx_count < 120; /* MSDU最大发送次数120, 小于该值认为发送成功 */
}

static inline uint8_t hmac_vsp_source_cfg_pkt_tx_succ(tx_layer_ctrl *layer)
{
    return hmac_vsp_source_pkt_tx_succ((uint8_t *)hmac_vsp_source_get_pkt_msdu_dscr(layer->data_addr));
}

static inline void hmac_vsp_source_cfg_pkt_tx_succ_proc(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    hmac_vsp_slice_enqueue(&source->comp_queue, slice);
}

static inline void hmac_vsp_source_cfg_pkt_retrans(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    slice->retrans_cnt--;
    hmac_vsp_slice_enqueue_head(&source->free_queue, slice);
    hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_NEW_SLICE);
}

static inline void hmac_vsp_source_slice_tx_done(hmac_vsp_tx_slice_stru *slice)
{
    hmac_vsp_log((uint8_t *)slice, VSP_LOG_BITMAP_SLICE_INFO, VSP_LOG_DATATYPE_SLICE, VSP_LOG_TYPE_RPT_SLICE);
    oal_dlist_delete_entry(&slice->entry);
    vsp_vcodec_tx_pkt_done(&slice->tx_result);
    oal_free(slice);
}

static inline void hmac_vsp_source_cfg_pkt_tx_fail_proc(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    if (slice->retrans_cnt) {
        hmac_vsp_source_cfg_pkt_retrans(source, slice);
    } else {
        hmac_vsp_slice_enqueue(&source->comp_queue, slice);
        vsp_vcodec_tx_send_done(&slice->tx_result); // 发送完成后通知venc，venc用于释放buff，利于循环使用
    }
}

static inline void hmac_vsp_source_cfg_pkt_tx_comp_proc(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    if (oal_likely(hmac_vsp_source_cfg_pkt_tx_succ(slice->head))) {
        hmac_vsp_source_cfg_pkt_tx_succ_proc(source, slice);
        vsp_vcodec_tx_send_done(&slice->tx_result); // 发送完成后通知venc，venc用于释放buff，利于循环使用
    } else {
        hmac_vsp_source_cfg_pkt_tx_fail_proc(source, slice);
    }
}

static inline void hmac_vsp_source_data_pkt_tx_comp_proc(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    hmac_vsp_slice_enqueue(&source->comp_queue, slice);
    hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_RX_FEEDBACK);
    hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_NEW_SLICE);
}

static void hmac_vsp_slice_tx_complete_proc(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice,
                                            uint32_t cur_tsf)
{
    if (source->vsp_base.timer_running && (hmac_is_vsp_source_cfg_pkt(slice->head) == OAL_FALSE)) {
        hmac_vsp_stop_timer_no_pm_wakeup(&source->vsp_base);
    }
    slice->slice_rtt = cur_tsf - slice->vsync_ts;
    hmac_vsp_log((uint8_t *)slice, VSP_LOG_BITMAP_RTT, VSP_LOG_DATATYPE_RTT, VSP_LOG_TYPE_RTT);

    if (oal_likely(!hmac_is_vsp_source_cfg_pkt(slice->head))) {
        vsp_vcodec_tx_send_done(&slice->tx_result); // 发送完成后通知venc，venc用于释放buff，利于循环使用
        hmac_vsp_source_data_pkt_tx_comp_proc(source, slice);
    } else {
        hmac_vsp_source_cfg_pkt_tx_comp_proc(source, slice);
        // cfg帧
    }
}

static void hmac_vsp_source_update_tx_result_layer_succ_num(hmac_vsp_tx_slice_stru *slice, uint16_t idx)
{
    vsp_msdu_hdr_stru *vsp_hdr = (vsp_msdu_hdr_stru*)(hmac_vsp_get_packet_va(idx) + hal_host_tx_get_msdu_dscr_len());

    if (oal_unlikely(!hmac_vsp_source_pkt_tx_succ((uint8_t *)hmac_vsp_get_packet_va(idx)))) {
        oam_warning_log3(0, 0, "{hmac_tx_complete_packet_process::frame[%d] slice[%d] layer[%d]tx failed}",
            vsp_hdr->frame_id, vsp_hdr->slice_num, vsp_hdr->layer_num);
        return;
    }

    if (hmac_is_vsp_source_cfg_pkt(slice->head) == 0) { // cfg帧发送完成不增加成功包数，一定要等到收到feedback帧确保发送成功
        hmac_vsp_source_inc_tx_result_layer_succ_num(slice, vsp_hdr->layer_num);
    }
}

static void hmac_tx_complete_packet_process(hmac_vsp_source_stru *source, uint16_t idx, uint32_t cur_tsf)
{
    hmac_vsp_tx_slice_stru *slice = NULL;
    hmac_vsp_tx_slice_stru *dequeue_slice = NULL;

    slice = hmac_vsp_get_slice_of_packet(idx);
    if (!slice) {
        oam_error_log0(0, 0, "{hmac_tx_complete_packet_process::record slice is null!}");
        return;
    }

    hmac_vsp_source_update_tx_result_layer_succ_num(slice, idx);

    if (--slice->pkts_in_ring != 0) {
        return;
    }

    dequeue_slice = hmac_vsp_slice_dequeue(&source->free_queue);
    if (!dequeue_slice) {
        oam_error_log0(0, 0, "{hmac_vsp_slice_tx_complete_proc::dequeue_slice NULL!}");
    }

    if (dequeue_slice != slice) {
        oam_error_log0(0, 0, "{hmac_vsp_slice_tx_complete_proc::slice mismatch!}");
    }

    hmac_vsp_slice_tx_complete_proc(source, slice, cur_tsf);
}

void hmac_vsp_tx_complete_process(void)
{
    hmac_vsp_set_evt_flag(&g_vsp_source, VSP_SRC_EVT_TX_COMP);
}

OAL_STATIC void hmac_vsp_tx_complete_handler(hmac_vsp_source_stru *source)
{
    hmac_msdu_info_ring_stru *tx_ring = &source->vsp_base.tid_info->tx_ring;
    un_rw_ptr release_ptr, target_ptr;
    unsigned long flags = 0;
    uint32_t cur_tsf = hmac_vsp_get_timestamp(&source->vsp_base);
    /*
        spin lock锁范围内不允许上下文切换
        tx complete流程会调用到stop timer写common timer寄存器，前提得唤醒dev
        而hal_pm_try_wakeup_dev_lock是允许线程睡眠，所以需要将hal_pm_try_wakeup_dev_lock提前到spin lock前
    */
    hal_pm_try_wakeup_forbid_sleep(HAL_PM_FORBIDE_SLEEP_VSP_TX_COMPLETE);
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_VSP_TX_COMPLETE);
        return;
    }
    oal_mutex_lock(&tx_ring->tx_lock);
    oal_spin_lock_irq_save(&source->vsp_base.vsp_lock, &flags);
    release_ptr.rw_ptr = tx_ring->release_index;
    target_ptr.rw_ptr = tx_ring->base_ring_info.read_index;
    while (hal_tx_rw_ptr_compare(release_ptr, target_ptr) == RING_PTR_SMALLER) {
        hmac_tx_complete_packet_process(source, release_ptr.st_rw_ptr.bit_rw_ptr, cur_tsf);

        hmac_tx_reset_msdu_info(tx_ring, release_ptr);
        hmac_tx_msdu_ring_inc_release_ptr(tx_ring);
        release_ptr.rw_ptr = tx_ring->release_index;
    }
    oal_spin_unlock_irq_restore(&source->vsp_base.vsp_lock, &flags);
    oal_mutex_unlock(&tx_ring->tx_lock);
    hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_VSP_TX_COMPLETE);
}

static uint32_t hmac_vsp_tx_set_msdu_info(hmac_vsp_info_stru *vsp_info, msdu_info_stru *msdu_info, uintptr_t iova,
                                          uint16_t msdu_len)
{
    uint32_t ret;
    uint64_t devva = 0;

    ret = (uint32_t)pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)iova, &devva);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_TX, "{hmac_vsp_tx_set_msdu_info::hostca to devva failed[%d]}", ret);
        return ret;
    }

    msdu_info->msdu_addr_lsb = get_low_32_bits(devva);
    msdu_info->msdu_addr_msb = get_high_32_bits(devva);
    msdu_info->msdu_len = msdu_len;
    msdu_info->data_type = DATA_TYPE_ETH;
#ifdef _PRE_WLAN_CHBA_MGMT
    if (hmac_chba_vap_start_check(vsp_info->hmac_vap) == OAL_TRUE) {
        // chba模式下to_ds 和 from_ds都是0
        msdu_info->from_ds = 0;
        msdu_info->to_ds = 0;
    } else
#endif
    {
        msdu_info->to_ds = vsp_info->hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA;
        msdu_info->from_ds = !msdu_info->to_ds;
    }
    return OAL_SUCC;
}

/* 成功返回ring idx， 失败返回-1 */
static int32_t hmac_vsp_tx_ring_push_msdu(hmac_vsp_info_stru *vsp_info, uintptr_t va, uintptr_t iova, uint16_t msdu_len)
{
    hmac_msdu_info_ring_stru *tx_ring = &vsp_info->tid_info->tx_ring;
    un_rw_ptr write_ptr = { .rw_ptr = tx_ring->base_ring_info.write_index };
    msdu_info_stru msdu_info = { 0 };
    uint8_t msdu_dscr_len = hal_host_tx_get_msdu_dscr_len();

    if (oal_unlikely(hmac_tid_ring_full(tx_ring))) {
        return -1;
    }
    memset_s((uint8_t *)(uintptr_t)va, msdu_dscr_len, 0, msdu_dscr_len);
    if (hmac_vsp_tx_set_msdu_info(vsp_info, &msdu_info, iova, msdu_len) != OAL_SUCC) {
        return -1;
    }
    hal_host_tx_set_msdu_ring(&tx_ring->ring_buf, &msdu_info, write_ptr.st_rw_ptr.bit_rw_ptr);
    hmac_tx_msdu_ring_inc_write_ptr(tx_ring);

    return write_ptr.st_rw_ptr.bit_rw_ptr;
}

/*
 * 功能描述  : vsp tx done发送完成通知
 * 1.日    期  : 2021年1月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
static int32_t hmac_vsp_tx_packet(hmac_vsp_info_stru *vsp_info, uintptr_t va, uintptr_t iova, uint16_t msdu_len)
{
    hmac_msdu_info_ring_stru *tx_ring = &vsp_info->tid_info->tx_ring;
    int32_t idx;

    if (!hmac_tx_ring_alloced(tx_ring) && hmac_alloc_tx_ring_after_inited(vsp_info->tid_info, tx_ring) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_vsp_tx_slice_paket::ring NULL}");
        return -1;
    }

    idx = hmac_vsp_tx_ring_push_msdu(vsp_info, va, iova, msdu_len);
    if (idx < 0) {
        oam_warning_log0(0, OAM_SF_TX, "hmac_vsp_tx_slice_paket: tx ring push fail");
        return -1;
    }

    return idx;
}

static void hmac_vsp_source_slice_update_vsync_ts(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    uint32_t vsync = source->last_vsync;

    /* slice到达wifi驱动的时延大于2*tslice，上层处理slice占用的时间开销过多 */
    if (oal_unlikely(hmac_vsp_calc_runtime(vsync, slice->arrive_ts) > 2 * source->vsp_base.param.tslice)) {
        oam_warning_log2(0, 0, "{hmac_vsp_build_tx_slice_struct::slice maybe late, vsync %d, arrive %d}",
            vsync, slice->arrive_ts);
    }

    slice->vsync_ts = vsync;
    source->pkt_ts_vsync = vsync;
}

static inline void hmac_vsp_source_cfg_pkt_update_tx_slice(hmac_vsp_tx_slice_stru *slice)
{
    slice->retrans_cnt = 3; /* cfg pkt重传3次, 尽可能保证传输成功 */
    hmac_vsp_source_set_tx_result_cfg_pkt(slice);
}

static void hmac_vsp_source_init_tx_slice_base_info(
    hmac_vsp_tx_slice_stru *slice, tx_layer_ctrl *layer, hmac_vsp_source_stru *source)
{
    uint8_t layer_idx;
    tx_layer_ctrl *curr_layer = layer;

    slice->arrive_ts = hmac_vsp_get_timestamp(&source->vsp_base);
    slice->vsync_ts = 0;
    slice->head = layer;
    slice->curr = layer;
    slice->tx_pos = 0;
    slice->pkts_in_ring = 0;
    slice->frame_id = hmac_vsp_source_get_tx_frame_id(layer);
    slice->slice_id = hmac_vsp_source_get_tx_slice_id(layer);
    slice->feedback = OAL_FALSE;
    slice->is_rpt = OAL_FALSE;
    slice->retrans_cnt = 0;
    for (layer_idx = 0; layer_idx < MAX_LAYER_NUM; layer_idx++) {
        if (curr_layer == NULL) {
            slice->layer_info[layer_idx].layer_number = (uint16_t)INVALID_LAYER_NUM;
            break;
        }
        slice->layer_info[layer_idx].layer_number = curr_layer->layer_number;
        slice->layer_info[layer_idx].paket_number = curr_layer->paket_number;
        slice->layer_info[layer_idx].normal_pack_length = curr_layer->normal_pack_length;
        slice->layer_info[layer_idx].last_paket_len = curr_layer->last_paket_len;

        curr_layer = (tx_layer_ctrl *)curr_layer->next;
    }
}

static void hmac_vsp_source_init_tx_slice(
    hmac_vsp_tx_slice_stru *slice, tx_layer_ctrl *layer, hmac_vsp_source_stru *source)
{
    hmac_vsp_source_init_tx_slice_base_info(slice, layer, source);

    hmac_vsp_source_slice_update_vsync_ts(source, slice);

    hmac_vsp_source_init_slice_tx_result(&source->vsp_base, slice, layer);

    if (oal_likely(!hmac_is_vsp_source_cfg_pkt(layer))) {
        hmac_vsp_source_set_vsp_hdr_cfg_pkt(slice, OAL_FALSE);
    } else {
        hmac_vsp_source_cfg_pkt_update_tx_slice(slice);
        hmac_vsp_source_set_vsp_hdr_cfg_pkt(slice, OAL_TRUE);
    }
}

static uint32_t hmac_vsp_source_tx_pkt(
    hmac_vsp_info_stru *vsp_info, hmac_vsp_tx_slice_stru *slice, tx_layer_ctrl *layer, uint32_t offset, uint16_t len)
{
    int32_t ring_idx;
    uintptr_t msdu_iova = hmac_vsp_source_get_pkt_msdu_dscr(layer->iova_addr + offset);
    uintptr_t msdu_va = hmac_vsp_source_get_pkt_msdu_dscr(layer->data_addr + offset);
    uintptr_t vsp_va = hmac_vsp_source_get_pkt_vsp_hdr(layer->data_addr + offset);

    ring_idx = hmac_vsp_tx_packet(vsp_info, msdu_va, msdu_iova, len - VSP_MSDU_CB_LEN);
    if (ring_idx < 0) {
        return OAL_FAIL;
    }

    hmac_vsp_record_packet_info((uint16_t)ring_idx, msdu_va, slice);
    hmac_tx_reg_write_ring_info(vsp_info->tid_info, TID_CMDID_ENQUE, OAL_FALSE);
    hmac_vsp_log((uint8_t *)vsp_va, VSP_LOG_BITMAP_PKT_FEEDBACK, VSP_LOG_DATATYPE_PKT, VSP_LOG_TYPE_SEND_PKT);

    return OAL_SUCC;
}

static uint16_t hmac_vsp_tx_layer_get_pkt_len(uint32_t pkt_idx, tx_layer_ctrl *layer)
{
    return pkt_idx != layer->paket_number - 1 ? layer->normal_pack_length : layer->last_paket_len;
}

static uint32_t hmac_vsp_source_tx_layer(
    hmac_vsp_info_stru *vsp_info, hmac_vsp_tx_slice_stru *slice, tx_layer_ctrl *layer)
{
    uint16_t pkt_len;
    uint32_t offset = 0;
    uint32_t pkt_idx;
    uint32_t ret;
    unsigned long flags = 0;

    if (layer->paket_number == 0) {
        oam_error_log1(0, 0, "{hmac_vsp_source_tx_layer::paket_number[%d]}", layer->paket_number);
    }

    for (pkt_idx = 0; pkt_idx < layer->paket_number; pkt_idx++) {
        pkt_len = hmac_vsp_tx_layer_get_pkt_len(pkt_idx, layer);

        oal_mutex_lock(&vsp_info->tid_info->tx_ring.tx_lock);
        oal_spin_lock_irq_save(&vsp_info->vsp_lock, &flags);
        ret = hmac_vsp_source_tx_pkt(vsp_info, slice, layer, offset, pkt_len);
        oal_spin_unlock_irq_restore(&vsp_info->vsp_lock, &flags);
        oal_mutex_unlock(&vsp_info->tid_info->tx_ring.tx_lock);

        if (ret != OAL_SUCC) {
            oam_error_log0(0, 0, "{hmac_vsp_source_tx_layer::pkt push ring failed}");
            break;
        }

        hmac_tx_sched_trigger(vsp_info->hmac_vap->st_vap_base_info.uc_vap_id, vsp_info->tid_info);
        offset += pkt_len;
    }

    return offset;
}

static void hmac_vsp_source_tx_slice_abnormal_process(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    hmac_vsp_slice_delete_entry(&source->free_queue, slice);
    vsp_vcodec_tx_send_done(&slice->tx_result); // 发送完成后通知venc，venc用于释放buff，利于循环使用
    vsp_vcodec_tx_pkt_done(&slice->tx_result);
    oal_free(slice);
}

static void hmac_vsp_source_tx_slice(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    tx_layer_ctrl *layer = slice->head;
    hmac_vsp_info_stru *vsp_info = &source->vsp_base;

    do {
        if (layer->normal_pack_length < 96 || layer->last_paket_len < 96) { // 96: pkt min length
            oam_error_log3(0, 0, "{hmac_vsp_source_tx_slice::frm[%d]pack_length[%d][%d]}", slice->frame_id,
                layer->normal_pack_length, layer->last_paket_len);
            hmac_vsp_source_tx_slice_abnormal_process(source, slice);
            return;
        }

        hmac_vsp_source_stat_tx_msdu(source, hmac_vsp_source_tx_layer(vsp_info, slice, layer));
    } while ((layer = (tx_layer_ctrl *)layer->next) != NULL);
}

static void hmac_vsp_source_update_pkt_timestamp(hmac_vsp_source_stru *source, vsp_msdu_hdr_stru *vsp_hdr)
{
    if (oal_unlikely(vsp_hdr->ts_type != VSP_TIMESTAMP_NOT_USED)) {
        oam_error_log1(0, 0, "{hmac_vsp_source_update_pkt_timestamp::unexpected timestamp type[%d]}", vsp_hdr->ts_type);
        return;
    }

    vsp_hdr->timestamp = source->pkt_ts_vsync;
    vsp_hdr->ts_type = VSP_TIMESTAMP_WIFI;
}

static void hmac_vsp_source_update_slice_timestamp(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    tx_layer_ctrl *layer = slice->head;
    uintptr_t va;
    uint16_t idx;

    while (layer) {
        for (idx = 0; idx < layer->paket_number; idx++) {
            va = hmac_vsp_source_get_pkt_vsp_hdr(layer->data_addr + idx * layer->normal_pack_length);
            hmac_vsp_source_update_pkt_timestamp(source, (vsp_msdu_hdr_stru *)va);
        }
        layer = (tx_layer_ctrl *)layer->next;
    }
}

static uint32_t hmac_vsp_source_prepare_slice(hmac_vsp_source_stru *source, tx_layer_ctrl *layer_head)
{
    hmac_vsp_tx_slice_stru *slice = oal_memalloc(sizeof(hmac_vsp_tx_slice_stru));

    if (!slice) {
        oam_error_log0(0, 0, "hmac_vsp_tx_slice::alloc slice queue entry failed");
        return OAL_FAIL;
    }

    /* vsp simulate vsync, del after vsync intr enabled */
    /* 每个slice过来都记录 */
    source->last_vsync = hmac_vsp_get_timestamp(&source->vsp_base);

    hmac_vsp_source_inc_slice_total(source);
    hmac_vsp_source_init_tx_slice(slice, layer_head, source);
    hmac_vsp_source_update_slice_timestamp(source, slice);
    hmac_vsp_log((uint8_t *)slice, VSP_LOG_BITMAP_SLICE_INFO, VSP_LOG_DATATYPE_SLICE, VSP_LOG_TYPE_BUILD_SLICE);
    hmac_vsp_slice_enqueue(&source->free_queue, slice);

    hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_NEW_SLICE);

    return OAL_SUCC;
}

/* 发送端入口, venc调用该接口, 以layer list的形式下发slice */
bool wifi_tx_venc_pkg(tx_layer_ctrl *layer_list)
{
    hmac_vsp_source_stru *source = &g_vsp_source;
    mac_user_stru *mac_user;

    if (oal_unlikely(!layer_list || !source->vsp_base.enable || !source->vsp_base.hmac_user)) {
        oam_error_log0(0, 0, "{wifi_tx_venc_pkg::vsp not enable, cannot send packet!}");
        return OAL_FALSE;
    }

    mac_user = hmac_vsp_info_get_mac_user(&source->vsp_base);
    if (oal_unlikely(oal_compare_mac_addr(layer_list->mac_ra_address, mac_user->auc_user_mac_addr))) {
        oam_error_log3(0, 0, "{wifi_tx_venc_pkg::mismatch slice ra %02X:XX:XX:XX:%02X:%02X}",
            layer_list->mac_ra_address[MAC_ADDR_0],
            layer_list->mac_ra_address[MAC_ADDR_4], layer_list->mac_ra_address[MAC_ADDR_5]);
        oam_error_log3(0, 0, "{wifi_tx_venc_pkg::mismatch user mac %02X:XX:XX:XX:%02X:%02X}",
            mac_user->auc_user_mac_addr[MAC_ADDR_0],
            mac_user->auc_user_mac_addr[MAC_ADDR_4], mac_user->auc_user_mac_addr[MAC_ADDR_5]);
        return OAL_FALSE;
    }

    return hmac_vsp_source_prepare_slice(source, layer_list) == OAL_SUCC;
}

static uint64_t g_tx_sync_stage_addr;
uint32_t hmac_set_vsp_info_addr(frw_event_mem_stru *frw_mem)
{
    frw_event_stru *frw_event = NULL;
    mac_d2h_device_vspinfo_addr *vsp_info = NULL;
    uint32_t ret;

    if (!frw_mem) {
        oam_error_log0(0, 0, "{hmac_set_vsp_info_addr::frw mem NULL!}");
        return OAL_FAIL;
    }

    frw_event = frw_get_event_stru(frw_mem);
    vsp_info = (mac_d2h_device_vspinfo_addr *)frw_event->auc_event_data;
    ret = (uint32_t)oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, vsp_info->device_vspinfo_addr, &g_tx_sync_stage_addr);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, 0, "{hmac_set_vsp_info_addr::vsp addr sync failed!}");
    }

    return ret;
}

/* 清ring需要host/device同步进行, 通过读写g_tx_sync_stage_addr对应的device内存, 来共享双方的状态 */
static uint32_t hmac_vsp_clear_tx_ring_sync(hmac_vsp_info_stru *vsp_info)
{
    uint32_t ret;
    uint32_t check_cnt = 1001;
    oal_spin_lock(&vsp_info->vsp_lock);
    hal_host_writel_irq(VSP_TX_SYNC_LOCK_ACQUIRE, g_tx_sync_stage_addr);
    do {
        oal_udelay(1);
        check_cnt--;
    } while ((ret = hal_host_readl_irq(g_tx_sync_stage_addr)) == VSP_TX_SYNC_LOCK_ACQUIRE && check_cnt);
    hal_host_writel_irq(VSP_TX_SYNC_LOCK_RELEASE, g_tx_sync_stage_addr);
    oal_spin_unlock(&vsp_info->vsp_lock);

    oam_warning_log2(0, 0, "{hmac_vsp_clear_tx_ring_sync::sync res %d, check %d}", ret, check_cnt);

    return ret == VSP_TX_SYNC_CLEAR_DONE ? OAL_SUCC : OAL_FAIL;
}

static void hmac_vsp_source_timer_intr_callback(void *data)
{
    hmac_vsp_source_stru *source = &g_vsp_source;

    hmac_vsp_clear_tx_ring_sync(&source->vsp_base);
    if (oal_unlikely(!hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_TIMEOUT))) {
        oam_error_log0(0, 0, "{hmac_vsp_timer_intr_callback::last timeout event not process yet!!}");
    }
}

static uint32_t hmac_vsp_source_update_slice_layer_succ_num(
    hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice, vsp_feedback_frame *fb)
{
    uint8_t layer_idx;

    if (memcpy_s(slice->tx_result.layer_suc_num, MAX_LAYER_NUM * VSP_LAYER_FEEDBACK_SIZE,
        fb->layer_succ_num, fb->layer_num * VSP_LAYER_FEEDBACK_SIZE) != EOK) {
        oam_error_log1(0, 0, "{hmac_vsp_source_rx_process::memcpy failed, layer num %d}", fb->layer_num);
        return OAL_FAIL;
    }

    for (layer_idx = 0; layer_idx < MAX_LAYER_NUM; layer_idx++) {
        if ((slice->tx_result.layer_suc_num[layer_idx] & BIT15) == 0) {
            break;
        }
        hmac_vsp_source_stat_inc_rx_succ_feedback(&source->source_stat, layer_idx);
    }

    return OAL_SUCC;
}

static uint32_t hmac_vsp_fill_feedback_info(
    hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice, vsp_feedback_frame *fb)
{
    if (slice->feedback) {
        oam_warning_log2(0, 0, "{hmac_vsp_fill_feedback_info::frm%d slice%d dup feedback!}", fb->frm_id, fb->slice_id);
        return OAL_FAIL;
    }

    slice->feedback = OAL_TRUE;

    return hmac_vsp_source_update_slice_layer_succ_num(source, slice, fb);
}

typedef struct {
    hmac_vsp_source_stru *source;
    vsp_feedback_frame *fb;
    uint32_t fb_len;
} hmac_vsp_feedback_context_stru;

static uint8_t hmac_vsp_source_check_feedback_slice(hmac_vsp_tx_slice_stru *slice,
                                                    hmac_vsp_feedback_context_stru *context)
{
    int slice_cnt = context->source->vsp_base.param.slice_cnt; // 一个视频帧slice个数
    uint16_t comp_slice_id = slice->frame_id * slice_cnt + slice->slice_id; // comp queue中slice的id
    uint16_t fb_slice_id = context->fb->frm_id * slice_cnt + context->fb->slice_id; // feedback的slice的id
    uint16_t half = VSP_FRAME_ID_TOTAL * slice_cnt / 2;

    if (context->fb->is_cfg_pkt == OAL_TRUE) {
        if (slice->is_cfg_pkt == OAL_TRUE) {
            oam_warning_log1(0, 0, "{hmac_vsp_source_check_feedback_slice::is_cfg_fb frm[%d]}", slice->frame_id);
            return OAL_TRUE;
        } else {
            oam_warning_log1(0, 0, "{hmac_vsp_source_check_feedback_slice::cfg_fb frm[%d] CONTINUE}", slice->frame_id);
            return OAL_FALSE;
        }
    }

    // 需要将feedback优先tx_comp的保留在队列中，其他的均出队处理。
    // 考虑到slice id的翻转，差值小于最大frame&slice id一半为有效值
    if ((fb_slice_id > comp_slice_id && fb_slice_id - comp_slice_id < half) ||
        (fb_slice_id < comp_slice_id && comp_slice_id - fb_slice_id > half)) {
        return OAL_FALSE;
    }
    return OAL_TRUE;
}

static uint8_t hmac_vsp_source_fill_feedback_info(hmac_vsp_tx_slice_stru *slice, void *param)
{
    hmac_vsp_feedback_context_stru *context = (hmac_vsp_feedback_context_stru *)param;

    if (hmac_vsp_source_check_feedback_slice(slice, context) == OAL_FALSE) {
        return OAL_CONTINUE; // netbuf保留在队列中
    }
    if (slice->frame_id == context->fb->frm_id && slice->slice_id == context->fb->slice_id) {
        hmac_vsp_fill_feedback_info(context->source, slice, context->fb);
        hmac_vsp_source_stat_inc_feedback_cnt(&context->source->source_stat);
        hmac_vsp_log((uint8_t *)context->fb, VSP_LOG_BITMAP_PKT_FEEDBACK, VSP_LOG_DATATYPE_FEEDBACK,
                     VSP_LOG_TYPE_RX_FEEDBACK);
    }
    // 没有匹配上，可能是feedback的重复包，已经处理过feedback的slice了，那么feedback的netbuf也需要出队处理
    return OAL_RETURN; // netbuf出队处理
}

static inline uint8_t hmac_vsp_source_foreach_fill_feedback_info(
    hmac_vsp_source_stru *source, hmac_vsp_feedback_context_stru *context)
{
    return hmac_vsp_slice_queue_foreach_safe(&source->comp_queue, hmac_vsp_source_fill_feedback_info, (void *)context);
}

static uint8_t hmac_vsp_source_check_feedback_timeout(hmac_vsp_info_stru *vsp_info, hmac_vsp_tx_slice_stru *s)
{
    uint32_t curr_ts = hmac_vsp_get_timestamp(vsp_info);
    uint32_t runtime = hmac_vsp_calc_runtime(s->arrive_ts, curr_ts);
    if (runtime >= vsp_info->param.tfeedback) {
        oam_error_log4(0, 0, "{hmac_vsp_source_check_feedback_timeout::frame[%d] slice[%d] curr_ts[%d][%d]\
            feedback timeout!}", s->frame_id, s->slice_id, curr_ts, runtime);
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

static inline uint8_t hmac_vsp_source_cfg_pkt_feedback_fail(hmac_vsp_tx_slice_stru *slice)
{
    return hmac_is_vsp_source_cfg_pkt(slice->head) && !hmac_vsp_source_get_tx_result_layer_succ_num(slice, 0);
}

static void hmac_vsp_source_update_tx_result(hmac_vsp_tx_slice_stru *slice)
{
    uint8_t idx = 0;

    for (idx = 0; idx < MAX_LAYER_NUM; idx++) {
        if (slice->layer_info[idx].layer_number == INVALID_LAYER_NUM ||
            hmac_vsp_source_tx_layer_comp(slice, idx) != OAL_TRUE) {
            break;
        }
        hmac_vsp_source_set_tx_result_layer_tx_comp(slice, idx);
    }

    /* 某个layer没发全，则后续的layer都不需要上报 */
    for (; idx < MAX_LAYER_NUM; idx++) {
        hmac_vsp_source_reset_tx_result_layer_succ_num(slice, idx);
    }

    hmac_vsp_source_set_tx_result_rpt_type(slice);
}

static inline void hmac_vsp_source_update_layer_max(hmac_vsp_source_stru *source, hmac_vsp_tx_slice_stru *slice)
{
    hmac_vsp_source_update_tx_result_layer0_max(slice, hmac_vsp_source_get_layer0_max(source, slice));
    hmac_vsp_source_update_tx_result_layerall_max(slice, hmac_vsp_source_get_layerall_max(source, slice));
}

static uint8_t hmac_vsp_rpt_slice(hmac_vsp_tx_slice_stru *slice, void *param)
{
    hmac_vsp_source_stru *source = param;

    if (!slice->feedback && !hmac_vsp_source_check_feedback_timeout(&source->vsp_base, slice)) {
        return OAL_CONTINUE;
    }

    if (!slice->feedback) {
        hmac_vsp_source_update_tx_result(slice);
    }

    hmac_vsp_source_update_layer_max(source, slice);

    hmac_vsp_source_slice_tx_done(slice);

    return OAL_CONTINUE;
}

static inline void hmac_vsp_source_foreach_rpt_slice(hmac_vsp_source_stru *source)
{
    hmac_vsp_slice_queue_foreach_safe(&source->comp_queue, hmac_vsp_rpt_slice, source);
}

static uint8_t hmac_vsp_process_one_feedback(hmac_vsp_source_stru *source, oal_netbuf_stru *netbuf)
{
    uint8_t ret;
    hmac_vsp_feedback_context_stru context = {
        .source = source,
        .fb = (vsp_feedback_frame *)oal_netbuf_data(netbuf),
        .fb_len = oal_netbuf_len(netbuf),
    };
    if (context.fb == NULL ||
        context.fb_len < sizeof(vsp_feedback_frame) ||
        context.fb_len > sizeof(vsp_feedback_frame) + VSP_LAYER_FEEDBACK_SIZE * context.fb->layer_num) {
        oam_error_log1(0, 0, "{hmac_vsp_process_one_feedback::frame len[%d] out of range!}", context.fb_len);
        return OAL_TRUE; // 不符合长度的netbuf，需要出队丢弃
    }

    ret = hmac_vsp_source_foreach_fill_feedback_info(source, &context);
    hmac_vsp_source_foreach_rpt_slice(source);
    return ret;
}

OAL_STATIC void hmac_vsp_feedback_handler(hmac_vsp_source_stru *source)
{
    oal_netbuf_stru *netbuf = oal_netbuf_peek(&source->feedback_q);
    oal_netbuf_stru *netbuf_tmp = NULL;

    hmac_vsp_watchdog_cnt_update(source);
    while (netbuf != NULL) {
        // feedback优先于tx_complete场景的netbuf保留在队列里
        if (hmac_vsp_process_one_feedback(source, netbuf) == OAL_FALSE) {
            netbuf = oal_netbuf_peek_next(netbuf, &source->feedback_q);
            continue;
        }
        netbuf_tmp = oal_netbuf_peek_next(netbuf, &source->feedback_q);
        oal_netbuf_delete(netbuf, &source->feedback_q);
        oal_netbuf_free(netbuf);
        netbuf = netbuf_tmp;
    }
    hmac_vsp_source_foreach_rpt_slice(source);
}

/* 接收流程调用, 发送端接收到一个反馈帧 */
void hmac_vsp_source_rx_amsdu_proc(oal_netbuf_stru *netbuf)
{
    hmac_vsp_source_stru *source = &g_vsp_source;

    if (oal_unlikely(!netbuf)) {
        return;
    }

    oal_netbuf_add_to_list_tail(netbuf, &source->feedback_q);
    hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_RX_FEEDBACK);
}

static void hmac_vsp_source_start_tx_slice_timer(hmac_vsp_info_stru *vsp_info, hmac_vsp_tx_slice_stru *slice)
{
    /* 每个slice刷新定时参照的vsync */
    vsp_info->timer_ref_vsync = slice->vsync_ts;

    hmac_vsp_set_slice_timeout(vsp_info, slice->slice_id, slice->is_cfg_pkt);
}

static void hmac_vsp_source_tx_sched(void)
{
    hmac_vsp_source_stru *source = &g_vsp_source;
    hmac_vsp_tx_slice_stru *slice = hmac_vsp_slice_queue_peek(&source->free_queue);

    if (!slice) {
        oam_error_log0(0, 0, "{hmac_vsp_source_tx_sched::slice NULL!}");
        return;
    }

    hmac_vsp_source_start_tx_slice_timer(&source->vsp_base, slice);
    hmac_vsp_source_tx_slice(source, slice);
}

OAL_STATIC void hmac_vsp_new_slice_handler(hmac_vsp_source_stru *source)
{
    hmac_msdu_info_ring_stru *tx_msdu_ring = &source->vsp_base.tid_info->tx_ring;
    hmac_vsp_watchdog_cnt_update(source);
    /* 新的slice到来时加速上报已超时的slice, 避免未收到feedback导致slice堆积、vdec无内存可用 */
    hmac_vsp_source_foreach_rpt_slice(source);

    if (!hmac_vsp_tid_ring_empty(&source->vsp_base) || hmac_vsp_slice_queue_empty(&source->free_queue)) {
        oal_io_print("hmac_vsp_new_slice_handler:write[0x%x]read[0x%x]",
            tx_msdu_ring->base_ring_info.write_index, tx_msdu_ring->base_ring_info.read_index);
        return;
    }

    hmac_vsp_source_tx_sched();
}

/*
 * VSP发送端通过事件驱动
 * 1. TIMEOUT: slice发送超时, 直接更新tx ring读指针, 并触发一次TX_COMP事件;
 * 2. TX_COMP: slice发送完成, 释放tx ring中的netbuf, 并判断某个slice是否发送完成,
 *             若是则加入complete queue, 并触发一次NEW_SLICE事件;
 * 3. NEW_SLICE: 尝试调度free queue中缓存的待发送slice;
 * 4. RX_FEEDBACK: 收到接收端发送的反馈帧, 在complete queue中查找对应的slice, 回填信息后上报给venc
 */
static void (*g_event_handler[VSP_SRC_EVT_BUTT])(hmac_vsp_source_stru *) = {
    [VSP_SRC_EVT_TIMEOUT] = hmac_vsp_source_timeout_handler,
    [VSP_SRC_EVT_TX_COMP] = hmac_vsp_tx_complete_handler,
    [VSP_SRC_EVT_NEW_SLICE] = hmac_vsp_new_slice_handler,
    [VSP_SRC_EVT_RX_FEEDBACK] = hmac_vsp_feedback_handler,
};

int hmac_vsp_source_thread(void *data)
{
#if _PRE_OS_VERSION_LINUX == _PRE_OS_VERSION
    int32_t ret;
    uint32_t evt_idx;
    hmac_vsp_source_stru *source = &g_vsp_source;

    allow_signal(SIGTERM);
    while (!oal_kthread_should_stop()) {
        ret = oal_wait_event_interruptible_m(source->evt_wq, READ_ONCE(source->evt_flags));
        if (ret) {
            oam_error_log1(0, 0, "{hmac_vsp_source_thread::interrupt by a signal, ret %d}", ret);
            break;
        }
        for (evt_idx = 0; evt_idx < VSP_SRC_EVT_BUTT; evt_idx++) {
            if (test_and_clear_bit(evt_idx, &source->evt_flags)) {
                g_event_handler[evt_idx](source);
            }
        }
    }
#endif
    return 0;
}

static void hmac_vsp_source_queue_init(hmac_vsp_source_stru *source)
{
    hmac_vsp_slice_queue_init(&source->free_queue);
    hmac_vsp_slice_queue_init(&source->comp_queue);
    oal_netbuf_list_head_init(&source->feedback_q);
}

static void hmac_vsp_source_set_fast_cpus(void)
{
#if defined(CONFIG_ARCH_HISI) && defined(CONFIG_NR_CPUS)
#if CONFIG_NR_CPUS > OAL_BUS_HPCPU_NUM
    struct cpumask fast_cpus;
    external_get_fast_cpus(&fast_cpus);
    oal_cpumask_clear_cpu(cpumask_first(&fast_cpus), &fast_cpus);
    set_cpus_allowed_ptr(g_vsp_source.evt_loop, &fast_cpus);
#endif
#endif
}

static void hmac_vsp_watchdog_timer_deinit(hmac_vsp_source_stru *source)
{
    if (source->vsp_watchdog_timer.en_is_registerd == OAL_FALSE) {
        return;
    }

    frw_timer_immediate_destroy_timer_m(&source->vsp_watchdog_timer);
}

uint8_t hmac_vsp_source_clear_comp_queue(hmac_vsp_tx_slice_stru *slice, void *param)
{
    hmac_vsp_source_stru *source = param;

    hmac_vsp_slice_delete_entry(&source->comp_queue, slice);
    vsp_vcodec_tx_pkt_done(&slice->tx_result);
    oal_free(slice);
    return OAL_CONTINUE;
}
uint8_t hmac_vsp_source_clear_free_queue(hmac_vsp_tx_slice_stru *slice, void *param)
{
    hmac_vsp_source_stru *source = param;

    hmac_vsp_source_tx_slice_abnormal_process(source, slice);
    return OAL_CONTINUE;
}

void hmac_vsp_source_queue_deinit(hmac_vsp_source_stru *source)
{
    hmac_vsp_slice_queue_foreach_safe(&source->comp_queue, hmac_vsp_source_clear_comp_queue, source);

    hmac_vsp_slice_queue_foreach_safe(&source->free_queue, hmac_vsp_source_clear_free_queue, source);
}

static void hmac_vsp_source_deinit(void)
{
    hmac_vsp_source_stru *source = &g_vsp_source;

    hmac_vsp_deinit(&source->vsp_base);
}

static uint32_t hmac_vsp_source_thread_init(hmac_vsp_source_stru *source)
{
    // vsp tx线程已经创建，就返回（该线程不会stop）
    if (source->evt_loop != NULL) {
        return OAL_SUCC;
    }
    oal_wait_queue_init_head(&source->evt_wq);
    source->evt_flags = 0;

    /* 98: 线程优先级; -1: 非绑核 */
    source->evt_loop = oal_thread_create(hmac_vsp_source_thread, NULL, NULL, "vsp_evt_loop", SCHED_FIFO, 98, -1);
    if (!source->evt_loop) {
        oam_error_log0(0, 0, "{hmac_vsp_source_thread_init::thread init failed!}");
        return OAL_FAIL;
    }

    hmac_vsp_source_set_fast_cpus();

    return OAL_SUCC;
}

static inline void hmac_vsp_source_layer_max_mgmt_init(hmac_vsp_source_stru *source)
{
    source->layer_max_mgmt.layer0_max = 0;
    source->layer_max_mgmt.layerall_max = 0;
    source->layer_max_mgmt.ratio_th[0] = 100; // layer数据量平滑值100
    source->layer_max_mgmt.ratio_th[1] = 200; // layer数据量平滑值200
    source->layer_max_mgmt.ratio_th[2] = 300; // layer数据量平滑值300
    source->layer_max_mgmt.ratio_th[3] = 500; // layer数据量平滑值500
    source->layer_max_mgmt.factor[0] = 1;     // layer数据量平滑因子1
    source->layer_max_mgmt.factor[1] = 2;     // layer数据量平滑因子2
    source->layer_max_mgmt.factor[2] = 2;     // layer数据量平滑因子2
    source->layer_max_mgmt.factor[3] = 3;     // layer数据量平滑因子3
}

static void hmac_vsp_source_init_timer(hmac_vsp_source_stru *source)
{
    source->vsp_base.timer.func = hmac_vsp_source_timer_intr_callback;
}

static uint32_t hmac_vsp_watchdog_timeout(void *p_arg)
{
    hmac_vsp_source_stru *source = &g_vsp_source;

    if (source->vsp_watchdog_cnt != 0) {
        source->vsp_watchdog_cnt = 0;
        return OAL_SUCC;
    }

    if (hmac_vsp_slice_queue_empty(&source->comp_queue) != OAL_TRUE) {
        oam_warning_log0(0, 0, "hmac_vsp_watchdog_timeout::comp_queue not empty! no event in 50ms");
        hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_RX_FEEDBACK);
    }
    if (hmac_vsp_slice_queue_empty(&source->free_queue) != OAL_TRUE) {
        oam_warning_log0(0, 0, "hmac_vsp_watchdog_timeout::free_queue nit empty! no event in 50ms");
        hmac_vsp_set_evt_flag(source, VSP_SRC_EVT_NEW_SLICE);
    }
    return OAL_SUCC;
}

void hmac_vsp_watchdog_cnt_update(hmac_vsp_source_stru *source)
{
    source->vsp_watchdog_cnt++;
}

static void hmac_vsp_watchdog_timer_init(hmac_vsp_source_stru *source)
{
    if (source->vsp_watchdog_timer.en_is_registerd == OAL_TRUE) {
        return;
    }

    source->vsp_watchdog_cnt = 0;
    frw_timer_create_timer_m(&source->vsp_watchdog_timer,
                             hmac_vsp_watchdog_timeout,
                             50, // 50ms
                             NULL, OAL_TRUE, OAM_MODULE_ID_HMAC, 0);
}

static uint32_t hmac_vsp_source_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, vsp_param *param)
{
    hmac_vsp_source_stru *source = &g_vsp_source;

    hmac_vsp_source_queue_init(source);
    hmac_vsp_source_layer_max_mgmt_init(source);
    memset_s(&source->source_stat, sizeof(hmac_vsp_source_stat_stru), 0, sizeof(hmac_vsp_source_stat_stru));
    memset_s(&g_vsp_tx_pkt_info, sizeof(g_vsp_tx_pkt_info), 0, sizeof(g_vsp_tx_pkt_info));
    source->last_vsync = hmac_vsp_get_timestamp(&g_vsp_source.vsp_base);
    hmac_vsp_source_init_timer(source);
    hmac_vsp_watchdog_timer_init(source);

    return hmac_vsp_source_thread_init(source);
}

uint32_t hmac_vsp_source_cfg(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, vsp_param *param)
{
    if (oal_any_null_ptr3(hmac_vap, hmac_user, param)) {
        return OAL_FAIL;
    }

    if (!hmac_host_ring_tx_enabled()) {
        return OAL_FAIL;
    }

    if (hmac_vsp_init(&g_vsp_source.vsp_base, hmac_vap, hmac_user, param, VSP_MODE_SOURCE) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if (hmac_vsp_source_init(hmac_vap, hmac_user, param) != OAL_SUCC) {
        hmac_vsp_source_deinit();
        return OAL_FAIL;
    }
    g_vsp_source.vsp_base.enable = OAL_TRUE;
    if (hmac_vsp_context_init(&g_vsp_source.vsp_base, hmac_vap, hmac_user) != OAL_SUCC) {
        g_vsp_source.vsp_base.enable = OAL_FALSE;
        hmac_vsp_source_deinit();
        oam_error_log0(0, 0, "hmac_vsp_source_cfg:hmac_vsp_context_init FAIL!");
        return OAL_FAIL;
    }
    hmac_vsp_set_current_handle(&g_vsp_source.vsp_base);
#ifdef CONFIG_VCODEC_VSP_SUPPORT
    hmac_vsp_dma_list_init();
#endif
    return OAL_SUCC;
}
uint32_t hmac_vsp_set_txop_limit(mac_vap_stru *mac_vap, uint32_t switch_value)
{
    mac_ioctl_alg_param_stru alg_param = { 0 };
    if (!is_sta(mac_vap)) {
        return OAL_SUCC;
    }
    alg_param.en_alg_cfg = MAC_ALG_CFG_TXOP_LIMIT_STA_EN;
    alg_param.value = switch_value;
    hmac_config_alg_param(mac_vap, sizeof(mac_ioctl_alg_param_stru), (uint8_t *)&alg_param);
    return OAL_SUCC;
}
void hmac_vsp_source_stop(void)
{
    g_vsp_source.vsp_base.enable = OAL_FALSE; // 先置位 stop过程中VENC下发新帧直接返回失败 不处理
    hmac_vsp_watchdog_timer_deinit(&g_vsp_source);
    hmac_vsp_set_txop_limit(&g_vsp_source.vsp_base.hmac_vap->st_vap_base_info, OAL_TRUE);
    hmac_vsp_context_deinit(&g_vsp_source.vsp_base);
    hmac_vsp_source_queue_deinit(&g_vsp_source);
#ifdef CONFIG_VCODEC_VSP_SUPPORT
    hmac_vsp_clear_dma_buf_list();
#endif
    hmac_vsp_source_deinit();
}

/* 发送端注册给显示模块，在vsync中断调用, 记录tsf，用于与接收端时间同步 */
void hmac_vsp_source_vsync_intr_callback(void)
{
    hmac_vsp_source_stru *source = &g_vsp_source;

    if (!source->vsp_base.enable) {
        return;
    }

    source->last_vsync = hmac_vsp_get_timestamp(&source->vsp_base);
}
uint8_t hmac_check_vsp_params(hmac_vsp_nav_edca_cmd *vsp_param)
{
    // 检查上层下发NAV门限及edca各档位值，保证配置OK
    uint32_t nav_duration[] = {0, 8, 16, 32, 50, 100, 500, 1000, 2000, 4096};
    uint32_t edca_win[] = {0x0, 0x10, 0x21, 0x32, 0x43, 0x64, 0xA4};
    uint8_t nav_match = OAL_FALSE;
    uint8_t edca_match = OAL_FALSE;
    uint8_t idx;
    for (idx = 0; idx < sizeof(nav_duration) / sizeof(uint32_t); idx++) {
        if (vsp_param->nav_duration == nav_duration[idx]) {
            nav_match = OAL_TRUE;
            break;
        }
    }
    for (idx = 0; idx < sizeof(edca_win) / sizeof(uint32_t); idx++) {
        if (vsp_param->edca_win == edca_win[idx]) {
            edca_match = OAL_TRUE;
            break;
        }
    }
    return nav_match == OAL_TRUE && edca_match == OAL_TRUE;
}

uint32_t hmac_package_vsp_params(
    mac_vap_stru *mac_vap, uint32_t *params, uint8_t *new_params, uint16_t *new_params_len)
{
    vsp_nav_edca_stru *vsp_param = NULL;
    hmac_vsp_source_stru *source = &g_vsp_source;
    hmac_vsp_nav_edca_cmd *cmd_param = (hmac_vsp_nav_edca_cmd *)params;

    if (!source->vsp_base.enable) {
        oam_error_log0(0, 0, "{hmac_package_vsp_params::vsp is not enable!}");
        return OAL_FAIL;
    }
    vsp_param = (vsp_nav_edca_stru *)new_params;

    vsp_param->user_id = source->vsp_base.hmac_user->st_user_base_info.us_assoc_id;
    vsp_param->vsp_nav_edca.cfg_switch = cmd_param->cfg_switch;
    vsp_param->vsp_nav_edca.nav_duration = cmd_param->nav_duration;
    vsp_param->vsp_nav_edca.edca_win = cmd_param->edca_win;
    vsp_param->vsp_nav_edca.self_cts_switch = cmd_param->self_cts_switch;
    if (hmac_check_vsp_params(&vsp_param->vsp_nav_edca) == OAL_FALSE) {
        oam_error_log2(0, 0, "{hmac_package_vsp_params::params check fail,nav_duration[%d], edca_win[0x%x]!}",
            vsp_param->vsp_nav_edca.nav_duration, vsp_param->vsp_nav_edca.edca_win);
        return OAL_FAIL;
    }

    *new_params_len = sizeof(vsp_nav_edca_stru);
    hmac_vsp_set_txop_limit(mac_vap, OAL_FALSE);
    return OAL_SUCC;
}

uint32_t hmac_vsp_set_nav_edca_param(mac_vap_stru *mac_vap, hmac_vsp_nav_edca_cmd *cmd)
{
    vsp_nav_edca_stru vsp_param = { 0 };
    uint16_t params_len = 0;
    uint32_t ret;
    if (hmac_package_vsp_params(mac_vap, (uint32_t *)cmd, (uint8_t *)&vsp_param, &params_len) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_VSP_NAV_EDCA, params_len, (uint8_t *)&vsp_param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{hmac_vsp_set_nav_edca_param::hmac_config_send_event fail[%d].", ret);
        return ret;
    }
    return ret;
}
#endif
