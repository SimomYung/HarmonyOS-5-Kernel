/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller ep logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_EP_H
#define HI_PCIE_CTRL_EP_H

#include "hi_pcie_ctrl.h"

#define PCIE_EP_MSI_CAP 0x80
#define PCIE_EP_MSIX_CAP 0xa0

int hi_pcie_ctrl_setup_ep(struct hi_pcie_ctrl *ctrl);

#endif
