/*
 * audio i2s2 pinctrl driver.
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include "audio_log.h"
#include "audio_pinctrl.h"

#define LOG_TAG "audio_i2s2_pinctrl"

static struct audio_pinctrl_handler g_i2s2_pinctrl_handler;

static int i2s2_pinctrl_probe(struct platform_device *pdev)
{
	int ret;
	AUDIO_LOGI("probe start");

	platform_set_drvdata(pdev, &g_i2s2_pinctrl_handler);

	ret = audio_pinctrl_handler_init(pdev);
	if (ret < 0) {
		AUDIO_LOGE("pintctrl init failed, ret %d", ret);
		return ret;
	}

	audio_pinctrl_handler_register(AUDIO_PINCTRL_I2S2, &g_i2s2_pinctrl_handler);
	AUDIO_LOGI("probe success");
	return 0;
}

static int i2s2_pinctrl_remove(struct platform_device *pdev)
{
	audio_pinctrl_handler_deinit(pdev);
	audio_pinctrl_handler_unregister(AUDIO_PINCTRL_I2S2);
	return 0;
}

static const struct of_device_id g_i2s2_pinctrl_match[] = {
    { .compatible = "hisilicon,audio-pinctrl-i2s2", },
	{},
};

static struct platform_driver g_i2s2_pinctrl = {
	.driver = {
		.name  = "audio-i2s2-pinctrl",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(g_i2s2_pinctrl_match),
	},
	.probe  = i2s2_pinctrl_probe,
	.remove = i2s2_pinctrl_remove,
};

static int __init i2s2_pinctrl_init(void)
{
	AUDIO_LOGI("init enter");
	return platform_driver_register(&g_i2s2_pinctrl);
}

fs_initcall(i2s2_pinctrl_init);

static void __exit i2s2_pinctrl_exit(void)
{
	platform_driver_unregister(&g_i2s2_pinctrl);
}

module_exit(i2s2_pinctrl_exit);

MODULE_DESCRIPTION("audio i2s2 pinctrl driver");
MODULE_LICENSE("GPL");
