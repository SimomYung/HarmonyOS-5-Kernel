/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host中断功能实现
 * 作    者 : wifi
 * 创建日期 : 2012年9月18日
 */

#include "pcie_linux.h"
#include "pcie_host.h"
#include "oal_util.h"
#include "oal_net.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "host_hal_irq.h"
#include "host_hal_ops.h"
#include "host_hal_ring.h"
#include "host_hal_dscr.h"
#include "host_hal_device.h"
#include "host_mac_mp12.h"
#include "host_dscr_mp12.h"
#include "host_irq_mp12.h"
#ifdef _PRE_WLAN_FEATURE_CSI
#include "host_csi_mp12.h"
#endif
#include "hmac_rx_complete.h"
#include "hmac_rx_data.h"
#include "hmac_user.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#include "hmac_tx_complete.h"
#include "host_hal_ext_if.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_IRQ_MP12_C

#ifdef _PRE_WLAN_FEATURE_VSP
static void mp12_irq_host_common_timer_vsp_event(hal_host_device_stru *hal_dev, hal_mac_common_timer *timer)
{
    frw_event_stru *common_timeout_event = NULL;
    hal_host_common_timerout_stru *common_timerout = NULL;
    frw_event_mem_stru *event_mem = frw_event_alloc_m(sizeof(hal_host_common_timerout_stru));

    if (!event_mem) {
        oam_error_log0(0, OAM_SF_FTM, "{irq_host_common_timer_vsp_event::alloc event_mem failed.}");
        return;
    }

    common_timeout_event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&common_timeout_event->st_event_hdr, FRW_EVENT_TYPE_HOST_DDR_DRX,
                       HAL_WLAN_COMMON_TIMEOUT_IRQ_EVENT_SUBTYPE, (uint16_t)sizeof(hal_host_location_isr_stru),
                       FRW_EVENT_PIPELINE_STAGE_0, 0, hal_dev->device_id, 0);

    common_timerout = (hal_host_common_timerout_stru *)common_timeout_event->auc_event_data;
    common_timerout->hal_device_id = hal_dev->device_id;
    common_timerout->chip_id = hal_dev->chip_id;
    common_timerout->timer = timer;

    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
}

static inline void mp12_irq_host_common_timer_vsp_isr(hal_host_device_stru *hal_dev)
{
    hal_mac_common_timer *timer = mp12_get_host_mac_common_timer_ptr(7); // VSP特性使用common timer 7

    if (!timer) {
        oam_warning_log0(0, 0, "{irq_host_common_timer_vsp_isr::timer not register}");
        return;
    }

    if (timer->func) {
        timer->func(timer);
    }

    mp12_irq_host_common_timer_vsp_event(hal_dev, timer);
}
#endif

