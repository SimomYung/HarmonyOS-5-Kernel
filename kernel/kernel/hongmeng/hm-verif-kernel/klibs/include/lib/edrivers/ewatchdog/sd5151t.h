/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header for ewatchdog sd5151t
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 20 15:46:50 2021
 */

#ifndef LIB_EDRIVERS_EWATCHDOG_SD5151T_H
#define LIB_EDRIVERS_EWATCHDOG_SD5151T_H

#include <lib/edrivers/ewatchdog.h>
#include <hmkernel/types.h>

struct ewatchdog_sd5151t_dev {
	struct ewatchdog_dev edev;
};

void ewatchdog_sd5151t_init(struct ewatchdog_sd5151t_dev *sd5151t_dev,
			    paddr_t base_addr_phy);

#ifdef __HOST_LLT__
int ewatchdog_sd5151t_init_devmap(void *args);
int ewatchdog_sd5151t_init_earlydev(void *args);
#endif

#endif
