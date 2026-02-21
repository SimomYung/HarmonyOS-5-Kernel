/*
 * hiusbc_core.c -- Driver for Hisilicon USB Controller.
 *
 * Copyright (c) 2019 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/pinctrl/consumer.h>
#include <linux/property.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/platform_drivers/usb/hiusb.h>
#ifdef CONFIG_SYS_PM_ULSR
#include <platform_include/cee/linux/ulsr.h>
#endif
#include "hiusbc_debug.h"
#include "hiusbc_drd.h"
#include "hiusbc_debugfs.h"
#include "hiusbc_gadget.h"
#include "hiusbc_host.h"
#include "hiusbc_core.h"

static void hiusbc_set_vbus_quirk(struct hiusbc *hiusbc)
{
	u32 reg;
	hiusbc_dbg(HIUSBC_DEBUG_ERR, "hiusbc->set_vbus_quirk %d\n", hiusbc->set_vbus_quirk);

	if(hiusbc->set_vbus_quirk) {
		reg = hiusbc_readl(hiusbc->com_regs,
			MPI_APP_COM_REG_U2_VBUS_SEL_OFFSET);
		reg |= hiusbc_u2_vbus_sel(HIUSBC_U2_VBUS_FORCE);
		hiusbc_writel(reg, hiusbc->com_regs,
			MPI_APP_COM_REG_U2_VBUS_SEL_OFFSET);

		reg = hiusbc_readl(hiusbc->com_regs,
			MPI_APP_COM_REG_U2_VBUS_FCE_EN_OFFSET);
		reg |= MPI_APP_COM_REG_U2_VBUS_FCE_EN_MASK;
		hiusbc_writel(reg, hiusbc->com_regs,
			MPI_APP_COM_REG_U2_VBUS_FCE_EN_OFFSET);
	}
}

static bool hiusbc_is_u2_only(struct hiusbc *hiusbc)
{
	int ret;
	u32 portid;
	struct device *parent = hiusbc->dev->parent;

	if (parent == NULL)
		return false;

	ret = device_property_read_u32(parent, "portid", &portid);
	if (ret != 0) {
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "get portid failed\n");
		return false;
	}
	hiusbc_dbg(HIUSBC_DEBUG_ERR, "%s:portid %d\n", __func__, portid);
	return hiusb_check_is_u2_only_by_portid(portid);
}

static int hiusbc_init_phy(struct hiusbc *hiusbc)
{
	u32 reg;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	if (hiusbc->max_speed < USB_SPEED_SUPER || hiusbc_is_u2_only(hiusbc)) {
		/* turn on U2 phy only. */
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "turn on U2 phy only\n");
		reg = hiusbc_readl(hiusbc->com_regs, MPI_APP_COM_REG_PHY_SEL_MODE_OFFSET);
		reg &= ~MPI_APP_COM_REG_PHY_SEL_MODE_MASK;
		reg |= hiusbc_phy_set(HIUSBC_PHY_SET_U2_ONLY);
		hiusbc_writel(reg, hiusbc->com_regs, MPI_APP_COM_REG_PHY_SEL_MODE_OFFSET);

		reg = hiusbc_readl(hiusbc->com_regs, MPI_APP_COM_REG_U3_CONTINUE_MODE_OFFSET);
		reg |= MPI_APP_COM_REG_USBC_U2_EN_MASK;
		hiusbc_writel(reg, hiusbc->com_regs, MPI_APP_COM_REG_U3_CONTINUE_MODE_OFFSET);
	}

	if (hiusbc->is_fpga == 0)
		return 0;

	hiusbc_dbg(HIUSBC_DEBUG_ERR, "Force phy don't go to low power\n");
	// Force phy don't go to low power. Because FPGA can't switch clk.
	// 0x28838 = 0
	hiusbc_writel(0, hiusbc->u2_piu_regs, MPI_U2_PM_UIU_REG_CSR_HST_U2_U3_PHY_LP_EN_OFFSET);
	// 0x28860 = 0
	hiusbc_writel(0, hiusbc->u2_piu_regs, MPI_U2_PM_UIU_REG_CSR_HST_POWERED_OFF_PHY_LP_EN_OFFSET);
	// 0x28a04 = 0
	hiusbc_writel(0, hiusbc->u2_piu_regs, MPI_U2_PM_UIU_REG_CSR_DEV_LP_CFG_OFFSET);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return 0;
}

static void hiusbc_init_clk(struct hiusbc *hiusbc)
{
	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	if (hiusbc->clk_delta_pn != 0) {
		hiusbc_writel(hiusbc->clk_delta_pn, hiusbc->com_regs,
			MPI_APP_COM_REG_DELTA_CFG_PN_OFFSET);
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "set hiusbc clk_delta_pn %u\n", hiusbc->clk_delta_pn);
	}

	if (hiusbc->clk_delta_cn != 0) {
		hiusbc_writel(hiusbc->clk_delta_cn, hiusbc->com_regs,
			MPI_APP_COM_REG_DELTA_CFG_CN_OFFSET);
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "set hiusbc clk_delta_cn %u\n", hiusbc->clk_delta_cn);
	}

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");
}

int hiusbc_get_irq(struct hiusbc *hiusbc)
{
	struct platform_device *hiusbc_pdev = to_platform_device(hiusbc->dev);
	int irq;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");
	irq = platform_get_irq(hiusbc_pdev, 0);
	if (irq > 0)
		goto out;

	if (irq != -EPROBE_DEFER)
		hiusbc_dbg(HIUSBC_DEBUG_SYS, "missing irq\n");

	if (irq == 0)
		irq = -EINVAL;

out:
	hiusbc_dbg(HIUSBC_DEBUG_SYS, "- irq = %d\n", irq);
	return irq;
}

