/*
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 * 功能说明 : 同信道监听相关文件
 * 作    者 : y30038709
 * 创建日期 : 2024年5月9日
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "hmac_scan.h"
#include "hmac_config.h"
#include "oal_cfg80211.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SCAN_SAME_CHAN_REMAIN_ON_CHAN_C

#define LOGN_SAME_CHAN_REMAIN_ON_CHAN_BIT 0
#define NORMAL_SAME_CHAN_REMAIN_ON_CHAN_BIT 1

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
static mac_same_chan_remain_on_chan_info_stru g_same_chan_remain_on_chan_info;

/*****************************************************************************
  3 函数实现
*****************************************************************************/
mac_same_chan_remain_on_chan_info_stru *hmac_get_same_chan_remain_on_chan_info(void)
{
    return &g_same_chan_remain_on_chan_info;
}

/* 功能描述: 判断长期同信道remain_on_chan是否正在运行 */
static inline oal_bool_enum_uint8 mac_long_same_remain_on_chan_is_running(
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info)
{
    return ((remain_on_chan_info->same_chan_remain_on_chan_bitmap & BIT(LOGN_SAME_CHAN_REMAIN_ON_CHAN_BIT)) != 0);
}

/* 功能描述: 判断普通同信道remain_on_chan是否正在运行 */
oal_bool_enum_uint8 mac_normal_same_remain_on_chan_is_running(
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info)
{
    return ((remain_on_chan_info->same_chan_remain_on_chan_bitmap & BIT(NORMAL_SAME_CHAN_REMAIN_ON_CHAN_BIT)) != 0);
}
static inline void mac_set_long_same_remain_on_chan(mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info,
    uint8_t listen_vap_id, uint8_t listen_chan, oal_bool_enum_uint8 is_start)
{
    if (is_start == OAL_TRUE) {
        remain_on_chan_info->same_chan_remain_on_chan_bitmap |= BIT(LOGN_SAME_CHAN_REMAIN_ON_CHAN_BIT);
        remain_on_chan_info->long_listen_chan_numer = listen_chan;
        remain_on_chan_info->long_listen_vap_id = listen_vap_id;
    } else {
        remain_on_chan_info->same_chan_remain_on_chan_bitmap &= ~(BIT(LOGN_SAME_CHAN_REMAIN_ON_CHAN_BIT));
        remain_on_chan_info->long_listen_chan_numer = 0;
        remain_on_chan_info->long_listen_vap_id = mac_chip_get_vap_support_max_num_limit();
    }
}
static inline void mac_set_normal_same_remain_on_chan(mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info,
    uint8_t listen_vap_id, uint8_t listen_chan, oal_bool_enum_uint8 is_start)
{
    if (is_start == OAL_TRUE) {
        remain_on_chan_info->same_chan_remain_on_chan_bitmap |= BIT(NORMAL_SAME_CHAN_REMAIN_ON_CHAN_BIT);
        remain_on_chan_info->normal_listen_chan_number = listen_chan;
        remain_on_chan_info->normal_listen_vap_id = listen_vap_id;
    } else {
        remain_on_chan_info->same_chan_remain_on_chan_bitmap &= ~(BIT(NORMAL_SAME_CHAN_REMAIN_ON_CHAN_BIT));
        remain_on_chan_info->normal_listen_chan_number = 0;
        remain_on_chan_info->normal_listen_vap_id = mac_chip_get_vap_support_max_num_limit();
    }
}

/* 功能描述: 普通同信道remain_on_chan超时处理 */
static uint32_t hmac_same_chan_remain_on_chan_timeout(void *arg)
{
    hmac_reset_same_chan_remain_on_chan((mac_vap_stru *)arg);
    oam_warning_log0(0, OAM_SF_SCAN, "{hmac_same_chan_remain_on_chan_timeout}");
    return OAL_SUCC;
}

