/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Init for serial port on stm32h7
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 26 14:39:41 2021
 */


#ifndef LIB_EDRIVERS_ECONSOLE_STM32H7_H
#define LIB_EDRIVERS_ECONSOLE_STM32H7_H

#include <lib/edrivers/econsole.h>

struct econsole_stm32h7_dev {
	struct econsole_dev edev;
};

void econsole_stm32h7_init(struct econsole_stm32h7_dev *dev, paddr_t base_addr_phy,
			   unsigned long flags);


#endif
