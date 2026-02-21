/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device header file for code signature cert chain
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 18 22:58:10 2023
 */

#ifndef VFS_DEV_CODESIGN_H
#define VFS_DEV_CODESIGN_H

#include <sys/ioctl.h>

#define DEV_CODESIGN_MINOR				0x21
#define HM_CODESIGN_IOCTL_BASE				'k'
#define HM_CODESIGN_ADD_CERT_CHAIN_ID			1
#define HM_CODESIGN_RM_CERT_CHAIN_ID			2

#define ADD_CERT_CHAIN_CMD _IOW(HM_CODESIGN_IOCTL_BASE, HM_CODESIGN_ADD_CERT_CHAIN_ID, struct cert_chain_info_s)
#define REMOVE_CERT_CHAIN_CMD _IOW(HM_CODESIGN_IOCTL_BASE, HM_CODESIGN_RM_CERT_CHAIN_ID, struct cert_chain_info_s)

struct cert_chain_info_s {
	uint32_t signing_length;
	uint32_t issuer_length;
	uint64_t signing_ptr;
	uint64_t issuer_ptr;
	uint32_t path_len;
	uint32_t cert_type;
	uint8_t __reserved[32];
};

#endif // VFS_DEV_CODESIGN_H
