/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : UAPSD hmac层处理
 * 作    者 :
 * 创建日期 : 2013年9月18日
 */

#include "mac_vap.h"
#include "mac_frame.h"
#include "hmac_mgmt_ap.h"
#include "plat_pm_wlan.h"
#include "hmac_tid_sche.h"
#include "hmac_tx_data.h"
#include "hmac_rx_data.h"
#include "hmac_dyn_pcie.h"
#include "hmac_uapsd.h"
#include "hmac_config.h"
#include "securec.h"
#include "hmac_tid_ring_switch.h"
#include "board_pm.h"
#include "hmac_fsm.h"
#include "hmac_scan.h"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_exception_rst.h"
#endif

#include "hisi_customize_wifi.h"
#include "hmac_pm.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "wal_linux_netdev_ops.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_PM_C
oal_bool_enum_uint8 g_force_sleep_switch = OAL_FALSE;  // S3强制休眠配置开关
uint8_t g_wlan_device_pm_switch = OAL_TRUE;  // device 低功耗开关
uint8_t g_wlan_ps_mode = 1;
uint8_t g_wlan_fast_ps_dyn_ctl = 0;  // app layer dynamic ctrl enable
uint8_t g_wlan_min_fast_ps_idle = 1;
uint8_t g_wlan_max_fast_ps_idle = 10;
uint8_t g_wlan_auto_ps_screen_on = 5;
uint8_t g_wlan_auto_ps_screen_off = 5;
uint8_t g_wlan_min_fast_ps_idle_p2p = 1;
uint8_t g_wlan_max_fast_ps_idle_p2p = 10;
uint8_t g_wlan_auto_ps_screen_on_p2p = 5;
uint8_t g_wlan_auto_ps_screen_off_p2p = 5;
static uint8_t g_wlan_wkup_src_parse_switch = OAL_FALSE;

static uint8_t hmac_is_trx_busy(void)
{
    struct wlan_pm_s *wlan_pm_info = wlan_pm_get_drv();

    if (oal_warn_on(wlan_pm_info == NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "hmac_is_trx_busy: wlan_pm is null \n");
        return OAL_FALSE;
    }

    if (hmac_ring_tx_enabled() != OAL_TRUE) {
        return OAL_FALSE;
    }

    if (hmac_is_tid_empty() == OAL_FALSE) {
        wlan_pm_info->tid_not_empty_cnt++;
        return OAL_TRUE;
    }

    if (oal_atomic_read(&g_tid_schedule_list.ring_mpdu_num) != 0) {
        wlan_pm_info->ring_has_mpdu_cnt++;
        return OAL_TRUE;
    }

    /* Host DDR接收不允许平台睡眠,低流量切device接收 */
    if (hal_master_is_in_ddr_rx() == OAL_TRUE) {
        wlan_pm_info->master_ddr_rx_cnt++;
        return OAL_TRUE;
    }
    if (hal_slave_is_in_ddr_rx() == OAL_TRUE) {
        wlan_pm_info->slave_ddr_rx_cnt++;
        return OAL_TRUE;
    }

    if (hmac_is_tid_device_ring() == OAL_FALSE) {
        wlan_pm_info->tid_ring_host_cnt++;
        return OAL_TRUE;
    }

    if (hal_is_host_forbid_sleep() == OAL_TRUE) {
        wlan_pm_info->host_forbid_sleep_cnt++;
        return OAL_TRUE;
    }
    /* 常发常收不允许平台睡眠 */
    if (hal_device_is_al_trx() == OAL_TRUE) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

void hmac_refresh_vap_pm_pause_cnt(hmac_vap_stru *hmac_vap, oal_bool_enum_uint8 *uc_is_any_cnt_exceed_limit,
                                   oal_bool_enum_uint8 *uc_is_any_timer_registerd)
{
    if (hmac_vap->st_ps_sw_timer.en_is_registerd == OAL_TRUE) {
        *uc_is_any_timer_registerd = OAL_TRUE;
        hmac_vap->us_check_timer_pause_cnt++;
        /* 低功耗pause计数 1000 次 打印输出一次 */
        if (hmac_vap->us_check_timer_pause_cnt % 1000 == 0) {
            oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                "{hmac_pm_pause_count_handle:: g_uc_check_timer_cnt[%d]",
                hmac_vap->us_check_timer_pause_cnt);
        }

        if (hmac_vap->us_check_timer_pause_cnt > HMAC_SWITCH_STA_PSM_MAX_CNT) {
            *uc_is_any_cnt_exceed_limit = OAL_TRUE;
            oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                "{hmac_pm_pause_count_handle::sw ps timer cnt too large[%d]> max[%d]}",
                hmac_vap->us_check_timer_pause_cnt, HMAC_SWITCH_STA_PSM_MAX_CNT);
        }
    } else {
        if (hmac_vap->us_check_timer_pause_cnt != 0) {
            oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                "{hmac_get_pm_pause_func::g_uc_check_timer_cnt end[%d],max[%d]",
                hmac_vap->us_check_timer_pause_cnt, HMAC_SWITCH_STA_PSM_MAX_CNT);
        }

        hmac_vap->us_check_timer_pause_cnt = 0;
    }
}

