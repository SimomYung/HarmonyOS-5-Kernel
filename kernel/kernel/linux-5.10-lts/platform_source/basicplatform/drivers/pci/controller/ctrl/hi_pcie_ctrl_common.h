/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller common functions used by all controllers.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_COMMON_H
#define HI_PCIE_CTRL_COMMON_H

#include "hi_pcie_ctrl.h"
#include "../core/hi_pcie_common.h"
#include "hi_pcie_ctrl_regs.h"
#include "../hi_pcie_defines.h"

static inline uint32_t pcie_ctrl_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return readl(ctrl->pcie->ctrl_base + reg);
}

static inline void pcie_ctrl_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	writel(val, ctrl->pcie->ctrl_base + reg);
}

static inline void pcie_ctrl_core_gbl_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + CORE_REG);
}

static inline uint32_t pcie_ctrl_core_gbl_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + CORE_REG);
}

static inline void pcie_ctrl_ap_gbl_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + AP_GLOBAL_REG);
}

static inline uint32_t pcie_ctrl_ap_gbl_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + AP_GLOBAL_REG);
}

static inline void pcie_ctrl_ap_int_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + AP_INT_REG);
}

static inline uint32_t pcie_ctrl_ap_int_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + AP_INT_REG);
}

static inline void pcie_ctrl_mac_writel(struct hi_pcie_ctrl_port *port, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(port->ctrl, val, reg + MAC_REG + port->port->port_id * 0x1000);
}

static inline uint32_t pcie_ctrl_mac_readl(struct hi_pcie_ctrl_port *port, uint32_t reg)
{
	return pcie_ctrl_readl(port->ctrl, reg + MAC_REG + port->port->port_id * 0x1000);
}

static inline uint32_t pcie_ctrl_dl_readl(struct hi_pcie_ctrl_port *port, uint32_t reg)
{
	return pcie_ctrl_readl(port->ctrl, DL_REG + reg + port->port->port_id * 0x1000);
}

static inline void pcie_ctrl_dl_writel(struct hi_pcie_ctrl_port *port, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(port->ctrl, val, DL_REG + reg + port->port->port_id * 0x1000);
}

static inline uint32_t pcie_ctrl_tl_readl(struct hi_pcie_ctrl_port *port, uint32_t reg)
{
	return pcie_ctrl_readl(port->ctrl, TL_REG + reg + port->port->port_id * 0x1000);
}

static inline void pcie_ctrl_tl_writel(struct hi_pcie_ctrl_port *port, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(port->ctrl, val, TL_REG + reg + port->port->port_id * 0x1000);
}

static inline uint32_t pcie_ctrl_tl_core_read(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, TL_CORE_REG + reg);
}

static inline void pcie_ctrl_tl_core_write(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, TL_CORE_REG + reg);
}

static inline uint32_t pcie_ctrl_tl_core_pf_readl(struct hi_pcie_ctrl_port *port, uint32_t reg)
{
	return pcie_ctrl_readl(port->ctrl, TL_CORE_PF_REG + 0x200 * port->port->pf + reg);
}

static inline void pcie_ctrl_tl_core_pf_writel(struct hi_pcie_ctrl_port *port, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(port->ctrl, val, TL_CORE_PF_REG + 0x200 * port->port->pf + reg);
}

static inline uint32_t pcie_ctrl_cfg_readl(struct hi_pcie_ctrl_port *port, uint32_t reg)
{
	return pcie_ctrl_readl(port->ctrl, TL_RPFN_CFGSPACE_REG + 0x1000 * port->port->port_id + reg);
}

static inline void pcie_ctrl_cfg_writel(struct hi_pcie_ctrl_port *port, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(port->ctrl, val, TL_RPFN_CFGSPACE_REG + 0x1000 * port->port->port_id + reg);
}

static inline uint32_t pcie_ctrl_cfg_readw(struct hi_pcie_ctrl_port *port, uint32_t reg)
{
	uint32_t val;
	void __iomem * addr = port->port->pcie->ctrl_base + TL_RPFN_CFGSPACE_REG + 0x1000 * port->port->port_id + reg;
	pcie_core_read(addr, PCIE_SIZE_WORD, &val);
	return val;
}

static inline void pcie_ctrl_cfg_writew(struct hi_pcie_ctrl_port *port, uint32_t val, uint32_t reg)
{
	void __iomem * addr = port->port->pcie->ctrl_base + TL_RPFN_CFGSPACE_REG + 0x1000 * port->port->port_id + reg;
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_core_write(addr, PCIE_SIZE_WORD, val);
}

static inline uint32_t pcie_ctrl_cfg_readb(struct hi_pcie_ctrl_port *port, uint32_t reg)
{
	uint32_t val;
	void __iomem * addr = port->port->pcie->ctrl_base + TL_RPFN_CFGSPACE_REG + 0x1000 * port->port->port_id + reg;
	pcie_core_read(addr, PCIE_SIZE_BYTE, &val);
	return val;
}

static inline void pcie_ctrl_cfg_writeb(struct hi_pcie_ctrl_port *port, uint32_t val, uint32_t reg)
{
	void __iomem * addr = port->port->pcie->ctrl_base + TL_RPFN_CFGSPACE_REG + 0x1000 * port->port->port_id + reg;
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_core_write(addr, PCIE_SIZE_BYTE, val);
}

static inline uint32_t pcie_ctrl_tl_dfx_read(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, TL_CORE_DFX_REG + reg);
}

