/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Host发送完成模块
 * 作    者 : wifi
 * 创建日期 : 2020年4月23日
 */

#ifndef HMAC_RX_COMPLETE_H
#define HMAC_RX_COMPLETE_H

#include "host_hal_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    struct semaphore rx_comp_sema;
    oal_atomic hal_dev_rx_comp_triggered[WLAN_DEVICE_MAX_NUM_PER_CHIP];
    oal_task_stru *rx_comp_thread;
} hmac_rx_comp_stru;

extern hmac_rx_comp_stru g_rx_comp_mgmt;

static inline hmac_rx_comp_stru *hmac_get_rx_comp_mgmt(void)
{
    return &g_rx_comp_mgmt;
}

static inline void hmac_set_rx_comp_triggered(uint8_t hal_dev_id)
{
    oal_atomic_set(&g_rx_comp_mgmt.hal_dev_rx_comp_triggered[hal_dev_id], OAL_TRUE);
}

static inline void hmac_clear_rx_comp_triggered(uint8_t hal_dev_id)
{
    oal_atomic_set(&g_rx_comp_mgmt.hal_dev_rx_comp_triggered[hal_dev_id], OAL_FALSE);
}

static inline uint8_t hmac_get_rx_comp_triggered(uint8_t hal_dev_id)
{
    return oal_atomic_read(&g_rx_comp_mgmt.hal_dev_rx_comp_triggered[hal_dev_id]) == OAL_TRUE;
}

static inline void hmac_rx_comp_scheduled(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    up(&g_rx_comp_mgmt.rx_comp_sema);
#endif
}
static inline void hmac_rx_comp_trigger_scheduled(hal_host_device_stru *hal_device)
{
    hmac_set_rx_comp_triggered(hal_device->device_id);
    hmac_rx_comp_scheduled();
}

int32_t hmac_rx_comp_thread(void *p_data);
void hmac_rx_comp_init(void);
void hmac_rx_add_buff_process(hal_host_device_stru *hal_device);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
