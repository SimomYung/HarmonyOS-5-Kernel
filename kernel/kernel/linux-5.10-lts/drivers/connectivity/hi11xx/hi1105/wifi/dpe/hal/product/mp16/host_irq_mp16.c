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
#include "oam_ext_if.h"
#include "host_hal_irq.h"
#include "host_hal_ops.h"
#include "host_hal_ring.h"
#include "host_hal_dscr.h"
#include "host_irq_mp16.h"
#include "host_mac_mp16.h"
#include "host_dscr_mp16.h"
#include "host_hal_ops_mp16.h"
#include "hmac_rx_complete.h"
#include "hmac_rx_data.h"
#include "hmac_user.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#include "hmac_tx_complete.h"
#include "host_hal_ext_if.h"
#ifdef _PRE_WLAN_FEATURE_CSI
#include "host_csi_mp16.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_IRQ_MP16_C

/*
 * 功能描述   : MAC通用定时器中断处理函数
 * 1.日    期   : 2020年12月23日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp16_irq_host_common_timer_isr(hal_host_device_stru *hal_dev, uint32_t mask)
{
#ifdef _PRE_WLAN_FEATURE_VSP
    frw_event_mem_stru *event_mem = NULL;
    frw_event_stru *common_timeout_event = NULL;
    hal_host_common_timerout_stru *common_timerout = NULL;
    hal_mac_common_timer *timer = NULL;
    mp16_clear_host_mac_int_status(hal_dev, mask);

    if (!hal_dev->intr_inited) {
        oam_error_log1(0, OAM_SF_RX, "irq_host_common_timer_isr:: device[%d] not inited", hal_dev->device_id);
        oam_warning_log4(0, OAM_SF_RX, "{irq_host_common_timer_isr::int status reg[0x%x], state val[0x%x], \
            mask reg[0x%x], mask val[0x%x]}", hal_dev->mac_regs.irq_status_reg, g_int_state_reg_val,
            hal_dev->mac_regs.irq_status_mask_reg, g_int_mask_reg_val);
    }
    timer = mp16_get_host_mac_common_timer_ptr(16); // 16: host mac common timer id
    if (timer == NULL) {
        oam_warning_log0(0, 0, "{irq_host_common_timer_isr::timer not register}");
        return;
    }
    if (timer->func) {
        timer->func(timer);
    }
    event_mem = frw_event_alloc_m(sizeof(hal_host_common_timerout_stru));
    if (event_mem == NULL) {
        oam_error_log0(0, OAM_SF_FTM, "{irq_host_common_timer_isr::alloc event_mem failed.}");
        return;
    }
    common_timeout_event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(common_timeout_event->st_event_hdr), FRW_EVENT_TYPE_HOST_DDR_DRX,
                       HAL_WLAN_COMMON_TIMEOUT_IRQ_EVENT_SUBTYPE,
                       sizeof(hal_host_location_isr_stru), FRW_EVENT_PIPELINE_STAGE_0,
                       0, hal_dev->device_id, 0);

    common_timerout = (hal_host_common_timerout_stru *)common_timeout_event->auc_event_data;
    common_timerout->hal_device_id = hal_dev->device_id;
    common_timerout->chip_id = hal_dev->chip_id;
    common_timerout->timer = timer;
    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
#endif
}

void mp16_host_mac_phy_irq_mask(void)
{
    hal_host_device_stru *hal_dev = NULL;
    uint32_t mask = 0xffffffff;
    uint8_t dev_id;
    uint8_t hal_dev_num = mac_chip_get_hal_dev_num();

    for (dev_id = 0; dev_id < hal_dev_num; dev_id++) {
        hal_dev = hal_get_host_device(dev_id);
        if (hal_dev == NULL) {
            continue;
        }
        if (!hal_dev->intr_inited) {
            oam_warning_log1(0, OAM_SF_RX, "{mp16c_mac_phy_irq_mask, dev_id %d no work.}", hal_dev->device_id);
            continue;
        }
        mp16_set_host_mac_irq_mask(hal_dev, mask);
        mp16_clear_host_mac_int_status(hal_dev, mask);

        mp16_set_host_phy_irq_mask(hal_dev, mask);
        mp16_clear_host_phy_int_status(hal_dev, mask);
    }
}

void mp16_irq_host_mac_isr_process(hal_host_device_stru *hal_dev, host_intr_status_union int_state)
{
    if (int_state.bits.rpt_host_intr_rx_complete != 0) {
        /* 接收数据报文中断 */
        hal_irq_host_rx_complete_isr(hal_dev, int_state.u32);
        mp16_clear_host_mac_int_status(hal_dev, int_state.u32);
    } else if (int_state.bits.rpt_host_intr_tx_complete) {
        /* 发送完成上报中断 */
        hal_irq_host_tx_complete_isr(hal_dev, int_state.u32);
        mp16_clear_host_mac_int_status(hal_dev, int_state.u32);
    } else if (int_state.bits.rpt_host_intr_ba_info_ring_overrun) {
        /* BA info ring满中断, 需要加速BA info ring的处理 */
        oam_warning_log0(0, OAM_SF_ANY, "{irq_host_mac_isr:: BA INFO RING OVERRUN.}\r\n");
        hal_irq_host_tx_complete_isr(hal_dev, int_state.u32);
        mp16_clear_host_mac_int_status(hal_dev, int_state.u32);
#ifdef _PRE_WLAN_FEATURE_CSI
    } else if (int_state.bits.rpt_host_intr_location_complete != 0) {
        hal_irq_host_ftm_csi_isr(hal_dev, int_state.u32);
        mp16_clear_host_mac_int_status(hal_dev, int_state.u32);
#endif
    } else if (int_state.bits.rpt_host_intr_norm_ring_empty != 0) {
        oam_warning_log0(0, OAM_SF_ANY, "{irq_host_mac_isr:: NORM FREE RING EMPTY.}\r\n");
        hal_irq_host_rx_ring_empty_isr(hal_dev, int_state.u32);
        mp16_clear_host_mac_int_status(hal_dev, int_state.u32);
    } else if (int_state.bits.rpt_host_intr_small_ring_empty != 0) {
        oam_warning_log0(0, OAM_SF_ANY, "{irq_host_mac_isr:: SMALL FREE RING EMPTY.}\r\n");
    } else if (int_state.bits.rpt_host_intr_ba_win_exceed != 0) {
        /* 接收到的BA窗口越界异常中断 */
        oam_warning_log0(0, OAM_SF_ANY, "{irq_host_mac_isr:: BA WIN EXCEED.}\r\n");
    } else if (int_state.bits.rpt_host_intr_rx_ppdu_desc != 0) {
        /* RX PPDU描述符Complete Ring上报中断 */
        oam_warning_log0(0, OAM_SF_ANY, "{irq_host_mac_isr:: INTR RX PPDU DESC.}\r\n");
    } else if (int_state.bits.rpt_host_intr_data_ring_overrun != 0) {
        /* 数据帧Complete Ring满写异常中断 */
        oam_warning_log0(0, OAM_SF_ANY, "{irq_host_mac_isr:: DATA RING OVERRUN.}\r\n");
    } else if (int_state.bits.rpt_host_intr_common_timer != 0) {
        mp16_irq_host_common_timer_isr(hal_dev, int_state.u32);
        /* 通用计时器中断 */
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{hal_irq_host_mac_isr error, state.u32:0x%x.}", int_state.u32);
    }
}

