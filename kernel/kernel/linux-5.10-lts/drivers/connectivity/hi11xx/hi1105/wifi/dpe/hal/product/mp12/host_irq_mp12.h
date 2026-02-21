/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host中断功能实现
 * 作    者 : wifi
 * 创建日期 : 2012年9月18日
 */

#ifndef HOST_IRQ_MP12_H
#define HOST_IRQ_MP12_H
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hal_common.h"

typedef struct {
    uint32_t value1; /* 中断状态1 */
    uint32_t value2; /* 中断状态2 */
} hal_intr_state_stru;

void mp12_host_chip_irq_init(void);
void mp12_irq_host_mac_isr(hal_host_device_stru *hal_dev);
void mp12_host_mac_clear_rx_irq(hal_host_device_stru *hal_device);
void mp12_host_mac_mask_rx_irq(hal_host_device_stru *hal_device);

#endif
