/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : _report
 * 创建日期 : 2020年7月31日
 */

#ifndef HMAC_OTA_REPORT_H
#define HMAC_OTA_REPORT_H

#include "mac_frame.h"
#include "mac_vap.h"
#include "hmac_vap.h"
#include "hmac_hid2d.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_OTA_REPORT_H

/* en_ota_type-> oam_ota_type_enum */
uint32_t mac_rx_report_80211_frame(uint8_t vap_idx, uint8_t *rx_cb, oal_netbuf_stru *netbuf, uint8_t ota_type);
/* en_ota_type->oam_ota_type_enum */
uint32_t mac_report_80211_frame(uint8_t *puc_mac_vap, uint8_t *puc_rx_cb, oal_netbuf_stru *pst_netbuf,
    uint8_t *puc_des_addr, uint8_t en_ota_type);
uint32_t hmac_tx_report_eth_frame(mac_vap_stru *pst_mac_vap, oal_netbuf_stru *pst_netbuf);
#ifdef _PRE_WLAN_DFT_DUMP_FRAME
void hmac_rx_report_eth_frame(uint8_t vap_idx, oal_netbuf_stru *netbuf);
#endif
void hmac_rx_lan_frame_no_user_log(uint8_t vap_idx, oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctrl);
#endif