/*
 * 功能描述   : MAC通用定时器中断处理函数
 * 1.日    期   : 2020年12月23日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_irq_host_common_timer_isr(hal_host_device_stru *hal_dev, uint32_t state)
{
#ifdef _PRE_WLAN_FEATURE_VSP
    if (!hal_dev->intr_inited) {
        oam_error_log1(0, OAM_SF_RX, "irq_host_common_timer_isr:: device[%d] not inited", hal_dev->device_id);
        oam_warning_log4(0, OAM_SF_RX, "{irq_host_common_timer_isr::int status reg[0x%x], state val[0x%x], \
            mask reg[0x%x], mask val[0x%x]}", hal_dev->mac_regs.irq_status_reg, g_int_state_reg_val,
            hal_dev->mac_regs.irq_status_mask_reg, g_int_mask_reg_val);
    }

    mp12_irq_host_common_timer_vsp_isr(hal_dev);
#endif
}

/*
 * 功能描述   : mp12_irq_host_ftm_csi_isr中断服务例程
 * 1.日    期   : 2020年11月13日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_irq_host_ftm_csi_isr(hal_host_device_stru *hal_device, uint32_t status)
{
#ifdef _PRE_WLAN_FEATURE_CSI
    hal_host_device_stru *hal_dev = NULL;
    uint8_t dev_id;
    uint8_t dev_num = mac_chip_get_hal_dev_num();

    for (dev_id = 0; dev_id < dev_num; dev_id++) {
        hal_dev = hal_get_host_device(dev_id);
        if (hal_dev == NULL || hal_dev->intr_inited != OAL_TRUE) {
            continue;
        }
        hal_irq_host_ftm_csi_isr(hal_dev, status);
    }
#endif
}

void mp12_host_mac_phy_irq_mask(void)
{
    hal_host_device_stru *hal_dev = NULL;
    uint32_t mask = 0xffffffff;
    uint8_t dev_id;
    uint8_t hal_dev_num = mac_chip_get_hal_dev_num();
    hal_intr_state_stru irq_status_mask;
    irq_status_mask.value1 = 0xffffffff;
    irq_status_mask.value2 = 0xffffffff;
    for (dev_id = 0; dev_id < hal_dev_num; dev_id++) {
        hal_dev = hal_get_host_device(dev_id);
        if (hal_dev != NULL) {
            continue;
        }
        if (!hal_dev->intr_inited) {
            oam_warning_log1(0, OAM_SF_IRQ, "{mp12_mac_phy_irq_mask, dev_id %d no work.}", hal_dev->device_id);
            continue;
        }
        mp12_set_host_mac_irq_mask(hal_dev, mask);
        mp12_clear_host_mac_int_status(hal_dev, mask);

        mp12_set_host_phy_int_mask(hal_dev, &irq_status_mask);
        mp12_clear_host_phy_int_status(hal_dev, &irq_status_mask);
    }
}

/* MAC中断处理注册接口 */
typedef void (*mac_isr_ptr)(hal_host_device_stru *hal_dev, uint32_t state);
OAL_STATIC mac_isr_ptr g_irq_mac_func_ptr_mp12[32] = {   /* mac普通中断 */
    [0] = hal_irq_host_tx_complete_isr,             /* 发送完成上报中断 */
    [1] = NULL,
    [2] = NULL,
    [3] = NULL,
    [4] = NULL,
    [5] = NULL,
    [6] = NULL,
    [7] = NULL,
    [8] = NULL,
    [9] = NULL,
    [10] = NULL,
    [11] = NULL,
    [12] = NULL,
    [13] = mp12_irq_host_common_timer_isr,          /* 通用定时器中断 */
    [14] = NULL,
    [15] = NULL,
    [16] = NULL,
    [17] = NULL,
    [18] = NULL,
    [19] = NULL,
    [20] = NULL,
    [21] = NULL,
    [22] = NULL,
    [23] = NULL,
    [24] = NULL,
    [25] = NULL,
    [26] = NULL,
    [27] = NULL,
    [28] = NULL,
    [29] = NULL,
    [30] = NULL,
    [31] = NULL,
};

void mp12_irq_host_mac_isr(hal_host_device_stru *hal_dev)
{
    uint32_t state = 0;
    uint32_t mask = 0;
    uint32_t bit = 0;
    uint32_t irq_bit = 0;

    if (hal_dev == NULL) {
        oal_io_print("irq_host_mac_isr:: input para err.\n");
        return;
    }

    if (!hal_dev->intr_inited) {
        oam_error_log2(0, OAM_SF_IRQ, "{irq_host_mac_isr error, dev_id %d, status reg 0x%x.}",
                       hal_dev->device_id, hal_dev->mac_regs.irq_status_mask_reg);
        return;
    }

    /* 读取MAC中断状态,中断mask寄存器 */
    mp12_get_host_mac_int_status(hal_dev, &state);
    mp12_get_host_mac_int_mask(hal_dev, &mask);
    if (state == HAL_HOST_READL_INVALID_VAL || mask == HAL_HOST_READL_INVALID_VAL) {
        oam_error_log2(0, OAM_SF_IRQ, "{irq_host_mac_isr:: host_readl invalid state[0x%x] mask[0x%x]}", state, mask);
        mp12_clear_host_mac_int_status(hal_dev, HAL_HOST_READL_INVALID_VAL);
        return;
    }

    /* 清除已mask却继续上报的中断 */
    if ((mask & state) != 0) {
        mp12_clear_host_mac_int_status(hal_dev, mask);
    }
    mp12_clear_host_mac_int_status(hal_dev, state);
    g_int_state_reg_val = state;
    g_int_mask_reg_val = mask;
    /* 清除被屏蔽的中断 */
    state &= ~mask;

    /* 从bit0遍历 */
    for (bit = 0; bit < sizeof(state) * 8; bit++) { /* 8 is bits of bytes */
        irq_bit = 1 << bit;
        if (state & irq_bit) {
            if (g_irq_mac_func_ptr_mp12[bit] != NULL) {
                g_irq_mac_func_ptr_mp12[bit](hal_dev, irq_bit);
            } else {
                oam_warning_log1(0, OAM_SF_IRQ, "{mp12_irq_host_mac_isr:: bit:[%d].}", bit);
                mp12_clear_host_mac_int_status(hal_dev, irq_bit); /* 清中断状态 */
            }
            state &= ~irq_bit;
        }
        if (!state) { /* all done */
            break;
        }
    }
}

