/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: check compatibility between tzdriver and tee.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "tee_info.h"
#include <linux/types.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <securec.h>
#include "tee_compat_check.h"
#include "tc_ns_log.h"
#include "tc_current_info.h"

int32_t tc_ns_get_tee_info(struct file *file, void __user *argp)
{
	int32_t ret;
	struct tc_ns_tee_info info;

	if (!argp) {
		tloge("error input parameter\n");
		return -EINVAL;
	}

	(void)file;
	ret = 0;
	(void)memset_s(&info, sizeof(info), 0, sizeof(info));
	info.tzdriver_version_major = TZDRIVER_LEVEL_MAJOR_SELF;
	info.tzdriver_version_minor = TZDRIVER_LEVEL_MINOR_SELF;
	if (copy_to_user(argp, &info, sizeof(info)) != 0)
		ret = -EFAULT;

	return ret;
}
