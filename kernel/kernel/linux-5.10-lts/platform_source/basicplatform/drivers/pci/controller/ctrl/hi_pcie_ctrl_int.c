/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller dfx logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_ctrl_int.h"
#include "hi_pcie_ctrl_regs.h"
#include "hi_pcie_ctrl_common.h"
#include "hi_pcie_ctrl_dma.h"
#include "../core/hi_pcie_msi.h"
#include "hi_pcie_ctrl_dfx.h"

enum {
	HI_PCIE_INT_NI = 0,
	HI_PCIE_INT_CE,
	HI_PCIE_INT_NFE,
	HI_PCIE_INT_FE,
	HI_PCIE_INT_MAX,
};

struct hi_pcie_int_desc {
	u32 bit;
	char *desc;
	u32 event;
};

static void hi_pcie_process_int(struct hi_pcie_ctrl *ctrl, u32 pdx,
	struct hi_pcie_int_desc *descs, u32 size, u32 status)
{
	int i;
	struct hi_pcie_port *port = NULL;

	for (i = 0; i < (int)size; i++) {
		if ((descs[i].bit & status) == 0)
			continue;
		hi_pcie_pr_e("[pcie %u, port %u] %s happened", ctrl->pcie->ctrl_id,
			pdx, descs[i].desc);
		list_for_each_entry(port, &ctrl->pcie->ports, node) {
			if ((port->port_id == pdx) && (descs[i].event == HI_PCIE_EVENT_LINKDOWN))
				schedule_work(&port->events_work[HI_PCIE_EVENT_LINKDOWN]);
		}
	}
}

/* local intr */
void pcie_local_intr_map(struct hi_pcie_ctrl *ctrl)
{
	u32 val;

	/* local interrupt map msi spi0/spi1 [23:8] */
	val = pcie_ctrl_ap_gbl_readl(ctrl, PCIE_ERR_MAPPING);
	val &= ~0x3fff00;
	val |= 0x144400;
	pcie_ctrl_ap_gbl_writel(ctrl, val, PCIE_ERR_MAPPING);
}

void pcie_ctrl_enable_port_int(struct hi_pcie_ctrl_port *port, bool enable)
{
	int i;
	u32 val;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	static u32 enable_list[] = {
		PCIE_CE_ENA,
		PCIE_UNF_ENA,
		PCIE_UF_ENA,
	};

	for (i = 0; i < ARRAY_SIZE(enable_list); i++) {
		val = pcie_ctrl_ap_gbl_readl(ctrl, enable_list[i]);
		if (enable)
			val |= BIT(port->port->port_id);
		else
			val &= ~BIT(port->port->port_id);
		pcie_ctrl_ap_gbl_writel(ctrl, val, enable_list[i]);
	}
}

void pcie_local_intr_port_cfg(struct hi_pcie_ctrl *ctrl, bool enable)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		u32 mask;
	} enable_list[] = {
		{ PCIE_AP_CE_ENA, 0x3FF },
		{ PCIE_AP_UNF_ENA, 0x3FF },
		{ PCIE_AP_UF_ENA, 0x3FF },
		{ PCIE_AP_NI_ENA, 0x3FF },
		{ PCIE_CORE_CE_ENA, BIT(0) },
		{ PCIE_CORE_UNF_ENA, BIT(0) },
		{ PCIE_CORE_UF_ENA, BIT(0) },
		{ PCIE_CORE_NI_ENA, BIT(0) },
	};

	for (i = 0; i < ARRAY_SIZE(enable_list); i++) {
		val = pcie_ctrl_ap_gbl_readl(ctrl, enable_list[i].reg);
		if (enable)
			val |= enable_list[i].mask;
		else
			val &= ~enable_list[i].mask;
		pcie_ctrl_ap_gbl_writel(ctrl, val, enable_list[i].reg);
	}
}

static void pcie_ctrl_mask_top_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		u32 mask;
	} mask_list[] = {
		{ PCIE_AP_CE_MASK, 0x3FF },
		{ PCIE_AP_UNF_MASK, 0x3FF },
		{ PCIE_AP_UF_MASK, 0x3FF },
		{ PCIE_AP_NI_MASK, 0x3FF },
		{ PCIE_CE_MASK, 0xFFFF },
		{ PCIE_UF_MASK, 0xFFFF },
		{ PCIE_UNF_MASK, 0xFFFF },
		{ PCIE_CORE_CE_MASK, BIT(0) },
		{ PCIE_CORE_UNF_MASK, BIT(0) },
		{ PCIE_CORE_UF_MASK, BIT(0) },
		{ PCIE_CORE_NI_MASK, BIT(0) },
	};

	for (i = 0; i < ARRAY_SIZE(mask_list); i++) {
		val = pcie_ctrl_ap_gbl_readl(ctrl, mask_list[i].reg);
		if (mask)
			val |= mask_list[i].mask;
		else
			val &= ~mask_list[i].mask;
		pcie_ctrl_ap_gbl_writel(ctrl, val, mask_list[i].reg);
	}
}

