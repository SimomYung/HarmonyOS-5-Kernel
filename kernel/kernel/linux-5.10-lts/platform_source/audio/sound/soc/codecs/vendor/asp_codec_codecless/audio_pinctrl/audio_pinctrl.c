/*
 * audio pinctrl.c driver.
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

#include "audio_pinctrl.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/bitops.h>
#include "audio_log.h"

#define LOG_TAG "audio_pinctrl"

static struct audio_pinctrl_handler *g_audio_pinctrl_hndls[AUDIO_PINCTRL_TYPE_MAX];

void audio_pinctrl_handler_register(enum audio_pinctrl_type pinctrl_type,
	struct audio_pinctrl_handler *pinctrl_hndl)
{
	AUDIO_LOGI("register pinctrl type %u", pinctrl_type);
	if ((pinctrl_type < AUDIO_PINCTRL_TYPE_MAX) && (pinctrl_hndl != NULL) &&
		(g_audio_pinctrl_hndls[pinctrl_type] == NULL))
		g_audio_pinctrl_hndls[pinctrl_type] = pinctrl_hndl;
}

void audio_pinctrl_handler_unregister(enum audio_pinctrl_type pinctrl_type)
{
	if (pinctrl_type < AUDIO_PINCTRL_TYPE_MAX)
		g_audio_pinctrl_hndls[pinctrl_type] = NULL;
}

static struct audio_pinctrl_handler *audio_pinctrl_get_handler(enum audio_pinctrl_type pinctrl_type)
{
	if (pinctrl_type < AUDIO_PINCTRL_TYPE_MAX)
		return g_audio_pinctrl_hndls[pinctrl_type];

	return NULL;
}

void audio_pinctrl_handler_deinit(struct platform_device *pdev)
{
	struct audio_pinctrl_handler *pinctrl_hndl;

	if (pdev == NULL) {
		AUDIO_LOGE("platform device is NULL");
		return;
	}

	pinctrl_hndl = platform_get_drvdata(pdev);
	if (pinctrl_hndl == NULL) {
		AUDIO_LOGE("pinctrl handler is NULL");
		return;
	}

	devm_pinctrl_put(pinctrl_hndl->pctrl);
	pinctrl_hndl->pctrl = NULL;
	pinctrl_hndl->pin_default = NULL;
	pinctrl_hndl->pin_idle = NULL;

	AUDIO_LOGI("pinctrl handler deinit ok");
}

int audio_pinctrl_handler_init(struct platform_device *pdev)
{
	struct audio_pinctrl_handler *pinctrl_hndl;

	if (pdev == NULL) {
		AUDIO_LOGE("platform device is NULL");
		return -EFAULT;
	}

	pinctrl_hndl = platform_get_drvdata(pdev);
	if (pinctrl_hndl == NULL) {
		AUDIO_LOGE("pinctrl handler is NULL");
		return -EFAULT;
	}

	pinctrl_hndl->pctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR_OR_NULL(pinctrl_hndl->pctrl)) {
		AUDIO_LOGE("can not get pinctrl (%li)", PTR_ERR_OR_ZERO(pinctrl_hndl->pctrl));
		pinctrl_hndl->pctrl = NULL;
		return -EFAULT;
	}

	pinctrl_hndl->pin_default = pinctrl_lookup_state(pinctrl_hndl->pctrl, PINCTRL_STATE_DEFAULT);
	if (IS_ERR_OR_NULL(pinctrl_hndl->pin_default)) {
		AUDIO_LOGE("can not get default state (%li)", PTR_ERR_OR_ZERO(pinctrl_hndl->pin_default));
		goto pin_init_err;
	}

	pinctrl_hndl->pin_idle = pinctrl_lookup_state(pinctrl_hndl->pctrl, PINCTRL_STATE_IDLE);
	if (IS_ERR_OR_NULL(pinctrl_hndl->pin_idle)) {
		AUDIO_LOGE("can not get idle state (%li)", PTR_ERR_OR_ZERO(pinctrl_hndl->pin_idle));
		goto pin_init_err;
	}

	AUDIO_LOGI("pinctrl handler init ok");
	return 0;

pin_init_err:
	audio_pinctrl_handler_deinit(pdev);
	return -EFAULT;
}

static bool is_pinctrl_handler_valid(const struct audio_pinctrl_handler *pinctrl_hndl)
{
	if (pinctrl_hndl == NULL) {
		AUDIO_LOGW("pin ctrl does not exist");
		return false;
	}

	if (pinctrl_hndl->pctrl == NULL) {
		AUDIO_LOGW("pin ctrl is null");
		return false;
	}

	if ((pinctrl_hndl->pin_default == NULL) || (pinctrl_hndl->pin_idle == NULL)) {
		AUDIO_LOGW("pin_default or pin_idle is null");
		return false;
	}
	return true;
}

int audio_pinctrl_set_pinctrl_mode(enum audio_pinctrl_type pinctrl_type,
	enum audio_pinctrl_mode pinctrl_mode)
{
	struct pinctrl_state *pinctrl_state = NULL;
	struct audio_pinctrl_handler *pinctrl_hndl = audio_pinctrl_get_handler(pinctrl_type);
	if (!is_pinctrl_handler_valid(pinctrl_hndl)) {
		AUDIO_LOGW("error pinctrl_type %u", pinctrl_type);
		return -EFAULT;
	}

	switch (pinctrl_mode) {
	case AUDIO_PINCTRL_DEFAULT:
		pinctrl_state = pinctrl_hndl->pin_default;
		break;
	case AUDIO_PINCTRL_IDLE:
		pinctrl_state = pinctrl_hndl->pin_idle;
		break;
	default:
		AUDIO_LOGE("error pinctrl mode %u", pinctrl_mode);
		return -EFAULT;
	}

	AUDIO_LOGI("pinctrl type %u, state %u", pinctrl_type, pinctrl_mode);
	return pinctrl_select_state(pinctrl_hndl->pctrl, pinctrl_state);
}

void audio_all_pinctrls_init(void)
{
	int ret = 0;
	unsigned int pinctrl_type;
	for (pinctrl_type = (unsigned int)AUDIO_PINCTRL_I2S1;
		pinctrl_type < (unsigned int)AUDIO_PINCTRL_TYPE_MAX; pinctrl_type++) {
		ret = audio_pinctrl_set_pinctrl_mode(pinctrl_type, AUDIO_PINCTRL_IDLE);
		if (ret < 0)
			AUDIO_LOGW("init error pinctrl type: %u, ret: %d", pinctrl_type, ret);
	}
}