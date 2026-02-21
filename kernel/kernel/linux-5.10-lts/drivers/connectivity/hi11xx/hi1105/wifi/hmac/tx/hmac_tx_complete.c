/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Host发送完成模块
 * 作    者 : wifi
 * 创建日期 : 2020年4月23日
 */

#include "hmac_tx_complete.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_host_tx_data.h"
#include "hmac_soft_retry.h"
#include "mac_common.h"
#include "host_hal_device.h"
#include "host_hal_ring.h"
#include "host_hal_ext_if.h"
#include "pcie_host.h"
#include "wlan_spec.h"
#include "host_hal_ops.h"
#include "hmac_vsp_if.h"
#include "hmac_vsp_source.h"
#include "hmac_tid_sche.h"
#include "hmac_tx_switch.h"
#include "hmac_tx_ring_alloc.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TX_COMPLETE_C

static inline hmac_tid_info_stru *hmac_tx_comp_tid_info_getter(void *entry)
{
    return oal_container_of(entry, hmac_tid_info_stru, tid_tx_comp_entry);
}

static inline void *hmac_tx_comp_entry_getter(hmac_tid_info_stru *tid_info)
{
    return &tid_info->tid_tx_comp_entry;
}
const hmac_tid_list_ops g_tid_tx_comp_list_ops = {
    .tid_info_getter = hmac_tx_comp_tid_info_getter,
    .entry_getter = hmac_tx_comp_entry_getter,
};

hmac_tx_comp_stru g_tx_comp_mgmt = { .tx_comp_thread = NULL};

void hmac_tx_comp_init(void)
{
    uint8_t hal_dev_id;
    uint8_t hal_dev_num = mac_chip_get_hal_dev_num();

    for (hal_dev_id = 0; hal_dev_id < hal_dev_num; hal_dev_id++) {
        hmac_clear_tx_comp_triggered(hal_dev_id);
    }

    hmac_tid_list_init(&g_tx_comp_mgmt.tid_tx_list, &g_tid_tx_comp_list_ops);
}

static uint32_t hmac_tx_complete_netbuf_retrans(hmac_tid_info_stru *tid_info, oal_netbuf_stru *netbuf)
{
    hal_tx_msdu_dscr_info_stru tx_msdu_dscr_info = { 0 };
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (tx_ctl == NULL || oal_likely(!mac_get_cb_is_needretry(tx_ctl))) {
        return OAL_FAIL;
    }

    hal_tx_msdu_dscr_info_get(netbuf, &tx_msdu_dscr_info);
    if (oal_likely(tx_msdu_dscr_info.tx_count < HAL_TX_MSDU_MAX_CNT)) {
        return OAL_FAIL;
    }

    if (hmac_tx_soft_retry_process(tid_info, netbuf) != OAL_SUCC) {
        return OAL_FAIL;
    }

    hmac_tx_msdu_update_free_cnt(&tid_info->tx_ring);

    return OAL_SUCC;
}

static void hmac_tx_complete_netbuf_process(
    hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_ring, un_rw_ptr release_ptr)
{
    oal_netbuf_stru *netbuf = hmac_tx_ring_get_netbuf(tx_ring, release_ptr);

    if (oal_unlikely(netbuf == NULL)) {
        oam_error_log3(0, 0, "{hmac_tx_complete_netbuf_process::user[%d] tid[%d] index[%d] netbuf null}",
            tid_info->user_index, tid_info->tid_no, release_ptr.rw_ptr);
        return;
    }

    /*
     * 先进行dma unmap操作, 无效msdu dscr(skb->data)的cache line, 保证软重传获取的数据包含MAC回填值
     * 此时skb->data不在cache中, 软重传访问skb->data会load cache
     */
    hmac_tx_ring_unmap_netbuf(tx_ring, netbuf, release_ptr.st_rw_ptr.bit_rw_ptr);
#ifdef _PRE_WLAN_FEATURE_TX_STAT
    tid_info->tx_stat.tx_comp_msdu_cnt++;
#endif

    /* 高流量(~2.8Gbps)不进行软重传, 避免load cache造成额外开销 */
    if (hmac_tid_ring_tx_opt(tid_info) || hmac_tx_complete_netbuf_retrans(tid_info, netbuf) != OAL_SUCC) {
        hmac_tx_ring_release_netbuf(tx_ring, netbuf, release_ptr.st_rw_ptr.bit_rw_ptr);
    }
}

