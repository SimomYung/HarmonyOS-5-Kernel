/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : vsp低时延丢包流程
 * 作者       : wifi
 * 创建日期   : 2020年12月24日
 */

#include "hmac_vsp.h"
#include "hmac_vsp_buf.h"
#include "hmac_tx_data.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tx_data.h"
#include "hmac_tx_ring_alloc.h"
#include "hmac_config.h"
#include "hmac_scan.h"
#include "hmac_tid_ring_switch.h"
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_ps.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_VSP_C

#ifdef _PRE_WLAN_FEATURE_VSP
static hmac_vsp_vcodec_vdec_ops g_default_vdec_ops = {
    .alloc_slice_mgmt = NULL,
    .rx_slice_done = NULL,
};

static hmac_vsp_vcodec_venc_ops g_default_venc_ops = {
    .wifi_tx_pkg_done = NULL,
    .wifi_tx_send_done = NULL,
};

hmac_vsp_vcodec_vdec_ops *g_vsp_vcodec_vdec_ops = &g_default_vdec_ops;
hmac_vsp_vcodec_venc_ops *g_vsp_vcodec_venc_ops = &g_default_venc_ops;
hmac_vsp_info_stru *g_vsp_info = NULL;

/* vdec call wifi to set callback */
void vdec_set_callback(struct vcodec_vdec_callback *callback)
{
    (void)memcpy_s(g_vsp_vcodec_vdec_ops, sizeof(struct vcodec_vdec_callback),
        callback, sizeof(struct vcodec_vdec_callback));
}

/* venc call wifi to set callback */
void venc_set_callback(struct vcodec_venc_callback *callback)
{
    (void)memcpy_s(g_vsp_vcodec_venc_ops, sizeof(struct vcodec_venc_callback),
        callback, sizeof(struct vcodec_venc_callback));
}

void hmac_vsp_set_current_handle(hmac_vsp_info_stru *vsp_info)
{
    g_vsp_info = vsp_info;
}

hmac_vsp_info_stru *hmac_vsp_get_current_handle(void)
{
    return g_vsp_info;
}

static uint32_t hmac_vsp_set_status(mac_vap_stru *mac_vap, uint8_t mode, uint16_t user_index)
{
    mac_vsp_cfg_stru vsp_cfg = {
        .user_index = user_index,
        .mode = mode,
    };

    return hmac_config_send_event(mac_vap, WLAN_CFGID_VSP_STATUS_CHANGE, sizeof(mac_vsp_cfg_stru), (uint8_t *)&vsp_cfg);
}

static uint32_t hmac_vsp_init_common_timer(hmac_vsp_info_stru *vsp_info)
{
    hal_mac_common_timer *timer = &vsp_info->timer;

    timer->timer_en = OAL_TRUE;
    timer->timer_mask_en = OAL_TRUE;
    timer->timer_unit = HAL_COMMON_TIMER_UNIT_1US;
    timer->timer_mode = HAL_COMMON_TIMER_MODE_ONE_SHOT;
    timer->func = NULL;
    timer->timer_id = g_wlan_spec_cfg->vsp_timer_id;
    hal_pm_try_wakeup_forbid_sleep(HAL_PM_FORBIDE_SLEEP_VSP_INIT_COMMON_TIMER);
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_VSP_INIT_COMMON_TIMER);
        return OAL_FAIL;
    }
    if (hal_host_init_common_timer(timer) != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_TX, "hmac_vsp_tx_info_init :: init common timer[%d] fail", timer->timer_id);
        hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_VSP_INIT_COMMON_TIMER);
        return OAL_FAIL;
    }
    hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_VSP_INIT_COMMON_TIMER);
    return OAL_SUCC;
}

static void hmac_vsp_deinit_common_timer(hmac_vsp_info_stru *vsp_info)
{
    hal_mac_common_timer *timer = &vsp_info->timer;

    timer->timer_en = OAL_FALSE;
    hal_host_set_mac_common_timer(timer);
}

