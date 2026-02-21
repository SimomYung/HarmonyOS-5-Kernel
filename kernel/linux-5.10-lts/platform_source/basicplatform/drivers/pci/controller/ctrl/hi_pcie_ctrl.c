/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller common logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_ctrl.h"

#include <linux/property.h>
#include <linux/pci.h>
#include <linux/irqchip/arm-gic-v3.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/iommu/mm_iommu.h>
#include <linux/delay.h>

#include "../hi_pcie_defines.h"
#include "../core/hi_pcie_common.h"

#include "../core/hi_pcie_core.h"
#include "hi_pcie_ctrl_rc.h"
#include "hi_pcie_ctrl_ep.h"
#include "hi_pcie_ctrl_regs.h"
#include "hi_pcie_ctrl_common.h"
#include "hi_pcie_ctrl_int.h"
#include "hi_pcie_ctrl_dfx.h"
#include "hi_pcie_ctrl_dma.h"

static struct hi_pcie_ctrl *
pcie_ctrl_new(struct platform_device *pdev, struct hi_pcie *pcie)
{
	struct hi_pcie_ctrl *ctrl;
	ctrl = devm_kzalloc(&pdev->dev, sizeof(struct hi_pcie_ctrl), GFP_KERNEL);
	if (!ctrl) {
		hi_pcie_pr_e("new pcie ctrl failed, no memory");
		return NULL;
	}

	ctrl->pcie = pcie;
	pcie->ctrl = ctrl;
	return ctrl;
}

static struct hi_pcie_ctrl_port *
pcie_ctrl_port_new(struct platform_device *pdev, u32 mode)
{
	struct hi_pcie_ctrl_port *ctrl_port = NULL;
	size_t size = 0;

	if (mode == PCIE_MODE_RC)
		size = sizeof(struct hi_pcie_ctrl_rc);
	else
		size = sizeof(struct hi_pcie_ctrl_ep);

	ctrl_port = devm_kzalloc(&pdev->dev, size, GFP_KERNEL);
	if (!ctrl_port) {
		hi_pcie_pr_e("new pcie ctrl port failed, no memory");
		return NULL;
	}

	return ctrl_port;
}

static int pcie_ctrl_init_ports(struct platform_device *pdev,
	struct hi_pcie *pcie)
{
	int ret;
	int i = 0;
	struct hi_pcie_port *port = NULL;
	struct hi_pcie_ctrl_port *ctrl_port = NULL;
	struct hi_pcie_ctrl *ctrl = pcie->ctrl;

	for (i = 0; i < (int)pcie->port_num; i++) {
		ctrl_port = pcie_ctrl_port_new(pdev, pcie->mode);
		if (!ctrl_port)
			return -ENOMEM;
		port = pcie_core_port_new(pdev, pcie, (u32)i, ctrl_port);
		if (!port)
			return -ENOMEM;
		ret = pcie_core_port_parse_config(port);
		if (ret != 0)
			return ret;
		ctrl_port->ctrl = ctrl;
		ctrl_port->port = port;
		pcie_core_port_add(pcie, port);
	}

	return 0;
}

int of_pci_parse_bus_range(struct device_node *node, struct resource *res);

static int pcie_ctrl_parse_int(struct platform_device *pdev,
					    	struct hi_pcie_ctrl *ctrl)
{
	int ret;
	struct resource res;
	struct device_node *its_node;
	u32 remainder;

	ret = device_property_read_u32(&pdev->dev, "int_type", &ctrl->int_type);
	if (ret != 0) {
		hi_pcie_pr_e("msi type is local default");
		ctrl->int_type = INT_TYPE_SPI;
	}
	if (ctrl->int_type != INT_TYPE_ITS)
		return 0;

	its_node = of_parse_phandle(pdev->dev.of_node, "msi-parent", 0);
	if (!its_node) {
		hi_pcie_pr_e("get its node failed");
		return -1;
	}

	ret = of_address_to_resource(its_node, 0, &res);
	if (ret) {
		hi_pcie_pr_e("translate its address failed");
		return ret;
	}

	ret = dma_set_mask(&pdev->dev, DMA_BIT_MASK(32));
	if (ret)
		hi_pcie_pr_e("set mask failed");
	if (!ctrl->pcie->bypass_smmu) {
		remainder = (res.start + GITS_TRANSLATER) % 0x1000;
		res.start = res.start + GITS_TRANSLATER - remainder;
		ctrl->its_addr = 0x40000040; // TOMODIFY mm_iommu_map(&pdev->dev, res.start, 0x1000, 0) + remainder;
	} else {
		ctrl->its_addr = res.start + GITS_TRANSLATER;
	}