int hiusbc_reset_controller(struct hiusbc *hiusbc)
{
	u32 reg;
	u32 drd_mode_backup;
	u32 timeout = 1000000;
	int ret = 0;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	if (hiusbc->is_fpga) {
		hiusbc_writel(0xff, hiusbc->u2_piu_regs,
			MPI_U2_PM_UIU_REG_CSR_ULPI_REG_RW_TIMEOUT_V_OFFSET);
		reg = hiusbc_readl(hiusbc->u2_piu_regs,
			MPI_U2_PM_UIU_REG_CSR_ULPI_REG_RW_TIMEOUT_V_OFFSET);
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "tx cmd timeout 0x%x  0x%x\n",
			MPI_U2_PM_UIU_REG_CSR_ULPI_REG_RW_TIMEOUT_V_OFFSET,
			reg);
	}

	/* we need to switch to device mode before dev_rst. */
	drd_mode_backup = hiusbc_readl(hiusbc->com_regs,
	MPI_APP_COM_REG_DRD_MODE_OFFSET);
	if (hiusbc_get_drd_mode(drd_mode_backup) == HIUSBC_DRD_MODE_HOST)
		hiusbc_writel(
			hiusbc_drd_mode(HIUSBC_DRD_MODE_DEVICE),
			hiusbc->com_regs, MPI_APP_COM_REG_DRD_MODE_OFFSET);

	udelay(100);
	reg = hiusbc_readl(hiusbc->dev_regs,
		MPI_APP_DEVICE_REG_DEV_RST_OFFSET);
	reg |= MPI_APP_DEVICE_REG_DEV_RST_MASK;
	hiusbc_writel(reg, hiusbc->dev_regs,
		MPI_APP_DEVICE_REG_DEV_RST_OFFSET);
	udelay(200);

	do {
		reg = hiusbc_readl(hiusbc->dev_regs,
			MPI_APP_DEVICE_REG_DEV_RST_OFFSET);
		reg &= MPI_APP_DEVICE_REG_DEV_RST_MASK;
		udelay(1);
	} while (reg && --timeout);

	if (reg != 0 && timeout == 0) {
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "phy done status 0x%x\n",
								hiusbc_readl(hiusbc->com_regs,
								MPI_APP_COM_REG_PHY_DONE_STATUS_OFFSET));
		dev_err(hiusbc->dev, "Timeout to reset controller!\n");
		ret = -ETIMEDOUT;
	}

	if (hiusbc_get_drd_mode(drd_mode_backup) == HIUSBC_DRD_MODE_HOST)
		hiusbc_writel(drd_mode_backup,
			hiusbc->com_regs, MPI_APP_COM_REG_DRD_MODE_OFFSET);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return ret;
}

void hiusbc_set_speed(struct hiusbc *hiusbc,
		enum usb_device_speed speed)
{
	bool u3_en = true;
	bool hs_en = true;
	bool ssp_en = true;
	u32 reg;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+ speed = %d\n", speed);

	switch (speed) {
	case USB_SPEED_LOW:
	case USB_SPEED_FULL:
		/* fall-throung */
		hs_en = false;
	case USB_SPEED_HIGH:
		u3_en = false;
		break;

	case USB_SPEED_SUPER:
		ssp_en = false;
		break;
	case USB_SPEED_SUPER_PLUS:
		/* fall-through */
		break;

	default:
		dev_err(hiusbc->dev, "invalid speed = %d.\n", speed);
	}

	reg = hiusbc_readl(hiusbc->com_regs,
		MPI_APP_COM_REG_U3_CONTINUE_MODE_OFFSET);
	if (u3_en)
		reg |= MPI_APP_COM_REG_USBC_U3_EN_MASK;
	else
		reg &= ~MPI_APP_COM_REG_USBC_U3_EN_MASK;
	hiusbc_writel(reg, hiusbc->com_regs,
		MPI_APP_COM_REG_U3_CONTINUE_MODE_OFFSET);

	reg = hiusbc_readl(hiusbc->u2_piu_regs,
		MPI_U2_PM_UIU_REG_CSR_DEV_PRTRST_HS_HANDSHAKE_EN_OFFSET);
	if (hs_en)
		reg |= MPI_U2_PM_UIU_REG_CSR_DEV_PRTRST_HS_HANDSHAKE_EN_MASK;
	else
		reg &= ~MPI_U2_PM_UIU_REG_CSR_DEV_PRTRST_HS_HANDSHAKE_EN_MASK;
	hiusbc_writel(reg, hiusbc->u2_piu_regs,
		MPI_U2_PM_UIU_REG_CSR_DEV_PRTRST_HS_HANDSHAKE_EN_OFFSET);

	reg = hiusbc_readl(hiusbc->u3_link_regs,
		MPI_U3_LINK_REG_CSR_TARGET_SPEED_OFFSET);
	reg &= ~MPI_U3_LINK_REG_CSR_TARGET_SPEED_MASK;
	if (ssp_en)
		reg |= (1 << MPI_U3_LINK_REG_CSR_TARGET_SPEED_SHIFT);
	hiusbc_writel(reg, hiusbc->u3_link_regs,
		MPI_U3_LINK_REG_CSR_TARGET_SPEED_OFFSET);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");
}

