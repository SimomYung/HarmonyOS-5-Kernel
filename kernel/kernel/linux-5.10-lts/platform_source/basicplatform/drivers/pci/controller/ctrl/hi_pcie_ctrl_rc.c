/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller rc logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_ctrl_rc.h"

#include <linux/irqdomain.h>
#include <linux/delay.h>

#include "../hi_pcie_external_api.h"
#include "../hi_pcie_defines.h"
#include "../core/hi_pcie_common.h"
#include "../core/hi_pcie_msi.h"

#include "hi_pcie_ctrl_regs.h"
#include "hi_pcie_ctrl_common.h"
#include "hi_pcie_ctrl_dfx.h"
#include "hi_pcie_ctrl_dma.h"

#define PCIE_MSI_VECTOR_NUM 32

static void pcie_ctrl_mask_irq(struct hi_pcie_msi *msi, u32 irq)
{
	u32 bit = irq % PCIE_MSI_VECTOR_NUM;
	struct hi_pcie_ctrl *ctrl = msi->priv;
	if (atomic_read(&(ctrl->pcie->is_power_on)) == 0)
		return;

	msi->irq_mask |= BIT(bit);
	pcie_ctrl_ap_gbl_writel(msi->priv, msi->irq_mask, PCIE_MSI_MASK);
}

static void pcie_ctrl_unmask_irq(struct hi_pcie_msi *msi, u32 irq)
{
	u32 bit = irq % PCIE_MSI_VECTOR_NUM;
	struct hi_pcie_ctrl *ctrl = msi->priv;
	if (atomic_read(&(ctrl->pcie->is_power_on)) == 0)
		return;

	msi->irq_mask &= ~BIT(bit);
	pcie_ctrl_ap_gbl_writel(msi->priv, msi->irq_mask, PCIE_MSI_MASK);
}

static void pcie_ctrl_ack_irq(struct hi_pcie_msi *msi, u32 irq)
{
	u32 bit = irq % PCIE_MSI_VECTOR_NUM;
	struct hi_pcie_ctrl *ctrl = msi->priv;
	if (atomic_read(&(ctrl->pcie->is_power_on)) == 0)
		return;

	pcie_ctrl_ap_gbl_writel(msi->priv, BIT(bit), PCIE_MSI_STATUS);
}

static u32 pcie_ctrl_get_irq_status(struct hi_pcie_msi *msi, u32 group)
{
	struct hi_pcie_ctrl *ctrl = msi->priv;
	if (atomic_read(&(ctrl->pcie->is_power_on)) == 0)
		return 0xFFFFFFFF;

	return pcie_ctrl_ap_gbl_readl(msi->priv, PCIE_MSI_STATUS);
}

static int pcie_ctrl_init_msi(struct hi_pcie_ctrl *ctrl)
{
	int ret;
	struct hi_pcie_msi *msi = &ctrl->msi;
	struct fwnode_handle *fwnode;
	if (msi->priv != NULL)
		return 0;

	msi->dev = &ctrl->pcie->pdev->dev;
	msi->num_vectors = PCIE_MSI_VECTOR_NUM;
	msi->vectors_per_group = PCIE_MSI_VECTOR_NUM;
	msi->mask_irq = pcie_ctrl_mask_irq;
	msi->unmask_irq = pcie_ctrl_unmask_irq;
	msi->ack_irq = pcie_ctrl_ack_irq;
	msi->get_irq_status = pcie_ctrl_get_irq_status;
	msi->priv = ctrl;

	fwnode = of_node_to_fwnode(msi->dev->of_node);
	ret = hi_pcie_init_msi(&ctrl->msi, fwnode);
	if (ret != 0)
		return ret;

	return 0;
}

int pcie_ctrl_port_workmode(struct hi_pcie_ctrl *ctrl)
{
	u32 mask = 0;
	u32 val;
	struct hi_pcie_port *port;

	list_for_each_entry(port, &ctrl->pcie->ports, node)
		mask |= BIT(port->port_id);

	val = pcie_ctrl_core_gbl_readl(ctrl, GLB_PCIEC_MODE_SEL);
	if (ctrl->pcie->mode == PCIE_MODE_RC)
		val |= mask;
	else
		val &= ~mask;
	pcie_ctrl_core_gbl_writel(ctrl, val, GLB_PCIEC_MODE_SEL);

	return 0;
}

static void pcie_remove_acs_capability(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->mode == PCIE_MODE_RC || pcie_ctrl_is_v500_ctrl(port->ctrl))
		return;

	val = pcie_ctrl_tl_readl(port, TL_CFG_HCK_EN);
	val &= ~BIT(7);
	pcie_ctrl_tl_writel(port, val, TL_CFG_HCK_EN);

	if (port->ctrl->pcie_version == PCIE_VERSION3) {
		val = pcie_ctrl_cfg_readl(port, DEVICE_SERIAL_NUMBER_CAP_HEADER);
		val &= ~(0xFFF << 20);
		val |= (0x6d0 << 20);
		pcie_ctrl_cfg_writel(port, val, DEVICE_SERIAL_NUMBER_CAP_HEADER);
	} else {
		val = pcie_ctrl_cfg_readl(port, L1PM_CAP_HEADER);;
		val &= ~(0xFFF << 20);
		val |= (0x6d0 << 20);
		pcie_ctrl_cfg_writel(port, val, L1PM_CAP_HEADER);
	}
}

static void pcie_tl_tx_range_order(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	if (pcie_ctrl_is_v500_ctrl(port->ctrl))
		return;

	val = pcie_ctrl_tl_readl(port, TL_TX_DFX_CTRL);
	val &= ~BIT(17);
	pcie_ctrl_tl_writel(port, val, TL_TX_DFX_CTRL);
}

static void pcie_tl_tx_crd_without_link(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	val = pcie_ctrl_tl_readl(port, TX_CRD_CTRL);
	val |= BIT(0);
	pcie_ctrl_tl_writel(port, val, TX_CRD_CTRL);
}

static void pcie_pf_en_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	val = pcie_ctrl_tl_core_read(port->ctrl, TL_CFG_PF_EN);
	val |= BIT(port->port->pf);
	pcie_ctrl_tl_core_write(port->ctrl, val, TL_CFG_PF_EN);
}

static void pcie_ctrl_init_bar(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->mode == PCIE_MODE_RC)
		return;

	val = pcie_ctrl_tl_core_pf_readl(port, PFN_BARN_ENABLE);
	val |= (BIT(0) | BIT(1));
	pcie_ctrl_tl_core_pf_writel(port, val, PFN_BARN_ENABLE);

	val = pcie_ctrl_cfg_readl(port, PCIHDR_BAR0);
	val |= BIT(2);
	pcie_ctrl_cfg_writel(port, val, PCIHDR_BAR0);

	val = pcie_ctrl_tl_core_pf_readl(port, PFN_BAR0_MASK);
	pcie_ctrl_tl_core_pf_writel(port, val, PFN_BAR0_MASK);
}

