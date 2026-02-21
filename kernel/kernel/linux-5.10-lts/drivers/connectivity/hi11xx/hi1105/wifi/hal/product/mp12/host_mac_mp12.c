/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST MAC 处理接口
 * 作    者 : wifi
 * 创建日期 : 2012年9月18日
 */

#include "host_mac_mp12.h"
#include "pcie_host.h"
#include "oal_util.h"
#include "oal_ext_if.h"
#include "host_hal_access_mgmt.h"
#include "host_hal_ring.h"
#include "host_hal_device.h"
#include "frw_ext_if.h"
#include "wlan_spec.h"
#include "wlan_spec_host_if.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_MAC_MP12_C

void mp12_get_host_phy_int_status(hal_host_device_stru *hal_device, hal_intr_state_stru *status)
{
    status->value1 = hal_host_readl_irq(hal_device->phy_regs.irq_status_reg);
    status->value2 = hal_host_readl_irq(hal_device->phy_regs.irq_status_reg2);
}

/*
 * 功能描述   : 读中断屏蔽状态寄存器
 * 1.日    期   : 2018年5月20日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_get_host_phy_int_mask(hal_host_device_stru *hal_device, hal_intr_state_stru *mask)
{
    mask->value1 = hal_host_readl_irq(hal_device->phy_regs.irq_status_mask_reg);
    mask->value2 = hal_host_readl_irq(hal_device->phy_regs.irq_status_mask_reg2);
}
/*
 * 功能描述   : 读中断屏蔽状态寄存器
 * 1.日    期   : 2018年5月20日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_set_host_phy_int_mask(hal_host_device_stru *hal_device, hal_intr_state_stru *mask)
{
    if (hal_device == NULL) {
        return;
    }
    hal_host_writel(mask->value1, hal_device->phy_regs.irq_status_mask_reg);
    hal_host_writel(mask->value2, hal_device->phy_regs.irq_status_mask_reg2);
}
/*
 * 功能描述   : 清中断状态寄存器
 * 1.日    期   : 2018年5月20日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_clear_host_phy_int_status(hal_host_device_stru *hal_device, hal_intr_rpt_stru *status)
{
    if (hal_device == NULL) {
        return;
    }
    hal_host_writel_irq(status->intr_rpt1.u32, hal_device->phy_regs.irq_clr_reg);
    hal_host_writel_irq(status->intr_rpt2.u32, hal_device->phy_regs.irq_clr_reg2);
}


/*
 * 功能描述   : 读中断状态寄存器
 * 1.日    期   : 2018年5月20日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_get_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t *status)
{
    *status = hal_host_readl_irq(hal_device->mac_regs.irq_status_reg);
}

void mp12_get_host_mac_common_int_status(hal_host_device_stru *hal_dev, uint32_t *status)
{
    *status = hal_host_readl_irq(hal_dev->mac_regs.irq_common_status_reg);
}

/*
 * 功能描述   : 读中断屏蔽状态寄存器
 * 1.日    期   : 2018年5月20日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_get_host_mac_int_mask(hal_host_device_stru *hal_device, uint32_t *mask)
{
    *mask = hal_host_readl_irq(hal_device->mac_regs.irq_status_mask_reg);
}

void mp12_get_host_mac_common_int_mask(hal_host_device_stru *hal_device, uint32_t *mask)
{
    *mask = hal_host_readl_irq(hal_device->mac_regs.irq_common_status_mask_reg);
}

/* 功能描述 : 刷中断屏蔽irq_status_mask */
void mp12_set_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t intr_mask)
{
    if (hal_device == NULL) {
        return;
    }
    hal_host_writel(intr_mask, (uintptr_t)hal_device->mac_regs.irq_status_mask_reg);
}

/* 功能描述 : 刷中断屏蔽irq_status_mask */
void mp12_set_host_mac_common_irq_mask(hal_host_device_stru *hal_device, uint32_t intr_mask)
{
    if (hal_device == NULL) {
        return;
    }
    hal_host_writel(intr_mask, hal_device->mac_regs.irq_common_status_mask_reg);
}

/*
 * 功能描述   : 清中断状态寄存器
 * 1.日    期   : 2018年5月20日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void mp12_clear_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t status)
{
    if (hal_device == NULL) {
        return;
    }
    hal_host_writel_irq(status, hal_device->mac_regs.irq_clr_reg);
}

void mp12_clear_host_mac_common_int_status(hal_host_device_stru *hal_device, uint32_t status)
{
    if (hal_device == NULL) {
        return;
    }
    hal_host_writel_irq(status, hal_device->mac_regs.irq_common_clr_reg);
}

/*
 * 功能描述 : unmask mac中断屏蔽
 * 1.日 期 : 2019年8月23日
 * 修改内容 : 新生成函数
 */
