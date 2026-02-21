/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller dfx logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_DFX_H
#define HI_PCIE_CTRL_DFX_H

#include "hi_pcie_ctrl.h"

/* for core ctrl ops */
void hi_pcie_dfx_register(struct hi_pcie *pcie);

/* for rc dfx */
void pcie_ctrl_show_link_state(struct hi_pcie_port *link);
void pcie_ctrl_dump_ltssm_tracer(struct hi_pcie_ctrl_port *link, bool reset);
/* end */

u32 pcie_ctrl_get_link_state(struct hi_pcie_port *port);
int pcie_ctrl_enter_loopback(struct hi_pcie_port *port, u32 type);
int pcie_ctrl_exit_loopback(struct hi_pcie_port *port);
void pcie_ctrl_dump_port_info(struct hi_pcie_port *port, u32 type);
void pcie_ctrl_dump_ctrl_info(struct hi_pcie *pcie, u32 type);
#endif