static void pcie_ctrl_mask_iobtx_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	u32 val = 0;
	if (mask)
		val = 0xffffffff;
	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_INT_MSK0);	
	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_INT_MSK1);
	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_INT_MSK2);
	pcie_ctrl_ap_iobtx_writel(ctrl, val, IOB_TX_INT_MSK3);
}

static void pcie_ctrl_iobtx_int_clr(struct hi_pcie_ctrl *ctrl)
{
	pcie_ctrl_ap_iobtx_writel(ctrl, 0xFFFFFFFF, IOB_TX_INT_STATUS1);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0xFFFFFFFF, IOB_TX_INT_STATUS2);
}

static void pcie_ctrl_mask_iobrx_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	u32 val = 0;
	if (mask)
		val = 0xFFFFFFFF;
	pcie_ctrl_ap_iobrx_writel(ctrl, val, IOB_RX_INT_MSK);
}

static void pcie_ctrl_mask_int_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
}

static void pcie_ctrl_mask_dma_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	u32 val;
	u32 dma_idx;

	for (dma_idx = 0; dma_idx < ctrl->dma_queue_num; dma_idx++) {
		val = pcie_ctrl_ap_dma_reg_readl(ctrl, dma_queue_int_msk(dma_idx));
		if (mask)
			val |= ~0xFFF80000;
		else
			val &= 0xFFF80000;
		pcie_ctrl_ap_dma_reg_writel(ctrl, val, dma_queue_int_msk(dma_idx));
	}

	val = pcie_ctrl_ap_dma_reg_readl(ctrl, DMA_EP_INT_MSK);
	if (mask)
		val |= ~0xFFFFFF00;
	else
		val &= 0xFFFFFF00;
	pcie_ctrl_ap_dma_reg_writel(ctrl, val, DMA_EP_INT_MSK);
}

static void pcie_ctrl_mask_core_top_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	if (mask)
		pcie_ctrl_top_reg_writel(ctrl, 0xFFFFFFFF, APB_TIMEOUT_INT_MASK);
	else
		pcie_ctrl_top_reg_writel(ctrl, 0x0, APB_TIMEOUT_INT_MASK);
}

static void pcie_ctrl_mask_ap_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	pcie_ctrl_mask_iobtx_int(ctrl, mask);
	pcie_ctrl_mask_iobrx_int(ctrl, mask);
	pcie_ctrl_mask_int_int(ctrl, mask);
	if (ctrl->pcie->mode == PCIE_MODE_EP)
		pcie_ctrl_mask_dma_int(ctrl, mask);
	else
		pcie_ctrl_mask_dma_int(ctrl, true);
	pcie_ctrl_mask_core_top_int(ctrl, mask);
}

void pcie_ctrl_clear_port_int(struct hi_pcie_ctrl_port *port)
{
	int i;
	u32 status;
	u32 reg;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	static struct {
		u32 port0_reg;
		u32 port8_reg;
	} clear_list[] = {
		{ CORE_INT_NI_STATUS_0, CORE_INT_NI_STATUS_1 },
		{ CORE_INT_CE_STATUS_0, CORE_INT_CE_STATUS_1 },
		{ CORE_INT_NFE_STATUS_0, CORE_INT_NFE_STATUS_1 },
		{ CORE_INT_FE_STATUS_0, CORE_INT_FE_STATUS_1 },
	};
	static u32 clear_list2[4] = {
		CORE_INT_CE_STATUS_2,
		CORE_INT_NI_STATUS_2,
		CORE_INT_NFE_STATUS_2,
		CORE_INT_FE_STATUS_2,
	};

	status = pcie_ctrl_mac_readl(port, MAC_REG_MAC_INT_STATUS);
	pcie_ctrl_mac_writel(port, status, MAC_REG_MAC_INT_STATUS);

	status = pcie_ctrl_dl_readl(port, DL_INT_STATUS);
	pcie_ctrl_dl_writel(port, status, DL_INT_STATUS);

	status = pcie_ctrl_tl_readl(port, TL_INT_STATUS0);
	pcie_ctrl_tl_writel(port, status, TL_INT_STATUS0);
	status = pcie_ctrl_tl_readl(port, TL_INT_STATUS1);
	pcie_ctrl_tl_writel(port, status, TL_INT_STATUS1);
	status = pcie_ctrl_tl_readl(port, TL_INT_STATUS2);
	pcie_ctrl_tl_writel(port, status, TL_INT_STATUS2);

	for (i = 0; i < 4; i++) {
		if (port->port->port_id < 8)
			reg = clear_list[i].port0_reg;
		else
			reg = clear_list[i].port8_reg;
		status = 0x7 << ((port->port->port_id % 8) * 3);
		pcie_ctrl_core_gbl_writel(ctrl, status, reg);
		pcie_ctrl_core_gbl_writel(ctrl, BIT(port->port->port_id), clear_list2[i]);
	}
}

