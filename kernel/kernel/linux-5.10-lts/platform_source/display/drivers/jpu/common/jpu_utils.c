/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 *
 * jpeg jpu utils
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "jpu_iommu.h"
#include "jpu_def.h"
#include "jpu_irq.h"

static void jpu_set_reg(char __iomem *addr, uint32_t val, uint8_t bw, uint8_t bs)
{
	uint32_t mask = (uint32_t)(((uint64_t)1UL << bw) - 1UL);
	uint32_t reg_tmp;

	reg_tmp = (uint32_t)inp32(addr);
	reg_tmp &= ~(mask << bs);

	outp32(addr, reg_tmp | ((val & mask) << bs));
}

static int32_t jpu_check_reg_state(const char __iomem *addr, uint32_t val)
{
	uint32_t tmp;
	int32_t delay_count = 0;
	bool is_timeout = true;

	while (1) {
		tmp = (uint32_t) inp32(addr);
		if (((tmp & val) == val) || (delay_count > 10)) { /* loop less 10 */
			is_timeout = (delay_count > 10) ? true : false;
			break;
		}

		udelay(10); /* 10us */
		++delay_count;
	}

	if (is_timeout) {
		jpu_err("fail to wait reg\n");
		return ERROR;
	}

	return 0;
}

void jpu_dec_normal_reset(const struct jpu_data_type *jpu_device)
{
	int32_t ret;

	if (jpu_device == NULL) {
		jpu_err("jpu_device is NULL\n");
		return;
	}

	jpu_set_reg(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_RD_CFG1, 0x1, 1, REG_SET_25_BIT);

	ret = jpu_check_reg_state(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_RD_CFG1, AXI_JPEG_NR_RD_STOP);
	if (ret != 0)
		jpu_err("fail to wait JPGDEC_CVDR_AXI_RD_CFG1\n");

	jpu_set_reg(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_RD_CFG1, 0x0, 1, REG_SET_25_BIT);
}

static void jpu_set_reg_cvdr(const struct jpu_data_type *jpu_device,
	uint32_t val)
{
	jpu_set_reg(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_RD_CFG1, val, 1, REG_SET_25_BIT);

	jpu_set_reg(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_RD_CFG2, val, 1, REG_SET_25_BIT);

	jpu_set_reg(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_WR_CFG1, val, 1, REG_SET_25_BIT);

	jpu_set_reg(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_WR_CFG2, val, 1, REG_SET_25_BIT);
}

static void jpu_check_reg(struct jpu_data_type *jpu_device)
{
	int32_t ret;

	ret = jpu_check_reg_state(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_RD_CFG1, AXI_JPEG_NR_RD_STOP);
	jpu_err_if_cond(ret != 0, "fail to wait JPGDEC_CVDR_AXI_RD_CFG1\n");

	ret = jpu_check_reg_state(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_RD_CFG2, AXI_JPEG_NR_RD_STOP);
	jpu_err_if_cond(ret != 0, "fail to wait JPGDEC_CVDR_AXI_RD_CFG2\n");

	ret = jpu_check_reg_state(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_WR_CFG1, AXI_JPEG_NR_RD_STOP);
	jpu_err_if_cond(ret != 0, "fail to wait JPGDEC_CVDR_AXI_WR_CFG1\n");

	ret = jpu_check_reg_state(jpu_device->jpu_cvdr_base +
		JPGDEC_CVDR_AXI_WR_CFG2, AXI_JPEG_NR_RD_STOP);
	jpu_err_if_cond(ret != 0, "fail to wait JPGDEC_CVDR_AXI_WR_CFG2\n");
}

void jpu_dec_error_reset(struct jpu_data_type *jpu_device)
{
	int32_t ret;
	if (jpu_device == NULL) {
		jpu_err("jpu_device is NULL\n");
		return;
	}

	/* step1 */
	if (jpu_device->jpu_support_platform == DSS_V510_CS) {
		jpu_set_reg(jpu_device->jpu_top_base + JPGDEC_PREF_STOP, 0x0, 1, 0);
	} else {
		jpu_set_reg(jpu_device->jpu_top_base + JPGDEC_CRG_CFG1,
			JPEGDEC_TPSRAM_2PRF_TRA, REG_SET_32_BIT, 0);
	}

	jpu_set_reg_cvdr(jpu_device, 0x1);

	/* step2 */
	if (jpu_device->jpu_support_platform == DSS_V510_CS) {
		ret = jpu_check_reg_state(jpu_device->jpu_top_base +
			JPGDEC_PREF_STOP, 0x10);

		jpu_err_if_cond(ret != 0, "fail to wait JPGDEC_PREF_STOP\n");
	} else {
		ret = jpu_check_reg_state(jpu_device->jpu_top_base +
			JPGDEC_RO_STATE, 0x2);

		jpu_err_if_cond(ret != 0, "fail to wait JPGDEC_RO_STATE\n");
	}
	jpu_check_reg(jpu_device);

	/* step3,read bit0 is 1 */
	jpu_set_reg(jpu_device->jpu_top_base + JPGDEC_CRG_CFG1, 1, 1, 0);
	ret = jpu_check_reg_state(jpu_device->jpu_top_base + JPGDEC_CRG_CFG1, 0x1);
	jpu_err_if_cond(ret != 0, "fail to wait JPGDEC_CRG_CFG1\n");

	/* step4 */
	jpu_set_reg_cvdr(jpu_device, 0x0);
	jpu_set_reg(jpu_device->jpu_top_base + JPGDEC_CRG_CFG1, 0x0, REG_SET_32_BIT, 0);
}

#pragma GCC diagnostic pop
