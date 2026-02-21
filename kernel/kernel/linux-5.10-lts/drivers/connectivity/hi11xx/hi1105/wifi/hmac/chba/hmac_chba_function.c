/*
* Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
* 功能说明   : CHBA基本功能
* 作者       : wifi
* 创建日期   : 2021年1月27日
*/

#ifdef _PRE_WLAN_CHBA_MGMT
/* 1 头文件包含 */
#include "oam_event_wifi.h"
#include "securec.h"
#include "securectype.h"
#include "oal_cfg80211.h"
#include "hmac_chba_interface.h"
#include "hmac_chba_frame.h"
#include "hmac_chba_mgmt.h"
#include "hmac_chba_coex.h"
#include "wlan_types.h"
#include "mac_device.h"
#include "mac_ie.h"
#include "mac_frame_inl.h"
#include "hmac_chan_mgmt.h"
#include "hmac_scan.h"
#include "hmac_config.h"
#include "hmac_mgmt_ap.h"
#include "hmac_mgmt_sta.h"
#include "hmac_sme_sta.h"
#include "hmac_chba_sync.h"
#include "hmac_chba_frame.h"
#include "hmac_chba_user.h"
#include "hmac_chba_channel_sequence.h"
#include "hmac_chba_fsm.h"
#include "hmac_chba_chan_switch.h"
#include "hmac_chba_multi_connect.h"
#include "hmac_sme_sta.h"
#include "hmac_chba_ps.h"
#include "hmac_pm.h"
#include "hmac_chba_function.h"
#ifdef _PRE_WLAN_FEATURE_STA_PM
#include "plat_pm_wlan.h"
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif
#include "oam_ext_if.h"
#include "platform_oam_oneimage_define.h"
#ifdef _PRE_WLAN_FEATURE_NBW
#include "hmac_nbw_connect.h"
#include "mac_nbw_common.h"
#include "hmac_nbw_frame.h"
#include "hmac_nbw_mgmt.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CHBA_FUNCTION_C

#define CHBA_CONN_RESPONDER_TIMER_DEFAULT 1000 /* 极速连接响应方默认超时时间(1000ms) */

/* 2 全局变量定义 */
hmac_chba_vap_stru g_chba_vap_info = {0};
hmac_chba_system_params g_chba_system_params;
uint32_t g_discovery_bitmap = 0x01;

/* 3 函数声明 */
uint32_t hmac_chba_sync_waiting_timeout(void *arg);

uint32_t hmac_chba_fill_connect_params(uint8_t *params, hmac_chba_conn_param_stru *chba_conn_params,
    uint8_t connect_role);
static uint32_t hmac_chba_connect_check(mac_vap_stru *mac_vap, hmac_vap_stru *hmac_vap,
    hmac_chba_conn_param_stru *chba_conn_params, uint8_t connect_role);
static uint32_t hmac_chba_cfg_security_params(mac_vap_stru *mac_vap, hmac_conn_param_stru *connect_params);
uint32_t hmac_chba_connect_prepare(mac_vap_stru *mac_vap, hmac_chba_conn_param_stru *chba_conn_params,
    uint16_t *user_idx, uint8_t connect_role);
uint32_t hmac_chba_assoc_waiting_timeout_responder(void *arg);
void hmac_chba_ready_to_connect(hmac_vap_stru *hmac_vap, hmac_chba_conn_param_stru *conn_param);

/* 功能描述: 根据ini配置, 判断是否支持chba极速连接 */
oal_bool_enum_uint8 hmac_chba_is_support_fast_connect(void)
{
    return ((g_expand_feature_switch_bitmap & CUSTOM_CHBA_FAST_CONNECT_ENABLE) != 0);
}

/*
 * 功能描述  : 获取CHBA系统参数
 * 日    期  : 2021年3月13日
 * 作    者  : wifi
 */
hmac_chba_system_params *hmac_get_chba_system_params(void)
{
    return &g_chba_system_params;
}

/*
 * 功能描述  : 判断chba vap是否使能
 * 日    期  : 2021年02月06日
 * 作    者  : wifi
 */
oal_bool_enum_uint8 hmac_chba_vap_start_check(hmac_vap_stru *hmac_vap)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_chba_vap_stru *chba_vap_info = NULL;

    if (hmac_vap == NULL) {
        return OAL_FALSE;
    }
    mac_vap = &(hmac_vap->st_vap_base_info);
    chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if ((mac_vap->chba_mode == CHBA_MODE) && (chba_vap_info == &g_chba_vap_info)) {
        if (hmac_chba_get_sync_state() > CHBA_INIT) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

/*
 * 功能描述  : 获取chba_vap_info指针(无论是否使能)
 * 日    期  : 2021年3月13日
 * 作    者  : wifi
 */
hmac_chba_vap_stru *hmac_get_chba_vap(void)
{
    return &g_chba_vap_info;
}

/*
 * 功能描述  : 获取chba_vap_info指针(如果返回值为NULL，说明chba_vap未使能)
 * 日    期  : 2021年3月13日
 * 作    者  : wifi
 */
hmac_chba_vap_stru *hmac_get_up_chba_vap_info(void)
{
    uint8_t vap_id;
    hmac_chba_vap_stru *chba_vap_info = NULL;
    hmac_vap_stru *hmac_vap = NULL;

    chba_vap_info = &(g_chba_vap_info);
    vap_id = chba_vap_info->mac_vap_id;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(vap_id);
    if (hmac_vap == NULL) {
        return NULL;
    }

    if (hmac_chba_vap_start_check(hmac_vap)) {
        return chba_vap_info;
    }

    return NULL;
}

uint32_t hmac_chba_is_valid_channel(uint8_t chan_number)
{
    wlan_channel_band_enum en_band = mac_get_band_by_channel_num(chan_number);
    if (en_band != WLAN_BAND_5G || mac_is_channel_num_valid(en_band, chan_number, OAL_FALSE) != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CHBA, "hmac_chba_is_valid_channel::invalid number %d.", chan_number);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 功能描述  : HAL层初始化过程中会下发命令到driver，配置相关系统参数
 * 日    期  : 2021年02月01日
 * 作    者  : wifi
 */
uint32_t hmac_config_chba_base_params(mac_chba_set_init_para *init_params)
{
    hmac_chba_system_params *system_params = &g_chba_system_params;
    hmac_chba_vap_stru *chba_vap_info = &g_chba_vap_info;

    if (init_params == NULL) {
        return OAL_SUCC;
    }

    /* 先销毁定时器 & 释放资源 */
    memset_s(chba_vap_info, sizeof(hmac_chba_vap_stru), 0, sizeof(hmac_chba_vap_stru));
    memset_s(system_params, sizeof(hmac_chba_system_params), 0, sizeof(hmac_chba_system_params));
    chba_vap_info->mac_vap_id = mac_chip_get_vap_support_max_num_limit(); /* 初始化vap id为无效值 */
    hmac_chba_set_sync_state(CHBA_INIT);
    system_params->schedule_period = init_params->config_para.schedule_period;
    system_params->sync_slot_cnt = init_params->config_para.discovery_slot;
    system_params->schedule_time_ms = MAC_CHBA_SLOT_DURATION * system_params->schedule_period;
    system_params->vap_alive_timeout = init_params->config_para.vap_alive_timeout;
    system_params->link_alive_timeout = init_params->config_para.link_alive_timeout;
#ifdef _PRE_WLAN_FEATURE_NBW
    if ((g_expand_feature_switch_bitmap & CUSTOM_NBW_CAP) != 0) {
        system_params->schedule_time_ms = MAC_NBW_SLOT_DURATION * system_params->schedule_period;
    }
#endif
    oam_warning_log2(0, 0, "hmac_config_chba_base_params::schedule_period[%d], shedule_period_ms[%d]",
        system_params->schedule_period, system_params->schedule_time_ms);
    return OAL_SUCC;
}

/*
 * 功能描述  : CHBA VAP使能后，初始化vap bitmap档位
 * 日    期  : 2021年01月12日
 * 作    者  : wifi7
 */
static void hmac_config_chba_vap_bitmap_level_init(hmac_vap_stru *hmac_vap, hmac_chba_vap_stru *chba_vap_info)
{
    /* 设置vap bitmap为最高档位 */
    chba_vap_info->vap_bitmap_level = CHBA_BITMAP_ALL_AWAKE_LEVEL;
    chba_vap_info->auto_bitmap_switch = CHBA_BITMAP_AUTO;

    /* 将初始化vap低功耗等级同步到dmac */
    if (hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_CHBA_SET_VAP_BITMAP_LEVEL, sizeof(uint8_t),
        &chba_vap_info->vap_bitmap_level) != OAL_SUCC) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{hmac_config_chba_vap_bitmap_level_init::send_event failed.}");
    }
}

/*
 * 功能描述  : CHBA VAP使能后，初始化CHBA信道配置
 * 日    期  : 2022年08月31日
 * 作    者  : wifi7
 */
static void hmac_chba_channel_init(hmac_vap_stru *hmac_vap, hmac_chba_vap_stru *chba_vap_info,
    uint8_t channel_number, uint8_t bandwidth)
{
    uint32_t ret;
    mac_channel_stru social_channel = {};
    mac_channel_stru work_channel = {};

    work_channel.uc_chan_number = channel_number;
    work_channel.en_bandwidth = bandwidth;
    work_channel.en_band = mac_get_band_by_channel_num(work_channel.uc_chan_number);
    ret = mac_get_channel_idx_from_num(work_channel.en_band, work_channel.uc_chan_number,
        OAL_FALSE, &(work_channel.uc_chan_idx));
    if (ret != OAL_SUCC) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
            "{hmac_chba_channel_init:invalid chan_cfg, chan_num[%d], bw[%d]}", channel_number, bandwidth);
        return;
    }

    if (hmac_chba_chan_check_is_valid(hmac_vap, &work_channel) == OAL_FALSE) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA, "{hmac_chba_channel_init:error cfg!}");
        return;
    }

    /* 初始化work channel */
    chba_vap_info->work_channel = work_channel;

    /* 根据work_channel获取合适的socail channel配置, 并初始化social channel */
    ret = hmac_chba_generate_social_channel_cfg(channel_number, bandwidth, &social_channel);
    if (ret != OAL_SUCC) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
            "{hmac_chba_channel_init:fail to init social chan, chan_num[%d], bw[%d]}", channel_number, bandwidth);
        return;
    }
    chba_vap_info->social_channel = social_channel;
}
/*
 * 功能描述  : CHBA VAP使能后，初始化CHBA VAP
 * 日    期  : 2021年02月01日
 * 作    者  : wifi
 */
static uint32_t hmac_config_chba_vap(mac_vap_stru *mac_vap, hmac_chba_vap_stru *chba_vap_info,
    mac_chba_param_stru *chba_params)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 初始化mac地址，生成域BSSID */
    oal_set_mac_addr(mac_mib_get_StationID(mac_vap), chba_params->mac_addr);
    hmac_chba_vap_update_domain_bssid(hmac_vap, mac_mib_get_StationID(mac_vap));

    /* 初始化vap基本能力 */
    mac_vap->st_cap_flag.bit_wpa2 = OAL_TRUE;

    /* 配置chba_vap相关变量 */
    hmac_chba_set_sync_state(CHBA_NON_SYNC); /* 初始化为non-sync */
    hmac_chba_set_role(CHBA_OTHER_DEVICE);

    /* 设置vap bitmap为全醒态 */
    hmac_config_chba_vap_bitmap_level_init(hmac_vap, chba_vap_info);

    /* chba维测日志默认关闭 */
    chba_vap_info->chba_log_level = CHBA_DEBUG_NO_LOG;

    /* 初始化chba信道 */
    hmac_chba_channel_init(hmac_vap, chba_vap_info, chba_params->init_channel.uc_chan_number,
        chba_params->init_channel.en_bandwidth);

    /* DBAC bitmap 初始化为全1 */
    hmac_chba_set_channel_seq_bitmap(chba_vap_info, CHBA_CHANNEL_SEQ_BITMAP_100_PERCENT);
    /* 初始化DBAC sta工作信道0 */
    memset_s(&(chba_vap_info->second_work_channel), sizeof(mac_channel_stru), 0, sizeof(mac_channel_stru));

    return OAL_SUCC;
}

#define OAM_GLOBAL_OTA_SWITCH_ON 0xFFFFFFFF
/*
 * 功能描述  : 启动chba，并进行初始化配置
 * 日    期  : 2021年02月01日
 * 作    者  : wifi
 */
