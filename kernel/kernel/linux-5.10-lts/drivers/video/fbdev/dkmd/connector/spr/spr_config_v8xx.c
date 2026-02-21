/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include "spr_config.h"
#include "dkmd_log.h"

#define PANEL_SPR_GAMMA_LOW_LUT_SIZE (3 * 18)
void spr_gamma_low_lut_config(char __iomem *spr_base, struct spr_info *spr)
{
	uint32_t i;
	uint32_t lut_val[SPR_GAMMA_LOW_LUT_ROW] = {0};
	uint32_t *spr_lut_addr;
	uint32_t lut_length;

	if (!spr_base || !spr || !spr->spr_gamma_low_lut_table ||
		spr->spr_gamma_low_lut_table_len != PANEL_SPR_GAMMA_LOW_LUT_SIZE) {
		dpu_pr_warn("spr or spr_base or spr_gamma_low_lut_table is null, or len invalid!\n");
		return;
	}
	spr_lut_addr = spr->spr_gamma_low_lut_table;
	lut_length = spr->spr_gamma_low_lut_table_len / SPR_GAMMA_LOW_LUT_ROW;

	for (i = 0; i < lut_length; i += 2) {
		lut_val[SPR_GAMMA_LOW_R] =
			(spr_lut_addr[i + lut_length * SPR_GAMMA_LOW_R + 1] << 16) | spr_lut_addr[i + lut_length * SPR_GAMMA_LOW_R];
		lut_val[SPR_GAMMA_LOW_G] =
			(spr_lut_addr[i + lut_length * SPR_GAMMA_LOW_G + 1] << 16) | spr_lut_addr[i + lut_length * SPR_GAMMA_LOW_G];
		lut_val[SPR_GAMMA_LOW_B] =
			(spr_lut_addr[i + lut_length * SPR_GAMMA_LOW_B + 1] << 16) | spr_lut_addr[i + lut_length * SPR_GAMMA_LOW_B];
		
		outp32(DPU_DPP_U_GAMA_R_COEF_LOW_ADDR(spr_base, i / 2), lut_val[SPR_GAMMA_LOW_R]);
		outp32(DPU_DPP_U_GAMA_G_COEF_LOW_ADDR(spr_base, i / 2), lut_val[SPR_GAMMA_LOW_G]);
		outp32(DPU_DPP_U_GAMA_B_COEF_LOW_ADDR(spr_base, i / 2), lut_val[SPR_GAMMA_LOW_B]);
	}
}

void spr_lut_config(char __iomem *spr_base, struct spr_info *spr)
{
	uint32_t i, row, column, idx;
	uint32_t lut_val[SPR_GAMMA_LUT_ROW] = {0};
	uint32_t *spr_lut_addr = spr->spr_lut_table;
	uint32_t lut_length = spr->spr_lut_table_len / SPR_GAMMA_LUT_ROW;
	for (i = 0, column = 0; column < lut_length; column += COEF_PER_REG, i++) {
		for (row = 0; row < (uint32_t)SPR_GAMMA_LUT_ROW; row++) {
			idx = lut_length * row + column;
			lut_val[row] = (spr_lut_addr[idx + 1] << 16) | spr_lut_addr[idx];
		}

		set_reg(DPU_DPP_U_GAMA_R_COEF_ADDR(spr_base, i), lut_val[SPR_GAMMA_R], 32, 0);
		set_reg(DPU_DPP_U_GAMA_G_COEF_ADDR(spr_base, i), lut_val[SPR_GAMMA_G], 32, 0);
		set_reg(DPU_DPP_U_GAMA_B_COEF_ADDR(spr_base, i), lut_val[SPR_GAMMA_B], 32, 0);
		set_reg(DPU_DPP_U_DEGAMA_R_COEF_ADDR(spr_base, i), lut_val[SPR_DEGAMMA_R], 32, 0);
		set_reg(DPU_DPP_U_DEGAMA_G_COEF_ADDR(spr_base, i), lut_val[SPR_DEGAMMA_G], 32, 0);
		set_reg(DPU_DPP_U_DEGAMA_B_COEF_ADDR(spr_base, i), lut_val[SPR_DEGAMMA_B], 32, 0);
	}

	spr_gamma_low_lut_config(spr_base, spr);
}

MODULE_LICENSE("GPL");