static int hiusbc_init_dr_mode(struct hiusbc *hiusbc)
{
	int ret = 0;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	hiusbc->current_dr_mode = HIUSBC_DR_MODE_UNKNOWN;

	switch (hiusbc->support_dr_mode) {
	case HIUSBC_DR_MODE_HOST:
	case HIUSBC_DR_MODE_DEVICE:
		hiusbc_set_mode(hiusbc, hiusbc->support_dr_mode);
		break;

	case HIUSBC_DR_MODE_BOTH:
		ret = hiusbc_drd_init(hiusbc);
		break;

	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static void hiusbc_exit_current_mode(struct hiusbc *hiusbc)
{
	unsigned long flags;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	switch (hiusbc->current_dr_mode) {
	case HIUSBC_DR_MODE_DEVICE:
		hiusbc_gadget_exit(hiusbc);
		break;
	case HIUSBC_DR_MODE_HOST:
		hiusbc_host_exit(hiusbc);
		break;
	default:
		break;
	}

	spin_lock_irqsave(&hiusbc->lock, flags);
	hiusbc->current_dr_mode = HIUSBC_DR_MODE_UNKNOWN;
	spin_unlock_irqrestore(&hiusbc->lock, flags);
}

static void hiusbc_set_txdeemph(struct hiusbc *hiusbc)
{
	u32 reg;
	u8 lane;

	if (!hiusbc->combophy_regs || hiusbc->gen2_txdeemph_reg == 0 || hiusbc->max_speed < USB_SPEED_SUPER)
		return;

	reg = readl(hiusbc->combophy_regs + HIUSBC_U3PHY_MODE);
	lane = (reg & BIT(3)) != 0 ? 1 : 0;
	pr_err("combophy phy mode reg 0x%x lane 0x%x\n", reg, lane);

	reg = readl(hiusbc->combophy_regs + hiusbc->gen2_txdeemph_reg);
	reg &= BIT(8);
	pr_err("ffe v48 0x%x\n", reg);
	if (reg) {
		if (lane == 0)
			writel(HIUSBC_U3PHY_FFE_V48_LANE0, u3_phy_ffe_sig_reg(hiusbc->combophy_regs, lane));
		else
			writel(HIUSBC_U3PHY_FFE_V48_LANE1, u3_phy_ffe_sig_reg(hiusbc->combophy_regs, lane));

		writel(HIUSBC_U3PHY_FFE_V48, u3_phy_txdeemphctrl(hiusbc->combophy_regs, lane));
		hiusbc_writel(hiusbc->gen2_txdeemph_v48_cp15, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH0_OFFSET);
		hiusbc_writel(hiusbc->gen2_txdeemph_v48_cp13, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH1_OFFSET);
		hiusbc_writel(hiusbc->gen2_txdeemph_v48_cp14, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH2_OFFSET);
		hiusbc_writel(hiusbc->gen2_txdeemph_v48_cp16, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH3_OFFSET);
	} else {
		if (lane == 0)
			writel(HIUSBC_U3PHY_FFE_V36_LANE0, u3_phy_ffe_sig_reg(hiusbc->combophy_regs, lane));
		else
			writel(HIUSBC_U3PHY_FFE_V36_LANE1, u3_phy_ffe_sig_reg(hiusbc->combophy_regs, lane));

		writel(HIUSBC_U3PHY_FFE_V36, u3_phy_txdeemphctrl(hiusbc->combophy_regs, lane));
		hiusbc_writel(hiusbc->gen2_txdeemph_v36_cp15, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH0_OFFSET);
		hiusbc_writel(hiusbc->gen2_txdeemph_v36_cp13, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH1_OFFSET);
		hiusbc_writel(hiusbc->gen2_txdeemph_v36_cp14, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH2_OFFSET);
		hiusbc_writel(hiusbc->gen2_txdeemph_v36_cp16, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_G2_TXDEEEMPH3_OFFSET);
	}
}

static void hiusbc_config_ssp(struct hiusbc *hiusbc)
{
	if (hiusbc->max_speed < USB_SPEED_SUPER_PLUS)
		return;

	dev_info(hiusbc->dev, "config hiusbc for super speed plus!\n");
	hiusbc_writel(HIUSBC_GEN2_SPEED, hiusbc->u3_link_regs,
		MPI_U3_LINK_REG_CSR_TARGET_SPEED_OFFSET);
	hiusbc_writel(HIUSBC_SCD_LOGIC0_SPACE, hiusbc->u3_link_regs,
		MPI_U3_LINK_REG_CSR_SCD_LOGIC0_SPACE_LENGTH_OFFSET);
	hiusbc_writel(HIUSBC_SCD_LOGIC1_SPACE, hiusbc->u3_link_regs,
		MPI_U3_LINK_REG_CSR_SCD_LOGIC1_SPACE_LENGTH_OFFSET);
	hiusbc_writel(HIUSBC_CSR_RXEQ_GEN, hiusbc->u3_link_regs,
		MPI_U3_LINK_REG_CSR_PL_2_RXEQ_GEN_OFFSET);
	hiusbc_writel(HIUSBC_CSR_REPEAT_MIN, hiusbc->u3_link_regs,
		MPI_U3_LINK_REG_CSR_SCD_LOGIC1_REPEAT_MIN_OFFSET);
}

static void hiusbc_host_reg_init(struct hiusbc *hiusbc)
{
	u32 reg;

	hiusbc_set_speed(hiusbc, hiusbc->max_speed);

	/* config trpu block mode */
	reg = hiusbc_readl(hiusbc->host_regs,
		MPI_APP_HOST_REG_TRPU_HOST_CTRL_MODE_OFFSET);
	reg &= ~MPI_APP_HOST_REG_TRPU_HOST_BLOCK_MODE_MASK;

	if (hiusbc->host_u3_low_perf_mode)
		reg |= TRPU_HOST_U2PERF_HIGH_MODE;
	else
		reg |= (TRPU_HOST_U3PERF_HIGH_MODE | TRPU_HOST_U2PERF_HIGH_MODE);

	dev_info(hiusbc->dev, "TRPU_HOST_CTRL_MODE value 0x%x\n", reg);
	hiusbc_writel(reg, hiusbc->host_regs,
		MPI_APP_HOST_REG_TRPU_HOST_CTRL_MODE_OFFSET);

	/* config doorbell interval */
	hiusbc_writel(HIUSBC_DOORBELL_INTERVAL, hiusbc->host_regs,
		MPI_APP_HOST_REG_TRPU_HOST_DB_ITVL_OFFSET);

	hiusbc_writel(HIUSBC_PWM_MIN_GEN2, hiusbc->u3_link_regs,
			MPI_U3_LINK_REG_CSR_LBPM_PWM_MIN_GEN2_OFFSET);
	/* config host perf mode */
	hiusbc_writel(HIUSBC_TRPU_HOST_U2SEP_MBS, hiusbc->host_regs,
		MPI_APP_HOST_REG_TRPU_HOST_SEP_MBS_OFFSET);

	if (hiusbc->ltssm_dump) {
		dev_err(hiusbc->dev, "set hiusbc dfx u3link and u3pl!\n");
		reg = hiusbc_readl(hiusbc->u3_link_regs, MPI_U3_LINK_REG_CSR_DFX_STATE_CFG_OFFSET);
		reg |= MPI_U3_LINK_REG_CSR_DFX_STATE_WR_EN_MASK;
		hiusbc_writel(reg, hiusbc->u3_link_regs, MPI_U3_LINK_REG_CSR_DFX_STATE_CFG_OFFSET);
	}
}

static void hiusbc_enter_desired_mode(struct hiusbc *hiusbc)
{
	unsigned long flags;
	int ret = 0;
	u32 reg;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	spin_lock_irqsave(&hiusbc->lock, flags);
	hiusbc->current_dr_mode = hiusbc->desired_dr_mode;
	spin_unlock_irqrestore(&hiusbc->lock, flags);

	hiusbc_init_phy(hiusbc);
	hiusbc_init_clk(hiusbc);
	hiusbc_set_vbus_quirk(hiusbc);
	hiusbc_set_txdeemph(hiusbc);
	hiusbc_config_ssp(hiusbc);

	switch (hiusbc->desired_dr_mode) {
	case HIUSBC_DR_MODE_DEVICE:
		ret = hiusbc_run_stop(hiusbc, 0);
		if (ret) {
			dev_err(hiusbc->dev, "failed to stop controller!\n");
			break;
		}

		/* switch clk */
		reg = hiusbc_readl(hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);
		reg |= (1 << USBC_HUB_REG_RST_SOFT_TRPU_DEV_N_SHIFT);
		hiusbc_writel(reg, hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);

		ret = hiusbc_reset_controller(hiusbc);
		if (ret) {
			dev_err(hiusbc->dev, "failed to reset controlle!\n");
			break;
		}

		hiusbc_writel(hiusbc_drd_mode(HIUSBC_DRD_MODE_DEVICE),
			hiusbc->com_regs, MPI_APP_COM_REG_DRD_MODE_OFFSET);

		reg = hiusbc_readl(hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);
		reg &= ~USBC_HUB_REG_RST_SOFT_TRPU_HOST_N_MASK;
		hiusbc_writel(reg, hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);

		ret = hiusbc_gadget_init(hiusbc);
		if (ret)
			dev_err(hiusbc->dev, "failed to init device mode!\n");
		break;
	case HIUSBC_DR_MODE_HOST:
		ret = hiusbc_run_stop(hiusbc, 0);
		if (ret < 0) {
			dev_err(hiusbc->dev, "failed to stop controller!\n");
			break;
		}

		/* switch clk */
		reg = hiusbc_readl(hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);
		reg |= (1 << USBC_HUB_REG_RST_SOFT_TRPU_HOST_N_SHIFT);
		hiusbc_writel(reg, hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);

		hiusbc_host_reg_init(hiusbc);
		ret = hiusbc_reset_controller(hiusbc);
		if (ret) {
			dev_err(hiusbc->dev, "failed to reset controller!\n");
			break;
		}

		hiusbc_writel(hiusbc_drd_mode(HIUSBC_DRD_MODE_HOST),
			hiusbc->com_regs, MPI_APP_COM_REG_DRD_MODE_OFFSET);

		reg = hiusbc_readl(hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);
		reg &= ~USBC_HUB_REG_RST_SOFT_TRPU_DEV_N_MASK;
		hiusbc_writel(reg, hiusbc->hub_regs,
			USBC_HUB_REG_USBC_SOFT_RST_OFFSET);

		ret = hiusbc_host_init(hiusbc);
		if (ret)
			dev_err(hiusbc->dev, "failed to init host mode!\n");

		break;
	default:
		break;
	}

	if (ret) {
		spin_lock_irqsave(&hiusbc->lock, flags);
		hiusbc->current_dr_mode = HIUSBC_DR_MODE_UNKNOWN;
		spin_unlock_irqrestore(&hiusbc->lock, flags);
		dev_err(hiusbc->dev, "Set current_dr_mode to _UNKNOWN!\n");
	}
}

static void hiusbc_exit_dr_mode(struct hiusbc *hiusbc)
{
	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	switch (hiusbc->support_dr_mode) {
	case HIUSBC_DR_MODE_HOST:
	case HIUSBC_DR_MODE_DEVICE:
		hiusbc_exit_current_mode(hiusbc);
		break;
	case HIUSBC_DR_MODE_BOTH:
		hiusbc_drd_exit(hiusbc);
		break;
	default:
		break;
	}
}

void hiusbc_set_mode(struct hiusbc *hiusbc,
			enum hiusbc_dr_mode desired_mode)
{
	enum hiusbc_dr_mode current_mode = hiusbc->current_dr_mode;
	unsigned long flags;

	hiusbc_dbg(HIUSBC_DEBUG_ERR, "current_mode = %d, desired_mode = %d\n",
		current_mode, desired_mode);

	if (desired_mode == current_mode ||
		desired_mode == HIUSBC_DR_MODE_BOTH)
		return;

	spin_lock_irqsave(&hiusbc->lock, flags);
	hiusbc->desired_dr_mode = desired_mode;
	spin_unlock_irqrestore(&hiusbc->lock, flags);

	hiusbc_exit_current_mode(hiusbc);

	hiusbc_enter_desired_mode(hiusbc);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");
}

static void hiusbc_gen2_txdeemph_properties(struct hiusbc *hiusbc)
{
	struct device *dev = hiusbc->dev;
	struct device_node *np = NULL;
	u32 reg_addr = 0;
	u32 reg_size = 0;
	int ret;

	/*
	 * map USB Combophy region
	 */
	np = of_get_child_by_name(dev->of_node, "usbc_combophy");
	if (!np)
		return;

	ret = of_property_read_u32(np, "combophy_addr", &reg_addr);
	if (ret !=0) {
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "combophy_addr not set\n");
		return;
	}
	ret = of_property_read_u32(np, "combophy_size", &reg_size);
	if (ret !=0) {
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "combophy_size not set\n");
		return;
	}

	pr_info("get combophy_addr 0x%x, combophy_size 0x%x\n", reg_addr, reg_size);
	hiusbc->combophy_regs =  ioremap(reg_addr, reg_size);
	if (!hiusbc->combophy_regs) {
		pr_err("get usbc_combophy iomap failed\n");
		return;
	}

	if (of_property_read_u32(np, "gen2_txdeemph_reg", &hiusbc->gen2_txdeemph_reg)) {
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "gen2_txdeemph_reg not set\n");
		hiusbc->gen2_txdeemph_reg = 0;
		return;
	}

	/*
	 * The default value of v48 depends on the combophy calibration result,
	 * if v48 is 1,using v48 arrays, else using v36 arrays
	 */
	hiusbc->gen2_txdeemph_v48_cp15 = 0x7D04;
	hiusbc->gen2_txdeemph_v48_cp13 = 0xF03;
	hiusbc->gen2_txdeemph_v48_cp14 = 0x3F00;
	hiusbc->gen2_txdeemph_v48_cp16 = 0xFC0;

	hiusbc->gen2_txdeemph_v36_cp15 = 0x59C3;
	hiusbc->gen2_txdeemph_v36_cp13 = 0xB42;
	hiusbc->gen2_txdeemph_v36_cp14 = 0x2B40;
	hiusbc->gen2_txdeemph_v36_cp16 = 0xBC0;
}

