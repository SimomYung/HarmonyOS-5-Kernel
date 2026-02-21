/*
 * audio_mic_block.c
 *
 * audio_mic_block driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#include "audio_mic_block.h"

#include <huawei_platform/log/hw_log.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <securec.h>
#include <linux/of.h>

enum BLOCK_MODE {
    TOP_UNBLOCKED = 0x1,
    TOP_BLOCKED = 0x2,
    BACK_UNBLOCKED = 0x10,
    BACK_BLOCKED = 0x20,
    BT_UNBLOCKED = 0x100,
    BT_BLOCKED = 0x200
};

#define HWLOG_TAG audio_mic_block
static const char *MOD_NAME = "audio_mic_block";

HWLOG_REGIST();

static long audio_mic_block_ioctl(unsigned int cmd, unsigned long arg)
{
    long ret = 0;
    uint32_t block_value = 0;
 
    if (cmd == AUDIO_MIC_BLOCK_NOTIFIER) {
        ret = copy_from_user(&block_value, (uint32_t *)(uintptr_t)arg, sizeof(block_value));
        if (ret) {
            hwlog_err("%s: copy_from_user failed\n", __func__);
            return ret;
        }
    } else {
        hwlog_err("%s: not support command = 0x%x\n", __func__, cmd);
        return -EINVAL;
    }
    
    if (block_value & TOP_UNBLOCKED) {
        audio_misc_report_uevent("NAME=mic_un_blocked", "top_mic");
    } else if (block_value & TOP_BLOCKED) {
        audio_misc_report_uevent("NAME=mic_blocked", "top_mic");
    }

    if (block_value & BACK_UNBLOCKED) {
        audio_misc_report_uevent("NAME=mic_un_blocked", "back_mic");
    } else if (block_value & BACK_BLOCKED) {
        audio_misc_report_uevent("NAME=mic_blocked", "back_mic");
    }

    if (block_value & BT_UNBLOCKED) {
        audio_misc_report_uevent("NAME=mic_un_blocked", "bottom_mic");
    } else if (block_value & BT_BLOCKED) {
        audio_misc_report_uevent("NAME=mic_blocked", "bottom_mic");
    }

    hwlog_info("%s: imedia_block_value = %d\n", __func__, block_value);
    return ret;
}

static void audio_mic_block_remove(void *priv)
{
    (void)(priv);
}

static const struct audio_misc_module_ops audio_mic_block_ops = {
    .ioctl = audio_mic_block_ioctl,
    .remove = audio_mic_block_remove,
};

int audio_mic_block_probe(struct device *dev, struct device_node *np)
{
    int ret;

    hwlog_info("%s: in\n", __func__);
    ret = audio_misc_module_constructor(MOD_NAME, NULL, AUDIO_MIC_BLOCK_IOCTL_TYPE, &audio_mic_block_ops);
    if (ret) {
        hwlog_err("%s: constructor audio_mic_block fail \n", __func__);
        ret = -ENOMEM;
    }

    return ret;
}