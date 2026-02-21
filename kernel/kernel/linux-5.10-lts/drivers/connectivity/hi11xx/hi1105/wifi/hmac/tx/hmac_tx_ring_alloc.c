/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Hmac tx ring内存分配模块
 * 作    者 : wifi
 * 创建日期 : 2021年12月23日
 */

#include "hmac_tx_ring_alloc.h"
#include "hmac_tid_sche.h"
#include "hmac_tid_update.h"
#include "hmac_tid_ring_switch.h"
#include "hmac_host_tx_data.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_config.h"
#include "pcie_linux.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TX_RING_ALLOC_C

static inline uint8_t hmac_get_ring_tx_mode(void)
{
    return hmac_host_ring_tx_enabled() ? HOST_RING_TX_MODE : DEVICE_RING_TX_MODE;
}

static void hmac_tid_info_init(hmac_tid_info_stru *tid_info, hmac_user_stru *hmac_user, uint8_t tid)
{
    memset_s(&tid_info->tx_ring, sizeof(hmac_msdu_info_ring_stru), 0, sizeof(hmac_msdu_info_ring_stru));

    tid_info->tid_no = tid;
    tid_info->user_index = hmac_user->st_user_base_info.us_assoc_id;
    tid_info->lut_index = hal_host_get_tx_ring_lut_idx(hmac_user->lut_index, hmac_user->ml_user_lut_idx);
    tid_info->is_in_ring_list = OAL_FALSE;
    oal_atomic_set(&tid_info->ring_tx_mode, hmac_get_ring_tx_mode());
    oal_atomic_set(&tid_info->tid_sche_th, 1);
    oal_netbuf_head_init(&tid_info->tid_queue);
    mutex_init(&tid_info->tx_ring.tx_lock);
    mutex_init(&tid_info->tx_ring.tx_comp_lock);
    oal_spin_lock_init(&tid_info->lock);

    if (hmac_tid_schedule_list_enqueue(tid_info) != OAL_SUCC) {
        oam_error_log2(0, 0, "{hmac_tid_info_init::user[%d] tid[%d] enqueue failed!}", tid_info->user_index, tid);
    }

    oal_atomic_set(&tid_info->inited, OAL_TRUE);
}

OAL_STATIC uint8_t hmac_tx_ring_get_mld_bitmap(hmac_user_stru *hmac_user)
{
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (hmac_vap == NULL) {
        return HAL_MULTILINK_TX_BITMAP_LINK0;
    }
#ifdef _PRE_WLAN_FEATURE_ML
    if (!hmac_vap_is_legacy_mld_mode(hmac_vap) &&
        hmac_vap->mld_vap_link_info.mld_vap_manage != NULL &&
        hmac_vap->mld_vap_link_info.mld_vap_manage->mld_assoc_mode == MLD_ASSOC_MODE_STR) {
        return HAL_MULTILINK_TX_BITMAP_ALL_LINK;
    }
#endif
    return HAL_MULTILINK_TX_BITMAP_LINK0;
}

void hmac_tx_ring_set_mld_bitmap(hmac_user_stru *hmac_user, uint8_t bitmap)
{
    uint32_t tid;
    hmac_msdu_info_ring_stru *tx_ring = NULL;
    msdu_info_ring_ml_stru *ml_info = NULL;
    hmac_tid_info_stru *tid_info = NULL;

    if (hmac_user == NULL) {
        oam_error_log1(0, OAM_SF_TX,
            "hmac_tx_ring_set_mld_bitmap::hmac_user is null. bitmap 0x%x", bitmap);
        return;
    }
    if (hmac_ring_tx_enabled() != OAL_TRUE) {
        return;
    }
    for (tid = 0; tid < WLAN_TID_MAX_NUM; tid++) {
        tid_info = hmac_get_tx_tid_info(hmac_user, tid);
        if (tid_info == NULL) {
            continue;
        }
        tx_ring = &tid_info->tx_ring;
        ml_info = &tx_ring->ml_ring_info;
        if (tid == WLAN_TIDNO_BCAST) {
            return;
        }
        ml_info->multi_link_bitmap = bitmap;
    }
}

