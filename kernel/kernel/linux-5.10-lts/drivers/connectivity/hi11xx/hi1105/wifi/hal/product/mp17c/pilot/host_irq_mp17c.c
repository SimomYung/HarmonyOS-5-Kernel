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
#include "mac_frame.h"
#include "host_hal_irq.h"
#include "host_hal_ops.h"
#include "host_hal_ring.h"
#include "host_hal_dscr.h"
#include "host_hal_device.h"
#include "host_mac_mp17c.h"
#include "host_dscr_mp17c.h"
#include "host_irq_mp17c.h"
#ifdef _PRE_WLAN_FEATURE_CSI
#include "host_csi_mp17c.h"
#endif
#include "hmac_rx_complete.h"
#include "hmac_rx_data.h"
#include "hmac_user.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#include "hmac_tx_complete.h"
#include "host_hal_ext_if.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_IRQ_MP17C_C

#ifdef _PRE_WLAN_FEATURE_VSP
static void mp17c_irq_host_common_timer_vsp_event(hal_host_device_stru *hal_dev, hal_mac_common_timer *timer)
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
                       HAL_WLAN_COMMON_TIMEOUT_IRQ_EVENT_SUBTYPE, sizeof(hal_host_location_isr_stru),
                       FRW_EVENT_PIPELINE_STAGE_0, 0, hal_dev->device_id, 0);

    common_timerout = (hal_host_common_timerout_stru *)common_timeout_event->auc_event_data;
    common_timerout->hal_device_id = hal_dev->device_id;
    common_timerout->chip_id = hal_dev->chip_id;
    common_timerout->timer = timer;

    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
}

static inline void mp17c_irq_host_common_timer_vsp_isr(hal_host_device_stru *hal_dev)
{
    hal_mac_common_timer *timer = mp17c_get_host_mac_common_timer_ptr(16);

    if (!timer) {
        oam_warning_log0(0, 0, "{irq_host_common_timer_vsp_isr::timer not register}");
        return;
    }

    if (timer->func) {
        timer->func(timer);
    }

    mp17c_irq_host_common_timer_vsp_event(hal_dev, timer);
}
#endif

