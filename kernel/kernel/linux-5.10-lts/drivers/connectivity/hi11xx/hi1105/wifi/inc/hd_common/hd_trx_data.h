/*
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 * 功能说明 : trx data公共头文件
 */

#ifndef HD_TRX_DATA_H
#define HD_TRX_DATA_H

typedef struct {
    uint8_t host_ring_tx_switch;
    uint8_t device_ring_tx_switch;
    uint8_t rx_switch;
    uint8_t device_loop_sched;
    uint8_t tx_ring_switch;
    uint8_t host_update_wptr;       /* skb入ring时wptr更新方式: PCIE直接写或抛事件 */
    uint8_t ring_switch_independent; /* Tx ring与Rx Ring同源切换 */
    uint8_t soft_irq_sched;         /* 软中断调度使能 */
    uint8_t netbuf_alloc_opt;
    uint8_t tx_netbuf_limit;
} wlan_trx_switch_stru;

#endif