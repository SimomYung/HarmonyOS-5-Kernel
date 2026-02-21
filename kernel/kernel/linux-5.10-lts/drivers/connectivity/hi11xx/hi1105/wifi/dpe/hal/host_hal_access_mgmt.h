/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Host访问Device控制
 * 作    者 : wifi
 * 创建日期 : 2023年2月13日
 */

#ifndef HOST_HAL_ACCESS_MGMT_H
#define HOST_HAL_ACCESS_MGMT_H

#include "host_hal_device.h"

#define HAL_HOST_READL_INVALID_VAL 0xffffffff

typedef enum {
    HAL_PM_FORBIDE_SLEEP_FTM_STATE,
    HAL_PM_FORBIDE_SLEEP_RESET_RX,
    HAL_PM_FORBIDE_SLEEP_CSI_CONFIG,
    HAL_PM_FORBIDE_SLEEP_HOST_REG_ADDR_INIT,
    HAL_PM_FORBIDE_SLEEP_SET_MAC_COMMON_TIMER,
    HAL_PM_FORBIDE_SLEEP_GET_TSF_LO,
    HAL_PM_FORBIDE_SLEEP_HOST_RING_TX,
    HAL_PM_FORBIDE_SLEEP_TID_PUSH_NETBUF_LIST,
    HAL_PM_FORBIDE_SLEEP_UPDATE_MLINK_RING_RPTR,
    HAL_PM_FORBIDE_SLEEP_UPDATE_TID_RING_RPTR,
    HAL_PM_FORBIDE_SLEEP_PROCESS_BY_BA_INFO,
    HAL_PM_FORBIDE_SLEEP_TX_TRIG_DEVICE_SOFTIRQ,
    HAL_PM_FORBIDE_SLEEP_HOST_TX_RING_INIT,
    HAL_PM_FORBIDE_SLEEP_WAIT_RING_TX_DONE,
    HAL_PM_FORBIDE_SLEEP_HOST_TX_RING_DEINIT,
    HAL_PM_FORBIDE_SLEEP_D2H_RX_MODE_SYN,
    HAL_PM_FORBIDE_SLEEP_VSP_SOURCE_TIMEOUT,
    HAL_PM_FORBIDE_SLEEP_VSP_TX_COMPLETE,
    HAL_PM_FORBIDE_SLEEP_VSP_INIT_COMMON_TIMER,
    HAL_PM_FORBIDE_SLEEP_STATE_BUTT = 0x20 // 最多32个枚举
} hal_pm_forbid_sleep_bitmap_type_enum;


void hal_host_forbid_access_device(void);
void hal_host_approve_access_device(void);
void hal_host_access_device_init(void);
uint32_t hal_host_readl(uintptr_t addr);
void hal_host_writel(uint32_t val, uintptr_t addr);
void hal_host_writew(uint16_t val, uintptr_t addr);
uint32_t hal_pm_try_wakeup_dev_lock(void);
void hal_pm_try_wakeup_forbid_sleep(hal_pm_forbid_sleep_bitmap_type_enum event);
void hal_pm_try_wakeup_allow_sleep(hal_pm_forbid_sleep_bitmap_type_enum event);
void hal_pm_try_wakeup_reset_forbid_sleep(void);
uint32_t hal_host_readl_irq(uintptr_t addr);
void hal_host_writel_irq(uint32_t val, uintptr_t addr);
void hal_pm_check_forbid_sleep_in_timer(void);
void hal_pm_set_check_forbid_sleep_enable(oal_bool_enum_uint8 enable);
#endif
