/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller common functions used by all controllers.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_ctrl_common.h"

#include <linux/delay.h>

#include "../hi_pcie_defines.h"

#include "../core/hi_pcie_core.h"
#ifdef CONFIG_PCIE_HCD_LDK
#include "../core/hi_pcie_udk_comm.h"
#endif
#include "../hi_pcie_external_api.h"
#include "hi_pcie_ctrl_dfx.h"
#include "hi_pcie_ctrl_regs.h"

bool pcie_ctrl_is_v500_ctrl(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->ctrl_version == CTRL_VERISON_V500) 
		return true;
	return false;
}

void pcie_ctrl_rx_margin_icg_en(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if ((port->ctrl->pcie->chip_type == BOARD_FPGA) || \
		(port->ctrl->pcie_version == PCIE_VERSION3))
		return;

	val = pcie_ctrl_mac_readl(port, MAC_RX_MARGIN_SELF_CTRL);
	val &= 0x7fffffff;
	val |= BIT(31);
	pcie_ctrl_mac_writel(port, val, MAC_RX_MARGIN_SELF_CTRL);
}


void pcie_set_dl_fc_update_infer_idle_en(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if ((port->ctrl->pcie->chip_type == BOARD_FPGA) || !pcie_ctrl_is_v500_ctrl(port->ctrl))
		return;

	/* disable fc_update infer idle */
	val = pcie_ctrl_mac_readl(port, MAC_LTSSM_CTRL_INFEER_ELEC);
	val &= ~(BIT(7));
	pcie_ctrl_mac_writel(port, val, MAC_LTSSM_CTRL_INFEER_ELEC);

	return;
}

int pcie_ctrl_unrst_corephy(struct hi_pcie_ctrl *ctrl)
{
	pcie_ctrl_core_gbl_writel(ctrl, 0, PCIE_CORE_PHY_COM_SFT_RST);
	return 0;
}

int pcie_ctrl_set_linkmode(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->pcie->port_num == 1)
		pcie_ctrl_core_gbl_writel(ctrl, PORT0_LINK_MODE_X4, PORT07_LINK_MODE);
	else if (ctrl->pcie->port_num == 2)
		pcie_ctrl_core_gbl_writel(ctrl, PORT02_LINK_MODE_X2, PORT07_LINK_MODE);
	
	return 0;
}

int pcie_ctrl_cfg_linkmode(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->pcie_version == PCIE_VERSION3 || pcie_ctrl_is_v500_ctrl(ctrl))
		return 0;

	pcie_ctrl_set_linkmode(ctrl);

	return 0;
}

int pcie_ctrl_mac_misc0_cfg(struct hi_pcie_ctrl *ctrl)
{
	u32 val;

	if (ctrl->pcie->chip_type == BOARD_FPGA) {
		/* [31:16] set 0, asci default 0xffff */
		val = pcie_ctrl_core_gbl_readl(ctrl, CORE_GLB_MISC_0);
		val &= 0xFFFF;
		val |= BIT(0);
		pcie_ctrl_core_gbl_writel(ctrl, val, CORE_GLB_MISC_0);
	}

	return 0;
}

int pcie_ctrl_init_serders(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->pcie->plat_ops->init_phy != NULL)
		return ctrl->pcie->plat_ops->init_phy(ctrl->pcie);
	return 0;
}

int pcie_ctrl_core_sft_unrst(struct hi_pcie_ctrl *ctrl)
{
	/* core tl common sft unrst bit0 set 0 */
	u32 val;
	val = pcie_ctrl_core_gbl_readl(ctrl, PCIE_CORE_TL_COM_SFT_RST);
	val = val & 0xFFFFFFFE;
	pcie_ctrl_core_gbl_writel(ctrl, val, PCIE_CORE_TL_COM_SFT_RST);
	return 0;
}

void pcie_ctrl_core_sft_reset(struct hi_pcie_ctrl *ctrl)
{
	pcie_ctrl_core_gbl_writel(ctrl, 0x1, PCIE_CORE_TL_COM_SFT_RST);
	pcie_ctrl_core_gbl_writel(ctrl, 0x1, PCIE_CORE_PHY_COM_SFT_RST);
}