STATIC uint8_t hmac_check_dfr_status(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct st_exception_info *pst_exception_data = get_exception_info_reference();

    if (oal_work_is_busy(&pst_exception_data->wifi_excp_worker) ||
        oal_work_is_busy(&pst_exception_data->wifi_excp_recovery_worker)) {
        return OAL_TRUE;
    }
#endif
    return OAL_FALSE;
}

oal_bool_enum_uint8 hmac_get_pm_pause_func(void)
{
    hmac_device_stru *pst_hmac_device = NULL;

    uint8_t uc_vap_idx;
    hmac_vap_stru *hmac_vap = NULL;
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();
    oal_bool_enum_uint8 uc_is_any_cnt_exceed_limit = OAL_FALSE;
    oal_bool_enum_uint8 uc_is_any_timer_registerd = OAL_FALSE;

    if (hmac_check_dfr_status() == OAL_TRUE) {
        return OAL_TRUE;
    }

    /* 获取mac device结构体指针 */
    pst_hmac_device = hmac_res_get_mac_dev(0);
    if (pst_hmac_device == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_get_pm_pause_func::pst_device null.}");
        return OAL_FALSE;
    }
    if (pst_hmac_device->pst_device_base_info->pm_work_disable == OAL_TRUE) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_get_pm_pause_func::pm_work_disable[%d]}",
            pst_hmac_device->pst_device_base_info->pm_work_disable);
        return OAL_TRUE;
    }
    if (hmac_is_trx_busy() == OAL_TRUE) {
        return OAL_TRUE;
    }

    if (pst_hmac_device->st_scan_mgmt.en_is_scanning == OAL_TRUE) {
        return OAL_TRUE;
    }

    /* host启动了同信道监听定时器, 定时结束前不可进入host休眠 */
    if ((remain_on_chan_info->same_chan_remain_on_chan_bitmap != 0) &&
        (remain_on_chan_info->remain_on_chan_timer.en_is_registerd)) {
        return OAL_TRUE;
    }
    if (hmac_get_pcie_switch_func() == HMAC_PCIE_SWITCH_UP) {
        return OAL_FALSE;
    }

    for (uc_vap_idx = 0; uc_vap_idx < pst_hmac_device->pst_device_base_info->uc_vap_num; uc_vap_idx++) {
        hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_hmac_device->pst_device_base_info->auc_vap_id[uc_vap_idx]);
        if (hmac_vap == NULL) {
            oam_error_log0(0, OAM_SF_CFG, "{hmac_get_pm_pause_func::pst_hmac_vap null.}");
            return OAL_FALSE;
        }
        uc_is_any_cnt_exceed_limit = OAL_FALSE;
        uc_is_any_timer_registerd = OAL_FALSE;
        hmac_refresh_vap_pm_pause_cnt(hmac_vap, &uc_is_any_cnt_exceed_limit, &uc_is_any_timer_registerd);
        if (uc_is_any_timer_registerd && !uc_is_any_cnt_exceed_limit) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