static void pcie_ctrl_pwr_indicator(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	if (port->ctrl->pcie->mode == PCIE_MODE_EP || port->ctrl->pcie_version == PCIE_VERSION3)
		return;

	val = pcie_ctrl_cfg_readl(port, SLOT_CTRL_STATUS);
	val = val | BIT(8) | BIT(9) | BIT(6) | BIT(7);
	pcie_ctrl_cfg_writel(port, val, SLOT_CTRL_STATUS);
}

static void pcie_ctrl_disable_atomic(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->mode == PCIE_MODE_RC ||
		port->ctrl->pcie_version == PCIE_VERSION4)
		return;

	val = pcie_ctrl_cfg_readl(port, DEVICE_CAPABILITY2);
	val &= ~(BIT(7) | BIT(8) | BIT(9));
	pcie_ctrl_cfg_writel(port, val, DEVICE_CAPABILITY2);
}

static void pcie_fir_auto_speed_change(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie *pcie = port->ctrl->pcie;

	if ((pcie->chip_type == BOARD_FPGA) || (pcie->mode == PCIE_MODE_EP))
		return;

	/* rc port0 cfg 1 ep default */
	val = pcie_ctrl_mac_readl(port, MAC_ENTER_LPBK_DISABLE);
	val = val | BIT(8) | BIT(9);
	pcie_ctrl_mac_writel(port, val, MAC_ENTER_LPBK_DISABLE);
}

static void pcie_equ_phase23_timeout(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie *pcie = port->ctrl->pcie;

	if ((pcie->chip_type == BOARD_FPGA) || (pcie->mode == PCIE_MODE_EP))
		return;

	/* rc port0/2 phase2 cfg 24 phase3 cfg 32 ep default  */
	val = pcie_ctrl_mac_readl(port, MAC_REG_EQ_PHASE23_TIMEOUT_VAL);
	val &= 0xffff0000;
	val |= 32 | (24 << 8);
	pcie_ctrl_mac_writel(port, val, MAC_REG_EQ_PHASE23_TIMEOUT_VAL);
}

static void pcie_start_mode_keep_time(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	u32 mask = 0x7ff;

	/* rc port0/2 ep port0 cfg 0x1f4 */
	if (port->ctrl->pcie->mode == PCIE_MODE_RC) {
		val = pcie_ctrl_mac_readl(port, MAC_REG_WAIT_LINK_NUM_TIMER);
		val &= ~mask;
		val |= 0x1f4;
		pcie_ctrl_mac_writel(port, val, MAC_REG_WAIT_LINK_NUM_TIMER);
	} else {
		val = pcie_ctrl_mac_readl(port, MAC_LANE_NUM_WAIT_TIMER);
		val &= ~mask;
		val |= 0x1f4;
		pcie_ctrl_mac_writel(port, val, MAC_LANE_NUM_WAIT_TIMER);
	}
}

static void pcie_acc_mode_keep_time(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	/* rc port0/2 cfg 0x190 */
	if (port->ctrl->pcie->mode == PCIE_MODE_RC) {
		val = pcie_ctrl_mac_readl(port, MAC_WAIT_LANE_NUM_TIMER);
		val &= ~0x7ff;
		val |= 0x190;
		pcie_ctrl_mac_writel(port, val, MAC_WAIT_LANE_NUM_TIMER);
	}
}

static void pcie_preset_cfg(struct hi_pcie_ctrl_port *port)
{
	int i;
	u32 val;
	u32 reg;
	u32 offset;
	struct hi_pcie *pcie = port->ctrl->pcie;
	u32 gen3_phy_cap_regs[8] = {
		LINK_CONTROL3_REGISTER01,
		LINK_CONTROL3_REGISTER23,
		LINK_CONTROL3_REGISTER45,
		LINK_CONTROL3_REGISTER67,
		LINK_CONTROL3_REGISTER89,
		LINK_CONTROL3_REGISTER1011,
		LINK_CONTROL3_REGISTER1213,
		LINK_CONTROL3_REGISTER1415,
	};
	u32 gen4_phy_cap_regs[4] = {
		GEN4_PHY_CAP_REG20,
		GEN4_PHY_CAP_REG24,
		GEN4_PHY_CAP_REG28,
		GEN4_PHY_CAP_REG2C,
	};

	/* gen3 gen4 cfg preset */
	if ((pcie->chip_type == BOARD_FPGA) || (pcie->mode == PCIE_MODE_EP))
		return;

	for (i = 0; i < (int)port->port->num_lanes; i++) {
		reg = (port->port->port_id + (u32)i) / 2;
		offset = ((port->port->port_id + (u32)i) % 2) * 16;
		val = pcie_ctrl_cfg_readl(port, gen3_phy_cap_regs[reg]);
		val &= ~(0xf0f << offset);
		val |= (0x404 << offset);
		pcie_ctrl_cfg_writel(port, val, gen3_phy_cap_regs[reg]);

		reg = (port->port->port_id + (u32)i) / 4;
		offset = ((port->port->port_id + (u32)i) % 4) * 8;
		val = pcie_ctrl_cfg_readl(port, gen4_phy_cap_regs[reg]);
		val &= ~(0xff << offset);
		val |= (0x99 << offset);
		pcie_ctrl_cfg_writel(port, val, gen4_phy_cap_regs[reg]);
	}

	return;
}

static void pcie_set_mac_powerdown_value(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->port->pcie->chip_type != BOARD_FPGA)
		return;

	val = pcie_ctrl_mac_readl(port, MAC_POWERDOWN_VALUE);
	val &= ~(0xf << 20);
	val |= (0x2 << 20);
	pcie_ctrl_mac_writel(port, val, MAC_POWERDOWN_VALUE);
}

static void pcie_set_mac_tx_idle(struct hi_pcie_ctrl_port *port)
{
	return;
}

