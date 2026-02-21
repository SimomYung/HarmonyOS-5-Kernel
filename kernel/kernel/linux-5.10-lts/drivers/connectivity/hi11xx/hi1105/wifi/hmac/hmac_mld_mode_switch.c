/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明   : hmac multilink 模式切换模块函数
 * 作    者   : duankaiyong
 * 创建日期   : 2024年7月5日
 */
#include "hmac_mld_ext.h"
#include "hmac_mld_vap.h"
#include "hmac_cali_mgmt.h"
#include "hmac_config.h"
#include "hmac_device.h"
#include "hmac_tx_ring_alloc.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MLD_MODE_SWITCH_C

#ifdef _PRE_WLAN_FEATURE_ML

static void hmac_mld_set_mode_switch_state(hmac_device_stru *hmac_device,
    mac_mld_mode_switch_enum mld_mode_switch_state)
{
    hmac_device->mld_mode_switch_state = mld_mode_switch_state;
}

static mac_mld_mode_switch_enum hmac_mld_get_mode_switch_state(hmac_device_stru *hmac_device)
{
    return hmac_device->mld_mode_switch_state;
}

void hmac_mld_reset_mode_switch_state(hmac_vap_stru *hmac_vap)
{
    hmac_device_stru *hmac_device = NULL;

    if (hmac_vap == NULL) {
        return;
    }

    hmac_device = hmac_res_get_mac_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (hmac_device == NULL) {
        return;
    }

    /* 非MLD vap 不处理 */
    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        return;
    }

    hmac_device->mld_mode_switch_state = MLD_MODE_SWITCH_INIT;
}

static uint32_t hmac_mld_mode_switch_check_input_params(hmac_vap_stru *hmac_vap,
    mac_mld_mode_switch_enum next_mld_mode)
{
    hmac_device_stru *hmac_device = NULL;
    if (hmac_vap == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_config_mld_mode_switch::hamc_vap or mld_vap_manage is null");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_config_mld_mode_switch::not mld vap %d.}", hmac_vap_get_mld_mode(hmac_vap));
        return OAL_FAIL;
    }

    hmac_device = hmac_res_get_mac_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (hmac_device == NULL) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "hmac_config_mld_mode_switch::hmac_device %d is null.",
            hmac_vap->st_vap_base_info.uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (hmac_mld_get_mode_switch_state(hmac_device) == next_mld_mode) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "hmac_config_mld_mode_switch::same mld mode %d, not need do anything.",
            next_mld_mode);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    if (hmac_vap->pst_net_device == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_mld_mode_switch_enter_mlsr_process::netdev null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    return OAL_SUCC;
}