/*
 * 功能描述   : MAC通用定时器中断处理函数
 * 1.日    期   : 2020年12月23日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp17c_irq_host_common_timer_isr(hal_host_device_stru *hal_dev)
{
#ifdef _PRE_WLAN_FEATURE_VSP
    if (!hal_dev->intr_inited) {
        oam_error_log1(0, OAM_SF_RX, "irq_host_common_timer_isr:: device[%d] not inited", hal_dev->device_id);
        oam_warning_log4(0, OAM_SF_RX, "{irq_host_common_timer_isr::int status reg[0x%x], state val[0x%x], \
            mask reg[0x%x], mask val[0x%x]}", hal_dev->mac_regs.irq_status_reg, g_int_state_reg_val,
            hal_dev->mac_regs.irq_status_mask_reg, g_int_mask_reg_val);
    }

    mp17c_irq_host_common_timer_vsp_isr(hal_dev);
#endif
}
static uint32_t mp17c_regs_smac_intr_transfer(hal_host_device_stru *hal_dev, uint32_t reg_addr, uint64_t *host_va)
{
    uint32_t mac_reg_offset[WLAN_DEVICE_MAX_NUM_PER_CHIP_MP17C] = {0, MP17C_MAC_BANK_REG_OFFSET};
    uint32_t offset = mac_reg_offset[hal_dev->device_id];
    if (oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, reg_addr + offset, host_va) != OAL_SUCC) {
        oam_error_log1(hal_dev->device_id, OAM_SF_ANY, "{mp17c_regs_smac_intr_transfer::regaddr[%x] \
         devca2hostva fail.}", reg_addr);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
void mp17c_irq_host_smac_isr(hal_host_device_stru *hal_dev)
{
    uint64_t smac_intr_clr_reg = 0;
    uint64_t smac_intr_status_reg = 0;
    uint64_t smac_intr_mask_reg = 0;
    uint32_t smac_intr_status = 0;
    uint32_t host_intr_mask = hal_host_readl_irq(hal_dev->mac_regs.irq_status_mask_reg);

    mp17c_regs_smac_intr_transfer(hal_dev, MAC_GLB_REG_SMAC_INTR_CLR_REG, &smac_intr_clr_reg);
    mp17c_regs_smac_intr_transfer(hal_dev, MAC_GLB_REG_SMAC_INTR_STATUS_REG, &smac_intr_status_reg);
    mp17c_regs_smac_intr_transfer(hal_dev, MAC_GLB_REG_SMAC_INTR_MASK_REG, &smac_intr_mask_reg);
    /* 清smac中断 */
    smac_intr_status = hal_host_readl_irq((uintptr_t)smac_intr_status_reg);
    hal_host_writel_irq(smac_intr_status, (uintptr_t)smac_intr_mask_reg);
    oam_warning_log1(hal_dev->device_id, OAM_SF_ANY, "{mp17c_irq_host_smac_isr::smac_intr_status[%x]",
        smac_intr_status);
    /* 屏蔽smac中断 */
    hal_host_writel_irq(0xffffffff, (uintptr_t)smac_intr_mask_reg);
    hal_host_writel_irq(host_intr_mask | MAC_GLB_REG_CFG_HOST_INTR_MASK_SMAC_INTR_MASK,
        (uintptr_t)hal_dev->mac_regs.irq_status_mask_reg);
}
/*
 * 功能描述   : mp17c_irq_host_ftm_csi_isr中断服务例程
 * 1.日    期   : 2020年11月13日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp17c_irq_host_ftm_csi_isr(uint32_t status)
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

void mp17c_host_mac_phy_irq_mask(void)
{
    hal_host_device_stru *hal_dev = NULL;
    uint32_t mask = 0xffffffff;
    uint8_t dev_id;
    uint8_t hal_dev_num = mac_chip_get_hal_dev_num();
    hal_intr_rpt_stru status_mask;
    hal_intr_state_stru irq_status_mask;

    status_mask.intr_rpt1.u32 = 0xffffffff;
    status_mask.intr_rpt2.u32 = 0xffffffff;
    irq_status_mask.value1 = 0xffffffff;
    irq_status_mask.value2 = 0xffffffff;
    for (dev_id = 0; dev_id < hal_dev_num; dev_id++) {
        hal_dev = hal_get_host_device(dev_id);
        if (hal_dev == NULL) {
            continue;
        }
        if (!hal_dev->intr_inited) {
            oam_warning_log1(0, OAM_SF_IRQ, "{mp17c_mac_phy_irq_mask, dev_id %d no work.}", hal_dev->device_id);
            continue;
        }
        mp17c_set_host_mac_irq_mask(hal_dev, mask);
        mp17c_clear_host_mac_int_status(hal_dev, mask);

        mp17c_set_host_phy_int_mask(hal_dev, &irq_status_mask);
        mp17c_clear_host_phy_int_status(hal_dev, &status_mask);
    }
}

static void mp17c_irq_host_mac_comm_isr_process(hal_host_device_stru *hal_dev,
    host_comm_intr_status_union intr_state)
{
    if (intr_state.bits.rpt_host_intr_location_complete != 0) {
        mp17c_irq_host_ftm_csi_isr(intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_rx_ppdu_desc != 0) {
        /* RX PPDU描述符Complete Ring上报中断 */
        mp17c_clear_host_mac_common_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_data_ring_overrun != 0) {
        /* 数据帧Complete Ring满写异常中断 */
        mp17c_clear_host_mac_common_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_rx_complete != 0) {
        /* 接收数据报文中断 */
        hal_irq_host_rx_complete_isr(hal_dev, intr_state.u32);
        mp17c_clear_host_mac_common_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_norm_ring_empty != 0) {
        /* free ring空中断 */
        hal_irq_host_rx_ring_empty_isr(hal_dev, intr_state.u32);
        mp17c_clear_host_mac_common_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_small_ring_empty != 0) {
        /* free ring空中断 */
        hal_irq_host_rx_ring_empty_isr(hal_dev, intr_state.u32);
        mp17c_clear_host_mac_common_int_status(hal_dev, intr_state.u32);
    } else {
        oam_warning_log1(0, OAM_SF_IRQ,
            "{mp17c_irq_host_mac_comm_isr_process error, state.u32:0x%x.}", intr_state.u32);
        mp17c_clear_host_mac_common_int_status(hal_dev, intr_state.u32);
    }
}
static void mp17c_irq_host_mac_isr_process(hal_host_device_stru *hal_dev, host_intr_status_union intr_state)
{
    if (intr_state.bits.rpt_host_intr_tx_complete != 0) {
        /* 发送完成上报中断 */
        hal_irq_host_tx_complete_isr(hal_dev, intr_state.u32);
        mp17c_clear_host_mac_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_rx_self_frame != 0) {
        /* MAC地址冲突异常中断,芯片会丢帧 */
        oam_error_log1(0, OAM_SF_IRQ, "{mp17c_irq_host_mac_isr_process error(rx self frame), state.u32:0x%x.}",
            intr_state.u32);
        mp17c_clear_host_mac_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_ba_win_exceed != 0) {
        /* 接收到的BA窗口越界异常中断 */
        mp17c_clear_host_mac_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_common_timer != 0) {
        /* 通用计时器中断 */
        mp17c_irq_host_common_timer_isr(hal_dev);
        mp17c_clear_host_mac_int_status(hal_dev, intr_state.u32);
    } else if (intr_state.bits.rpt_host_intr_smac_intr_o != 0) {
        mp17c_irq_host_smac_isr(hal_dev);
        mp17c_clear_host_mac_int_status(hal_dev, intr_state.u32);
    } else {
        oam_warning_log1(0, OAM_SF_IRQ, "{mp17c_irq_host_mac_isr_process error, state.u32:0x%x.}", intr_state.u32);
        mp17c_clear_host_mac_int_status(hal_dev, intr_state.u32);
    }
}

