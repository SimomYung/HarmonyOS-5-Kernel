/*
 * audio pinctrl.h driver.
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

#ifndef __AUDIO_PINCTRL_H__
#define __AUDIO_PINCTRL_H__

#include <linux/platform_device.h>

struct audio_pinctrl_handler {
	struct pinctrl *pctrl;
	struct pinctrl_state *pin_default;
	struct pinctrl_state *pin_idle;
};

enum audio_pinctrl_type {
	AUDIO_PINCTRL_I2S1,
	AUDIO_PINCTRL_I2S2,
	AUDIO_PINCTRL_I2S3,
	AUDIO_PINCTRL_I2S4,
	AUDIO_PINCTRL_TYPE_MAX,
};

enum audio_pinctrl_mode {
	AUDIO_PINCTRL_DEFAULT,
	AUDIO_PINCTRL_IDLE,
	AUDIO_PINCTRL_MODE_MAX,
};

void audio_pinctrl_handler_register(enum audio_pinctrl_type pinctrl_type,
	struct audio_pinctrl_handler *pinctrl_hndl);
void audio_pinctrl_handler_unregister(enum audio_pinctrl_type pinctrl_type);
void audio_pinctrl_handler_deinit(struct platform_device *pdev);
int audio_pinctrl_handler_init(struct platform_device *pdev);
int audio_pinctrl_set_pinctrl_mode(enum audio_pinctrl_type pinctrl_type,
	enum audio_pinctrl_mode pinctrl_mode);
void audio_all_pinctrls_init(void);

#endif