uint32_t hmac_start_chba(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    uint8_t chba_log_level;
    uint32_t ota_switch;
    mac_chba_param_stru *chba_params = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_chba_vap_stru *chba_vap_info = &g_chba_vap_info;
#ifdef _PRE_WLAN_FEATURE_NBW
    hmac_nbw_vap_stru *nbw_vap_info = &g_nbw_vap_info;
#endif

    if (oal_any_null_ptr2(mac_vap, param)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    chba_params = (mac_chba_param_stru *)param;

    if (hmac_chba_get_sync_state() < CHBA_INIT) {
        oam_warning_log0(0, 0, "hmac_start_chba::fail to start chba vap beacause of uninitializing.");
        return OAL_FAIL;
    }

    if (hmac_chba_is_valid_channel(chba_params->init_channel.uc_chan_number) != OAL_SUCC) {
        oam_warning_log1(0, 0, "hmac_start_chba::fail to start chba vap beacause of dfs channel[%d].",
            chba_params->init_channel.uc_chan_number);
        return OAL_FAIL;
    }

    chba_vap_info->mac_vap_id = mac_vap->uc_vap_id;
    /* 获取hmac_vap */
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_warning_log0(mac_vap->uc_vap_id, 0, "{hmac_start_chba::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_vap->hmac_chba_vap_info = chba_vap_info;
#ifdef _PRE_WLAN_FEATURE_NBW
    hmac_vap->hmac_chba_vap_info->nbw_info = nbw_vap_info;
#endif
    /* 初始化白名单 */
    hmac_chba_whitelist_init();
    /* 初始化chba私有ie数据链表 */
    hmac_chba_assoc_priv_ie_list_init();
    /* 初始化chba活跃设备哈希表 */
    hmac_chba_alive_dev_table_init();
    /* 初始化chba vap info */
    hmac_config_chba_vap(mac_vap, chba_vap_info, chba_params);
    /* CHBA1.0 暂只支持单岛 */
    chba_vap_info->is_support_multiple_island = OAL_FALSE;
    hmac_chba_vap_start(mac_mib_get_StationID(mac_vap), &chba_vap_info->social_channel,
        &chba_vap_info->work_channel);

    /* 同步参数到dmac */
    hmac_chba_params_sync_after_start(mac_vap, chba_vap_info);

    oam_warning_log3(mac_vap->uc_vap_id, 0, "{hmac_start_chba::chba_mode=[%d], sync_state=[%d], chba_vap_info=[%d]}",
        mac_vap->chba_mode, hmac_chba_get_sync_state(), (hmac_vap->hmac_chba_vap_info == chba_vap_info));

    /* 维测上库, 如果已开启ota则同时开启chba log, 问题定位后删除 */
    ota_switch = oam_ota_get_global_switch_ctx(CUR_OAM_CONTEXT);
    if (ota_switch == OAM_GLOBAL_OTA_SWITCH_ON) {
        chba_log_level = CHBA_DEBUG_ALL_LOG;
        hmac_config_chba_log_level(mac_vap, sizeof(chba_log_level), &chba_log_level);
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CHBA, "{hmac_start_chba:chba log level switch_on!}");
    }
    return OAL_SUCC;
}

/*
 * 功能描述  : 去使能chba，并进行deinit操作
 * 日    期  : 2021年04月30日
 * 作    者  : wifi
 */
uint32_t hmac_stop_chba(hmac_vap_stru *hmac_vap)
{
    hmac_chba_vap_stru *chba_vap_info = &g_chba_vap_info;
    fast_conn_rsp_info_stru *fast_conn_rsp_info = &(chba_vap_info->fast_conn_rsp_info);

    if (hmac_vap->st_vap_base_info.chba_mode != CHBA_MODE) {
        return OAL_SUCC;
    }

    /* chba去使能, 停止全部chba定时器 */
    if (fast_conn_rsp_info->assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(fast_conn_rsp_info->assoc_waiting_timer));
    }

    hmac_chba_whitelist_clear();
    hmac_chba_assoc_priv_ie_list_clear();
    /* 全局变量清空 */
    memset_s(chba_vap_info, sizeof(hmac_chba_vap_stru), 0, sizeof(hmac_chba_vap_stru));
    if (hmac_chba_get_sync_state() >= CHBA_INIT) {
        hmac_chba_set_sync_state(CHBA_INIT);
    }

    /* 通知HAL层 */
    hmac_chba_vap_stop();

    oam_warning_log1(0, 0, "hmac_stop_chba::chba_state is %d now.", hmac_chba_get_sync_state());

    return OAL_SUCC;
}
/*
 * 函 数 名  : hmac_chba_reset_device_mgmt_info
 * 功能描述  : CHBA 重置设备管理信息
 * 1.日    期  : 2021年08月21日
 *   作    者  : wifi7
 *   修改内容  : 新生成函数
 */
static void hmac_chba_reset_device_mgmt_info(hmac_vap_stru *hmac_vap, uint8_t *own_addr)
{
    uint8_t device_id;
    mac_chba_self_conn_info_stru *self_conn_info = hmac_chba_get_self_conn_info();
    hmac_chba_per_device_id_info_stru *device_id_info = hmac_chba_get_device_id_info();

    /* 清空设备管理信息 */
    hmac_chba_clear_device_mgmt_info();

    /* 获取自身的device_id */
    device_id = hmac_chba_one_dev_update_alive_dev_table(own_addr);
    if (device_id >= MAC_CHBA_MAX_DEVICE_NUM) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_reset_device_mgmt_info:device_id[%d], max_dev_num[%d]!}", device_id, MAC_CHBA_MAX_DEVICE_NUM);
        return;
    }

    /* 在设备管理信息中更新自身的信息 */
    self_conn_info->self_device_id = device_id;
    device_id_info[device_id].use_flag = TRUE;
    if (memcpy_s(device_id_info[device_id].mac_addr, sizeof(uint8_t) * OAL_MAC_ADDR_LEN, own_addr,
        sizeof(uint8_t) * OAL_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_chba_reset_device_mgmt_info: memcpy_s fail!}");
    }
}
/*
 * 函 数 名  : hmac_chba_cur_master_info_init
 * 功能描述  : CHBA 初始化cur_master_info
 * 1.日    期  : 2021年11月14日
 *   作    者  : wifi7
 *   修改内容  : 新生成函数
 */
void hmac_chba_cur_master_info_init(uint8_t *own_mac_addr, mac_channel_stru *work_channel)
{
    hmac_chba_sync_info *sync_info = hmac_chba_get_sync_info();

    if (own_mac_addr == NULL || work_channel == NULL) {
        oam_error_log0(0, OAM_SF_CHBA, "{hmac_chba_cur_master_info_init:ptr is null!}");
        return;
    }

    sync_info->cur_master_info.master_rp = sync_info->own_rp_info;
    sync_info->cur_master_info.master_work_channel = work_channel->uc_chan_number;
    oal_set_mac_addr(sync_info->cur_master_info.master_addr, own_mac_addr);
}
/*
 * 函 数 名  : hmac_chba_reset_sync_info
 * 功能描述  : CHBA 重置同步模块
 * 1.日    期  : 2021年08月21日
 *   作    者  : wifi7
 *   修改内容  : 新生成函数
 */
static void hmac_chba_reset_sync_info(hmac_vap_stru *hmac_vap)
{
    /* 还原CHBA同步模块 */
    hmac_chba_sync_module_deinit();

    /* 设置为非同步状态 */
    hmac_chba_set_sync_state(CHBA_NON_SYNC);

    /* 设置为slave设备 */
    hmac_chba_set_role(CHBA_OTHER_DEVICE);

    /* 将当前Master信息还原为自身 */
    hmac_chba_cur_master_info_init(mac_mib_get_StationID(&hmac_vap->st_vap_base_info),
        &hmac_vap->st_vap_base_info.st_channel);
}
/*
 * 函 数 名  : hmac_chba_reset_chba_vap_info
 * 功能描述  : CHBA 重置hmac_chba_vap_stru数据结构
 * 1.日    期  : 2021年10月31日
 *   作    者  : wifi7
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_chba_reset_chba_vap_info(hmac_vap_stru *hmac_vap, hmac_chba_vap_stru *chba_vap_info)
{
    uint8_t auto_bitmap_switch_tmp = chba_vap_info->auto_bitmap_switch;
    uint8_t mac_vap_id_tmp = chba_vap_info->mac_vap_id;
    uint8_t is_support_multiple_island = chba_vap_info->is_support_multiple_island;
    mac_channel_stru social_channel_tmp = chba_vap_info->social_channel;
    fast_conn_rsp_info_stru fast_conn_rsp_info_temp = chba_vap_info->fast_conn_rsp_info;

    /* 清空chba_vap_info */
    memset_s(chba_vap_info, sizeof(hmac_chba_vap_stru), 0, sizeof(hmac_chba_vap_stru));

    /* 还原基本配置 */
    chba_vap_info->auto_bitmap_switch = auto_bitmap_switch_tmp;
    chba_vap_info->mac_vap_id = mac_vap_id_tmp;
    chba_vap_info->social_channel = social_channel_tmp;
    chba_vap_info->work_channel = hmac_vap->st_vap_base_info.st_channel;
    chba_vap_info->is_support_multiple_island = is_support_multiple_island;
    /* 初始化信道序列为全1 */
    hmac_chba_set_channel_seq_bitmap(chba_vap_info, CHBA_CHANNEL_SEQ_BITMAP_100_PERCENT);
    chba_vap_info->fast_conn_rsp_info = fast_conn_rsp_info_temp;

    /* 重置白名单 */
    hmac_chba_whitelist_clear();
    hmac_chba_whitelist_init();
    /* 极速连接响应方连接过程中, 即便user被删除也并不一定意味着连接失败, 需重新添加极速连接mac地址等待下一次assoc */
    if (hmac_chba_is_not_verified_fast_connect_response(chba_vap_info) == OAL_TRUE) {
        hmac_chba_whitelist_add_user(BROADCAST_MACADDR, OAL_MAC_ADDR_LEN);
    }
}

/*
 * 功能描述  : 暂停chba vap调度
 * 日    期  : 2021年08月28日
 * 作    者  : wifi7
 */
static void hmac_chba_vap_stop_schedule(hmac_vap_stru *hmac_vap)
{
    uint32_t ret;
    uint8_t *own_addr = NULL;
    hmac_chba_vap_stru *chba_vap_info = NULL;
    chba_params_config_stru chba_params_sync;

    chba_vap_info = hmac_get_up_chba_vap_info();
    if (chba_vap_info == NULL) {
        return;
    }

    own_addr = mac_mib_get_StationID(&hmac_vap->st_vap_base_info);
    if (own_addr == NULL) {
        return;
    }

    /* 还原同步相关标记位 */
    hmac_chba_reset_sync_info(hmac_vap);

    /* 还原设备管理信息 */
    hmac_chba_reset_device_mgmt_info(hmac_vap, own_addr);

    /* 将bssid还原为自身的mac addr */
    hmac_chba_generate_domain_bssid(hmac_chba_sync_get_domain_bssid(), own_addr, OAL_MAC_ADDR_LEN);

    /* 刷新beacon/pnf/assoc等帧的相关字段 */
    hmac_chba_save_update_beacon_pnf(hmac_chba_sync_get_domain_bssid());

    /* 重置hmac_chba_vap_stru数据结构 */
    hmac_chba_reset_chba_vap_info(hmac_vap, chba_vap_info);

    /* 将更新后的状态同步给dmac */
    memset_s(&chba_params_sync, sizeof(chba_params_sync), 0, sizeof(chba_params_sync));
    chba_params_sync.chba_role = hmac_chba_get_role();
    chba_params_sync.chba_state = hmac_chba_get_sync_state();
    oal_set_mac_addr(chba_params_sync.domain_bssid, hmac_chba_sync_get_domain_bssid());
    chba_params_sync.info_bitmap = CHBA_STATE | CHBA_ROLE | CHBA_BSSID | CHBA_BEACON_BUF |
        CHBA_BEACON_LEN | CHBA_PNF_BUF | CHBA_PNF_LEN | CHBA_MASTER_INFO;
    ret = hmac_chba_params_sync(&hmac_vap->st_vap_base_info, &chba_params_sync);
    if (ret != OAL_SUCC) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_vap_stop_schedule:fail to send params to dmac![%d]}", ret);
        return;
    }
    oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{hmac_chba_vap_stop_schedule:CHBA schedule stop!}");
}

/*
 * 功能描述  : 更新chba vap相关信息
 * 日    期  : 2021年02月08日
 * 作    者  : wifi
 */
