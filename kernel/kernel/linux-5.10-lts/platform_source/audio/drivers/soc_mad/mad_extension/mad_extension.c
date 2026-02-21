/*
 * mad extension driver.
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

#include "mad_extension.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/hwspinlock.h>
#include "audio_log.h"

#define LOG_TAG "mad_extension"

#define PINCTRL_STATE_NORMAL_1 "normal_1"

#define HWSPIN_LOCK_WAIT_TIMEOUT 10

enum mad_ex_mode {
	HIGH_FREQ_MODE,
	LOW_FREQ_MODE,
	MODE_CNT,
};

struct mad_ex_priv {
	struct pinctrl *pctrl;
	struct pinctrl_state *pin_default;
	struct pinctrl_state *pin_idle;
	struct pinctrl_state *pin_normal_1;
	unsigned long pinctrl_state;
	unsigned int mad_mode;
	struct hwspinlock *hwlock;
	bool get_hwspin_lock_failed;
};

static struct mad_ex_priv g_mad_ex_data;

static const struct of_device_id g_mad_ex_match[] = {
	{ .compatible = "hisilicon,mad-extension", },
	{},
};

static int mad_ex_try_set_pinctrl_state(struct pinctrl *pctrl, struct pinctrl_state *state)
{
	int ret = hwspin_lock_timeout(g_mad_ex_data.hwlock, HWSPIN_LOCK_WAIT_TIMEOUT);
	if (ret) {
		AUDIO_LOGE("get hwspin_lock timeout");
		g_mad_ex_data.get_hwspin_lock_failed = true;
		return -EPERM;
	}

	ret = pinctrl_select_state(pctrl, state);
	AUDIO_LOGI("pinctrl_select_state, ret: %d", ret);
	hwspin_unlock(g_mad_ex_data.hwlock);
	return ret;
}

static int mad_ex_select_mode(void)
{
	if (g_mad_ex_data.pctrl == NULL) {
		AUDIO_LOGW("pin ctrl is null");
		return -EFAULT;
	}

	if ((g_mad_ex_data.pin_default == NULL) || (g_mad_ex_data.pin_idle == NULL)) {
		AUDIO_LOGE("pin_default or pin_idle is null");
		return -EFAULT;
	}

	if (test_bit(HIGH_FREQ_MODE, &g_mad_ex_data.pinctrl_state) != 0) {
		if (g_mad_ex_data.get_hwspin_lock_failed) {
			g_mad_ex_data.get_hwspin_lock_failed = false;
			(void)mad_ex_try_set_pinctrl_state(g_mad_ex_data.pctrl, g_mad_ex_data.pin_idle);
		}
		return mad_ex_try_set_pinctrl_state(g_mad_ex_data.pctrl, g_mad_ex_data.pin_default);
	}

	if (test_bit(LOW_FREQ_MODE, &g_mad_ex_data.pinctrl_state)) {
		/* pin_normal_1 is NULL means it supports only two states(pin_idle,pin_default) */
		if (g_mad_ex_data.pin_normal_1 == NULL)
			return mad_ex_try_set_pinctrl_state(g_mad_ex_data.pctrl, g_mad_ex_data.pin_idle);

		return mad_ex_try_set_pinctrl_state(g_mad_ex_data.pctrl, g_mad_ex_data.pin_normal_1);
	}

	return mad_ex_try_set_pinctrl_state(g_mad_ex_data.pctrl, g_mad_ex_data.pin_idle);
}

int mad_ex_request_pinctrl_state(unsigned int mode)
{
	set_bit(mode, &g_mad_ex_data.pinctrl_state);

	AUDIO_LOGI("pinctrl_state: %lu", g_mad_ex_data.pinctrl_state);

	return mad_ex_select_mode();
}

int mad_ex_release_pinctrl_state(unsigned int mode)
{
	clear_bit(mode, &g_mad_ex_data.pinctrl_state);

	AUDIO_LOGI("pinctrl_state: %lu", g_mad_ex_data.pinctrl_state);

	return mad_ex_select_mode();
}

void mad_ex_set_pinctrl_state(unsigned int mode)
{
	int ret = 0;

	if (mode >= MODE_CNT) {
		AUDIO_LOGE("pinctrl state error: %d", mode);
		return;
	}

	if (g_mad_ex_data.pctrl == NULL) {
		AUDIO_LOGW("pin ctrl is null");
		return;
	}

	AUDIO_LOGI("soc mad set pin state mode: %d", mode);

	if (mode == HIGH_FREQ_MODE)
		ret = mad_ex_try_set_pinctrl_state(g_mad_ex_data.pctrl, g_mad_ex_data.pin_default);
	else if (mode == LOW_FREQ_MODE)
		ret = mad_ex_try_set_pinctrl_state(g_mad_ex_data.pctrl, g_mad_ex_data.pin_idle);
	else
		AUDIO_LOGE("invalid mode: %d", mode);

	if (ret)
		AUDIO_LOGE("soc mad set pinctrl state error, ret: %d, mode: %d", ret, mode);
}

