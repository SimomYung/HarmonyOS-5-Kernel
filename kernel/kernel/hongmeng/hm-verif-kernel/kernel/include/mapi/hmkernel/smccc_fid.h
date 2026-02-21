/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Interfaces of arm smccc fid
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 2 20:01:48 2025
 */
#ifndef MAPI_HMKERNEL_SMCCC_FID_H
#define MAPI_HMKERNEL_SMCCC_FID_H

#define FID_HIBERNATE_GROUP    0x24
#define make_fid_smc(group, id)        ((((0xc3U << 16) + group) << 8) + id)

#define BL31_HIBERNATE_ENCRYPT_KERNEL  make_fid_smc(FID_HIBERNATE_GROUP, 0x16)
#define BL31_HIBERNATE_DECRYPT_KERNEL  make_fid_smc(FID_HIBERNATE_GROUP, 0x17)
#endif
