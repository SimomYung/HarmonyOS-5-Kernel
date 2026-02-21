/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 :低功耗相关命令
 * 创建日期 : 2022年11月19日
 */

#include "oal_types.h"
#include "mac_vap.h"
#include "mac_mib.h"
#include "oal_util.h"
#include "plat_pm_wlan.h"
#include "mac_device.h"
#include "hmac_resource.h"
#include "wal_linux_ioctl.h"
#include "wal_cfg_ioctl.h"
#include "wal_main.h"
#include "hmac_pm.h"
#include "hmac_cali_mgmt.h"
#include "wal_linux_netdev_ops.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_PM_CFG_IOCTL_C

#ifdef WIFI_DEBUG_ENABLE
/*
 * 功能描述  : UAPSD参数配置
 * 1.日    期  : 2020年7月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_set_uapsd_para(mac_vap_stru *mac_vap, uint32_t *params)
{
    uint8_t idx;
    mac_cfg_uapsd_sta_stru uapsd_param;

    uapsd_param.uc_max_sp_len = (uint8_t)params[0];
    if (uapsd_param.uc_max_sp_len > 6) { /* uc_max_sp_len最大为6 */
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_PWR,
            "{wal_set_uapsd_para::uc_max_sp_len[%d] > 6!}", uapsd_param.uc_max_sp_len);
        return OAL_FAIL;
    }

    for (idx = 0; idx < WLAN_WME_AC_BUTT; idx++) {
        uapsd_param.uc_delivery_enabled[idx] = (uint8_t)params[idx + 1];
        uapsd_param.uc_trigger_enabled[idx] = (uint8_t)params[idx + 1];
    }
    mac_vap_set_uapsd_para(mac_vap, &uapsd_param);
    return OAL_SUCC;
}
/*
 * 功能描述  : 低功耗全局使能
 * 1.日    期  : 2020年7月29日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_set_pm_switch(mac_vap_stru *mac_vap, uint32_t *params)
{
    return wal_ioctl_set_pm_switch(mac_vap, params);
}

#define ELP_CONFIG_NUM_MAX 3
#define ELP_CONFIG_FLAG_STEP 10
OAL_STATIC uint32_t wal_hipriv_input_elp_ctrl_flag(mac_pm_debug_cfg_stru *pm_debug_cfg, int8_t *value)
{
    uint8_t elp_ctrl_flag_temp = 0;
    uint8_t elp_ctrl_flag = 0;
    uint8_t flag_index = 0;
    if (oal_strcmp("help", value) == 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug elp [WFSC|CCPU|SCAN]'}");
        return OAL_SUCC;
    }

    elp_ctrl_flag_temp = (uint8_t)oal_atoi(value);

    /* 第二个参数含义为使用字符串表示的二进制 */
    for (flag_index = 0; flag_index < ELP_CONFIG_NUM_MAX; flag_index++) {
        if ((elp_ctrl_flag_temp % ELP_CONFIG_FLAG_STEP) != 0) {
            elp_ctrl_flag |= ((uint8_t)(1 << flag_index));
        }
        elp_ctrl_flag_temp /= ELP_CONFIG_FLAG_STEP;
    }

    pm_debug_cfg->cmd_bit_map |= BIT(MAC_PM_DEBUG_ELP_CTRL);
    pm_debug_cfg->elp_ctrl_flg = elp_ctrl_flag;
    oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_input_elp_ctrl_flag::elp ctrl flag[%x]}", elp_ctrl_flag);
    return OAL_SUCC_GO_ON;
}

OAL_STATIC uint32_t wal_hipriv_input_srb_switch(mac_pm_debug_cfg_stru *pm_debug_cfg, int8_t *value)
{
    uint8_t srb_switch;
    if (oal_strcmp("help", value) == 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug srb [0|1]'}");
        return OAL_SUCC;
    }

    srb_switch = (uint8_t)oal_atoi(value);
    if (srb_switch > 1) {
        oam_error_log1(0, OAM_SF_CFG, "{CMD format::input srb switch val[%d] invalid!!}", srb_switch);
        return OAL_FAIL;
    }
    pm_debug_cfg->cmd_bit_map |= BIT(MAC_PM_DEBUG_SISO_RECV_BCN);
    pm_debug_cfg->uc_srb_switch = srb_switch;
    oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_input_srb_switch::siso recv beacon switch[%d]}", srb_switch);
    return OAL_SUCC_GO_ON;
}