#define MAC_REG_16G_PHY_EQFB_SEL		0x03D0
#define MAC_REG_ADJ_HILINK_MODE_EN		0x00A0
#define REG_FIX_LP_16G_TX_COEFF			0x03C8
#define MAC_REG_EQ_FIX_LP_TX_PRESET		0x009C
#define MAC_REG_8G_PHY_EQ_FB_SEL		0x0024
#define MAC_REG_FIX_LP_8G_TX_COEFF		0x03C4
static void pcie_set_mac_preset(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	if (port->ctrl->tx_preset == 0xFFFFFFFF)
		return;

	val = pcie_ctrl_mac_readl(port, MAC_REG_8G_PHY_EQ_FB_SEL);
	val = val & (~(BIT(21) | BIT(20)));
	pcie_ctrl_mac_writel(port, val, MAC_REG_8G_PHY_EQ_FB_SEL);

	val = pcie_ctrl_mac_readl(port, MAC_REG_8G_PHY_EQ_FB_SEL);
	val = val & (~(BIT(16) | BIT(17)));
	val = val | BIT(16);
	pcie_ctrl_mac_writel(port, val, MAC_REG_8G_PHY_EQ_FB_SEL);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val = val & 0xFFFFFFC0;
	val = val | 0x9;
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val &= ~BIT(11);
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val &= ~BIT(19);
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);

	val = pcie_ctrl_mac_readl(port, MAC_REG_FIX_LP_8G_TX_COEFF);
	val |= BIT(31);
	pcie_ctrl_mac_writel(port, val, MAC_REG_FIX_LP_8G_TX_COEFF);
	
	/* 8G PRESET SET P4 */
	val = pcie_ctrl_mac_readl(port, MAC_REG_EQ_FIX_LP_TX_PRESET);
	val = val & 0xFFFFFFF0;
	val = val | port->ctrl->tx_preset;
	pcie_ctrl_mac_writel(port, val, MAC_REG_EQ_FIX_LP_TX_PRESET);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val = val & (~BIT(21));
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);

	if (port->ctrl->pcie_version == PCIE_VERSION3)
		return;

	val = pcie_ctrl_mac_readl(port, MAC_REG_16G_PHY_EQFB_SEL);
	val = val & (~(BIT(0) | BIT(1)));
	pcie_ctrl_mac_writel(port, val, MAC_REG_16G_PHY_EQFB_SEL);

	val = pcie_ctrl_mac_readl(port, MAC_REG_16G_PHY_EQFB_SEL);
	val = val & (~(BIT(16) | BIT(17)));
	val = val | BIT(16);
	pcie_ctrl_mac_writel(port, val, MAC_REG_16G_PHY_EQFB_SEL);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val = val & 0xC0FFFFFF;
	val = val | 0x9000000;
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val &= ~BIT(18);
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val &= ~BIT(20);
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);

	val = pcie_ctrl_mac_readl(port, REG_FIX_LP_16G_TX_COEFF);
	val |= BIT(31);
	pcie_ctrl_mac_writel(port, val, REG_FIX_LP_16G_TX_COEFF);

	/* 16G PRESET SET P4 */
	val = pcie_ctrl_mac_readl(port, MAC_REG_EQ_FIX_LP_TX_PRESET);
	val = val & 0xFFFFFF0F;
	val = val | (port->ctrl->tx_preset << 4);
	pcie_ctrl_mac_writel(port, val, MAC_REG_EQ_FIX_LP_TX_PRESET);

	val = pcie_ctrl_mac_readl(port, MAC_REG_ADJ_HILINK_MODE_EN);
	val = val & (~BIT(22));
	pcie_ctrl_mac_writel(port, val, MAC_REG_ADJ_HILINK_MODE_EN);
}

static void pcie_add_recovery_lock_time(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	if (port->ctrl->recv_lock_time == 0)
		return;

	val = pcie_ctrl_mac_readl(port, MAC_CFG_RCV_HOLD_EN);
	val = val | BIT(0);
	pcie_ctrl_mac_writel(port, val, MAC_CFG_RCV_HOLD_EN);

	// delay 200us
	val = pcie_ctrl_mac_readl(port, MAC_CFG_RCV_CFG_HOLD_TIME);
	val = val & (~(0xFFFF));
	val = val | port->ctrl->recv_lock_time;
	pcie_ctrl_mac_writel(port, val, MAC_CFG_RCV_CFG_HOLD_TIME);

	val = pcie_ctrl_cfg_readl(port, L1PM_CONTROL1);
	val = val & (~(0xFF00));
	val |= (port->ctrl->recv_lock_time) << 8;
	pcie_ctrl_cfg_writel(port, val, L1PM_CONTROL1);
}

static void pcie_set_ltssm_tracer_mode(struct hi_pcie_ctrl_port *port)
{
	u32 addr;
	addr = pcie_ctrl_mac_readl(port, MAC_REG_LTSSM_TRACER_INPUT);
	addr |= MAC_LTSSM_TRACER_CAP_MODE;
	pcie_ctrl_mac_writel(port, addr, MAC_REG_LTSSM_TRACER_INPUT);
}

static int pcie_ctrl_init_mac(struct hi_pcie_ctrl_port *port)
{
	int i;
	static struct {
		char *desc;
		void (*action)(struct hi_pcie_ctrl_port *);
	} init_mac_actions[] = {
		{"fir_auto_speed_change", pcie_fir_auto_speed_change},
		{"open_ssc", pcie_open_ssc},
		{"auto_speed_enable", pcie_auto_speed_enable},
		{"equ_phase23_timeout", pcie_equ_phase23_timeout},
		{"start_mode_keep_time", pcie_start_mode_keep_time},
		{"acc_mode_keep_time", pcie_acc_mode_keep_time},
		{"async_fifo_cfg", pcie_async_fifo_cfg},
		{"fix_core_clk", pcie_fix_core_clk},
		{"l1ss_core_freqdown", pcie_l1ss_core_freqdown},
		{"close_unused_lane", pcie_close_unused_lane},
		{"ap_core_clk_sync", pcie_ap_core_clk_sync},
		{"cfg_max_link_width", pcie_cfg_max_link_width},
		{"preset_cfg", pcie_preset_cfg},
		{"set preset", pcie_set_mac_preset},
		{"ctrl_mac_rx_idle", pcie_ctrl_mac_rx_idle},
		{"ctrl_full_equalization_test", pcie_ctrl_full_eq_cfg},
		{"ctrl_rx_standby", pcie_ctrl_rx_standby},
		{"ctrl_idle_delay", pcie_ctrl_idle_delay},
		{"cfg_enter_l1l2_timeout", pcie_cfg_enter_l1l2_timeout},
		{"set_mac_powerdown_value", pcie_set_mac_powerdown_value},
		{"ctrl_reg_rcv_tx_idle", pcie_set_mac_tx_idle},
		{"dl_fc_update_infer_idle", pcie_set_dl_fc_update_infer_idle_en},
		{"recovery lock time", pcie_add_recovery_lock_time},
		{"set_ltssm_tracer_mode", pcie_set_ltssm_tracer_mode},
	};

	hi_pcie_pr_i("initing %u mac", port->port->port_id);
	for (i = 0; i < ARRAY_SIZE(init_mac_actions); i++) {
		hi_pcie_pr_d("%s", init_mac_actions[i].desc);
		init_mac_actions[i].action(port);
	}

	return 0;
}

