/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller dfx logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_DSM_H
#define HI_PCIE_DSM_H

#include <securec.h>
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
#include <dsm/dsm_pub.h>
#endif

void pcie_register_dsm_client(void);
void pcie_dsm_report(u32 link_status);
void pcie_dsm_record_pipe_status(u32 val);
#endif
