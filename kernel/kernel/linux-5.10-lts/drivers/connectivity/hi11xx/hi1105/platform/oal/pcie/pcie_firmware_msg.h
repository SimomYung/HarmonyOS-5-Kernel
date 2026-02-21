/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie_firmware_msg.c header file
 * Author: @CompanyNameTag
 */

#ifndef PCIE_FIRMWARE_MSG_H
#define PCIE_FIRMWARE_MSG_H

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
 #include "pcie_linux.h"

#define SEPERATE_MODE      0xceed

int32_t oal_pcie_firmware_msg_read(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len,
                                   uint32_t timeout, uint32_t file_flag);
int32_t oal_pcie_firmware_msg_write(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len, uint32_t file_flag);
int32_t oal_pcie_firmware_msg_init(oal_pcie_res *pcie_res);
void oal_pcie_firmware_msg_set_ch(oal_pcie_res *pst_pcie_res);
void oal_pcie_firmware_msg_int_unmask(oal_pcie_res *pst_pci_res);
void oal_pcie_firmware_msg_int_mask(oal_pcie_res *pst_pci_res);
int32_t oal_pcie_firmware_msg_sharemem_update(oal_pcie_res *pcie_res);
int32_t oal_firmware_msg_download_pre(hcc_bus *bus);
void oal_firmware_msg_download_post(hcc_bus *bus);
#endif
#endif