void hmac_update_chba_vap_info(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_chba_vap_stru *chba_vap_info = NULL;

    if (hmac_vap == NULL || hmac_user == NULL) {
        return;
    }

    chba_vap_info = hmac_chba_get_chba_vap_info(hmac_vap);
    if (chba_vap_info == NULL) {
        return;
    }

    /* 更新完拓扑信息之后，同步更新Beacon和PNF */
    // 等待user从链表中摘除后在更新beacon/PNF，否则link info仍携带该删链设备
    hmac_chba_save_update_beacon_pnf(hmac_chba_sync_get_domain_bssid());
    /* 删除链路时删除chba assoc_waiting_timer定时器 */
    if (hmac_user->chba_user.connect_info.assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(hmac_user->chba_user.connect_info.assoc_waiting_timer));
    }

    /* 无链路场景下暂停chba业务 */
    if (hmac_vap->st_vap_base_info.us_user_nums == 0) {
        hmac_chba_vap_stop_schedule(hmac_vap);
    }

    oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{hmac_update_chba_vap_info:del chab user, link_up_user_cnt is[%d]}", hmac_vap->st_vap_base_info.us_user_nums);
}

/*
 * 功能描述  : 更新bssid到Dmac
 * 日    期  : 2021年03月07日
 * 作    者  : wifi
 */
void hmac_chba_h2d_update_bssid(mac_vap_stru *mac_vap, uint8_t *domain_bssid, uint8_t domain_bssid_len)
{
    chba_params_config_stru chba_params_sync = { 0 };
    uint32_t ret;

    if (oal_any_null_ptr2(mac_vap, domain_bssid)) {
        return;
    }

    oal_set_mac_addr(chba_params_sync.domain_bssid, domain_bssid);
    chba_params_sync.info_bitmap = CHBA_BSSID;
    oam_warning_log4(0, 0, "hmac_chba_h2d_update_bssid::send bssid(%02X:XX:XX:XX:%02X:%02X) to dmac, bitmap 0x%X.",
        chba_params_sync.domain_bssid[MAC_ADDR_0], chba_params_sync.domain_bssid[MAC_ADDR_4],
        chba_params_sync.domain_bssid[MAC_ADDR_5], chba_params_sync.info_bitmap);
    ret = hmac_chba_params_sync(mac_vap, &chba_params_sync);
    if (ret != OAL_SUCC) {
        return;
    }
}

/*
 * 功能描述  : 启动一次主动请求
 * 日    期  : 2021年03月07日
 * 作    者  : wifi
 */
uint32_t hmac_chba_request_sync(mac_vap_stru *mac_vap, hmac_chba_vap_stru *chba_vap_info,
    mac_chba_set_sync_request *sync_req_params)
{
    request_sync_info_stru *request_sync_info = NULL;
    sync_peer_info *peer_info = NULL;
    hmac_chba_sync_info *sync_info = hmac_chba_get_sync_info();
    uint8_t domain_bssid[WLAN_MAC_ADDR_LEN];
    int32_t err;

    if (oal_any_null_ptr3(mac_vap, chba_vap_info, sync_req_params)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    request_sync_info = &(sync_info->request_sync_info);
    peer_info = &sync_req_params->request_peer;
    err = (int32_t)mac_is_channel_num_valid(chba_vap_info->work_channel.en_band, peer_info->chan_num, OAL_FALSE);
    /* 判断下发的参数是否有效 */
    if ((sync_req_params->mgmt_type != MAC_CHBA_SYNC_REQUEST) ||
        (sync_req_params->mgmt_buf_len == 0) || (err != OAL_SUCC)) {
        oam_warning_log3(0, 0,
            "hmac_chba_request_sync::invalid sync request, mgmt type[%d], buf len [%d], peer channel number [%d].",
            sync_req_params->mgmt_type, sync_req_params->mgmt_buf_len, peer_info->chan_num);
        return OAL_FAIL;
    }

    /* 停掉之前的sync request */
    if (sync_info->sync_request_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(sync_info->sync_request_timer));
    }
    request_sync_info->request_cnt = 0;
    if (memcpy_s(request_sync_info->sync_req_buf, MAC_CHBA_MGMT_SHORT_PAYLOAD_BYTES,
        sync_req_params->mgmt_buf, sync_req_params->mgmt_buf_len) != EOK) {
        return OAL_FAIL;
    }
    request_sync_info->sync_req_buf_len = sync_req_params->mgmt_buf_len;
    oal_set_mac_addr(request_sync_info->peer_addr, peer_info->peer_addr);

    /* 生成新的域BSSID */
    hmac_generate_chba_domain_bssid(domain_bssid, WLAN_MAC_ADDR_LEN,
        peer_info->master_addr, WLAN_MAC_ADDR_LEN);
    /*  若开启硬件过滤, 其他bssid的广播帧将无法收到, 因此现暂定在请求同步前更新一次bssid
        此处根据后续方案决定是否保留 */
    mac_vap_set_bssid(mac_vap, domain_bssid, WLAN_MAC_ADDR_LEN);
    hmac_chba_h2d_update_bssid(mac_vap, domain_bssid, sizeof(domain_bssid));

    /* 封装并发送sync request action帧 */
    oam_warning_log3(0, 0, "hmac_chba_request_sync::send sync request action, peer addr is %02X:XX:XX:XXX:%02X:%02X.",
        peer_info->peer_addr[MAC_ADDR_0], peer_info->peer_addr[MAC_ADDR_4], peer_info->peer_addr[MAC_ADDR_5]);
    hmac_send_chba_sync_request_action(mac_vap, chba_vap_info, peer_info->peer_addr,
        request_sync_info->sync_req_buf, request_sync_info->sync_req_buf_len);

    /* 启动Sync Waiting定时器 */
    frw_timer_create_timer_m(&(sync_info->sync_request_timer),
        hmac_chba_sync_waiting_timeout, MAC_CHBA_SYNC_WAITING_TIMEOUT,
        chba_vap_info, OAL_FALSE, OAM_MODULE_ID_HMAC, mac_vap->core_id);

    return OAL_SUCC;
}

/*
 * 功能描述  : Sync Waiting定时器到期处理函数
 * 日    期  : 2021年03月10日
 * 作    者  : wifi
 */
uint32_t hmac_chba_sync_waiting_timeout(void *arg)
{
    hmac_chba_vap_stru *chba_vap_info = NULL;
    mac_vap_stru *mac_vap = NULL;
    request_sync_info_stru *request_sync_info = NULL;
    hmac_chba_sync_info *sync_info = hmac_chba_get_sync_info();
    uint32_t ret;

    chba_vap_info = (hmac_chba_vap_stru *)arg;
    if (chba_vap_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(chba_vap_info->mac_vap_id);
    if (mac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    request_sync_info = &(sync_info->request_sync_info);

    request_sync_info->request_cnt++;
    /* 如果已经发送完三次，则退出 */
    if (request_sync_info->request_cnt >= MAC_CHBA_SYNC_REQUEST_CNT) {
        return OAL_SUCC;
    }

    /* 如果已经同步，则退出 */
    if ((hmac_chba_get_sync_state() > CHBA_NON_SYNC) &&
        (hmac_chba_get_sync_request_flag(&sync_info->sync_flags) == FALSE)) {
        oam_warning_log0(0, 0, "hmac_chba_sync_waiting_timeout::sync waitint timer timeout, but already sync.");
        return OAL_SUCC;
    }

    /* 再次发送sync request action帧 */
    oam_warning_log3(0, 0, "hmac_chba_sync_waiting_timeout::send sync req action, peer addr %02X:XX:XX:XX:%02X:%02X.",
        request_sync_info->peer_addr[MAC_ADDR_0], request_sync_info->peer_addr[MAC_ADDR_4],
        request_sync_info->peer_addr[MAC_ADDR_5]);
    ret = hmac_send_chba_sync_request_action(mac_vap, chba_vap_info, request_sync_info->peer_addr,
        request_sync_info->sync_req_buf, request_sync_info->sync_req_buf_len);
    if (ret != OAL_SUCC) {
        return ret;
    }

    /* 启动Sync Waiting定时器, 每间隔16ms重发一次同步请求 */
    frw_timer_create_timer_m(&(sync_info->sync_request_timer),
        hmac_chba_sync_waiting_timeout, MAC_CHBA_SLOT_DURATION,
        chba_vap_info, OAL_FALSE, OAM_MODULE_ID_HMAC, mac_vap->core_id);

    return OAL_SUCC;
}

/* 功能描述: 更新岛内最大rp值设备信息 */
static void hmac_chba_update_master_election_info(hmac_vap_stru *hmac_vap, hmac_chba_sync_info *sync_info,
    master_info *new_max_rp_device)
{
    int32_t err;
    master_info *cur_max_rp_device = &(sync_info->max_rp_device);

    err = memcpy_s(cur_max_rp_device, sizeof(master_info), new_max_rp_device, sizeof(master_info));
    if (err != EOK) {
        oam_error_log1(0, OAM_SF_CHBA, "hmac_chba_update_master_election_info::memcpy failed, err[%d].", err);
        return;
    }
    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{hmac_chba_update_master_election_info:mac_addr[%02x:XX:XX:XX:%02x:%02x]}",
        new_max_rp_device->master_addr[MAC_ADDR_0], new_max_rp_device->master_addr[MAC_ADDR_4],
        new_max_rp_device->master_addr[MAC_ADDR_5]);
}
/*
 * 功能描述  : dmac上报同步时隙开始或结束信息
 * 日    期  : 2021年03月27日
 * 作    者  : wifi
 */
uint32_t hmac_chba_d2h_sync_event(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param)
{
    hmac_chba_sync_info *sync_info = hmac_chba_get_sync_info();
    hmac_vap_stru *hmac_vap = NULL;
    mac_chba_d2h_sync_event_info_stru *sync_event_info = (mac_chba_d2h_sync_event_info_stru *)param;

    if (mac_vap == NULL || param == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_chba_vap_start_check(hmac_vap) == OAL_FALSE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CHBA, "{hmac_chba_d2h_sync_event: CHBA vap is not start, return}");
        return OAL_FAIL;
    }

    /* 调度已停止, 不再处理上报 */
    if (mac_vap->us_user_nums == 0) {
        return OAL_SUCC;
    }

    switch (sync_event_info->sync_event) {
        case CHBA_UPDATE_TOPO_INFO:
            hmac_chba_update_topo_info_proc(hmac_vap);
            break;
        case CHBA_NON_SYNC_START_SYNC_REQ:
            hmac_chba_start_sync_request(sync_info);
            break;
        case CHBA_RP_CHANGE_START_MASTER_ELECTION:
            /* master选举信息发生变化直接进行master选举 */
            hmac_chba_update_master_election_info(hmac_vap, sync_info, &(sync_event_info->max_rp_dev_info));
            hmac_chba_sync_master_election_proc(hmac_vap, sync_info, MASTER_ALIVE);
            break;
        case CHBA_NON_SYNC_START_MASTER_ELECTION:
        case CHBA_DOMAIN_SEQARATE_START_MASTER_ELECTION:
            /* 域拆分或失同步等原因失去旧master, 重新进行主设备选举 */
            hmac_chba_update_master_election_info(hmac_vap, sync_info, &(sync_event_info->max_rp_dev_info));
            hmac_chba_sync_master_election_proc(hmac_vap, sync_info, MASTER_LOST);
            break;
        default:
            break;
    }
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_chba_d2h_sync_event:sync_event[%d]}",
        sync_event_info->sync_event);
    return OAL_SUCC;
}
static void hmac_config_connect_update_app_ie_to_vap(mac_vap_stru *mac_vap, hmac_conn_param_stru *connect_param)
{
    oal_app_ie_stru app_ie;
    int32_t ret;
    /* BEGIN: WLAN发送的 assoc request 不携带P2P IE */
    if (is_legacy_vap(mac_vap)) {
        hmac_config_del_p2p_ie(connect_param->puc_ie, &(connect_param->ie_len));
    }
    /* END: WLAN发送的 assoc request 不携带P2P IE */
    app_ie.ie_len = connect_param->ie_len;
    ret = memcpy_s(app_ie.auc_ie, WLAN_WPS_IE_MAX_SIZE, connect_param->puc_ie, app_ie.ie_len);
    if (ret != EOK) {
        oam_error_log2(mac_vap->uc_vap_id, OAM_SF_ASSOC, "hmac_config_connect_update_app_ie_to_vap::copy ie fail.",
            connect_param->ie_len, ret);
    }
    app_ie.en_app_ie_type = OAL_APP_ASSOC_REQ_IE;
    if (hmac_config_set_app_ie_to_vap(mac_vap, &app_ie, app_ie.en_app_ie_type) != OAL_SUCC) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_connect_update_app_ie_to_vap::set_app_ie FAIL}");
    }
}