OAL_STATIC uint32_t wal_hipriv_input_dto_switch(mac_pm_debug_cfg_stru *pm_debug_cfg, int8_t *value)
{
    uint8_t dto_switch;
    if (oal_strcmp("help", value) == 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug dto [0|1]'}");
        return OAL_SUCC;
    }

    dto_switch = (uint8_t)oal_atoi(value);
    if (dto_switch > 1) {
        oam_error_log1(0, OAM_SF_CFG, "{CMD format::input dto switch val[%d] invalid!!}", dto_switch);
        return OAL_FAIL;
    }
    pm_debug_cfg->cmd_bit_map |= BIT(MAC_PM_DEBUG_DYN_TBTT_OFFSET);
    pm_debug_cfg->uc_dto_switch = dto_switch;
    oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_input_dto_switch::dyn tbtt offset switch[%d].}", dto_switch);
    return OAL_SUCC_GO_ON;
}

OAL_STATIC uint32_t wal_hipriv_input_nfi_switch(mac_pm_debug_cfg_stru *pm_debug_cfg, int8_t *value)
{
    uint8_t nfi_switch;
    if (oal_strcmp("help", value) == 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug nfi [0|1]'}");
        return OAL_SUCC;
    }

    nfi_switch = (uint8_t)oal_atoi(value);
    if (nfi_switch > 1) {
        oam_error_log1(0, OAM_SF_CFG, "{CMD format::input nfi switch val[%d] invalid!!}", nfi_switch);
        return OAL_FAIL;
    }
    pm_debug_cfg->cmd_bit_map |= BIT(MAC_PM_DEBUG_NO_PS_FRM_INT);
    pm_debug_cfg->uc_nfi_switch = nfi_switch;
    oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_input_nfi_switch::no ps frm int switch[%d].}", nfi_switch);
    return OAL_SUCC_GO_ON;
}

OAL_STATIC uint32_t wal_hipriv_input_pkt_filter_switch(mac_pm_debug_cfg_stru *pm_debug_cfg, int8_t *value)
{
    uint8_t pkt_filter_switch;
    if (oal_strcmp("help", value) == 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug pkt_filter [0|1]'}");
        return OAL_SUCC;
    }

    pkt_filter_switch = (uint8_t)oal_atoi(value);
    if (pkt_filter_switch > 1) {
        oam_error_log1(0, OAM_SF_CFG,
            "{CMD format::input pkt_filter switch val[%d] invalid!!}", pkt_filter_switch);
        return OAL_FAIL;
    }
    pm_debug_cfg->cmd_bit_map |= BIT(MAC_PM_DEBUG_PKT_FILTER);
    pm_debug_cfg->uc_pkt_filter_switch = pkt_filter_switch;
    oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_debug_cmp_switch::pkt_filter switch[%d].}", pkt_filter_switch);
    return OAL_SUCC_GO_ON;
}

OAL_STATIC uint32_t wal_hipriv_input_ao_switch(mac_pm_debug_cfg_stru *pm_debug_cfg, int8_t *value)
{
    uint8_t ao_switch;
    if (oal_strcmp("help", value) == 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug ao [0|1]'}");
        return OAL_SUCC;
    }

    ao_switch = (uint8_t)oal_atoi(value);
    if (ao_switch > 1) {
        oam_error_log1(0, OAM_SF_CFG, "{CMD format::input ao switch val[%d] invalid!!}", ao_switch);
        return OAL_FAIL;
    }
    pm_debug_cfg->cmd_bit_map |= BIT(MAC_PM_DEBUG_AO);
    pm_debug_cfg->uc_ao_switch = ao_switch;
    oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_input_ao_switch::arp offload switch[%d].}", ao_switch);
    return OAL_SUCC_GO_ON;
}

