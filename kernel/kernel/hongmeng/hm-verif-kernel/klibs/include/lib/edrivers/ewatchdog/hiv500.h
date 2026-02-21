/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header for ewatchdog hiv500
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 27 21:37:52 2023
 */

#ifndef LIB_EDRIVERS_EWATCHDOG_HIV500_H
#define LIB_EDRIVERS_EWATCHDOG_HIV500_H

#include <hmkernel/types.h>
#include <lib/edrivers/ewatchdog.h>

struct ewatchdog_hiv500_dev {
	struct ewatchdog_dev edev;
};

void ewatchdog_hiv500_init(struct ewatchdog_hiv500_dev *hiv500_dev, paddr_t base_addr_phy);

#endif