OAL_STATIC void hmac_chba_initiate_connect_start(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_asoc_req_stru assoc_req;
    hmac_auth_req_stru auth_req;

    /* 发起端简易连接跳至auth complete阶段, 直接发起assoc req. 包括chba2.0极速连接，以及chba并行连接 */
    if (hmac_chba_is_fast_connect_initiate(hmac_user) == OAL_TRUE) {
        hmac_chba_connect_initiator_fsm_change_state(hmac_user, CHBA_USER_STATE_AUTH_COMPLETE);
        assoc_req.us_assoc_timeout =
            (uint16_t)mac_mib_get_dot11AssociationResponseTimeOut(&(hmac_vap->st_vap_base_info)) >> 1;
        hmac_fsm_call_func_chba_conn_initiator(hmac_vap, hmac_user, HMAC_FSM_INPUT_ASOC_REQ, (void *)&assoc_req);
    } else {
        /* 普通连接external_auth上报，然后等待上层发起auth流程 */
        hmac_chba_connect_initiator_fsm_change_state(hmac_user, CHBA_USER_STATE_JOIN_COMPLETE);
        hmac_prepare_auth_req(hmac_vap, &auth_req);
        /* 调用函数 hmac_chba_wait_auth */
        hmac_fsm_call_func_chba_conn_initiator(hmac_vap, hmac_user, HMAC_FSM_INPUT_AUTH_REQ, (void *)&auth_req);
    }

    oam_warning_log1(0, OAM_SF_CHBA, "hmac_chba_initiate_connect_start::initiate connect. conn_flag[%d]",
        hmac_user->chba_user.connect_info.conn_flag);
}
/*
 * 功能描述  : 连接方发起建链
 * 日    期  : 2021年02月08日
 * 作    者  : wifi
 */
uint32_t hmac_chba_initiate_connect(mac_vap_stru *mac_vap, uint16_t len, uint8_t *params)
{
    hmac_conn_param_stru *connect_params = NULL;
    hmac_chba_conn_param_stru chba_conn_params;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    uint16_t user_idx;
    uint32_t ret;

    connect_params = (hmac_conn_param_stru *)params;
    if (connect_params->ie_len > WLAN_WPS_IE_MAX_SIZE) {
        oam_error_log1(0, 0, "hmac_chba_initiate_connect::invalid config, ie_len[%x] err!", connect_params->ie_len);
        hmac_free_connect_param_resource(connect_params);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        hmac_free_connect_param_resource(connect_params);
        return OAL_FAIL;
    }

    hmac_config_connect_update_app_ie_to_vap(mac_vap, connect_params);

    /* 根据下发参数填写chba_conn_params */
    hmac_chba_fill_connect_params(params, &chba_conn_params, CHBA_CONN_INITIATOR);

    /* 根据下发的安全参数，配置vap */
    ret = hmac_chba_cfg_security_params(&hmac_vap->st_vap_base_info, connect_params);
    if (ret != OAL_SUCC) {
        hmac_free_connect_param_resource(connect_params);
        hmac_chba_fail_to_connect(hmac_vap, hmac_user, &chba_conn_params, CHBA_CONN_INITIATOR);
        oam_warning_log1(0, 0, "hmac_chba_initiate_connect::fail to config security params[%d].", ret);
        return ret;
    }
    hmac_free_connect_param_resource(connect_params);

    /* 检查是否满足建链条件 */
    if (hmac_chba_connect_check(mac_vap, hmac_vap, &chba_conn_params, CHBA_CONN_INITIATOR) != OAL_SUCC ||
        hmac_chba_connect_prepare(mac_vap, &chba_conn_params, &user_idx, CHBA_CONN_INITIATOR) != OAL_SUCC) {
        hmac_chba_fail_to_connect(hmac_vap, hmac_user, &chba_conn_params, CHBA_CONN_INITIATOR);
        oam_warning_log0(0, 0, "hmac_chba_initiate_connect::fail to connect");
        return OAL_FAIL;
    }

    /* 在建链准备完成之后，不允许再使用hmac_chba_fail_to_connect来上报失败 */
    hmac_user = mac_res_get_hmac_user(user_idx);
    if (hmac_user == NULL) {
        return OAL_FAIL;
    }

    hmac_chba_user_set_ssid(hmac_user, connect_params->auc_ssid, connect_params->uc_ssid_len);
    hmac_chba_initiate_connect_start(hmac_vap, hmac_user);
    return OAL_SUCC;
}

/* 功能描述: chba极速连接响应方事件处理完成操作 */
uint32_t hmac_chba_not_verified_fast_conn_rsp_finish_proc(void *arg)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_chba_vap_stru *chba_vap_info = NULL;
    fast_conn_rsp_info_stru *fast_conn_rsp_info = NULL;

    hmac_vap = (hmac_vap_stru *)arg;
    if (hmac_chba_vap_start_check(hmac_vap) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_CHBA, "{hmac_chba_not_verified_fast_conn_rsp_finish_proc:fail to find hmac_vap}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    chba_vap_info = hmac_vap->hmac_chba_vap_info;
    fast_conn_rsp_info = &(chba_vap_info->fast_conn_rsp_info);

    /* 清空白名单 */
    hmac_chba_whitelist_del_user(BROADCAST_MACADDR, OAL_MAC_ADDR_LEN);
    /* responder即便超时连接失败，也不需要上报建链失败，只恢复建链前的状态 */
    if (fast_conn_rsp_info->assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(fast_conn_rsp_info->assoc_waiting_timer));
    }

    /* 清除该极速连接响应端对应信息 */
    hmac_chba_clear_one_fast_conn_rsp_info(hmac_vap, chba_vap_info, NULL);
    return OAL_SUCC;
}

/* 功能描述: chba连接响应方启动等待连接定时 */
OAL_STATIC uint32_t hmac_chba_response_connect_start(hmac_vap_stru *hmac_vap,
    hmac_chba_conn_param_stru *chba_conn_params, uint16_t user_idx)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_chba_vap_stru *chba_vap_info = NULL;
    fast_conn_rsp_info_stru *fast_conn_rsp_info = NULL;
    uint32_t waiting_time = chba_conn_params->connect_timeout * HMAC_S_TO_MS;

    /* 通知上层ready to connect */
    chba_conn_params->status_code = MAC_SUCCESSFUL_STATUSCODE;
    hmac_chba_ready_to_connect(hmac_vap, chba_conn_params);

    if ((hmac_chba_check_fast_connect_flag(chba_conn_params->conn_flag) == OAL_TRUE) &&
        (oal_compare_mac_addr(BROADCAST_MACADDR, chba_conn_params->peer_addr) == 0)) {
        chba_vap_info = hmac_vap->hmac_chba_vap_info;
        fast_conn_rsp_info = &(chba_vap_info->fast_conn_rsp_info);
        /* 启动assoc--端口开启 waiting定时器 */
        frw_timer_create_timer_m(&(fast_conn_rsp_info->assoc_waiting_timer),
            hmac_chba_not_verified_fast_conn_rsp_finish_proc, waiting_time,
            hmac_vap, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
    } else {
        hmac_user = mac_res_get_hmac_user(user_idx);
        if (hmac_user == NULL) {
            oam_warning_log1(0, 0, "hmac_chba_response_connect_start:hmac user is null, user_idx[%d]", user_idx);
            return OAL_FAIL;
        }
        hmac_user->chba_user.connect_info.assoc_waiting_time = waiting_time;
        /* 启动assoc waiting定时器 */
        frw_timer_create_timer_m(&(hmac_user->chba_user.connect_info.assoc_waiting_timer),
            hmac_chba_assoc_waiting_timeout_responder, hmac_user->chba_user.connect_info.assoc_waiting_time,
            hmac_user, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
    }
    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
        "{hmac_chba_response_connect_start:response connect, fast_conn[%d], wait_time[%d]ms}",
        chba_conn_params->conn_flag, waiting_time);
    return OAL_SUCC;
}

/* 功能描述: ConnectNotify命令处理接口 */
uint32_t hmac_chba_response_connect(mac_vap_stru *mac_vap, uint16_t len, uint8_t *params)
{
    hmac_chba_conn_param_stru chba_conn_params;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    uint16_t user_idx;
    uint32_t ret;
#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
    hmac_nbw_current_connect_info_stru *nbw_curr_conn_info = NULL;
#endif

    if (oal_any_null_ptr2(mac_vap, params)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_FAIL;
    }

    /* 根据下发参数填写chba_conn_params */
    ret = hmac_chba_fill_connect_params(params, &chba_conn_params, CHBA_CONN_RESPONDER);
    if (ret != OAL_SUCC) {
        hmac_chba_fail_to_connect(hmac_vap, hmac_user, &chba_conn_params, CHBA_CONN_RESPONDER);
        return OAL_FAIL;
    }

    /* 检查是否满足建链条件 */
    ret = hmac_chba_connect_check(mac_vap, hmac_vap, &chba_conn_params, CHBA_CONN_RESPONDER);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, 0, "hmac_chba_response_connect::fail to connect");
        hmac_chba_fail_to_connect(hmac_vap, hmac_user, &chba_conn_params, CHBA_CONN_RESPONDER);
        return OAL_FAIL;
    }

    /* 建链准备 */
    ret = hmac_chba_connect_prepare(mac_vap, &chba_conn_params, &user_idx, CHBA_CONN_RESPONDER);
    if (ret != OAL_SUCC) {
        hmac_chba_fail_to_connect(hmac_vap, hmac_user, &chba_conn_params, CHBA_CONN_RESPONDER);
        return ret;
    }

    /* 保存chba连接私有app ie */
    ret = hmac_chba_save_assoc_priv_ie(hmac_vap, &chba_conn_params);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, 0, "hmac_chba_response_connect::fail to save priv data, ret[%d]", ret);
        hmac_chba_fail_to_connect(hmac_vap, hmac_user, &chba_conn_params, CHBA_CONN_RESPONDER);
        return ret;
    }

#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
    if (mac_vap->nbw_mode == NBW_MODE) {
        /* 启动信道监听定时器，在2个周期内接收beacon，用于选举master */
        nbw_curr_conn_info = hmac_nbw_get_curr_conn_info();
        frw_timer_create_timer_m(&(nbw_curr_conn_info->assoc_waiting_timer), hmac_nbw_master_election_finish_proc,
            WAITTING_TWO_DURATION_MS, mac_vap, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
        oam_error_log0(0, 0, "NBW: hmac_chba_response_connect::start listen channel and select master.");
    }
#endif

    return hmac_chba_response_connect_start(hmac_vap, &chba_conn_params, user_idx);
}

/* 功能描述: 获取极速连接发起方连接模式flags */
OAL_STATIC uint8_t hmac_chba_get_connect_initiator_conn_flag(uint32_t flags)
{
    uint8_t conn_flag = (uint8_t)(flags >> CHBA_CONN_INITIATOR_FLAG_OFFSET); /* flags bit30与bit31为conn_flag */
    return ((hmac_chba_is_support_fast_connect() == OAL_TRUE) ? conn_flag : CHBA_NORMAL_CONNECT_FLAG);
}

/* 功能描述: 获取极速连接响应方连接模式flags */
OAL_STATIC uint8_t hmac_chba_get_connect_notify_conn_flag(uint8_t flags)
{
    return ((hmac_chba_is_support_fast_connect() == OAL_TRUE) ? flags : CHBA_NORMAL_CONNECT_FLAG);
}
/*
 * 功能描述  : 根据下发的connect参数填写chba_connect_params结构体
 * 日    期  : 2021年02月08日
 * 作    者  : wifi
 */
