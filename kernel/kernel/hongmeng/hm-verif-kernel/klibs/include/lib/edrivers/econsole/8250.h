/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: econsole 8250
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 26 10:32:17 2021
 */

#ifndef LIB_EDRIVER_ECONSOLE_8250_H
#define LIB_EDRIVER_ECONSOLE_8250_H

#include <lib/edrivers/econsole.h>

struct econsole_8250_dev {
	struct econsole_dev edev;
};

void econsole_8250_init(struct econsole_8250_dev *dev, paddr_t base_addr_phy,
			unsigned long flags);
#endif
