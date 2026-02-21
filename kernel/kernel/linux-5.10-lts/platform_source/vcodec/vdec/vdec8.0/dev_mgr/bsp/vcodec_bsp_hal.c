/*
 * vcodec_bsp_hal.c
 *
 * Implementation of interfaces related to bsp register operations
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

#include "vcodec_bsp.h"
#include "vcodec_bsp_hal.h"
#include "vcodec_vdec_irq.h"
#include "vdec_memory_map.h"
#include "tvp_adapter.h"
#include "dbg.h"

int32_t vcodec_bsp_init_hal(struct vcodec_dev_bsp* dev, uint8_t *reg_base)
{
	dev->reg_base_vir_addr = reg_base;
	dev->subctrl_vir_addr =
		(S_VDE_SUB_CTRL_NS_REGS_TYPE *)(reg_base + OFFSET_OF_VDEC_BASE(SUB_CTRL_REG_BASE));

	return 0;
}

void vcodec_bsp_deinit_hal(struct vcodec_dev_bsp* dev)
{
	dev->reg_base_vir_addr = NULL;
	dev->subctrl_vir_addr = NULL;
}

// TODO: refactor this operation
static int32_t vcodec_bsp_open_task(struct vcodec_dev_bsp* dev)
{
	bsp_powerup_cfg cfg = {0};
	struct bsp_buffer_cfg* buffer_info = &dev->buffer_info;

	cfg.nsf_sw_addr = buffer_info->sw_addr;
	cfg.use_ringbuffer_flag = 1;
	// we do not reload bsp image when reset or resume
	if (dev->reset_flag) {
		cfg.poweron_mode = BSP_POWERON_RESET;
	} else if (dev->status == VCODEC_BSP_STATUS_SUSPEND) {
		cfg.poweron_mode = BSP_POWERON_SR;
	} else {
		cfg.poweron_mode = BSP_POWERON_NORMAL;
	}
	cfg.ctu_info_len = buffer_info->ctu_info_len;
	cfg.ctu_res_len = buffer_info->ctu_res_len;
	cfg.ringbuffer_info_nsf.ctu_info_addr[0] = buffer_info->ctu_info_addr[0];
	cfg.ringbuffer_info_nsf.ctu_info_addr[1] = buffer_info->ctu_info_addr[1];
	cfg.ringbuffer_info_nsf.ctu_res_addr[0] = buffer_info->ctu_res_addr[0];
	cfg.ringbuffer_info_nsf.ctu_res_addr[1] = buffer_info->ctu_res_addr[1];

	return tvp_vdec_bsp_open(&cfg);
}

static int32_t vcodec_bsp_poweron_wait_result(struct vcodec_dev_bsp* dev)
{
	if (vcodec_osal_event_wait(dev->bsp_power_on_ack, BSP_ACK_TIME_OUT)) {
		dprint(PRN_ERROR, "wait bsp init done timeout...");
		return -EFAULT;
	}

	return 0;
}

int32_t vcodec_bsp_hal_poweron(struct vcodec_dev_bsp* dev)
{
	vcodec_vdec_enable_irq(BSP_NORM_IRQ);
	if (vcodec_bsp_open_task(dev)) {
		dprint(PRN_ERROR, "vcodec_bsp_open_task err");
		goto error;
	}
	if (vcodec_bsp_poweron_wait_result(dev))
		goto error;

	return 0;
error:
	vcodec_vdec_disable_irq(BSP_NORM_IRQ);
	return -EFAULT;
}

static int32_t vcodec_bsp_poweroff_wait_result(struct vcodec_dev_bsp* dev)
{
	if (vcodec_osal_event_wait(dev->bsp_power_off_ack, BSP_ACK_TIME_OUT)) {
		dprint(PRN_ERROR, "wait bsp poweroff done timeout...");
		return -EFAULT;
	}

	return 0;
}

int32_t vcodec_bsp_hal_poweroff(struct vcodec_dev_bsp* dev)
{
	if (tvp_vdec_bsp_close()) {
		dprint(PRN_ERROR, "tvp_vdec_bsp_close err");
		goto error;
	}
	if (vcodec_bsp_poweroff_wait_result(dev))
		goto error;
	vcodec_vdec_disable_irq(BSP_NORM_IRQ);
	return 0;
error:
	vcodec_vdec_disable_irq(BSP_NORM_IRQ);
	return -EFAULT;
}

static void vcodec_bsp_close_stop_pxpc_push(struct vcodec_dev_bsp* dev)
{
	uint32_t reg_value;
	uint32_t cnt;

	cnt = 0;
	wr_vreg(dev->reg_base_vir_addr, 0x280080, 0xFFFFFFFF);
	while (1) {
		rd_vreg(dev->reg_base_vir_addr, 0x280080, reg_value);
		if (reg_value == 0xFFFF || cnt >= 5)
			break;

		dprint(PRN_DBG, "bsp STEP1 ret %x", reg_value);
		cnt++;
		OS_MSLEEP(1);
	}
	dprint(PRN_DBG, "bsp STEP1: %x", reg_value);
}

static void vcodec_bsp_open_stop_pxpc_push(struct vcodec_dev_bsp* dev)
{
	wr_vreg(dev->reg_base_vir_addr, 0x280080, 0xFFFF0000);
}

void vcodec_bsp_hal_reset_pre_process(struct vcodec_dev_bsp* dev)
{
	// STEP1 close PXPC push addr to BSP
	vcodec_bsp_close_stop_pxpc_push(dev);
	dprint(PRN_DBG, "bsp reset STEP1 close pxpc push pass");
}

void vcodec_bsp_hal_unreset_host_process(struct vcodec_dev_bsp* dev)
{
	// STEP4 open PXPC push addr to BSP
	vcodec_bsp_open_stop_pxpc_push(dev);
	dprint(PRN_DBG, "bsp reset STEP4 open pxpc push pass");
}


void vcodec_bsp_hal_irq_mask(struct vcodec_dev_bsp* dev)
{
	uint32_t acpu_mask = 0xFFFFFFFF;
	uint32_t mcu_mask = 0xFFFFFFFF;
	S_VDE_SUB_CTRL_NS_REGS_TYPE *subctrl = dev->subctrl_vir_addr;

	acpu_mask ^= BIT(BSP_INIT_DONE);
	acpu_mask ^= BIT(BSP_POWERDOWN_ACK);
	acpu_mask ^= BIT(BSP_CLR_ACK0);
	acpu_mask ^= BIT(BSP_CLR_ACK1);

	mcu_mask ^= BIT(BSP_TASK_DONE_NSF0);
	mcu_mask ^= BIT(BSP_TASK_DONE_NSF1);

	subctrl->INTR_MASK_NS_BSP1 = acpu_mask;
	subctrl->INTR_MASK_NS_BSP0 = mcu_mask;

	dprint(PRN_ALWS, "bsp acpu_mask is %#x, mcu_mask is %#x", acpu_mask, mcu_mask);
}

static void vcodec_bsp_notify_power_on(struct vcodec_dev_bsp* dev)
{
	vcodec_osal_event_give(dev->bsp_power_on_ack);
}

static void vcodec_bsp_notify_power_off(struct vcodec_dev_bsp* dev)
{
	vcodec_osal_event_give(dev->bsp_power_off_ack);
}

irqreturn_t vcodec_bsp_hal_interrupt_handling(struct vcodec_dev_bsp* dev)
{
	uint32_t exit = 0;
	uint32_t state;

	while (!exit) {
		exit = 1;
		if (!dev->subctrl_vir_addr) {
			dprint(PRN_ALWS, "bsp subctrl_vir_addr is null");
			return IRQ_HANDLED;
		}
		state = dev->subctrl_vir_addr->INTR_NS_BSP1;
		dprint(PRN_DBG, "bsp int state: 0x%x", state);

		if (state == 0xFFFFFFFF) {
			dprint(PRN_ERROR, "bsp int state error");
			break;
		}

		if (state & BIT(BSP_INIT_DONE)) {
			dev->subctrl_vir_addr->INTR_CLR_NS_BSP0 = BIT(BSP_INIT_DONE);
			vcodec_bsp_notify_power_on(dev);
		}

		if (state & BIT(BSP_POWERDOWN_ACK)) {
			/* MCU_INSTR_MEM_CTRL2_LOW reg is added to determine whether
			  there is an intr to prevent the ACPU from clearing the intr
			  before the SecureOS queries the intr status. */
			dev->subctrl_vir_addr->MCU_INSTR_MEM_CTRL2_LOW = 1;
			dev->subctrl_vir_addr->INTR_CLR_NS_BSP0 = BIT(BSP_POWERDOWN_ACK);
			vcodec_bsp_notify_power_off(dev);
		}

		if (state & BIT(BSP_CLR_ACK0)) {
			dev->subctrl_vir_addr->INTR_CLR_NS_BSP0 = BIT(BSP_CLR_ACK0);
			vcodec_osal_event_give(dev->bsp0_clear_ack);
		}

		if (state & BIT(BSP_CLR_ACK1)) {
			dev->subctrl_vir_addr->INTR_CLR_NS_BSP0 = BIT(BSP_CLR_ACK1);
			vcodec_osal_event_give(dev->bsp1_clear_ack);
		}
	}

	return IRQ_HANDLED;
}
