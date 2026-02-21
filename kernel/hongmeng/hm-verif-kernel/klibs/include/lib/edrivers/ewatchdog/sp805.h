/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header for ewatchdog sp805
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 3 02:08:40 2021
 */

#ifndef LIB_EDRIVERS_EWATCHDOG_SP805_H
#define LIB_EDRIVERS_EWATCHDOG_SP805_H

#include <hmkernel/types.h>
#include <lib/edrivers/ewatchdog.h>

struct ewatchdog_sp805_dev {
	struct ewatchdog_dev edev;
	u32 load_val;
	bool active;
};

void ewatchdog_sp805_init(struct ewatchdog_sp805_dev *sp805_dev, paddr_t base_addr_phy);

#endif