static void hiusbc_get_properties(struct hiusbc *hiusbc)
{
	int ret;
	struct device *dev = hiusbc->dev;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	hiusbc->support_dr_mode = HIUSBC_DR_MODE_BOTH;

	hiusbc_gen2_txdeemph_properties(hiusbc);

	if (device_property_read_bool(dev, "linux,sysdev_is_parent"))
		hiusbc->sysdev = dev->parent;
	else
		hiusbc->sysdev = dev;

	if (device_property_read_bool(dev, "set_vbus_quirk"))
		hiusbc->set_vbus_quirk = 1;
	else
		hiusbc->set_vbus_quirk = 0;

	if (device_property_read_bool(dev, "is-fpga"))
		hiusbc->is_fpga = 1;
	else
		hiusbc->is_fpga = 0;

	if (device_property_read_bool(dev, "host_abort_cmd_quirk"))
		hiusbc->host_abort_cmd_quirk = 1;
	else
		hiusbc->host_abort_cmd_quirk = 0;

	if (device_property_read_bool(dev, "host_ca_timeout_quirk"))
		hiusbc->host_ca_timeout_quirk = 1;
	else
		hiusbc->host_ca_timeout_quirk = 0;

	if (device_property_read_bool(dev, "host_u3_low_perf_mode"))
		hiusbc->host_u3_low_perf_mode = 1;
	else
		hiusbc->host_u3_low_perf_mode = 0;

	pr_info("hiusbc abort cmd quirks %d, ca quirks %d, host_u3_low_perf_mode %d\n",
		hiusbc->host_abort_cmd_quirk, hiusbc->host_ca_timeout_quirk, hiusbc->host_u3_low_perf_mode);

	ret = device_property_read_u32(hiusbc->dev, "clk_delta_pn", &hiusbc->clk_delta_pn);
	if (ret !=0) {
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "clk_delta_pn use default\n");
		hiusbc->clk_delta_pn = 0;
	}

	ret = device_property_read_u32(hiusbc->dev, "clk_delta_cn", &hiusbc->clk_delta_cn);
	if (ret !=0) {
		hiusbc_dbg(HIUSBC_DEBUG_ERR, "clk_delta_cn use default\n");
		hiusbc->clk_delta_cn = 0;
	}

	if (device_property_read_u32(hiusbc->dev, "sg_supported", &hiusbc->sg_supported))
		hiusbc->sg_supported = 0;

	hiusbc->max_speed = usb_get_maximum_speed(dev);
	hiusbc_dbg(HIUSBC_DEBUG_SYS, "max_speed = %d\n", hiusbc->max_speed);
	if (hiusbc->max_speed == USB_SPEED_UNKNOWN)
		hiusbc->max_speed = USB_SPEED_SUPER;

	hiusbc->hird_suspend = 12;

	hiusbc->usb2_dev_lpm_capable = 0;
	hiusbc->usb3_dev_lpm_capable = 0;
	hiusbc->usb3_dev_lpm_u1_accept = 1;
	hiusbc->usb3_dev_lpm_u2_accept = 1;
	hiusbc->usb3_dev_lpm_u1_initiate = 1;
	hiusbc->usb3_dev_lpm_u2_initiate = 1;
	hiusbc->usb3_dev_lpm_ux_exit = 0;
	hiusbc->dis_gadget_u2_lpm_bulk_always_idle = 0;

	hiusbc->usb3_host_lpm_capable = 0;
	hiusbc->usb2_host_lpm_capable = 0;