void pcie_ctrl_mask_port_int(struct hi_pcie_ctrl_port *port, bool mask)
{
	int i;
	u32 val;
	u32 reg;
	struct hi_pcie_ctrl *ctrl = port->ctrl;
	static struct {
		u32 port0_reg;
		u32 port8_reg;
	} mask_list[4] = {
		{ CORE_INT_NI_MSK_0, CORE_INT_NI_MSK_1 },
		{ CORE_INT_CE_MSK_0, CORE_INT_CE_MSK_1 },
		{ CORE_INT_NFE_MSK_0, CORE_INT_NFE_MSK_1 },
		{ CORE_INT_FE_MSK_0, CORE_INT_FE_MSK_1 },
	};
	static u32 mask_list2[4] = {
		CORE_INT_CE_MSK_2,
		CORE_INT_NI_MSK_2,
		CORE_INT_NFE_MSK_2,
		CORE_INT_FE_MSK_2,
	};
	for (i = 0; i < 4; i++) {
		if (port->port->port_id < 8)
			reg = mask_list[i].port0_reg;
		else
			reg = mask_list[i].port8_reg;
		val = pcie_ctrl_core_gbl_readl(ctrl, reg);
		if (mask)
			val |= 0x7 << ((port->port->port_id % 8) * 3);
		else
			val &= ~(0x7 << ((port->port->port_id % 8) * 3));
		pcie_ctrl_core_gbl_writel(ctrl, val, reg);
		val = pcie_ctrl_core_gbl_readl(ctrl, mask_list2[i]);
		if (mask)
			val |= BIT(port->port->port_id);
		else
			val &= ~BIT(port->port->port_id);
		pcie_ctrl_core_gbl_writel(ctrl, val, mask_list2[i]);
	}
}

static void pcie_ctrl_mask_core_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	int i;
	u32 val;
	static struct {
		u32 reg;
		u32 mask;
	} mask_list[] = {
		{ CORE_INT_CE_MSK_0, 0xFF000000 },
		{ CORE_INT_NI_MSK_0, 0xFF000000 },
		{ CORE_INT_NFE_MSK_0, 0xFF000000 },
		{ CORE_INT_FE_MSK_0, 0xFF000000 },
		{ CORE_INT_CE_MSK_1, 0xFBFFFFFF },
		{ CORE_INT_NI_MSK_1, 0xFBFFFFFF },
		{ CORE_INT_NFE_MSK_1, 0xFBFFFFFF },
		{ CORE_INT_FE_MSK_1, 0xFBFFFFFF },
	};

	for (i = 0; i < ARRAY_SIZE(mask_list); i++) {
		val = pcie_ctrl_core_gbl_readl(ctrl, mask_list[i].reg);
		if (mask)
			val |= ~mask_list[i].mask;
		else
			val &= mask_list[i].mask;
		pcie_ctrl_core_gbl_writel(ctrl, val, mask_list[i].reg);
	}
}

static void pcie_ctrl_mask_tl_int(struct hi_pcie_ctrl_port *port, bool mask)
{
	u32 val = 0;
	if (mask)
		val = 0xffffffff;
	pcie_ctrl_tl_writel(port, val, TL_INT_MASK0);
}

static void pcie_ctrl_mask_dl_int(struct hi_pcie_ctrl_port *port, bool mask)
{
	u32 val;
	if (mask)
		val = 0xffffffff;
	else
		val = (u32)(~BIT(17)); // only care about linkdown event;
	pcie_ctrl_dl_writel(port, val, DL_INT_MASK);
}

static void pcie_ctrl_mask_mac_int(struct hi_pcie_ctrl_port *port, bool mask)
{
	u32 val = 0;
	if (mask)
		val = 0xffffffff;
	else
		val = (u32)(~BIT(1)); // only care about linkdown event;
	pcie_ctrl_mac_writel(port, val, MAC_REG_INT_MASK);
}

static void pcie_ctrl_mask_bottom_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	u32 val;
	struct hi_pcie_port *port;

	/* PF FLR int mask */
	val = pcie_ctrl_tl_core_read(ctrl, TL_PF_FLR_INT_MASK);
	if (mask)
		val |= ~0xFFFFFFFE;
	else
		val &= 0xFFFFFFFE;
	pcie_ctrl_tl_core_write(ctrl, val, TL_PF_FLR_INT_MASK);

	val = 0;
	if (mask)
		val = 0xffffffff;
	/* tl core int mask */
	pcie_ctrl_tl_core_write(ctrl, val, TL_CORE_INT_MASK0);

	/* dl mac int mask */
	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		pcie_ctrl_mask_tl_int(port->priv, mask);
		pcie_ctrl_mask_dl_int(port->priv, mask);
		pcie_ctrl_mask_mac_int(port->priv, mask);
	}

	/* hilink mask */
	val = pcie_ctrl_core_gbl_readl(ctrl, HILINK_INT_MASK);
	if (mask)
		val |= ~0xFFF00000;
	else
		val &= 0xFFF00000;
	pcie_ctrl_core_gbl_writel(ctrl, val, HILINK_INT_MASK);
}