void pcie_ctrl_soft_reset_port(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	val = pcie_ctrl_core_gbl_readl(ctrl, PORT_RESET);
	val |= BIT(port->port->port_id);
	pcie_ctrl_core_gbl_writel(ctrl, val, PORT_RESET);
}

void pcie_ctrl_soft_unreset_port(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	val = pcie_ctrl_core_gbl_readl(ctrl, PORT_RESET);
	val &= ~BIT(port->port->port_id);
	pcie_ctrl_core_gbl_writel(ctrl, val, PORT_RESET);
}

void pcie_ctrl_soft_reset_port_logic(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	val = pcie_ctrl_core_gbl_readl(ctrl, PORT_RESET);
	val |= (BIT(port->port->port_id) << 16);
	pcie_ctrl_core_gbl_writel(ctrl, val, PORT_RESET);
}

void pcie_ctrl_soft_unreset_port_logic(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	val = pcie_ctrl_core_gbl_readl(ctrl, PORT_RESET);
	val &= ~(BIT(port->port->port_id) << 16);
	pcie_ctrl_core_gbl_writel(ctrl, val, PORT_RESET);
}

void pcie_ctrl_soft_reset_port_phy(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	val = pcie_ctrl_core_gbl_readl(ctrl, CORE_PHY_RESET);
	val |= BIT(port->port->port_id);
	pcie_ctrl_core_gbl_writel(ctrl, val, CORE_PHY_RESET);
}

void pcie_ctrl_soft_unreset_port_phy(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	val = pcie_ctrl_core_gbl_readl(ctrl, CORE_PHY_RESET);
	val &= ~BIT(port->port->port_id);
	pcie_ctrl_core_gbl_writel(ctrl, val, CORE_PHY_RESET);
}

int pcie_ctrl_port_unrst(struct hi_pcie_ctrl *ctrl)
{
	struct hi_pcie_port *port;

	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		pcie_ctrl_soft_unreset_port(port->priv);
		pcie_ctrl_soft_unreset_port_logic(port->priv);
		pcie_ctrl_soft_unreset_port_phy(port->priv);
	}

	return 0;
}

int pcie_ctrl_apbaxi_unrst(struct hi_pcie_ctrl *ctrl)
{
	/* axi apb sync unrst */
	pcie_ctrl_ap_gbl_writel(ctrl, 0x0, PCIE_AP_AXI_SYNC_RST);
	pcie_ctrl_ap_gbl_writel(ctrl, 0x0, PCIE_AP_APB_SYNC_RST);
	return 0;
}

void pcie_ctrl_apbaxi_reset(struct hi_pcie_ctrl *ctrl)
{
	/* axi apb sync unrst */
	pcie_ctrl_ap_gbl_writel(ctrl, 0x1, PCIE_AP_AXI_SYNC_RST);
	pcie_ctrl_ap_gbl_writel(ctrl, 0x1, PCIE_AP_APB_SYNC_RST);
}

void pcie_open_ssc(struct hi_pcie_ctrl_port *port)
{
	if (port->ctrl->pcie->chip_type == BOARD_FPGA)
		return;
}

void pcie_auto_speed_enable(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->chip_type == BOARD_FPGA)
		return;

	/* rc port0 cfg 1 ep default */
	val = pcie_ctrl_mac_readl(port, MAC_ENTER_LPBK_DISABLE);
	val = val & 0xFFFFFF7F;
	pcie_ctrl_mac_writel(port, val, MAC_ENTER_LPBK_DISABLE);
}

void pcie_async_fifo_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->chip_type != BOARD_FPGA)
		return;

	val = pcie_ctrl_core_gbl_readl(port->ctrl, MAC_CDC_DPTX_FIFO);
	val &= ~(0x1f << 24);
	val |= (0xD << 24);
	pcie_ctrl_core_gbl_writel(port->ctrl, val, MAC_CDC_DPTX_FIFO);
}

void pcie_fix_core_clk(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	// todo nmanager cal new val
	if (port->ctrl->pcie->chip_type != BOARD_FPGA)
		return;

	/* set freq. core freq 640M set 0x11; 1200M set 0x20; 1000M set 0x16. */
	val = pcie_ctrl_mac_readl(port, MAC_REG_USE_FIX_CORE_CLK_EN);
	val = val & 0xFFFFE0FF;
	val |= 1 << 0x8;
	pcie_ctrl_mac_writel(port, val, MAC_REG_USE_FIX_CORE_CLK_EN);
}