#ifdef CONFIG_HIUSBC_EDA_TEST_CASE
	hiusbc->force_maxp = 1;
#endif
}

static int hiusbc_init_regs(struct hiusbc *hiusbc, struct resource *res)
{
	void __iomem *regs = NULL;
	int ret = 0;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	res->start += HIUSBC_DEVICE_MODE_REG_BASE;

	regs = devm_ioremap_resource(hiusbc->dev, res);
	if (IS_ERR(regs)) {
		pr_err("hiusbc ioremap fail.\n");
		ret = PTR_ERR(regs);
		goto exit;
	}

	hiusbc->regs = regs;
	hiusbc->regs_size = resource_size(res);

	hiusbc->host_regs = regs + MPI_APP_HOST_REG_BASE_ADDR;
	hiusbc->dev_regs = regs + MPI_APP_DEVICE_REG_BASE_ADDR;
	hiusbc->com_regs = regs + MPI_APP_COM_REG_BASE_ADDR;
	hiusbc->hub_regs = regs + USBC_HUB_REG_BASE_ADDR;

	hiusbc->lmi_regs = regs + MPI_MAU_LMI_REG_BASE_ADDR;

	hiusbc->u2_pl_regs = regs + MPI_U2_PL_REG_BASE_ADDR;
	hiusbc->u2_piu_regs = regs + MPI_U2_PM_UIU_REG_BASE_ADDR;
	hiusbc->u3_pl_regs = regs + MPI_U3PL_REG_BASE_ADDR;
	hiusbc->u3_link_regs = regs + MPI_U3_LINK_REG_BASE_ADDR;

	hiusbc_dbg(HIUSBC_DEBUG_ERR,
		"base add = 0x%pK, dev_regs = 0x%pK, u3_link_regs = 0x%pK\n",
		regs, hiusbc->dev_regs,
		hiusbc->u3_link_regs);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return ret;

exit:
	res->start -= HIUSBC_DEVICE_MODE_REG_BASE;
	return ret;
}

