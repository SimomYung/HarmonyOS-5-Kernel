/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Header for ewatchdog ipq6028
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 20 14:46:50 2022
 */

#ifndef LIB_EDRIVERS_EWATCHDOG_IPQ6028_H
#define LIB_EDRIVERS_EWATCHDOG_IPQ6028_H

#include <lib/edrivers/ewatchdog.h>
#include <hmkernel/types.h>

struct ewatchdog_ipq6028_dev {
	struct ewatchdog_dev edev;
};

void ewatchdog_ipq6028_init(struct ewatchdog_ipq6028_dev *ipq6028_dev,
			    paddr_t base_addr_phy);

#endif
