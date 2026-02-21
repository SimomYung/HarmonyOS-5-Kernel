/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Header for ewatchdog sd6219e
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 28 14:38:18 2021
 */

#ifndef LIB_EDRIVERS_EWATCHDOG_SD6219E_H
#define LIB_EDRIVERS_EWATCHDOG_SD6219E_H

#include <lib/edrivers/ewatchdog.h>
#include <hmkernel/types.h>

struct ewatchdog_sd6219e_dev {
	struct ewatchdog_dev edev;
	u32 level;
};

void ewatchdog_sd6219e_init(struct ewatchdog_sd6219e_dev *sd6219e_dev,
			    paddr_t base_addr_phy);

#endif