static void hmac_vsp_clear_tx_ring(hmac_vsp_info_stru *vsp_info)
{
    hmac_tid_info_stru *tid_info = vsp_info->tid_info;
    hmac_msdu_info_ring_stru *tx_ring = &tid_info->tx_ring;
    unsigned long flags = 0;

    oal_mutex_lock(&tx_ring->tx_lock);
    oal_mutex_lock(&tx_ring->tx_comp_lock);
    hmac_wait_tx_ring_empty(tid_info);
    oal_spin_lock_irq_save(&vsp_info->vsp_lock, &flags);

    hmac_tx_ring_release_all_netbuf(tx_ring);

    oal_spin_unlock_irq_restore(&vsp_info->vsp_lock, &flags);
    oal_mutex_unlock(&tx_ring->tx_lock);
    oal_mutex_unlock(&tx_ring->tx_comp_lock);
}

typedef struct {
    hmac_scan_state_enum scan_state;
    mac_cfg_ps_open_stru pm_state;
} hmac_vsp_config_stru;

static void hmac_vsp_config_sta(mac_vap_stru *mac_vap, hmac_vsp_config_stru *param)
{
    if (!is_legacy_sta(mac_vap)) {
        return;
    }

    hmac_config_set_sta_pm_on(mac_vap, 0, (uint8_t *)&param->pm_state);
    hmac_bgscan_enable(mac_vap, 0, (uint8_t *)&param->scan_state);
}

static void hmac_vsp_stop_config_sta(hmac_vsp_info_stru *vsp_info)
{
    hmac_vsp_config_stru param = {
        .scan_state = HMAC_BGSCAN_ENABLE,
        .pm_state = {
            .uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_CMD,
            .uc_pm_enable = MAC_STA_PM_SWITCH_ON,
        }
    };

    hmac_vsp_config_sta(&vsp_info->hmac_vap->st_vap_base_info, &param);
}

/* 现阶段为保证投屏时延与稳定性, 需要关闭背景扫描与协议低功耗 */
static void hmac_vsp_start_config_sta(hmac_vsp_info_stru *vsp_info)
{
    hmac_vsp_config_stru param = {
        .scan_state = HMAC_BGSCAN_DISABLE,
        .pm_state = {
            .uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_CMD,
            .uc_pm_enable = MAC_STA_PM_SWITCH_OFF,
        }
    };

    hmac_vsp_config_sta(&vsp_info->hmac_vap->st_vap_base_info, &param);
}

static uint32_t hmac_vsp_tid_ring_switch(hmac_vsp_info_stru *vsp_info)
{
    uint8_t cnt = 5; // 最多重试5次

    if (vsp_info->tid_info == NULL) {
        return OAL_FAIL;
    }
    while (oal_atomic_read(&vsp_info->tid_info->ring_tx_mode) != HOST_RING_TX_MODE &&
        oal_atomic_read(&vsp_info->tid_info->ring_tx_mode) != D2H_SWITCHING_MODE && cnt != 0) {
        hmac_tid_ring_switch(vsp_info->hmac_user->st_user_base_info.us_assoc_id, vsp_info->tid_info, HOST_RING_TX_MODE);
        oal_msleep(2); // 间隔2ms
        cnt--;
    }

    if (oal_atomic_read(&vsp_info->tid_info->ring_tx_mode) == HOST_RING_TX_MODE ||
        oal_atomic_read(&vsp_info->tid_info->ring_tx_mode) == D2H_SWITCHING_MODE) {
        return OAL_SUCC;
    } else {
        oam_error_log2(0, 0, "hmac_vsp_tid_ring_switch:FAIL! ring_tx_mode[%d]cnt[%d]",
            oal_atomic_read(&vsp_info->tid_info->ring_tx_mode), cnt);
        return OAL_FAIL;
    }
}

uint32_t hmac_vsp_context_init(hmac_vsp_info_stru *vsp_info, hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_tid_info_stru *tid_info = vsp_info->tid_info;

    hmac_vsp_start_config_sta(vsp_info);
    hmac_tid_info_queue_release(&tid_info->tid_queue);
    hmac_vsp_clear_tx_ring(vsp_info);
    if (hmac_vsp_tid_ring_switch(vsp_info) != OAL_SUCC) {
        return OAL_FAIL;
    }
    if (hmac_tid_need_ba_session(hmac_vap, hmac_user, WLAN_TIDNO_VSP) == OAL_TRUE) {
        hmac_tx_ba_setup(hmac_vap, hmac_user, WLAN_TIDNO_VSP);
    }
    hmac_user->vsp_hdl = vsp_info;
    return OAL_SUCC;
}