void pcie_ctrl_mask_all_int(struct hi_pcie_ctrl *ctrl, bool mask)
{
	pcie_ctrl_mask_top_int(ctrl, mask);
	pcie_ctrl_mask_ap_int(ctrl, mask);
	pcie_ctrl_mask_core_int(ctrl, mask);
	pcie_ctrl_mask_bottom_int(ctrl, mask);
}

static void hi_pcie_handle_ap_top_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	static struct hi_pcie_int_desc descs[] = {
		{ BIT(0), "apb timeout", HI_PCIE_EVENT_INVALID },
	};

	if (ctrl && ctrl->pcie && ctrl->pcie->plat_ops && ctrl->pcie->plat_ops->dump_plat_info)
		(void)ctrl->pcie->plat_ops->dump_plat_info(ctrl->pcie);

	u32 status = pcie_ctrl_top_reg_readl(ctrl, APB_TIMEOUT_INT_RO);

	pcie_ctrl_mask_core_top_int(ctrl, true);

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_top_reg_writel(ctrl, status, APB_TIMEOUT_INT_STATUS);

	pcie_ctrl_mask_core_top_int(ctrl, false);
}

static void hi_pcie_handle_int_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	u32 status;
	static u32 regs[HI_PCIE_INT_MAX] = {
		0, LOCAL_CE_INT, LOCAL_NFE_INT, LOCAL_FE_INT,
	};
	static struct hi_pcie_int_desc descs[] = {
	};

	if (level == HI_PCIE_INT_NI)
		return;
	status = pcie_ctrl_ap_int_readl(ctrl, regs[level]);

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_ap_int_writel(ctrl, 0xffffffff, ECC_ERR_INT_SRC_STS);
	pcie_ctrl_ap_int_writel(ctrl, 0xffffffff, ABNORMAL_INT_STS);
}

#ifdef CONFIG_HIPCIE_CTRL_DMA
static void hi_pcie_handle_dma_top_int(struct hi_pcie_ctrl *ctrl)
{
	u32 status;
	static struct hi_pcie_int_desc descs[] = {
	};

	status = pcie_ctrl_ap_dma_reg_readl(ctrl, DMA_EP_INT);
	if (status == 0)
		return;

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_ap_dma_reg_writel(ctrl, status, DMA_EP_INT);
}

static void hi_pcie_handle_dma_queue_int(struct hi_pcie_ctrl *ctrl)
{
	int i;
	u32 status;

	for (i = 0; i < (int)ctrl->dma_queue_num; i++) {
		status = pcie_ctrl_ap_dma_reg_readl(ctrl, dma_queue_int_sts(i));
		if (status == 0)
			continue;

		if (status & BIT(0))
			pcie_ctrl_dma_finish(ctrl, &ctrl->dma_queues[i]);

		pcie_ctrl_ap_dma_reg_writel(ctrl, status, dma_queue_int_sts(i));
	}
}

static void hi_pcie_handle_dma_queue_err_int(struct hi_pcie_ctrl *ctrl)
{
	u32 status;
	static struct hi_pcie_int_desc descs[] = {
	};

	status = pcie_ctrl_ap_dma_reg_readl(ctrl, dma_queue_err_int_sts(0));
	if (status == 0)
		return;

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_ap_dma_reg_writel(ctrl, status, dma_queue_err_int_sts(0));
}
#endif

static void hi_pcie_handle_dma_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
#ifdef CONFIG_HIPCIE_CTRL_DMA
	hi_pcie_handle_dma_top_int(ctrl);
	hi_pcie_handle_dma_queue_int(ctrl);
	hi_pcie_handle_dma_queue_err_int(ctrl);
#endif
}

static void hi_pcie_handle_iob_tx_axis_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	u32 status;
	static u32 regs[HI_PCIE_INT_MAX] = {
		0, IOB_TX_INT_RO_CE0, IOB_TX_INT_RO_NFE0, IOB_TX_INT_RO_FE0
	};
	static struct hi_pcie_int_desc descs[] = {
		{ BIT(0), "axis bresp with error", HI_PCIE_EVENT_INVALID },
		{ BIT(1), "axis rresp with error", HI_PCIE_EVENT_INVALID },
		{ BIT(2), "axis write tx tlp with error", HI_PCIE_EVENT_INVALID },
		{ BIT(3), "axis read tx tlp with error", HI_PCIE_EVENT_INVALID },
		{ BIT(4), "rx cpl with error", HI_PCIE_EVENT_INVALID },
		{ BIT(5), "axis write transfer is not support", HI_PCIE_EVENT_INVALID },
		{ BIT(6), "axis read transfer is not support", HI_PCIE_EVENT_INVALID },
	};

	status = pcie_ctrl_ap_iobtx_readl(ctrl, regs[level]);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0xffffffff, IOB_TX_INT_MSK0);

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_ap_iobtx_writel(ctrl, 0xFFFFFFFF, IOB_TX_INT_STATUS0);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, IOB_TX_INT_MSK0);
}