static void pcie_dlinit_fc_dllp(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->mode == PCIE_MODE_EP)
		return;

	val = pcie_ctrl_dl_readl(port, INIT_FC_SEND_EN);
	val &= ~BIT(0);
	pcie_ctrl_dl_writel(port, val, INIT_FC_SEND_EN);
}

static int pcie_ctrl_init_dl(struct hi_pcie_ctrl_port *port)
{
	pcie_dlinit_fc_dllp(port);

	return 0;
}

static void pcie_capability_enable(struct hi_pcie_ctrl_port *port)
{
	u32 tl_cssel;
	u32 tl_pf_cssel;
	struct hi_pcie_ctrl *ctrl = port->ctrl;

	if (ctrl->pcie->mode == PCIE_MODE_EP) {
		if (ctrl->pcie_version == PCIE_VERSION3)
			tl_cssel = 0x82194000;
		else
			tl_cssel = 0x8019C000;
	} else {
		tl_cssel = 0x500D4000;
		if (ctrl->pcie_version == PCIE_VERSION3)
			tl_pf_cssel = 0x10040000;
		else
			tl_pf_cssel = 0x10060000;
		pcie_ctrl_tl_core_pf_writel(port, tl_pf_cssel, TL_PF_VF_CFGSPACE_SEL);
	}

	pcie_ctrl_tl_core_pf_writel(port, tl_cssel, TL_CFGSPACE_SEL);
}

static void pcie_set_crs_sw_visibility(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->mode == PCIE_MODE_EP)
		return;

	val = pcie_ctrl_cfg_readl(port, ROOT_CTRL_STATUS);
	val = val | BIT(16);
	pcie_ctrl_cfg_writel(port, val, ROOT_CTRL_STATUS);
}

static void pcie_ctrl_rcb_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->mode == PCIE_MODE_EP)
		return;

	val = pcie_ctrl_cfg_readl(port, LINK_CTRL_STATUS);
	val = val | BIT(3);
	pcie_ctrl_cfg_writel(port, val, LINK_CTRL_STATUS);
}

static void pcie_cvtest_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;

	if (ctrl->pcie->mode == PCIE_MODE_EP) {
		val = pcie_ctrl_cfg_readl(port, UNCR_ERR_MASK);
		val &= ~BIT(26);
		pcie_ctrl_cfg_writel(port, val, UNCR_ERR_MASK);
	}

	if (ctrl->pcie->mode == PCIE_MODE_RC && ctrl->pcie_version == PCIE_VERSION3)
		return;

	val = pcie_ctrl_cfg_readl(port, L1PM_CAP_HEADER);
	val = val & 0xFFF0FFFF;
	val = val | BIT(16);
	pcie_ctrl_cfg_writel(port, val, L1PM_CAP_HEADER);
}

static void pcie_ctrl_ltr_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;
 
	val = pcie_ctrl_cfg_readl(port, DEVICE_CAPABILITY2);
	val |= BIT(11);
	pcie_ctrl_cfg_writel(port, val, DEVICE_CAPABILITY2);
}

static void pcie_ctrl_init_lowpower_cap(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->port->disable_l0s) {
		val = pcie_ctrl_cfg_readl(port, LINK_CAPBILITY);
		val &= (~(u32)BIT(10));
		pcie_ctrl_cfg_writel(port, val, LINK_CAPBILITY);
	}
}

static void pcie_tl_rx_ats_disable(struct hi_pcie_ctrl_port *port)
{
	if (!pcie_ctrl_is_v500_ctrl(port->ctrl))
		return;
	pcie_ctrl_tl_core_pf_writel(port, 0x0, PFN_TLRX_ERR_FILT);
}

static int pcie_ctrl_init_tl(struct hi_pcie_ctrl_port *port)
{
	int i;
	static struct {
		char *desc;
		void (*action)(struct hi_pcie_ctrl_port *);
	} init_tl_actions[] = {
		{"max_tag_cfg", pcie_max_tag_cfg},
		{"portpf_mapping_cfg", pcie_portpf_mapping_cfg},
		{"pf_en_cfg", pcie_pf_en_cfg},
		{"tlp_md_indicator", pcie_tlp_md_indicator},
		{"capability_enable", pcie_capability_enable},
		{"remove_acs_capability", pcie_remove_acs_capability},
		{"tl_tx_order_preserving", pcie_tl_tx_range_order },
		{"tl_tx_crd_without_link", pcie_tl_tx_crd_without_link },
		{"tl_rx_dfxfunc_en", pcie_tl_rx_dfxfunc_en},
		{"port_pwrn_scale", pcie_port_pwrn_scale},
		{"max_payload_support", pcie_max_payload_support},
		{"init_bar", pcie_ctrl_init_bar},
		{"tl_pwr_indicator_ctrl", pcie_ctrl_pwr_indicator},
		{"set_crs_sw_visibility", pcie_set_crs_sw_visibility},
		{"support_l2_aux", pcie_support_l2_aux},
		{"set_speed_limit", pcie_set_speed_limit},
		{"ctrl_rcb_cfg", pcie_ctrl_rcb_cfg},
		{"cvtest_cfg", pcie_cvtest_cfg},
		{"disable_atomic", pcie_ctrl_disable_atomic},
		{"disable_tl_rx_ats", pcie_tl_rx_ats_disable},
		{"support ltr cfg", pcie_ctrl_ltr_cfg},
		{"init low power cap", pcie_ctrl_init_lowpower_cap},
	};

	hi_pcie_pr_i("initing %u tl", port->port->port_id);
	for (i = 0; i < ARRAY_SIZE(init_tl_actions); i++) {
		hi_pcie_pr_d("%s", init_tl_actions[i].desc);
		init_tl_actions[i].action(port);
	}

	return 0;
}

static void pcie_ecam_init(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	u64 base = ctrl->pcie->ecam.base_phy;

	if (ctrl->pcie->mode == PCIE_MODE_EP)
		return;
	/* base addr */
	pcie_ctrl_ap_iobtx_writel(ctrl, lower_32_bits(base), IOB_TX_ECAM_ADDR_LOW);
	pcie_ctrl_ap_iobtx_writel(ctrl, upper_32_bits(base), IOB_TX_ECAM_ADDR_HIGH);

	/* bus_num start from 0 */
	val = (u32)ctrl->pcie->ecam.busr.start;

	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_ECAM_CTRL0);

	/* tx ecam ctrl set */
	val = pcie_ctrl_ap_iobtx_readl(ctrl, IOB_TX_ECAM_CTRL1);
	val = val | BIT(4) | BIT(1);
	val = val & 0x7FFE0FF2;
	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_ECAM_CTRL1);
}

