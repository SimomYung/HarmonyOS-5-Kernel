/*
 * vendorcountry_state.c
 *
 * vendorcountry_state driver
 *
 * Copyright (c) 2024-2025 Huawei Technologies Co., Ltd.
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

#include "vendorcountry_state.h"

#include <huawei_platform/log/hw_log.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/init.h>
#include <securec.h>

#define HWLOG_TAG vendorcountry_state
#define MOD_NAME "vendorcountry_state"

static char vendorcountry[VENDORCOUNTRY_STATE_MAX_LENGTH] = {0};

HWLOG_REGIST();

static long vendorcountry_state_ioctl(unsigned int cmd, unsigned long arg)
{
    long ret = 0;
    void __user *data = (void __user *)(uintptr_t)arg;

    if (vendorcountry[0] == '\0') {
        if (!saved_command_line) {
            hwlog_err("%s: saved_command_line invalid argument", __func__);
            return -EFAULT;
        }
        char cmdline[CMDLINE_MAX_LENGTH] = {0};
        ret = strcpy_s(cmdline, sizeof(cmdline), saved_command_line);
        if (ret != EOK) {
            hwlog_err("%s: strcpy_s fail", __func__);
            return -EFAULT;
        }
        const char *tmp = strstr(cmdline, "vendorcountry=");
        if (!tmp) {
            hwlog_err("%s: strstr fail", __func__);
            return -EFAULT;
        }
        int state = sscanf_s(tmp, "vendorcountry=%[^ ]s", &vendorcountry, VENDORCOUNTRY_STATE_MAX_LENGTH);
        if (state != 1) { // if sscanf_s success, get 1 value
            hwlog_err("%s: sscanf_s fail", __func__);
            return -EFAULT;
        }
    }

    if (cmd == VENDORCOUNTRY_STATE_READ) {
        ret = copy_to_user(data, &vendorcountry, sizeof(vendorcountry));
        if (ret) {
            hwlog_err("%s: copy_to_user fail", __func__);
            ret = -EFAULT;
        }
    } else {
        hwlog_err("%s: not support command = 0x%x", __func__, cmd);
        return -EINVAL;
    }
    return ret;
}

static void vendorcountry_state_remove(void *priv)
{
    (void)(priv);
}

static const struct audio_misc_module_ops vendorcountry_state_ops = {
	.ioctl = vendorcountry_state_ioctl,
	.remove = vendorcountry_state_remove,
};

int vendorcountry_state_probe(struct device *dev, struct device_node *np)
{
    int ret;

    hwlog_info("%s: in\n", __func__);
    ret = audio_misc_module_constructor(MOD_NAME, NULL, VENDORCOUNTRY_STATE_IOCTL_TYPE, 
        &vendorcountry_state_ops);
	if (ret) {
		hwlog_err("%s: constructor vendorcountry_state fail", __func__);
		ret = -ENOMEM;
	}

	return ret;
}