void mp12_host_mac_irq_unmask(hal_host_device_stru *hal_device, uint32_t irq)
{
    uint32_t intr_mask;

    intr_mask = hal_host_readl(hal_device->mac_regs.irq_status_mask_reg);
    intr_mask &= (~((uint32_t)1 << irq));

    hal_host_writel(intr_mask, hal_device->mac_regs.irq_status_mask_reg);
}

/*
 * 功能描述 : mask mac中断屏蔽bit
 * 1.日 期 :  2019年8月23日
 * 作 者   :  wifi
 * 修改内容 : 新生成函数
 */
void mp12_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t irq)
{
    uint32_t intr_mask;

    intr_mask = hal_host_readl(hal_device->mac_regs.irq_status_mask_reg);
    if (intr_mask == HAL_HOST_READL_INVALID_VAL) {
        return;
    }
    intr_mask |= (((uint32_t)1 << irq));
    hal_host_writel(intr_mask, hal_device->mac_regs.irq_status_mask_reg);
}

static uint32_t mp12_common_regs_addr_get_offset(uint32_t reg_addr, uint32_t *offset)
{
#ifndef _PRE_LINUX_TEST
    switch (reg_addr) {
        case MAC_COMMON_REG_HOST_COMMON_INTR_STATUS_REG:
        case MAC_COMMON_REG_HOST_COMMON_INTR_MASK_REG:
        case MAC_COMMON_REG_HOST_COMMON_INTR_CLR_REG:
        case MAC_PPU_REG_CFG_ML0_CSI_BUF_BASE_ADDR_LSB_REG:
        case MAC_PPU_REG_CFG_ML1_CSI_BUF_BASE_ADDR_LSB_REG:
        case MAC_PPU_REG_CFG_ML0_CSI_BUF_BASE_ADDR_MSB_REG:
        case MAC_PPU_REG_CFG_ML1_CSI_BUF_BASE_ADDR_MSB_REG:
        case MAC_PPU_REG_CFG_ML0_CSI_BUF_SIZE_REG:
        case MAC_PPU_REG_CFG_ML1_CSI_BUF_SIZE_REG:
        case MAC_PPU_REG_CFG_ML0_CSI_PROCESS_REG:
        case MAC_PPU_REG_CFG_ML1_CSI_PROCESS_REG:
        case MAC_PPU_REG_CFG_ML0_CSI_WHITELIST_ADDR_LSB_0_REG:
        case MAC_PPU_REG_CFG_ML0_CSI_WHITELIST_ADDR_MSB_0_REG:
        case MAC_PPU_REG_CFG_ML1_CSI_WHITELIST_ADDR_LSB_0_REG:
        case MAC_PPU_REG_CFG_ML1_CSI_WHITELIST_ADDR_MSB_0_REG:
        case MAC_PPU_REG_LOCATION_INFO_MASK_REG:
        case MAC_PPU_REG_CSI_INFO_ADDR_LSB_REG:
        case MAC_PPU_REG_CSI_INFO_ADDR_MSB_REG:
        case MAC_PPU_REG_FTM_INFO_ADDR_MSB_REG:
        case MAC_PPU_REG_FTM_INFO_ADDR_LSB_REG:
        case MAC_PPU_REG_FTM_PROCESS_REG:
        case MAC_PPU_REG_FTM_BUF_BASE_ADDR_LSB_REG:
        case MAC_PPU_REG_FTM_BUF_BASE_ADDR_MSB_REG:
        case MAC_PPU_REG_FTM_WHITELIST_ADDR_LSB_0_REG:
        case MAC_PPU_REG_FTM_WHITELIST_ADDR_MSB_0_REG:
            *offset = 0;
            break;
        default:
            return OAL_FAIL;
    }
#endif
    return OAL_SUCC;
}

