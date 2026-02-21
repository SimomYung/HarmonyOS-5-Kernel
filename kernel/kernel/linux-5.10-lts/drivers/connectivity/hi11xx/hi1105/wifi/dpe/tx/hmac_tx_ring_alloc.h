/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Hmac tx ring内存分配模块
 * 作    者 : wifi
 * 创建日期 : 2021年12月23日
 */

#ifndef HMAC_TX_RING_ALLOC_H
#define HMAC_TX_RING_ALLOC_H

#include "dpe_wlan_tid.h"

uint32_t hmac_ring_tx_init(uint16_t user_idx, uint8_t tid);
void hmac_ring_tx_deinit(uint16_t user_idx, uint8_t tid);
void hmac_tid_info_deinit(hmac_tid_info_stru *tid_info);
uint32_t hmac_alloc_tx_ring_by_tid(hmac_msdu_info_ring_stru *tx_ring, uint8_t tid_no);
uint32_t hmac_alloc_tx_ring_after_inited(hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_ring);
void hmac_user_tx_ring_deinit(uint16_t user_idx);
uint32_t hmac_set_tx_ring_device_base_addr(frw_event_mem_stru *frw_mem);
void hmac_tx_host_ring_release_nolock(hmac_msdu_info_ring_stru *tx_ring);
void hmac_tx_host_ring_release(hmac_msdu_info_ring_stru *tx_ring);
void hmac_wait_tx_ring_empty(hmac_tid_info_stru *tid_info);
void hmac_tx_ring_release_all_netbuf(hmac_msdu_info_ring_stru *tx_ring);

static inline uint8_t hmac_tx_ring_alloced(hmac_msdu_info_ring_stru *tx_ring)
{
    return hal_host_tx_ring_alloced(&tx_ring->ring_buf) && tx_ring->netbuf_list != NULL;
}
void hmac_tx_ring_set_mld_bitmap_next(uint16_t user_idx, uint8_t bitmap);
void hmac_tid_info_queue_release(oal_netbuf_head_stru *tid_queue);
#endif
