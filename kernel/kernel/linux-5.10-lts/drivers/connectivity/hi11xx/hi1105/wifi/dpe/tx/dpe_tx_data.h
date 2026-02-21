/*
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 * 功能说明 : Host发送完成模块
 */

#ifndef DPE_TX_DATA_H
#define DPE_TX_DATA_H

#include "dpe_wlan_user.h"
#include "dpe_wlan_vap.h"
#include "hmac_tid_sche.h"
#include "hd_trx_data.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern wlan_trx_switch_stru g_dpe_trx_switch;
void dpe_trx_switch_sync(wlan_trx_switch_stru *trx_switch);
static inline uint8_t dpe_host_ring_tx_enabled(void)
{
    return g_dpe_trx_switch.host_ring_tx_switch;
}

static inline uint8_t dpe_device_ring_tx_enabled(void)
{
    return g_dpe_trx_switch.device_ring_tx_switch;
}

static inline uint8_t dpe_device_loop_sched_enabled(void)
{
    return g_dpe_trx_switch.device_loop_sched;
}

static inline uint8_t dpe_tx_ring_switch_enabled(void)
{
    return g_dpe_trx_switch.tx_ring_switch;
}
static inline uint8_t dpe_rx_ring_switch_enabled(void)
{
    return g_dpe_trx_switch.rx_switch;
}
static inline uint8_t dpe_tx_ring_soft_irq_sched_enabled(void)
{
    return g_dpe_trx_switch.soft_irq_sched;
}

static inline uint8_t dpe_host_update_wptr_enabled(void)
{
    return g_dpe_trx_switch.host_update_wptr;
}

static inline uint8_t dpe_tx_ring_switch_sync_mode(void)
{
    return g_dpe_trx_switch.ring_switch_independent;
}

static inline oal_bool_enum_uint8 dpe_ring_tx_enabled(void)
{
    return (dpe_host_ring_tx_enabled() || dpe_device_ring_tx_enabled()) ? OAL_TRUE : OAL_FALSE;
}

static inline oal_bool_enum_uint8 dpe_device_ring_tx_only(void)
{
    return (!dpe_host_ring_tx_enabled() && dpe_device_ring_tx_enabled()) ? OAL_TRUE : OAL_FALSE;
}

static inline void hmac_ring_tx_update_tid(mac_tx_ctl_stru *tx_ctl)
{
    if (mac_get_cb_wme_tid_type(tx_ctl) == WLAN_TIDNO_BCAST) {
        mac_get_cb_wme_tid_type(tx_ctl) = WLAN_TIDNO_VOICE;
        mac_get_cb_wme_ac_type(tx_ctl) = WLAN_WME_TID_TO_AC(WLAN_TIDNO_VOICE);
    }
}

uint32_t hmac_host_tx_tid_enqueue(hmac_tid_info_stru *tid_info, oal_netbuf_stru *netbuf);
uint32_t hmac_host_ring_tx(dpe_wlan_vap_stru *dpe_vap, uint16_t user_idx, oal_netbuf_stru *netbuf);
void hmac_host_ring_tx_resume(dpe_wlan_vap_stru *dpe_vap, dpe_wlan_user_stru *dpe_user, hmac_tid_info_stru *tid_info);
uint32_t hmac_host_tx_data(uint8_t vap_idx, uint16_t user_idx, oal_netbuf_stru *netbuf);
uint16_t dpe_tx_get_user_id(uint8_t vap_id, mac_ether_header_stru *ether_hdr);
void hmac_tx_cb_field_init(mac_tx_ctl_stru *tx_ctl, uint8_t uc_vap_id, uint8_t en_vap_mode,
    uint16_t user_idx, mac_ether_header_stru *ether_hdr);
oal_bool_enum_uint8 dpe_is_ring_tx(uint8_t vapid, oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl);
void hmac_mcast_ampdu_netbuf_process(dpe_wlan_vap_stru *mac_vap, oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl,
    mac_ether_header_stru *eth_hdr, oal_bool_enum_uint8 *multicast_need_host_tx);
void hmac_ether_set_data_type(mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf);
void hmac_free_netbuf_list(oal_netbuf_stru *netbuf);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