static void hmac_host_tx_ring_ml_init(hmac_user_stru *hmac_user, hmac_msdu_info_ring_stru *tx_ring)
{
    msdu_info_ring_ml_stru *ml_info = &tx_ring->ml_ring_info;

#ifdef _PRE_WLAN_FEATURE_ML
    ml_info->ml_mode = 0;
    ml_info->ml0_dup_en = 0;
    ml_info->ml1_dup_en = 0;
#endif
    ml_info->multi_link_bitmap = hmac_tx_ring_get_mld_bitmap(hmac_user);
}

#define HMAC_TX_RING_MAX_AMSDU_NUM 7
static uint32_t hmac_host_tx_ring_init(hmac_user_stru *hmac_user, hmac_tid_info_stru *tid_info, uint8_t tid)
{
    hmac_msdu_info_ring_stru *tx_ring = &tid_info->tx_ring;

    if (!hmac_host_ring_tx_enabled()) {
        return OAL_SUCC;
    }

    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return OAL_FAIL;
    }

    if (hal_host_tx_init_tid_ring_table(tid_info->lut_index, tid, &tx_ring->tx_ring_table_addr) != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        oam_error_log0(0, OAM_SF_CFG, "{hmac_host_tx_ring_init::tx ring device info init failed!}");
        return OAL_FAIL;
    }
    hal_pm_try_wakeup_allow_sleep();

    tx_ring->base_ring_info.size = hal_host_tx_tid_ring_size_get(HAL_TX_RING_SIZE_GRADE_DOWNGRADE_TWICE, tid);
    if (tx_ring->base_ring_info.size == HAL_HOST_USER_TID_TX_RING_SIZE_ERROR) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_host_tx_ring_init::tx ring size is error!}");
        return OAL_FAIL;
    }

    tx_ring->base_ring_info.read_index = 0;
    tx_ring->base_ring_info.write_index = 0;
    tx_ring->base_ring_info.max_amsdu_num = (tid != WLAN_TIDNO_BCAST) ? HMAC_TX_RING_MAX_AMSDU_NUM : 0;
    tx_ring->tx_msdu_cnt = 0;
    tx_ring->ring_sync_cnt = 0;

    hmac_host_tx_ring_ml_init(hmac_user, tx_ring);
    oal_atomic_set(&tx_ring->msdu_cnt, 0);
    oal_atomic_set(&tx_ring->last_period_tx_msdu, 0);
    oal_atomic_set(&tx_ring->ring_sync_th, 1);
    oal_atomic_set(&tx_ring->ring_tx_opt, OAL_FALSE);

    oal_atomic_set(&tx_ring->inited, OAL_TRUE);

    return OAL_SUCC;
}

static uint32_t hmac_alloc_netbuf_list(hmac_msdu_info_ring_stru *tx_ring, uint16_t host_ring_size)
{
    uint32_t host_netbuf_buf_size = host_ring_size * sizeof(oal_netbuf_stru *);

    tx_ring->netbuf_list = (oal_netbuf_stru **)vmalloc(host_netbuf_buf_size);
    if (tx_ring->netbuf_list == NULL) {
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    memset_s(tx_ring->netbuf_list, host_netbuf_buf_size, 0, host_netbuf_buf_size);

    return OAL_SUCC;
}

uint32_t hmac_alloc_tx_ring(hmac_msdu_info_ring_stru *tx_ring)
{
    uint16_t host_ring_size = hal_host_tx_tid_ring_depth_get(tx_ring->base_ring_info.size);

    if (hal_host_alloc_tx_ring(&tx_ring->ring_buf, host_ring_size) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_alloc_tx_ring::dma alloc ring buf failed!}");
        return OAL_FAIL;
    }

    if (hmac_alloc_netbuf_list(tx_ring, host_ring_size) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_alloc_tx_ring::kmalloc netbuf list failed!}");
        hal_host_release_tx_ring(&tx_ring->ring_buf);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/* 多线程可能导致tx ring deinit后仍然alloc tx ring，检查inited为false时要释放tx ring */
static uint32_t hmac_tx_ring_deinit_check(hmac_msdu_info_ring_stru *tx_ring)
{
    if (oal_atomic_read(&tx_ring->inited) == OAL_TRUE) {
        return OAL_SUCC;
    }

    hmac_tx_host_ring_release_nolock(tx_ring);
    return OAL_FAIL;
}

uint32_t hmac_alloc_tx_ring_by_tid(hmac_msdu_info_ring_stru *tx_ring, uint8_t tid_no)
{
    uint8_t ring_size_grade;

    // tx ring内存申请梯度&申请失败异常处理
    for (ring_size_grade = HAL_TX_RING_SIZE_GRADE_DEFAULT; ring_size_grade < HAL_TX_RING_SIZE_GRADE_BUTT;
         ring_size_grade++) {
        tx_ring->base_ring_info.size = hal_host_tx_tid_ring_size_get(ring_size_grade, tid_no);
        if (tx_ring->base_ring_info.size == HAL_HOST_USER_TID_TX_RING_SIZE_ERROR) {
            return OAL_FAIL;
        }
        if (hmac_alloc_tx_ring(tx_ring) == OAL_SUCC) {
            return hmac_tx_ring_deinit_check(tx_ring);
        }
    }

    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                         CHR_WIFI_DRV_EVENT_RING_TX, CHR_WIFI_DRV_ERROR_TX_RING_ALLOC);
    oam_error_log1(0, OAM_SF_TX, "{hmac_alloc_tx_ring::tx ring alloc fail([%d]times)!}", HAL_TX_RING_SIZE_GRADE_BUTT);
    return OAL_FAIL;
}

