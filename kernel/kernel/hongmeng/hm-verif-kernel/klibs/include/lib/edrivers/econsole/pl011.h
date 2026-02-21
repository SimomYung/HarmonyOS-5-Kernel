/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: header file for early console driver for pl011
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 26 11:38:11 2021
 */

#ifndef LIB_EDRIVER_ECONSOLE_PL011_H
#define LIB_EDRIVER_ECONSOLE_PL011_H

#include <lib/edrivers/econsole.h>

struct econsole_pl011_dev {
	struct econsole_dev edev;
};

void econsole_pl011_init(struct econsole_pl011_dev *dev, paddr_t base_addr_phy,
			 unsigned long flags);

#endif
