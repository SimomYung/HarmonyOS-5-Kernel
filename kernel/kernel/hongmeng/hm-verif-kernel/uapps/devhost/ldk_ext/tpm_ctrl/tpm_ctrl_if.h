/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes and type definitions for LDK tpmctrl APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 12 10:51:12 2023
 */
#ifndef __DEVHOST_PLUGIN_TPM_CTRL_IF_H__
#define __DEVHOST_PLUGIN_TPM_CTRL_IF_H__

#include <tpm_ctrl.h>

struct tpm_ctrl_ops {
	int (*pcr_extend)(int pcr, size_t size, uint8_t *digest);
	int (*pcr_read)(int pcr, size_t size, uint8_t *digest); /* digest is a pointer from user */
};

int devhost_register_tpm_ctrl(struct tpm_ctrl_ops *tpm_ops);

#endif
