/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: MP12 gpio board-level implementation header file.
 * Author: @CompanyNameTag
 */

#ifndef GPIO_MP12_H
#define GPIO_MP12_H
#include <linux/platform_device.h>

#include "plat_gpio.h"
#include "oal_types.h"

int32_t mp12_gpio_init(struct platform_device *pdev);
void mp12_gpio_remove(struct platform_device *pdev);
#endif
