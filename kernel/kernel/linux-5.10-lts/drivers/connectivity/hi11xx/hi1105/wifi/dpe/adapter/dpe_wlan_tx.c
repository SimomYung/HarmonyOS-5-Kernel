/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_tx.c
 * 作    者 :
 * 创建日期 : 2024年11月22日
 */

/* 1 头文件包含 */
#include "dpe_tx_data.h"

#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD
static void dpe_wlan_xmit(oal_net_device_stru *net_device, oal_netbuf_stru *netbuf)
{
    dpe_wlan_vap_stru *vap = NULL;
    mac_tx_ctl_stru *tx_ctl = NULL;
    dpe_wlan_user_stru *user = NULL;
    mac_ether_header_stru *ether_hdr = NULL;
    uint16_t us_user_idx;

    oal_netbuf_next(netbuf) = NULL;
    oal_netbuf_prev(netbuf) = NULL;

    vap = (dpe_wlan_vap_stru *)oal_net_dev_priv(net_device);
    if (oal_unlikely(vap == NULL)) {
        oal_netbuf_free(netbuf);
        return;
    }
    /* VAP模式判断 */
    if (oal_unlikely(vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP && vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA)) {
        oal_netbuf_free(netbuf);
        return;
    }
    /* 如果关联用户数量为0，则丢弃报文 */
    if (oal_unlikely(vap->us_user_nums == 0)) {
        oal_netbuf_free(netbuf);
        return;
    }
    ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    us_user_idx = dpe_tx_get_user_id(vap->uc_vap_id, ether_hdr);
    user = (dpe_wlan_user_stru *)dpe_wlan_user_get(us_user_idx);
    if (oal_unlikely(user == NULL)) {
        oal_netbuf_free(netbuf);
        return;
    }
    hmac_stat_user_tx_netbuf(us_user_idx, netbuf);

    /* 申请并清空cb */
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    hmac_tx_cb_field_init(tx_ctl, vap->uc_vap_id, vap->en_vap_mode, us_user_idx, ether_hdr);

    if (dpe_is_ring_tx(vap->uc_vap_id, netbuf, tx_ctl)) {
        /* 非ring约束 */
        hmac_ring_tx_update_tid(tx_ctl);
        /* csum上层填写在skb cb里, 后续流程根据该字段填写 */
        hmac_ether_set_data_type(tx_ctl, netbuf);
        /* 入队 */
        hmac_host_tx_data(vap->uc_vap_id, us_user_idx, netbuf);
    } else {
        /* Device Tx帧统一流转到ACPU进行封装处理 */
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD
        /* hcc tx */
#endif
    }
}
/*
 * 功能描述  : 挂接到DPA modem转发表的发送源的入口函数, 注册给对应的ops
 */
int32_t dpe_wlan_xmit_from_modem(oal_net_device_stru *net_device, oal_netbuf_stru *netbuf)
{
    if (oal_unlikely(net_device == NULL || netbuf == NULL)) {
        return 0;
    }
    /* 统一异步获取EHT HDR信息 */
    /* payload read */
    dpe_wlan_xmit(net_device, netbuf);
    return 0;
}
/*
 * 功能描述  : 挂接到DPA TCP/IP协议栈的发送源的入口函数, 注册给对应的ops
 */
int32_t dpe_wlan_xmit_from_tcpip(oal_net_device_stru *net_device, oal_netbuf_stru *netbuf)
{
    if (oal_unlikely(net_device == NULL || netbuf == NULL)) {
        return 0;
    }
    /* EHT HDR信息可直接访问 */
    /* payload read */
    dpe_wlan_xmit(net_device, netbuf);
}

/* 处理所有来源于ACPU的数据帧, 包含ACPU协议栈、DPA协议栈/Modem的Ring Tx数据; 非Ring tx数据透传至WCPU */
int32_t dpe_wlan_xmit_from_acpu(oal_netbuf_stru *netbuf)
{
    dpe_wlan_vap_stru *vap = NULL;
    mac_tx_ctl_stru *tx_ctl = NULL;
    dpe_wlan_user_stru *user = NULL;
    if (oal_unlikely(netbuf == NULL)) {
        return 0;
    }

    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    vap = (dpe_wlan_vap_stru *)dpe_wlan_vap_get(mac_get_cb_tx_vap_index(tx_ctl));
    user = (dpe_wlan_user_stru *)dpe_wlan_user_get(mac_get_cb_tx_user_idx(tx_ctl));

    /* ACPU来帧直接入队或转发 */
    if (dpe_is_ring_tx(vap->uc_vap_id, netbuf, tx_ctl)) {
        /* 入队 */
        hmac_host_tx_data(vap->uc_vap_id, user->us_assoc_id, netbuf);
    } else {
        /* 已封装80211 HDR, FRW事件透传流转至DEVICE */
        /* 此处不存在非Ring帧 */
    }
    return 0;
}

int32_t dpe_wlan_xmit_from_wlan(oal_netbuf_stru *netbuf)
{
    dpe_wlan_vap_stru *vap = NULL;
    mac_tx_ctl_stru *tx_ctl = NULL;
    dpe_wlan_user_stru *user = NULL;
    if (oal_unlikely(netbuf == NULL)) {
        return 0;
    }

    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    vap = (dpe_wlan_vap_stru *)dpe_wlan_vap_get(mac_get_cb_tx_vap_index(tx_ctl));
    user = (dpe_wlan_user_stru *)dpe_wlan_user_get(mac_get_cb_tx_user_idx(tx_ctl));

    /* ACPU来帧直接入队或转发 */
    if (dpe_is_ring_tx(vap->uc_vap_id, netbuf, tx_ctl)) {
        /* 入队 */
        hmac_host_tx_data(vap, user->us_user_idx, netbuf);
    } else {
        /* 已封装80211 HDR, FRW事件透传流转至DEVICE */
        /* 此处不存在非Ring帧 */
    }
    return 0;
}
#endif