/*
 * 功能描述  : 比较 pm debug 接口 传入的 控制命令
 * 1.日    期  : 2020年11月04日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_hipriv_debug_cmp_switch(mac_pm_debug_cfg_stru *pm_debug_cfg,
    int8_t *value, uint32_t value_len, int8_t *name, uint32_t name_len)
{
    uint32_t ret = OAL_SUCC_GO_ON;
    if (name == NULL || value_len == 0 || name_len == 0 ||
        value_len > WAL_HIPRIV_CMD_VALUE_MAX_LEN || name_len > WAL_HIPRIV_CMD_NAME_MAX_LEN) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_hipriv_debug_cmp_switch::error input!}");
    }
    if (oal_strcmp("srb", name) == 0) {
        ret = wal_hipriv_input_srb_switch(pm_debug_cfg, value);
    } else if (oal_strcmp("dto", name) == 0) {
        ret = wal_hipriv_input_dto_switch(pm_debug_cfg, value);
    } else if (oal_strcmp("nfi", name) == 0) {
        ret = wal_hipriv_input_nfi_switch(pm_debug_cfg, value);
    } else if (oal_strcmp("pkt_filter", name) == 0) {
        ret = wal_hipriv_input_pkt_filter_switch(pm_debug_cfg, value);
    } else if (oal_strcmp("ao", name) == 0) {
        ret = wal_hipriv_input_ao_switch(pm_debug_cfg, value);
    } else if (oal_strcmp("elp", name) == 0) {
        ret = wal_hipriv_input_elp_ctrl_flag(pm_debug_cfg, value);
    } else {
        oam_error_log0(0, OAM_SF_CFG,
            "{CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1] [dto 0|1] [nfi 0|1] [pkt_filter 0|1] \
            [elp wfsc|ccpu|scan]!}");
        return OAL_FAIL;
    }
    if (ret != OAL_SUCC_GO_ON) {
        return ret;
    }
    return OAL_SUCC_GO_ON;
}
/*
 * 功能描述  : pm debug 接口
 * 1.日    期  : 2017年09月07日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_hipriv_set_pm_debug_switch(oal_net_device_stru *net_dev, int8_t *param)
{
    wal_msg_write_stru write_msg;
    mac_vap_stru *mac_vap = NULL;
    uint32_t ret;
    int32_t l_ret;
    uint32_t offset = 0;
    int8_t cmd_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = {0};
    int8_t cfg_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN] = {0};
    mac_pm_debug_cfg_stru *pm_debug_cfg = NULL;
    oal_bool_enum_uint8 cmd_updata = OAL_FALSE;

    /* sh hipriv.sh "wlan0 pm_debug srb " */
    mac_vap = oal_net_dev_priv(net_dev);
    if (mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_pm_debug_switch::mac_vap is null!}");
        return OAL_FAIL;
    }

    pm_debug_cfg = (mac_pm_debug_cfg_stru *)(write_msg.auc_value);
    memset_s(pm_debug_cfg, sizeof(mac_pm_debug_cfg_stru), 0, sizeof(mac_pm_debug_cfg_stru));

    do {
        ret = wal_get_cmd_one_arg(param, cmd_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offset);
        if ((ret != OAL_SUCC) && (offset != 0)) {
            oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::cmd format err, ret:%d;!!}\r\n", ret);
            return ret;
        }
        param += offset;

        if (cmd_updata == OAL_FALSE) {
            cmd_updata = OAL_TRUE;
        } else if (offset == 0) {
            break;
        }

        ret = wal_get_cmd_one_arg(param, cfg_value, WAL_HIPRIV_CMD_VALUE_MAX_LEN, &offset);
        if ((ret != OAL_SUCC) || ((!isdigit(cfg_value[0])) && (strcmp("help", cfg_value) != 0))) {
            oam_error_log0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1] [dto 0|1] [nfi 0|1] \
                [pkt_filter 0|1] [elp wfsc|ccpu|scan]!}");
            return ret;
        }
        param += offset;
        ret = wal_hipriv_debug_cmp_switch(pm_debug_cfg, cfg_value, sizeof(cfg_value), cmd_name, sizeof(cmd_name));
        if (ret != OAL_SUCC_GO_ON) {
            return ret;
        }
    } while (*param != '\0');

    /***************************************************************************
                               抛事件到wal层处理
    ***************************************************************************/
    wal_write_msg_hdr_init(&write_msg, WLAN_CFGID_PM_DEBUG_SWITCH, sizeof(mac_pm_debug_cfg_stru));

    l_ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_pm_debug_cfg_stru), (uint8_t *)&write_msg, OAL_FALSE, NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_dbdc_debug_switch::return err code[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#endif

uint32_t wal_ioctl_set_pm_switch(mac_vap_stru *mac_vap, uint32_t *params)
{
    uint8_t switch_pm = (uint8_t)params[0];

    oam_warning_log1(0, OAM_SF_PWR, "{wal_ioctl_set_pm_switch:[%d]}\r\n", switch_pm);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wlan_pm_set_host_pm_switch(switch_pm);
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    g_wlan_device_pm_switch = switch_pm;
#endif
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_ioctl_set_fast_sleep_para
 * 功能描述  : 下发fastsleep参数
 * 1.日    期  : 2019年4月17日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_ioctl_set_fast_sleep_para(oal_net_device_stru *cfg_net_dev, int8_t *param, uint8_t len)
{
    uint32_t cmd_id;
    uint32_t offset = 0;
    int32_t params[CMD_PARAMS_MAX_CNT] = {0};
    uint32_t ret;
    uint8_t idx;

    ret = wal_get_cmd_id("set_fast_sleep_para", &cmd_id, &offset);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "wal_ioctl_set_fast_sleep_para:find set_fast_sleep_para cmd is fail");
        return ret;
    }
     /* 获取携带的4个参数<min listen时间><max listen时间><亮屏收发包门限><暗屏收发包门限> */
    for (idx = 0; idx < len; idx++) {
        params[idx] = (int32_t)param[idx];
    }
    return wal_process_cmd_params(cfg_net_dev, cmd_id, params);
}

