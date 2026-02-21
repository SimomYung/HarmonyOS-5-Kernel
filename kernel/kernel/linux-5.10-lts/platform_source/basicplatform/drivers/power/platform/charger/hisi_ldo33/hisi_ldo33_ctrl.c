/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Device driver for hisi ldo33
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regmap.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>

#include "./hisi_ldo33_ctrl.h"

#define ldo33_err(fmt, args...)      pr_err("[hisi_ldo33]" fmt, ## args)
#define ldo33_evt(fmt, args...)      pr_warn("[hisi_ldo33]" fmt, ## args)
#define ldo33_inf(fmt, args...)      pr_info("[hisi_ldo33]" fmt, ## args)
#define ldo33_dbg(fmt, args...)      pr_debug("[hisi_ldo33]" fmt, ## args)

struct hisi_ldo33_ops *g_ldo33_ops[LDO33_MAX_NUM];

struct hisi_ldo33_ops *hisi_get_ldo33_ops(enum ldo33_index index)
{
	if (index >= LDO33_MAX_NUM)
		return NULL;

	return g_ldo33_ops[index];
}

void hisi_ldo33_enable(enum ldo33_index index, enum ldo33_user user, unsigned int enable)
{
	int i;
	struct hisi_ldo33_ops *ldo33_ops = hisi_get_ldo33_ops(index);

	if (ldo33_ops == NULL || ldo33_ops->ldo33_enable == NULL) {
		ldo33_err("%s ldo33_ops or ldo33_enable is null\n", __func__);
		return;
	}

	if (user >= LDO33_USER_MAX || user < 0) {
		ldo33_err("%s user is invalid\n", __func__);
		return;
	}

	mutex_lock(&ldo33_ops->ldo33_en_lock);
	if (enable) {
		ldo33_ops->ldo33_state[user] = 1;
		if (ldo33_ops->ldo33_en == 1) {
			ldo33_inf("%s ldo33 already enable\n", __func__);
		} else {
			ldo33_ops->ldo33_enable(1, ldo33_ops->dev_data);
			ldo33_ops->ldo33_en = 1;
		}
	} else {
		ldo33_ops->ldo33_state[user] = 0;
		for (i = 0 ; i < LDO33_USER_MAX ; i++) {
			if(ldo33_ops->ldo33_state[i] == 1) {
				ldo33_inf("%s user%d still need ldo33 enable\n", __func__, i);
				break;
			}
		}
		if (i == LDO33_USER_MAX) {
			ldo33_inf("%s no user need ldo33 enable\n", __func__);
			ldo33_ops->ldo33_enable(0, ldo33_ops->dev_data);
			ldo33_ops->ldo33_en = 0;
		}
	}
	mutex_unlock(&ldo33_ops->ldo33_en_lock);
}

void hisi_ldo33_register(enum ldo33_index index, struct hisi_ldo33_ops *ops)
{
	if (ops == NULL || index >= LDO33_MAX_NUM) {
		ldo33_err("%s ops is null or index %d is error\n", __func__, index);
		return;
	}
	g_ldo33_ops[index] = ops;

	mutex_init(&g_ldo33_ops[index]->ldo33_en_lock);
}