static void pcie_local_intr_timeout_cfg(struct hi_pcie_ctrl *ctrl)
{
	u32 val;

	if (ctrl->pcie_version == PCIE_VERSION3)
		return;

	/* link actions when the request timeout happened,set bit0/bit2 to 0 */
	val = pcie_ctrl_ap_iobtx_readl(ctrl, IOB_TX_REQ_TOUT_NOTIFY_MSK);
	val &= 0xFFFFFFFA;
	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_REQ_TOUT_NOTIFY_MSK);

	/* timeout timer threshold as 50'd */
	val = pcie_ctrl_ap_iobtx_readl(ctrl, IOB_TX_REQ_TIMER_CFG);
	val &= 0xffffff00;
	val |= 0x32;
	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_REQ_TIMER_CFG);

	/* timeout timer threshold as 50'd */
	val = pcie_ctrl_tl_core_read(ctrl, TL_TX_REQ_TIMER_CTRL);
	val &= 0xffffff00;
	val |= 0x32;
	pcie_ctrl_tl_core_write(ctrl, val, TL_TX_REQ_TIMER_CTRL);
}

void pcie_setup_tx_atu(struct hi_pcie_ctrl *ctrl,
	u32 idx, u64 src, u64 dst, u32 size)
{
	u32 val;

	val = pcie_ctrl_ap_iobtx_readl(ctrl, tx_atu_ctrl0(idx));
	if (ctrl->pcie->mode == PCIE_MODE_RC)
		val &= ~BIT(1);
	else
		val |= BIT(1);
	pcie_ctrl_ap_iobtx_writel(ctrl, val, tx_atu_ctrl0(idx));

	/* atu type */
	val = pcie_ctrl_ap_iobtx_readl(ctrl, tx_atu_ctrl0(idx));
	val = val & 0xFFFFFFFB;
	pcie_ctrl_ap_iobtx_writel(ctrl, val, tx_atu_ctrl0(idx));

	/* size */
	pcie_ctrl_ap_iobtx_writel(ctrl, lower_32_bits(size), tx_atu_size_low(idx));
	pcie_ctrl_ap_iobtx_writel(ctrl, upper_32_bits(size), tx_atu_size_high(idx));

	/* src */
	pcie_ctrl_ap_iobtx_writel(ctrl, lower_32_bits(src), tx_atu_src_low(idx));
	pcie_ctrl_ap_iobtx_writel(ctrl, upper_32_bits(src), tx_atu_src_high(idx));

	/* dst */
	pcie_ctrl_ap_iobtx_writel(ctrl, lower_32_bits(dst), tx_atu_dst_low(idx));
	pcie_ctrl_ap_iobtx_writel(ctrl, upper_32_bits(dst), tx_atu_dst_high(idx));

	/* tx_atu enable */
	val = pcie_ctrl_ap_iobtx_readl(ctrl, tx_atu_ctrl0(idx));
	val = val | BIT(0);
	pcie_ctrl_ap_iobtx_writel(ctrl, val, tx_atu_ctrl0(idx));
}

static void pcie_setup_rx_atu(struct hi_pcie_ctrl *ctrl,
	u32 idx, u64 src, u64 dst, u32 size)
{
	u32 val;

	/* size */
	pcie_ctrl_ap_iobrx_writel(ctrl, lower_32_bits(size), rx_atu_size_low(idx));
	pcie_ctrl_ap_iobrx_writel(ctrl, upper_32_bits(size), rx_atu_size_high(idx));

	/* src */
	pcie_ctrl_ap_iobrx_writel(ctrl, lower_32_bits(src), rx_atu_src_low(idx));
	pcie_ctrl_ap_iobrx_writel(ctrl, upper_32_bits(src), rx_atu_src_high(idx));

	/* dst */
	pcie_ctrl_ap_iobrx_writel(ctrl, lower_32_bits(dst), rx_atu_dst_low(idx));
	pcie_ctrl_ap_iobrx_writel(ctrl, upper_32_bits(dst), rx_atu_dst_high(idx));

	val = pcie_ctrl_ap_iobrx_readl(ctrl, rx_atu_ctrl1(idx));
	val &= ~0xff;
	pcie_ctrl_ap_iobrx_writel(ctrl, val, rx_atu_ctrl1(idx));

	/* rx_atu enable */
	val = pcie_ctrl_ap_iobrx_readl(ctrl, rx_atu_ctrl0(idx));
	val &= ~(0xf << 4); // cache
	val &= ~BIT(1); // func mode
	val &= ~BIT(8); // snoop
	val |= BIT(0); // enable
	pcie_ctrl_ap_iobrx_writel(ctrl, val, rx_atu_ctrl0(idx));
}

static void pcie_mem_iatu_cfg(struct hi_pcie_ctrl *ctrl)
{
	u32 idx = 0;
	struct resource_entry *entry;
	void* addr;

	if (ctrl->pcie->mode == PCIE_MODE_RC) {
		resource_list_for_each_entry(entry, &ctrl->bridge->windows) {
			if (resource_type(entry->res) != IORESOURCE_MEM)
				continue;
			pcie_setup_tx_atu(ctrl, idx, entry->res->start,
				entry->res->start - entry->offset,
				resource_size(entry->res));
			idx++;
		}
	} else {
		if (ctrl->re_com == NULL) {
			addr = dma_alloc_coherent(ctrl->pcie->dev, EP_BAR0_SIZE, &ctrl->dma_addr, GFP_KERNEL);
			if (addr == NULL) {
				hi_pcie_pr_e("dma addr alloc failed");
				return;
			}
			ctrl->re_com = addr;
			ctrl->re_com_phy = ctrl->dma_addr;
			ctrl->rc_dma_addr = (union hi_pcie_re_com *)((u8 *)addr + EP_BAR0_OFFSET);
		}
		pcie_setup_rx_atu(ctrl, 0, 0, ctrl->dma_addr, EP_BAR0_SIZE);
	}
}

