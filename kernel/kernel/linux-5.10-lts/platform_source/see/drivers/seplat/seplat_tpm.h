/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: seplat tpm driver
 * Create : 2024/05/15
 */
#ifndef SEPLAT_TPM_H
#define SEPLAT_TPM_H

#include <linux/tpm.h>
#include <linux/device.h>
#include <linux/platform_device.h>

/* tpm_chip_alloc defined in tpm-chip.c */
struct tpm_chip *tpm_chip_alloc(struct device *pdev, const struct tpm_class_ops *ops);
/* tpm_chip_register defined in tpm-chip.c */
int tpm_chip_register(struct tpm_chip *chip);

int seplat_tpm_init(struct platform_device *pdev);

#endif /* SEPLAT_TPM_H */