/* 功能描述: 在监听信道查找已up工作vap */
mac_vap_stru *mac_find_listen_chan_work_vap(mac_device_stru *mac_device, mac_vap_stru *mac_self_vap,
    oal_bool_enum_uint8 is_allow_listen_byself, uint8_t scan_channel_num)
{
    uint8_t vap_idx;
    mac_vap_stru *mac_vap = NULL;

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        mac_vap = mac_res_get_dmac_vap(mac_device->auc_vap_id[vap_idx]);
        if (mac_vap == NULL) {
            continue;
        }
        /* 如果不允许通过mac_self_vap进行同信道监听, 则跳过自身 */
        if ((is_allow_listen_byself == OAL_FALSE) && (mac_vap->uc_vap_id == mac_self_vap->uc_vap_id)) {
            continue;
        }
        if (mac_vap_is_up(mac_vap) && (mac_vap->st_channel.uc_chan_number == scan_channel_num)) {
            return mac_vap;
        }
    }
    return NULL;
}

/* 功能描述: 同步同信道监听配置至device */
OAL_STATIC void hmac_same_chan_remain_on_chan_sync_device(mac_vap_stru *mac_vap, uint8_t remain_on_chan_bitmap,
    uint8_t listen_vap_id, uint8_t listen_chan_number, oal_bool_enum_uint8 is_mgmt_tx)
{
    uint32_t ret;
    mac_same_chan_remain_on_chan_h2d_info_stru h2d_params = { 0 };

    /* 配置同步到dmac */
    h2d_params.remain_on_chan_bitmap = remain_on_chan_bitmap;
    h2d_params.listen_vap_id = listen_vap_id;
    h2d_params.listen_chan_number = listen_chan_number;
    h2d_params.is_mgmt_tx = is_mgmt_tx;

    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_SAME_CHAN_REMAIN_ON_CHAN_H2D_SYNC,
        sizeof(mac_same_chan_remain_on_chan_h2d_info_stru), (uint8_t *)&h2d_params);
    oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_SCAN,
        "{hmac_same_chan_remain_on_chan_sync_device:bitmap[0x%x], is_mgmt_tx[%d], listen_vap[%d], channel[%d]}",
        remain_on_chan_bitmap, is_mgmt_tx, listen_vap_id, listen_chan_number);
}

/* 功能描述: 长期同信道监听配置下发device */
OAL_STATIC void hmac_long_same_remain_on_chan_sync_device(mac_vap_stru *mac_vap)
{
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    /* 长期同信道监听优先级低, 如果定时监听正在运行, 则不下发长期监听参数，等到定时监听结束后再执行长期监听 */
    if (mac_normal_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_TRUE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_long_same_remain_on_chan_sync_device:normal listen is running! wait to update long listen}");
        return;
    }
    hmac_same_chan_remain_on_chan_sync_device(mac_vap, remain_on_chan_info->same_chan_remain_on_chan_bitmap,
        remain_on_chan_info->long_listen_vap_id, remain_on_chan_info->long_listen_chan_numer, OAL_FALSE);
}

/* 功能描述: 定期同信道监听配置下发device */
OAL_STATIC void hmac_normal_same_remain_on_chan_sync_device(mac_vap_stru *mac_vap, oal_bool_enum_uint8 is_mgmt_tx)
{
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    /* 如果定期监听结束, 而长期监听还在运行，则下发长期监听配置 */
    if ((mac_normal_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_FALSE) &&
        (mac_long_same_remain_on_chan_is_running(remain_on_chan_info)) == OAL_TRUE) {
        oam_warning_log0(mac_vap->uc_vap_id,
            OAM_SF_ANY, "{hmac_normal_same_remain_on_chan_sync_device:reset long listen cfg!}");
        hmac_long_same_remain_on_chan_sync_device(mac_vap);
        return;
    }
    hmac_same_chan_remain_on_chan_sync_device(mac_vap, remain_on_chan_info->same_chan_remain_on_chan_bitmap,
        remain_on_chan_info->normal_listen_vap_id, remain_on_chan_info->normal_listen_chan_number, is_mgmt_tx);
}