#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_RX_MASK (MP12_MAC_COMMON_REG_RPT_HOST_INTR_RX_COMPLETE_MASK | \
                                                   MP12_MAC_COMMON_REG_RPT_HOST_INTR_NORM_RING_EMPTY_MASK | \
                                                   MP12_MAC_COMMON_REG_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK | \
                                                   MP12_MAC_COMMON_REG_RPT_HOST_INTR_DATA_RING_OVERRUN_MASK)
void mp12_host_mac_clear_rx_irq(hal_host_device_stru *hal_device)
{
    mp12_clear_host_mac_common_int_status(hal_device, MP12_MAC_COMMON_REG_RPT_HOST_INTR_RX_MASK);
}
void mp12_host_mac_mask_rx_irq(hal_host_device_stru *hal_device)
{
    uint32_t mask;
    mp12_get_host_mac_common_int_mask(hal_device, &mask);

    mask |= MP12_MAC_COMMON_REG_RPT_HOST_INTR_RX_MASK;
    mp12_set_host_mac_common_irq_mask(hal_device, mask);
}
void mp12_mac_irq_host_all(void *p_dev)
{
    mp12_irq_host_mac_isr((hal_host_device_stru *)p_dev);
}

/*
 * 功能描述  : phy中断处理
 * 1.日    期  : 2020年2月23日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void mp12_irq_host_phy_isr(hal_host_device_stru *hal_dev)
{
    hal_intr_state_stru state = {0};
    hal_intr_state_stru mask = {0};
    hal_intr_state_stru int_state = {0};

    if (hal_dev == NULL) {
        oal_io_print("host_phy_isr input para err\n");
        return;
    }
    if (hal_dev->intr_inited != OAL_TRUE) {
        oam_error_log2(0, OAM_SF_IRQ, "{irq_host_phy_isr error, dev_id %d, status reg 0x%x.}",
                       hal_dev->device_id, hal_dev->phy_regs.irq_status_reg);
        return;
    }
    /* 读取PHY中断状态寄存器 */
    mp12_get_host_phy_int_status(hal_dev, &state);

    /* 读取PHY中断mask寄存器 */
    mp12_get_host_phy_int_mask(hal_dev, &mask);
    if (((state.value1 == HAL_HOST_READL_INVALID_VAL) && (state.value2 == HAL_HOST_READL_INVALID_VAL)) ||
        ((mask.value1 == HAL_HOST_READL_INVALID_VAL) && (mask.value2 == HAL_HOST_READL_INVALID_VAL))) {
        oam_error_log4(0, OAM_SF_IRQ, "{irq_host_phy_isr:: \
            hal_host_readl invalid val state1[0x%x] state2[0x%x] mask1[0x%x] mask2[0x%x]}",
            state.value1, state.value2, mask.value1, mask.value2);
        return;
    }

    state.value1 &= ~mask.value1;
    state.value2 &= ~mask.value2;
    int_state.value1 = state.value1;
    int_state.value2 = state.value2;
    mp12_clear_host_phy_int_status(hal_dev, &int_state);
}

void mp12_phy_irq_host_all(void *p_dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)p_dev;

    oam_error_log0(0, 0, "mp12_phy_irq_host_all:: phy error intr!");
    mp12_irq_host_phy_isr(hal_dev);
}

/* MAC COMMON中断处理注册接口 */
typedef void (*mac_comm_isr_ptr)(hal_host_device_stru *hal_dev, uint32_t state);
OAL_STATIC mac_comm_isr_ptr g_irq_mac_comm_func_ptr_mp12[32] = {
    [0] = mp12_irq_host_ftm_csi_isr,                /* 发送完成上报中断 */
    [1] = NULL,
    [2] = NULL,
    [3] = NULL,
    [4] = NULL,
    [5] = NULL,
    [6] = NULL,
    [7] = NULL,
    [8] = NULL,
    [9] = NULL,
    [10] = NULL,
    [11] = NULL,
    [12] = NULL,
    [13] = NULL,
    [14] = hal_irq_host_rx_complete_isr,            /* 接收数据报文中断 */
    [15] = NULL,
    [16] = hal_irq_host_rx_ring_empty_isr,          /* free ring空中断 */
    [17] = hal_irq_host_rx_ring_empty_isr,          /* free ring空中断 */
    [18] = NULL,
    [19] = NULL,
    [20] = NULL,
    [21] = NULL,
    [22] = NULL,
    [23] = NULL,
    [24] = NULL,
    [25] = NULL,
    [26] = NULL,
    [27] = NULL,
    [28] = NULL,
    [29] = NULL,
    [30] = NULL,
    [31] = NULL,
};

