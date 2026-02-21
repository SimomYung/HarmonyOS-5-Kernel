/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes and type definitions for tpmctrl APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 11 19:00:14 2023
 */
#ifndef __DEVHOST_API_TPMCTRL_H__
#define __DEVHOST_API_TPMCTRL_H__

#define TPM_CTRL_PCR_EXTEND 0x1
#define TPM_CTRL_PCR_READ   0x2

struct tpm_ioctl_args {
	uint8_t *digest;
	size_t size;
	int pcr;
};

#endif