	return 0;
}

static int pcie_ctrl_parse_config_version(struct platform_device *pdev,
					      struct hi_pcie_ctrl *ctrl)
{
	int ret;
	ret = device_property_read_u32(&pdev->dev, "version", &ctrl->pcie_version);
	if (ret != 0) {
		hi_pcie_pr_i("pcie version is gen3 default");
		ctrl->pcie_version = PCIE_VERSION3;
	}
	ret = device_property_read_u32(&pdev->dev, "ctrl_version", &ctrl->ctrl_version);
	if (ret != 0) {
		hi_pcie_pr_i("pcie ctrl version default v300");
		ctrl->ctrl_version = CTRL_VERISON_V300;
	}
	ret = device_property_read_u32(&pdev->dev, "ctrl_lane_spec", &ctrl->ctrl_lane_spec);
	if (ret != 0) {
		hi_pcie_pr_i("pcie ctrl lane spec default x1");
		ctrl->ctrl_lane_spec = PCIE_LANE_X1;
	}

	hi_pcie_pr_i("pcie version %u ctrl version %u lane spec %u",
			ctrl->pcie_version, ctrl->ctrl_version, ctrl->ctrl_lane_spec);
	return 0;
}

static int pcie_ctrl_parse_config(struct platform_device *pdev,
					      struct hi_pcie_ctrl *ctrl)
{
	int ret;
	struct resource *ctrl_res = NULL;
	struct resource *ecam = NULL;

	ctrl_res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "ctrl");
	if (ctrl_res == NULL) {
		hi_pcie_pr_e("Failed to get ctrl addr");
		return -1;
	}
	ctrl->pcie->ctrl_base = devm_ioremap_resource(&pdev->dev, ctrl_res);
	if (IS_ERR(ctrl->pcie->ctrl_base)) {
		hi_pcie_pr_e("Failed to get PCIeCTRL base");
		return (int)PTR_ERR(ctrl->pcie->ctrl_base);
	}

	ecam = platform_get_resource_byname(pdev, IORESOURCE_MEM, "ecam");
	if (ecam == NULL) {
		hi_pcie_pr_e("Failed to get ecam addr");
		return -1;
	}
	ctrl->pcie->ecam.base_phy = ecam->start;
	ctrl->pcie->ecam.base = devm_ioremap_resource(&pdev->dev, ecam);
	if (IS_ERR(ctrl->pcie->ecam.base)) {
		hi_pcie_pr_e("Failed to get PCIe conf base");
		return (int)PTR_ERR(ctrl->pcie->ecam.base);
	}

	ctrl->ecam_base = ctrl->pcie->ecam.base;
	ctrl->ecam_phy_base = ctrl->pcie->ecam.base_phy;

	ret = of_pci_parse_bus_range(pdev->dev.of_node, &ctrl->pcie->ecam.busr);
	if (ret != 0) {
		hi_pcie_pr_e("Failed to get PCIe bus range base");
		return ret;
	}

	ret = pcie_ctrl_parse_int(pdev, ctrl);
	if (ret != 0) {
		hi_pcie_pr_e("ctrl parse int failed");
		return ret;
	}

	pcie_ctrl_parse_config_version(pdev, ctrl);

	ret = device_property_read_u32(&pdev->dev, "dma_queue_num", &ctrl->dma_queue_num);
	if (ret != 0)
		ctrl->dma_queue_num = 1;

	ret = device_property_read_u32(&pdev->dev, "tx_preset", &ctrl->tx_preset);
	if (ret != 0)
		ctrl->tx_preset = 4;

	ret = device_property_read_u32(&pdev->dev, "mem_shutdown", &ctrl->mem_shutdown);
	if (ret != 0)
		ctrl->mem_shutdown = SRAM_TIMEING_CFG6;

	ret = device_property_read_u32(&pdev->dev, "recv_lock_time", &ctrl->recv_lock_time);
	if (ret != 0)
		ctrl->recv_lock_time = 0;

	return 0;
}