static void hmac_reset_tx_ring(hmac_msdu_info_ring_stru *tx_ring)
{
    memset_s(&tx_ring->base_ring_info, sizeof(msdu_info_ring_stru), 0, sizeof(msdu_info_ring_stru));
    memset_s(&tx_ring->ring_buf, sizeof(hal_host_tx_msdu_ring_stru), 0, sizeof(hal_host_tx_msdu_ring_stru));
    tx_ring->release_index = 0;
    oal_atomic_set(&tx_ring->msdu_cnt, 0);
    if (tx_ring->netbuf_list != NULL) {
        vfree(tx_ring->netbuf_list);
        tx_ring->netbuf_list = NULL;
    }
}

uint32_t hmac_ring_tx_init(hmac_user_stru *hmac_user, uint8_t tid)
{
    hmac_tid_info_stru *tid_info = hmac_get_tx_tid_info(hmac_user, tid);
    if (tid_info == NULL) {
        return OAL_FAIL;
    }
    if ((hmac_ring_tx_enabled() != OAL_TRUE) || tid == WLAN_TIDNO_BCAST) {
        return OAL_SUCC;
    }

    hmac_tid_info_init(tid_info, hmac_user, tid);

    if (hmac_host_tx_ring_init(hmac_user, tid_info, tid) != OAL_SUCC) {
        hmac_tid_info_deinit(tid_info);
        oam_error_log2(0, 0, "{hmac_ring_tx_init::user[%d] tid[%d] host ring init failed}", tid_info->user_index, tid);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_RING_TX, CHR_WIFI_DRV_ERROR_TX_RING_INIT);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

uint32_t hmac_alloc_tx_ring_after_inited(hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_ring)
{
    if (oal_atomic_read(&tid_info->tx_ring.inited) != OAL_TRUE) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_alloc_tx_ring::tx ring not inited!}");
        return OAL_FAIL;
    }

    mutex_lock(&tid_info->tx_ring.tx_comp_lock);
    if (hmac_alloc_tx_ring_by_tid(tx_ring, tid_info->tid_no) != OAL_SUCC) {
        hmac_reset_tx_ring(tx_ring);
        mutex_unlock(&tid_info->tx_ring.tx_comp_lock);
        return OAL_FAIL;
    }

    hmac_tx_reg_write_ring_info(tid_info, TID_CMDID_CREATE);
    mutex_unlock(&tid_info->tx_ring.tx_comp_lock);
    oam_warning_log2(0, OAM_SF_CFG, "{hmac_alloc_tx_ring::tid update list enqueue, user[%d] tid[%d]}",
        tid_info->user_index, tid_info->tid_no);
    hmac_tid_update_list_enqueue(tid_info);

    return OAL_SUCC;
}

