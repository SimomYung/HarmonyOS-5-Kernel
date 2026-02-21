/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: provide gpio debug function interfaces.
 * Create: 2022-3-27
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/version.h>
#include <linux/gpio.h>
#ifdef CONFIG_GPIO_HI9030
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <platform_include/basicplatform/linux/hi9030_gpio.h>

static int g_peri_gpio[64];
static int g_ao_gpio[8];
#define PERI_GPIO_BASE 384
#define AO_GPIO_BASE 448
#endif

int gpio_input_test(unsigned gpio)
{
	return gpiod_direction_input(gpio_to_desc(gpio));
}

int gpio_output_test(unsigned gpio, int value)
{
	return gpiod_direction_output(gpio_to_desc(gpio), value);
}

#ifdef CONFIG_GPIO_HI9030
static irqreturn_t gpio_irq_dbg_handle(int irq, void *data)
{
	if (data != NULL) {
		int gpio = *(int *)data;
		pr_err("%s: gpio %d irq is coming!\n ", __func__, gpio);
	}
	return IRQ_HANDLED;
}

int gpio_irq_test(unsigned gpio)
{
	int	ret;
	uint32_t irq;
	uint32_t irq_flag = IRQF_NO_SUSPEND | IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING;

	if (gpio_request(gpio, "gpiodbg")) {
		pr_err("%s: request gpio %d failed\n", __func__, gpio);
		return -EFAULT;
	}

	ret = gpio_direction_output(gpio, 0);
	if (ret) {
		pr_err("%s: gpio %d set output failed\n", __func__, gpio);
		return -EFAULT;
	}

	irq = gpio_to_irq(gpio);
	if (irq < 0) {
		pr_err("%s: gpio %d to irq failed!\n", __func__, gpio);
		return -EFAULT;
	}

	g_peri_gpio[gpio - PERI_GPIO_BASE] = gpio;
	ret = request_irq(irq, gpio_irq_dbg_handle, irq_flag,
		"gpiodbg", (void *)(&(g_peri_gpio[gpio - PERI_GPIO_BASE])));
	if (ret) {
		pr_err("%s: request gpio %d irq failed, ret = %d\n", __func__, gpio, ret);
		return -EFAULT;
	}

	pr_err("%s: test gpio %d irq end!\n", __func__, gpio);
	return 0;
}

static void ao_irq_dbg_handle(int gpioid, void *data)
{
	if (data != NULL) {
		int gpio = *(int *)data;
		pr_err("%s: gpio %d irq is coming, gpioid is %d!\n ", __func__, gpio, gpioid);
	}
	return;
}

int hi9030_ao_gpio_irq_test(unsigned gpio, int irq_flag)
{
	int ret;

	g_ao_gpio[gpio - AO_GPIO_BASE] = gpio;
	ret = hi9030_gpio_irq_request(gpio, ao_irq_dbg_handle, irq_flag,
		(void *)(&(g_ao_gpio[gpio - AO_GPIO_BASE])));
	if (ret) {
		pr_err("%s: gpio %d irq request fail\n", __func__, gpio);
		return -1;
	}

	return 0;
}
#endif
