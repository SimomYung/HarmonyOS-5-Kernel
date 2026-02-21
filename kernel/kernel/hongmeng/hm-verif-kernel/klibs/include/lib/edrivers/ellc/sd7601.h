/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header for sd7601 ellc
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 27 20:08:03 2021
 */

#ifndef LIB_EDRIVERS_ELLC_SD7601_H
#define LIB_EDRIVERS_ELLC_SD7601_H

#include <lib/edrivers/ellc.h>

struct ellc_sd7601_dev {
	struct ellc_dev edev;
};

void ellc_sd7601_init(struct ellc_sd7601_dev *sd7601_dev, paddr_t base_addr_phy);

#endif
