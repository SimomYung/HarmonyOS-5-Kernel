/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_device.c 的头文件
 * 作    者 :
 * 创建日期 : 2024年11月21日
 */

#ifndef DPE_WLAN_DEVICE_H
#define DPE_WLAN_DEVICE_H

#include "wlan_protocol_types.h"
#include "dpe_wlan_stat_stru.h"

typedef struct {
    dpe_wlan_device_stat_stru *device_stats;
    uint8_t uc_device_id;
    uint8_t en_al_trx_flag;
    uint32_t pre_host_mac_irq_mask;             /* 保存pcie切换前的host中断屏蔽bitmap */
} dpe_wlan_device_stru;

dpe_wlan_device_stru *dpe_wlan_device_get(uint16_t us_idx);
void dpe_wlan_device_init(uint16_t us_idx, uint16_t cfg_vap_id, uint8_t chip_id);
void dpe_wlan_device_al_trx_flag_sync(uint8_t in_al_trx);
void dpe_wlan_device_init_dyn_pcie(uint8_t dev_idx);
#endif