static void mp17c_irq_host_mac_isr_process_all(hal_host_device_stru *hal_dev, uint32_t *state)
{
    host_intr_status_union intr_state;
    uint32_t state_tmp = *state;

    /* 处理所有中断 */
    intr_state.u32 = state_tmp;
    while (state_tmp) {
        /* 保留最低置1位，其它位为0 */
        intr_state.u32 = (uint32_t)((uint32_t)(-state_tmp) & state_tmp);
        state_tmp &= ~((uint32_t)intr_state.u32);
        mp17c_irq_host_mac_isr_process(hal_dev, intr_state);
    }
    *state = intr_state.u32;
}

static void mp17c_irq_host_mac_common_isr_process_all(hal_host_device_stru *hal_dev, uint32_t *state)
{
    host_comm_intr_status_union intr_state;
    uint32_t state_tmp = *state;

    /* 处理所有中断 */
    intr_state.u32 = state_tmp;
    while (state_tmp) {
        /* 保留最低置1位，其它位为0 */
        intr_state.u32 = (uint32_t)((uint32_t)(-state_tmp) & state_tmp);
        state_tmp &= ~((uint32_t)intr_state.u32);
        mp17c_irq_host_mac_comm_isr_process(hal_dev, intr_state);
    }
    *state = intr_state.u32;
}

void mp17c_irq_host_mac_isr(hal_host_device_stru *hal_dev)
{
    uint32_t state = 0;
    uint32_t mask = 0;

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
    mp17c_get_host_mac_int_status(hal_dev, &state);
    mp17c_get_host_mac_int_mask(hal_dev, &mask);
    if (state == HAL_HOST_READL_INVALID_VAL || mask == HAL_HOST_READL_INVALID_VAL) {
        oam_error_log2(0, OAM_SF_IRQ, "{irq_host_mac_isr:: host_readl invalid state[0x%x] mask[0x%x]}", state, mask);
        return;
    }

    /* 清除已mask却继续上报的中断 */
    if ((mask & state) != 0) {
        mp17c_clear_host_mac_int_status(hal_dev, mask);
    }

    g_int_state_reg_val = state;
    g_int_mask_reg_val = mask;
    /* 清除被屏蔽的中断 */
    state &= ~mask;

    mp17c_irq_host_mac_isr_process_all(hal_dev, &state);
    mp17c_clear_host_mac_int_status(hal_dev, state);
}

