/*
 * vcodec_pxp_hal.c
 *
 * Implementation of interfaces related to pxp register operations
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "vcodec_pxp.h"
#include "vcodec_pxp_hal.h"
#include "dbg.h"
#include "tvp_adapter.h"
#include "vdec_memory_map.h"
#include "vcodec_vdec_plat.h"
#include "vcodec_vdec_irq.h"

static void vcodec_pxp_hal_reset_pre_process(struct vcodec_dev_pxp* dev);
static void vcodec_pxp_hal_unreset_pre_process(struct vcodec_dev_pxp* dev);

int32_t vcodec_pxp_init_hal(struct vcodec_dev_pxp* dev, uint8_t *reg_base)
{
	dev->reg_base_vir_addr = reg_base;

	dev->subctrl_vir_addr =
		(S_VDE_SUB_CTRL_NS_REGS_TYPE *)(reg_base + OFFSET_OF_VDEC_BASE(SUB_CTRL_REG_BASE));
	dev->pxpc_subctrl_vir_addr =
		(S_PXPC_SC_REGS_TYPE *)(reg_base + OFFSET_OF_VDEC_BASE(PXPC_SCTRL));

	return 0;
}

void vcodec_pxp_deinit_hal(struct vcodec_dev_pxp *dev)
{
	dev->reg_base_vir_addr = NULL;
	dev->subctrl_vir_addr = NULL;
	dev->pxpc_subctrl_vir_addr = NULL;
}

int32_t vcodec_pxp_hal_poweron(struct vcodec_dev_pxp* dev)
{
	pxp_powerup_cfg cfg = {0};

	if (dev->reset_flag)
		vcodec_pxp_hal_unreset_pre_process(dev);

	cfg.syscache_start_addr = dev->buffer_info.syscache_start_addr;
	cfg.syscache_len = dev->buffer_info.syscache_len;
	cfg.muti_core_en = (uint8_t)dev->multi_core_enable;
	// reset or resume do not need reload image
	cfg.is_reset = (uint8_t)(dev->reset_flag || dev->status == VCODEC_PXP_STATUS_SUSPEND);
	cfg.is_sc_allocated = (uint8_t)dev->is_sc_allocated;
	vcodec_vdec_enable_irq(PXPC_NORM_IRQ);
	if (tvp_vdec_pxp_open(&cfg) != VDEC_OK) {
		vcodec_vdec_disable_irq(PXPC_NORM_IRQ);
		return VDEC_ERR;
	}
	return VDEC_OK;
}

static void vcodec_pxp_poweroff_set_bus_idle(struct vcodec_dev_pxp* dev)
{
	uint32_t reg_value;
	uint32_t cnt;
	bus_idle_cfg bus_idle = {0};
	vdec_plat_ops *ops = get_vdec_plat_ops();

	ops->pxp_config_bus_idle(&bus_idle, true);
	cnt = 0;
	wr_vreg(dev->reg_base_vir_addr, bus_idle.offset, bus_idle.target_value);
	while (1) {
		rd_vreg(dev->reg_base_vir_addr, bus_idle.offset, reg_value);
		if (reg_value == bus_idle.expect_value || cnt >= 5) {
			dprint(PRN_ALWS, "pxp bus idle set ack pass");
			break;
		}
		OS_MSLEEP(1);
		cnt++;
		dprint(PRN_DBG, "pxp bus idle set ack current is %x", reg_value);
	}
}

static void vcodec_pxp_poweroff_pxpc_stop_sc_ctrl(struct vcodec_dev_pxp* dev)
{
	dev->pxpc_subctrl_vir_addr->SC_CTRL_EN_NS |= BIT(1);
	dprint(PRN_DBG, "power off pxp stop sc ctrl pass");
}

static int32_t vcodec_pxp_poweroff_wait_result(struct vcodec_dev_pxp* dev)
{
	if (vcodec_osal_event_wait(dev->pxp_power_off_ack, PXP_ACK_TIME_OUT) != OSAL_OK) {
		dprint(PRN_ERROR, "wait pxp poweroff done timeout...");
		return -EFAULT;
	}
	dprint(PRN_DBG, "pxp poweroff done");
	return 0;
}

static void vcodec_pxp_poweroff_pxpc_stop_core(struct vcodec_dev_pxp* dev)
{
	int32_t wait_cnt = 5;
	S_PXPC_SC_REGS_TYPE *pxp = dev->pxpc_subctrl_vir_addr;

	// write stop command and wait stop result
	pxp->PXPC_CORE_HALT.bits.pxpc_core0_halt_ena = 1;
	while (pxp->SYS_IDLE_STAT.bits.core0_wfi != 1 ||
		pxp->SYS_IDLE_STAT.bits.core1_wfi != 1) {
		udelay(2);
		wait_cnt--;
		dprint(PRN_ALWS, "wait pxp wfi done...");
		if (wait_cnt < 0) {
			dprint(PRN_ERROR, "wait pxp core_wfi 10 us timeout...");
			break;
		}
	}
	dprint(PRN_DBG, "power off pxp stop core pass");
}

int32_t vcodec_pxp_hal_poweroff(struct vcodec_dev_pxp* dev)
{
	dprint(PRN_ALWS, "power off pxp");
	if (tvp_vdec_pxp_close() != 0)
		dprint(PRN_ERROR, "tvp_vdec_pxp_close err");

	if (dev->reset_flag)
		vcodec_pxp_hal_reset_pre_process(dev);

	// step0 set bus idle
	vcodec_pxp_poweroff_set_bus_idle(dev);

	vcodec_pxp_poweroff_pxpc_stop_sc_ctrl(dev);

	if (vcodec_pxp_poweroff_wait_result(dev)) {
		vcodec_vdec_disable_irq(PXPC_NORM_IRQ);
		return -EFAULT;
	}

	vcodec_pxp_poweroff_pxpc_stop_core(dev);
	vcodec_vdec_disable_irq(PXPC_NORM_IRQ);
	dprint(PRN_ALWS, "power off pxp success");
	return 0;
}

static void vcodec_pxp_reset_config_pxphw_softrst(
	struct vcodec_dev_pxp* dev, uint32_t offset, uint32_t config_val, uint32_t expect)
{
	uint32_t reg_value;
	uint32_t cnt;

	cnt = 0;
	wr_vreg(dev->reg_base_vir_addr, offset, config_val);
	while (1) {
		rd_vreg(dev->reg_base_vir_addr, offset, reg_value);
		if (reg_value == expect || cnt >= 5) {
			dprint(PRN_ALWS, "reset pxp ack pass");
			break;
		}
		OS_MSLEEP(1);
		cnt++;
		dprint(PRN_DBG, "reset pxp ack current is %x", reg_value);
	}
}

static void vcodec_pxp_reset_config_subctrl_crg(struct vcodec_dev_pxp* dev, bool enable)
{
	U_VDEC_CRG reg_crg;
	vdec_plat_ops *ops = get_vdec_plat_ops();

	reg_crg.u32 = dev->subctrl_vir_addr->VDEC_CRG.u32;
	dprint(PRN_DBG, "subctrl->VDEC_CRG 0x%x", reg_crg.u32);
	if (!enable) {
		/* pxpc clk disable */
		reg_crg.bits.pxpc_vdec_clk_en = 0;
		reg_crg.bits.pxpc_clk_en = 0;
	}
	reg_crg.bits.pxp_hw_clk_en = enable ? 1: 0;
	reg_crg.bits.hw_qic_clk_axi_en = enable ? 1: 0;
	ops->pxp_reset_config_subctrl_crg(&reg_crg, enable);
	dev->subctrl_vir_addr->VDEC_CRG.u32 = reg_crg.u32;
	dprint(PRN_DBG, "subctrl->VDEC_CRG 0x%x", reg_crg.u32);
}

