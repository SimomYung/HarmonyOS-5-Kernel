/*
 * Huawei hi116x ringing
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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

#include <securec.h>
#include <crypto/hash.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/completion.h>
#include "hi116x_ringing.h"

static int hi116x_get_ringing_cmd(struct hi1162_device_info *di, u8 *ringing_cmd)
{
	int ret;

	if (!di) {
		sparklink_errmsg("ringing_cmd is null\n");
		return -EINVAL;
	}

	ret = sparklink_i2c_read_block(di->client,
		HI116X_RINGING_REPORT_REG, HI116X_RINGING_REG_LEN, ringing_cmd);
	if (ret < 0) {
		sparklink_errmsg("read ringing_cmd failed\n");
		return -EINVAL;
	}

	return 0;
}

static int hi116x_ringing_report_uevent(struct hi1162_device_info *di, const u8 *ringing_cmd)
{
	int ret;
	char *p_uevent_data[2] = {NULL};
	char *p_data = NULL;

	if (!di || !di->dev || !ringing_cmd) {
		sparklink_errmsg("di or dev or ringing_cmd is null\n");
		return -EINVAL;
	}

	p_data = devm_kzalloc(di->dev, HI116X_RINGING_MSG_MAX_LEN, GFP_KERNEL);
	if (!p_data)
		return -ENOMEM;

	if (ringing_cmd[HI116X_RINGING_ACK_VAL_LOC] == HI116X_RINGING_OPEN_FLAG) {
		ret = snprintf_s(p_data, HI116X_RINGING_MSG_MAX_LEN, HI116X_RINGING_MSG_MAX_LEN - 1, "%s", "CMDID=1");
	} else if (ringing_cmd[HI116X_RINGING_ACK_VAL_LOC] == HI116X_RINGING_CLOSE_FLAG) {
		ret = snprintf_s(p_data, HI116X_RINGING_MSG_MAX_LEN, HI116X_RINGING_MSG_MAX_LEN - 1, "%s", "CMDID=2");
	} else {
		sparklink_errmsg("hill6x ringing_cmd is invalid \n");
		goto RINGING_DATA_FREE;
	}

	if (ret < 0) {
		sparklink_errmsg("hill6x ringing_cmd snprintf_s failed\n");
		goto RINGING_DATA_FREE;
	}

	// 0: ringing_msg reported to FindNetwork
	p_uevent_data[0] = p_data;
	p_uevent_data[1] = NULL;

	ret = kobject_uevent_env(&di->dev->kobj, KOBJ_CHANGE, p_uevent_data);
	if (ret < 0) {
		sparklink_errmsg("hill6x ringing_cmd call kobject_uevent_env failed\n");
	}

	devm_kfree(di->dev, p_data);
	p_data = NULL;
	return 0;

RINGING_DATA_FREE:
	devm_kfree(di->dev, p_data);
	p_data = NULL;
	return -EINVAL;
}

int hi116x_report_ringing_cmd(struct hi1162_device_info *di)
{
	int ret;
	u8 ringing_cmd[HI116X_RINGING_REG_LEN] = { 0 };

	if (!di)
		return -EINVAL;

	ret = hi116x_get_ringing_cmd(di, ringing_cmd);
	if (ret < 0) {
		sparklink_errmsg("hill6x get ringing_cmd failed\n");
		return ret;
	}

	ret = hi116x_ringing_report_uevent(di, ringing_cmd);
	if (ret < 0) {
		sparklink_errmsg("ringing_cmd report uevent failed\n");
		return ret;
	}

	return 0;
}