/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_vap.c
 * 作    者 :
 * 创建日期 : 2024年11月15日
 */

/* 1 头文件包含 */
#include "dpe_wlan_vap.h"
#include "dpe_wlan_resource.h"
#include "dpe_wlan_stat.h"
#include "oam_ext_if.h"
#include "dpe_wlan_rx_reorder.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DPE_WLAN_VAP_C

/*
 * 函 数 名  : dpe_wlan_vap_get
 * 功能描述  : 获取对应DPE VAP
 * 1.日    期  : 2024年11月21日
 *   修改内容  : 新生成函数
 */
dpe_wlan_vap_stru *dpe_wlan_vap_get(uint16_t us_idx)
{
    if (us_idx >= WLAN_VAP_MAX_NUM_LIMIT) {
        oam_error_log1(0, 0, "{dpe_wlan_vap_get::dpe_vap null,vap_idx=%d.}", us_idx);
        return NULL;
    }
    return (dpe_wlan_vap_stru *)(&g_dpe_ast_wlan_res.dpe_wlan_vap_res[us_idx]);
}

OAL_STATIC void dpe_wlan_vap_init(dpe_wlan_vap_stru *dpe_vap)
{
    uint32_t loop;
    memset_s(dpe_vap, sizeof(dpe_wlan_vap_stru), 0, sizeof(dpe_wlan_vap_stru));
    oal_atomic_set(&dpe_vap->tx_suspend_user_cnt, 0);
    for (loop = 0; loop < DPE_VAP_USER_HASH_MAX_VALUE; loop++) {
        oal_dlist_init_head(&(dpe_vap->ast_user_hash[loop]));
    }
    /* cache user 锁初始化 */
    oal_spin_lock_init(&dpe_vap->st_cache_user_lock);
    oal_dlist_init_head(&dpe_vap->st_mac_user_list_head);
    dpe_vap->us_cache_user_id = g_wlan_spec_cfg->invalid_user_id;
    dpe_vap->us_assoc_vap_id = g_wlan_spec_cfg->invalid_user_id;
}

#ifdef _PRE_WLAN_FEATURE_ML
OAL_STATIC void dpe_wlan_mld_vap_init(dpe_wlan_vap_stru *dpe_vap, mac_cfg_add_vap_param_stru *vap_param)
{
    if (vap_param->mld_vap_param.mld_mode == WLAN_ML_MODE || vap_param->mld_vap_param.mld_mode == WLAN_ML_LINK_MODE) {
        dpe_vap->mld_assoc_mode = MLD_ASSOC_MODE_MLSR;
#ifdef _PRE_WLAN_FEATURE_EMLSR
        dpe_vap->emlsr_state = EMLSR_CLOSED;
#endif
    }
}
#endif

void dpe_wlan_config_add_vap(uint8_t *param, uint8_t dev_idx, uint8_t chip_id, uint32_t core_id)
{
    mac_cfg_add_vap_param_stru *vap_param;
    dpe_wlan_vap_stru *dpe_vap;

    if (param == NULL) {
        oam_error_log0(0, 0, "{dpe_wlan_config_add_vap::param is NULL!}");
        return;
    }

    vap_param = (mac_cfg_add_vap_param_stru *)param;
    dpe_vap = dpe_wlan_vap_get(vap_param->uc_vap_id);
    if (dpe_vap == NULL) {
        return;
    }

    dpe_wlan_vap_init(dpe_vap);
    dpe_wlan_multi_user_add(vap_param->us_muti_user_id, vap_param->uc_vap_id, vap_param->multi_user_lut_idx);
    /* 初始化重排序超时时间 */
    dpe_vap_init_set_rx_reorder_timeout(dpe_vap);
    dpe_vap->pst_net_device = vap_param->pst_net_dev;
    dpe_vap->uc_vap_id = vap_param->uc_vap_id;
    dpe_vap->uc_chip_id = chip_id;
    dpe_vap->hal_dev_id = vap_param->uc_dst_hal_dev_id;
    dpe_vap->us_multi_user_idx = vap_param->us_muti_user_id;
    dpe_vap->en_vap_mode = vap_param->en_vap_mode;
    dpe_vap->en_p2p_mode = vap_param->en_p2p_mode;
    dpe_vap->core_id = core_id;
    dpe_vap->uc_device_id = dev_idx;
#ifdef _PRE_WLAN_FEATURE_ML
    dpe_wlan_mld_vap_init(dpe_vap, vap_param);
#endif
}

void dpe_wlan_config_start_vap(uint16_t us_idx, mac_cfg_start_vap_param_stru *start_vap_param)
{
    dpe_wlan_vap_stru *dpe_vap;

    if (start_vap_param == NULL) {
        return;
    }
    dpe_vap = dpe_wlan_vap_get(us_idx);
    if (dpe_vap == NULL) {
        return;
    }

    dpe_vap->en_p2p_mode = start_vap_param->en_p2p_mode;
}

