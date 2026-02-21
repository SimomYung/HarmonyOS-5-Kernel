/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host侧Shenkuo芯片相关TX功能
 * 作    者 : wifi
 * 创建日期 : 2021年3月1日
 */

#ifndef HOST_TX_MP12_H
#define HOST_TX_MP12_H

#include "oal_ext_if.h"
#include "host_hal_ring.h"
#include "host_dscr_mp12.h"
#include "mac_common.h"

#define MP12_RING_TABLE_MSDU_INFO_RING_LSB 0
#define MP12_RING_TABLE_MSDU_INFO_RING_MSB 1
#define MP12_RING_TABLE_MSDU_ADDR_RING_LSB 2
#define MP12_RING_TABLE_MSDU_ADDR_RING_MSB 3
#define MP12_RING_TABLE_WPTR_OFFSET 4
#define MP12_RING_TABLE_RING_DEPTH_OFFSET 4
#define MP12_RING_TABLE_MAX_AGGR_AMSDU_OFFSET 4
#define MP12_RING_TABLE_ML_MODE_OFFSET 4
#define MP12_RING_TABLE_ML0_RPTR_OFFSET 5

uint8_t mp12_host_get_device_tx_ring_num(void);
uint32_t mp12_host_alloc_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint32_t size);
uint32_t mp12_host_release_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring);
uint8_t mp12_host_tx_ring_alloced(hal_host_tx_msdu_ring_stru *msdu_ring);
uint32_t mp12_host_tx_set_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index);
uint32_t mp12_host_tx_get_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index);
uint32_t mp12_host_tx_reset_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index);
uint32_t mp12_host_tx_get_msdu_info_devva(hal_host_tx_msdu_ring_stru *msdu_ring,
    hal_tx_ring_table_addr_stru *tx_ring_addr);
void mp12_host_tx_set_ring_table_base_addr(uint32_t addr);
uint32_t mp12_host_tx_init_tid_ring_table(uint8_t lut_idx, uint8_t tid,
    hal_tx_ring_table_addr_val_stru *ring_tb_addr);
uint32_t mp12_host_tx_ring_update_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type);
uint32_t mp12_host_tx_ring_get_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type);
uint16_t mp12_host_get_tx_ring_lut_idx(uint16_t lut_idx, uint16_t mld_user_lut_idx);
uint32_t mp12_host_tx_get_hw_ml0_rptr(
    mp12_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val);
mp12_tx_msdu_ring_table_stru *mp12_host_tx_get_tid_ring_table_mgmt(uint8_t lut_idx, uint8_t tid);

#endif