static inline void pcie_ctrl_tl_dfx_write(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, TL_CORE_DFX_REG + reg);
}

static inline void pcie_ctrl_ap_iobtx_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + AP_IOB_TX_REG);
}

static inline uint32_t pcie_ctrl_ap_iobtx_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + AP_IOB_TX_REG);
}

static inline void pcie_ctrl_ap_iobrx_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + AP_IOB_RX_REG);
}

static inline uint32_t pcie_ctrl_ap_iobrx_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + AP_IOB_RX_REG);
}

static inline void pcie_ctrl_ap_dma_reg_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + AP_DMA_REG);
}

static inline uint32_t pcie_ctrl_ap_dma_reg_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + AP_DMA_REG);
}

static inline void pcie_ctrl_top_reg_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + TOP_REG);
}

static inline uint32_t pcie_ctrl_top_reg_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + TOP_REG);
}

static inline uint32_t pcie_ctrl_ap_engine_readl(struct hi_pcie_ctrl *ctrl, uint32_t reg)
{
	return pcie_ctrl_readl(ctrl, reg + AP_ENGINE_REG);
}

static inline void pcie_ctrl_ap_engine_writel(struct hi_pcie_ctrl *ctrl, uint32_t val, uint32_t reg)
{
	hi_pcie_pr_d("%s, value %x, reg %x", __func__, val, reg);
	pcie_ctrl_writel(ctrl, val, reg + AP_ENGINE_REG);
}

bool pcie_ctrl_is_v500_ctrl(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_rx_standby_x8(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_rx_margin_icg_en(struct hi_pcie_ctrl_port *port);
void pcie_set_dl_fc_update_infer_idle_en(struct hi_pcie_ctrl_port *port);
int pcie_ctrl_set_linkmode(struct hi_pcie_ctrl *ctrl);

int pcie_ctrl_unrst_corephy(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_cfg_linkmode(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_mac_misc0_cfg(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_serders_unrst(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_init_serders(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_core_sft_unrst(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_port_unrst(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_apbaxi_unrst(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_apbaxi_reset(struct hi_pcie_ctrl *ctrl);
void pcie_open_ssc(struct hi_pcie_ctrl_port *port);
void pcie_auto_speed_enable(struct hi_pcie_ctrl_port *port);
void pcie_async_fifo_cfg(struct hi_pcie_ctrl_port *port);
void pcie_fix_core_clk(struct hi_pcie_ctrl_port *port);
void pcie_l1ss_core_freqdown(struct hi_pcie_ctrl_port *port);
void pcie_close_unused_lane(struct hi_pcie_ctrl_port *port);
void pcie_ap_core_clk_sync(struct hi_pcie_ctrl_port *port);
void pcie_cfg_max_link_width(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_mac_rx_idle(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_full_eq_cfg(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_rx_standby(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_idle_delay(struct hi_pcie_ctrl_port *port);
void pcie_cfg_enter_l1l2_timeout(struct hi_pcie_ctrl_port *port);
void pcie_max_tag_cfg(struct hi_pcie_ctrl_port *port);
void pcie_portpf_mapping_cfg(struct hi_pcie_ctrl_port *port);
void pcie_tlp_md_indicator(struct hi_pcie_ctrl_port *port);
void pcie_tl_rx_dfxfunc_en(struct hi_pcie_ctrl_port *port);
void pcie_port_pwrn_scale(struct hi_pcie_ctrl_port *port);
void pcie_set_speed_limit(struct hi_pcie_ctrl_port *port);
void pcie_max_payload_support(struct hi_pcie_ctrl_port *port);
void pcie_support_l2_aux(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_poison_tl(struct hi_pcie_ctrl_port *port);
void pcie_dma_cfg(struct hi_pcie_ctrl *ctrl);
void pcie_local_intr_map(struct hi_pcie_ctrl *ctrl);
void pcie_local_intr_port_cfg(struct hi_pcie_ctrl *ctrl, bool enable);
void pcie_ctrl_mask_all_int(struct hi_pcie_ctrl *ctrl, bool mask);
void pcie_init_tx_buf(struct hi_pcie_ctrl *ctrl);
void pcie_init_rx_buf(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_open_clk_gate(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_ena_port(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_chk_link_up(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_enable_port(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_disable_port(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_enable_port_ap(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_disable_port_ap(struct hi_pcie_ctrl_port *port);
int pcie_ctrl_check_port_linkup(struct hi_pcie_ctrl_port *port);
int pcie_ctrl_check_port_linkup(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_soft_reset_port(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_soft_unreset_port(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_soft_reset_port_logic(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_soft_unreset_port_logic(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_soft_reset_port_phy(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_soft_unreset_port_phy(struct hi_pcie_ctrl_port *port);
void pcie_ctrl_enable_port_int(struct hi_pcie_ctrl_port *port, bool enable);
void pcie_ctrl_mask_port_int(struct hi_pcie_ctrl_port *port, bool mask);
void pcie_ctrl_clear_port_int(struct hi_pcie_ctrl_port *port);
int pcie_ctrl_port_workmode(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_exit_shutdown(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_init_core(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_ap_init(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_ena_ldrst(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_reset(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_core_sft_reset(struct hi_pcie_ctrl *ctrl);
void hi_pcie_handle_int(struct hi_pcie_ctrl *ctrl);
void pcie_local_intr_msi_intx_cfg(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_disable_irq(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_enable_irq(struct hi_pcie_ctrl *ctrl);
#endif