static void mad_ex_pinctrl_deinit(struct platform_device *pdev)
{
	struct mad_ex_priv *priv = platform_get_drvdata(pdev);

	devm_pinctrl_put(priv->pctrl);
	priv->pctrl = NULL;
	priv->pin_default = NULL;
	priv->pin_idle = NULL;
	priv->pin_normal_1 = NULL;

	if (g_mad_ex_data.hwlock && hwspin_lock_free(g_mad_ex_data.hwlock) < 0) {
		AUDIO_LOGE("free hwlock failed");
	}
	g_mad_ex_data.hwlock = NULL;

	AUDIO_LOGI("pinctrl deinit ok");
}

static int mad_ex_pinctrl_init(struct platform_device *pdev)
{
	struct pinctrl *pin = NULL;
	struct pinctrl_state *pin_state = NULL;
	struct mad_ex_priv *priv = platform_get_drvdata(pdev);

	pin = pinctrl_get(&pdev->dev);
	if (IS_ERR(pin)) {
		AUDIO_LOGE("can not get pinctrl");
		return -EFAULT;
	}
	priv->pctrl = pin;

	pin_state = pinctrl_lookup_state(pin, PINCTRL_STATE_DEFAULT);
	if (IS_ERR(pin_state)) {
		AUDIO_LOGE("can not get default state (%li)", PTR_ERR(pin_state));
		goto pin_init_err;
	}
	priv->pin_default = pin_state;

	pin_state = pinctrl_lookup_state(pin, PINCTRL_STATE_IDLE);
	if (IS_ERR(pin_state)) {
		AUDIO_LOGE("can not get idle state (%li)", PTR_ERR(pin_state));
		goto pin_init_err;
	}
	priv->pin_idle = pin_state;

	pin_state = pinctrl_lookup_state(pin, PINCTRL_STATE_NORMAL_1);
	if (IS_ERR(pin_state)) {
		AUDIO_LOGW("can not get normal_l state (%li)", PTR_ERR(pin_state));
		priv->pin_normal_1 = NULL;
	} else {
		priv->pin_normal_1 = pin_state;
	}

	AUDIO_LOGI("mad_ex pinctrl init ok");

	return 0;

pin_init_err:
	mad_ex_pinctrl_deinit(pdev);

	return -EFAULT;
}

static int mad_ex_probe(struct platform_device *pdev)
{
	int ret;
	unsigned int mad_hwspin_lock_id = 0;
	g_mad_ex_data.pinctrl_state = 0;
	g_mad_ex_data.get_hwspin_lock_failed = false;

	AUDIO_LOGI("mad_ex driver probe start");
	platform_set_drvdata(pdev, &g_mad_ex_data);

	ret = mad_ex_pinctrl_init(pdev);
	if (ret) {
		AUDIO_LOGE("mad_ex pin init failed, ret: %d", ret);
		return ret;
	}

	ret = of_property_read_u32(pdev->dev.of_node, "mad_hwspin_lock_id", &mad_hwspin_lock_id);
	if (ret != 0) {
		AUDIO_LOGW("read mad_hwspin_lock_id error, ret: %d", ret);
		return ret;
	}

	g_mad_ex_data.hwlock = hwspin_lock_request_specific(mad_hwspin_lock_id);
	if (g_mad_ex_data.hwlock == NULL) {
		AUDIO_LOGE("unable to get hwlock");
		return -EFAULT;
	}

	AUDIO_LOGI("mad_ex driver probe ok");
	return 0;
}

static int mad_ex_remove(struct platform_device *pdev)
{
	mad_ex_pinctrl_deinit(pdev);
	return 0;
}


static struct platform_driver g_mad_ex_driver = {
	.driver = {
		.name  = "mad-extension",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(g_mad_ex_match),
	},
	.probe  = mad_ex_probe,
	.remove = mad_ex_remove,
};

static int __init mad_ex_init(void)
{
	AUDIO_LOGI("mad_ex_init");
	return platform_driver_register(&g_mad_ex_driver);
}

module_init(mad_ex_init);

static void __exit mad_ex_exit(void)
{
	platform_driver_unregister(&g_mad_ex_driver);
}

module_exit(mad_ex_exit);

MODULE_DESCRIPTION("audio soc mad ex driver");
MODULE_LICENSE("GPL");

