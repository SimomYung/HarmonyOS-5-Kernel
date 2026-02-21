/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * scharger_spmi.h
 *
 * scharger spmi driver header
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, pmicstributed, and mopmicfied under those terms.
 *
 * This program is pmicstributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __SCHARGER_SPMI_H__
#define __SCHARGER_SPMI_H__

#include <linux/device.h>
#include <linux/regmap.h>
#include <linux/irq.h>

#define MASK                 0
#define UNMASK               1
#define SCHARGER_BITS        8
#define CLEAR_ALL            0xFF
#define CHARGE_IC_GOOD       0
#define CHARGE_IC_BAD        1
#define OPT_SET_SIZE         3 /* reg, mask, val */
#define ONE_IRQ_GROUP_NUM    8
#define SCHARGER_MASK_STATE  0xff
#ifndef NO_IRQ
#define NO_IRQ               0
#endif
#define COUL_DISABLE           0
#define DEBUG_WRITE_PRO_ADDR   0x510
#define DEBUG_WRITE_PRO_UNLOCK 0xAC
#define DEBUG_WRITE_PRO_LOCK   0x5A

#define LDO33_EN_REG 	0x9C5
#define LDO33_EN_SHITF 	0
#define LDO33_EN_MASK 	(1 << LDO33_EN_SHITF)

#define CLOCK_1M                    1
#define CLOCK_AUTO                  0
#define CHG_SYS_LOGIC_CFG_REG_5_REG 0x9C6
#define CHG_CLOCK_MODE_SHIFT        3
#define CHG_CLOCK_MODE_MSK          (0x1 << CHG_CLOCK_MODE_SHIFT)

struct opt_regs {
	u32 reg;
	u32 mask;
	u32 val;
};

struct scharger_spmi_priv {
	struct device *dev;
	struct regmap *regmap;
	spinlock_t lock;
	struct irq_domain *domain;
	int gpio;
	int irq;
	int irqnum;
	int irqarray_num;
	unsigned int *irqs;
	unsigned int *irq_mask_addr_arry;
	unsigned int *irq_addr_arry;
	int opt_regs_num;
	struct opt_regs *opt_regs_set;
	struct irq_chip irq_chip;
	unsigned int scharger_version;
	unsigned int chip_id;
	int irq_cnt;
	unsigned long first_irq_time;
};

#endif