void mp16_irq_host_mac_isr(hal_host_device_stru *hal_dev)
{
    uint32_t state, mask;
    host_intr_status_union int_state;

    if (hal_dev == NULL) {
        oal_io_print("irq_host_mac_isr:: input para err.\n");
        return;
    }
    if (!hal_dev->intr_inited) {
        oam_error_log3(0, OAM_SF_RX, "{irq_host_mac_isr error, dev_id %d, tx_inited %d, status 0x%x.}",
            hal_dev->device_id, hal_dev->intr_inited, hal_dev->mac_regs.irq_status_mask_reg);
        return;
    }

    /* 读取MAC中断状态,中断mask寄存器 */
    mp16_get_host_mac_int_status(hal_dev, &state);
    mp16_get_host_mac_int_mask(hal_dev, &mask);
    if (state == HAL_HOST_READL_INVALID_VAL || mask == HAL_HOST_READL_INVALID_VAL) {
        oam_error_log2(0, OAM_SF_RX, "{irq_host_mac_isr:: host_readl invalid state[0x%x] mask[0x%x]}", state, mask);
        return;
    }

    /* 清除已mask却继续上报的中断 */
    if ((mask & state) != 0) {
        mp16_clear_host_mac_int_status(hal_dev, mask);
    }

    g_int_state_reg_val = state;
    g_int_mask_reg_val = mask;
    /* 一次处理所有中断 */
    state &= ~mask;
    int_state.u32 = state;
    while (state) {
        /* 保留最低置1位，其它位为0 */
        int_state.u32 = (uint32_t)((uint32_t)(-state) & state);
        state &= ~((uint32_t)int_state.u32);
        mp16_irq_host_mac_isr_process(hal_dev, int_state);
    }
    mp16_clear_host_mac_int_status(hal_dev, int_state.u32);
}