uint32_t hmac_chba_fill_connect_params(uint8_t *params, hmac_chba_conn_param_stru *chba_conn_params,
    uint8_t connect_role)
{
    hmac_conn_param_stru *connect_params = NULL;
    hmac_chba_conn_notify_stru *notify_params = NULL;
    uint32_t ret;

    if (oal_any_null_ptr2(params, chba_conn_params)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (connect_role == CHBA_CONN_INITIATOR) {
        connect_params = (hmac_conn_param_stru *)params;
        chba_conn_params->status_code = MAC_CHBA_INIT_CODE;
        oal_set_mac_addr(chba_conn_params->peer_addr, connect_params->auc_bssid);
        chba_conn_params->conn_flag = hmac_chba_get_connect_initiator_conn_flag(connect_params->flags);
#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
        /* nbw初始化当前建链参数信息，发起方保存建链参数 */
        hmac_nbw_save_curr_conn_info(chba_conn_params, connect_role);
#endif
        return OAL_SUCC;
    }

    notify_params = (hmac_chba_conn_notify_stru *)params;
    chba_conn_params->op_id = notify_params->id;
    chba_conn_params->status_code = MAC_CHBA_INIT_CODE;

    oal_set_mac_addr(chba_conn_params->peer_addr, notify_params->peer_mac_addr);
    ret = hmac_chba_chan_convert_center_freq_to_bw(&(notify_params->channel),
        &(chba_conn_params->assoc_channel));
    if (ret != OAL_SUCC) {
        chba_conn_params->status_code = MAC_CHBA_INVAILD_CONNECT_CMD;
        return OAL_FAIL;
    }
    chba_conn_params->connect_timeout = notify_params->expire_time;
    chba_conn_params->conn_flag = hmac_chba_get_connect_notify_conn_flag(notify_params->conn_flag);
    chba_conn_params->assoc_priv_ie_len = notify_params->assoc_priv_ie_len;
    memcpy_s(chba_conn_params->assoc_priv_ie, HMAC_CHBA_USER_ASSOC_PRIV_IE_MAX_LEN,
        notify_params->assoc_priv_ie, HMAC_CHBA_USER_ASSOC_PRIV_IE_MAX_LEN);
    oam_warning_log3(0, 0, "hmac_chba_fill_connect_params::responder, peer_addr %02X:XX:XX:XX:%02X:%02X",
        chba_conn_params->peer_addr[MAC_ADDR_0], chba_conn_params->peer_addr[MAC_ADDR_4],
        chba_conn_params->peer_addr[MAC_ADDR_5]);

#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
    /* nbw初始化当前建链参数信息，响应方保存建链参数 */
    hmac_nbw_save_curr_conn_info(chba_conn_params, connect_role);
#endif

    return OAL_SUCC;
}

/*
 * 功能描述  : chba建链条件check
 * 日    期  : 2021年02月08日
 * 作    者  : wifi
 */
static uint32_t hmac_chba_connect_check(mac_vap_stru *mac_vap, hmac_vap_stru *hmac_vap,
    hmac_chba_conn_param_stru *chba_conn_params, uint8_t connect_role)
{
    hmac_chba_vap_stru *chba_vap_info = NULL;

    if (oal_any_null_ptr3(mac_vap, hmac_vap, chba_conn_params)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if (chba_vap_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 检查chba vap链路数是否达到上限 */
    if (mac_vap->us_user_nums >= MAC_CHBA_MAX_LINK_NUM) {
        chba_conn_params->status_code = MAC_CHBA_CREATE_NEW_USER_FAIL;
        return OAL_FAIL;
    }

    /* 极速连接模式允许并发 */
    if ((hmac_chba_check_fast_connect_flag(chba_conn_params->conn_flag) == OAL_TRUE) &&
        (connect_role == CHBA_CONN_RESPONDER)) {
        return OAL_SUCC;
    }

    /* 并行建链存在较多问题，暂不接受并行建链 */
    /* 响应方极速连接状态时vap下不一定存在新增user，但此时也应被视为处于建链流程中，不可再重复发起建链了 */
    if (hmac_vap_is_connecting(mac_vap) ||
        (hmac_chba_is_fast_connect_response(hmac_vap))) {
        chba_conn_params->status_code = MAC_CHBA_UNSUP_PARALLEL_CONNECT;
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 功能描述  : 根据下发的安全参数，配置vap的安全信息
 * 日    期  : 2021年02月08日
 * 作    者  : wifi
 */
static uint32_t hmac_chba_cfg_security_params(mac_vap_stru *mac_vap, hmac_conn_param_stru *connect_params)
{
    mac_conn_security_stru conn_sec;
    uint32_t ret;
    mac_ies_info ies_info = {0};

    ies_info.frame_ies = connect_params->puc_ie;
    ies_info.frame_len = connect_params->ie_len;
    memset_s(&conn_sec, sizeof(mac_conn_security_stru), 0, sizeof(mac_conn_security_stru));
    conn_sec.uc_wep_key_len = connect_params->uc_wep_key_len;
    conn_sec.en_privacy = connect_params->en_privacy;
    conn_sec.st_crypto = connect_params->st_crypto;
    conn_sec.uc_wep_key_index = connect_params->uc_wep_key_index;
    conn_sec.uc_wep_key_len = oal_min(connect_params->uc_wep_key_len, WLAN_WEP104_KEY_LEN);
    mac_mib_set_AuthenticationMode(mac_vap, connect_params->en_auth_type);
    if (connect_params->uc_wep_key_len > 0) {
        if (memcpy_s(conn_sec.auc_wep_key, conn_sec.uc_wep_key_len, connect_params->puc_wep_key,
            connect_params->uc_wep_key_len) != EOK) {
            hmac_free_connect_param_resource(connect_params);
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }
    conn_sec.en_mgmt_proteced = connect_params->en_mfp;
#ifdef _PRE_WLAN_FEATURE_PMF
    conn_sec.en_pmf_cap = mac_get_pmf_cap(connect_params->puc_ie, connect_params->ie_len);
#endif
    conn_sec.en_wps_enable = OAL_FALSE;
    if (mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_WLAN_OUI_TYPE_MICROSOFT_WPS, &ies_info)) {
        conn_sec.en_wps_enable = OAL_TRUE;
    }
    hmac_free_connect_param_resource(connect_params);
    ret = mac_vap_init_privacy(mac_vap, &conn_sec);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "CHBA connect: mac_vap_init_privacy fail[%d]!", ret);
        return ret;
    }
    /* 同步加密参数到dmac */
    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_CONNECT_REQ, sizeof(conn_sec), (uint8_t *)&conn_sec);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "CHBA connect: hmac_config_send_event failed[%d].", ret);
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC void hmac_chba_fast_conn_rsp_init(hmac_chba_vap_stru *chba_vap_info,
    hmac_chba_conn_param_stru *chba_conn_params)
{
    uint8_t idx;
    fast_conn_rsp_info_stru *fast_conn_rsp_info = &chba_vap_info->fast_conn_rsp_info;

    if (fast_conn_rsp_info->is_fast_conn_rsp == OAL_FALSE) {
        fast_conn_rsp_info->is_fast_conn_rsp = OAL_TRUE;
    }

    /* 极速连接响应端mac地址为全F时需配置特殊标记位not_verified_rsp_conn_mode */
    if (oal_compare_mac_addr(BROADCAST_MACADDR, chba_conn_params->peer_addr) == 0) {
        /* 特殊响应端已经在运行了 */
        if (fast_conn_rsp_info->not_verified_rsp_conn_mode != CHBA_NOT_VERIFIED_CONN_RSP_NOT_RUNNING) {
            oam_error_log0(0, OAM_SF_CHBA, "{hmac_chba_fast_conn_rsp_init:is running!");
        }
        fast_conn_rsp_info->not_verified_rsp_conn_mode =
            (hmac_chba_check_no_encry_connect_flag(chba_conn_params->conn_flag) ?
            CHBA_NOT_VERIFIED_CONN_RSP_NOT_ENCRY_CONN_MODE : CHBA_NOT_VERIFIED_CONN_RSP_ENCRY_CONN_MODE);
        for (idx = 0; idx < MAC_RES_MAX_USER_LIMIT; idx++) {
            fast_conn_rsp_info->not_verified_user_id[idx] = CHBA_NOT_VERIFIED_USER_ID_INIT;
        }
    }
    oam_warning_log1(0, OAM_SF_CHBA, "{hmac_chba_fast_conn_rsp_init:not_verified_rsp_conn_mode[%d]}",
        fast_conn_rsp_info->not_verified_rsp_conn_mode);
}

/*
 * 功能描述  : 建链准备流程
 * 日    期  : 2021年02月08日
 * 作    者  : wifi
 */
uint32_t hmac_chba_connect_prepare(mac_vap_stru *mac_vap, hmac_chba_conn_param_stru *chba_conn_params,
    uint16_t *user_idx, uint8_t connect_role)
{
    uint32_t ret;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_chba_vap_stru *chba_vap_info = NULL;

    /* 如果是建链响应者，则将peer addr添加到白名单 */
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if (chba_vap_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 极速连接响应方配置 */
    if ((hmac_chba_check_fast_connect_flag(chba_conn_params->conn_flag) == OAL_TRUE) &&
        (connect_role == CHBA_CONN_RESPONDER)) {
        hmac_chba_fast_conn_rsp_init(chba_vap_info, chba_conn_params);
    }

    /* 下发mac地址非广播时, 增加添加对应hmac_user */
    if (oal_compare_mac_addr(BROADCAST_MACADDR, chba_conn_params->peer_addr) != 0) {
        /* 创建user，并将状态设置为wait assoc */
        ret = hmac_chba_user_mgmt_conn_prepare(mac_vap, chba_conn_params, user_idx, connect_role,
            chba_conn_params->conn_flag);
        if (ret != OAL_SUCC) {
            return ret;
        }
    }
    hmac_chba_whitelist_add_user(chba_conn_params->peer_addr, OAL_MAC_ADDR_LEN);

    /* 告知dmac准备建链 唤醒device开前端 */
    hmac_config_send_event(mac_vap, WLAN_CFGID_CHBA_CONNECT_PREPARE, sizeof(uint8_t),
        &(chba_vap_info->fast_conn_rsp_info.is_fast_conn_rsp));

    /* 设置vap低功耗为最高档 */
    chba_vap_info->vap_bitmap_level = CHBA_BITMAP_ALL_AWAKE_LEVEL;
    hmac_chba_sync_vap_bitmap_info(&(hmac_vap->st_vap_base_info), chba_vap_info);
#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
    if (mac_vap->nbw_mode == NBW_MODE) {
        // TDO: nbw需要调用新接口重新配置vap bitmap, 与chba复用方式
        /*
         1. 停止使用chba配置vap bitmap方式，与chba为if else方式
         2. 调用hmac_nbw_set_uslot_bitmap先设置bitmap
         3. 再调用hmac_nbw_sync_vap_bitmap_info配置bitmap给device
        */
    }
#endif
    /* 切换到建链信道 */
    /* 如果未同步，直接切换，如果已经同步，调用vap bitmap配置函数 */
    oam_warning_log3(mac_vap->uc_vap_id, OAM_SF_CHBA,
        "hmac_chba_connect_prepare::connect channel is [%d], bandwidth is [%d], connect_role[%d].",
        chba_conn_params->assoc_channel.uc_chan_number, chba_conn_params->assoc_channel.en_bandwidth, connect_role);

    /* 入网阶段, sleep work超时延长至400ms */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, WLAN_SLEEP_LONG_CHECK_CNT);
#endif
    return OAL_SUCC;
}

/*
 * 功能描述  : 通知WPAS已经准备好建链
 * 日    期  : 2021年02月18日
 * 作    者  : wifi
 */
void hmac_chba_ready_to_connect(hmac_vap_stru *hmac_vap, hmac_chba_conn_param_stru *conn_param)
{
    if (oal_any_null_ptr2(hmac_vap, conn_param)) {
        return;
    }

    oal_cfg80211_connect_ready_report(hmac_vap->pst_net_device, conn_param->peer_addr,
        conn_param->op_id, conn_param->status_code, GFP_ATOMIC);
}

/*
 * 功能描述  : 不满足建链条件，无法启动建链的处理
 * 日    期  : 2021年02月18日
 * 作    者  : wifi
 */
void hmac_chba_fail_to_connect(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    hmac_chba_conn_param_stru *conn_param, uint8_t connect_role)
{
    mac_status_code_enum_uint16 reason_code;
    hmac_chba_vap_stru *chba_vap_info = NULL;

    /* hmac_user有可能为空，在后续逻辑中判断处理 */
    if (oal_any_null_ptr2(hmac_vap, conn_param)) {
        return;
    }

    reason_code = conn_param->status_code;
    oam_warning_log1(0, 0, "hmac_chba_fail_to_connect::fail to connect, reason code[%d].", reason_code);

    chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if (chba_vap_info == NULL) {
        return;
    }

    /* 如果角色是CHBA_CONN_INITIATOR，延用原接口上报建链失败 */
    if (connect_role == CHBA_CONN_INITIATOR) {
        hmac_report_connect_failed_result(hmac_vap, reason_code, conn_param->peer_addr);
    } else {
        /* 填写上报结果，并调用接口上报建链准备失败 */
        oal_cfg80211_connect_ready_report(hmac_vap->pst_net_device, conn_param->peer_addr,
            conn_param->op_id, conn_param->status_code, GFP_ATOMIC);
    }

    /* 如果用户已经创建，删除用户 */
    if (hmac_user != NULL) {
        hmac_user_del(&hmac_vap->st_vap_base_info, hmac_user);
    }

#ifdef _PRE_WLAN_FEATURE_NBW
    if (hmac_vap->st_vap_base_info.nbw_mode == NBW_MODE) {
        hmac_nbw_clear_curr_conn_info();
    }
#endif
}
/*
 * 功能描述  : 建链后直接进行主设备选举
 * 日    期  : 2021年10月23日
 * 作    者  : wifi7
 */
static void hmac_chba_master_election_after_asoc(hmac_vap_stru *hmac_vap, uint8_t *sa_addr,
    chba_req_sync_after_assoc_stru *req_sync)
{
    hmac_chba_sync_info *sync_info = hmac_chba_get_sync_info();
    hmac_chba_vap_stru *chba_vap_info = hmac_vap->hmac_chba_vap_info;
    master_info own_rp_info;
    master_info peer_rp_info;

    /* 1.获取对端RP值信息 */
    peer_rp_info.master_rp = req_sync->peer_rp;
    oal_set_mac_addr(peer_rp_info.master_addr, sa_addr);
    peer_rp_info.master_work_channel = chba_vap_info->work_channel.uc_chan_number;

    /* 2.获取自身RP值信息 */
    own_rp_info.master_rp = sync_info->own_rp_info;
    oal_set_mac_addr(own_rp_info.master_addr, mac_mib_get_StationID(&hmac_vap->st_vap_base_info));
    own_rp_info.master_work_channel = chba_vap_info->work_channel.uc_chan_number;

    /* 3.RP值比较，自身RP值较大, 成为Master */
    if (hmac_chba_sync_dev_rp_compare(&own_rp_info, &peer_rp_info) == OAL_TRUE) {
        hmac_chba_sync_become_master_handler(sync_info);
        oam_warning_log0(0, OAM_SF_CHBA, "{hmac_chba_master_election_after_asoc:become master!}");
        return;
    }

    /* 4.自身RP值较小, 以建链对端为Master，申请向建链对端同步 */
    /* 更新自身Master信息 */
    hmac_chba_set_role(CHBA_OTHER_DEVICE);
    sync_info->cur_master_info = peer_rp_info;
    /* 准备向对端同步 */
    hmac_chba_ask_for_sync(peer_rp_info.master_work_channel, sa_addr, peer_rp_info.master_addr);

    oam_warning_log3(0, OAM_SF_CHBA,
        "{hmac_chba_master_election_after_asoc:become slave! Master is %02x:XX:XX:XX:%02x:%02x}",
        peer_rp_info.master_addr[MAC_ADDR_0], peer_rp_info.master_addr[MAC_ADDR_4],
        peer_rp_info.master_addr[MAC_ADDR_5]);
}

#ifdef _PRE_WLAN_FEATURE_STA_PM
/*
 * 功能描述  : CHBA入网获取IP超时处理函数
 * 1.日    期  : 2021年11月15日
 *   作    者  : wifi7
 *   修改内容  : 新生成函数
 */
uint32_t hmac_set_chba_ipaddr_timeout(void *para)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)para;
    uint8_t timeout;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
        if ((hmac_vap->uc_ps_mode == MAX_FAST_PS) || (hmac_vap->uc_ps_mode == AUTO_FAST_PS)) {
            timeout = (g_wlan_min_fast_ps_idle > 1) ? (g_wlan_min_fast_ps_idle - 1) : g_wlan_min_fast_ps_idle;
            wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, timeout);
        } else {
            wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, WLAN_SLEEP_DEFAULT_CHECK_CNT);
        }