uint32_t mp12_regs_addr_get_offset(uint8_t device_id, uint32_t reg_addr)
{
    uint32_t offset = 0;
    uint32_t mac_reg_offset[WLAN_DEVICE_MAX_NUM_PER_CHIP_MP12] = {0};
    uint32_t phy_reg_offset[WLAN_DEVICE_MAX_NUM_PER_CHIP_MP12] = {0};
    uint32_t soc_reg_offset[WLAN_DEVICE_MAX_NUM_PER_CHIP_MP12] = {0};

    if (mp12_common_regs_addr_get_offset(reg_addr, &offset) == OAL_SUCC) {
        return offset;
    }
#ifndef _PRE_LINUX_TEST
    switch (reg_addr) {
        case MAC_GLB_REG_HOST_INTR_CLR_REG:
        case MAC_GLB_REG_HOST_INTR_MASK_REG:
        case MAC_GLB_REG_HOST_INTR_STATUS_REG:
        case MAC_GLB_REG_DMAC_INTR_MASK_REG:
        case MAC_RPU_REG_RX_CTRL_REG:
        case MAC_TMU_REG_VAP_TSFTIMER_RDVALL_STATUS_12_REG:
            offset = mac_reg_offset[device_id];
            break;
        case DFX_REG_BANK_CFG_INTR_CLR_REG:
        case DFX_REG_BANK_CFG_INTR_MASK_HOST_REG:
        case DFX_REG_BANK_PHY_INTR_RPT_REG:
        case DFX_REG_BANK_CFG_INTR_CLR2_REG:
        case DFX_REG_BANK_CFG_INTR_MASK_HOST2_REG:
        case DFX_REG_BANK_PHY_INTR_RPT2_REG:
        case RX_CHN_EST_REG_BANK_CHN_EST_COM_REG:
        case CALI_TEST_REG_BANK_0_FTM_CFG_REG:
        case RF_CTRL_REG_BANK_0_CFG_ONLINE_CALI_USE_RF_REG_EN_REG:
        case CALI_TEST_REG_BANK_0_FIFO_FORCE_EN_REG:
        case PHY_GLB_REG_BANK_PHY_BW_MODE_STA_REG:
            offset = phy_reg_offset[device_id];
            break;
        /* soc寄存器不区分主辅路,所以不区分主辅路偏移 */
        case MP12_W_SOFT_INT_SET_AON_REG:
            offset = soc_reg_offset[device_id];
            break;
        default:
            oam_error_log1(0, OAM_SF_ANY, "{regs_addr_get_offset::reg addr[%x] not used.}", reg_addr);
    }
#endif
    return offset;
}

static uint32_t mp12_regs_addr_transfer(hal_host_device_stru *hal_device, uint32_t reg_addr)
{
    uint64_t host_va = 0;
    uint32_t offset = mp12_regs_addr_get_offset(hal_device->device_id, reg_addr);
    if (oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, reg_addr + offset, &host_va) != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{hal_mac_regs_addr_transfer::regaddr[%x] devca2hostva fail.}", reg_addr);
        return OAL_FAIL;
    }
#ifndef _PRE_LINUX_TEST
    switch (reg_addr) {
        case MAC_GLB_REG_HOST_INTR_CLR_REG:
            hal_device->mac_regs.irq_clr_reg = (uintptr_t)host_va;
            break;
        case MAC_GLB_REG_HOST_INTR_MASK_REG:
            hal_device->mac_regs.irq_status_mask_reg = (uintptr_t)host_va;
            break;
        case MAC_GLB_REG_HOST_INTR_STATUS_REG:
            hal_device->mac_regs.irq_status_reg = (uintptr_t)host_va;
            break;
        case MAC_TMU_REG_VAP_TSFTIMER_RDVALL_STATUS_12_REG:
            hal_device->mac_regs.tsf_lo_reg = (uintptr_t)host_va;
            break;
        case MAC_COMMON_REG_HOST_COMMON_INTR_STATUS_REG:
            hal_device->mac_regs.irq_common_status_reg = (uintptr_t)host_va;
            break;
        case MAC_COMMON_REG_HOST_COMMON_INTR_MASK_REG:
            hal_device->mac_regs.irq_common_status_mask_reg = (uintptr_t)host_va;
            break;
        case MAC_COMMON_REG_HOST_COMMON_INTR_CLR_REG:
            hal_device->mac_regs.irq_common_clr_reg = (uintptr_t)host_va;
            break;
        case DFX_REG_BANK_CFG_INTR_CLR_REG:
            hal_device->phy_regs.irq_clr_reg = (uintptr_t)host_va;
            break;
        case DFX_REG_BANK_CFG_INTR_MASK_HOST_REG:
            hal_device->phy_regs.irq_status_mask_reg = (uintptr_t)host_va;
            break;
        case DFX_REG_BANK_PHY_INTR_RPT_REG:
            hal_device->phy_regs.irq_status_reg = (uintptr_t)host_va;
            break;
        case DFX_REG_BANK_CFG_INTR_CLR2_REG:
            hal_device->phy_regs.irq_clr_reg2 = (uintptr_t)host_va;
            break;
        case DFX_REG_BANK_CFG_INTR_MASK_HOST2_REG:
            hal_device->phy_regs.irq_status_mask_reg2 = (uintptr_t)host_va;
            break;
        case DFX_REG_BANK_PHY_INTR_RPT2_REG:
            hal_device->phy_regs.irq_status_reg2 = (uintptr_t)host_va;
            break;
        case MP12_W_SOFT_INT_SET_AON_REG:
            hal_device->soc_regs.w_soft_int_set_aon_reg = (uintptr_t)host_va;
            break;
        default:
            oam_warning_log1(0, OAM_SF_ANY, "{hal_mac_regs_addr_transfer::reg addr[%x] not used.}", reg_addr);
            return OAL_FAIL;
    }