void hmac_vsp_context_deinit(hmac_vsp_info_stru *vsp_info)
{
    hmac_vsp_clear_tx_ring(vsp_info);
    hmac_vsp_stop_config_sta(vsp_info);
}

#ifdef _PRE_WLAN_CHBA_MGMT
static void hmac_vsp_init_fix_chba_bitmap(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user)
{
    // vsp+chba场景：固定全醒
    hmac_chba_set_auto_bitmap_switch_event(mac_vap, CHBA_BITMAP_FIX);
    hmac_user->chba_user.user_bitmap_level = CHBA_BITMAP_ALL_AWAKE_LEVEL;
    hmac_chba_set_user_bitmap_event(mac_vap, hmac_user);
    hmac_chba_set_vap_bitmap_event(mac_vap, CHBA_BITMAP_ALL_AWAKE_LEVEL);
}

static void hmac_vsp_deinit_set_chba_bitmap(mac_vap_stru *mac_vap)
{
    // vsp+chba场景：退出时恢复自动调整
    hmac_chba_set_auto_bitmap_switch_event(mac_vap, CHBA_BITMAP_AUTO);
}
#endif

uint32_t hmac_vsp_init(
    hmac_vsp_info_stru *vsp_info, hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, vsp_param *param, uint8_t mode)
{
    mac_vap_stru *mac_vap = &hmac_vap->st_vap_base_info;
    hal_host_device_stru *hal_device = hal_get_host_device(mac_vap->uc_device_id);
    if (!hal_device) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_vsp_init::hal_device NULL}");
        return OAL_FAIL;
    }

    vsp_info->hmac_user = hmac_user;
    vsp_info->hmac_vap = hmac_vap;
    vsp_info->tid_info = hmac_get_tx_tid_info(hmac_user->st_user_base_info.us_assoc_id, WLAN_TIDNO_VSP);
    if (vsp_info->tid_info == NULL) {
        return OAL_FAIL;
    }
    vsp_info->host_hal_device = hal_device;
    vsp_info->mode = mode;
    oal_spin_lock_init(&vsp_info->vsp_lock);
    if (hmac_vsp_init_common_timer(vsp_info) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if (memcpy_s(&vsp_info->param, sizeof(vsp_param), param, sizeof(vsp_param)) != EOK) {
        return OAL_FAIL;
    }

    vsp_info->timer_running = OAL_FALSE;
    hmac_vsp_set_status(mac_vap, mode, hmac_user->st_user_base_info.us_assoc_id);
    hmac_vsp_set_vcodec_vdec_ops(&g_default_vdec_ops);
    hmac_vsp_set_vcodec_venc_ops(&g_default_venc_ops);

    oam_warning_log3(0, 0, "{hmac_vsp_init::VSP[%d] init SUCC, vap idx[%d], user idx[%d]!}",
        mode, mac_vap->uc_vap_id, hmac_user->st_user_base_info.us_assoc_id);
#ifdef _PRE_WLAN_CHBA_MGMT
    hmac_vsp_init_fix_chba_bitmap(mac_vap, hmac_user);
#endif

    return OAL_SUCC;
}

void hmac_vsp_deinit(hmac_vsp_info_stru *vsp_info)
{
    if (oal_any_null_ptr3(vsp_info, vsp_info->hmac_vap, vsp_info->hmac_user)) {
        oam_error_log0(0, 0, "{hmac_vsp_deinit::vsp deinit failed due to null ptr!}");
        return;
    }

    vsp_info->enable = OAL_FALSE;
    hmac_vsp_deinit_common_timer(vsp_info);
    hmac_vsp_set_status(
        &vsp_info->hmac_vap->st_vap_base_info, VSP_MODE_DISABLED, vsp_info->hmac_user->st_user_base_info.us_assoc_id);
#ifdef _PRE_WLAN_CHBA_MGMT
    hmac_vsp_deinit_set_chba_bitmap(&vsp_info->hmac_vap->st_vap_base_info);
#endif
    vsp_info->hmac_vap = NULL;
    vsp_info->hmac_user = NULL;
    vsp_info->host_hal_device = NULL;
}