/*
 * 功能描述  : S3休眠通知wifi侧业务
  *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_wifi_pm_suspend_notify(pm_notify_param* param)
{
    if (param->force_sleep == OAL_FALSE) {
        return;
    }
    if (g_freq_lock_control.hmac_freq_timer.en_is_registerd == OAL_TRUE) {
        g_wlan_freq_timer_period = WLAN_FREQ_TIMER_PERIOD / 4; // 周期更改为1/4
        hmac_freq_timer_param_reset();
        frw_timer_restart_timer_m(&(g_freq_lock_control.hmac_freq_timer), g_wlan_freq_timer_period, OAL_TRUE);
    }
    /* 定时器tx_ring快速切换处理 */
    g_tid_switch_list.pm_quick_switch_mode = OAL_TRUE;
    /* 固定rx_mode为ram */
    if ((hal_master_is_in_ddr_rx() == OAL_TRUE) || (hal_slave_is_in_ddr_rx() == OAL_TRUE)) {
        hmac_config_pm_rx_mode(mac_res_get_mac_vap(0), HAL_RAM_RX);
    }
    /* 禁止tid调度 */
    oal_atomic_set(&g_tid_sche_forbid, OAL_TRUE);
    oam_warning_log3(0, 0,
        "{hmac_wifi_pm_suspend_notify::S3 change freq_timer_period[%d] quick_switch[%d] tid_sche_forbid[%d]}",
        g_wlan_freq_timer_period, g_tid_switch_list.pm_quick_switch_mode, oal_atomic_read(&g_tid_sche_forbid));
}

/*
 * 功能描述  : S3恢复通知wifi侧业务
  *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_wifi_pm_resume_notify(pm_notify_param* param)
{
    if (param->force_sleep == OAL_FALSE) {
        return;
    }
    g_wlan_freq_timer_period = WLAN_FREQ_TIMER_PERIOD;
    g_tid_switch_list.pm_quick_switch_mode = OAL_FALSE;
    /* 取消固定rx_mode */
    hmac_config_pm_rx_mode(mac_res_get_mac_vap(0), HAL_RX_BUTT);
    oal_atomic_set(&g_tid_sche_forbid, OAL_FALSE);
    oam_warning_log3(0, 0,
        "{hmac_wifi_pm_resume_notify::S3 change freq_timer_period[%d] quick_switch[%d] tid_sche_forbid[%d]}",
        g_wlan_freq_timer_period, g_tid_switch_list.pm_quick_switch_mode, oal_atomic_read(&g_tid_sche_forbid));
}

/*
 * Prototype    : hal_tas_pm_notify_func
 * Description  : wifi模块低功耗处理回调
 */
static int hmac_wifi_pm_notify_func(unsigned long event, pm_notify_param* param, void *data)
{
    switch (event) {
        case PM_SUSPEND_PREPARE:
            hmac_wifi_pm_suspend_notify(param);
            break;
        case PM_HIBERNATION_PREPARE:
            break;
        case PM_POST_SUSPEND:
            hmac_wifi_pm_resume_notify(param);
            break;
        case PM_POST_HIBERNATION:
            break;
        default:
            break;
    }
    return BOARD_SUCC;
}

static pm_notify_node g_wifi_node = {
    .name = "wifi_pm_node",
    .priority = WIFI_NOITFY_PRIO,
    .cb_func = hmac_wifi_pm_notify_func,
};

/*
 * Prototype    : hmac_register_pm_init
 * Description  : wifi模块初始化
 */