static void pcie_receive_aer_cfg(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	u32 val_bits;
	struct hi_pcie_port *port;

	if (ctrl->pcie->mode == PCIE_MODE_EP)
		return;

	/* aer interrupt map msi spi0/spi1 [5:0][31:28] */
	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_ERR_MAPPING);
	val = val | 0x29;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_ERR_MAPPING);

	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_ERR_MAPPING);
	val = val & 0x0FFFFFE9;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_ERR_MAPPING);

	/* port enable bit cfg */
	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_CE_ENA);
	val = val | 0xFFFF;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_CE_ENA);

	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_UNF_ENA);
	val = val | 0xFFFF;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_UNF_ENA);

	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_UF_ENA);
	val = val | 0xFFFF;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_UF_ENA);

	/* port mask bit cfg */
	if (ctrl->pcie->port_num == 2)
		val_bits = 0xFFFFFFFA;
	else
		val_bits = 0xFFFFFFFE;

	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_CE_MASK);
	val = val & val_bits;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_CE_MASK);

	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_UNF_MASK);
	val = val & val_bits;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_UNF_MASK);

	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_UF_MASK);
	val = val & val_bits;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_UF_MASK);

	/* override cfg */
	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		val = pcie_ctrl_tl_readl(port->priv, SYS_ERR_OVERRIDE_CTRL);
		val = val | BIT(3);
		pcie_ctrl_tl_writel(port->priv, val, SYS_ERR_OVERRIDE_CTRL);

		val = pcie_ctrl_tl_readl(port->priv, SYS_ERR_OVERRIDE_CTRL);
		val = val | BIT(0) | BIT(1) | BIT(2);
		pcie_ctrl_tl_writel(port->priv, val, SYS_ERR_OVERRIDE_CTRL);
	}
}

static void pcie_local_intr_ecc_cfg(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->pcie_version == PCIE_VERSION4)
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x100, IOB_TX_INT_SEVERITY0);

	pcie_ctrl_ap_iobtx_writel(ctrl, 0x2A1FFFF, IOB_TX_INT_SEVERITY3);
	pcie_ctrl_ap_dma_reg_writel(ctrl, 0x2AA, DMA_CM_RAS_LEVEL);
}

static void pcie_local_intr_cfg(struct hi_pcie_ctrl *ctrl)
{
	pcie_local_intr_map(ctrl);
	pcie_local_intr_ecc_cfg(ctrl);
	pcie_local_intr_port_cfg(ctrl, true);
	pcie_local_intr_timeout_cfg(ctrl);
}

static void pcie_msi_cfg(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	u64 msi_addr;

	if (ctrl->pcie->mode == PCIE_MODE_EP)
		return;

	if (ctrl->int_type == INT_TYPE_SPI) {
		pcie_ctrl_init_msi(ctrl);
		msi_addr = ctrl->msi.msi_data;
	} else if (ctrl->int_type == INT_TYPE_ITS) {
		msi_addr = ctrl->its_addr;
	} else {
		return;
	}

	pcie_ctrl_tl_core_write(ctrl, lower_32_bits(msi_addr), TL_RX_MSI_ADDR_WINDOW_L);
	pcie_ctrl_tl_core_write(ctrl, upper_32_bits(msi_addr), TL_RX_MSI_ADDR_WINDOW_H);

	pcie_ctrl_ap_iobrx_writel(ctrl, lower_32_bits(msi_addr), 0x1048);

	pcie_ctrl_ap_iobrx_writel(ctrl, upper_32_bits(msi_addr), 0x1044);

	val = pcie_ctrl_ap_iobrx_readl(ctrl, 0x1040);
	val |= (BIT(0) | BIT(8));
	if (ctrl->int_type == INT_TYPE_SPI)
		val |= BIT(1);
	else
		val &= ~BIT(1);
	pcie_ctrl_ap_iobrx_writel(ctrl, val, 0x1040);
}

static void pcie_set_pre_alloc_buf(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	if (ctrl->pcie->mode == PCIE_MODE_RC || ctrl->pcie->chip_type != BOARD_FPGA)
		return;
	val = pcie_ctrl_ap_int_readl(ctrl, ALLOCATE_BUF_CTRL);
	val &= ~0x7;
	val |= 0x1;
	pcie_ctrl_ap_int_writel(ctrl, val, ALLOCATE_BUF_CTRL);
}

int pcie_ctrl_ap_init(struct hi_pcie_ctrl *ctrl)
{
	int i;
	static struct {
		char *desc;
		void (*action)(struct hi_pcie_ctrl *);
	} init_ap_actions[] = {
		{"ecam_init", pcie_ecam_init},
		{"mem_iatu_cfg", pcie_mem_iatu_cfg},
		{"receive_aer_cfg", pcie_receive_aer_cfg},
		{"local_intr_cfg", pcie_local_intr_cfg},
		{"msi_cfg", pcie_msi_cfg},
		{"dma_cfg", pcie_dma_cfg},
		{"set pre-alloc buf", pcie_set_pre_alloc_buf},
		{"init_tx_buf", pcie_init_tx_buf},
		{"init_rx_buf", pcie_init_rx_buf},
	};

	hi_pcie_pr_i("initing %u ap", ctrl->pcie->ctrl_id);
	for (i = 0; i < ARRAY_SIZE(init_ap_actions); i++) {
		hi_pcie_pr_d("%s", init_ap_actions[i].desc);
		init_ap_actions[i].action(ctrl);
	}

	return 0;
}

int pcie_ctrl_ena_ldrst(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	u32 clr = BIT(0);
	if (ctrl->pcie->port_num == 2)
		clr |= BIT(2);

	val = pcie_ctrl_core_gbl_readl(ctrl, PCIE_LINK_DOWN_CLR_PORT_EN);
	if (ctrl->pcie->mode == PCIE_MODE_RC)
		val &= ~clr;
	else
		val |= clr;
	pcie_ctrl_core_gbl_writel(ctrl, val, PCIE_LINK_DOWN_CLR_PORT_EN);

	if (ctrl->pcie->mode == PCIE_MODE_RC) {
		val = pcie_ctrl_core_gbl_readl(ctrl, PCIE_LINK_DOWN_RST_EN);
		val |= clr;
		pcie_ctrl_core_gbl_writel(ctrl, val, PCIE_LINK_DOWN_RST_EN);
	}

	return 0;
}

int pcie_ctrl_exit_shutdown(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->pcie_version == PCIE_VERSION4)
		return 0;

	pcie_ctrl_core_gbl_writel(ctrl, 0x5858, 0x424);

	pcie_ctrl_core_gbl_writel(ctrl, ctrl->mem_shutdown, 0x428);
	
	pcie_ctrl_core_gbl_writel(ctrl, 0x1, 0x42C);

	return 0;
}