/*
 * 函 数 名  : wal_ioctl_set_fastsleep3_para
 * 功能描述  : 下发fastsleep3参数
 */
uint32_t wal_ioctl_set_fastsleep3_para(oal_net_device_stru *cfg_net_dev, int8_t *pc_param, uint8_t len)
{
    uint32_t cmd_id;
    uint32_t offset = 0;
    int32_t params[CMD_PARAMS_MAX_CNT] = {0};
    uint32_t ret;
    uint8_t idx;

    ret = wal_get_cmd_id("set_fastsleep3_para", &cmd_id, &offset);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "wal_ioctl_set_fastsleep3_para:find set_fast_sleep_para cmd is fail");
        return ret;
    }
    for (idx = 0; idx < len; idx++) {
        params[idx] = (int32_t)pc_param[idx];
    }
    return wal_process_cmd_params(cfg_net_dev, cmd_id, params);
}
/*
 * 函 数 名  : wal_ioctl_set_sta_ps_mode
 * 功能描述  : 配置ps mode
 * 1.日    期  : 2019年04月17日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_ioctl_set_sta_ps_mode(oal_net_device_stru *cfg_net_dev,
    int8_t ps_mode)
{
    uint32_t cmd_id;
    uint32_t params[CMD_PARAMS_MAX_CNT] = {0};
    uint32_t offset = 0;
    uint32_t ret;

    ret = wal_get_cmd_id("set_ps_mode", &cmd_id, &offset);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "wal_ioctl_set_sta_ps_mode:find set_ps_mode cmd is fail");
        return ret;
    }
    params[0] = (uint32_t)ps_mode;
    return wal_process_cmd_params(cfg_net_dev, cmd_id, params);
}

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
int32_t wal_hipriv_sta_pm_on_set_rx_listen(oal_net_device_stru *net_dev, uint8_t pm_en)
{
    wal_msg_write_stru write_msg_powersaving = { 0 };
    mac_rx_listen_ps_switch_stru *rx_listen_ps_switch = NULL;
    wal_write_msg_hdr_init(&write_msg_powersaving, WLAN_CFGID_RX_LISTEN_PS_SWITCH,
        sizeof(mac_rx_listen_ps_switch_stru));
    rx_listen_ps_switch = (mac_rx_listen_ps_switch_stru *)write_msg_powersaving.auc_value;
    rx_listen_ps_switch->rx_listen_enable = pm_en;
    rx_listen_ps_switch->rx_listen_ctrl_type = MAC_RX_LISTEN_CFG80211_POWER_MGMT;
    return wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE,
                              WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_rx_listen_ps_switch_stru),
                              (uint8_t *)&write_msg_powersaving, OAL_FALSE, NULL);
}
#endif

/*
 * 功能描述  : 私有命令,sta psm的listen interval / tbtt offset
 * 1.日    期  : 2015年5月3日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_hipriv_sta_pm_on(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = {0};
    uint32_t ret;
    int32_t l_ret;
    uint8_t uc_sta_pm_open;
    mac_cfg_ps_open_stru *pst_sta_pm_open = NULL;

    ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_pm_open::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ret);
        return ret;
    }

    uc_sta_pm_open = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + off_set;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    wal_write_msg_hdr_init(&st_write_msg, WLAN_CFGID_SET_STA_PM_ON, sizeof(mac_cfg_ps_open_stru));

    /* 设置配置命令参数 */
    pst_sta_pm_open = (mac_cfg_ps_open_stru *)(st_write_msg.auc_value);
    /* MAC_STA_PM_SWITCH_ON / MAC_STA_PM_SWITCH_OFF */
    pst_sta_pm_open->uc_pm_enable = uc_sta_pm_open;
    pst_sta_pm_open->uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_CMD;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_cfg_ps_open_stru),
                               (uint8_t *)&st_write_msg, OAL_FALSE, NULL);