#endif
    return OAL_SUCC;
}

uint32_t mp12_host_regs_addr_init(hal_host_device_stru *hal_device)
{
    uint32_t idx;
    uint32_t hal_regs[] = {
        MAC_GLB_REG_HOST_INTR_CLR_REG,
        MAC_GLB_REG_HOST_INTR_MASK_REG,
        MAC_GLB_REG_HOST_INTR_STATUS_REG,
        MAC_TMU_REG_VAP_TSFTIMER_RDVALL_STATUS_12_REG,
        MAC_COMMON_REG_HOST_COMMON_INTR_STATUS_REG,
        MAC_COMMON_REG_HOST_COMMON_INTR_MASK_REG,
        MAC_COMMON_REG_HOST_COMMON_INTR_CLR_REG,
        DFX_REG_BANK_CFG_INTR_CLR_REG,
        DFX_REG_BANK_CFG_INTR_MASK_HOST_REG,
        DFX_REG_BANK_PHY_INTR_RPT_REG,
        DFX_REG_BANK_CFG_INTR_CLR2_REG,
        DFX_REG_BANK_CFG_INTR_MASK_HOST2_REG,
        DFX_REG_BANK_PHY_INTR_RPT2_REG,
        MP12_W_SOFT_INT_SET_AON_REG,
    };
    uint8_t dev_num = mac_chip_get_hal_dev_num();
    if (hal_device->device_id >= dev_num) {
        oam_warning_log1(0, OAM_SF_ANY, "{mp12_host_regs_addr_init::device_id[%x]!}", hal_device->device_id);
        return OAL_FAIL;
    }
    /* 唤醒低功耗内存 */
    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return OAL_FAIL;
    }

    for (idx = 0; idx < sizeof(hal_regs) / sizeof(uint32_t); idx++) {
        if (mp12_regs_addr_transfer(hal_device, hal_regs[idx]) != OAL_SUCC) {
            hal_pm_try_wakeup_allow_sleep();
            oam_error_log1(0, OAM_SF_ANY, "{host_regs_addr_init::regs addr trans fail. %x}", hal_regs[idx]);
            return OAL_FAIL;
        }
    }
    hal_pm_try_wakeup_allow_sleep();
    oam_warning_log0(0, OAM_SF_ANY, "{host_regs_addr_init::regs addr trans succ.}");
    return OAL_SUCC;
}

static hal_mac_common_timer *g_mac_timer_table[MP12_MAC_MAX_COMMON_TIMER + 1] = { 0 };
/* 功能描述 : 初始化host通用定时器，填写转换地址 */
int32_t mp12_host_init_common_timer(hal_mac_common_timer *mac_timer)
{
    uint64_t host_va = 0;
    uint8_t timer_id = mac_timer->timer_id;
    uint32_t mask_dev_reg = MAC_GLB_REG_COMMON_TIMER_MASK_REG;
    uint32_t timer_ctrl = MAC_TMU_REG_COMMON_TIMER_CTRL_0_REG + MP12_MAC_COMMON_TIMER_OFFSET * timer_id;
    uint32_t timer_val = MAC_TMU_REG_COMMON_TIMER_VAL_0_REG + MP12_MAC_COMMON_TIMER_OFFSET * timer_id;

    if (timer_id > MP12_MAC_MAX_COMMON_TIMER) {
        oam_error_log1(0, OAM_SF_ANY, "{host_init_common_timer::timer id[%d] error.}", timer_id);
        return OAL_FAIL;
    }

    if (oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, mask_dev_reg, &host_va) != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{host_init_common_timer::regaddr[%x] devca2hostva fail.}", mask_dev_reg);
        return OAL_FAIL;
    }
    mac_timer->mask_reg_addr = (uintptr_t)host_va;

    if (oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, timer_ctrl, &host_va) != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{host_init_common_timer::regaddr[%x] devca2hostva fail.}", timer_ctrl);
        return OAL_FAIL;
    }
    mac_timer->timer_ctrl_addr = host_va;

    if (oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, timer_val, &host_va) != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{host_init_common_timer::regaddr[%x] devca2hostva fail.}", timer_val);
        return OAL_FAIL;
    }
    mac_timer->timer_val_addr = (uintptr_t)host_va;
    g_mac_timer_table[timer_id] = mac_timer;

    return OAL_SUCC;
}