/*
 * 功能描述 : 发送完成处理ring中所有msdu对应的netbuf, 并修改ring的release ptr
 * 1.日    期 : 2020年4月22日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
static void hmac_tx_complete_msdu_info_process(hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_ring)
{
    un_rw_ptr release_ptr = { .rw_ptr = tx_ring->release_index };
    un_rw_ptr target_ptr = { .rw_ptr = tx_ring->base_ring_info.read_index };

    while (hal_tx_rw_ptr_compare(target_ptr, release_ptr) == RING_PTR_GREATER) {
        hmac_tx_complete_netbuf_process(tid_info, tx_ring, release_ptr);

        host_cnt_inc_record_performance(TX_BH_PROC);

        hmac_tx_reset_msdu_info(tx_ring, release_ptr);
        hmac_tx_msdu_ring_inc_release_ptr(tx_ring);

        release_ptr.rw_ptr = tx_ring->release_index;
    }
}

/*
 * 功能描述 : 发送完成处理ring中的msdu
 * 1.日    期 : 2020年2月12日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 * 2.日    期 : 2020年4月22日
 *   作    者 : wifi
 *   修改内容 : 重构函数
 */
void hmac_tx_complete_tid_ring_process(hmac_user_stru *hmac_user, hmac_tid_info_stru *tid_info)
{
    hmac_msdu_info_ring_stru *tx_ring = &tid_info->tx_ring;
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
    uint16_t last_release_ptr = tx_ring->release_index;

    if (oal_unlikely(hmac_vap == NULL)) {
        return;
    }

    if (oal_unlikely(hmac_tx_ring_rw_ptr_check(tx_ring) != OAL_TRUE)) {
        /* 进入该流程合理:场景tx complete->tx pps switch->tx complete时会进入 */
        oam_warning_log4(0, OAM_SF_TX, "{hmac_tx_complete_tid_ring_process::tid[%d] release[%d] rptr[%d], wptr[%d]}",
            tid_info->tid_no, tx_ring->release_index, tx_ring->base_ring_info.read_index,
            tx_ring->base_ring_info.write_index);
        return;
    }

    hmac_tx_complete_msdu_info_process(tid_info, tx_ring);
    hmac_host_ring_tx_resume(hmac_vap, hmac_user, tid_info);

    /*
     * 本次发送完成处理释放过帧才判断是否切换ring, 否则可能存在如下时序(假设ring中有10个帧):
     * 1. 第一次发送完成中断上半部, 此时发帧数5/10
     * 2. 第二次发送完成中断上半部, 此时发帧数10/10
     * 3. 第一次发送完成中断下半部, 读到的read指针为10, netbuf全部释放, ring空, 下发ring切换事件
     * 4. 第二次发送完成中断下半部, 读到的read指针为10, 无netbuf释放, ring空, 下发ring切换事件
     * 导致ring切换事件重复下发
     */
    if (last_release_ptr != tx_ring->release_index) {
        hmac_tx_mode_h2d_switch_process(tid_info);
    }
}

OAL_STATIC void hmac_tx_comp_update_ml_bitmap(hmac_tid_info_stru *tid_info, uint32_t ml1_rptr)
{
    uint32_t ml_bitmap;
    /* 更新软件存储的ml bitmap */
    if (tid_info->tx_ring.ml_ring_info.multi_link_bitmap == HAL_MULTILINK_TX_BITMAP_LINK0 && ml1_rptr != 0) {
        hmac_tx_reg_read_ml_bitmap(tid_info, &ml_bitmap);
        tid_info->tx_ring.ml_ring_info.multi_link_bitmap = ml_bitmap;
    } else if (tid_info->tx_ring.ml_ring_info.multi_link_bitmap == HAL_MULTILINK_TX_BITMAP_ALL_LINK && ml1_rptr == 0) {
        hmac_tx_reg_read_ml_bitmap(tid_info, &ml_bitmap);
        tid_info->tx_ring.ml_ring_info.multi_link_bitmap = ml_bitmap;
    }
}