void pcie_l1ss_core_freqdown(struct hi_pcie_ctrl_port *port)
{
	return;
}

void pcie_close_unused_lane(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->chip_type == BOARD_FPGA)
		return;

	val = pcie_ctrl_mac_readl(port, MAC_POWERDOWN_VALUE);
	val &= ~(0xf << 28);
	if (port->ctrl->pcie_version == PCIE_VERSION3)
		val |= (0xC << 28);
	else
		val |= (0x6 << 28);
	pcie_ctrl_mac_writel(port, val, MAC_POWERDOWN_VALUE);
}

void pcie_ap_core_clk_sync(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;

	/* ap core clk sync */
	if (port->ctrl->pcie->chip_type != BOARD_FPGA) {
		if (ctrl->pcie_version == PCIE_VERSION3) {
			val = pcie_ctrl_mac_readl(port, MAC_REG_USE_FIX_CORE_CLK_EN);
			val = val & ~BIT(0);
			pcie_ctrl_mac_writel(port, val, MAC_REG_USE_FIX_CORE_CLK_EN);
		} else {
			/* fix ap core clk sync */
			val = pcie_ctrl_mac_readl(port, MAC_REG_USE_FIX_CORE_CLK_EN);
			val = val | BIT(0);
			pcie_ctrl_mac_writel(port, val, MAC_REG_USE_FIX_CORE_CLK_EN);
		}
		return;
	}

	/* cfg txdata valid mode */
	val = pcie_ctrl_mac_readl(port, MAC_ENTER_LPBK_DISABLE);
	val = val | BIT(28);
	pcie_ctrl_mac_writel(port, val, MAC_ENTER_LPBK_DISABLE);
}

void pcie_cfg_max_link_width(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	/* rc ep x4 */
	val = pcie_ctrl_mac_readl(port, MAC_TARGET_LINK_WIDTH);
	val &= 0xffffffc0;
	val |= port->port->num_lanes;
	pcie_ctrl_mac_writel(port, val, MAC_TARGET_LINK_WIDTH);
}

void pcie_ctrl_mac_rx_idle(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->chip_type == BOARD_FPGA || pcie_ctrl_is_v500_ctrl(port->ctrl))
		return;

	val = pcie_ctrl_mac_readl(port, MAC_REG_FXIDLE_DELAY_TIME);
	/* bit 8 set 1, [7:0] set 0xc, version3 bit 9 set 0, version4 set 1 */
	val &= (u32)(~0x1FF);
	if (port->ctrl->pcie_version == PCIE_VERSION4)
		val |= BIT(9);

	val |= BIT(8);
	val |= 0xc;
	pcie_ctrl_mac_writel(port, val, MAC_REG_FXIDLE_DELAY_TIME);
}

void pcie_ctrl_full_eq_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	val = pcie_ctrl_cfg_readl(port, GEN5_PHY_CAP_REG08);
	val = val | BIT(0) | BIT(1);
	pcie_ctrl_cfg_writel(port, val, GEN5_PHY_CAP_REG08);
}

void pcie_ctrl_rx_standby(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	/* standby disable */
	val = pcie_ctrl_mac_readl(port, MAC_LTSSM_CTRL_INFEER_ELEC);
	val &= ~(0xf << 8);
	if (port->ctrl->pcie->chip_type == BOARD_FPGA)
		val |= (0xf << 8);
	else
		val |= (0x3 << 8);
	pcie_ctrl_mac_writel(port, val, MAC_LTSSM_CTRL_INFEER_ELEC);

	if (port->ctrl->pcie->chip_type == BOARD_FPGA)
		return;

	/* eios idle delay */
	val = pcie_ctrl_mac_readl(port, MAC_REQ_EIOS_TO_ELEIDLE_DELAY);
	val &= ~0x3f;
	val |= BIT(1);
	pcie_ctrl_mac_writel(port, val, MAC_REQ_EIOS_TO_ELEIDLE_DELAY);
}

void pcie_ctrl_idle_delay(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie->chip_type != BOARD_FPGA)
		return;

	val = pcie_ctrl_mac_readl(port, MAC_REQ_EIOS_TO_ELEIDLE_DELAY);
	val &= ~(0xff << 8);
	val |= (0x8 << 8);
	pcie_ctrl_mac_writel(port, val, MAC_REQ_EIOS_TO_ELEIDLE_DELAY);
}