static uint32_t hmac_mld_switch_find_vap_to_emlsr(hmac_vap_stru *hmac_vap,
    hmac_mld_manage_stru *mld_vap_manage,
    mac_mld_mode_switch_stru *mld_mode_switch)
{
    hmac_vap_stru *link_vap = hmac_mld_get_another_vap(hmac_vap);

    /* device抛事件指示的是发送omn帧的vap，emlsr进入时该vap主link，emlsr退出时该vap为保留的mlsr工作vap */
    if (link_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_mld_switch_find_vap_to_emlsr::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_mode_switch->vap_id = link_vap->st_vap_base_info.uc_vap_id;
    mld_mode_switch->next_vap_state = MAC_VAP_STATE_UP;
    return OAL_SUCC;
}

static uint32_t hmac_mld_switch_find_vap_to_str(hmac_vap_stru *hmac_vap,
    hmac_mld_manage_stru *mld_vap_manage,
    mac_mld_mode_switch_stru *mld_mode_switch)
{
    hmac_vap_stru *link_vap = hmac_mld_get_link_vap(hmac_vap);
    if (link_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    mld_mode_switch->vap_id = link_vap->st_vap_base_info.uc_vap_id;
    mld_mode_switch->next_vap_state = MAC_VAP_STATE_UP;
    return OAL_SUCC;
}

static uint32_t hmac_mld_switch_find_vap_to_mlsr(hmac_vap_stru *hmac_vap,
    hmac_mld_manage_stru *mld_vap_manage,
    mac_mld_mode_switch_stru *mld_mode_switch)
{
    hmac_vap_stru *link_vap = hmac_mld_get_another_vap(hmac_vap);

    /* device抛事件指示的是发送omn帧的vap，emlsr进入时该vap主link，emlsr退出时该vap为保留的mlsr工作vap */
    if (link_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_mld_switch_find_vap_to_emlsr::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_mode_switch->vap_id = link_vap->st_vap_base_info.uc_vap_id;
    mld_mode_switch->next_vap_state = MAC_VAP_STATE_STA_LINK_DOWN;
    return OAL_SUCC;
}

/* 功能描述：根据MLD 模式，查找并返回需要状态变化的vap_id */
static uint32_t hmac_mld_switch_find_switch_vap(hmac_vap_stru *hmac_vap,
    mac_mld_mode_switch_stru *mld_mode_switch)
{
    uint32_t ret;
    hmac_mld_manage_stru *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;

    switch (mld_mode_switch->next_mld_mode) {
        case MLD_MODE_SWITCH_EMLSR:
            ret = hmac_mld_switch_find_vap_to_emlsr(hmac_vap, mld_vap_manage, mld_mode_switch);
            return ret;
        case MLD_MODE_SWITCH_STR:
            /* 切换到STR模式，传入的是work_vap，找到非工作LINK_VAP转换成UP */
            ret = hmac_mld_switch_find_vap_to_str(hmac_vap, mld_vap_manage, mld_mode_switch);
            return ret;
        case MLD_MODE_SWITCH_MLSR:
            /* 切换到MLSR模式, 需要当前两个vap 都是work，传入的vap 变成link_down */
            ret = hmac_mld_switch_find_vap_to_mlsr(hmac_vap, mld_vap_manage, mld_mode_switch);
            return ret;
        default:
            oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
                "{hmac_mld_switch_find_switch_vap::invalid next_mld_mode %d.}",
                mld_mode_switch->next_mld_mode);
            break;
    }
    return OAL_FAIL;
}

static void hmac_mld_mode_switch_send_cali_data(mac_mld_mode_switch_stru *mld_mode_switch)
{
    hmac_vap_stru *link_vap = NULL;

    if (mld_mode_switch->next_mld_mode == MLD_MODE_SWITCH_STR ||
        mld_mode_switch->next_mld_mode == MLD_MODE_SWITCH_EMLSR) {
        /* 进入STR/EMLSR模式，提前下发校准数据, 避免device 启动link-down vap 时没有校准数据 */
        link_vap = mac_res_get_hmac_vap(mld_mode_switch->vap_id);
        if (link_vap == NULL) {
            oam_error_log1(0, OAM_SF_MLD,
                "{hmac_mld_mode_switch_send_cali_data::link_vap %d null.}",
                mld_mode_switch->vap_id);
            return;
        }

        hmac_cali_send_work_channel_cali_data(&link_vap->st_vap_base_info,
            &link_vap->st_vap_base_info.st_channel, CALI_DATA_NORMAL_JOIN_TYPE);
    }
}

static void hmac_mld_mode_switch_enter_mlsr_process(hmac_vap_stru *hmac_vap, hmac_vap_stru *hmac_switch_vap,
    mac_mld_mode_switch_stru *mld_mode_switch)
{
    hmac_vap_stru *hmac_another_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    oal_net_device_stru *netdev = hmac_vap_get_net_device(hmac_vap->st_vap_base_info.uc_vap_id);

    /* 切换到MLSR,且是link_down,需要交换work_vap和link_vap */
    if (hmac_switch_vap->mld_vap_link_info.mld_mode != WLAN_ML_LINK_MODE ||
        mld_mode_switch->next_mld_mode != MLD_MODE_SWITCH_MLSR ||
        mld_mode_switch->next_vap_state != MAC_VAP_STATE_STA_LINK_DOWN) {
        return;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    hmac_another_vap = hmac_mld_get_another_vap(hmac_vap);
    if (hmac_another_vap == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_mld_mode_switch_enter_mlsr_process::hmac_another_vap null.}");
        return;
    }

    oal_net_tx_stop_all_queues(netdev); /* 暂停发送队列 */
    /* 更新vap user的mld参数 */
    hmac_mld_change_vap_user_mld_param(hmac_vap, hmac_another_vap, mld_vap_manage);
    /* 交换netdev<--->vap指针关系 */
    hmac_mlsr_change_netdev_vap_relation(hmac_vap, hmac_another_vap);
    oal_net_tx_wake_all_queues(netdev); /* 启动发送队列 */
}

uint32_t hmac_config_mld_mode_switch(hmac_vap_stru *hmac_vap, mac_mld_mode_switch_enum next_mld_mode)
{
    hmac_vap_stru *hmac_switch_vap = NULL;
    hmac_device_stru *hmac_device = NULL;
    mac_mld_mode_switch_stru mld_mode_switch = {
        .next_mld_mode = next_mld_mode,
    };
    uint32_t ret;

    ret = hmac_mld_mode_switch_check_input_params(hmac_vap, next_mld_mode);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = hmac_mld_switch_find_switch_vap(hmac_vap, &mld_mode_switch);
    if (ret != OAL_SUCC) {
        oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "hmac_config_mld_mode_switch::not find switch vap. next_mld_mode %d, result %d",
            next_mld_mode, ret);
        return ret;
    }

    /* MLD 模式切换完成后，更新状态变化的VAP vap_state */
    hmac_switch_vap = mac_res_get_hmac_vap(mld_mode_switch.vap_id);
    if (hmac_switch_vap == NULL) {
        oam_error_log0(mld_mode_switch.vap_id, OAM_SF_MLD, "{hmac_config_mld_mode_switch::hmac_switch_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
        "hmac_config_mld_mode_switch::switch vap_id %d, mld_mode_switch %d, next_vap_state %d",
        mld_mode_switch.vap_id, mld_mode_switch.next_mld_mode, mld_mode_switch.next_vap_state);

    hmac_mld_mode_switch_send_cali_data(&mld_mode_switch);

    hmac_config_send_event(&(hmac_vap->st_vap_base_info), WLAN_CFGID_MLD_MODE_SWITCH,
        sizeof(mac_mld_mode_switch_stru), (uint8_t *)&mld_mode_switch);

    /* 模式切换，从STR/EMSR进入MLSR 模式，配置工作vap和非工作vap */
    hmac_mld_mode_switch_enter_mlsr_process(hmac_vap, hmac_switch_vap, &mld_mode_switch);

    mac_vap_state_change(&hmac_switch_vap->st_vap_base_info, mld_mode_switch.next_vap_state);

    hmac_mld_set_mld_assoc_mode(hmac_vap,
        (mld_mode_switch.next_mld_mode == MLD_MODE_SWITCH_STR) ? MLD_ASSOC_MODE_STR : MLD_ASSOC_MODE_MLSR);

    hmac_device = hmac_res_get_mac_dev(hmac_vap->st_vap_base_info.uc_device_id);
    hmac_mld_set_mode_switch_state(hmac_device, next_mld_mode);

    return ret;
}

uint32_t hmac_config_mld_mode_switch_cmd(mac_vap_stru *mac_vap, uint32_t *param)
{
    hmac_vap_stru *hmac_vap = NULL;
    uint32_t next_mld_mode = param[0];
    mac_mld_mode_switch_stru mld_mode_switch = {
        .next_mld_mode = next_mld_mode,
    };

    if (mac_vap == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_config_mld_mode_switch::mac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_MLD, "{hmac_config_mld_mode_switch::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (next_mld_mode == 0) {
        /* 调试命令，抛事件到device输出当前mld_fsm 状态 */
        hmac_config_send_event(mac_vap, WLAN_CFGID_MLD_MODE_SWITCH,
            sizeof(mac_mld_mode_switch_stru), (uint8_t *)&mld_mode_switch);
        return OAL_SUCC;
    }

    hmac_config_mld_mode_switch(hmac_vap, next_mld_mode);
    return OAL_SUCC;
}

void hmac_mld_mode_switch_complete_update_user_tx_ring_bitmap(hmac_vap_stru *hmac_vap,
    mac_mld_mode_switch_enum next_mld_mode)
{
    hmac_user_stru *hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);

    /* mld_assoc_mode变化,修改host user tx_ring_bitmap */
    if (hmac_user == NULL) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "hmac_mld_mode_switch_complete_update_user_tx_ring_bitmap:: user %d is NULL",
            hmac_vap->st_vap_base_info.us_assoc_vap_id);
        return;
    }

    if (next_mld_mode == MLD_MODE_SWITCH_MLSR) {
        hmac_tx_ring_set_mld_bitmap(hmac_user, HAL_MULTILINK_TX_BITMAP_LINK0);
    } else {
        hmac_tx_ring_set_mld_bitmap(hmac_user, HAL_MULTILINK_TX_BITMAP_ALL_LINK);
    }
}

#endif