static int pcie_ctrl_register_int(struct platform_device *pdev,
					      struct hi_pcie_ctrl *ctrl)
{
	int irq;
	int ret;
#ifdef CONFIG_PCIE_HCD_LDK
	if (ctrl->pcie->udk_local_irq) {
		hi_pcie_pr_i("%s: skip ldk local_irq!", __func__);		
		return 0;
	}
#endif
	irq = platform_get_irq(pdev, INT_INDEX_LOCAL);
	hi_pcie_pr_i("ctrl local irq %d", irq);
	ret = devm_request_irq(&pdev->dev, (unsigned int)irq, hi_pcie_local_irq_handler,
			(unsigned long)IRQF_SHARED | (unsigned long)IRQF_NO_THREAD, "pcie_local", ctrl);
	if (ret != 0) {
		hi_pcie_pr_e("request local irq-%dfailed", irq);
		return ret;
	}
	ctrl->local_irq = irq;

	return 0;
}

static int pcie_ctrl_init(struct platform_device *pdev,
	struct hi_pcie *pcie)
{
	struct hi_pcie_ctrl *ctrl;
	int ret = 0;
	hi_pcie_pr_i("++%s++", __func__);
	ctrl = pcie_ctrl_new(pdev, pcie);
	if (!ctrl)
		return -ENOMEM;
	ret = pcie_ctrl_init_ports(pdev, pcie);
	if (ret != 0)
		return ret;
	ret = pcie_ctrl_parse_config(pdev, ctrl);
	if (ret != 0)
		return ret;
	ret = pcie_ctrl_register_int(pdev, ctrl);
	if (ret != 0)
		return ret;
	return 0;
}

static int pcie_ctrl_setup(struct hi_pcie *pcie)
{
	int ret = 0;
	struct hi_pcie_ctrl *ctrl = pcie->ctrl;
	hi_pcie_pr_i("++%s++", __func__);
	if (pcie->mode == PCIE_MODE_RC)
		ret = pcie_ctrl_setup_rc(ctrl);
#ifdef CONFIG_HIPCIE_CTRL_EP
	if (pcie->mode == PCIE_MODE_EP)
		ret = hi_pcie_ctrl_setup_ep(ctrl);
#endif
	return ret;
}

static int pcie_ctrl_ltssm_enable(struct hi_pcie *pcie)
{
	int ret;
	struct hi_pcie_ctrl *ctrl = pcie->ctrl;
	hi_pcie_pr_i("++%s++", __func__);

	ret = pcie_ctrl_ena_port(pcie->ctrl);

	if (pcie->mode == PCIE_MODE_EP)
		return ret;

	/* chk link up */
	ret = pcie_ctrl_chk_link_up(ctrl);
	if (ret != 0)
		pcie->err_code = PCIE_ERRCODE_LINKUP_FAILED;

	return ret;
}

static int pcie_ctrl_probe(struct hi_pcie *pcie)
{
	struct hi_pcie_ctrl *ctrl = pcie->ctrl;
	hi_pcie_pr_i("++%s++", __func__);
	return pci_host_probe(ctrl->bridge);
}

static void pcie_ctrl_remove_devices(struct hi_pcie *pcie)
{
	struct hi_pcie_ctrl *ctrl = pcie->ctrl;
	hi_pcie_pr_i("++%s++", __func__);
	pci_remove_root_bus(ctrl->bridge->bus);
}

static int pcie_ctrl_low_power(struct hi_pcie_port *port, bool enable)
{
	if (enable) {
		pcie_core_config_l0sl1(port, PCIE_ASPM_L1);
		pcie_core_config_l1ss(port, PCIE_L1SS_PM_ASPM_ALL);
	} else {
		pcie_core_config_l1ss(port, PCIE_L1SS_CLOSE);
		pcie_core_config_l0sl1(port, PCIE_ASPM_CLOSE);
	}

	return 0;
}

static int pcie_ctrl_change_lane(struct hi_pcie_port *port, u32 lane)
{
	u32 val;

	val = pcie_ctrl_mac_readl(port->priv, MAC_TXELEIDLE_TM);
	val |= REG_RCV_CFG_TXIDLE_EN;
	pcie_ctrl_mac_writel(port->priv, val, MAC_TXELEIDLE_TM);

	val = pcie_ctrl_mac_readl(port->priv, MAC_TARGET_LINK_WIDTH);
	val &= 0xffffffc0;
	val |= lane;
	pcie_ctrl_mac_writel(port->priv, val, MAC_TARGET_LINK_WIDTH);

	val = pcie_ctrl_cfg_readl(port->priv, LINK_CTRL_STATUS);
	val = val | BIT(5);
	pcie_ctrl_cfg_writel(port->priv, val, LINK_CTRL_STATUS);

	return 0;
}