void dpe_wlan_config_down_vap(uint8_t vap_idx, mac_cfg_down_vap_param_stru *down_vap_param)
{
}

void dpe_wlan_config_del_vap(uint8_t vap_idx, uint8_t *param, oal_net_device_stru *net_dev)
{
    dpe_wlan_vap_stru *dpe_vap;

    if (param == NULL) {
        oam_error_log0(0, 0, "{dpe_wlan_config_del_vap::param is NULL!}");
        return;
    }
    dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }
    dpe_vap->pst_net_device = net_dev;
}

void dpe_wlan_config_del_cfg_vap(uint8_t vap_idx)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }

    dpe_vap->pst_net_device = NULL;
    dpe_wlan_del_multi_user(dpe_vap->us_multi_user_idx);
}

/*
 * 函 数 名  : hmac_vap_init_set_rx_reorder_timeout
 * 功能描述  : 初始化重排序超时时间
 * 1.日    期  : 2012年10月24日
 *   修改内容  : 新生成函数
 */
void dpe_vap_init_set_rx_reorder_timeout(dpe_wlan_vap_stru *dpe_vap)
{
    dpe_vap->us_rx_timeout[WLAN_WME_AC_BK] = HMAC_BA_RX_BK_TIMEOUT;
    dpe_vap->us_rx_timeout[WLAN_WME_AC_BE] = HMAC_BA_RX_BE_TIMEOUT;
    dpe_vap->us_rx_timeout[WLAN_WME_AC_VI] = HMAC_BA_RX_VI_TIMEOUT;
    dpe_vap->us_rx_timeout[WLAN_WME_AC_VO] = HMAC_BA_RX_VO_TIMEOUT;

    dpe_vap->us_rx_timeout_min[WLAN_WME_AC_BE] = HMAC_BA_RX_BE_TIMEOUT_MIN;
    dpe_vap->us_rx_timeout_min[WLAN_WME_AC_BK] = HMAC_BA_RX_BK_TIMEOUT_MIN;
    dpe_vap->us_rx_timeout_min[WLAN_WME_AC_VI] = HMAC_BA_RX_VI_TIMEOUT_MIN;
    dpe_vap->us_rx_timeout_min[WLAN_WME_AC_VO] = HMAC_BA_RX_VO_TIMEOUT_MIN;
}

/*
 * 函 数 名  : dpe_wlan_cfg_vap_init
 * 功能描述  : DPE侧配置VAP初始化
 *   日    期  : 2024年12月26日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void dpe_wlan_cfg_vap_init(uint8_t dev_idx, uint16_t vap_idx, uint8_t chip_id)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }

    dpe_wlan_vap_init(dpe_vap);
    /* 初始化重排序超时时间 */
    dpe_vap_init_set_rx_reorder_timeout(dpe_vap);
    dpe_vap->uc_vap_id = vap_idx;
    dpe_vap->uc_chip_id = chip_id;
    dpe_vap->hal_dev_id = 0;    /* 配置vap挂在主路 */
    dpe_vap->en_vap_mode = WLAN_VAP_MODE_CONFIG;
    dpe_vap->en_vap_state = MAC_VAP_STATE_INIT;
    dpe_vap->uc_device_id = dev_idx;
}

void dpe_wlan_vap_state_sync(uint16_t vap_idx, uint8_t *param)
{
    dpe_wlan_vap_stru *dpe_vap;
    mac_vap_state_enum_uint8 vap_state;

    if (param == NULL) {
        return;
    }
    vap_state = (mac_vap_state_enum_uint8)(*param);

    dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }

    dpe_vap->en_vap_state = vap_state;
}

void dpe_wlan_cfg_vap_net_dev_set(uint8_t cfg_vap_idx, oal_net_device_stru *net_dev)
{
    dpe_wlan_vap_stru *dpe_cfg_vap;

    if (net_dev == NULL) {
        oam_error_log0(0, 0, "{dpe_wlan_cfg_vap_net_dev_set::net_device is NULL!}");
        return;
    }

    dpe_cfg_vap = dpe_wlan_vap_get(cfg_vap_idx);
    if (dpe_cfg_vap == NULL) {
        return;
    }
    dpe_cfg_vap->pst_net_device = net_dev;
}

void dpe_vap_set_assoc_id(uint8_t vap_id, uint16_t assoc_vap_id)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_id);
    if (dpe_vap == NULL) {
        oam_error_log0(0, 0, "{dpe_vap_set_assoc_id::dpe vap is NULL}");
        return;
    }
    dpe_vap->us_assoc_vap_id = assoc_vap_id;
}