void pcie_cfg_enter_l1l2_timeout(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	val = pcie_ctrl_mac_readl(port, MAC_REG_ENTER_L1L2_TIMEOUT_VAL);
	val &= ~0x3ff;
	val |= 0xC;
	pcie_ctrl_mac_writel(port, val, MAC_REG_ENTER_L1L2_TIMEOUT_VAL);
}

/* TL init */
void pcie_max_tag_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	u32 reg;
	struct hi_pcie_ctrl *ctrl = port->ctrl;

	if (ctrl->pcie->chip_type == BOARD_ASIC)
		return;

	reg = tl_tx_tag_ctrl(port->port->port_id);
	val = pcie_ctrl_tl_core_read(ctrl, reg);
	val &= ~0xff;
	val |= 0x2;
	pcie_ctrl_tl_core_write(ctrl, val, reg);
}

void pcie_portpf_mapping_cfg(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	u32 reg;
	struct hi_pcie_ctrl *ctrl = port->ctrl;

	if (ctrl->pcie_version == PCIE_VERSION4 && !pcie_ctrl_is_v500_ctrl(port->ctrl))
		return;

	reg = PN_PF_STR_IDX0;

	val = pcie_ctrl_tl_core_read(ctrl, reg);
	val = val & 0xFFFFFFE0;
	pcie_ctrl_tl_core_write(ctrl, val, reg);

	val = pcie_ctrl_tl_core_read(ctrl, reg);
	val &= ~0x1f1f1f00;
	val = val | BIT(8) | BIT(16) | BIT(24);
	pcie_ctrl_tl_core_write(ctrl, val, reg);

	for (reg = PN_PF_STR_IDX1; reg <= PN_PF_STR_IDX3; reg += 0x4) {
		val = pcie_ctrl_tl_core_read(ctrl, reg);
		val &= ~0x1f1f1f1f;
		val = val | BIT(0) | BIT(8) | BIT(16) | BIT(24);
		pcie_ctrl_tl_core_write(ctrl, val, reg);
	}

	val = pcie_ctrl_tl_core_read(ctrl, reg);
	val &= ~0x1f;
	val = val | BIT(0);
	pcie_ctrl_tl_core_write(ctrl, val, reg);
}

void pcie_tlp_md_indicator(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	val = pcie_ctrl_tl_core_read(port->ctrl, TL_TX_CTRL);
	val = val | BIT(0);
	pcie_ctrl_tl_core_write(port->ctrl, val, TL_TX_CTRL);
}

void pcie_tl_rx_dfxfunc_en(struct hi_pcie_ctrl_port *port)
{
	struct hi_pcie_ctrl *ctrl = port->ctrl;

	if (ctrl->pcie_version == PCIE_VERSION3 || pcie_ctrl_is_v500_ctrl(port->ctrl))
		return;

	pcie_ctrl_tl_core_write(ctrl, 0x4010, TL_DFX_PM_CORE_FUNC_EN);
}

void pcie_port_pwrn_scale(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie_version == PCIE_VERSION3)
		return;

	val = pcie_ctrl_cfg_readl(port, TL_L1PM_CAPBILITY);
	val &= ~0x30000;
	val = val | BIT(16);
	pcie_ctrl_cfg_writel(port, val, TL_L1PM_CAPBILITY);
}

void pcie_set_speed_limit(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	val = pcie_ctrl_cfg_readl(port, LINK_CAPBILITY);
	if (port->ctrl->pcie->chip_type == BOARD_FPGA) {
		val &= 0xfffffc00;
		val |= 0x1;
	} else {
		val &= 0xfffffc00;
		val |= port->ctrl->pcie_version;
	}
	val |= (port->port->num_lanes << 4);
	pcie_ctrl_cfg_writel(port, val, LINK_CAPBILITY);

	val = pcie_ctrl_cfg_readl(port, LINK_CTRL_STATUS2);
	val &= 0xFFFFFFF0;
	if (port->port->limit_speed <= PCIE_LINK_SPEED_LIMIT ||
		port->port->limit_speed >= PCIE_LINK_SPEED_BOTTEOM)
		val = val | port->ctrl->pcie_version;
	else
		val = val | port->port->limit_speed;
	pcie_ctrl_cfg_writel(port, val, LINK_CTRL_STATUS2);
}

