/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL MAIN
 * 作    者 : wifi
 * 创建日期 : 2020年10月19日
 */

#ifndef HOST_HAL_IRQ_H
#define HOST_HAL_IRQ_H

#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hal_common.h"
#include "host_hal_device.h"

extern uint32_t g_int_state_reg_val;
extern uint32_t g_int_mask_reg_val;

void hal_clear_host_mac_irq_mask(void);
void hal_recover_host_mac_irq_mask(void);
void hal_irq_host_rx_ring_empty_isr(hal_host_device_stru *hal_device, uint32_t mask);
void hal_irq_host_rx_complete_isr(hal_host_device_stru *hal_device, uint32_t mask);
void hal_irq_host_tx_complete_isr(hal_host_device_stru *hal_dev, uint32_t mask);
void hal_irq_host_ftm_csi_isr(hal_host_device_stru *hal_dev, uint32_t mask);
#endif