void hmac_register_pm_init(void)
{
    int32_t ret;
    int32_t cfg_value = 0;
    /* 注册接收低功耗PM事件 */
    board_pm_register_notify_cb(&g_wifi_node);
    /* 获取ini的强制睡眠配置 */
    ret = get_cust_conf_int32(INI_MODU_PLAT, "force_sleep", &cfg_value);
    if (ret == INI_FAILED) {
        oam_warning_log1(0, 0, "{hmac_register_pm_init::fail to get force_sleep ini, keep disable[%d]}", cfg_value);
    }
    g_force_sleep_switch = (uint8_t)cfg_value;
}
#endif
void hmac_register_pm_callback(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

    struct wifi_srv_callback_handler *pst_wifi_srv_handler;

    pst_wifi_srv_handler = wlan_pm_get_wifi_srv_handler();
    if (pst_wifi_srv_handler != NULL) {
        pst_wifi_srv_handler->p_wifi_srv_get_pm_pause_func = hmac_get_pm_pause_func;
        pst_wifi_srv_handler->p_wifi_srv_open_notify = hmac_wifi_state_notify;
        pst_wifi_srv_handler->p_wifi_srv_pm_state_notify = hmac_wifi_pm_state_notify;
        pst_wifi_srv_handler->p_wifi_srv_pcie_switch_func = hmac_pcie_callback_fsm_func;
    } else {
        oal_io_print("hmac_register_pm_callback:wlan_pm_get_wifi_srv_handler is null\n");
    }

#endif
}

OAL_STATIC void hmac_fastsleep_cnter_clean(hmac_vap_stru *hmac_vap)
{
    hmac_vap->fastsleep_entry_cnt = 0;
    hmac_vap->fastsleep_exit_cnt = 0;
}

OAL_STATIC uint8_t hmac_fastsleep_check_enable(hmac_vap_stru *hmac_vap)
{
    if (is_sta(&(hmac_vap->st_vap_base_info)) == OAL_FALSE) {
        return OAL_FALSE;
    }

    if (hmac_vap->fastsleep_en == OAL_FALSE && hmac_vap->fastsleep3_en == OAL_FALSE) {
        return OAL_FALSE;
    }
    return OAL_TRUE;
}

OAL_STATIC uint32_t hmac_fastsleep2_packet_param(hmac_vap_stru *hmac_vap, uint8_t entry,
    uint8_t *fastsleep2_param, uint8_t len)
{
    int32_t ret = EOK;
    if (is_legacy_sta(&hmac_vap->st_vap_base_info)) {
        if (entry == OAL_TRUE) {
            fastsleep2_param[BYTE_OFFSET_0] = g_wlan_min_fast_ps_idle;
            fastsleep2_param[BYTE_OFFSET_1] = g_wlan_max_fast_ps_idle;
            fastsleep2_param[BYTE_OFFSET_2] = g_wlan_auto_ps_screen_on;
            fastsleep2_param[BYTE_OFFSET_3] = g_wlan_auto_ps_screen_off;
        } else {
            ret = memcpy_s(fastsleep2_param, len, &g_cust_cap.fast_ps, sizeof(cust_fast_pow_save_stru));
        }
    }

    if (is_p2p_cl(&hmac_vap->st_vap_base_info)) {
        if (entry == OAL_TRUE) {
            fastsleep2_param[BYTE_OFFSET_0] = g_wlan_min_fast_ps_idle_p2p;
            fastsleep2_param[BYTE_OFFSET_1] = g_wlan_max_fast_ps_idle_p2p;
            fastsleep2_param[BYTE_OFFSET_2] = g_wlan_auto_ps_screen_on_p2p;
            fastsleep2_param[BYTE_OFFSET_3] = g_wlan_auto_ps_screen_off_p2p;
        } else {
            ret = memcpy_s(fastsleep2_param, len, &g_cust_cap.p2p_fast_ps, sizeof(cust_p2p_fast_pow_save_stru));
        }
    }

    if (ret != EOK) {
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_fastsleep2_update_param(hmac_vap_stru *hmac_vap, uint8_t entry)
{
    uint8_t fastsleep2_param[BYTE_OFFSET_4] = {0};
    if (hmac_vap->fastsleep_en == OAL_FALSE) {
        return OAL_FAIL;
    }

    if (hmac_fastsleep2_packet_param(hmac_vap, entry, &fastsleep2_param[0], sizeof(fastsleep2_param)) != OAL_SUCC) {
        return OAL_FAIL;
    }

    return hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_SET_FASTSLEEP_PARA,
        sizeof(fastsleep2_param), &fastsleep2_param[0]);
}

OAL_STATIC uint32_t hmac_fastsleep3_update_param(hmac_vap_stru *hmac_vap, fastsleep3_param_stru *fastsleep3_param)
{
    if (hmac_vap->fastsleep3_en == OAL_FALSE) {
        return OAL_FAIL;
    }
    return hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_SET_FASTSLEEP3_PARA,
        sizeof(fastsleep3_param_stru), (uint8_t *)(uintptr_t)fastsleep3_param);
}