uint32_t hmac_tx_complete_update_mlink_ring_rptr(hmac_tid_info_stru *tid_info)
{
    uint32_t rptr, ml1_rptr;

    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return OAL_FAIL;
    }

    if (hmac_tx_reg_read_rptr(tid_info, &rptr) != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        oam_error_log2(0, 0, "{hmac_tx_complete_update_mlink_ring_rptr::read fail, user[%d] tid[%d]}",
            tid_info->lut_index, tid_info->tid_no);
        return OAL_FAIL;
    }
    if (hmac_tx_reg_read_ml1_rptr(tid_info, &ml1_rptr) != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        oam_error_log2(0, 0, "{hmac_tx_complete_update_mlink_ring_rptr::read ml1 fail, user[%d] tid[%d]}",
            tid_info->lut_index, tid_info->tid_no);
        return OAL_FAIL;
    }
    hmac_tx_comp_update_ml_bitmap(tid_info, ml1_rptr);

    hal_pm_try_wakeup_allow_sleep();
    if (oal_unlikely(!hal_tx_complete_ring_rptr_valid(tid_info->tx_ring.base_ring_info.size, rptr) ||
        !hal_tx_complete_ring_rptr_valid(tid_info->tx_ring.base_ring_info.size, ml1_rptr))) {
        oam_warning_log3(0, 0, "{hmac_tx_complete_update_mlink_ring_rptr::user[%d] tid[%d] invalid read_index[%d]}",
            tid_info->user_index, tid_info->tid_no, rptr);
        return OAL_FAIL;
    }

    if (tid_info->tx_ring.ml_ring_info.multi_link_bitmap == HAL_MULTILINK_TX_BITMAP_LINK0) {
        tid_info->tx_ring.base_ring_info.read_index = (uint16_t)rptr;
    } else if (tid_info->tx_ring.ml_ring_info.multi_link_bitmap == HAL_MULTILINK_TX_BITMAP_LINK1) {
        tid_info->tx_ring.base_ring_info.read_index = (uint16_t)ml1_rptr;
    } else if (tid_info->tx_ring.ml_ring_info.multi_link_bitmap == HAL_MULTILINK_TX_BITMAP_ALL_LINK) {
        tid_info->tx_ring.base_ring_info.read_index = hal_tx_rw_smaller_ptr((uint16_t)rptr, (uint16_t)ml1_rptr);
    }

    return OAL_SUCC;
}

uint32_t hmac_tx_complete_update_tid_ring_rptr(hmac_tid_info_stru *tid_info)
{
    uint32_t rptr_val;

    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return OAL_FAIL;
    }

    if (hmac_tx_reg_read_rptr(tid_info, &rptr_val) != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return OAL_FAIL;
    }

    hal_pm_try_wakeup_allow_sleep();
    if (oal_unlikely(!hal_tx_complete_ring_rptr_valid(tid_info->tx_ring.base_ring_info.size, rptr_val))) {
        oam_warning_log3(0, 0, "{hmac_tx_complete_update_tid_ring_rptr::user[%d] tid[%d] invalid read_index[%d]}",
            tid_info->user_index, tid_info->tid_no, rptr_val);
        return OAL_FAIL;
    }
    tid_info->tx_ring.base_ring_info.read_index = (uint16_t)rptr_val;

    return OAL_SUCC;
}

uint32_t hmac_tx_complete_update_ring_rptr(hmac_tid_info_stru *tid_info)
{
    if (g_wlan_spec_cfg->multi_link == 0) {
        return hmac_tx_complete_update_tid_ring_rptr(tid_info);
    } else {
        return hmac_tx_complete_update_mlink_ring_rptr(tid_info);
    }
}

#define TX_BA_INFO_LOOP
/* 方案2：发送完成后，foreach循环查找有wptr更新的TID INFO RING，不依赖于TX BA INFO */
static void hmac_tx_complete_ba_info_process_nolock(hmac_user_stru *hmac_user, hmac_tid_info_stru *tid_info)
{
    if (oal_unlikely(!hmac_tid_is_host_ring_tx(tid_info) || !hmac_tx_ring_alloced(&tid_info->tx_ring))) {
        return;
    }

    oal_smp_mb();

    if (oal_unlikely(hmac_tx_complete_update_ring_rptr(tid_info) != OAL_SUCC)) {
        return;
    }
#ifdef _PRE_WLAN_FEATURE_VSP
    if (hmac_is_vsp_source_tid(tid_info)) {
        hmac_vsp_tx_complete_process();
        return;
    }
#endif
    hmac_tx_complete_tid_ring_process(hmac_user, tid_info);

    if (hmac_tid_sche_allowed(tid_info) && oal_likely(!hmac_tx_ring_switching(tid_info))) {
        hmac_tid_schedule();
    }
}

