/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host侧Shenkuo芯片相关TX功能
 * 作    者 : wifi
 * 创建日期 : 2021年3月1日
 */

#ifndef HOST_TX_MP16C_H
#define HOST_TX_MP16C_H

#include "oal_ext_if.h"
#include "host_hal_ring.h"
#include "mac_common.h"

uint8_t mp16c_host_get_device_tx_ring_num(void);
uint32_t mp16c_host_alloc_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint32_t size);
uint32_t mp16c_host_release_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring);
uint8_t mp16c_host_tx_ring_alloced(hal_host_tx_msdu_ring_stru *msdu_ring);
uint32_t mp16c_host_tx_set_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index);
uint32_t mp16c_host_tx_get_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index);
uint32_t mp16c_host_tx_reset_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index);
uint32_t mp16c_host_tx_get_msdu_info_devva(hal_host_tx_msdu_ring_stru *msdu_ring,
    uint64_t *info_ring_devva, uint64_t *addr_ring_devva);
void mp16c_host_tx_set_ring_table_base_addr(uint32_t addr);
uint32_t mp16c_host_tx_init_tid_ring_table(uint8_t lut_idx, uint8_t tid, uint64_t *tx_ring_table_addr);
uint32_t mp16c_host_tx_ring_update_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type);
uint32_t mp16c_host_tx_ring_get_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type);
uint16_t mp16c_host_get_tx_ring_lut_idx(uint16_t lut_idx, uint16_t mld_user_lut_idx);

#endif

