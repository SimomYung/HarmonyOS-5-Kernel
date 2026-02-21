/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * driver for vbat drop protect
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __HISI_VBAT_DROP_PROTECT_XIA_H
#define __HISI_VBAT_DROP_PROTECT_XIA_H


/* pmic vsys drop reg */
#define PMIC_VSYS_DROP_VOL_SET			PMIC_NP_VSYS_DROP_SET_ADDR(0)
#define PMIC_VSYS_DROP_IRQ_MASK_REG		PMIC_VSYS_DROP_IRQ_MASK_ADDR(0)
#define PMIC_VBAT_DROP_VOL_SET_REG		PMIC_NP_VSYS_DROP_SET_ADDR(0)

/* core enable reg in pmctrl */
#define LITTLE_VOL_DROP_EN_ADDR(base)       0
#define MIDDLE_VOL_DROP_EN_ADDR(base)       0
#define BIG_VOL_DROP_EN_ADDR(base)          0
#define L3_VOL_DROP_EN_ADDR(base)           0
#define GPU_VOL_DROP_EN_ADDR(base)          0
#define NPU_VOL_DROP_EN_ADDR(base)          0

#define LITTLE_VOL_DROP_EN_BIT          0
#define MIDDLE_VOL_DROP_EN_BIT          0
#define BIG_VOL_DROP_EN_BIT             0
#define L3_VOL_DROP_EN_BIT              0
#define GPU_VOL_DROP_EN_BIT             0

/* reg bit write en */
#define LITTLE_VOL_DROP_MASK          0
#define MIDDLE_VOL_DROP_MASK          0
#define BIG_VOL_DROP_MASK             0
#define L3_VOL_DROP_MASK              0
#define GPU_VOL_DROP_MASK             0

static inline int _hisi_vbat_drop_vol_set(int mv)
{
	unsigned int reg_val;

	if (mv <= 2350)       /* Battery voltage :2350 mv */
		reg_val = 0;  /* PMIC_VSYS_DROP_VOL_SET: 0 */
	else if (mv <= 2500)  /* Battery voltage :2500 mv */
		reg_val = 1;  /* PMIC_VSYS_DROP_VOL_SET: 1 */
	else if (mv <= 2650)  /* Battery voltage :2650 mv */
		reg_val = 2;  /* PMIC_VSYS_DROP_VOL_SET: 2 */
	else if (mv <= 2800)  /* Battery voltage :2800 mv */
		reg_val = 3;  /* PMIC_VSYS_DROP_VOL_SET: 3 */
	else if (mv <= 2950)  /* Battery voltage :2950 mv */
		reg_val = 4;  /* PMIC_VSYS_DROP_VOL_SET: 4 */
	else if (mv <= 3100)  /* Battery voltage :3100 mv */
		reg_val = 5;  /* PMIC_VSYS_DROP_VOL_SET: 5 */
	else if (mv <= 3250)  /* Battery voltage :3250 mv */
		reg_val = 6;  /* PMIC_VSYS_DROP_VOL_SET: 6 */
	else
		reg_val = 7;

	return reg_val;
}

#endif
