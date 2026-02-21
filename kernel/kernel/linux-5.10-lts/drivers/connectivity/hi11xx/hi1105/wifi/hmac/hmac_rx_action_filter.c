/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_rx_action_filter.c 文件
 * 作    者 : w30072618
 * 创建日期 : 2025年2月14日
 */
#include "oam_wdk.h"
#include "mac_vap.h"
#include "frw_ext_if.h"
#include "hmac_config.h"
#include "hmac_vap.h"
#include "wlan_frame_action_defs.h"
#include "hmac_rx_action_filter.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_ACTION_FILTER_C

mac_rx_action_blacklist_stru g_hmac_rx_action_blacklist[] = {};

oal_netbuf_stru *hmac_fill_rx_action_netbuf(mac_rx_action_blacklist_stru *action_blacklist, size_t blacklist_items)
{
    oal_netbuf_stru *netbuf = NULL;
    uint8_t *data = NULL;
    size_t netbuf_len = blacklist_items * sizeof(mac_rx_action_blacklist_stru);
    errno_t ret;
    netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, netbuf_len, OAL_NETBUF_PRIORITY_MID);
    if (netbuf == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_fill_rx_action_netbuf:netbuf alloc null}");
        return NULL;
    }
    data = oal_netbuf_data(netbuf);
    if (data == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_fill_rx_action_netbuf:data null}");
        oal_netbuf_free(netbuf);
        return NULL;
    }
    memset_s(data, netbuf_len, 0, netbuf_len);
    ret = memcpy_s(data, netbuf_len, action_blacklist, netbuf_len);
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_fill_rx_action_netbuf:memcpy_s failed.}");
        oal_netbuf_free(netbuf);
        return NULL;
    }
    oal_netbuf_put(netbuf, netbuf_len);
    oam_warning_log0(0, OAM_SF_ANY, "{hmac_fill_rx_action_netbuf:fill_rx_action_netbuf succ.}");
    return netbuf;
}
/*
 * 函 数 名  : hmac_config_update_action_filter
 * 功能描述  : rx action过滤功能的相关参数配置接口
 * 1.日    期  : 2017年4月17日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_update_action_filter(mac_vap_stru *mac_vap)
{
    uint32_t ret;
    dmac_tx_event_stru *dtx_event = NULL;
    frw_event_mem_stru *event_mem = NULL;
    oal_netbuf_stru *netbuf = NULL;
    frw_event_stru *event = NULL;
    mac_chip_stru *mac_chip = NULL;
    uint32_t vap_num;
    size_t blacklist_items;

    mac_chip = hmac_res_get_mac_chip(mac_vap->uc_chip_id);
    if (mac_chip == NULL) {
        return;
    }
    vap_num = hmac_get_chip_vap_num(mac_chip);
    if (vap_num > 1) {
        return;
    }
    blacklist_items = sizeof(g_hmac_rx_action_blacklist) / sizeof(mac_rx_action_blacklist_stru);
    if (blacklist_items == 0) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_config_update_action_filter::action_blacklist is empty.}");
        return;
    }
    netbuf = hmac_fill_rx_action_netbuf(g_hmac_rx_action_blacklist, blacklist_items);
    if (netbuf == NULL) {
        return;
    }
    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    event_mem = frw_event_alloc_m(sizeof(dmac_tx_event_stru));
    if (event_mem == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_update_action_filter::event_mem null.}");
        oal_netbuf_free(netbuf);
        return;
    }

    event = (frw_event_stru *)event_mem->puc_data;
    frw_event_hdr_init(&(event->st_event_hdr),
        FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_ACTION_FILTER,
        sizeof(dmac_tx_event_stru), FRW_EVENT_PIPELINE_STAGE_1,
        mac_vap->uc_chip_id, mac_vap->uc_device_id, mac_vap->uc_vap_id);

    dtx_event = (dmac_tx_event_stru *)(event->auc_event_data);
    dtx_event->pst_netbuf = netbuf;
    dtx_event->us_frame_len = oal_netbuf_len(netbuf);

    ret = frw_event_dispatch_event(event_mem);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_config_update_ip_filter::frw_event_dispatch_event failed[%d].}", ret);
    }
    oal_netbuf_free(netbuf);
    frw_event_free_m(event_mem);
    return;
}