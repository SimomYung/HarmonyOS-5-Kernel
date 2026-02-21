/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 *
 * jpgdec irq
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "jpu.h"
#include "jpu_def.h"
#include "jpu_utils.h"
#include "jpu_osal.h"

static irqreturn_t jpgdec_irq_isr(int32_t irq, void *ptr)
{
	struct jpu_data_type *jpu_device = NULL;
	uint32_t reg_val;
	uint32_t isr_state;

	jpu_check_null_return(ptr, IRQ_HANDLED);
	jpu_device = (struct jpu_data_type *)ptr;
	jpu_check_null_return(jpu_device->jpu_top_base, IRQ_HANDLED);

	isr_state = inp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG2);
	if (isr_state & BIT(DEC_DONE_ISR_BIT)) { /* use 16bit to decide */
		jpu_info("done isr occured\n");
		reg_val = inp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0);
		reg_val |= BIT(0);
		outp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0, reg_val);
		jpu_device->jpu_dec_done_flag = 1;
		jpu_wake_up_interrupt((struct jpu_interrupt_t *)(jpu_device->jpu_irq_num));
	}

	if (isr_state & BIT(DEC_ERR_ISR_BIT)) { /* use 17bit to decide */
		jpu_info("err isr occured\n");
		reg_val = inp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0);
		reg_val |= BIT(1);
		outp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0, reg_val);
		jpu_dec_error_reset(jpu_device);
	}

	if (isr_state & BIT(DEC_OVERTIME_ISR_BIT)) { /* use 18bit to decide */
		jpu_info("overtime isr occured\n");
		reg_val = inp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0);
		reg_val |= BIT(2); /* 2 is reg assign 1 to second */
		outp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0, reg_val);
		jpu_dec_error_reset(jpu_device);
	}

	if (isr_state & BIT(DEC_BS_RESUME_BIT)) { /* use 19bit to decide */
		jpu_info("bitstream resume isr occured\n");
		reg_val = inp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0);
		reg_val |= BIT(3); /* 3 is reg assign 1 to second */
		outp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0, reg_val);
		jpu_dec_error_reset(jpu_device);
	}

	return IRQ_HANDLED;
}

int32_t jpgdec_request_irq(struct jpu_data_type *jpu_device)
{
	int32_t index;
	int32_t ret = 0;

	jpu_check_null_return(jpu_device, -EINVAL);

	for (index = 0; index < JPGDEC_IRQ_NUM; index++) {
		ret = os_request_irq(jpu_device->jpu_irq_num[index], jpgdec_irq_isr, (void *)jpu_device);
		if (ret != 0) {
			jpu_err("request irq failed, irq_num = %d error = %d\n", jpu_device->jpu_irq_num[index], ret);
			return ret;
		}
		os_disable_irq(jpu_device->jpu_irq_num[index]);
	}
	return ret;
}

int32_t jpgdec_enable_irq(struct jpu_data_type *jpu_device)
{
	int32_t index;
	jpu_check_null_return(jpu_device, -EINVAL);

	for (index = 0; index < JPGDEC_IRQ_NUM; index++) {
		if (jpu_device->jpu_irq_num[index] == 0)
			return -EINVAL;
		os_enable_irq(jpu_device->jpu_irq_num[index]);
	}

	return 0;
}

void jpgdec_disable_irq(struct jpu_data_type *jpu_device)
{
	int32_t index;
	if (jpu_device == NULL) {
		jpu_err("jpu_device is nullptr\n");
		return;
	}

	for (index = 0; index < JPGDEC_IRQ_NUM; index++) {
		if (jpu_device->jpu_irq_num[index] != 0)
			os_disable_irq(jpu_device->jpu_irq_num[index]);
	}
}

void jpgdec_free_irq(struct jpu_data_type *jpu_device)
{
	int32_t index;
	if (jpu_device == NULL) {
		jpu_err("jpu_device is nullptr\n");
		return;
	}

	for (index = 0; index < JPGDEC_IRQ_NUM; index++) {
		if (jpu_device->jpu_irq_num[index] != 0) {
			os_free_irq(jpu_device->jpu_irq_num[index]);
			jpu_device->jpu_irq_num[index] = 0;
		}
	}
}

void jpu_dec_interrupt_unmask(const struct jpu_data_type *jpu_device)
{
	uint32_t unmask;

	unmask = ~0;
	unmask &= ~(BIT_JPGDEC_INT_DEC_ERR | BIT_JPGDEC_INT_DEC_FINISH);

	outp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG1, unmask);
}

void jpu_dec_interrupt_mask(const struct jpu_data_type *jpu_device)
{
	uint32_t mask;

	mask = ~0;
	outp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG1, mask);
}

void jpu_dec_interrupt_clear(const struct jpu_data_type *jpu_device)
{
	/* clear jpg decoder IRQ state
	 * [3]: jpgdec_int_over_time;
	 * [2]: jpgdec_int_dec_err;
	 * [1]: jpgdec_int_bs_res;
	 * [0]: jpgdec_int_dec_finish;
	 */
	outp32(jpu_device->jpu_top_base + JPGDEC_IRQ_REG0, 0xF); /* 0xF clera irq */
}