void hmac_tx_ring_release_all_netbuf(hmac_msdu_info_ring_stru *tx_ring)
{
    un_rw_ptr release_ptr = { .rw_ptr = tx_ring->release_index };
    un_rw_ptr target_ptr = { .rw_ptr = tx_ring->base_ring_info.write_index };
    oal_netbuf_stru *netbuf = NULL;

    while (hal_tx_rw_ptr_compare(target_ptr, release_ptr) == RING_PTR_GREATER) {
        netbuf = hmac_tx_ring_get_netbuf(tx_ring, release_ptr);
        if (netbuf != NULL) {
            hmac_tx_ring_unmap_netbuf(tx_ring, netbuf, release_ptr.st_rw_ptr.bit_rw_ptr);
            hmac_tx_ring_release_netbuf(tx_ring, netbuf, release_ptr.st_rw_ptr.bit_rw_ptr);
        }

        hmac_tx_reset_msdu_info(tx_ring, release_ptr);
        hmac_tx_msdu_ring_inc_release_ptr(tx_ring);

        release_ptr.rw_ptr = tx_ring->release_index;
    }
}

static inline void hmac_tx_ring_release_netbuf_record(hmac_msdu_info_ring_stru *tx_ring)
{
    if (tx_ring->netbuf_list == NULL) {
        return;
    }

    vfree(tx_ring->netbuf_list);
    tx_ring->netbuf_list = NULL;
}

void hmac_tx_host_ring_release_nolock(hmac_msdu_info_ring_stru *tx_ring)
{
    if (!hmac_tx_ring_alloced(tx_ring)) {
        return;
    }

    hmac_tx_ring_release_all_netbuf(tx_ring);
    hmac_tx_ring_release_netbuf_record(tx_ring);

    if (hal_host_release_tx_ring(&tx_ring->ring_buf) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_host_ring_release::host buf release failed}");
    }

    hmac_reset_tx_ring(tx_ring);
}

void hmac_tx_host_ring_release(hmac_msdu_info_ring_stru *tx_ring)
{
    mutex_lock(&tx_ring->tx_lock);
    mutex_lock(&tx_ring->tx_comp_lock);

    hmac_tx_host_ring_release_nolock(tx_ring);

    mutex_unlock(&tx_ring->tx_comp_lock);
    mutex_unlock(&tx_ring->tx_lock);
}

static void hmac_tx_tid_info_dump(hmac_tid_info_stru *tid_info)
{
    oam_warning_log4(0, 0, "hmac_tx_tid_info_dump::user[%d] tid[%d] ring_tx_mode[%d] msdu_cnt[%d]",
        tid_info->user_index, tid_info->tid_no,
        oal_atomic_read(&tid_info->ring_tx_mode), oal_atomic_read(&tid_info->tx_ring.msdu_cnt));

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oam_warning_log3(0, 0, "hmac_tx_tid_info_dump::release[%d] rptr[%d] wptr[%d]",
        READ_ONCE(tid_info->tx_ring.release_index), READ_ONCE(tid_info->tx_ring.base_ring_info.read_index),
        READ_ONCE(tid_info->tx_ring.base_ring_info.write_index));
#else
    oam_warning_log3(0, 0, "hmac_tx_tid_info_dump::release[%d] rptr[%d] wptr[%d]",
        tid_info->tx_ring.release_index, tid_info->tx_ring.base_ring_info.read_index,
        tid_info->tx_ring.base_ring_info.write_index);
#endif
}

#define HMAC_WAIT_TX_RING_EMPTY_MAX_CNT 25
void hmac_wait_tx_ring_empty(hmac_tid_info_stru *tid_info)
{
    uint8_t wait_cnt = 0;

    while (oal_atomic_read(&tid_info->tx_ring.msdu_cnt) && wait_cnt < HMAC_WAIT_TX_RING_EMPTY_MAX_CNT) {
        if (++wait_cnt == HMAC_WAIT_TX_RING_EMPTY_MAX_CNT) {
            hmac_tx_tid_info_dump(tid_info);
        }
        oal_msleep(2); // 2: sleep 2ms
    }
}