#define MP16_MAC_RPT_HOST_INTR_RX_MASK (MP16_MAC_RPT_HOST_INTR_RX_COMPLETE_MASK | \
                                           MP16_MAC_RPT_HOST_INTR_NORM_RING_EMPTY_MASK | \
                                           MP16_MAC_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK | \
                                           MP16_MAC_RPT_HOST_INTR_DATA_RING_OVERRUN_MASK)
void mp16_host_mac_clear_rx_irq(hal_host_device_stru *hal_device)
{
    mp16_clear_host_mac_int_status(hal_device, MP16_MAC_RPT_HOST_INTR_RX_MASK);
}
void mp16_host_mac_mask_rx_irq(hal_host_device_stru *hal_device)
{
    uint32_t mask;
    mp16_get_host_mac_int_mask(hal_device, &mask);

    mask |= MP16_MAC_RPT_HOST_INTR_RX_MASK;
    mp16_set_host_mac_irq_mask(hal_device, mask);
}
void mp16_mac_irq_host_all(void *p_dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)p_dev;
    mp16_irq_host_mac_isr(hal_dev);
}

/*
 * 功能描述  : phy中断处理
 * 1.日    期  : 2020年2月23日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void mp16_irq_host_phy_isr(hal_host_device_stru *hal_dev)
{
    uint32_t               state = 0;
    uint32_t               mask = 0;
    phy_intr_rpt_union     int_state;

    if (hal_dev == NULL) {
        oal_io_print("host_phy_isr input para err\n");
        return;
    }
    if (!hal_dev->intr_inited) {
        oam_error_log3(0, OAM_SF_RX, "{irq_host_phy_isr error, dev_id %d, tx_inited %d, status 0x%x.}",
            hal_dev->device_id, hal_dev->intr_inited, hal_dev->phy_regs.irq_status_reg);
        return;
    }
    /* 读取MAC中断状态寄存器 */
    mp16_get_host_phy_int_status(hal_dev, &state);

    /* 读取中断mask寄存器 */
    mp16_get_host_phy_int_mask(hal_dev, &mask);
    if (state == HAL_HOST_READL_INVALID_VAL || mask == HAL_HOST_READL_INVALID_VAL) {
        oam_error_log2(0, OAM_SF_RX, "{irq_host_phy_isr:: hal_host_readl invalid val state[0x%x] mask[0x%x]}",
                       state, mask);
        return;
    }

    state &= ~mask;
    int_state.u32 = state;
    mp16_clear_host_phy_int_status(hal_dev, int_state.u32);
}

void mp16_phy_irq_host_all(void *p_dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)p_dev;
    mp16_irq_host_phy_isr(hal_dev);
}

/*
 * 功能描述  : host中断注册
 * 1.日    期  : 2020年2月23日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void mp16_host_chip_irq_init(void)
{
    static pcie_wlan_callback_group_stru st_isr_cb;

    memset_s(&st_isr_cb, sizeof(pcie_wlan_callback_group_stru),
        0, sizeof(pcie_wlan_callback_group_stru));

    st_isr_cb.pcie_mac_2g_isr_callback.para = (void *)hal_get_host_device(0);
    st_isr_cb.pcie_mac_2g_isr_callback.pf_func = mp16_mac_irq_host_all;
    st_isr_cb.pcie_phy_2g_isr_callback.para = (void *)hal_get_host_device(0);
    st_isr_cb.pcie_phy_2g_isr_callback.pf_func = mp16_phy_irq_host_all;

    st_isr_cb.pcie_mac_5g_isr_callback.para = (void *)hal_get_host_device(1);
    st_isr_cb.pcie_mac_5g_isr_callback.pf_func = mp16_mac_irq_host_all;
    st_isr_cb.pcie_phy_5g_isr_callback.para = (void *)hal_get_host_device(1);
    st_isr_cb.pcie_phy_5g_isr_callback.pf_func = mp16_phy_irq_host_all;

    pcie_wlan_func_register(&st_isr_cb);
    oam_warning_log0(0, OAM_SF_RX, "{host chip irq registered succ.}");
}