void mp12_mac_comm_irq_host_all(void *dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)dev;
    uint32_t state = 0;
    uint32_t mask = 0;
    uint32_t bit = 0;
    uint32_t irq_bit = 0;

    if (hal_dev == NULL) {
        oal_io_print("mac_comm_irq_host_all:: input para err.\n");
        return;
    }

    if (!hal_dev->intr_inited) {
        oam_error_log1(0, OAM_SF_IRQ, "{mac_comm_irq_host_all:: error status reg 0x%x.}",
            hal_dev->mac_regs.irq_common_status_reg);
        return;
    }

    mp12_get_host_mac_common_int_status(hal_dev, &state);
    mp12_get_host_mac_common_int_mask(hal_dev, &mask);
    if (state == HAL_HOST_READL_INVALID_VAL || mask == HAL_HOST_READL_INVALID_VAL) {
        oam_error_log2(0, OAM_SF_IRQ,
            "{mac_comm_irq_host_all:: host_readl invalid state[0x%x] mask[0x%x]}", state, mask);
        mp12_clear_host_mac_common_int_status(hal_dev, HAL_HOST_READL_INVALID_VAL);
        return;
    }

    /* 清除已mask却继续上报的中断 */
    if ((mask & state) != 0) {
        mp12_clear_host_mac_common_int_status(hal_dev, mask);
    }

    mp12_clear_host_mac_common_int_status(hal_dev, state);
    g_int_state_reg_val = state;
    g_int_mask_reg_val = mask;
    state &= ~mask;

    /* 从bit0遍历 */
    for (bit = 0; bit < sizeof(state) * 8; bit++) { /* 8 is bits of bytes */
        irq_bit = 1 << bit;
        if (state & irq_bit) {
            if (g_irq_mac_comm_func_ptr_mp12[bit] != NULL) {
                g_irq_mac_comm_func_ptr_mp12[bit](hal_dev, irq_bit);
            } else {
                oam_warning_log1(0, OAM_SF_IRQ, "{mp12_mac_comm_irq_host_all:: bit:[%d].}", bit);
                mp12_clear_host_mac_common_int_status(hal_dev, irq_bit); /* 清中断状态 */
            }
            state &= ~irq_bit;
        }
        if (!state) { /* all done */
            break;
        }
    }
}

/*
 * 功能描述  : host中断注册
 * 1.日    期  : 2020年2月23日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void mp12_host_chip_irq_init(void)
{
    static pcie_wlan_callback_group_stru st_isr_cb;

    oal_io_print("host_chip_irq_init registered succ.\r\n");
    memset_s(&st_isr_cb, sizeof(pcie_wlan_callback_group_stru),
        0, sizeof(pcie_wlan_callback_group_stru));

    st_isr_cb.pcie_mac_2g_isr_callback.para = (void *)hal_get_host_device(HAL_DEVICE_ID_MASTER);
    st_isr_cb.pcie_mac_2g_isr_callback.pf_func = mp12_mac_irq_host_all;      /* host mac中断 */
    st_isr_cb.pcie_phy_2g_isr_callback.para = (void *)hal_get_host_device(HAL_DEVICE_ID_MASTER);
    st_isr_cb.pcie_phy_2g_isr_callback.pf_func = mp12_phy_irq_host_all;      /* host phy中断 */
    if (mac_chip_is_support_slave() == OAL_TRUE) {
        st_isr_cb.pcie_mac_5g_isr_callback.para = (void *)hal_get_host_device(HAL_DEVICE_ID_SLAVE);
        st_isr_cb.pcie_mac_5g_isr_callback.pf_func = mp12_mac_irq_host_all;      /* host mac中断 */
        st_isr_cb.pcie_phy_5g_isr_callback.para = (void *)hal_get_host_device(HAL_DEVICE_ID_SLAVE);
        st_isr_cb.pcie_phy_5g_isr_callback.pf_func = mp12_phy_irq_host_all;      /* host phy中断 */
    }
    st_isr_cb.pcie_mac_common_isr_callback.para = (void *)hal_comm_rx_get_host_device();
    st_isr_cb.pcie_mac_common_isr_callback.pf_func = mp12_mac_comm_irq_host_all; /* host mac common中断 */

    pcie_wlan_func_register(&st_isr_cb);
    oam_warning_log0(0, OAM_SF_IRQ, "{host chip irq registered succ.}");
}