#endif
    return OAL_SUCC;
}
#endif
static void hmac_chba_connect_time_dump(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    uint32_t now = (uint32_t)oal_time_get_stamp_ms();
    hmac_chba_sync_info *sync_info = hmac_chba_get_sync_info();
    /* CHBA关联命令下发到add key完成时间作为关联总时长 */
    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
        "hmac_chba_connect_time_dump::cur user[%d] connect time[%d]ms.",
        hmac_user->st_user_base_info.us_assoc_id,
        (uint32_t)oal_time_get_runtime(hmac_user->chba_user.add_user_tsf, now));
    hmac_user->chba_user.add_user_tsf = 0;
    sync_info->start_sync_tsf = (uint32_t)oal_time_get_stamp_ms();
}

/* 功能描述: 连接完成后开启向对端设备请求同步 */
OAL_STATIC void hmac_chba_start_sync_after_asoc(mac_vap_stru *mac_vap, mac_user_stru *mac_user, uint8_t work_channel,
    hmac_chba_sync_info *sync_info, chba_req_sync_after_assoc_stru *req_sync)
{
    /* 自身原为master, 与slave设备快速断连又快速重连场景, 低概率出现对端还未选出新master, 仍以自身为master的场景 */
    if (oal_compare_mac_addr(req_sync->peer_master_addr, mac_mib_get_StationID(mac_vap)) == 0) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CHBA,
            "hmac_chba_sync_process_after_asoc:peer master is myself!");
        hmac_chba_sync_become_master_handler(sync_info);
        return;
    }

    oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CHBA,
        "hmac_chba_sync_process_after_asoc::peer device is already sync, try to sync with it.");
    oal_set_mac_addr(sync_info->cur_master_info.master_addr, req_sync->peer_master_addr);
    hmac_chba_ask_for_sync(work_channel, mac_user->auc_user_mac_addr, req_sync->peer_master_addr);
}

/*
 * 功能描述  : 建链完成后的同步流程处理
 * 日    期  : 2021年03月01日
 * 作    者  : wifi
 */
void hmac_chba_sync_process_after_asoc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    chba_req_sync_after_assoc_stru *req_sync = &hmac_user->chba_user.req_sync;
    hmac_chba_vap_stru *chba_vap_info = hmac_vap->hmac_chba_vap_info;
    hmac_chba_sync_info *sync_info = hmac_chba_get_sync_info();
#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
    if (hmac_vap->st_vap_base_info.nbw_mode == NBW_MODE && hmac_chba_get_sync_state() == CHBA_WORK) {
        /* nbw 模式下，加密完成后，且已同步上，则开始申请时隙 */
        hmac_nbw_apply_slot(hmac_vap);
        return;
    }
#endif

    /* 非CHBA vap或者不需要进行同步场景无需进行处理 */
    if (hmac_chba_vap_start_check(hmac_vap) != OAL_TRUE || req_sync->need_sync_flag != OAL_TRUE) {
        return;
    }

    hmac_chba_connect_time_dump(hmac_vap, hmac_user);
    /* 同步标志位还原 */
    req_sync->need_sync_flag = OAL_FALSE;
    /* 连接流程结束, 将对应mac地址从白名单中删除 */
    hmac_chba_whitelist_del_user(hmac_user->st_user_base_info.auc_user_mac_addr, OAL_MAC_ADDR_LEN);
    /* 极速连接响应端, 端口开启视为连接流程结束 */
    if ((hmac_chba_check_fast_connect_flag(req_sync->conn_flag) && (req_sync->conn_role == CHBA_CONN_RESPONDER))) {
        if (hmac_chba_is_not_verified_user(chba_vap_info, hmac_user->st_user_base_info.us_assoc_id) == OAL_TRUE) {
            hmac_chba_not_verified_fast_conn_rsp_finish_proc((void *)hmac_vap);
        } else {
            hmac_chba_clear_one_fast_conn_rsp_info(hmac_vap, chba_vap_info, hmac_user);
        }
    }

    /*  将新user添加到topo与岛信息中
       为避免出现eapol交互完成前便向新岛成员进行域合并, 将新user信息更新放在eapol完成之后 */
    hmac_chba_add_conn_proc(hmac_vap, hmac_user);

    /* 1、如果自己已经同步，但对方尚未同步，直接返回 */
    if (hmac_chba_get_sync_state() > CHBA_NON_SYNC && req_sync->peer_sync_state == MAC_CHBA_DECLARE_NON_SYNC) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
            "hmac_chba_sync_process_after_asoc::already sync, return.");
        return;
    }

    /* 2、如果自己尚未同步，而对方已经同步，将对方信息填入Request List，并调用Request List函数 */
    if (hmac_chba_get_sync_state() == CHBA_NON_SYNC && req_sync->peer_sync_state > MAC_CHBA_DECLARE_NON_SYNC) {
        hmac_chba_start_sync_after_asoc(&hmac_vap->st_vap_base_info, &hmac_user->st_user_base_info,
            chba_vap_info->work_channel.uc_chan_number, sync_info, req_sync);
        return;
    }

    /* 3、如果都未同步，则进行Master选举 */
    if (hmac_chba_get_sync_state() == CHBA_NON_SYNC && req_sync->peer_sync_state == MAC_CHBA_DECLARE_NON_SYNC) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
            "hmac_chba_sync_process_after_asoc::nobody sync, start Master elec!");
        hmac_chba_master_election_after_asoc(hmac_vap, hmac_user->st_user_base_info.auc_user_mac_addr, req_sync);
        return;
    }

    /* 4、如果都同步，且master一致,无需处理 */
    if (oal_compare_mac_addr(req_sync->peer_master_addr, hmac_chba_get_master_mac_addr()) == 0) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
            "hmac_chba_sync_process_after_asoc::both of us are sync with the same master.");
        return;
    }
    /* 5、如果都已经同步但不是同一个域,则进行域合并 */
    oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
        "hmac_chba_sync_process_after_asoc::sync with the different master.");
    hmac_chba_multiple_domain_detection_after_asoc(hmac_user->st_user_base_info.auc_user_mac_addr, req_sync);
}
/*
 * 功能描述  : 建链完成后保存同步所需信息
 * 日    期  : 2021年11月05日
 * 作    者  : wifi7
 */
static void hmac_chba_save_sync_info_after_assoc(hmac_user_stru *hmac_user,
    hmac_chba_vap_stru *chba_vap_info, uint8_t *master_attr, uint8_t conn_flag, uint8_t conn_role)
{
    int32_t ret;
    chba_req_sync_after_assoc_stru *req_sync = &hmac_user->chba_user.req_sync;

    /* 获取对端同步状态 */
    req_sync->peer_sync_state = master_attr[MAC_CHBA_ATTR_HDR_LEN];
    /* 获取对端Master RP */
    ret = memcpy_s((uint8_t *)&req_sync->peer_master_rp, sizeof(ranking_priority),
        master_attr + MAC_CHBA_MASTER_RANKING_LEVEL_POS, sizeof(ranking_priority));
    if (ret != EOK) {
        oam_error_log1(0, OAM_SF_CHBA,
            "{hmac_chba_save_sync_info_after_assoc:memcpy failed, ret[%d]}", ret);
        return;
    }
    /* 获取对端自身 RP */
    ret = memcpy_s((uint8_t *)&req_sync->peer_rp, sizeof(ranking_priority),
        master_attr + MAC_CHBA_RANKING_LEVEL_POS, sizeof(ranking_priority));
    if (ret != EOK) {
        oam_error_log1(0, OAM_SF_CHBA,
            "{hmac_chba_save_sync_info_after_assoc:memcpy failed, ret[%d]}", ret);
        return;
    }
    /* 获取对端Master mac地址 */
    oal_set_mac_addr(req_sync->peer_master_addr, master_attr + MAC_CHBA_MASTER_ADDR_POS);
    /* 获取对端工作信道 */
    req_sync->peer_work_channel = chba_vap_info->work_channel.uc_chan_number;
    /* 保存建链过程中角色状态 */
    req_sync->conn_flag = conn_flag;
    req_sync->conn_role = conn_role;

    /* need_sync_flag置位, 在eapol结束后启动sync流程 */
    req_sync->need_sync_flag = OAL_TRUE;
}

