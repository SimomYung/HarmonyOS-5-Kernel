/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: usb phy chip basic interface defination
 * Author: Hisilicon
 * Create: 2023-12-5
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */
#ifndef __HI9030_GPIO__
#define __HI9030_GPIO__

enum gpiomux_mode {
	PINMUX_M0 = 0,
	PINMUX_M1,
	PINMUX_M2,
	PINMUX_M3,
	PINMUX_M4,
	PINMUX_M5,
	PINMUX_M6,
	PINMUX_M7,
};

int hi9030_gpio_irq_request(uint32_t, void(*)(int, void *), uint32_t, void *);
int hi9030_gpio_set_irq_type(uint32_t, int);
int hi9030_gpio_irq_free(uint32_t);
int hi9030_config_iomg(unsigned int gpio_id, enum gpiomux_mode mode);
#endif /* __HI9030_GPIO__ */
