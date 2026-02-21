/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * hisi_ldo33_ctrl.h
 *
 * Hisi ldo33 contrl header.
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
#ifndef __HISI_LDO33_CTRL_H__
#define __HISI_LDO33_CTRL_H__

#include <linux/of_device.h>

enum ldo33_user {
	EFUSE_WR = 0,
	GATE1_CTRL,
	GATE2_CTRL,
	HKADC_DET,
	LDO33_USER_MAX,
};

#define LDO33_MAX_NUM          2
enum ldo33_index {
	SCHARGER_V800_IC = 0,
	SCHARGER_V860_IC,
	SCHARGER_IC_TOTAL,
};

struct hisi_ldo33_ops {
	void *dev_data;
	void (*ldo33_enable)(unsigned int enable, void *dev_data);
	int ldo33_state[LDO33_USER_MAX];
	struct mutex ldo33_en_lock;
	int ldo33_en;
};

void hisi_ldo33_register(enum ldo33_index index, struct hisi_ldo33_ops *ops);
void hisi_ldo33_enable(enum ldo33_index index, enum ldo33_user user, unsigned int enable);
#endif