hal_mac_common_timer *mp12_get_host_mac_common_timer_ptr(uint8_t id)
{
    return id <= MP12_MAC_MAX_COMMON_TIMER ? g_mac_timer_table[id] : NULL;
}
void mp12_host_set_hal_mac_common_timer(hal_mac_common_timer *mac_common_timer)
{
    uint8_t timer_id = mac_common_timer->timer_id;
    uint32_t timer_mask;
    uint32_t timer_ctrl;

    timer_mask = hal_host_readl(mac_common_timer->mask_reg_addr);
    timer_mask = timer_mask & (~BIT(timer_id)); /* 打开timer_id对应的mask */
    hal_host_writel(timer_mask, mac_common_timer->mask_reg_addr); /* 打开中断处理 */
    timer_ctrl = ((mac_common_timer->timer_en) | (mac_common_timer->timer_mode << 1) |
        (mac_common_timer->timer_unit << 2)); /* bit0: timer en bit1: timer_mode bit2~3: unit */
    hal_host_writel(timer_ctrl, mac_common_timer->timer_ctrl_addr);
    hal_host_writel(mac_common_timer->common_timer_val, mac_common_timer->timer_val_addr);
}
/*
 * 函 数 名  : bismp12st_set_mac_common_timer
 * 功能描述  : 配置通用MAC定时器
 * 1.日    期  : 2020年12月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void mp12_host_set_mac_common_timer(hal_mac_common_timer *mac_common_timer)
{
    uint8_t timer_id = mac_common_timer->timer_id;

    if (timer_id > MP12_MAC_MAX_COMMON_TIMER) {
        oam_error_log1(0, OAM_SF_ANY, "host_set_mac_common_timer :: timer[%d] id error.", timer_id);
        return;
    }
    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return;
    }
    if (mac_common_timer->timer_en == OAL_FALSE) {
        hal_host_writel(0, mac_common_timer->timer_ctrl_addr);
        hal_pm_try_wakeup_allow_sleep();
        return;
    }
    mp12_host_set_hal_mac_common_timer(mac_common_timer);
    hal_pm_try_wakeup_allow_sleep();
}

void mp12_host_set_mac_common_timer_no_pm_wakeup(hal_mac_common_timer *mac_common_timer)
{
    uint8_t timer_id = mac_common_timer->timer_id;

    if (timer_id > MP12_MAC_MAX_COMMON_TIMER) {
        oam_error_log1(0, OAM_SF_ANY, "host_set_mac_common_timer_no_pm_wakeup :: timer[%d] id error.", timer_id);
        return;
    }
    if (mac_common_timer->timer_en == OAL_FALSE) {
        hal_host_writel(0, mac_common_timer->timer_ctrl_addr);
        return;
    }
    mp12_host_set_hal_mac_common_timer(mac_common_timer);
}

uint32_t mp12_host_get_tsf_lo(hal_host_device_stru *hal_device, uint8_t hal_vap_id, uint32_t *tsf)
{
    if (oal_unlikely(hal_vap_id >= HAL_MAX_VAP_NUM_MP12 || !hal_device->mac_regs.tsf_lo_reg)) {
        return OAL_FAIL;
    }
    hal_pm_try_wakeup_forbid_sleep();
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep();
        return OAL_FAIL;
    }

    *tsf = hal_host_readl(hal_device->mac_regs.tsf_lo_reg + (hal_vap_id * 4)); // 4: 每个vap一个tsf寄存器（4字节)
    hal_pm_try_wakeup_allow_sleep();
    return OAL_SUCC;
}