static void hiusbc_clear_regs(struct hiusbc *hiusbc, struct resource *res)
{
	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	res->start -= HIUSBC_DEVICE_MODE_REG_BASE;
	hiusbc->regs	= NULL;
	hiusbc->regs_size	= 0;

	hiusbc->host_regs = NULL;
	hiusbc->dev_regs = NULL;
	hiusbc->com_regs = NULL;
	hiusbc->hub_regs = NULL;
	hiusbc->lmi_regs = NULL;
	hiusbc->u2_pl_regs = NULL;
	hiusbc->u2_piu_regs = NULL;
	hiusbc->u3_pl_regs = NULL;
	hiusbc->u3_link_regs = NULL;
}

static int hiusbc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct resource *res = NULL;
	struct hiusbc *hiusbc = NULL;
	int ret;

	hiusbc_dbg(HIUSBC_DEBUG_ERR, "+\n");
	hiusbc = devm_kzalloc(dev, sizeof(*hiusbc), GFP_KERNEL);
	if (!hiusbc) {
		ret = -ENOMEM;
		goto exit;
	}

	hiusbc->imodi = DEV_INT_MOD_INTERVAL;

	hiusbc->dev = dev;
	dev_set_drvdata(dev, hiusbc);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "Can't get memory resource of hiusbc.\n");
		ret = -ENODEV;
		goto exit;
	}

	ret = hiusbc_init_regs(hiusbc, res);
	if (ret) {
		dev_err(dev, "Failed to initial registers of hiusbc.\n");
		goto exit;
	}

	hiusbc_get_properties(hiusbc);

	platform_set_drvdata(pdev, hiusbc);

	spin_lock_init(&hiusbc->lock);

	pm_runtime_set_active(dev);
	pm_runtime_use_autosuspend(dev);
	pm_runtime_set_autosuspend_delay(dev, HIUSBC_AUTOSUSPEND_DELAY);
	pm_runtime_enable(dev);
	ret = pm_runtime_get_sync(dev);
	if (ret < 0)
		goto pm_err;

	pm_runtime_forbid(dev);

	ret = hiusbc_init_dr_mode(hiusbc);
	if (ret) {
		dev_err(dev, "Failed to initial dr mode of hiusbc.\n");
		goto dr_mode_err;
	}

	hiusbc_debugfs_init(hiusbc);

	pm_runtime_put(dev);

	hiusbc_dbg(HIUSBC_DEBUG_ERR, "-\n");
	return 0;

