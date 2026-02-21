/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * pmic_vbus.h
 *
 * Device driver for PMU VBUS interrupts
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

#ifndef _PMIC_VBUS_H_
#define _PMIC_VBUS_H_

#include <linux/types.h>
#include <linux/regmap.h>
#include <linux/notifier.h>
#include <linux/platform_device.h>

#define VBUS_IRQ_TYPE_NUM 2

enum vbus_irq_type{
	VBUS_PLUGIN,
	VBUS_PLUGOUT,
	VBUS_PLUGIN_AND_OUT,
	VBUS_PLUGEOF
};

struct pmic_vbus {
	struct device	*dev;
	char *parent_irq_name;
	int irq_id;
	u32 event_reg;
	u32 *event_reg_bits;
	struct regmap *reg_map;
};

int pmic_vbus_register_notifier(struct notifier_block *nb);
int pmic_vbus_unregister_notifier(struct notifier_block *nb);

#endif /* _PMIC_VBUS_H_ */
