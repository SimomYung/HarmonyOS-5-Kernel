/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Header for ewatchdog hi121x(hi1210/hi1215)
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 28 13:17:45 2021
 */

#ifndef LIB_EDRIVERS_EWATCHDOG_Hi121X_H
#define LIB_EDRIVERS_EWATCHDOG_Hi121X_H

#include <lib/edrivers/ewatchdog.h>
#include <hmkernel/types.h>

struct ewatchdog_hi121x_pin_info {
	void __iomem* base_addr;

	u32 pin;
	u32 pulse_width;
	u32 last_width;
	u32 gpio_pin;
	u32 reset_pin;
};

#define EWATCHDOG_HI121X_MAX_PULSE_NUM	5u

struct ewatchdog_hi121x_dev {
	struct ewatchdog_dev edev;
	struct ewatchdog_hi121x_pin_info wdt_array[EWATCHDOG_HI121X_MAX_PULSE_NUM];
	unsigned int pulse_num;
	unsigned int wdog_clr_reg_offset;
	unsigned int pin;
};

void ewatchdog_hi121x_init(struct ewatchdog_hi121x_dev *hi121x_dev, paddr_t base_addr_phy);

#endif