uint32_t hmac_vsp_get_timestamp(hmac_vsp_info_stru *vsp_info)
{
    uint32_t tsf = 0;

    if (oal_any_null_ptr3(vsp_info, vsp_info->host_hal_device, vsp_info->hmac_vap)) {
        return 0;
    }

    return hal_host_get_tsf_lo(vsp_info->host_hal_device, vsp_info->hmac_vap->hal_vap_id, &tsf) == OAL_SUCC ? tsf : 0;
}

void hmac_vsp_start_timer(hmac_vsp_info_stru *vsp_info, uint32_t start_time, uint32_t timeout_us)
{
    if (!vsp_info) {
        oam_error_log0(0, 0, "{hmac_vsp_start_timer::vsp timer start failed!}");
        return;
    }

    oal_warn_on(vsp_info->timer_running);
    vsp_info->timer.start_time = start_time;
    vsp_info->timer.common_timer_val = timeout_us;
    vsp_info->timer.timer_en = OAL_TRUE;
    hal_host_set_mac_common_timer(&vsp_info->timer);
    vsp_info->timer_running = OAL_TRUE;
}

void hmac_vsp_stop_timer(hmac_vsp_info_stru *vsp_info)
{
    if (!vsp_info) {
        oam_error_log0(0, 0, "{hmac_vsp_stop_timer::vsp timer stop failed!}");
        return;
    }

    vsp_info->timer.timer_en = OAL_FALSE;
    hal_host_set_mac_common_timer(&vsp_info->timer);
    vsp_info->timer_running = OAL_FALSE;
}

void hmac_vsp_stop_timer_no_pm_wakeup(hmac_vsp_info_stru *vsp_info)
{
    if (!vsp_info) {
        oam_error_log0(0, 0, "{hmac_vsp_stop_timer_no_pm_wakeup::vsp timer stop failed!}");
        return;
    }

    vsp_info->timer.timer_en = OAL_FALSE;
    hal_host_set_mac_common_timer_no_pm_wakeup(&vsp_info->timer);
    vsp_info->timer_running = OAL_FALSE;
}

static uint32_t hmac_vsp_calc_slice_timeout(hmac_vsp_info_stru *vsp_info, uint8_t slice_id, uint32_t cur,
    uint8_t is_cfg_pkt)
{
    uint32_t consume = hmac_vsp_calc_runtime(vsp_info->timer_ref_vsync, cur); // 当前slice已经run的时间
    uint32_t target = vsp_info->param.tmax; // 以每个slice各自的时间作为超时时间
    uint32_t timeout;

    if (is_cfg_pkt == OAL_TRUE) {
        target = vsp_info->param.tfeedback;
    }

    if (consume < target) {
        timeout = target - consume; // 当前slice的剩余时间作为超时时间
        /* min timeout for a slice 100us */
        if (oal_warn_on(timeout < 100)) {
            timeout = vsp_info->param.tslice;
        }
    } else {
        oam_error_log2(0, 0, "{hmac_vsp_calc_slice_timeout::slice[%d] consume[%d] is lager than target transmit dly}",
            slice_id, consume);
        timeout = vsp_info->param.tslice;
    }

    return timeout;
}

void hmac_vsp_set_slice_timeout(hmac_vsp_info_stru *vsp_info, uint8_t slice_id, uint8_t is_cfg_pkt)
{
    uint32_t cur;

    if (!vsp_info) {
        oam_error_log0(0, 0, "{hmac_vsp_stop_timer::vsp timer stop failed!}");
        return;
    }

    cur = hmac_vsp_get_timestamp(vsp_info);
    hmac_vsp_start_timer(vsp_info, cur, hmac_vsp_calc_slice_timeout(vsp_info, slice_id, cur, is_cfg_pkt));

    hmac_vsp_log((uint8_t *)&vsp_info->timer, VSP_LOG_BITMAP_TIMER, VSP_LOG_DATATYPE_TIMER, VSP_LOG_TYPE_START_TIMER);
}
void hmac_vsp_set_vcodec_vdec_ops(hmac_vsp_vcodec_vdec_ops *ops)
{
    g_vsp_vcodec_vdec_ops = ops;
}

void hmac_vsp_set_vcodec_venc_ops(hmac_vsp_vcodec_venc_ops *ops)
{
    g_vsp_vcodec_venc_ops = ops;
}
#endif