/* 功能描述: 监听命令触发同信道监听 */
uint32_t hmac_listen_start_same_chan_remain_on_chan(mac_vap_stru *mac_self_vap, mac_vap_stru *mac_work_vap,
    uint32_t wait_time)
{
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    if (g_wlan_spec_cfg->feature_same_chan_remain_on_chan_support == OAL_FALSE) {
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }
    if (wait_time) {
        if (mac_normal_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_TRUE) {
            oam_warning_log0(mac_self_vap->uc_vap_id, OAM_SF_ANY,
                "{hmac_listen_start_same_chan_remain_on_chan:normal remain_on_chan has been running!}");
        }
        if (remain_on_chan_info->remain_on_chan_timer.en_is_registerd) {
            frw_timer_immediate_destroy_timer_m(&(remain_on_chan_info->remain_on_chan_timer));
        }
        mac_set_normal_same_remain_on_chan(remain_on_chan_info, mac_self_vap->uc_vap_id,
            mac_work_vap->st_channel.uc_chan_number, OAL_TRUE);
        hmac_normal_same_remain_on_chan_sync_device(mac_self_vap, OAL_FALSE);
        frw_timer_create_timer_m(&remain_on_chan_info->remain_on_chan_timer,
            hmac_same_chan_remain_on_chan_timeout, wait_time, (void *)mac_self_vap,
            OAL_FALSE, OAM_MODULE_ID_HMAC, mac_self_vap->core_id);
        return OAL_SUCC;
    }

    /* wait_time为0即可长期保持监听状态, 直到上层取消监听 */
    if (mac_long_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_TRUE) {
        oam_error_log0(mac_self_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_listen_start_same_chan_remain_on_chan:long remain_on_chan has been running!}");
        return OAL_FAIL;
    }
    mac_set_long_same_remain_on_chan(remain_on_chan_info, mac_self_vap->uc_vap_id,
        mac_work_vap->st_channel.uc_chan_number, OAL_TRUE);
    hmac_long_same_remain_on_chan_sync_device(mac_self_vap);
    return OAL_SUCC;
}

/* 功能描述: 取消长期同信道监听 */
uint32_t hmac_cancel_same_chan_remain_on_chan(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    if (g_wlan_spec_cfg->feature_same_chan_remain_on_chan_support == OAL_FALSE) {
        return OAL_SUCC;
    }
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY,
        "{hmac_cancel_same_chan_remain_on_chan:cur_bitmap[0x%x]}",
        remain_on_chan_info->same_chan_remain_on_chan_bitmap);
    if (mac_long_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_FALSE) {
        return OAL_SUCC;
    }

    mac_set_long_same_remain_on_chan(remain_on_chan_info, mac_chip_get_vap_support_max_num_limit(), 0, OAL_FALSE);
    hmac_long_same_remain_on_chan_sync_device(mac_vap);
    return OAL_SUCC;
}