static void vcodec_pxp_reset_check_root_rst(struct vcodec_dev_pxp* dev)
{
	uint32_t reg_value;

	rd_vreg(dev->reg_base_vir_addr, 0x408028, reg_value);
	dprint(PRN_DBG, "reset pxp VDEC_ROOT_RST %x", reg_value);
}

static void vcodec_pxp_reset_config_subctrl_rst(struct vcodec_dev_pxp* dev, bool enable)
{
	U_VDEC_RST reg_rst;
	vdec_plat_ops *ops = get_vdec_plat_ops();

	reg_rst.u32 = dev->subctrl_vir_addr->VDEC_RST.u32;
	dprint(PRN_DBG, "subctrl->VDEC_RST %x", reg_rst.u32);
	if (enable) {
		/* pxpc rst */
		reg_rst.bits.pxpc_vdec_soft_rst = 1;
		reg_rst.bits.pxpc_top_soft_rst = 1;
	}
	reg_rst.bits.pxphw_soft_rst = enable ? 1: 0;
	reg_rst.bits.hw_qic_clk_axi_soft_rst = enable ? 1: 0;
	ops->pxp_reset_config_subctrl_rst(&reg_rst, enable);
	dev->subctrl_vir_addr->VDEC_RST.u32 = reg_rst.u32;
	dprint(PRN_DBG, "subctrl->VDEC_RST %x", reg_rst.u32);
}