#define MP17C_MAC_COMMON_REG_RPT_HOST_INTR_RX_MASK (MP17C_MAC_COMMON_REG_RPT_HOST_INTR_RX_COMPLETE_MASK | \
                                                   MP17C_MAC_COMMON_REG_RPT_HOST_INTR_NORM_RING_EMPTY_MASK | \
                                                   MP17C_MAC_COMMON_REG_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK | \
                                                   MP17C_MAC_COMMON_REG_RPT_HOST_INTR_DATA_RING_OVERRUN_MASK)
void mp17c_host_mac_clear_rx_irq(hal_host_device_stru *hal_device)
{
    mp17c_clear_host_mac_common_int_status(hal_device, MP17C_MAC_COMMON_REG_RPT_HOST_INTR_RX_MASK);
}
void mp17c_host_mac_mask_rx_irq(hal_host_device_stru *hal_device)
{
    uint32_t mask;
    mp17c_get_host_mac_common_int_mask(hal_device, &mask);

    mask |= MP17C_MAC_COMMON_REG_RPT_HOST_INTR_RX_MASK;
    mp17c_set_host_mac_common_irq_mask(hal_device, mask);
}
void mp17c_mac_irq_host_all(void *p_dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)p_dev;
    mp17c_irq_host_mac_isr(hal_dev);
}

/*
 * 功能描述  : phy中断处理
 * 1.日    期  : 2020年2月23日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void mp17c_irq_host_phy_isr(hal_host_device_stru *hal_dev)
{
    hal_intr_state_stru state = {0};
    hal_intr_state_stru mask = {0};
    hal_intr_rpt_stru  int_state;

    if (hal_dev == NULL) {
        oal_io_print("host_phy_isr input para err\n");
        return;
    }
    if (hal_dev->intr_inited != OAL_TRUE) {
        oam_error_log2(0, OAM_SF_IRQ, "{irq_host_phy_isr error, dev_id %d, status reg 0x%x.}",
                       hal_dev->device_id, hal_dev->phy_regs.irq_status_reg);
        return;
    }
    /* 读取MAC中断状态寄存器 */
    mp17c_get_host_phy_int_status(hal_dev, &state);

    /* 读取中断mask寄存器 */
    mp17c_get_host_phy_int_mask(hal_dev, &mask);
    if (((state.value1 == HAL_HOST_READL_INVALID_VAL) && (state.value2 == HAL_HOST_READL_INVALID_VAL)) ||
        ((mask.value1 == HAL_HOST_READL_INVALID_VAL) && (mask.value2 == HAL_HOST_READL_INVALID_VAL))) {
        oam_error_log4(0, OAM_SF_IRQ, "{irq_host_phy_isr:: \
            hal_host_readl invalid val state1[0x%x] state2[0x%x] mask1[0x%x] mask2[0x%x]}",
            state.value1, state.value2, mask.value1, mask.value2);
        return;
    }

    state.value1 &= ~mask.value1;
    state.value2 &= ~mask.value2;
    int_state.intr_rpt1.u32 = state.value1;
    int_state.intr_rpt2.u32 = state.value2;
    mp17c_clear_host_phy_int_status(hal_dev, &int_state);
}

void mp17c_phy_irq_host_all(void *p_dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)p_dev;

    oam_error_log0(0, 0, "mp17c_phy_irq_host_all:: phy error intr!");
    mp17c_irq_host_phy_isr(hal_dev);
}