OAL_STATIC uint32_t hmac_update_ps_mode(mac_vap_stru *mac_vap, uint8_t ps_mode)
{
    uint32_t ps_mode_param = ps_mode;
    mac_cfg_ps_mode_param_stru mac_cfg_ps_mode_param = {0};

    hmac_set_sta_ps_mode(mac_vap, &ps_mode_param);
    mac_cfg_ps_mode_param.uc_vap_ps_mode = ps_mode;
    return hmac_config_send_event(mac_vap, WLAN_CFGID_SET_PS_MODE,
        sizeof(mac_cfg_ps_mode_param_stru), (uint8_t *)(uintptr_t)&mac_cfg_ps_mode_param);
}

OAL_STATIC void hmac_fastsleep_exit(hmac_vap_stru *hmac_vap)
{
    fastsleep3_param_stru fastsleep3_param = {0};

    if (g_wlan_spec_cfg->high_pps_fastsleep_close == OAL_FALSE) {
        return;
    }

    if (hmac_fastsleep_check_enable(hmac_vap) == OAL_FALSE) {
        return;
    }
    if (hmac_vap->uc_ps_mode != AUTO_FAST_PS) {
        return;
    }
    /* fastsleep2.0 退出，下发定制化参数 */
    hmac_fastsleep2_update_param(hmac_vap, OAL_FALSE);
    /* fastsleep3.0 退出，下发全0参数 */
    hmac_fastsleep3_update_param(hmac_vap, &fastsleep3_param);

    /* ps mode修改为MIN_FAST_PS，睡眠定时器修改为默认值 */
    hmac_update_ps_mode(&hmac_vap->st_vap_base_info, MIN_FAST_PS);
    hmac_fastsleep_cnter_clean(hmac_vap);
}

OAL_STATIC void hmac_fastsleep_entry(hmac_vap_stru *hmac_vap)
{
    if (g_wlan_spec_cfg->high_pps_fastsleep_close == OAL_FALSE) {
        return;
    }

    if (hmac_fastsleep_check_enable(hmac_vap) == OAL_FALSE) {
        return;
    }
    if (hmac_vap->uc_ps_mode == AUTO_FAST_PS) {
        return;
    }
    /* 进入fastsleep2.0，下发原上层命令参数 */
    hmac_fastsleep2_update_param(hmac_vap, OAL_TRUE);
    /* 进入fastsleep3.0，下发原上层命令参数 */
    hmac_fastsleep3_update_param(hmac_vap, &hmac_vap->fastsleep3_param);

    /* ps mode修改为AUTO_FAST_PS，调整睡眠定时器 */
    hmac_update_ps_mode(&hmac_vap->st_vap_base_info, AUTO_FAST_PS);
    hmac_fastsleep_cnter_clean(hmac_vap);
}

#define HMAC_FASTSLEEP_EXIT_PPS     8
#define HMAC_FASTSLEEP_ENTRY_PPS    4
#define HMAC_FASTSLEEP_EXIT_CNT     3
#define HMAC_FASTSLEEP_ENTRY_CNT    10
OAL_STATIC void hmac_fastsleep_vap_pps_proc(hmac_vap_stru *hmac_vap, uint32_t throughput)
{
    /* 高流量退出计数以及处理 */
    if (throughput > HMAC_FASTSLEEP_EXIT_PPS) {
        hmac_vap->fastsleep_exit_cnt++;
    } else {
        hmac_vap->fastsleep_exit_cnt = 0;
    }
    if (hmac_vap->fastsleep_exit_cnt > HMAC_FASTSLEEP_EXIT_CNT) {
        hmac_fastsleep_exit(hmac_vap);
    }
    /* 低流量进入计数以及处理 */
    if (throughput < HMAC_FASTSLEEP_ENTRY_PPS) {
        hmac_vap->fastsleep_entry_cnt++;
    } else {
        hmac_vap->fastsleep_entry_cnt = 0;
    }
    if (hmac_vap->fastsleep_entry_cnt > HMAC_FASTSLEEP_ENTRY_CNT) {
        hmac_fastsleep_entry(hmac_vap);
    }
}