static void hi_pcie_handle_iob_tx_req_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	u32 status;
	static u32 regs[HI_PCIE_INT_MAX] = {
		0, IOB_TX_INT_RO_CE2, IOB_TX_INT_RO_NFE2, IOB_TX_INT_RO_FE2
	};

	static struct hi_pcie_int_desc descs[] = {
		{ 0xFFFF, "Tx Post Request timeout", HI_PCIE_EVENT_INVALID },
		{ 0xFFFF0000, "Tx NonPost Request timeout", HI_PCIE_EVENT_INVALID },
	};

	status = pcie_ctrl_ap_iobtx_readl(ctrl, regs[level]);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0xffffffff, IOB_TX_INT_MSK2);

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_ap_iobtx_writel(ctrl, 0xFFFFFFFF, IOB_TX_INT_STATUS2);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, IOB_TX_INT_MSK2);
}

static void hi_pcie_handle_iob_tx_data_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	u32 status;
	static u32 regs[HI_PCIE_INT_MAX] = {
		0, IOB_TX_INT_RO_CE3, IOB_TX_INT_RO_NFE3, IOB_TX_INT_RO_FE3
	};

	static struct hi_pcie_int_desc descs[] = {
		{ 0xFFFF,  "Tx Completion stuck timeout interrupt", HI_PCIE_EVENT_INVALID },
		{ BIT(16), "Tx Cfg0 Request timeour interrupt", HI_PCIE_EVENT_INVALID },
		{ BIT(20), "Tx Head buffer 1bit ecc error interrupt", HI_PCIE_EVENT_INVALID },
		{ BIT(21), "Tx Head buffer 2bit ecc error interrup", HI_PCIE_EVENT_INVALID },
		{ BIT(22), "Tx Data buffer 1bit ecc error interrup", HI_PCIE_EVENT_INVALID },
		{ BIT(23), "Tx Data buffer 2bit ecc error interrup", HI_PCIE_EVENT_INVALID },
		{ BIT(24), "Tx Data be buffer 1bit ecc error interrupt", HI_PCIE_EVENT_INVALID },
		{ BIT(25), "Tx Data be buffer 2bit ecc error interrupt", HI_PCIE_EVENT_INVALID },
	};

	status = pcie_ctrl_ap_iobtx_readl(ctrl, regs[level]);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0xffffffff, IOB_TX_INT_MSK3);

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_ap_iobtx_writel(ctrl, 0xFFFFFFFF, IOB_TX_INT_STATUS3);
	pcie_ctrl_ap_iobtx_writel(ctrl, 0x0, IOB_TX_INT_MSK3);
}

static void hi_pcie_handle_iob_tx_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	if (level == HI_PCIE_INT_NI)
		return;

	pcie_ctrl_mask_iobtx_int(ctrl, true);

	hi_pcie_handle_iob_tx_axis_int(ctrl, level);
	hi_pcie_handle_iob_tx_req_int(ctrl, level);
	hi_pcie_handle_iob_tx_data_int(ctrl, level);

	pcie_ctrl_iobtx_int_clr(ctrl);

	pcie_ctrl_mask_iobtx_int(ctrl, false);
}

static void hi_pcie_handle_iob_rx_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	u32 status;
	static u32 regs[HI_PCIE_INT_MAX] = {
		0, IOB_RX_INT_RO_CE, IOB_RX_INT_RO_NFE, IOB_RX_INT_RO_FE
	};
	static struct hi_pcie_int_desc descs[] = {
	};

	if (level == HI_PCIE_INT_NI)
		return;
	status = pcie_ctrl_ap_iobrx_readl(ctrl, regs[level]);

	pcie_ctrl_mask_iobrx_int(ctrl, true);

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_ap_iobrx_writel(ctrl, status, IOB_RX_INT_STATUS);

	pcie_ctrl_mask_iobrx_int(ctrl, false);
}

static void hi_pcie_handle_ap_int(struct hi_pcie_ctrl *ctrl, u32 status, u32 level)
{
	int i;
	static struct {
		u32 bit;
		void (*handler)(struct hi_pcie_ctrl *, u32);
	} handlers[] = {
		{ 1 << 7, hi_pcie_handle_int_int },
		{ 1 << 5, hi_pcie_handle_dma_int },
		{ 1 << 4, hi_pcie_handle_ap_top_int },
		{ 1 << 3, hi_pcie_handle_iob_tx_int },
		{ 1 << 2, hi_pcie_handle_iob_rx_int },
	};
	for (i = 0; i < ARRAY_SIZE(handlers); i++) {
		if ((status & handlers[i].bit) != 0)
			handlers[i].handler(ctrl, level);
	}
}

