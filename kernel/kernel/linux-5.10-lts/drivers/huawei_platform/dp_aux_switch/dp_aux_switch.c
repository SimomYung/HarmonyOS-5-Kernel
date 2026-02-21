/*
 * dp_aux_switch.c
 *
 * dp aux switch driver
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <huawei_platform/dp_aux_switch/dp_aux_switch.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <linux/pm_wakeup.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0))
#include <linux/ion.h>
#endif
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/audio/usb_analog_hs_interface.h>
#include "ana_hs_kit/ana_hs.h"

#define HWLOG_TAG dp_aux_switch
HWLOG_REGIST();

#define INVALID_GPIO   (-1)
#define SET_GPIO_HIGH  1
#define SET_GPIO_LOW   0
#define SET_PMU_HIGH   2950000
#define SET_PMU_LOW    1800000
#define INIT_DEALY     1000

struct dp_aux_switch_priv {
	bool from_fsa4476;
	bool with_uart;
	bool uart_from_pmu;
	bool dp_aux_from_v800;
	uint32_t ch_polarity;
	int aux_gpio;
	int uart_gpio;
	int en_gpio;
	int external_uart_gpio;
};

static struct dp_aux_switch_priv aux_switch_priv;
static struct regulator *g_uart_pmu;
struct device_node *g_np = NULL;
struct delayed_work aux_init_dwork;

enum aux_switch_channel_type get_aux_switch_channel(void)
{
	if (aux_switch_priv.from_fsa4476)
		return CHANNEL_FSA4476;

	return CHANNEL_SUPERSWITCH;
}
EXPORT_SYMBOL_GPL(get_aux_switch_channel);

// aux polarity switch
void dp_aux_switch_v800(uint32_t value)
{
	if (value == 0 || value == 1) {
		hwlog_info("Aux polarity: %u-%s\n", value, value ? "Flipped" : "Normal");
		gpio_direction_output(aux_switch_priv.aux_gpio, value ? 0 : 1);
		hwlog_info("%s: dp_aux_p_gpio %d\n", __func__, aux_switch_priv.aux_gpio);
		gpio_direction_output(aux_switch_priv.uart_gpio, value ? 1 : 0);
		hwlog_info("%s: dp_aux_n_gpio %d\n",__func__, aux_switch_priv.uart_gpio);
	} else {
		hwlog_info("disconnect\n");
		gpio_direction_output(aux_switch_priv.aux_gpio, SET_GPIO_LOW);
		gpio_direction_output(aux_switch_priv.uart_gpio, SET_GPIO_LOW);
	}
}

void dp_aux_switch_control(int gpio_state)
{
	if (gpio_is_valid(aux_switch_priv.en_gpio))
		gpio_direction_output(aux_switch_priv.en_gpio, gpio_state);
	else
		hwlog_err("%s: invalid gpio %d\n",
			__func__, aux_switch_priv.en_gpio);
}

void dp_aux_switch_op(uint32_t value)
{
	if (aux_switch_priv.from_fsa4476) {
		aux_switch_priv.ch_polarity = value;
		return;
	}

	if (aux_switch_priv.dp_aux_from_v800) {
		dp_aux_switch_v800(value);
		return;
	}

	dp_aux_switch_control(SET_GPIO_LOW);

	if (gpio_is_valid(aux_switch_priv.aux_gpio))
		gpio_direction_output(aux_switch_priv.aux_gpio, value);
	else
		hwlog_err("%s: invalid gpio %d\n",
			__func__, aux_switch_priv.aux_gpio);
}
EXPORT_SYMBOL_GPL(dp_aux_switch_op);

// NOTE: Don't pay attention to the name of the function!
// 1. aux-uart switch is existed: this func is switch aux or uart.
// 2. aux-uart switch is not existed: this func is dp aux enable or disable.
void dp_aux_uart_switch_enable(void)
{
	int ret;

	if (aux_switch_priv.from_fsa4476) {
		// SBU bypass switch
		if (aux_switch_priv.ch_polarity) {
			// ENN L, EN1/EN2 01
			// SBU1 to SBU2_H, SBU2 to SBU1_H
			usb_analog_hs_plug_in_out_handle(DP_PLUG_IN_CROSS);
			ana_hs_plug_handle(DP_PLUG_IN_CROSS);
			hwlog_info("%s: dp plug in cross\n", __func__);
		} else {
			// ENN L, EN1/EN2 00
			// SBU1 to SBU1_H, SBU2 to SBU2_H
			usb_analog_hs_plug_in_out_handle(DP_PLUG_IN);
			ana_hs_plug_handle(DP_PLUG_IN);
			hwlog_info("%s: dp plug in\n", __func__);
		}

		// 1. aux-uart switch is not existed.
		// 2. aux polarity switch of dp by fsa4476.
		// Then, return directly.
		if (!aux_switch_priv.with_uart)
			return;
	}

	if (aux_switch_priv.dp_aux_from_v800) {
		if (gpio_is_valid(aux_switch_priv.external_uart_gpio))
			gpio_direction_output(aux_switch_priv.external_uart_gpio, SET_GPIO_HIGH);
		else
			hwlog_err("%s: invalid gpio %d\n",
				__func__, aux_switch_priv.uart_gpio);
		return;
	}

	if (gpio_is_valid(aux_switch_priv.uart_gpio))
		gpio_direction_output(aux_switch_priv.uart_gpio, SET_GPIO_HIGH);
	else
		hwlog_err("%s: invalid gpio %d\n",
			__func__, aux_switch_priv.uart_gpio);

	if (aux_switch_priv.uart_from_pmu) {
		if (!g_uart_pmu) {
			hwlog_err("%s:g_uart_pmu is null\n", __func__);
			return;
		}

		ret = regulator_set_voltage(g_uart_pmu, SET_PMU_HIGH, SET_PMU_HIGH);
		if (ret != 0) {
			hwlog_err("%s:set pmu_uart_switch high failed.\n", __func__);
			return;
		}
	}
}
EXPORT_SYMBOL_GPL(dp_aux_uart_switch_enable);

void dp_aux_uart_switch_disable(void)
{
	int ret;

	dp_aux_switch_control(SET_GPIO_HIGH);
	if (aux_switch_priv.from_fsa4476) {
		// ENN H, EN1/EN2 00
		usb_analog_hs_plug_in_out_handle(DP_PLUG_OUT);
		ana_hs_plug_handle(DP_PLUG_OUT);
		hwlog_info("%s: dp plug out\n", __func__);

		// aux-uart switch is not existed.
		if (!aux_switch_priv.with_uart)
			return;
	}

	if (aux_switch_priv.dp_aux_from_v800) {
		dp_aux_switch_v800(INVALID_GPIO);
		if (gpio_is_valid(aux_switch_priv.external_uart_gpio))
			gpio_direction_output(aux_switch_priv.external_uart_gpio, SET_GPIO_LOW);
		else
			hwlog_err("%s: invalid gpio %d\n",
				__func__, aux_switch_priv.external_uart_gpio);
		return;
	}

	if (gpio_is_valid(aux_switch_priv.uart_gpio))
		gpio_direction_output(aux_switch_priv.uart_gpio, SET_GPIO_LOW);
	else
		hwlog_err("%s: invalid gpio %d\n",
			__func__, aux_switch_priv.uart_gpio);

	if (aux_switch_priv.uart_from_pmu) {
		if (!g_uart_pmu) {
			hwlog_err("%s:g_uart_pmu is null\n", __func__);
			return;
		}

		ret = regulator_set_voltage(g_uart_pmu, SET_PMU_LOW, SET_PMU_LOW);
		if (ret != 0) {
			hwlog_err("%s:set pmu_uart_switch low failed.\n", __func__);
			return;
		}
	}
}
EXPORT_SYMBOL_GPL(dp_aux_uart_switch_disable);

static void gpio_aux_pin_v800_init(struct work_struct *work)
{
	int ret;
	u32 value;

	hwlog_info("%s enter\n", __func__);

	if (!g_np)
		return;

	ret = of_property_read_u32(g_np, "gpio_aux_p_v800", &value);
	if (!ret)
		aux_switch_priv.aux_gpio = value;
	else
		aux_switch_priv.aux_gpio = INVALID_GPIO;

	ret = of_property_read_u32(g_np, "gpio_aux_n_v800", &value);
	if (!ret)
		aux_switch_priv.uart_gpio = value;
	else
		aux_switch_priv.uart_gpio = INVALID_GPIO;

	hwlog_info("%s: get aux switch gpio %d,%d\n", __func__,
		aux_switch_priv.aux_gpio, aux_switch_priv.uart_gpio);

	ret = gpio_request(aux_switch_priv.aux_gpio, "dp_aux_p_gpio");
	if (ret < 0) {
		hwlog_err("%s: request gpio %d failed %d\n",
			__func__, aux_switch_priv.aux_gpio, ret);
		aux_switch_priv.uart_gpio = INVALID_GPIO;
	} else {
		gpio_direction_output(aux_switch_priv.aux_gpio, SET_GPIO_LOW);
	}

	ret = gpio_request(aux_switch_priv.uart_gpio, "dp_aux_n_gpio");
	if (ret < 0) {
		hwlog_err("%s: request gpio %d failed %d\n",
			__func__, aux_switch_priv.uart_gpio, ret);
		aux_switch_priv.uart_gpio = INVALID_GPIO;
	} else {
		gpio_direction_output(aux_switch_priv.uart_gpio, SET_GPIO_LOW);
	}

	aux_switch_priv.external_uart_gpio  = of_get_named_gpio(g_np, "uart-gpio", 0);
	if (!gpio_is_valid(aux_switch_priv.external_uart_gpio)) {
		hwlog_err("%s: invalid external uart gpio %d\n",
			__func__, aux_switch_priv.external_uart_gpio);
		return;
	}
	hwlog_info("%s: get external uart gpio %d\n", __func__,
		aux_switch_priv.external_uart_gpio);

	ret = gpio_request(aux_switch_priv.external_uart_gpio, "dp_external_uart_gpio");
	if (ret < 0)
		hwlog_err("%s: request  external uart gpio %d failed %d\n",
			__func__, aux_switch_priv.external_uart_gpio, ret);
	else
		gpio_direction_output(aux_switch_priv.external_uart_gpio, SET_GPIO_LOW);
}

static void pmu_uart_switch_init(struct device *dev)
{
	int ret;

	g_uart_pmu = devm_regulator_get(dev, "pmu_uart_switch");
	if (IS_ERR(g_uart_pmu)) {
		hwlog_err("%s:pmu_uart_switch regulator get fail\n", __func__);
		g_uart_pmu = NULL;
		return;
	}

	ret = regulator_set_voltage(g_uart_pmu, SET_PMU_LOW, SET_PMU_LOW);
	if (ret != 0) {
		hwlog_err("%s:set pmu_uart_switch low failed.\n", __func__);
		return;
	}
}

static void dp_aux_switch_en_gpio_init(struct device_node *np)
{
	int ret;

	aux_switch_priv.en_gpio  = of_get_named_gpio(np, "en-gpios", 0);
	hwlog_info("%s: get aux switch en gpio %d\n", __func__,
		aux_switch_priv.en_gpio);

	if (!gpio_is_valid(aux_switch_priv.en_gpio)) {
		hwlog_err("%s: invalid gpio %d\n",
			__func__, aux_switch_priv.en_gpio);
		return;
	}

	ret = gpio_request(aux_switch_priv.en_gpio, "dp_en_gpio");
	if (ret < 0)
		hwlog_err("%s: request gpio %d failed %d\n",
			__func__, aux_switch_priv.en_gpio, ret);
	else
		gpio_direction_output(aux_switch_priv.en_gpio, SET_GPIO_HIGH);
}

static void dp_aux_switch_gpio_init(struct device_node *np)
{
	int ret;

	aux_switch_priv.aux_gpio  = of_get_named_gpio(np, "cs-gpios", 0);
	aux_switch_priv.uart_gpio = of_get_named_gpio(np, "cs-gpios", 1);
	hwlog_info("%s: get aux switch gpio %d,%d\n", __func__,
		aux_switch_priv.aux_gpio, aux_switch_priv.uart_gpio);

	if (!gpio_is_valid(aux_switch_priv.aux_gpio)) {
		hwlog_err("%s: invalid gpio %d\n",
			__func__, aux_switch_priv.aux_gpio);
		return;
	}

	ret = gpio_request(aux_switch_priv.aux_gpio, "dp_aux_gpio");
	if (ret < 0)
		// NOTE:
		// For blanc, here gpio_request failed, because of gpio
		// requested by fsa4476. But, aux uart_gpio
		// need gpio_request, so don't goto return.
		hwlog_err("%s: request gpio %d failed %d\n",
			__func__, aux_switch_priv.aux_gpio, ret);
	else
		gpio_direction_output(aux_switch_priv.aux_gpio, SET_GPIO_LOW);

	if (!gpio_is_valid(aux_switch_priv.uart_gpio)) {
		hwlog_err("%s: invalid gpio %d\n",
			__func__, aux_switch_priv.uart_gpio);
		return;
	}

	ret = gpio_request(aux_switch_priv.uart_gpio, "dp_aux_uart_gpio");
	if (ret < 0)
		hwlog_err("%s: request gpio %d failed %d\n",
			__func__, aux_switch_priv.uart_gpio, ret);
	else
		gpio_direction_output(aux_switch_priv.uart_gpio, SET_GPIO_LOW);
}

static int dp_aux_switch_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int gpio;
	int ret;

	aux_switch_priv.from_fsa4476 = false;
	aux_switch_priv.dp_aux_from_v800 = false;
	aux_switch_priv.with_uart = false;
	aux_switch_priv.uart_from_pmu = false;
	aux_switch_priv.ch_polarity = 0;
	aux_switch_priv.aux_gpio = INVALID_GPIO;
	aux_switch_priv.uart_gpio = INVALID_GPIO;
	aux_switch_priv.en_gpio = INVALID_GPIO;
	aux_switch_priv.external_uart_gpio = INVALID_GPIO;

	g_np = dev->of_node;
	/*
	 * Both the third-party PD and V800 are available.
	 * The third-party PD is responsible for VDM communication
	 * and invokes V800 to implement AUX switching.
	 */
	if (of_property_read_bool(dev->of_node, "dp_aux_from_v800")) {
		aux_switch_priv.dp_aux_from_v800 = true;
		hwlog_info("%s: dp aux from V800\n", __func__);
		INIT_DELAYED_WORK(&aux_init_dwork, gpio_aux_pin_v800_init);
		schedule_delayed_work(&aux_init_dwork, msecs_to_jiffies(INIT_DEALY));
		return 0;
	}

	if (of_property_read_bool(dev->of_node, "aux_switch_from_fsa4476")) {
		aux_switch_priv.from_fsa4476 = true;
		hwlog_info("%s: aux switch from fsa4476\n", __func__);

		if (of_property_read_bool(dev->of_node, "aux_switch_with_uart")) {
			aux_switch_priv.with_uart = true;
			hwlog_info("%s: aux switch with uart\n", __func__);
		}
	}

	if (of_property_read_bool(dev->of_node, "uart_switch_from_pmu")) {
		aux_switch_priv.uart_from_pmu = true;
		hwlog_info("%s: uart switch from pmu\n", __func__);
		pmu_uart_switch_init(&pdev->dev);
	}

	// use two switches, not use fsa4476
	if (!aux_switch_priv.from_fsa4476) {
		dp_aux_switch_gpio_init(dev->of_node);
		dp_aux_switch_en_gpio_init(dev->of_node);
		return 0;
	}

	// aux switch from fsa4476, whether use aux/uart switch or not
	if (!aux_switch_priv.with_uart)
		return 0;

	gpio = of_get_named_gpio(dev->of_node, "aux_uart-gpio", 0);
	if (!gpio_is_valid(gpio)) {
		hwlog_err("%s: get aux_uart-gpio failed %d\n", __func__, gpio);
		return 0;
	}

	// request aux uart gpio
	ret = gpio_request(gpio, "dp_aux_uart_gpio");
	if (ret < 0) {
		hwlog_err("%s: request uart gpio failed %d\n", __func__, ret);
	} else {
		gpio_direction_output(gpio, SET_GPIO_LOW);
		aux_switch_priv.uart_gpio = gpio;
		hwlog_info("%s: init uart gpio success %d\n", __func__, gpio);
	}

	return 0;
}