static inline void hmac_tid_info_queue_release(oal_netbuf_head_stru *tid_queue)
{
    oal_netbuf_stru *netbuf = NULL;

    while ((netbuf = hmac_tid_netbuf_dequeue(tid_queue)) != NULL) {
        oal_netbuf_free(netbuf);
#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
        hmac_tid_queue_depth_decrease(1);
#endif
    }
}

void hmac_tid_info_deinit(hmac_tid_info_stru *tid_info)
{
    oal_spin_lock_bh(&tid_info->lock);

    if (!hmac_tid_info_inited(tid_info)) {
        oal_spin_unlock_bh(&tid_info->lock);
        return;
    }

    oal_atomic_set(&tid_info->inited, OAL_FALSE);
    hmac_tid_info_queue_release(&tid_info->tid_queue);

    oal_spin_unlock_bh(&tid_info->lock);

    hmac_tid_schedule_list_delete(tid_info);
}

static uint32_t hmac_wait_ring_tx_done(hmac_tid_info_stru *tid_info)
{
    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return OAL_FAIL;
    }

    hmac_tx_reg_write_ring_info(tid_info, TID_CMDID_STOP_TX);
    hal_pm_try_wakeup_allow_sleep();

    oal_msleep(20); /* wait 20ms for mac to complete tx */

    return OAL_SUCC;
}

static void hmac_host_tx_ring_deinit(hmac_user_stru *hmac_user, hmac_tid_info_stru *tid_info)
{
    hmac_vap_stru *hmac_vap = NULL;

    if (!hmac_host_ring_tx_enabled() || !oal_atomic_read(&tid_info->tx_ring.inited)) {
        return;
    }

    oal_atomic_set(&tid_info->tx_ring.inited, OAL_FALSE);

    if (hmac_wait_ring_tx_done(tid_info) != OAL_SUCC) {
        hmac_wait_tx_ring_empty(tid_info);
    }
    oam_warning_log2(0, OAM_SF_CFG, "{hmac_host_tx_ring_deinit::tid update list dequeue, user[%d] tid[%d]}",
        tid_info->user_index, tid_info->tid_no);
    hmac_tid_update_list_delete(tid_info);
    /* 清除tx switch ring list中的TID节点 */
    hmac_tid_ring_switch_disable(hmac_user);
    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() == OAL_SUCC) {
        hmac_tx_reg_write_ring_info(tid_info, TID_CMDID_DEL);
    }
    hal_pm_try_wakeup_allow_sleep();
    hmac_tx_host_ring_release(&tid_info->tx_ring);

    hmac_vap = mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (hmac_vap == NULL) {
        return;
    }

    hmac_host_ring_tx_resume(hmac_vap, hmac_user, tid_info);
}

static void hmac_check_tid_info_deinit_done(hmac_tid_info_stru *tid_info)
{
    if (hmac_tid_info_inited(tid_info) != OAL_FALSE) {
        oam_error_log3(0, OAM_SF_CFG, "{hmac_check_tid_info_deinit_done::user[%d] tid[%d], inited error[%d]}",
            tid_info->user_index, tid_info->tid_no, oal_atomic_read(&tid_info->inited));
    }
    if (tid_info->is_in_ring_list != OAL_FALSE) {
        oam_error_log3(0, OAM_SF_CFG, "{hmac_check_tid_info_deinit_done::user[%d] tid[%d], ring list error[%d]}",
            tid_info->user_index, tid_info->tid_no, tid_info->is_in_ring_list);
    }
    if (tid_info->tid_ring_switch_entry.pst_prev != NULL || tid_info->tid_ring_switch_entry.pst_next != NULL) {
        oam_error_log2(0, OAM_SF_CFG, "{hmac_check_tid_info_deinit_done::user[%d] tid[%d], tid_switch_list error}",
            tid_info->user_index, tid_info->tid_no);
    }
}