static void hi_pcie_handle_ap_port_int(struct hi_pcie_ctrl *ctrl, u32 status, u32 level)
{
	u32 port_id;
	unused(ctrl);
	if (level == HI_PCIE_INT_NI)
		return;

	for (port_id = 0; port_id < 16; port_id++) {
		if ((BIT(port_id) & status) != 0)
			hi_pcie_pr_i("port_id %u has level %u err hapened", port_id, level);
	}
}

static void hi_pcie_handle_tl_rx_err(struct hi_pcie_ctrl_port *port)
{
	u32 status;
	static struct hi_pcie_int_desc descs[] = {
		{ BIT(17), "rx_ur_cpl_fifo_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(16), "port_fifo_ovfl_int_4,cpl2_hidx_buf_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(15), "port_fifo_ovfl_int_3,cpl1_hidx_buf_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(14), "port_fifo_ovfl_int_2,np1_hidx_buf_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(13), "port_fifo_ovfl_int_1,np0_hidx_buf_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(12), "port_fifo_ovfl_int_0,p_hidx_buf_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(11), "port_buf_ovfl_int, BANK buf ptr ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(10), "tl rx long length error", HI_PCIE_EVENT_INVALID },
		{ BIT(9), "tl rx length short error", HI_PCIE_EVENT_INVALID },
		{ BIT(8), "cpl_dsp_port_fifo3_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(7), "cpl_dsp_port_fifo2_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(6), "cpl_dsp_port_fifo1_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(5), "cpl_dsp_port_fifo0_ovfl", HI_PCIE_EVENT_INVALID },
		{ BIT(4), "tl rx uc error", HI_PCIE_EVENT_INVALID },
		{ BIT(3), "tl rx ur error", HI_PCIE_EVENT_INVALID },
		{ BIT(2), "tl rx mal error", HI_PCIE_EVENT_INVALID },
		{ BIT(1), "tl rx ecrc error", HI_PCIE_EVENT_INVALID },
		{ BIT(0), "tl rx poison error", HI_PCIE_EVENT_INVALID },
	};

	status = pcie_ctrl_tl_readl(port, TL_RX_ERR_STATUS);

	pcie_ctrl_tl_writel(port, status, TL_RX_ERR_STATUS);

	hi_pcie_process_int(port->ctrl, port->port->id, descs, ARRAY_SIZE(descs), status);
}

static void hi_pcie_handle_tl_int(struct hi_pcie_ctrl_port *port, u32 level)
{
	u32 status0 = 0;
	u32 status1 = 0;
	u32 status2 = 0;
	static u32 regs[HI_PCIE_INT_MAX] = {
		0, TL_INT_CE_RO, TL_INT_NFE_RO, TL_INT_FE_RO
	};
	static struct hi_pcie_int_desc status0_descs[] = {
	};
	static struct hi_pcie_int_desc status1_descs[] = {
	};
	static struct hi_pcie_int_desc status2_descs[] = {
	};

	if (level > HI_PCIE_INT_NI)
		status0 = pcie_ctrl_tl_readl(port, regs[level]);

	if (status0 != 0) {
		if ((status0 | BIT(2) | BIT(12)) != 0)
			hi_pcie_handle_tl_rx_err(port);
		pcie_ctrl_mask_tl_int(port, true);
		hi_pcie_process_int(port->ctrl, port->port->id,
			status0_descs, ARRAY_SIZE(status0_descs), status0);
		pcie_ctrl_tl_writel(port, status0, TL_INT_STATUS0);
		// TOCHECK umask tl int ?;
	}

	if (status1 != 0) {
		pcie_ctrl_tl_writel(port, 0xFFFFFFFF, TL_INT_MASK1);
		hi_pcie_process_int(port->ctrl, port->port->id,
			status1_descs, ARRAY_SIZE(status1_descs), status1);
		pcie_ctrl_tl_writel(port, status1, TL_INT_STATUS1);
		pcie_ctrl_tl_writel(port, 0, TL_INT_MASK1);
	}

	if (status2 != 0) {
		pcie_ctrl_tl_writel(port, 0xFFFFFFFF, TL_INT_MASK2);
		hi_pcie_process_int(port->ctrl, port->port->id,
			status2_descs, ARRAY_SIZE(status2_descs), status2);
		pcie_ctrl_tl_writel(port, status2, TL_INT_STATUS2);
		pcie_ctrl_tl_writel(port, 0, TL_INT_MASK2);
	}
}

static void hi_pcie_handle_dl_int(struct hi_pcie_ctrl_port *port, u32 level)
{
	static u32 regs[HI_PCIE_INT_MAX] = {
		DL_NI_INT_RO, DL_CE_INT_RO, DL_NFE_INT_RO, DL_FE_INT_RO
	};
	static struct hi_pcie_int_desc descs[] = {
		{ BIT(17), "dl linkdown", HI_PCIE_EVENT_LINKDOWN },
	};
	u32 status = pcie_ctrl_dl_readl(port, regs[level]);

	pcie_ctrl_mask_dl_int(port, true);

	hi_pcie_process_int(port->ctrl, port->port->id, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_dl_writel(port, status, DL_INT_STATUS);

	pcie_ctrl_mask_dl_int(port, false);
}

static void hi_pcie_handle_mac_int(struct hi_pcie_ctrl_port *port, u32 level)
{
	static u32 regs[HI_PCIE_INT_MAX] = {
		MAC_REG_NI_INT_RO, MAC_REG_CE_INT_RO,
		MAC_REG_NFE_INT_RO, MAC_REG_FE_INT_RO
	};
	static struct hi_pcie_int_desc descs[] = {
		{BIT(1), "mac linkdown", HI_PCIE_EVENT_INVALID },
	};
	u32 status = pcie_ctrl_mac_readl(port, regs[level]);

	pcie_ctrl_mask_mac_int(port, true);

	hi_pcie_process_int(port->ctrl, port->port->id, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_mac_writel(port, status, MAC_REG_MAC_INT_STATUS);

	pcie_ctrl_mask_mac_int(port, false);
}

static void hi_pcie_handle_port_int(struct hi_pcie_ctrl_port *port, u32 status, u32 level)
{
	int i;
	static struct {
		u32 bit;
		void (*handler)(struct hi_pcie_ctrl_port *, u32);
	} handlers[] = {
		{ 1 << 0, hi_pcie_handle_tl_int },
		{ 1 << 1, hi_pcie_handle_dl_int },
		{ 1 << 2, hi_pcie_handle_mac_int },
	};
	for (i = 0; i < ARRAY_SIZE(handlers); i++) {
		if ((status & handlers[i].bit) != 0)
			handlers[i].handler(port, level);
	}
}

static void hi_pcie_handle_tl_core_int(struct hi_pcie_ctrl *ctrl, u32 level)
{
	static struct {
		u32 status;
		u32 clear;
	} regs[HI_PCIE_INT_MAX] = {
		{ TL_CORE_INT_STATUS0, TL_CORE_INT_STATUS0 },
		{ TL_CORE_INT_CE_RO0, TL_CORE_INT_STATUS0 },
		{ TL_CORE_INT_NFE_RO0, TL_CORE_INT_STATUS0 },
		{ TL_CORE_INT_FE_RO0, TL_CORE_INT_STATUS0 },
	};
	static struct hi_pcie_int_desc descs[] = {
	};
	u32 status = pcie_ctrl_tl_core_read(ctrl, regs[level].status);

	pcie_ctrl_tl_core_write(ctrl, 0xffffffff, TL_CORE_INT_MASK0);

	hi_pcie_process_int(ctrl, 0xFFFFFFFF, descs, ARRAY_SIZE(descs), status);

	pcie_ctrl_tl_core_write(ctrl, status, regs[level].clear);
	pcie_ctrl_tl_core_write(ctrl, 0, TL_CORE_INT_MASK0);
}

static void hi_pcie_handle_core_int(struct hi_pcie_ctrl *ctrl, u32 unused, u32 level)
{
	struct hi_pcie_port *port;
	u32 status;
	u32 port0_status;
	u32 port8_status;
	u32 port_status;
	static struct {
		u32 port0_reg;
		u32 port8_reg;
		u32 port0_mask;
		u32 port8_mask;
	} regs[HI_PCIE_INT_MAX] = {
		{ CORE_INT_NI_STATUS_0, CORE_INT_NI_STATUS_1, CORE_INT_NI_MSK_0, CORE_INT_NI_MSK_1 },
		{ CORE_INT_CE_STATUS_0, CORE_INT_CE_STATUS_1, CORE_INT_CE_MSK_0, CORE_INT_CE_MSK_1 },
		{ CORE_INT_NFE_STATUS_0, CORE_INT_NFE_STATUS_1, CORE_INT_NFE_MSK_0, CORE_INT_NFE_MSK_1 },
		{ CORE_INT_FE_STATUS_0, CORE_INT_FE_STATUS_1, CORE_INT_FE_MSK_0, CORE_INT_FE_MSK_1 },
	};

	port0_status = pcie_ctrl_core_gbl_readl(ctrl, regs[level].port0_reg);
	port8_status = pcie_ctrl_core_gbl_readl(ctrl, regs[level].port8_reg);
	status = port0_status;
	if (port0_status != 0)
		pcie_ctrl_core_gbl_writel(ctrl, 0xFFFFFFFF, regs[level].port0_mask);
	if (port8_status != 0)
		pcie_ctrl_core_gbl_writel(ctrl, 0xFFFFFFFF, regs[level].port8_mask);
	list_for_each_entry(port, &ctrl->pcie->ports, node) {
		if (port->port_id == 8)
			status = port8_status;
		port_status = (status >> ((port->port_id % 8) * 3)) & 0x7;
		hi_pcie_pr_d("pdx %u, status %u, port_status %u", port->port_id, status, port_status);
		if (port_status != 0)
			hi_pcie_handle_port_int(port->priv, port_status, level);
	}
	if ((port8_status & (1 << 26)) != 0)
		hi_pcie_handle_tl_core_int(ctrl, level);

	if (port0_status != 0) {
		pcie_ctrl_core_gbl_writel(ctrl, port0_status, regs[level].port0_reg);
		pcie_ctrl_core_gbl_writel(ctrl, 0xFF000000, regs[level].port0_mask);
	}
	if (port8_status != 0) {
		pcie_ctrl_core_gbl_writel(ctrl, port8_status, regs[level].port8_reg);
		pcie_ctrl_core_gbl_writel(ctrl, 0xFBFFFFFF, regs[level].port8_mask);
	}
}

void hi_pcie_handle_int(struct hi_pcie_ctrl *ctrl)
{
	u32 status;
	int i;
	static struct {
		u32 reg;
		u32 level;
		u32 mask;
		u32 mask_value;
		void (*handler)(struct hi_pcie_ctrl *, u32, u32);
	} handlers[] = {
		{ PCIE_AP_NI_STATUS, HI_PCIE_INT_NI, PCIE_AP_NI_MASK, 0x3FF, hi_pcie_handle_ap_int },
		{ PCIE_AP_CE_STATUS, HI_PCIE_INT_CE, PCIE_AP_CE_MASK, 0x3FF, hi_pcie_handle_ap_int },
		{ PCIE_AP_UNF_STATUS, HI_PCIE_INT_NFE, PCIE_AP_UNF_MASK, 0x3FF, hi_pcie_handle_ap_int },
		{ PCIE_AP_UF_STATUS, HI_PCIE_INT_FE, PCIE_AP_UF_MASK, 0x3FF, hi_pcie_handle_ap_int },
		{ PCIE_CORE_NI_STATUS, HI_PCIE_INT_NI, PCIE_CORE_NI_MASK, BIT(0), hi_pcie_handle_core_int },
		{ PCIE_CORE_CE_STATUS, HI_PCIE_INT_CE, PCIE_CORE_CE_MASK, BIT(0), hi_pcie_handle_core_int },
		{ PCIE_CORE_UNF_STATUS, HI_PCIE_INT_NFE, PCIE_CORE_UNF_MASK, BIT(0), hi_pcie_handle_core_int },
		{ PCIE_CORE_UF_STATUS, HI_PCIE_INT_FE, PCIE_CORE_UF_MASK, BIT(0), hi_pcie_handle_core_int },
		{ PCIE_CE_STATUS, HI_PCIE_INT_CE, PCIE_CE_MASK, 0xFFFF, hi_pcie_handle_ap_port_int },
		{ PCIE_UNF_STATUS, HI_PCIE_INT_NFE, PCIE_UNF_MASK, 0xFFFF, hi_pcie_handle_ap_port_int },
		{ PCIE_UF_STATUS, HI_PCIE_INT_FE, PCIE_UF_MASK, 0xFFFF, hi_pcie_handle_ap_port_int },
	};
	for (i = 0; i < ARRAY_SIZE(handlers); i++) {
		status = pcie_ctrl_ap_gbl_readl(ctrl, handlers[i].reg);
		if (status == 0)
			continue;
		pcie_ctrl_ap_gbl_writel(ctrl, handlers[i].mask_value, handlers[i].mask);
		handlers[i].handler(ctrl, status, handlers[i].level);
		pcie_ctrl_ap_gbl_writel(ctrl, status, handlers[i].reg);
		pcie_ctrl_ap_gbl_writel(ctrl, 0, handlers[i].mask);
	}
}

irqreturn_t hi_pcie_local_irq_handler(int irq, void *arg)
{
	struct hi_pcie_ctrl *ctrl = arg;
	if (atomic_read(&(ctrl->pcie->is_power_on)) == 0) {
		hi_pcie_pr_d("PCIe%u already power down, not support irq hadler", ctrl->pcie->ctrl_id);
		return IRQ_HANDLED;
	}

	hi_pcie_pr_d("++++%s[%u]++++", __func__, ctrl->pcie->ctrl_id);

	hi_pcie_handle_int(ctrl);

	if ((ctrl->pcie->mode == PCIE_MODE_RC) && (ctrl->int_type == INT_TYPE_SPI))
		hi_pcie_handle_msi_irq(&ctrl->msi);

	return IRQ_HANDLED;
}