#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    l_ret += wal_hipriv_sta_pm_on_set_rx_listen(pst_cfg_net_dev, uc_sta_pm_open);
#endif
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_sta_pm_open::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
/*
 * 功能描述  : CMD_SET_POWER_ON命令
 * 1.日    期  : 2017年7月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
int32_t wal_set_power_on(oal_net_device_stru *net_dev, int32_t power_flag)
{
    int32_t ret;

    // ap上下电，配置VAP
    if (power_flag == 0) { // 下电
        /* 下电host device_stru去初始化 */
        wal_host_dev_exit(net_dev);
        wal_wake_lock();
#ifndef _PRE_EMU /* EMU平台上下电速度慢，再ap场景下，emu平台不下电 */
        wlan_pm_close_dev(wal_get_net_device_id(net_dev));
#endif
#ifdef _PRE_WLAN_EXPORT
        g_hitalk_set = 0;
#endif
        wal_wake_unlock();
    } else if (power_flag == 1) {  // 上电
        wal_wake_lock();
        ret = wlan_pm_open_dev(wal_get_net_device_id(net_dev), OAL_TRUE);
        wal_wake_unlock();
        if (ret == OAL_FAIL) {
            oam_error_log0(0, OAM_SF_ANY, "{wal_set_power_on::wlan_pm_open Fail!}");
            return -OAL_EFAIL;
        } else if (ret != OAL_ERR_CODE_ALREADY_OPEN) {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
            /* 重新上电时置为FALSE */
            hwifi_config_init_force();
#endif
            // 重新上电场景，下发配置VAP
            ret = wal_cfg_vap_h2d_event(net_dev);
            if (ret != OAL_SUCC) {
                return -OAL_EFAIL;
            }
        }

        /* 上电host device_stru初始化 */
        ret = wal_host_dev_init(net_dev);
        if (ret != OAL_SUCC) {
            oal_io_print("wal_set_power_on FAIL %d \r\n", ret);
            return -OAL_EFAIL;
        }
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_set_power_on::pupower_flag:%d error.}", power_flag);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