static void vcodec_pxp_hal_reset_pre_process(struct vcodec_dev_pxp* dev)
{
	vdec_plat_ops *ops = get_vdec_plat_ops();
	bus_idle_cfg bus_idle = {0};

	// set bus idle
	ops->pxp_config_bus_idle(&bus_idle, true);
	vcodec_pxp_reset_config_pxphw_softrst(dev, bus_idle.offset, bus_idle.target_value, bus_idle.expect_value);
}

static void vcodec_pxp_hal_unreset_pre_process(struct vcodec_dev_pxp* dev)
{
	vcodec_pxp_reset_config_subctrl_crg(dev, false);
	vcodec_pxp_reset_check_root_rst(dev);
	vcodec_pxp_reset_config_subctrl_rst(dev, true);

	// release reset operation
	vcodec_pxp_reset_config_subctrl_rst(dev, false);
	vcodec_pxp_reset_config_subctrl_crg(dev, true);
}

void vcodec_pxp_hal_unreset_host_process(struct vcodec_dev_pxp* dev)
{
	vdec_plat_ops *ops = get_vdec_plat_ops();
	bus_idle_cfg bus_idle = {0};

	// clear bus idle
	ops->pxp_config_bus_idle(&bus_idle, false);
	vcodec_pxp_reset_config_pxphw_softrst(dev, bus_idle.offset, bus_idle.target_value, bus_idle.expect_value);
}

void vcodec_pxp_hal_irq_mask(struct vcodec_dev_pxp* dev)
{
	uint32_t acpu_mask = 0xFFFFFFFF;
	uint32_t mcu_mask = 0xFFFFFFFF;
	S_VDE_SUB_CTRL_NS_REGS_TYPE *subctrl = dev->subctrl_vir_addr;

	acpu_mask ^= BIT(PXPC_INIT_DONE);
	acpu_mask ^= BIT(PXPC_PWR_DOWN_ACK);
	mcu_mask ^= BIT(PXPC_EOP);

	subctrl->INTR_MASK_NS_PXPC0.bits.intr_pxpc_mask = mcu_mask;
	subctrl->INTR_MASK_NS_PXPC1.bits.intr_pxpc_acpu_mask = acpu_mask;

	dprint(PRN_DBG, "pxp acpu_mask is %#x, mcu_mask is %#x", acpu_mask, mcu_mask);
}

static void vcodec_pxp_notify_power_on(struct vcodec_dev_pxp* dev)
{
	vcodec_osal_event_give(dev->pxp_power_on_ack);
}

static void vcodec_pxp_notify_power_off(struct vcodec_dev_pxp* dev)
{
	vcodec_osal_event_give(dev->pxp_power_off_ack);
}

irqreturn_t vcodec_pxp_hal_interrupt_handling(struct vcodec_dev_pxp* dev)
{
	uint32_t exit = 0;
	uint32_t state;

	while (!exit) {
		exit = 1;
		if (!dev->subctrl_vir_addr) {
			dprint(PRN_ALWS, "pxp subctrl_vir_addr is null");
			return IRQ_HANDLED;
		}
		state = dev->subctrl_vir_addr->INTR_NS_PXPC1.bits.intr_pxpc_acpu_ns;
		dprint(PRN_DBG, "pxp int state:0x%x", state);
		if (state == 0xFFFFFFFF) {
			dprint(PRN_ERROR, "pxp int state:0x%x error", state);
			break;
		}

		if (state & BIT(PXPC_INIT_DONE)) {
			/* MCU_INSTR_MEM_CTRL2_HIGH reg is added to determine whether
			  there is an intr to prevent the ACPU from clearing the intr
			  before the SecureOS queries the intr status. */
			dev->subctrl_vir_addr->MCU_INSTR_MEM_CTRL2_HIGH = 1;
			dev->subctrl_vir_addr->INTR_CLR_NS_PXPC0.bits.intr_pxpc_clr = BIT(PXPC_INIT_DONE);
			vcodec_pxp_notify_power_on(dev);
		}
		if (state & BIT(PXPC_PWR_DOWN_ACK)) {
			dev->subctrl_vir_addr->INTR_CLR_NS_PXPC0.bits.intr_pxpc_clr = BIT(PXPC_PWR_DOWN_ACK);
			vcodec_pxp_notify_power_off(dev);
		}
	}

	return IRQ_HANDLED;
}