int pcie_ctrl_init_core(struct hi_pcie_ctrl *ctrl)
{
	int ret;
	struct hi_pcie_port *port;

	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		if (port->disable)
			continue;
		ret = pcie_ctrl_init_mac(port->priv);
		if (ret != 0)
			continue;
		ret = pcie_ctrl_init_dl(port->priv);
		if (ret != 0)
			continue;
		ret = pcie_ctrl_init_tl(port->priv);
		if (ret != 0)
			continue;
	}

	return 0;
}

static void pcie_ctrl_disable_msi(struct hi_pcie_ctrl_port *port)
{
	pcie_ctrl_tl_writel(port, 0xFFFFFFFF, SYS_ERR_OVERRIDE_CTRL);
}

static int pcie_ctrl_wait_port_idle(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	u32 timeout = 100;

	while (timeout > 0) {
		val = pcie_ctrl_tl_readl(port, TL_PORT_IDLE_CTRL_STATUS);
		if (val | PCIE_PORT_IDLE_ST)
			return 0;
		udelay(1);
		timeout--;
	}

	return -1;
}

static void pcie_ctrl_reset_ap_captured_tl(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_ap_gbl_readl(port->ctrl, AP_CAP_TL_REG_RST);
	val |= BIT(port->port->port_id);
	pcie_ctrl_ap_gbl_writel(port->ctrl, val, AP_CAP_TL_REG_RST);
}

static void pcie_ctrl_reset_ap_captured_cfgspace(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_ap_gbl_readl(port->ctrl, AP_CFGSPACE_RST);
	val |= BIT(port->port->port_id);
	pcie_ctrl_ap_gbl_writel(port->ctrl, val, AP_CFGSPACE_RST);
}

static void pcie_ctrl_unreset_ap_captured_tl(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_ap_gbl_readl(port->ctrl, AP_CAP_TL_REG_RST);
	val &= ~BIT(port->port->port_id);
	pcie_ctrl_ap_gbl_writel(port->ctrl, val, AP_CAP_TL_REG_RST);
}

static void pcie_ctrl_unreset_ap_captured_cfgspace(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_ap_gbl_readl(port->ctrl, AP_CFGSPACE_RST);
	val &= ~BIT(port->port->port_id);
	pcie_ctrl_ap_gbl_writel(port->ctrl, val, AP_CFGSPACE_RST);
}

void pcie_ctrl_enable_port(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_core_gbl_readl(port->ctrl, PORT_EN);
	val |= BIT(port->port->port_id);
	pcie_ctrl_core_gbl_writel(port->ctrl, val, PORT_EN);
}

void pcie_ctrl_enable_port_ap(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_ap_gbl_readl(port->ctrl, AP_PORT_EN);
	val |= BIT(port->port->port_id);
	pcie_ctrl_ap_gbl_writel(port->ctrl, val, AP_PORT_EN);
}

void pcie_ctrl_disable_port(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_core_gbl_readl(port->ctrl, PORT_EN);
	val &= ~BIT(port->port->port_id);
	pcie_ctrl_core_gbl_writel(port->ctrl, val, PORT_EN);
}

void pcie_ctrl_disable_port_ap(struct hi_pcie_ctrl_port *port)
{
	u32 val = pcie_ctrl_ap_gbl_readl(port->ctrl, AP_PORT_EN);
	val &= ~BIT(port->port->port_id);
	pcie_ctrl_ap_gbl_writel(port->ctrl, val, AP_PORT_EN);
}

void pcie_ctrl_setup_outbound(struct hi_pcie *pcie, u32 index, u32 base, u32 target, u32 size)
{
	pcie_setup_tx_atu(pcie->ctrl, index, base, target, size);
}

void pcie_ctrl_setup_inbound(struct hi_pcie *pcie, u32 index, u32 base, u32 target, u32 size)
{
	pcie_setup_rx_atu(pcie->ctrl, index, base, target, size);
}

int pcie_ctrl_reset_port(struct hi_pcie_port *port)
{
	int ret;
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;

	pcie_core_remove_device(port);

	pcie_ctrl_enable_port_int(ctrl_port, false);
	pcie_ctrl_mask_port_int(ctrl_port, true);
	pcie_ctrl_clear_port_int(ctrl_port);
	pcie_ctrl_disable_msi(ctrl_port);
	pcie_ctrl_disable_port(ctrl_port);

	ret = pcie_ctrl_wait_port_idle(ctrl_port);
	if (ret != 0) {
		hi_pcie_pr_e("wait port idle failed");
		return -1;
	}

	pcie_ctrl_disable_port_ap(ctrl_port);
	pcie_ctrl_soft_reset_port(ctrl_port);
	pcie_ctrl_soft_reset_port_phy(ctrl_port);
	pcie_ctrl_reset_ap_captured_tl(ctrl_port);
	pcie_ctrl_reset_ap_captured_cfgspace(ctrl_port);

	atomic_set(&(port->is_link_up), 0);
	return 0;
}

int pcie_ctrl_unreset_port(struct hi_pcie_port *port)
{
	int ret;
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;

	pcie_ctrl_soft_unreset_port(ctrl_port);
	pcie_ctrl_soft_unreset_port_phy(ctrl_port);
	pcie_ctrl_unreset_ap_captured_cfgspace(ctrl_port);
	pcie_ctrl_unreset_ap_captured_tl(ctrl_port);
	pcie_ctrl_init_mac(ctrl_port);
	pcie_ctrl_init_dl(ctrl_port);
	pcie_ctrl_init_tl(ctrl_port);
	pcie_ctrl_mask_port_int(ctrl_port, false);
	pcie_ctrl_enable_port_int(ctrl_port, true);
	pcie_ctrl_enable_port(ctrl_port);
	pcie_ctrl_enable_port_ap(ctrl_port);

	ret = pcie_ctrl_check_port_linkup(ctrl_port);
	if (ret != 0)
		return ret;

	pcie_core_rescan_device(port);

	return 0;
}

static bool pcie_ctrl_is_port_linkup(struct hi_pcie_ctrl_port *port)
{
	u32 flag = BIT(18);
	int count = 0;
	int timeout = port->ctrl->pcie->chip_type == BOARD_EMU ? 20 : 300;
	u32 val;

	while(count <= timeout) {
		val = pcie_ctrl_dl_readl(port, DL_INT_STATUS);
		if ((val & flag) == flag)
			break;
		mdelay(1);
		count++;
	};

	pcie_ctrl_show_link_state(port->port);

	if (count > timeout) {
		atomic_set(&(port->port->is_link_up), 0);
		pcie_ctrl_dump_ltssm_tracer(port, true);
		hi_pcie_pr_e("port %u link fail", port->port->id);
		pcie_dsm_client_report_type(DSM_REPORT_TYPE_PCIE_LINK_FAIL);
		pcie_dsm_report(val);
		return false;
	}

	atomic_set(&(port->port->is_link_up), 1);
	hi_pcie_pr_i("port %u link succ", port->port->id);
	return true;
}