static int pcie_ctrl_change_speed(struct hi_pcie_port *port, u16 speed)
{
	u16 reg;
	int ret;

	if (!port->self_dev)
		return -EINVAL;

	ret = pcie_capability_read_word(port->self_dev, PCI_EXP_LNKCTL2, &reg);
	if (ret != 0)
		return ret;

	reg &= (u16)~PCI_EXP_LNKCTL2_TLS;
	reg |= speed;
	pcie_capability_write_word(port->self_dev, PCI_EXP_LNKCTL2, reg);

	ret = pcie_capability_read_word(port->self_dev, PCI_EXP_LNKCTL, &reg);
	if (ret != 0)
		return ret;

	reg |= BIT(5);
	pcie_capability_write_word(port->self_dev, PCI_EXP_LNKCTL, reg);

	return 0;
}

static int pcie_ctrl_read_by_dma(struct hi_pcie_port *port, u32 queue,
	struct hi_pcie_dma_para *para)
{
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;
	struct hi_pcie_ctrl *ctrl = ctrl_port->ctrl;
	if (queue >= ctrl->dma_queue_num)
		return -EINVAL;
#ifdef CONFIG_HIPCIE_CTRL_DMA
	pcie_ctrl_read_from_remote(ctrl, &ctrl->dma_queues[queue], para);
	return 0;
#else
	hi_pcie_pr_e("don't support dma");
	return -1;
#endif
}

static int pcie_ctrl_write_by_dma(struct hi_pcie_port *port, u32 queue,
	struct hi_pcie_dma_para *para)
{
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;
	struct hi_pcie_ctrl *ctrl = ctrl_port->ctrl;
	if (queue >= ctrl->dma_queue_num)
		return -EINVAL;
#ifdef CONFIG_HIPCIE_CTRL_DMA
	pcie_ctrl_write_to_remote(ctrl, &ctrl->dma_queues[queue], para);
	return 0;
#else
	hi_pcie_pr_e("don't support dma");
	return -1;
#endif
}

static int pcie_ctrl_cfg_ecam_atu(struct hi_pcie_port *port)
{
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;
	struct hi_pcie_ctrl *ctrl = ctrl_port->ctrl;
	u64 tar = ((u64)ctrl->re_com->message.addr_h << 32) | ctrl->re_com->message.addr_l;
	pcie_setup_tx_atu(ctrl, 0, ctrl->ecam_phy_base, tar, ctrl->re_com->message.size);
	return 0;
}

static int pcie_ctrl_read_by_cpu(struct hi_pcie_port *port, void *buf, u32 size)
{
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;
	struct hi_pcie_ctrl *ctrl = ctrl_port->ctrl;
	int i;
	for (i = 0; i < (int)(size / sizeof(u32)); i++)
		((u32 *)buf)[i] = ((u32 *)ctrl->ecam_base)[i];
	return 0;
}

static int pcie_ctrl_write_by_cpu(struct hi_pcie_port *port, void *buf, u32 size)
{
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;
	struct hi_pcie_ctrl *ctrl = ctrl_port->ctrl;
	int i;
	for (i = 0; i < (int)size / ((int)sizeof(u32)); i++)
		((u32 *)ctrl->ecam_base)[i] = ((u32 *)buf)[i];
	return 0;
}

static int pcie_ctrl_enter_l2(struct hi_pcie_port *port)
{
	int ret;
	u32 val;

	ret = pci_set_power_state(port->target_dev, PCI_D3hot);
	if (ret != 0) {
		hi_pcie_pr_e("failed to set power to d3hot");
		return ret;
	}

	msleep(100);

	val = pcie_ctrl_tl_readl(port->priv, TL_PM_UC_CTRL);
	val |= TL_UC_TURNOFF_MSG_REQ;
	pcie_ctrl_tl_writel(port->priv, val, TL_PM_UC_CTRL);

	msleep(100);

	val &= ~TL_UC_TURNOFF_MSG_REQ;
	pcie_ctrl_tl_writel(port->priv, val, TL_PM_UC_CTRL);

	return 0;
}

static int pcie_ctrl_exit_l2(struct hi_pcie_port *port)
{
	u32 val;

	val = pcie_ctrl_tl_readl(port->priv, TL_ENTER_L0_CTRL);
	val |= TL_ENTER_L0_EN;
	pcie_ctrl_tl_writel(port->priv, val, TL_ENTER_L0_CTRL);

	return 0;
}