void pcie_max_payload_support(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	if (port->ctrl->pcie_version == PCIE_VERSION4 || port->ctrl->pcie->mode == PCIE_MODE_RC)
		return;

	val = pcie_ctrl_cfg_readl(port, DEVICE_CAPBILITY);
	val &= ~0x7;
	val = val | BIT(0);
	pcie_ctrl_cfg_writel(port, val, DEVICE_CAPBILITY);
}

void pcie_support_l2_aux(struct hi_pcie_ctrl_port *port)
{
	u32 val;

	val = pcie_ctrl_cfg_readl(port, DEVICE_CTRL_STATUS);
	val = val & ~BIT(20);
	pcie_ctrl_cfg_writel(port, val, DEVICE_CTRL_STATUS);
}

void pcie_ctrl_poison_tl(struct hi_pcie_ctrl_port *port)
{
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;

	val = pcie_ctrl_tl_readl(port, TL_RX_CHK_EN);
	val &= ~BIT(24);
	pcie_ctrl_tl_writel(port, val, TL_RX_CHK_EN);

	val = pcie_ctrl_tl_core_read(ctrl, TL_RX_MISC_CFG);
	val &= ~BIT(2);
	pcie_ctrl_tl_core_write(ctrl, val, TL_RX_MISC_CFG);
}

/* DMA */
static int pcie_ctrl_dma_rst(struct hi_pcie_ctrl *ctrl, u32 dma_idx)
{
#ifdef CONFIG_HIPCIE_CTRL_DMA
	pcie_ctrl_reset_dma_queue(ctrl, &ctrl->dma_queues[dma_idx]);
#endif
	return 0;
}

static void pcie_ctrl_dma_init(struct hi_pcie_ctrl *ctrl, u32 dma_idx)
{
#ifdef CONFIG_HIPCIE_CTRL_DMA
	pcie_ctrl_init_dma_queue(ctrl, &ctrl->dma_queues[dma_idx]);
#endif
}

void pcie_dma_cfg(struct hi_pcie_ctrl *ctrl)
{
	u32 dma_idx;
	for (dma_idx = 0; dma_idx < ctrl->dma_queue_num; dma_idx++) {
		ctrl->dma_queues[dma_idx].channel = dma_idx;
		if (pcie_ctrl_dma_rst(ctrl, dma_idx))
			continue;
		pcie_ctrl_dma_init(ctrl, dma_idx);
	}
}

static void pcie_tx_buffer_cfg1link(struct hi_pcie_ctrl *ctrl)
{
	pcie_ctrl_ap_iobtx_writel(ctrl, 0, iob_tx_dbuf_p_pri_rgn_addr(0));
	pcie_ctrl_ap_iobtx_writel(ctrl, 0, iob_tx_dbuf_p_pri_rgn_addr(1));
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, iob_tx_hbuf_p_pri_rgn_addr(0));
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, iob_tx_hbuf_p_pri_rgn_addr(1));
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x70000, iob_tx_hbuf_src_pri_rgn_addr(0));
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x50, IOB_TX_SRC_PRI_RGN_MAP);

	if (ctrl->pcie->chip_type == BOARD_FPGA) {
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, IOB_TX_DBUF_P_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, IOB_TX_DBUF_CPL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x1F0000, iob_tx_dbuf_src_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x3F0020, iob_tx_dbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x5F0040, iob_tx_dbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x7F0060, IOB_TX_DBUF_ALL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, IOB_TX_HBUF_P_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, IOB_TX_HBUF_CPL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0xF0008, iob_tx_hbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x170010, iob_tx_hbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x3F0018, IOB_TX_HBUF_ALL_SHR_RGN_ADDR);
	} else {
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x9F0080, IOB_TX_DBUF_P_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0xBF00A0, IOB_TX_DBUF_CPL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x3F0000, iob_tx_dbuf_src_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x5F0020, iob_tx_dbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x7F0060, iob_tx_dbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x2FF00C0, IOB_TX_DBUF_ALL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x1B0014, IOB_TX_HBUF_P_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x23001C, IOB_TX_HBUF_CPL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0xB0004, iob_tx_hbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x13000C, iob_tx_hbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x7F0024, IOB_TX_HBUF_ALL_SHR_RGN_ADDR);
	}
}