static void hmac_tx_complete_ba_info_process(hmac_tx_comp_info_stru *tx_comp_info, uint8_t tx_comp_info_cnt)
{
    uint8_t idx;
    hmac_tid_info_stru *tid_info = NULL;

    for (idx = 0; idx < tx_comp_info_cnt; idx++) {
        tid_info = hmac_get_tx_tid_info(tx_comp_info[idx].hmac_user, tx_comp_info[idx].tid_no);
        if (tid_info == NULL) {
            continue;
        }
        mutex_lock(&tid_info->tx_ring.tx_comp_lock);
        hmac_tx_complete_ba_info_process_nolock(tx_comp_info[idx].hmac_user, tid_info);
        mutex_unlock(&tid_info->tx_ring.tx_comp_lock);
    }
}

/*
 * 函 数 名 : hmac_tx_get_ba_info_count
 * 功能描述 : 获取ba info ring中待处理的info数量
 * 1.日    期 : 2020年2月20日
 *   修改内容 : 新生成函数
 */
static uint16_t hmac_tx_get_ba_info_count(hal_host_device_stru *hal_dev)
{
    uint16_t ba_info_count = 0;
    hal_host_ring_ctl_stru *ba_info_ring = NULL;

    if (hal_dev == NULL) {
        return ba_info_count;
    }

    ba_info_ring = &hal_dev->host_ba_info_ring;
    if (hal_ring_get_entry_count(hal_dev, ba_info_ring, &ba_info_count) != OAL_SUCC) {
        return (hal_host_ba_ring_depth_get() + 1);
    }

    return ba_info_count;
}

/*
 * 函 数 名 : hmac_tx_get_ba_info
 * 功能描述 : 获取ba info ring中待处理的ba info
 * 1.日    期 : 2020年2月20日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_tx_get_ba_info(
    hal_host_device_stru *hal_device, uint16_t ba_info_cnt, oal_dlist_head_stru *ba_info_list)
{
    uint32_t index;
    uint8_t ba_info_data[BA_INFO_DATA_SIZE];
    hal_tx_ba_info_stru *ba_info = NULL;
    hal_host_ring_ctl_stru *ba_info_ring = NULL;

    if (hal_device == NULL) {
        return;
    }
    ba_info_ring = &hal_device->host_ba_info_ring;
    for (index = 0; index < ba_info_cnt; index++) {
        if (oal_unlikely(hal_ring_get_entries(ba_info_ring, ba_info_data, 1) != OAL_SUCC)) {
            oam_error_log3(0, OAM_SF_TX, "{hmac_tx_get_ba_info:: count[%d] read[%d] write[%d]}",
                ba_info_cnt, ba_info_ring->un_read_ptr.read_ptr, ba_info_ring->un_write_ptr.write_ptr);
            return;
        }

        ba_info = (hal_tx_ba_info_stru *)oal_memalloc(sizeof(hal_tx_ba_info_stru));
        if (oal_unlikely(ba_info == NULL)) {
            oam_error_log0(0, OAM_SF_TX, "{hmac_tx_get_ba_info::alloc ba_info failed}");
            break;
        }

        hal_tx_ba_info_dscr_get(ba_info_data, BA_INFO_DATA_SIZE, ba_info);
        oal_dlist_add_tail(&ba_info->entry, ba_info_list);
    }

    /* 尽快更新ba info ring rptr, 避免ring满, 后续再进行其他ba info处理 */
    hal_ring_set_sw2hw(ba_info_ring);

    return;
}

static inline oal_bool_enum_uint8 hmac_tx_complete_ba_info_valid(hal_tx_ba_info_stru *ba_info)
{
    return ba_info->ba_info_vld && ba_info->tid_no < WLAN_TIDNO_BUTT && ba_info->user_id < HAL_MAX_TX_BA_LUT_SIZE;
}

