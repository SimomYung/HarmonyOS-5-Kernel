/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller interrupt logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_INT_H
#define HI_PCIE_CTRL_INT_H

#include "hi_pcie_ctrl.h"

irqreturn_t hi_pcie_local_irq_handler(int irq, void *arg);

#endif