static void pcie_tx_buffer_cfg2link(struct hi_pcie_ctrl *ctrl)
{
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, IOB_TX_DBUF_P_SHR_RGN_ADDR);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, IOB_TX_DBUF_CPL_SHR_RGN_ADDR);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, IOB_TX_HBUF_P_SHR_RGN_ADDR);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, IOB_TX_HBUF_CPL_SHR_RGN_ADDR);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x70000, iob_tx_hbuf_src_pri_rgn_addr(0));
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0350, IOB_TX_SRC_PRI_RGN_MAP);

	if (ctrl->pcie->chip_type == BOARD_FPGA) {
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, iob_tx_dbuf_p_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, iob_tx_dbuf_p_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x1F0000, iob_tx_dbuf_src_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x3F0020, iob_tx_dbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x5F0040, iob_tx_dbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x7F0060, IOB_TX_DBUF_ALL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, iob_tx_hbuf_p_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0, iob_tx_hbuf_p_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0xF0008, iob_tx_hbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x170010, iob_tx_hbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x3F0018, IOB_TX_HBUF_ALL_SHR_RGN_ADDR);
	} else {
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x19F0080, iob_tx_dbuf_p_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x2BF01A0, iob_tx_dbuf_p_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x3F0000, iob_tx_dbuf_src_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x5F0020, iob_tx_dbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x7F0060, iob_tx_dbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x2FF02C0, IOB_TX_DBUF_ALL_SHR_RGN_ADDR);
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x430014, iob_tx_hbuf_p_pri_rgn_addr(0));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x730044, iob_tx_hbuf_p_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0xB0004, iob_tx_hbuf_src_pri_rgn_addr(1));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x13000C, iob_tx_hbuf_src_pri_rgn_addr(2));
		pcie_ctrl_ap_iobtx_writel(ctrl, 0x7F0074, IOB_TX_HBUF_ALL_SHR_RGN_ADDR);
	}
}

void pcie_init_tx_buf(struct hi_pcie_ctrl *ctrl)
{
	if (ctrl->pcie_version == PCIE_VERSION3)
		return;
	if (pcie_ctrl_is_v500_ctrl(ctrl))
		return;

	if (ctrl->pcie->port_num == 2)
		pcie_tx_buffer_cfg2link(ctrl);
	else
		pcie_tx_buffer_cfg1link(ctrl);
}

static void pcie_rx_aml_err_polic_set(struct hi_pcie_ctrl *ctrl)
{
	u32 val;

	if (!pcie_ctrl_is_v500_ctrl(ctrl))
		return;

	val = pcie_ctrl_ap_iobrx_readl(ctrl, IOB_RX_AML_ERR_WDATA_CTRL);
	val = val | BIT(4);
	pcie_ctrl_ap_iobrx_writel(ctrl, val, IOB_RX_AML_ERR_WDATA_CTRL);

	val = pcie_ctrl_ap_engine_readl(ctrl, RBA_MODE_CFG);
	val &= ~BIT(5);
	pcie_ctrl_ap_engine_writel(ctrl, val, RBA_MODE_CFG);
}

void pcie_init_rx_buf(struct hi_pcie_ctrl *ctrl)
{
	u32 val;

	val = pcie_ctrl_ap_iobrx_readl(ctrl, IOB_RX_DATA_BUFFER_SIZE);
	val |= BIT(10);
	pcie_ctrl_ap_iobrx_writel(ctrl, val, IOB_RX_DATA_BUFFER_SIZE);

	val = pcie_ctrl_ap_iobrx_readl(ctrl, IOB_RX_DATA_BUFFER_SIZE);
	val &= ~BIT(10);
	pcie_ctrl_ap_iobrx_writel(ctrl, val, IOB_RX_DATA_BUFFER_SIZE);

	if (ctrl->pcie_version == PCIE_VERSION4) {
		pcie_rx_aml_err_polic_set(ctrl);
		return;
	}

	val = pcie_ctrl_ap_iobrx_readl(ctrl, IOB_RX_AML_ERR_WDATA_CTRL);
	val = val | BIT(4) | BIT(5);
	pcie_ctrl_ap_iobrx_writel(ctrl, val, IOB_RX_AML_ERR_WDATA_CTRL);
}

