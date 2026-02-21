/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: econsole msm
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 21 15:18:02 2022
 */

#ifndef LIB_EDRIVER_ECONSOLE_MSM_H
#define LIB_EDRIVER_ECONSOLE_MSM_H

#include <lib/edrivers/econsole.h>

struct econsole_msm_dev {
	struct econsole_dev edev;
};

void econsole_msm_init(struct econsole_msm_dev *dev, paddr_t base_addr_phy,
		       unsigned long flags);
#endif
