/*
 * Copyright (c) @CompanyNameMagicTag 2021-2025. All rights reserved.
 * 功能说明   : DMA 基本操作
 * 作者       : wifi
 * 创建日期   : 2025年1月4日
 */

#ifndef HOST_HAL_DMA_H
#define HOST_HAL_DMA_H

#include "oal_types.h"
#include "oal_net.h"
#include "dpe_wlan_tid.h"
#include "host_hal_dma_batch.h"

#define DMA_MAP_ONCE_PROC_NUM_DEFAULT 8
#define DMA_RX_MAP_ERROR_ADDR 0x4000000

uint16_t hal_get_host_dma_once_proc_num(void);
void hal_host_tx_dma_map_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after);
void hal_host_tx_dma_unmap_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after);
void hal_host_rx_dma_map_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after);
void hal_host_rx_dma_unmap_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after);
uint32_t hal_host_tx_get_devva(oal_netbuf_stru *netbuf, uint64_t *devva);
uint32_t hal_host_tx_dma_map_phyaddr(uint8_t *host_addr, uint32_t len, dma_addr_t *physaddr);
uint32_t hal_host_tx_dma_unmap_addr(uint8_t *dma_addr, uint32_t len);
uint32_t hal_host_ring_tx_single_map(oal_netbuf_stru *netbuf);
uint8_t *hal_host_tx_netbuf_init_msdu_dscr(oal_netbuf_stru *netbuf);
void hal_host_clear_netbuf_list(oal_netbuf_head_stru *netbub_list);
void hal_host_tx_unmap_single_netbuf(oal_netbuf_stru *netbuf);
void hal_host_tx_unmap_netbuf_list(oal_netbuf_stru *netbuf);
void hal_host_tx_dma_unmap_step_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after);
#endif /* HOST_HAL_DMA_H */