int pcie_ctrl_open_clk_gate(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	struct hi_pcie_port *port;

	/* ap open clk gating */
	val = pcie_ctrl_ap_gbl_readl(ctrl, AP_SUB_ICG_EN);
	val = val | BIT(0);
	pcie_ctrl_ap_gbl_writel(ctrl, val, AP_SUB_ICG_EN);

	if (ctrl->pcie_version == PCIE_VERSION3) {
		val = pcie_ctrl_ap_gbl_readl(ctrl, AP_SUB_AXI_ICG_EN);
		val = val | 0x1F;
		pcie_ctrl_ap_gbl_writel(ctrl, val, AP_SUB_AXI_ICG_EN);

		return 0;
	}

	/* core open clk gating */
	val = pcie_ctrl_core_gbl_readl(ctrl, CORE_PORT_CLK_ICG_EN_CTRL);

	if (ctrl->pcie->port_num == 2)
		val = val & 0xFFFAFFFA;
	else
		val = val & 0xFFFEFFFE;

	pcie_ctrl_core_gbl_writel(ctrl, val, CORE_PORT_CLK_ICG_EN_CTRL);

	/* dl phy open clk gating */
	val = pcie_ctrl_core_gbl_readl(ctrl, DLPHY_PMT_ICG_EN_CTRL);
	val = val | BIT(0);
	pcie_ctrl_core_gbl_writel(ctrl, val, DLPHY_PMT_ICG_EN_CTRL);

	/* tl open clk gating */
	pcie_ctrl_tl_dfx_write(ctrl, 0xFFFFFFFF, TL_ICG_SW_CTRL);

	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		/* dl open clk gating */
		pcie_ctrl_dl_writel(port->priv, 0x1FFFF, DFX_ICG_CTRL);
		/* mac open clk gating */
		pcie_ctrl_mac_writel(port->priv, 0xFFFFFFFF, MAC_ICG_EN_CTRL);
	}

	return 0;
}

int pcie_ctrl_ena_port(struct hi_pcie_ctrl *ctrl)
{
	u32 val;
	u32 val_bits = 0;
	struct hi_pcie_port *port;

	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		if (port->disable)
			continue;
		val_bits |= BIT(port->port_id);
	}

	/* core port en */
	val = pcie_ctrl_core_gbl_readl(ctrl, PORT_EN);
	val = val | val_bits;
	pcie_ctrl_core_gbl_writel(ctrl, val, PORT_EN);

	if (ctrl->pcie->loopback_type != PCIE_LOOPBACK_NONE &&
		ctrl->pcie->loopback_type != PCIE_LOOPBACK_DM)
		return 0;
	/* ap port ena */
	val = pcie_ctrl_ap_gbl_readl(ctrl, AP_PORT_EN);
	val = val | val_bits;
	pcie_ctrl_ap_gbl_writel(ctrl, val, AP_PORT_EN);

	return 0;
}

void pcie_ctrl_disable_irq(struct hi_pcie_ctrl *ctrl)
{
#ifdef CONFIG_PCIE_HCD_LDK
	if (ctrl->pcie->udk_local_irq) {
		liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_SD_PROXY_PCIE_MAJOR_CMD,
			SD_PROXY_UDK_CMD_PCIE_IRQ_DISABLE), NULL, 0, (void *)ctrl->pcie->ctrl_id, sizeof(ctrl->pcie->ctrl_id));
		return;
	}	
#endif
	if (ctrl->local_irq >= 0)
		disable_irq((u32)ctrl->local_irq);
}

void pcie_ctrl_enable_irq(struct hi_pcie_ctrl *ctrl)
{
#ifdef CONFIG_PCIE_HCD_LDK
	if (ctrl->pcie->udk_local_irq) {
		liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_SD_PROXY_PCIE_MAJOR_CMD,
			SD_PROXY_UDK_CMD_PCIE_IRQ_ENABLE), NULL, 0, (void *)ctrl->pcie->ctrl_id, sizeof(ctrl->pcie->ctrl_id));
		return;
	}
#endif
	if (ctrl->local_irq >= 0)
		enable_irq((u32)ctrl->local_irq);
}