/*
 * 功能描述  : CMD_SET_POWER_MGMT_ON命令
 * 1.日    期  : 2017年7月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void wal_set_power_mgmt_on(oal_net_device_stru *net_dev, uint8_t power_mgmt_flag)
{
    struct wlan_pm_s *wlan_pm = NULL;
    wlan_pm = wal_netdev_pm_get_dev(net_dev);
    if (wlan_pm != NULL) {
        /* ap模式下，是否允许下电操作,1:允许,0:不允许 */
        wlan_pm->apmode_allow_pm_flag = power_mgmt_flag;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_set_power_mgmt_on::wlan_pm_get_drv return null.");
    }
}

uint32_t wal_set_close_fem_cali_status(mac_vap_stru *mac_vap, uint32_t *params)
{
    uint8_t value = (params[0] == 0) ? 0 : 1;
    hmac_set_close_fem_cali_status(value);
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_RR_PERFORMANCE
/*
 * 功能描述  : 调频参数检查
 * 1.日    期  : 2019年11月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_ext_priv_cmd_dev_freq_check_para(oal_net_device_stru *net_dev)
{
    if (oal_any_null_ptr1(net_dev)) {
        oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_dev_freq_check_para::null_ptr.");
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

/*
 * 功能描述  : 调频参数填充
 * 1.日    期  : 2019年11月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_priv_cmd_dev_freq_para(mac_vap_stru *mac_vap, mac_cfg_set_tlv_stru *config_para,
                                              uint8_t cmd_type, uint8_t value)
{
    int32_t     ret;
    config_para->uc_cmd_type = cmd_type;
    config_para->value    = value;
    config_para->uc_len      = sizeof(mac_cfg_set_tlv_stru);
    config_para->us_cfg_id   = WLAN_CFGID_SET_DEVICE_FREQ;

    ret = hmac_config_set_tlv_cmd(mac_vap, 0, (uint8_t *)config_para);
    oam_error_log3(0, OAM_SF_CFG, "hmac_config_set_tlv_cmd[%d,%d] ret:[%d]!\n", cmd_type, value, ret);
    return ret;
}

/*
 * 功能描述  : 调频。
 * 1.日    期  : 2019年11月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_ext_priv_cmd_set_dev_freq(oal_net_device_stru *net_dev, uint8_t flag)
{
    mac_cfg_set_tlv_stru *config_para = NULL;
    mac_vap_stru *mac_vap = NULL;
    int32_t result;

    result = wal_ext_priv_cmd_dev_freq_check_para(net_dev);
    if (result != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_dev_freq_check_para fail!\n");
        return result;
    }
    mac_vap = oal_net_dev_priv(net_dev);
    if (mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_ext_priv_cmd_set_dev_freq::vap is null.}");
        return -OAL_EINVAL;
    }

    /* 申请内存 */
    config_para = (mac_cfg_set_tlv_stru *)oal_memalloc(sizeof(mac_cfg_set_tlv_stru));
    if (oal_unlikely(config_para == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::config_para malloc fail!\n");
        return -OAL_EFAIL;
    }
    memset_s((void *)config_para, sizeof(mac_cfg_set_tlv_stru), 0, sizeof(mac_cfg_set_tlv_stru));

    if (flag == FREQ_AUTO_FLAG) {
        result = wal_priv_cmd_dev_freq_para(mac_vap, config_para, CMD_SET_AUTO_FREQ_ENDABLE, FREQ_LOCK_ENABLE);
        if (result != OAL_SUCC) {
            oal_free(config_para);
            config_para = NULL;
            oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_fill_para fail!\n");
            return result;
        }
    } else if (flag == FREQ_MAX_FLAG) {
        result = wal_priv_cmd_dev_freq_para(mac_vap, config_para, CMD_SET_AUTO_FREQ_ENDABLE, FREQ_LOCK_DISABLE);
        if (result != OAL_SUCC) {
            oal_free(config_para);
            config_para = NULL;
            oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_fill_para fail!\n");
            return result;
        }

        result = wal_priv_cmd_dev_freq_para(mac_vap, config_para, CMD_SET_DEVICE_FREQ_VALUE, FREQ_MAX_VALUE);
        if (result != OAL_SUCC) {
            oal_free(config_para);
            config_para = NULL;
            oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_fill_para fail!\n");
            return result;
        }
    }

    oal_free(config_para);
    config_para = NULL;
    oam_warning_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::success!\n");
    return OAL_SUCC;
}

/*
 * 功能描述  : aspm命令
 * 1.日    期  : 2019年11月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_ext_priv_cmd_pcie_aspm(int32_t aspm)
{
    if (aspm == OAL_FALSE) {
        g_freq_lock_control.hcc_aspm_close_bitmap |= BIT(ASPM_CALL_EXT_PRIV_CMD);
    } else {
        g_freq_lock_control.hcc_aspm_close_bitmap &= ~BIT(ASPM_CALL_EXT_PRIV_CMD);
    }
    return mpxx_hcc_ip_pm_ctrl(aspm, HCC_EP_WIFI_DEV);
}

/*
 * 功能描述  : 扩展的私有命令处理
 * 1.日    期  : 2019年11月18日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
int32_t wal_ext_priv_cmd(oal_net_device_stru *net_dev, oal_ifreq_stru *pst_ifr)
{
    wal_wifi_pc_cmd_stru  priv_cmd = {0};
    int32_t               ret = OAL_SUCC;

    if (oal_any_null_ptr2(pst_ifr->ifr_data, net_dev)) {
        return -OAL_EINVAL;
    }
    /* 将用户态数据拷贝到内核态 */
    if (oal_copy_from_user(&priv_cmd, pst_ifr->ifr_data, sizeof(wal_wifi_pc_cmd_stru))) {
        return -OAL_EINVAL;
    }

    switch (priv_cmd.cmd) {
        case WAL_EXT_PRI_CMD_SET_DEVICE_FREQ_MAX:
            ret = wal_ext_priv_cmd_set_dev_freq(net_dev, FREQ_MAX_FLAG);
            break;
        case WAL_EXT_PRI_CMD_SET_DEVICE_FREQ_AUTO:
            ret = wal_ext_priv_cmd_set_dev_freq(net_dev, FREQ_AUTO_FLAG);
            break;
        case WAL_EXT_PRI_CMD_ENBLE_PCIE_ASPM:
            ret = wal_ext_priv_cmd_pcie_aspm(ASPM_ENABLE);
            oam_warning_log2(0, OAM_SF_CFG, "wal_ext_priv_cmd_pcie_aspm::cmd = %d, ret = %d\n", ASPM_ENABLE, ret);
            break;
        case WAL_EXT_PRI_CMD_DISABLE_PCIE_ASPM:
            ret = wal_ext_priv_cmd_pcie_aspm(ASPM_DISABLE);
            oam_warning_log2(0, OAM_SF_CFG, "wal_ext_priv_cmd_pcie_aspm::cmd = %d, ret = %d\n", ASPM_DISABLE, ret);
            break;
        default:
            break;
    }
    chr_exception_p(CHR_WIFI_IWARE_RR_EVENTID, (uint8_t *)&priv_cmd.cmd, sizeof(uint32_t));
    return ret;
}