static u32 pcie_ctrl_read_conf(struct hi_pcie_port *port,
						 u32 offset, u32 size)
{
	u32 val = 0;
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;
	if (size == PCIE_SIZE_DWORD)
		val = pcie_ctrl_cfg_readl(ctrl_port, offset);
	else if (size == PCIE_SIZE_WORD)
		val = pcie_ctrl_cfg_readw(ctrl_port, offset);
	else if (size == PCIE_SIZE_BYTE)
		val = pcie_ctrl_cfg_readb(ctrl_port, offset);
	return val;
}

static int pcie_ctrl_write_conf(struct hi_pcie_port *port,
					    u32 offset, u32 size, u32 val)
{
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;
	if (size == PCIE_SIZE_DWORD)
		pcie_ctrl_cfg_writel(ctrl_port, val, offset);
	else if (size == PCIE_SIZE_WORD)
		pcie_ctrl_cfg_writew(ctrl_port, val, offset);
	else if (size == PCIE_SIZE_BYTE)
		pcie_ctrl_cfg_writeb(ctrl_port, val, offset);
	return 0;
}

#define PCIE_SHUTDOWN_TIMEOUT 1000
static int pcie_ctrl_shutdown_prepare(struct hi_pcie_port *port)
{
	u32 val;
	int index = 0;
	struct hi_pcie_ctrl_port *ctrl_port = port->priv;

	val = pcie_ctrl_tl_readl(ctrl_port, TL_INT_STATUS1);
	val = val | BIT(12);
	pcie_ctrl_tl_writel(ctrl_port, val, TL_INT_STATUS1);

	val = pcie_ctrl_cfg_readl(ctrl_port, PME_STATUS);
	val = val | BIT(8);
	pcie_ctrl_cfg_writel(ctrl_port, val, PME_STATUS);

	val = pcie_ctrl_tl_readl(ctrl_port, TL_PM_UC_CTRL);
	val = val | BIT(25);
	pcie_ctrl_tl_writel(ctrl_port, val, TL_PM_UC_CTRL);

	do {
		if (index >= PCIE_SHUTDOWN_TIMEOUT) {
			hi_pcie_pr_e("Failed to get PME ACK");
			return -1;
		}

		val = pcie_ctrl_tl_readl(ctrl_port, TL_INT_STATUS1);
		val = val & BIT(12);
		index++;
		udelay(10);
	} while (val != BIT(12));

	hi_pcie_pr_e("Get PME ACK succ");
	val = val | BIT(12);
	pcie_ctrl_tl_writel(ctrl_port, val, TL_INT_STATUS1);
	return 0;
}

static struct hi_pcie_ctrl_ops ctrl_ops = {
	.init			= pcie_ctrl_init,
	.setup			= pcie_ctrl_setup,
	.ltssm_enable		= pcie_ctrl_ltssm_enable,
	.probe			= pcie_ctrl_probe,
	.remove_devices		= pcie_ctrl_remove_devices,
	.low_power		= pcie_ctrl_low_power,
	.save_state		= NULL,
	.restore_state		= NULL,
	.reset_port		= pcie_ctrl_reset_port,
	.unreset_port		= pcie_ctrl_unreset_port,
	.change_lane		= pcie_ctrl_change_lane,
	.change_speed		= pcie_ctrl_change_speed,
	.enter_l2		= pcie_ctrl_enter_l2,
	.exit_l2		= pcie_ctrl_exit_l2,
	.read_conf		= pcie_ctrl_read_conf,
	.write_conf		= pcie_ctrl_write_conf,
	.shutdown_prepare	= pcie_ctrl_shutdown_prepare,
};

static struct hi_pcie_transfer_ops transfer_ops = {
	.read_by_dma		= pcie_ctrl_read_by_dma,
	.write_by_dma		= pcie_ctrl_write_by_dma,
	.cfg_ecam_atu		= pcie_ctrl_cfg_ecam_atu,
	.read_by_cpu		= pcie_ctrl_read_by_cpu,
	.write_by_cpu		= pcie_ctrl_write_by_cpu,
	.setup_inbound		= pcie_ctrl_setup_inbound,
	.setup_outbound		= pcie_ctrl_setup_outbound,
};

void hi_pcie_ctrl_register(struct hi_pcie *pcie)
{
	pcie->ctrl_ops = &ctrl_ops;
	pcie->transfer_ops = &transfer_ops;
}