static void mp17c_mac_comm_except_rx_complete_irq(hal_host_device_stru *hal_dev, uint32_t state, uint32_t mask)
{
    host_comm_intr_status_union intr_state;
    if (((state & MAC_COMMON_REG_RPT_HOST_INTR_RX_COMPLETE_MASK) != 0) &&
        (((mask & MAC_COMMON_REG_RPT_HOST_INTR_RX_COMPLETE_MASK) != 0))) {
        intr_state.u32 = MAC_COMMON_REG_RPT_HOST_INTR_RX_COMPLETE_MASK;
        oam_error_log2(0, OAM_SF_IRQ,
            "{mac_comm_irq_host_all::rx_complete_irq rpt after mask state[0x%x] mask[0x%x]}", state, mask);
        mp17c_irq_host_mac_comm_isr_process(hal_dev, intr_state);
    }
}

void mp17c_mac_comm_irq_host_all(void *dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)dev;
    uint32_t state = 0;
    uint32_t mask = 0;

    if (hal_dev == NULL) {
        oal_io_print("mac_comm_irq_host_all:: input para err.\n");
        return;
    }

    if (!hal_dev->intr_inited) {
        oam_error_log1(0, OAM_SF_IRQ, "{mac_comm_irq_host_all:: error status reg 0x%x.}",
            hal_dev->mac_regs.irq_common_status_reg);
        return;
    }
    mp17c_get_host_mac_common_int_status(hal_dev, &state);
    mp17c_get_host_mac_common_int_mask(hal_dev, &mask);
    mp17c_mac_comm_except_rx_complete_irq(hal_dev, state, mask);
    if (state == HAL_HOST_READL_INVALID_VAL || mask == HAL_HOST_READL_INVALID_VAL) {
        oam_error_log2(0, OAM_SF_IRQ,
            "{mac_comm_irq_host_all:: host_readl invalid state[0x%x] mask[0x%x]}", state, mask);
        return;
    }

    /* 清除已mask却继续上报的中断 */
    if ((mask & state) != 0) {
        mp17c_clear_host_mac_common_int_status(hal_dev, mask);
    }

    g_int_state_reg_val = state;
    g_int_mask_reg_val = mask;

    state &= ~mask;
    mp17c_irq_host_mac_common_isr_process_all(hal_dev, &state);
    mp17c_clear_host_mac_common_int_status(hal_dev, mask);
}

/*
 * 功能描述  : host中断注册
 * 1.日    期  : 2020年2月23日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void mp17c_host_chip_irq_init(void)
{
    static pcie_wlan_callback_group_stru st_isr_cb;

    oal_io_print("host_chip_irq_init registered succ.\r\n");
    memset_s(&st_isr_cb, sizeof(pcie_wlan_callback_group_stru),
        0, sizeof(pcie_wlan_callback_group_stru));

    st_isr_cb.pcie_mac_2g_isr_callback.para = (void *)hal_get_host_device(0);
    st_isr_cb.pcie_mac_2g_isr_callback.pf_func = mp17c_mac_irq_host_all;      /* host mac中断 */
    st_isr_cb.pcie_phy_2g_isr_callback.para = (void *)hal_get_host_device(0);
    st_isr_cb.pcie_phy_2g_isr_callback.pf_func = mp17c_phy_irq_host_all;      /* host phy中断 */

    st_isr_cb.pcie_mac_5g_isr_callback.para = (void *)hal_get_host_device(1);
    st_isr_cb.pcie_mac_5g_isr_callback.pf_func = mp17c_mac_irq_host_all;      /* host mac中断 */
    st_isr_cb.pcie_phy_5g_isr_callback.para = (void *)hal_get_host_device(1);
    st_isr_cb.pcie_phy_5g_isr_callback.pf_func = mp17c_phy_irq_host_all;      /* host phy中断 */

    st_isr_cb.pcie_mac_common_isr_callback.para = (void *)hal_comm_rx_get_host_device();
    st_isr_cb.pcie_mac_common_isr_callback.pf_func = mp17c_mac_comm_irq_host_all; /* host mac common中断 */

    pcie_wlan_func_register(&st_isr_cb);
    oam_warning_log0(0, OAM_SF_IRQ, "{host chip irq registered succ.}");
}
