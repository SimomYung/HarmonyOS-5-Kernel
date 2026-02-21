/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller rc logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_RC_H
#define HI_PCIE_CTRL_RC_H

#include "hi_pcie_ctrl.h"

int pcie_ctrl_setup_rc(struct hi_pcie_ctrl *ctrl);
int pcie_ctrl_reset_port(struct hi_pcie_port *port);
int pcie_ctrl_unreset_port(struct hi_pcie_port *port);
void pcie_ctrl_setup_outbound(struct hi_pcie *pcie, u32 index, u32 base, u32 target, u32 size);
void pcie_ctrl_setup_inbound(struct hi_pcie *pcie, u32 index, u32 base, u32 target, u32 size);
void pcie_setup_tx_atu(struct hi_pcie_ctrl *ctrl, u32 idx, u64 src, u64 dst, u32 size);

#endif