#endif /* _PRE_WLAN_RR_PERFORMANCE */

uint8_t wal_fastsleep3_check_param_can_update(const mac_vap_stru *mac_vap)
{
    mac_device_stru *mac_device = mac_res_get_mac_dev();
    mac_vap_stru *up_vap_array[WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE] = { NULL };
    hmac_vap_stru *hmac_vap = NULL;
    uint32_t up_vap_num;
    uint32_t idx;

    if (mac_vap == NULL) {
        return OAL_FALSE;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_FALSE;
    }

    /* 当前vap没有UP, 不下发 */
    if (mac_vap->en_vap_state != MAC_VAP_STATE_UP) {
        return OAL_FALSE;
    }

    /* 当前为多vap, 且有AP不下发 */
    up_vap_num = mac_device_find_up_vaps(mac_device, up_vap_array, WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE);
    for (idx = 0; idx < up_vap_num; idx++) {
        mac_vap_stru *tmp_vap = up_vap_array[idx];
        if (tmp_vap != NULL && is_ap(tmp_vap) == OAL_TRUE) {
            return OAL_FALSE;
        }
    }

#ifdef _PRE_WLAN_FEATURE_WUR
    /* 快睡和wur互斥 */
    if (hmac_vap->wur_info.is_enabled == OAL_TRUE) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "wal_fastsleep3_check_param::wur is enabled, NOT enter fs3");
        return OAL_FALSE;
    }