void hmac_ring_tx_deinit(hmac_user_stru *hmac_user, uint8_t tid)
{
    hmac_tid_info_stru *tid_info = hmac_get_tx_tid_info(hmac_user, tid);
    if (tid_info == NULL || hmac_user->tid_res == NULL) {
        return;
    }

    if ((hmac_ring_tx_enabled() != OAL_TRUE) || tid == WLAN_TIDNO_BCAST) {
        return;
    }

    /* 走入deinit必然有对应的init，不为0 */
    if (hmac_user->tid_res->work_flag == 0) {
        oam_error_log2(0, 0, "hmac_ring_tx_deinit::user[%d] tid[%d] work flag error = 0",
            hmac_user->st_user_base_info.us_assoc_id, tid);
    }
    /* =1说明只有当前单个user使用tid资源，不存在共用，可直接释放tx ring */
    if (hmac_user->tid_res->work_flag != 1) {
        oam_warning_log1(0, 0, "hmac_ring_tx_deinit::another link using, not deinit(work_flag[%d])",
            hmac_user->tid_res->work_flag);
        return;
    }

    hmac_tid_info_deinit(tid_info);
    hmac_host_tx_ring_deinit(hmac_user, tid_info);
    /* 检查tid_info */
    hmac_check_tid_info_deinit_done(tid_info);
}

void hmac_user_tx_ring_deinit(hmac_user_stru *hmac_user)
{
    uint32_t tid;

    if (hmac_ring_tx_enabled() != OAL_TRUE) {
        return;
    }
    hmac_tid_switch_list_dump(HMAC_TID_SWITCH_LIST_BEFORE_DEL_USER);
    hmac_tid_update_list_dump(HMAC_TID_UPDATE_LIST_BEFORE_DEL_USER);
    hmac_tid_sche_list_dump(HMAC_TID_SCHE_LIST_BEFORE_DEL_USER);
    for (tid = 0; tid < WLAN_TID_MAX_NUM; tid++) {
        hmac_ring_tx_deinit(hmac_user, tid);
    }
    hmac_tid_switch_list_dump(HMAC_TID_SWITCH_LIST_AFTER_DEL_USER);
    hmac_tid_update_list_dump(HMAC_TID_UPDATE_LIST_AFTER_DEL_USER);
    hmac_tid_sche_list_dump(HMAC_TID_SCHE_LIST_AFTER_DEL_USER);
}

#define get_low_32bits(val)  ((uint32_t)(((uint64_t)(val)) & 0x00000000FFFFFFFFUL))
#define get_high_32bits(val) ((uint32_t)((((uint64_t)(val)) & 0xFFFFFFFF00000000UL) >> 32UL))
static void hmac_sync_host_ba_ring_addr(uint8_t hal_dev_id)
{
    hal_host_ring_ctl_stru *ba_ring_ctl = &hal_get_host_device(hal_dev_id)->host_ba_info_ring;
    host_ba_ring_info_sync_stru host_ba_ring_info = {
        .hal_dev_id = hal_dev_id,
        .size = ba_ring_ctl->entries,
        .lsb = get_low_32bits(ba_ring_ctl->devva),
        .msb = get_high_32bits(ba_ring_ctl->devva),
    };

    (void)hmac_config_send_event(mac_res_get_mac_vap(0), WLAN_CFGID_HOST_BA_INFO_RING_SYN,
                                 sizeof(host_ba_ring_info_sync_stru), (uint8_t *)&host_ba_ring_info);
}

uint32_t hmac_set_tx_ring_device_base_addr(frw_event_mem_stru *frw_mem)
{
    frw_event_stru *frw_event = NULL;
    mac_d2h_tx_ring_base_addr_stru *d2h_tx_ring_base_addr = NULL;

    if (frw_mem == NULL) {
        return OAL_FAIL;
    }

    if (!hmac_host_ring_tx_enabled()) {
        return OAL_SUCC;
    }

    frw_event = frw_get_event_stru(frw_mem);
    d2h_tx_ring_base_addr = (mac_d2h_tx_ring_base_addr_stru *)frw_event->auc_event_data;

    hal_host_tx_set_ring_table_base_addr(d2h_tx_ring_base_addr->tx_ring_base_addr);
    /* ba info默认未使能, device空处理 */
    hal_host_ba_ring_regs_init(HAL_DEVICE_ID_MASTER);
    hal_host_ba_ring_regs_init(HAL_DEVICE_ID_SLAVE);
    hmac_sync_host_ba_ring_addr(HAL_DEVICE_ID_SLAVE);

    return OAL_SUCC;
}