/* 功能描述: 启动一次tx_mgmt引起的同信道监听 */
uint32_t hmac_tx_mgmt_start_same_chan_remain_on_chan(mac_vap_stru *mac_self_vap, mac_vap_stru *mac_work_vap,
    oal_bool_enum_uint8 remian_on_chan_need_wait, uint32_t wait_time)
{
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    if (g_wlan_spec_cfg->feature_same_chan_remain_on_chan_support == OAL_FALSE) {
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }
    if ((mac_self_vap == NULL) || (mac_work_vap == NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_tx_mgmt_start_same_chan_remain_on_chan:mac_vap is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 上一次普通同信道remain_on_chan未结束 */
    if (mac_normal_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_TRUE) {
        oam_warning_log0(mac_self_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_tx_mgmt_start_same_chan_remain_on_chan:normal remain_on_chan has been running!}");
    }
    // 如果上一次监听时间长，下一次监听时间短，会影响上一次的监听时间？
    if (remain_on_chan_info->remain_on_chan_timer.en_is_registerd) {
        frw_timer_immediate_destroy_timer_m(&(remain_on_chan_info->remain_on_chan_timer));
    }
    /* 配置监听标记与监听定时器 */
    mac_set_normal_same_remain_on_chan(remain_on_chan_info, mac_self_vap->uc_vap_id,
        mac_work_vap->st_channel.uc_chan_number, OAL_TRUE);
    if (remian_on_chan_need_wait == OAL_TRUE) {
        frw_timer_create_timer_m(&remain_on_chan_info->remain_on_chan_timer,
            hmac_same_chan_remain_on_chan_timeout, wait_time, (void *)mac_self_vap,
            OAL_FALSE, OAM_MODULE_ID_HMAC, mac_self_vap->core_id);
    }
    /* 配置同步到dmac */
    hmac_normal_same_remain_on_chan_sync_device(mac_self_vap, OAL_TRUE);
    oam_warning_log4(mac_self_vap->uc_vap_id, OAM_SF_ANY,
        "{hmac_tx_mgmt_start_same_chan_remain_on_chan:bitmap[0x%x], chan[%d], timer[%d], wait_time[%d]}",
        remain_on_chan_info->same_chan_remain_on_chan_bitmap, remain_on_chan_info->normal_listen_chan_number,
        remian_on_chan_need_wait, wait_time);
    return OAL_SUCC;
}

/* 功能描述: tx_mgmt/p2p scan后恢复长期同信道监听状态 */
void hmac_reset_same_chan_remain_on_chan(mac_vap_stru *mac_self_vap)
{
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    if (g_wlan_spec_cfg->feature_same_chan_remain_on_chan_support == OAL_FALSE) {
        return;
    }
    /* tx_mgmt/p2p scan会使用普通同信道监听, 占用p2p0资源, 发送结束后重置同信道监听状态 */
    if (mac_normal_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_TRUE) {
        mac_set_normal_same_remain_on_chan(remain_on_chan_info, mac_chip_get_vap_support_max_num_limit(), 0, OAL_FALSE);
        if (remain_on_chan_info->remain_on_chan_timer.en_is_registerd) {
            frw_timer_immediate_destroy_timer_m(&(remain_on_chan_info->remain_on_chan_timer));
        }
        hmac_normal_same_remain_on_chan_sync_device(mac_self_vap, OAL_FALSE);
        return;
    }

    /* 定时监听未运行, 但长期监听正在运行, 恢复原监听配置 */
    if (mac_long_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_TRUE) {
        hmac_long_same_remain_on_chan_sync_device(mac_self_vap);
    }
}

/* 功能描述: 长期监听被打断事件上报 */
OAL_STATIC void hmac_abort_long_same_chan_remain_on_chan_report(uint8_t long_listen_chan_numer, uint8_t reascon_code)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    mac_same_chan_remain_on_chan_abort_report_stru remain_on_chan_abort_info = { 0 };

    remain_on_chan_abort_info.report_type = HMAC_SAME_CHAN_REMAIN_ON_CHAN_ABORT;
    remain_on_chan_abort_info.listen_chan_number = long_listen_chan_numer;
    remain_on_chan_abort_info.reason_code = reascon_code;
    /* 复用chba上报接口通知上层 */
    oal_cfg80211_chba_report((uint8_t *)(&remain_on_chan_abort_info), sizeof(remain_on_chan_abort_info));
#endif
}

/* 功能描述: 长期同信道监听被打断 */
void hmac_abort_long_same_chan_remain_on_chan(mac_vap_stru *mac_vap, oal_bool_enum_uint8 is_allow_listen_byself,
    uint8_t reascon_code)
{
    uint8_t long_listen_chan_numer;
    mac_device_stru *mac_device = NULL;
    mac_vap_stru *mac_work_vap = NULL;
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    if (g_wlan_spec_cfg->feature_same_chan_remain_on_chan_support == OAL_FALSE) {
        return;
    }
    /* 未进行长期同信道监听, 不处理 */
    if (mac_long_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_FALSE) {
        return;
    }

    /* 查询当前是否还存在其他可用于同信道监听的工作vap */
    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_device == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_abort_long_same_chan_remain_on_chan:fail to get device, device_id[%d]}",
            mac_vap->uc_device_id);
        return;
    }
    mac_work_vap = mac_find_listen_chan_work_vap(mac_device, mac_vap, is_allow_listen_byself,
        remain_on_chan_info->long_listen_chan_numer);
    if (mac_work_vap != NULL) {
        oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_SCAN,
            "{hmac_abort_long_same_chan_remain_on_chan:event[%d] abort, continue listen on mac_vap[%d]}",
            reascon_code, mac_work_vap->uc_vap_id);
        hmac_long_same_remain_on_chan_sync_device(mac_vap); /* 同步至device重置监听参数 */
        return;
    }

    /* 已不存在可用于同信道监听的工作vap，结束同信道监听 */
    long_listen_chan_numer = remain_on_chan_info->long_listen_chan_numer;
    mac_set_long_same_remain_on_chan(remain_on_chan_info, mac_chip_get_vap_support_max_num_limit(), 0, OAL_FALSE);
    hmac_long_same_remain_on_chan_sync_device(mac_vap);

    /* 长期同信道监听被打断, 需通知上层决策是否重新下发监听命令 */
    hmac_abort_long_same_chan_remain_on_chan_report(long_listen_chan_numer, reascon_code);
    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_SCAN,
        "{hmac_abort_long_same_chan_remain_on_chan:chan:%d, reason_code:%d}",
        long_listen_chan_numer, reascon_code);
}

