/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi helper interface
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_HELPER_H
#define I2A_HELPER_H
#include <linux/platform_device.h>

void __iomem *devm_ioremap_named_iomem(struct platform_device *pdev, const char *name);

#endif /* end of include guard: I2A_HELPER_H */