static int dp_aux_switch_remove(struct platform_device *pdev)
{
	if (!aux_switch_priv.from_fsa4476) {
		if (gpio_is_valid(aux_switch_priv.aux_gpio)) {
			gpio_free((unsigned int)aux_switch_priv.aux_gpio);
			aux_switch_priv.aux_gpio = INVALID_GPIO;
		}

		if (gpio_is_valid(aux_switch_priv.uart_gpio)) {
			gpio_free((unsigned int)aux_switch_priv.uart_gpio);
			aux_switch_priv.uart_gpio = INVALID_GPIO;
		}

		if (gpio_is_valid(aux_switch_priv.en_gpio)) {
			gpio_free((unsigned int)aux_switch_priv.en_gpio);
			aux_switch_priv.en_gpio = INVALID_GPIO;
		}

		return 0;
	}

	if (aux_switch_priv.with_uart) {
		if (gpio_is_valid(aux_switch_priv.uart_gpio)) {
			gpio_free((unsigned int)aux_switch_priv.uart_gpio);
			aux_switch_priv.uart_gpio = INVALID_GPIO;
		}
	}

	if (aux_switch_priv.dp_aux_from_v800) {
		if (gpio_is_valid(aux_switch_priv.external_uart_gpio)) {
			gpio_free((unsigned int)aux_switch_priv.external_uart_gpio);
			aux_switch_priv.external_uart_gpio = INVALID_GPIO;
		}
	}

	return 0;
}

static const struct of_device_id dp_aux_switch_match[] = {
	{ .compatible = "huawei,dp_aux_switch", },
	{},
};

static struct platform_driver dp_aux_switch_driver = {
	.driver = {
		.name = "dp_aux_switch",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(dp_aux_switch_match),
	},
	.probe  = dp_aux_switch_probe,
	.remove = dp_aux_switch_remove,
};

static int __init dp_aux_switch_init(void)
{
	int ret;

	hwlog_info("%s: enter\n", __func__);
	ret = platform_driver_register(&dp_aux_switch_driver);
	if (ret < 0) {
		hwlog_err("%s: register driver failed %d\n", __func__, ret);
		return ret;
	}

	hwlog_info("%s: success\n", __func__);
	return 0;
}

static void __exit dp_aux_switch_exit(void)
{
	hwlog_info("%s: enter\n", __func__);
	platform_driver_unregister(&dp_aux_switch_driver);
}

module_init(dp_aux_switch_init);
module_exit(dp_aux_switch_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Huawei dp aux driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