static bool pcie_ctrl_is_linkup(struct hi_pcie_ctrl *ctrl)
{
	struct hi_pcie_port *port;
	u32 is_link_up;
	u32 res = 0;

	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		if (port->disable)
			continue;
		pcie_ctrl_is_port_linkup(port->priv);
		is_link_up = (u32)atomic_read(&(port->is_link_up));
		res |= is_link_up;
	}
	return (bool)res;
}

int pcie_ctrl_check_port_linkup(struct hi_pcie_ctrl_port *port)
{
	if (pcie_ctrl_is_port_linkup(port))
		return 0;

	hi_pcie_pr_e("linkup failed!!!!!");
	return -1;
}

int pcie_ctrl_chk_link_up(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->pcie->loopback_type != PCIE_LOOPBACK_NONE &&
		ctrl->pcie->loopback_type != PCIE_LOOPBACK_DM)
		return 0;

	if (pcie_ctrl_is_linkup(ctrl))
		return 0;

	hi_pcie_pr_e("linkup failed!!!!!");

	return -1;
}

void pcie_local_intr_msi_intx_cfg(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_MSI_MASK);
	val = val | 0xFFFFFFFF;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_MSI_MASK);

	val = pcie_ctrl_ap_gbl_readl(ctrl, PORT_INTX_ASSERT_MASK);
	val = val | 0xFFFF;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PORT_INTX_ASSERT_MASK);

	val = pcie_ctrl_ap_gbl_readl(ctrl, PORT_INTX_DASSERT_MASK);
	val = val | 0xFFFF;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PORT_INTX_DASSERT_MASK);
}

#define PCIE_CTRL_IDLE_TIME  1000
static int pcie_ctrl_ap_idle_check(struct hi_pcie_ctrl *ctrl)
{
	u32 index = 0;
	u32 idle = pcie_ctrl_ap_gbl_readl(ctrl, AP_IDLE);;
	idle = idle & BIT(0);

	do {
		if (index >= PCIE_CTRL_IDLE_TIME) {
			hi_pcie_pr_e("PCIe %u wait idle timeout", ctrl->pcie->ctrl_id);
			return -1;
		}

		idle = pcie_ctrl_ap_gbl_readl(ctrl, AP_IDLE);;
		idle = idle & BIT(0);

		index++;
		udelay(10);
	} while(idle != BIT(0));

	hi_pcie_pr_i("PCIe %u wait idle OK", ctrl->pcie->ctrl_id);
	return 0;
}

int pcie_ctrl_reset(struct hi_pcie_ctrl *ctrl)
{
	struct hi_pcie_port *port;

	pcie_ctrl_ap_idle_check(ctrl);

	list_for_each_entry(port, &ctrl->pcie->ports, node)
		pcie_ctrl_enable_port_int(port->priv, false);

	pcie_local_intr_port_cfg(ctrl, false);
	pcie_local_intr_msi_intx_cfg(ctrl);

	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		pcie_ctrl_disable_port_ap(port->priv);
		pcie_ctrl_disable_port(port->priv);
	}

	pcie_ctrl_apbaxi_reset(ctrl);

	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		pcie_ctrl_soft_reset_port(port->priv);
		pcie_ctrl_soft_reset_port_logic(port->priv);
	}

	pcie_ctrl_core_sft_reset(ctrl);

	list_for_each_entry(port, &ctrl->pcie->ports, node)
		pcie_ctrl_soft_reset_port_phy(port->priv);

	if (ctrl->re_com != NULL) {
		hi_pcie_pr_e("PCIe %u free bar memory", ctrl->pcie->ctrl_id);
		dma_free_coherent(&ctrl->pcie->pdev->dev, EP_BAR0_SIZE, ctrl->re_com, ctrl->re_com_phy);
		ctrl->re_com = NULL;
	}

	list_for_each_entry(port, &ctrl->pcie->ports, node)
		atomic_set(&(port->is_link_up), 0);

#ifdef CONFIG_HIPCIE_OCT
	atomic_set(&(ctrl->pcie->is_ctrl_setup), 0);
#endif

	return 0;
}

static struct {
	char *name;
	int (*action)(struct hi_pcie_ctrl *ctrl);
} rc_setup_steps[] = {
	{ "unrset core and phy", pcie_ctrl_unrst_corephy },
	{ "set linkmode", pcie_ctrl_cfg_linkmode },
	{ "set mac misc0", pcie_ctrl_mac_misc0_cfg },
	{ "init serdes", pcie_ctrl_init_serders},
	{ "unreset core software", pcie_ctrl_core_sft_unrst },
	{ "unreset port", pcie_ctrl_port_unrst },
	{ "unreset apb axi", pcie_ctrl_apbaxi_unrst },
	{ "set port workmode", pcie_ctrl_port_workmode },
	{ "exit mem shutdown", pcie_ctrl_exit_shutdown },
	{ "init core", pcie_ctrl_init_core },
	{ "init ap", pcie_ctrl_ap_init },
	{ "reset when linkdown", pcie_ctrl_ena_ldrst },
};

static struct pci_ops pcie_ops = {
	.map_bus = pcie_core_generic_map_bus,
	.read	 = pcie_core_generic_read_conf,
	.write	 = pcie_core_generic_write_conf,
};

int pcie_ctrl_setup_rc(struct hi_pcie_ctrl *ctrl)
{
	int ret;
	u32 i;
	struct pci_host_bridge *bridge;

	if (ctrl->bridge == NULL) {
		bridge = devm_pci_alloc_host_bridge(&ctrl->pcie->pdev->dev, 0);
		if (!bridge)
			return -ENOMEM;
		bridge->ops = &pcie_ops;
		bridge->sysdata = ctrl->pcie;
		ctrl->bridge = bridge;
		ctrl->pcie->host_bridge = bridge;
	}

#if !defined(CONFIG_PCIE_HCD_LDK) && defined (CONFIG_HIPCIE_CTRL_DMA) 
	pcie_ctrl_init_dma(ctrl);
#endif

	for (i = 0; i < ARRAY_SIZE(rc_setup_steps); i++) {
		hi_pcie_pr_d("rc setup(%s)", rc_setup_steps[i].name);
		ret = rc_setup_steps[i].action(ctrl);
		if (ret != 0) {
			hi_pcie_pr_e("rc setup(%s) failed", rc_setup_steps[i].name);
			return ret;
		}
	}

	pcie_ctrl_mask_all_int(ctrl, false);

	return 0;
}