static void hmac_tx_complete_record_ba_info(
    hmac_user_stru *hmac_user, hal_tx_ba_info_stru *ba_info, hmac_tx_comp_info_stru *tx_comp_info, uint8_t *info_cnt)
{
    uint8_t idx;

    for (idx = 0; idx < *info_cnt; idx++) {
        if (hmac_user->st_user_base_info.us_assoc_id == tx_comp_info[idx].hmac_user->st_user_base_info.us_assoc_id &&
            ba_info->tid_no == tx_comp_info[idx].tid_no) {
            return;
        }
    }

    tx_comp_info[*info_cnt].hmac_user = hmac_user;
    tx_comp_info[*info_cnt].tid_no = ba_info->tid_no;
    (*info_cnt)++;
}

static void hmac_tx_complete_process_ba_info(
    hal_tx_ba_info_stru *ba_info, hmac_tx_comp_info_stru *tx_comp_info, uint8_t *tx_comp_info_cnt)
{
    hmac_user_stru *hmac_user = NULL;

    if (oal_unlikely(!hmac_tx_complete_ba_info_valid(ba_info))) {
        return;
    }

    hmac_user = hmac_get_user_by_lut_index(ba_info->user_id);
    if (oal_unlikely(hmac_user == NULL)) {
        return;
    }

    /* 记录待处理user tid, 筛选掉重复的user tid ba info, 避免反复处理 */
    hmac_tx_complete_record_ba_info(hmac_user, ba_info, tx_comp_info, tx_comp_info_cnt);
}

#define HMAC_BA_INFO_COUNT 64
/*
 * 函 数 名  : hmac_tx_complete_process_ba_info_list
 * 功能描述  : 解析每个TID的tx ba ring info信息并做host ring skb释放工作
 * 1.日    期  : 2020年2月24日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_tx_complete_process_ba_info_list(oal_dlist_head_stru *ba_info_list)
{
    uint8_t tx_comp_info_cnt = 0;
    hmac_tx_comp_info_stru tx_comp_info[HMAC_BA_INFO_COUNT];
    hal_tx_ba_info_stru *ba_info = NULL;
    oal_dlist_head_stru *entry = NULL;
    oal_dlist_head_stru *entry_tmp = NULL;

    oal_dlist_search_for_each_safe(entry, entry_tmp, ba_info_list) {
        ba_info = (hal_tx_ba_info_stru *)oal_dlist_get_entry(entry, hal_tx_ba_info_stru, entry);

        if (oal_likely(tx_comp_info_cnt < HMAC_BA_INFO_COUNT)) {
            hmac_tx_complete_process_ba_info(ba_info, tx_comp_info, &tx_comp_info_cnt);
        }

        oal_dlist_delete_entry(&ba_info->entry);
        oal_free(ba_info);
    }

    hmac_tx_complete_ba_info_process(tx_comp_info, tx_comp_info_cnt);
}

static uint32_t hmac_tx_complete_get_ba_info(hal_host_device_stru *hal_device, oal_dlist_head_stru *ba_info_list)
{
    uint16_t ba_info_cnt = hmac_tx_get_ba_info_count(hal_device);
    if (oal_unlikely(ba_info_cnt == 0 || ba_info_cnt >= (hal_host_ba_ring_depth_get() + 1))) {
        return OAL_FAIL;
    }

    hmac_tx_get_ba_info(hal_device, ba_info_cnt, ba_info_list);

    return OAL_SUCC;
}

void hmac_tx_complete_process_by_ba_info(hal_host_device_stru *hal_device)
{
    oal_dlist_head_stru ba_info_list;
    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return;
    }

    oal_dlist_init_head(&ba_info_list);
    if (hmac_tx_complete_get_ba_info(hal_device, &ba_info_list) == OAL_SUCC) {
        hmac_tx_complete_process_ba_info_list(&ba_info_list);
    }
    hal_pm_try_wakeup_allow_sleep();
}

#define TX_TID_INFO_LOOP
/* 方案2：发送完成后，foreach循环查找有wptr更新的TID INFO RING，不依赖于TX BA INFO */
static void hmac_tx_complete_process_tid_tx_list_nolock(hmac_tid_info_stru *tid_info)
{
    hmac_user_stru *hmac_user = mac_res_get_hmac_user(tid_info->user_index); /* user id 被修改后，确保切之前的发送完成 能处理完 */

    if (hmac_user == NULL) {
        return;
    }

    if (oal_unlikely(!hmac_tid_is_host_ring_tx(tid_info) || !hmac_tx_ring_alloced(&tid_info->tx_ring))) {
        return;
    }

    oal_smp_mb();

    if (oal_unlikely(hmac_tx_complete_update_ring_rptr(tid_info) != OAL_SUCC)) {
        return;
    }
#ifdef _PRE_WLAN_FEATURE_VSP
    if (hmac_is_vsp_source_tid(tid_info)) {
        hmac_vsp_tx_complete_process();
        return;
    }
#endif
    hmac_tx_complete_tid_ring_process(hmac_user, tid_info);

    if (hmac_tid_sche_allowed(tid_info) && oal_likely(!hmac_tx_ring_switching(tid_info))) {
        hmac_tid_schedule();
    }
}

