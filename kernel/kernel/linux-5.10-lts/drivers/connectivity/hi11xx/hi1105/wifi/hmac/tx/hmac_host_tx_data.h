/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Host发送完成模块
 * 作    者 : wifi
 * 创建日期 : 2020年4月23日
 */

#ifndef HMAC_HOST_TX_DATA_H
#define HMAC_HOST_TX_DATA_H

#include "hmac_vap.h"
#include "hmac_tid_sche.h"
#include "dpe_tx_data.h"

uint32_t hmac_ring_tx(hmac_vap_stru *hmac_vap, oal_netbuf_stru **netbuf, mac_tx_ctl_stru *tx_ctl);
uint32_t hmac_tx_mode_switch(mac_vap_stru *mac_vap, uint32_t *params);
#endif
