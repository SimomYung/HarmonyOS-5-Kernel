/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : Hmac tx统计模块
 * 作    者 : wifi
 * 创建日期 : 2024年1月15日
 */

#ifndef HMAC_TX_STAT_H
#define HMAC_TX_STAT_H

#ifdef _PRE_WLAN_FEATURE_TX_STAT
uint32_t dpe_wlan_tx_stats_set_log_on(uint32_t *params);
void hmac_tx_stats_log(hmac_tid_info_stru *tid_info, uint32_t last_period_tx_msdu);
void hmac_stat_tx_ring(hmac_tid_info_stru *tid_info, uint32_t avail_size);
#endif

#endif