/* 功能描述: 查找chba主设备选举字段 */
OAL_STATIC uint8_t *hmac_chba_find_master_election_attr(uint8_t *src_payload, uint16_t src_payload_len)
{
    uint8_t *chba_ie = NULL;
    uint8_t *master_attr = NULL;
    uint8_t *remain_payload = src_payload;
    uint16_t chba_ie_len;
    uint16_t master_attr_len;
    uint16_t remain_len = src_payload_len;
    mac_ies_info ies_info = {0};

    /* 轮询全部chba ie, 从中获取chba主设备选举attr */
    while (remain_len > MAC_IE_HDR_LEN) {
        ies_info.frame_ies = remain_payload;
        ies_info.frame_len = src_payload_len;
        chba_ie = mac_find_vendor_ie(MAC_CHBA_VENDOR_IE, MAC_OUI_TYPE_CHBA, &ies_info);
        if (chba_ie == NULL) {
            break;
        }
        chba_ie_len = MAC_IE_HDR_LEN + chba_ie[1];
        /* chba ie长度校验 */
        if ((chba_ie_len < sizeof(mac_ieee80211_vendor_ie_stru)) ||
            ((uint16_t)((chba_ie - src_payload) + chba_ie_len) > src_payload_len)) {
            oam_error_log2(0, OAM_SF_CHBA,
                "{hmac_chba_find_master_election_attr:chba_ie_len[%u], src_len[%u]}", chba_ie_len, src_payload_len);
            break;
        }

        /* 从chba ie中查找chba主设备选举字段 */
        master_attr_len = chba_ie_len - sizeof(mac_ieee80211_vendor_ie_stru);
        master_attr = chba_ie + sizeof(mac_ieee80211_vendor_ie_stru);
        master_attr = hmac_find_chba_attr(MAC_CHBA_ATTR_MASTER_ELECTION, master_attr, master_attr_len);
        if (master_attr && (MAC_CHBA_ATTR_HDR_LEN + master_attr[1] == MAC_CHBA_MASTER_ELECTION_ATTR_LEN)) {
            return master_attr;
        }

        /* chba ie中未找到主设备选举字段, 跳至下一个ie继续查找 */
        remain_payload = chba_ie + chba_ie_len;
        remain_len = src_payload_len - (uint16_t)(remain_payload - src_payload);
    }
    return NULL;
}
/*
 * 功能描述  : 建链完成后的处理
 * 日    期  : 2021年02月18日
 * 作    者  : wifi
 */
void hmac_chba_connect_succ_handler(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len)
{
    uint8_t conn_flag = hmac_user->chba_user.connect_info.conn_flag;
    uint8_t conn_role = hmac_user->chba_user.connect_info.connect_role;
    uint8_t *master_attr = NULL;
    hmac_chba_vap_stru *chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if ((mac_is_chba_mode(&hmac_vap->st_vap_base_info) == OAL_FALSE) || (chba_vap_info == NULL)) {
        return;
    }

    /* 关联成功后，停止关联超时定时器 */
    if (hmac_user->chba_user.connect_info.assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(hmac_user->chba_user.connect_info.assoc_waiting_timer));
    }

    /* 更新CHBA用户状态为关联UP */
    hmac_chba_user_set_connect_role(hmac_user, CHBA_CONN_ROLE_BUTT, CHBA_INVALID_CONNECT_FLAG);
    hmac_chba_connect_initiator_fsm_change_state(hmac_user, CHBA_USER_STATE_LINK_UP);
#ifdef _PRE_WLAN_FEATURE_NBW
    if (hmac_vap->st_vap_base_info.nbw_mode == NBW_MODE) {
        oam_warning_log0(0, OAM_SF_CHBA, "{NBW: hmac_chba_connect_succ_handler: nbw connect succ}");
#ifdef _PRE_WLAN_FEATURE_STA_PM
        frw_timer_create_timer_m(&(hmac_vap->st_ps_sw_timer), hmac_set_chba_ipaddr_timeout, CHBA_SWITCH_STA_PSM_PERIOD,
            (void *)hmac_vap, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
        hmac_vap->us_check_timer_pause_cnt = 0;
#endif
        return;
    }
#endif

    /* 查找Master选举属性 */
    /* assoc req/rsp 帧体中查找CHBA IE */
    master_attr = hmac_chba_find_master_election_attr(payload, payload_len);
    if (master_attr == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
            "hmac_chba_connect_succ_handler:fail to find master election attr");
        return;
    }
    hmac_chba_save_sync_info_after_assoc(hmac_user, chba_vap_info, master_attr, conn_flag, conn_role);

    /* 非加密模式直接开启同步流程, 无需等待加密 */
    if (hmac_chba_check_no_encry_connect_flag(conn_flag) == OAL_TRUE) {
        oam_warning_log1(0, OAM_SF_CHBA, "{hmac_chba_connect_succ_handler:user[%d] no_encry_connect}",
            hmac_user->st_user_base_info.us_assoc_id);
        hmac_chba_sync_process_after_asoc(hmac_vap, hmac_user);
    }

#ifdef _PRE_WLAN_FEATURE_STA_PM
    /* 获取IP超时时, 配置低功耗开关 */
    frw_timer_create_timer_m(&(hmac_vap->st_ps_sw_timer), hmac_set_chba_ipaddr_timeout, CHBA_SWITCH_STA_PSM_PERIOD,
        (void *)hmac_vap, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
    /*  低功耗开关定时器pause计数, 启动定时器的同时需要将计数清0, 否则会一直累加 */
    hmac_vap->us_check_timer_pause_cnt = 0;
#endif

    oam_warning_log0(0, OAM_SF_CHBA, "{hmac_chba_connect_succ_handler:connect succ}");
}

/*
 * 功能描述  : 已做好建链准备，但建链失败的处理
 * 日    期  : 2021年02月18日
 * 作    者  : wifi
 */
static void hmac_chba_connect_fail_handler(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t connect_role)
{
    hmac_chba_user_stru *hmac_chba_user = NULL;
    chba_connect_status_stru *connect_info = NULL;
    hmac_chba_vap_stru *chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if (chba_vap_info == NULL) {
        return;
    }

    hmac_chba_user = &(hmac_user->chba_user);
    connect_info = &(hmac_chba_user->connect_info);

    /* 如果connect_role为CHBA_CONN_INITIATOR，上报WAPS建链失败; connect_role为CHBA_CONN_RESPONDER，无需上报建链失败 */
    if (connect_role == CHBA_CONN_INITIATOR) {
        hmac_report_connect_failed_result(hmac_vap, connect_info->status_code,
            hmac_user->st_user_base_info.auc_user_mac_addr);
    }

    /* 删除用户 */
    hmac_user_del(&hmac_vap->st_vap_base_info, hmac_user);
}
/*
 * 功能描述  : 建链响应者Assoc等待超时响应函数
 * 日    期  : 2021年02月09日
 * 作    者  : wifi
 */
uint32_t hmac_chba_assoc_waiting_timeout_responder(void *arg)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;

    hmac_user = (hmac_user_stru *)arg;
    if (hmac_user == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* responder不上报建链失败，只恢复建链前的状态 */
    oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
        "hmac_chba_assoc_waiting_timeout_responder::responder assoc failed, restore.");
    hmac_chba_connect_fail_handler(hmac_vap, hmac_user, CHBA_CONN_RESPONDER);
    return OAL_SUCC;
}

/*
 * 功能描述  : 根据mac地址生成域bssid
 * 日    期  : 2021年02月09日
 * 作    者  : wifi
 */
void hmac_generate_chba_domain_bssid(uint8_t *bssid, uint8_t bssid_len,
    uint8_t *mac_addr, uint8_t mac_addr_len)
{
    if (memcpy_s(bssid, bssid_len, mac_addr, mac_addr_len) != EOK) {
        oam_warning_log0(0, 0, "hmac_generate_chba_domain_bssid: memcpy_s failed.");
        return;
    }
    bssid[MAC_ADDR_0] = 0x00;
    bssid[MAC_ADDR_1] = 0xE0;
    bssid[MAC_ADDR_2] = 0xFC;
}

/*
 * 功能描述  : 根据mac地址更新vap bssid
 * 日    期  : 2021年09月10日
 * 作    者  : wifi7
 */
void hmac_chba_vap_update_domain_bssid(hmac_vap_stru *hmac_vap, uint8_t *mac_addr)
{
    mac_vap_stru *mac_vap = NULL;

    if ((hmac_vap == NULL) || (mac_addr == NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_chba_vap_update_domain_bssid: ptr is NULL}");
        return;
    }
    mac_vap = &hmac_vap->st_vap_base_info;

    /* 更新mac_vap下bssid */
    hmac_generate_chba_domain_bssid(mac_vap->auc_bssid, WLAN_MAC_ADDR_LEN, mac_addr, WLAN_MAC_ADDR_LEN);
}
static uint32_t hmac_chba_send_chba_topo_info(mac_vap_stru *mac_vap, chba_h2d_topo_info_stru *topo_info)
{
    dmac_tx_event_stru *tx_event = NULL;
    frw_event_mem_stru *event_mem = NULL;
    oal_netbuf_stru *cmd_netbuf = NULL;
    frw_event_stru *hmac_to_dmac_ctx_event = NULL;
    uint32_t ret;
    int32_t err;

    cmd_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, sizeof(chba_h2d_topo_info_stru), OAL_NETBUF_PRIORITY_MID);
    if (cmd_netbuf == NULL) {
        oam_error_log0(0, OAM_SF_CHBA, "hmac_chba_send_chba_topo_info::netbuf alloc null!");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 拷贝命令结构体到netbuf */
    err = memcpy_s(oal_netbuf_data(cmd_netbuf), sizeof(chba_h2d_topo_info_stru),
        topo_info, sizeof(chba_h2d_topo_info_stru));
    oal_netbuf_put(cmd_netbuf, sizeof(chba_h2d_topo_info_stru));
    if (err != EOK) {
        oam_error_log0(0, OAM_SF_CHBA, "hmac_chba_send_chba_topo_info::memcpy fail!");
        oal_netbuf_free(cmd_netbuf);
        return OAL_FAIL;
    }
    /* 抛事件到DMAC */
    event_mem = frw_event_alloc_m(sizeof(dmac_tx_event_stru));
    if (event_mem == NULL) {
        oam_error_log0(0, OAM_SF_CHBA, "hmac_chba_send_chba_topo_info::event_mem null.");
        oal_netbuf_free(cmd_netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_to_dmac_ctx_event = (frw_event_stru *)event_mem->puc_data;
    frw_event_hdr_init(&(hmac_to_dmac_ctx_event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX,
        DMAC_WLAN_CTX_EVENT_SUB_TYPE_CHBA_TOPO_INFO_SYNC, sizeof(dmac_tx_event_stru), FRW_EVENT_PIPELINE_STAGE_1,
        mac_vap->uc_chip_id, mac_vap->uc_device_id, mac_vap->uc_vap_id);

    tx_event = (dmac_tx_event_stru *)(hmac_to_dmac_ctx_event->auc_event_data);
    tx_event->pst_netbuf = cmd_netbuf;
    tx_event->us_frame_len = oal_netbuf_len(cmd_netbuf);

    ret = frw_event_dispatch_event(event_mem);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_CHBA, "hmac_chba_send_chba_topo_info::dispatch_event fail[%d]!", ret);
    }

    oal_netbuf_free(cmd_netbuf);
    frw_event_free_m(event_mem);
    return OAL_SUCC;
}
/*
 * 功能描述  : 同步连接拓扑图和device id信道到DMAC
 * 1.日    期  : 2021年10月28日
 */
uint32_t hmac_chba_topo_info_sync(void)
{
    uint32_t ret;
    int32_t err;
    hmac_chba_vap_stru *chba_vap_info = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    chba_h2d_topo_info_stru topo_info;
    uint32_t *network_topo_addr = hmac_chba_get_network_topo_addr();

    chba_vap_info = hmac_get_up_chba_vap_info();
    if (chba_vap_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(chba_vap_info->mac_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(&topo_info, sizeof(chba_h2d_topo_info_stru), 0, sizeof(chba_h2d_topo_info_stru));

    /* 填写连接拓扑 */
    err = memcpy_s(topo_info.network_topo, sizeof(uint32_t) * (MAC_CHBA_MAX_DEVICE_NUM * MAC_CHBA_MAX_BITMAP_WORD_NUM),
        network_topo_addr, sizeof(uint32_t) * (MAC_CHBA_MAX_DEVICE_NUM * MAC_CHBA_MAX_BITMAP_WORD_NUM));
    if (err != EOK) {
        oam_warning_log1(0, OAM_SF_CHBA, "hmac_chba_topo_info_sync::memcpy failed, err[%d].", err);
        return OAL_FAIL;
    }

    /* 同步给DMAC */
    ret = hmac_chba_send_chba_topo_info(&hmac_vap->st_vap_base_info, &topo_info);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CHBA, "hmac_chba_topo_info_sync::sync topo info err[%u]", ret);
    }
    return ret;
}

/*
 * 功能描述  : 增加或删除某个device id同步到DMAC
 * 1.日    期  : 2021年10月28日
 */
uint32_t hmac_chba_device_info_sync(uint8_t update_type, uint8_t device_id, const uint8_t *mac_addr)
{
    uint32_t ret;
    hmac_chba_vap_stru *chba_vap_info = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    chba_h2d_device_update_stru device_update_info = { 0 };

    chba_vap_info = hmac_get_up_chba_vap_info();
    if (chba_vap_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(chba_vap_info->mac_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 填写接口结构体 */
    device_update_info.update_type = update_type;
    if (update_type != CHBA_DEVICE_ID_CLEAR) {
        device_update_info.device_id = device_id;
        oal_set_mac_addr(device_update_info.mac_addr, mac_addr);
    }

    /* 同步给DMAC */
    ret = hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_CHBA_DEVICE_INFO_UPDATE,
        sizeof(chba_h2d_device_update_stru), (uint8_t *)&device_update_info);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CHBA, "hmac_chba_device_info_sync::sync device info err[%u]", ret);
    }
    return ret;
}