dr_mode_err:
	pm_runtime_allow(dev);

pm_err:
	pm_runtime_put_sync(dev);
	pm_runtime_disable(dev);
	hiusbc_clear_regs(hiusbc, res);

exit:
	hiusbc_dbg(HIUSBC_DEBUG_ERR, "-: ret = %d\n", ret);
	return ret;
}

static int hiusbc_remove(struct platform_device *pdev)
{
	struct hiusbc *hiusbc = platform_get_drvdata(pdev);
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	struct device *dev = &pdev->dev;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	pm_runtime_get_sync(dev);

	hiusbc_debugfs_exit(hiusbc);

	hiusbc_exit_dr_mode(hiusbc);

	pm_runtime_allow(dev);

	pm_runtime_put_sync(dev);
	pm_runtime_disable(dev);
	hiusbc_clear_regs(hiusbc, res);

	if (hiusbc->combophy_regs)
		iounmap(hiusbc->combophy_regs);

	return 0;
}

static int hiusbc_suspend_event_hold(struct hiusbc *hiusbc)
{
	u32 reg;
	u32 timeout = 100;

	reg = hiusbc_readl(hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_HOLD_OFFSET);
	reg |= MPI_APP_COM_REG_SUSPEND_EVENT_HOLD_MASK;
	hiusbc_writel(reg, hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_HOLD_OFFSET);

	do {
		reg = hiusbc_readl(hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_HOLD_OFFSET);
		reg &= MPI_APP_COM_REG_USBC_EVENT_HANDLE_IDLE_MASK;
		udelay(1);
	} while (!reg && ((--timeout) != 0));

	if (!reg && timeout == 0) {
		dev_err(hiusbc->dev, "Timeout to set  controller suspend event hold!\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static int hiusbc_common_suspend(struct hiusbc *hiusbc)
{
	unsigned long flags;
	u32 reg;
	int ret = 0;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	spin_lock_irqsave(&hiusbc->lock, flags);
	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_DEVICE) {
		if (!hiusbc->gadget_driver)
			goto exit;

		hiusbc_run_stop(hiusbc, 0);
		hiusbc_gadget_disconnect(hiusbc);
		hiusbc_stop(hiusbc);
	}

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_HOST) {
#ifdef CONFIG_SYS_PM_ULSR
		if (is_ulsr()) {
			pr_info("hiusb suspend ulsr with hiusbc power down\n");
			goto exit;
		}
#endif
		/* set suspend hold */
		reg = hiusbc_readl(hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_INT_OFFSET);
		reg |= MPI_APP_COM_REG_SUSPEND_EVENT_HOLD_PULSE_MASK;
		hiusbc_writel(reg, hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_INT_OFFSET);

		ret = hiusbc_suspend_event_hold(hiusbc);
		if (ret)
			goto exit;
		/* sys clk switch */
		reg = hiusbc_readl(hiusbc->hub_regs, USBC_HUB_REG_USBC_CLK_CTRL_OFFSET);
			
		reg |= (USBC_HUB_REG_CFG_U3_SUSPEND_MODE_MASK | USBC_HUB_REG_CFG_U3_SUSPEND_MODE_SEL_MASK);
		hiusbc_writel(reg, hiusbc->hub_regs, USBC_HUB_REG_USBC_CLK_CTRL_OFFSET);
	}

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

exit:
	spin_unlock_irqrestore(&hiusbc->lock, flags);

	return ret;
}

static void hiusbc_host_reinit_with_powerdown(struct hiusbc *hiusbc)
{
	pr_info("hiusb resume reinit with hiusbc power down\n");
	hiusbc_init_phy(hiusbc);
	hiusbc_init_clk(hiusbc);
	hiusbc_config_ssp(hiusbc);
	hiusbc_host_reg_init(hiusbc);
}

static int hiusbc_common_resume(struct hiusbc *hiusbc)
{
	unsigned long flags;
	int ret = 0;
	u32 reg;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	spin_lock_irqsave(&hiusbc->lock, flags);

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_DEVICE) {
		if (!hiusbc->gadget_driver)
			goto exit;

		ret = hiusbc_init_phy(hiusbc);
		if (ret) {
			dev_err(hiusbc->dev,
				"failed to init phy during %s.\n", __func__);
			goto exit;
		}

		ret = hiusbc_run_stop(hiusbc, 1);
		if (ret) {
			hiusbc_run_stop(hiusbc, 0);
			goto exit;
		}
	}

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_HOST) {
#ifdef CONFIG_SYS_PM_ULSR
		if (is_ulsr()) {
			pr_info("hiusb resume ulsr with hiusbc power down, hiusbc reinit\n");
			hiusbc_host_reinit_with_powerdown(hiusbc);
			goto exit;
		}
#endif

		/* set suspend hold */
		reg = hiusbc_readl(hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_INT_OFFSET);
		reg &= ~MPI_APP_COM_REG_SUSPEND_EVENT_HOLD_PULSE_MASK;
		hiusbc_writel(reg, hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_INT_OFFSET);

		reg = hiusbc_readl(hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_HOLD_OFFSET);
		reg &= ~MPI_APP_COM_REG_SUSPEND_EVENT_HOLD_MASK;
		hiusbc_writel(reg, hiusbc->com_regs,
			MPI_APP_COM_REG_SUSPEND_MODE_EVENT_HOLD_OFFSET);
		/* sys clk switch */
		reg = hiusbc_readl(hiusbc->hub_regs, USBC_HUB_REG_USBC_CLK_CTRL_OFFSET);
		reg &= ~(USBC_HUB_REG_CFG_U3_SUSPEND_MODE_MASK | USBC_HUB_REG_CFG_U3_SUSPEND_MODE_SEL_MASK);
		hiusbc_writel(reg, hiusbc->hub_regs, USBC_HUB_REG_USBC_CLK_CTRL_OFFSET);
	}

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

