/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : DPE统计结构体文件
 * 作    者 :
 * 创建日期 : 2024年11月22日
 */

#ifndef DPE_WLAN_STAT_STRU_H
#define DPE_WLAN_STAT_STRU_H

/* 1 头文件包含 */
#include "wlan_protocol_types.h"

/* DPE VAP级别统计 */
typedef struct {
    uint32_t rx_packets;
    uint32_t rx_dropped_misc;
    uint32_t tx_packets;
    uint32_t tx_failed;
    uint32_t tx_retries;
    uint64_t rx_bytes;
    uint64_t tx_bytes;
} dpe_wlan_vap_stat_stru;

/* HMAC device级别统计 */
typedef struct {
    oal_atomic rx_packets;
    oal_atomic rx_bytes;
    oal_atomic tx_packets;
    oal_atomic tx_bytes;
    oal_atomic host_ring_rx_packets;

    uint32_t total_rx_packets;
    uint64_t total_rx_bytes;

    uint32_t total_tx_packets;
    uint64_t total_tx_bytes;

    oal_atomic rx_tcp_checksum_fail;
    oal_atomic rx_tcp_checksum_pass;
    oal_atomic rx_ip_checksum_fail;
    oal_atomic rx_ip_checksum_pass;

    uint64_t rx_tcp_checksum_total_fail;
    uint64_t rx_tcp_checksum_total_pass;
    uint64_t rx_ip_checksum_total_fail;
    uint64_t rx_ip_checksum_total_pass;
} dpe_wlan_device_stat_stru;

/* DPE user级别统计 */
typedef struct {
    uint32_t rx_packets;
    uint32_t rx_dropped_misc;
    uint32_t tx_packets;
    uint32_t tx_failed;
    uint32_t tx_retries;
    uint64_t rx_bytes;
    uint64_t tx_bytes;
} dpe_wlan_user_stat_stru;

#endif