/*
 * 功能描述  : 获取chba_vap_info指针
 * 日    期  : 2022年1月6日
 * 作    者  : wifi
 */
hmac_chba_vap_stru *hmac_chba_get_chba_vap_info(hmac_vap_stru *hmac_vap)
{
    /* chba vap未使能 */
    if (hmac_chba_vap_start_check(hmac_vap) == OAL_FALSE) {
        return NULL;
    }

    return ((hmac_chba_vap_stru *)hmac_vap->hmac_chba_vap_info);
}

/*
 * 功能描述  : 获取hmac chba vap
 * 日    期  : 2022年5月18日
 * 作    者  : wifi7
 */
hmac_vap_stru *hmac_chba_find_chba_vap(mac_device_stru *mac_device)
{
    uint8_t vap_idx;
    mac_vap_stru *mac_vap = NULL;
    hmac_vap_stru *hmac_vap = NULL;

    /* chba定制化开关未开启, 无需进行查找 */
    if (!(g_optimized_feature_switch_bitmap & BIT(CUSTOM_OPTIMIZE_CHBA))) {
        return NULL;
    }

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(mac_device->auc_vap_id[vap_idx]);
        if (mac_vap == NULL) {
            continue;
        }

        hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
        if (hmac_chba_vap_start_check(hmac_vap) == OAL_TRUE) {
            return hmac_vap;
        }
    }

    return NULL;
}
void hmac_chba_set_freq_lock_th(uint16_t *cpu_freq_high_limit, uint16_t *cpu_freq_low_limit)
{
    hmac_device_stru *hmac_device = hmac_res_get_mac_dev(0);
    uint16_t cpu_freq_high_limit_record = *cpu_freq_high_limit;
    uint16_t cpu_freq_low_limit_record = *cpu_freq_low_limit;
    if (hmac_device == NULL) {
        return;
    }
    /* 存在CHBA vap时，降低锁频门限为600M */
    if (hmac_chba_find_chba_vap(hmac_device->pst_device_base_info) != NULL) {
        *cpu_freq_high_limit = cpu_freq_high_limit_record / 2; // 2表示取定制化值一半
        *cpu_freq_low_limit = cpu_freq_low_limit_record / 2; // 2表示取定制化值一半
    }
}

/* 功能描述: 未校验直接add的极速连接user, 需启动超时定时器, 防止assoc rsp发送失败时无后续处理 */
static void hmac_chba_start_not_verified_user_timer(hmac_vap_stru *hmac_vap, uint16_t user_idx)
{
    hmac_user_stru *hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(user_idx);
    if (hmac_user == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_chba_start_not_verified_user_timer:fail to get user[%d]}", user_idx);
        return;
    }

    hmac_user->chba_user.connect_info.assoc_waiting_time = CHBA_CONN_RESPONDER_TIMER_DEFAULT;
    frw_timer_create_timer_m(&(hmac_user->chba_user.connect_info.assoc_waiting_timer),
        hmac_chba_assoc_waiting_timeout_responder, hmac_user->chba_user.connect_info.assoc_waiting_time,
        hmac_user, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
}

/* 功能描述: 不进行白名单校验的极速连接响应端添加user */
OAL_STATIC uint32_t hmac_chba_not_verified_fast_connect_rsp_add_user(hmac_vap_stru *hmac_vap,
    hmac_chba_vap_stru *chba_vap_info, hmac_chba_conn_param_stru *conn_params, uint8_t *sta_addr, size_t maclen)
{
    uint8_t user_conn_flag;
    uint32_t ret;
    uint16_t user_idx;
#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
    /* 响应方新增user后，更新nbw保存的建链参数 */
    hmac_nbw_current_connect_info_stru *nbw_curr_conn_info = hmac_nbw_get_curr_conn_info();
#endif

    /* 只有不进行白名单校验的极速连接响应端才会在rx assoc req时添加user */
    if (hmac_chba_is_not_verified_fast_connect_response(chba_vap_info) == OAL_FALSE) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_not_verified_fast_connect_rsp_add_user:not_verified_flag is false!}");
        return OAL_FAIL;
    }
    /* 为极速连接响应方添加user */
    user_conn_flag =
        ((chba_vap_info->fast_conn_rsp_info.not_verified_rsp_conn_mode == CHBA_NOT_VERIFIED_CONN_RSP_ENCRY_CONN_MODE) ?
        CHBA_ENCRY_FAST_CONNECT_FLAG : CHBA_NOT_ENCRY_FAST_CONNECT_FLAG);
    oal_set_mac_addr(conn_params->peer_addr, sta_addr);
    conn_params->assoc_channel = hmac_vap->st_vap_base_info.st_channel;
    ret = hmac_chba_user_mgmt_conn_prepare(&(hmac_vap->st_vap_base_info), conn_params, &user_idx,
        CHBA_CONN_RESPONDER, user_conn_flag);
    if (ret != OAL_SUCC) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_not_verified_fast_connect_rsp_add_user:fail to add user, ret[%d], status_code[%d]}",
            ret, conn_params->status_code);
        return ret;
    }
    hmac_chba_start_not_verified_user_timer(hmac_vap, user_idx);
    /* 未经过校验的user保存其user_id, 待上层校验完成后删除 */
    hmac_chba_save_not_verified_user_id(chba_vap_info, user_idx);
#if (defined _PRE_WLAN_FEATURE_NBW) && (defined _PRE_WLAN_NBW_DEMO)
    /* 响应方新增user后，更新nbw保存的建链参数 */
    oal_set_mac_addr(nbw_curr_conn_info->peer_addr, sta_addr);
    oam_warning_log0(0, 0, "{hmac_chba_not_verified_fast_connect_rsp_add_user: update nbw conn info peer addr}");
#endif
    return OAL_SUCC;
}

/* 功能描述: chba连接关联响应方收到assoc req处理 */
uint32_t hmac_chba_conn_response_rx_assoc_proc(hmac_vap_stru *hmac_vap, mac_rx_ctl_stru *rx_info, uint8_t *mac_hdr)
{
    uint32_t ret;
    uint16_t user_idx;
    uint8_t sta_addr[WLAN_MAC_ADDR_LEN];
    hmac_chba_vap_stru *chba_vap_info = NULL;
    hmac_chba_conn_param_stru conn_params = {};

    /* 如果极速连接响应方模式未运行, 则无需继续执行 */
    if (hmac_chba_is_fast_connect_response(hmac_vap) == OAL_FALSE) {
        return OAL_SUCC;
    }
    chba_vap_info = hmac_vap->hmac_chba_vap_info;

    mac_get_address2(mac_hdr, sta_addr, WLAN_MAC_ADDR_LEN);
    /* 判断对端mac地址是否为有效，不能为全0 */
    if (mac_addr_is_zero(sta_addr)) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_conn_response_rx_assoc_proc:mac_addr is zero!}");
        return OAL_FAIL;
    }
    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{hmac_chba_conn_response_rx_assoc_proc:rx assoc_req[%02x:XX:XX:XX:%02x:%02x]}",
        sta_addr[MAC_ADDR_0], sta_addr[MAC_ADDR_4], sta_addr[MAC_ADDR_5]);
    /* 白名单检测, 极速连接响应方如果白名单配置为广播地址则可接受一切合法assoc帧mac地址, 否则就需要对mac地址进行校验 */
    /* 检查顺序:先检查对端mac地址是否在白名单中，如果不在则再检查白名单中是否存在广播mac地址 */
    if ((hmac_chba_whitelist_check(sta_addr, WLAN_MAC_ADDR_LEN) != OAL_SUCC) &&
        (hmac_chba_whitelist_check(BROADCAST_MACADDR, WLAN_MAC_ADDR_LEN) != OAL_SUCC)) {
        return OAL_FAIL;
    }

    /* 因黑名单过滤检查 */
    if (hmac_blacklist_filter(&(hmac_vap->st_vap_base_info), sta_addr, WLAN_MAC_ADDR_LEN) == OAL_TRUE) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_conn_response_rx_assoc_proc:blacklist check filter!}");
        return OAL_FAIL;
    }
    /* 用户已存在, 无需重复添加user */
    ret = mac_vap_find_user_by_macaddr(&(hmac_vap->st_vap_base_info), sta_addr, &user_idx);
    if (ret == OAL_SUCC) {
        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_conn_response_rx_assoc_proc:find user[%d] succ!}", user_idx);
        return OAL_SUCC;
    }

    /* 无需进行白名单校验的极速连接相应端, 将新收到的assoc req对应设备均作为新增user */
    return hmac_chba_not_verified_fast_connect_rsp_add_user(hmac_vap, chba_vap_info, &conn_params,
        sta_addr, WLAN_MAC_ADDR_LEN);
}

/* 功能描述: 外部事件输入是否允许打断当前流程检查，检查chba vap是否正在入网或者正在同步 */
oal_bool_enum_uint8 hmac_chba_is_connecting_or_syncing(mac_vap_stru *mac_vap)
{
    sync_info_flags *sync_flags = hmac_chba_get_sync_flags();
    hmac_chba_domain_merge_info *domain_merge = hmac_chba_get_domain_merge_info();
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);

    /* 非chba vap不处理 */
    if (hmac_chba_vap_start_check(hmac_vap) == OAL_FALSE) {
        return OAL_FALSE;
    }

    /* chba正在处于同步流程或域合并流程, 不可发起扫描 */
    if (hmac_chba_get_sync_status() == CHBA_SYNC_IS_RUNNING) {
        oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_SCAN,
            "{hmac_chba_is_connecting_or_syncing:sync_flag[%d], domain_merge_flag[%d]}",
            hmac_chba_get_sync_request_flag(sync_flags), hmac_chba_get_domain_merge_flag(domain_merge));
        return OAL_TRUE;
    }

    /* chba正作为响应端进行极速连接, 不可发起扫描 */
    if (hmac_chba_is_fast_connect_response(hmac_vap) == OAL_TRUE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_SCAN,
            "{hmac_chba_is_connecting_or_syncing:is_fast_connect_response}");
        return OAL_TRUE;
    }

    return hmac_user_is_connecting(mac_vap);
}

/* 功能描述: chba极速连接响应端在assoc rsp发送完成后再上报内核连接成功 */
uint32_t hmac_chba_tx_complete_assoc_rsp(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    mac_chba_tx_assoc_rsp_complete_stru *tx_complete_report = NULL;

    if (mac_vap == NULL || param == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    tx_complete_report = (mac_chba_tx_assoc_rsp_complete_stru *)param;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_chba_vap_start_check(hmac_vap) == OAL_FALSE) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CHBA,
            "{hmac_chba_tx_complete_assoc_rsp: CHBA vap is not start}");
        return OAL_FAIL;
    }

    hmac_user = mac_res_get_hmac_user(tx_complete_report->user_idx);
    if (hmac_user == NULL) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_CHBA,
            "{hmac_chba_tx_complete_assoc_rsp: fail to get chba_user[%d]}", tx_complete_report->user_idx);
        return OAL_FAIL;
    }

    /* assoc rsp发送失败处理 */
    if (tx_complete_report->tx_status != MAC_TX_SUCC) {
        oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_chba_tx_complete_assoc_rsp:user[%d] tx rsp fail, tx_status[%d], retry[%d], back to [%d]}",
            tx_complete_report->user_idx, tx_complete_report->tx_status, tx_complete_report->retry_num,
            hmac_chba_get_last_user_asoc_state(hmac_user));
        hmac_user_set_asoc_state(mac_vap, &hmac_user->st_user_base_info, hmac_chba_get_last_user_asoc_state(hmac_user));
        return OAL_SUCC;
    }
    oam_warning_log3(mac_vap->uc_vap_id, OAM_SF_ANY,
        "{hmac_chba_tx_complete_assoc_rsp:user[%d] tx assoc rsp complete, tx_status[%d], retry_num[%d]}",
        tx_complete_report->user_idx, tx_complete_report->tx_status, tx_complete_report->retry_num);
    hmac_ap_up_process_asoc_succ_report(hmac_vap, hmac_user);
    return OAL_SUCC;
}
#endif /* end of this file */