void dpe_wlan_vap_unicast_pkt_reset(uint8_t vap_idx)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }
    oal_atomic_set(&(dpe_vap->rx_unicast_pkt_to_lan), 0);
}

uint32_t dpe_wlan_vap_unicast_pkt_get(uint8_t vap_idx)
{
    uint32_t pkt_num;
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return 0;
    }

    pkt_num = (uint32_t)oal_atomic_read(&(dpe_vap->rx_unicast_pkt_to_lan));
    return pkt_num;
}

#ifdef _PRE_WLAN_FEATURE_ML
void dpe_wlan_mlsr_change_netdev_vap_relation(uint8_t cur_vap_idx, uint8_t next_vap_idx,
    oal_net_device_stru *cur_net_dev, oal_net_device_stru *next_net_dev)
{
    dpe_wlan_vap_stru *cur_dpe_vap;
    dpe_wlan_vap_stru *next_dpe_vap;

    if (cur_net_dev == NULL || next_net_dev == NULL) {
        oam_error_log0(0, 0, "{dpe_wlan_mlsr_change_netdev_vap_relation::net_device is NULL!}");
        return;
    }

    cur_dpe_vap = dpe_wlan_vap_get(cur_vap_idx);
    next_dpe_vap = dpe_wlan_vap_get(next_vap_idx);
    if (cur_dpe_vap == NULL || next_dpe_vap == NULL) {
        return;
    }

    cur_dpe_vap->pst_net_device = cur_net_dev;
    next_dpe_vap->pst_net_device = next_net_dev;
}

void dpe_wlan_vap_mld_assoc_mode_set(uint8_t vap_idx, uint8_t mld_assoc_mode)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }
    dpe_vap->mld_assoc_mode = mld_assoc_mode;
}

void dpe_wlan_vap_mld_mode_set(uint8_t vap_idx, uint8_t mld_mode)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }
    dpe_vap->mld_mode = mld_mode;
}

void dpe_wlan_vap_mld_addr_set(uint8_t vap_idx, uint8_t *mld_addr)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }
    memcpy_s(&dpe_vap->mld_addr, WLAN_MAC_ADDR_LEN, mld_addr, WLAN_MAC_ADDR_LEN);
}
#endif  /* _PRE_WLAN_FEATURE_ML */

#ifdef _PRE_WLAN_FEATURE_EMLSR
void dpe_wlan_vap_emlsr_state_sync(uint8_t vap_idx, uint8_t emlsr_state)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }
    dpe_vap->emlsr_state = emlsr_state;
}
#endif

#ifdef _PRE_WLAN_CHBA_MGMT
OAL_STATIC void dpe_wlan_chba_dbac_vap_init_set_rx_reorder_timeout(dpe_wlan_vap_stru *dpe_vap)
{
    uint8_t ac_type;

    for (ac_type = WLAN_WME_AC_BE; ac_type < WLAN_WME_AC_BUTT; ac_type++) {
        dpe_vap->us_rx_timeout[ac_type] = DPE_WLAN_CHBA_DBAC_BA_RX_DEFAULT_TIMEOUT;
        dpe_vap->us_rx_timeout_min[ac_type] = DPE_WLAN_CHBA_DBAC_BA_RX_DEFAULT_TIMEOUT;
    }
    oam_warning_log1(0, OAM_SF_ANY, "{dpe_wlan_chba_dbac_vap_init_set_rx_reorder_timeout:set chba rx_timeout[%d]}",
        DPE_WLAN_CHBA_DBAC_BA_RX_DEFAULT_TIMEOUT);
}

void dpe_wlan_dbac_vap_set_rx_reorder_timeout(uint8_t vap_idx, uint8_t is_dbac_running)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        oam_error_log0(0, OAM_SF_CHBA, "{dpe_wlan_dbac_vap_set_rx_reorder_timeout::vap is NULL!}");
        return;
    }

    /* 退出dbac模式, 恢复CHBA vap rx重排序定时器超时配置 */
    if (is_dbac_running == OAL_FALSE) {
        dpe_vap_init_set_rx_reorder_timeout(dpe_vap);
        oam_warning_log0(dpe_vap->uc_vap_id, OAM_SF_ANY,
            "{dpe_wlan_dbac_vap_set_rx_reorder_timeout:reset chba vap rx_reorder_timeout}");
        return;
    }
    /* chba进入dbac模式, 初始化chba独有的强制移窗超时定时器定时时间门限配置 */
    dpe_wlan_chba_dbac_vap_init_set_rx_reorder_timeout(dpe_vap);
}
#endif  /* _PRE_WLAN_CHBA_MGMT */