void hmac_fastsleep_high_pps_close_check(uint32_t throughput)
{
    uint8_t vap_idx;
    hmac_vap_stru *hmac_vap = NULL;
    mac_device_stru *mac_device = NULL;

    if (g_wlan_spec_cfg->high_pps_fastsleep_close == OAL_FALSE) {
        return;
    }

    mac_device = mac_res_get_dev(0);
    if (mac_device == NULL) {
        return;
    }

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        hmac_vap = mac_res_get_hmac_vap(mac_device->auc_vap_id[vap_idx]);
        if (hmac_vap == NULL) {
            continue;
        }

        if ((hmac_fastsleep_check_enable(hmac_vap) == OAL_TRUE) &&
            (mac_vap_is_up(&hmac_vap->st_vap_base_info) == OAL_TRUE)) {
            hmac_fastsleep_vap_pps_proc(hmac_vap, throughput);
        }
    }
}

void hmac_fastsleep_clear_param(mac_vap_stru *mac_vap)
{
    hmac_vap_stru *hmac_vap = NULL;
    if (mac_vap == NULL) {
        return;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        return;
    }

    if (hmac_vap->fastsleep_en == OAL_FALSE && hmac_vap->fastsleep3_en == OAL_FALSE) {
        return;
    }
    hmac_vap->fastsleep_en = OAL_FALSE;
    hmac_vap->fastsleep3_en = OAL_FALSE;
    hmac_fastsleep_cnter_clean(hmac_vap);
    memset_s(&(hmac_vap->fastsleep3_param), sizeof(fastsleep3_param_stru), 0, sizeof(fastsleep3_param_stru));
}

void hmac_fastsleep_stop(mac_vap_stru *mac_vap)
{
    hmac_vap_stru *hmac_vap = NULL;
    if (mac_vap == NULL) {
        return;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        return;
    }
    hmac_fastsleep_exit(hmac_vap);
    hmac_fastsleep_clear_param(mac_vap);
}

uint32_t hmac_set_sta_ps_mode(mac_vap_stru *mac_vap, uint32_t *params)
{
    hmac_vap_stru *hmac_vap = NULL;
    if (mac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_vap->uc_ps_mode = (uint8_t)params[0];
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_PWR,
                     "hmac_set_sta_ps_mode ps_mode[%d]",
                     hmac_vap->uc_ps_mode);

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if ((hmac_vap->uc_ps_mode == MAX_FAST_PS) || (hmac_vap->uc_ps_mode == AUTO_FAST_PS)) {
        wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id,
            (g_wlan_min_fast_ps_idle > 1) ? (g_wlan_min_fast_ps_idle - 1) : g_wlan_min_fast_ps_idle);
    } else {
#endif
        wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, WLAN_SLEEP_DEFAULT_CHECK_CNT);
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    }
#endif
    return OAL_SUCC;
}

void hmac_mvap_stop_fastsleep(void)
{
    uint8_t vap_idx;
    hmac_vap_stru *hmac_vap = NULL;
    mac_device_stru *mac_device = mac_res_get_dev(0);
    if (mac_device == NULL) {
        return;
    }

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        hmac_vap = mac_res_get_hmac_vap(mac_device->auc_vap_id[vap_idx]);
        if (hmac_vap == NULL) {
            continue;
        }

        if (hmac_fastsleep_check_enable(hmac_vap) == OAL_TRUE) {
            hmac_fastsleep_stop(&hmac_vap->st_vap_base_info);
        }
    }
}

void hmac_pm_wkup_src_parse_set(uint8_t en)
{
    g_wlan_wkup_src_parse_switch = en;
}

uint8_t hmac_pm_wkup_src_parse_get(void)
{
    return g_wlan_wkup_src_parse_switch;
}