exit:
	spin_unlock_irqrestore(&hiusbc->lock, flags);
	return ret;
}

static int hiusbc_runtime_suspend(struct device *dev)
{
	struct hiusbc *hiusbc = dev_get_drvdata(dev);
	int ret;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_DEVICE &&
		hiusbc->connected != 0)
		return -EBUSY;

	ret = hiusbc_common_suspend(hiusbc);
	if (ret)
		return ret;

	device_init_wakeup(dev, true);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return ret;
}

static int hiusbc_runtime_resume(struct device *dev)
{
	struct hiusbc *hiusbc = dev_get_drvdata(dev);
	int ret;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	device_init_wakeup(dev, false);

	ret = hiusbc_common_resume(hiusbc);
	if (ret)
		return ret;

	pm_runtime_mark_last_busy(dev);
	pm_runtime_put(dev);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return ret;
}

static int hiusbc_runtime_idle(struct device *dev)
{
	struct hiusbc *hiusbc = dev_get_drvdata(dev);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_DEVICE &&
		hiusbc->connected != 0)
		return -EBUSY;

	pm_runtime_mark_last_busy(dev);
	pm_runtime_autosuspend(dev);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return 0;
}

static int hiusbc_suspend(struct device *dev)
{
	struct hiusbc *hiusbc = dev_get_drvdata(dev);
	int ret;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	ret = hiusbc_common_suspend(hiusbc);
	if (ret)
		return ret;

	pinctrl_pm_select_sleep_state(dev);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return ret;
}

static int hiusbc_resume(struct device *dev)
{
	struct hiusbc *hiusbc = dev_get_drvdata(dev);
	int ret;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	pinctrl_pm_select_default_state(dev);

	ret = hiusbc_common_resume(hiusbc);
	if (ret)
		return ret;

	pm_runtime_disable(dev);
	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "-\n");

	return 0;
}

static int hiusbc_freeze(struct device *dev)
{
	struct hiusbc *hiusbc = dev_get_drvdata(dev);
	unsigned long flags;

	spin_lock_irqsave(&hiusbc->lock, flags);

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_DEVICE) {
		if (!hiusbc->gadget_driver)
			goto exit;

		hiusbc_run_stop(hiusbc, 0);
		hiusbc_gadget_disconnect(hiusbc);
		hiusbc_stop(hiusbc);
	}

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_HOST)
		hiusbc_run_stop(hiusbc, 0);

exit:
	spin_unlock_irqrestore(&hiusbc->lock, flags);
	return 0;
}

static int hiusbc_thaw(struct device *dev)
{
	struct hiusbc *hiusbc = dev_get_drvdata(dev);
	unsigned long flags;
	int ret = 0;

	hiusbc_dbg(HIUSBC_DEBUG_SYS, "+\n");

	spin_lock_irqsave(&hiusbc->lock, flags);

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_DEVICE) {
		if (!hiusbc->gadget_driver)
			goto exit;

		ret = hiusbc_init_phy(hiusbc);
		if (ret) {
			dev_err(hiusbc->dev,
				"failed to init phy during %s.\n", __func__);
			goto exit;
		}

		ret = hiusbc_run_stop(hiusbc, 1);
		if (ret) {
			hiusbc_run_stop(hiusbc, 0);
			goto exit;
		}
	}

	if (hiusbc->current_dr_mode == HIUSBC_DR_MODE_HOST)
		hiusbc_host_reinit_with_powerdown(hiusbc);

exit:
	spin_unlock_irqrestore(&hiusbc->lock, flags);
	return 0;
}

static const struct dev_pm_ops hiusbc_dev_pm_ops = {
#ifdef CONFIG_PM_SLEEP
	.suspend = hiusbc_suspend,
	.resume = hiusbc_resume,
	.freeze = hiusbc_freeze,
	.thaw = hiusbc_thaw,
	.poweroff = hiusbc_freeze,
	.restore = hiusbc_thaw,
#endif
	SET_RUNTIME_PM_OPS(hiusbc_runtime_suspend, hiusbc_runtime_resume,
			hiusbc_runtime_idle)
};

static const struct of_device_id of_hiusbc_match[] = {
	{ .compatible = "hisilicon,hiusbc" },
	{ .compatible = "hisi,hiusbc" },
	{},
};

static struct platform_driver hiusbc_driver = {
	.probe		= hiusbc_probe,
	.remove		= hiusbc_remove,
	.driver		= {
		.name	= "hiusbc",
		.pm	= &hiusbc_dev_pm_ops,
		.of_match_table	= of_hiusbc_match,
	},
};

module_platform_driver(hiusbc_driver);

MODULE_DESCRIPTION("Driver for the Hisilicon USB Controller");
MODULE_ALIAS("platform:hiusbc");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Wu Kaijie <wukaijie@hisilicon.com>");