OAL_STATIC uint8_t hmac_tx_complete_process_tid_tx_list(hmac_tid_info_stru *tid_info, void *param)
{
    mutex_lock(&tid_info->tx_ring.tx_comp_lock);
    hmac_tx_complete_process_tid_tx_list_nolock(tid_info);
    mutex_unlock(&tid_info->tx_ring.tx_comp_lock);

    return OAL_CONTINUE;
}
static uint32_t hmac_tx_complete_tid_tx_list_enqueue(hmac_tid_info_stru *tid_info, void *param)
{
    return hmac_tid_list_enqueue_nolock((hmac_tid_list_stru *)param, tid_info);
}
uint8_t hmac_tx_complete_get_tx_tid_info(hmac_tid_info_stru *tid_info, void *param)
{
    if (oal_unlikely(tid_info == NULL)) {
        oam_error_log0(0, 0, "{hmac_tx_complete_get_tx_tid_info::tid_info NULL}");
        return OAL_RETURN;
    }

    if (oal_atomic_read(&tid_info->tx_ring.inited) != OAL_TRUE) {
        return OAL_CONTINUE;
    }

    hmac_tx_complete_tid_tx_list_enqueue(tid_info, param);

    return OAL_CONTINUE;
}

static inline uint8_t hmac_tx_complete_tid_tx_list_for_each(hmac_tid_list_func func, void *param)
{
    return hmac_tid_list_for_each_nolock((hmac_tid_list_stru *)param, func, NULL);
}

void hmac_tx_complete_process_by_tid_info(hal_host_device_stru *hal_device)
{
    hmac_tx_comp_stru *tx_comp = hmac_get_tx_comp_mgmt();

    hmac_tid_schedule_list_for_each(hmac_tx_complete_get_tx_tid_info, &tx_comp->tid_tx_list);
    hmac_tx_complete_tid_tx_list_for_each(hmac_tx_complete_process_tid_tx_list, &tx_comp->tid_tx_list);
    hmac_tid_list_clear_nolock(&tx_comp->tid_tx_list);
}

void hmac_tx_complete_process(hal_host_device_stru *hal_device)
{
    if (g_wlan_spec_cfg->tx_ba_info == 0) {
        hmac_tx_complete_process_by_tid_info(hal_device);
    } else {
        hmac_tx_complete_process_by_ba_info(hal_device);
    }
}
OAL_STATIC void hmac_tx_comp_thread_process(void)
{
    uint8_t hal_dev_id;
    uint8_t hal_dev_num = mac_chip_get_hal_dev_num();

    host_start_record_performance(TX_BH_PROC);

    for (hal_dev_id = 0; hal_dev_id < hal_dev_num; hal_dev_id++) {
        if (!hmac_get_tx_comp_triggered(hal_dev_id)) {
            continue;
        }

        hmac_clear_tx_comp_triggered(hal_dev_id);

        if (!oal_pcie_link_state_up(HCC_EP_WIFI_DEV)) {
            oam_warning_log0(0, OAM_SF_TX, "{hmac_hal_dev_tx_complete_process::pcie link down!}");
            continue;
        }

        hmac_tx_complete_process(hal_get_host_device(hal_dev_id));
    }

    host_end_record_performance(host_cnt_get_record_performance(TX_BH_PROC), TX_BH_PROC);
}

int32_t hmac_tx_comp_thread(void *p_data)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct sched_param param;

    param.sched_priority = 97; /* set priority: 97 */
    oal_set_thread_property(current, SCHED_FIFO, &param, -10); /* set nice -10 */

    allow_signal(SIGTERM);

    while (oal_likely(!down_interruptible(&g_tx_comp_mgmt.tx_comp_sema))) {
        if (oal_kthread_should_stop()) {
            break;
        }

        hmac_tx_comp_thread_process();
    }
#endif

    return OAL_SUCC;
}
