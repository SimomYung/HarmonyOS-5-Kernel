/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * 功能说明 : wfsc扫描模块 hmac功能
 * 创建日期 : 2023年12月19日
 */
/* 1 头文件包含 */
#ifdef _PRE_WLAN_FEATURE_ELP
#include "hmac_device.h"
#include "wlan_chip_i.h"
#include "hmac_sme_sta.h"
#include "hmac_scan.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SCAN_ELP_C

/* 2 全局变量定义 */
hmac_scan_stru g_wfsc_hmac_scan_mgmt;
uint32_t g_elp_scan_disable_bitmap = HMAC_ELP_SCAN_FLAG_MASK;

/*
 * 函 数 名  : hmac_get_wfsc_scan_mgmt
 * 功能描述  : 获取WFSC扫描管理结构体
 */
hmac_scan_stru *hmac_get_wfsc_scan_mgmt(void)
{
    return &g_wfsc_hmac_scan_mgmt;
}

uint32_t hmac_elp_scan_enable(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    if (*param == OAL_TRUE) {
        g_elp_scan_disable_bitmap &= (~HMAC_ELP_SCAN_IOCFG);
    } else {
        g_elp_scan_disable_bitmap |= HMAC_ELP_SCAN_IOCFG;
    }
    oam_warning_log1(0, OAM_SF_SCAN, "hmac_elp_scan_enable: g_elp_scan_enable_flag=%x.", g_elp_scan_disable_bitmap);
    return OAL_SUCC;
}

#define ELP_CAP_BG_SCAN_MASK     BIT2
#define ELP_CAP_WFSC_MASK        BIT3
#define PHY_CAP_LP_MASK          BIT7
void hmac_set_elp_scan_switch(uint32_t phy_cap, uint32_t lp_cap)
{
    if (((lp_cap & ELP_CAP_WFSC_MASK) != 0) &&
        ((lp_cap & ELP_CAP_BG_SCAN_MASK) != 0) &&
        ((phy_cap & PHY_CAP_LP_MASK) != 0)) {
        g_elp_scan_disable_bitmap &= (~HMAC_ELP_SCAN_CUS);
    } else {
        g_elp_scan_disable_bitmap |= HMAC_ELP_SCAN_CUS;
    }
}

/*
 * 函 数 名  : hmac_elp_scan_proc_scan_req_event
 * 功能描述  : 准备向dmac下发扫描消息
 */
uint32_t hmac_elp_scan_proc_scan_req_event(hmac_vap_stru *hmac_vap, hmac_scan_stru *scan_mgmt,
    mac_scan_req_h2d_stru *scan_h2d_params)
{
    frw_event_mem_stru *event_mem = NULL;
    frw_event_stru *event = NULL;
    oal_netbuf_stru *netbuf_scan_req = NULL;
    mac_scan_req_stru *scan_params = NULL;
    uint32_t ret = 0;

    scan_params = &(scan_h2d_params->st_scan_params);

    /* 抛扫描请求事件到DMAC, 申请event事件内存 */
    event_mem = frw_event_alloc_m(sizeof(dmac_tx_event_stru));
    if (event_mem == NULL) {
        /* 恢复扫描状态为非运行状态 */
        scan_mgmt->en_is_scanning = OAL_FALSE;
        return OAL_ERR_CODE_PTR_NULL;
    }

    ret = wlan_chip_scan_req_alloc_and_fill_netbuf(event_mem, hmac_vap, &netbuf_scan_req, scan_h2d_params);
    if (ret != OAL_SUCC) {
        /* 恢复扫描状态为非运行状态 */
        scan_mgmt->en_is_scanning = OAL_FALSE;
        frw_event_free_m(event_mem);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 改写事件头为LP扫描 */
    event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_ELP_SCAN_REQ,
        sizeof(dmac_tx_event_stru), FRW_EVENT_PIPELINE_STAGE_1, hmac_vap->st_vap_base_info.uc_chip_id,
        hmac_vap->st_vap_base_info.uc_device_id, hmac_vap->st_vap_base_info.uc_vap_id);

    /* 起hmac层保护性超时定时器 */
    hmac_scan_req_event_scan_timeout_mechanism_set(scan_params, hmac_vap, scan_mgmt);

    /* 分发事件 */
    frw_event_dispatch_event(event_mem);

    oal_netbuf_free(netbuf_scan_req);
    frw_event_free_m(event_mem);
    return OAL_SUCC;
}

uint32_t hmac_scan_param_proc(mac_vap_stru *mac_vap, mac_scan_req_h2d_stru *scan_h2d_params,
    mac_cfg80211_scan_param_stru *cfg80211_scan_param)
{
    mac_scan_req_stru *scan_params = NULL;
    /* 初始扫描列表清0 */
    memset_s(scan_h2d_params, sizeof(mac_scan_req_h2d_stru), 0, sizeof(mac_scan_req_h2d_stru));

    /* 将内核下发的扫描参数更新到驱动扫描请求结构体中 */
    scan_params = &(scan_h2d_params->st_scan_params);
    hmac_cfg80211_pre_start_scan(scan_params, mac_vap, cfg80211_scan_param);

    scan_h2d_params->scan_flag = cfg80211_scan_param->scan_flag;
    hmac_cfg80211_free_scan_param_resource(cfg80211_scan_param);
    return OAL_SUCC;
}

/* 扫描处理 */
uint32_t hmac_elp_scan_proc(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    mac_scan_req_h2d_stru scan_h2d_params = { 0 };
    mac_cfg80211_scan_param_stru *cfg80211_scan_param = hmac_cfg80211_get_scan_param(mac_vap, param);
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    hmac_scan_stru *scan_mgmt = hmac_get_wfsc_scan_mgmt();

    if (cfg80211_scan_param == NULL || hmac_vap == NULL) {
        return OAL_FAIL;
    }
    hmac_scan_param_proc(mac_vap, &scan_h2d_params, cfg80211_scan_param);
    hmac_scan_update_scan_params(hmac_vap, &(scan_h2d_params.st_scan_params), OAL_FALSE);
    hmac_scan_update_scan_record(hmac_vap, &(scan_h2d_params.st_scan_params), scan_mgmt);
    hmac_elp_scan_proc_scan_req_event(hmac_vap, scan_mgmt, &scan_h2d_params);
    return OAL_SUCC;
}
#endif /* _PRE_WLAN_FEATURE_ELP */

