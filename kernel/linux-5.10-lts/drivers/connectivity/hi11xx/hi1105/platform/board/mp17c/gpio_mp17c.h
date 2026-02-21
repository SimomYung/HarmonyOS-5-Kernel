/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: MP17c gpio board-level implementation header file.
 * Author: @CompanyNameTag
 */

#ifndef GPIO_MP17C_H
#define GPIO_MP17C_H
#include <linux/platform_device.h>

#include "plat_gpio.h"
#include "oal_types.h"

int32_t mp17c_gpio_init(struct platform_device *pdev);
void mp17c_tas_gpio_reset(void);
void mp17c_gpio_remove(struct platform_device *pdev);
int32_t mp17c_gugong_subsys_power_on(uint32_t subsys);
void mp17c_gugong_subsys_power_off(uint32_t subsys);
#endif