void hmac_del_vap_same_chan_remain_handler(mac_vap_stru *mac_vap)
{
    uint8_t listen_chan;
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    if (g_wlan_spec_cfg->feature_same_chan_remain_on_chan_support == OAL_FALSE) {
        return;
    }

    /* 定期监听执行vap被删除 */
    if ((mac_normal_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_TRUE) &&
        (remain_on_chan_info->normal_listen_vap_id == mac_vap->uc_vap_id)) {
        mac_set_normal_same_remain_on_chan(remain_on_chan_info, mac_chip_get_vap_support_max_num_limit(), 0, OAL_FALSE);
        if (remain_on_chan_info->remain_on_chan_timer.en_is_registerd) {
            frw_timer_immediate_destroy_timer_m(&(remain_on_chan_info->remain_on_chan_timer));
        }
        hmac_normal_same_remain_on_chan_sync_device(mac_vap, OAL_FALSE);
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_del_vap_same_chan_remain_handler:normal listen vap is del!}");
    }

    if (mac_long_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_FALSE) {
        return;
    }
    /* 长期监听执行vap被删除 */
    if (remain_on_chan_info->long_listen_vap_id == mac_vap->uc_vap_id) {
        listen_chan = remain_on_chan_info->long_listen_chan_numer;
        mac_set_long_same_remain_on_chan(remain_on_chan_info, mac_chip_get_vap_support_max_num_limit(), 0, OAL_FALSE);
        hmac_long_same_remain_on_chan_sync_device(mac_vap);
        /* 长期同信道监听被打断, 需通知上层决策是否重新下发监听命令 */
        hmac_abort_long_same_chan_remain_on_chan_report(listen_chan, MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_DEL_VAP);
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_del_vap_same_chan_remain_handler:long listen vap is del!, listen_chan[%d]}", listen_chan);
    } else { /* 长期同信道监听还需检测同信道work vap是否被删除, 如果删除并且无其他同信道work vap, 则需中止长期监听 */
        hmac_abort_long_same_chan_remain_on_chan(mac_vap, OAL_FALSE, MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_DEL_VAP);
    }
}