#endif

    return OAL_TRUE;
}

void wal_set_fastsleep3_para(oal_net_device_stru *cfg_net_dev, uint8_t *params, uint8_t len)
{
    hmac_vap_stru *hmac_vap = NULL;
    fastsleep3_param_stru *fastsleep3_param;
    mac_vap_stru *mac_vap = NULL;
    if (cfg_net_dev == NULL || params == NULL) {
        return;
    }

    mac_vap = oal_net_dev_priv(cfg_net_dev);
    if (mac_vap == NULL) {
        return;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        return;
    }
    fastsleep3_param = &hmac_vap->fastsleep3_param;
    if (memcpy_s(fastsleep3_param, sizeof(fastsleep3_param_stru), params, len) != EOK) {
        oam_warning_log0(0, OAM_SF_CFG, "wal_set_fastsleep3_para:memcpy buf fail");
        return;
    }
    oam_warning_log3(0, OAM_SF_CFG, "wal_set_fastsleep3_para:fastsleep3 t0[%d] t1[%d] t2[%d]!",
        fastsleep3_param->fastsleep3_start_listen_time, fastsleep3_param->fastsleep3_sleep_rtc_time,
        fastsleep3_param->fastsleep3_rtc_wakeup_work_time);
    oam_warning_log2(0, OAM_SF_CFG, "wal_set_fastsleep3_para:fastsleep3 total_rx[%d] t2_rx[%d]!",
        fastsleep3_param->fastsleep3_total_rx_cnt_per_period, fastsleep3_param->fastsleep3_wakeup_rx_cnt_per_period);
    return;
}

void wal_set_fastsleep2_mode(oal_net_device_stru *cfg_net_dev, uint8_t enable)
{
    hmac_vap_stru *hmac_vap = NULL;
    mac_vap_stru *mac_vap = NULL;
    if (cfg_net_dev == NULL) {
        return;
    }

    mac_vap = oal_net_dev_priv(cfg_net_dev);
    if (mac_vap == NULL) {
        return;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        return;
    }
    hmac_vap->fastsleep_en = enable;
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_PWR,
                     "wal_set_fastsleep2_mode ps_mode[%d]",
                     hmac_vap->fastsleep_en);
}

void wal_set_fastsleep3_mode(oal_net_device_stru *cfg_net_dev, uint8_t enable)
{
    hmac_vap_stru *hmac_vap = NULL;
    mac_vap_stru *mac_vap = NULL;
    if (cfg_net_dev == NULL) {
        return;
    }

    mac_vap = oal_net_dev_priv(cfg_net_dev);
    if (mac_vap == NULL) {
        return;
    }

    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        return;
    }
    hmac_vap->fastsleep3_en = enable;
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_PWR,
                     "wal_set_fastsleep3_mode ps_mode[%d]",
                     hmac_vap->fastsleep3_en);
}

void wal_clear_fastsleep_param(oal_net_device_stru *cfg_net_dev)
{
    mac_vap_stru *mac_vap = NULL;
    if (cfg_net_dev == NULL) {
        return;
    }

    mac_vap = oal_net_dev_priv(cfg_net_dev);
    if (mac_vap == NULL) {
        return;
    }
    hmac_fastsleep_clear_param(mac_vap);
}