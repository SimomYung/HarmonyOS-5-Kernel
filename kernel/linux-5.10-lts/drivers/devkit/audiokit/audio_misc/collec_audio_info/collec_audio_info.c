/*
 * collec_audio_info.c
 *
 * collec_audio_info driver
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

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regulator/consumer.h>
#include <linux/pinctrl/consumer.h>
#include <linux/workqueue.h>
#include <linux/of_device.h>
#include <huawei_platform/log/hw_log.h>
#include "collec_audio_info.h"
#include "securec.h"

#define HWLOG_TAG collec_audio_info
#define MOD_NAME "collec_audio_info"

HWLOG_REGIST();

static struct collec_audio_info_priv *priv = NULL;

static long collec_audio_info_ioctl(unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	if (priv == NULL || arg == 0) {
		return -1;
	}
    void __user *argp = (void __user *)(uintptr_t)arg;
	mutex_lock(&priv->do_ioctl_lock);
	switch (cmd) {
		case SEND_UNDERPATH_W:
			ret = copy_from_user(&priv->all_received_info.from_platform_info.path, argp, sizeof(struct underpath_info));
			hwlog_info("%s: succeed recived cmd SEND_UNDERPATH_W. output_path: %s, input_path: %s\n", __func__,
				priv->all_received_info.from_platform_info.path.output_path, priv->all_received_info.from_platform_info.path.input_path);
			break;
		case SEND_PASTATE_W:
			ret = copy_from_user(&priv->all_received_info.from_platform_info.pastate, argp, sizeof(int));
			hwlog_info("%s: succeed recived cmd SEND_PASTATE_W, pa_state :%d\n", __func__,
				priv->all_received_info.from_platform_info.pastate);
			break;
		case SEND_MUTESTATE_W:
			ret = copy_from_user(&priv->all_received_info.from_platform_info.mutestate, argp, sizeof(int));
			hwlog_info("%s: succeed recived cmd SEND_MUTESTATE_W, mute_state : %d\n", __func__,
				priv->all_received_info.from_platform_info.mutestate);
			break;
		case SEND_TDDSTATE_W:
			ret = copy_from_user(&priv->all_received_info.from_platform_info.tddstate, argp, sizeof(int));
			hwlog_info("%s: succeed recived cmd SEND_TDDSTATE_W, tdd_state : %d\n", __func__,
				priv->all_received_info.from_platform_info.tddstate);
			break;
		default:
			hwlog_err("%s: not support cmd = 0x%u\n", __func__, cmd);
			ret = -EIO;
			break;
	}
	mutex_unlock(&priv->do_ioctl_lock);
	return ret;
}

static void collec_audio_info_remove(void *priv_to_remove)
{
	struct collec_audio_info_priv *ca_priv = (struct collec_audio_info_priv *)priv_to_remove;
	kfree(ca_priv);
	ca_priv = NULL;
	priv = NULL;
}


static const struct audio_misc_module_ops collec_audio_info_ops = {
	.remove = collec_audio_info_remove,
	.ioctl = collec_audio_info_ioctl,
};

int collec_audio_info_probe(struct device *dev, struct device_node *np)
{
	int ret;
	hwlog_info("%s: collec_audio_info_probe in\n", __func__);
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		return -ENOMEM;
	}
    priv->all_received_info.from_driver_info.pa_interrupt_times = 0;
    priv->all_received_info.from_driver_info.write_exception_times = 0;

	mutex_init(&priv->do_ioctl_lock);
	mutex_init(&priv->pa_interrupt_times_lock);
	priv->pa_interrupt_times_lock_state = true;
    mutex_init(&priv->write_exception_times_lock);
	priv->write_exception_times_lock_state = true;
    mutex_init(&priv->pa_manufacturer_lock);
	priv->pa_manufacturer_lock_state = true;

    ret = audio_misc_module_constructor(MOD_NAME, priv, GET_INFO_IOC_MAGIC, &collec_audio_info_ops);
    if (ret) {
        ret = -ENOMEM;
        kfree(priv);
        priv = NULL;
	}
	return ret;
}

void get_collected_audio_info(struct received_info *info)
{
	if (priv == NULL || info == NULL) {
		hwlog_info("%s: priv is null \n", __func__);
		return;
	}
	info->from_driver_info = priv->all_received_info.from_driver_info;
    info->from_platform_info = priv->all_received_info.from_platform_info;
	hwlog_info("%s:inpput_path:%s, output_path: %s, pastate:%d, mutestate:%d, tddstate:%d, pa_interrupt_times:%d,\
		write_exception_times:%d , pa_manufacturer:%s, analog_headphone_state:%hd.\n", __func__,
		info->from_platform_info.path.input_path, info->from_platform_info.path.output_path, info->from_platform_info.pastate,
		info->from_platform_info.mutestate, info->from_platform_info.tddstate, info->from_driver_info.pa_interrupt_times,
		info->from_driver_info.write_exception_times, info->from_driver_info.pa_manufacturer,
		info->from_driver_info.analog_headphone_state);
}
EXPORT_SYMBOL(get_collected_audio_info);

void send_painterrupt_to_collec_audio_info(void)
{
	if (priv == NULL || priv->pa_interrupt_times_lock_state == false) {
		hwlog_err("%s: priv is null \n", __func__);
		return;
	}
    mutex_lock(&priv->pa_interrupt_times_lock);
    priv->all_received_info.from_driver_info.pa_interrupt_times++;
	hwlog_info("%s: set pa_interrupt_times++ \n", __func__);
    mutex_unlock(&priv->pa_interrupt_times_lock);
}
EXPORT_SYMBOL(send_painterrupt_to_collec_audio_info);

void send_write_exception_to_collec_audio_info(void)
{
	if (priv == NULL || priv->write_exception_times_lock_state == false) {
		hwlog_err("%s: priv is null \n", __func__);
		return;
	}
    mutex_lock(&priv->write_exception_times_lock);
    priv->all_received_info.from_driver_info.write_exception_times++;
	hwlog_info("%s: set write_exception_times++ \n", __func__);
    mutex_unlock(&priv->write_exception_times_lock);
}
EXPORT_SYMBOL(send_write_exception_to_collec_audio_info);

void send_pa_manufacturer_to_collec_audio_info(const char *manufacturer)
{
	int ret;
	if (priv == NULL || manufacturer == NULL || priv->pa_manufacturer_lock_state == false) {
		hwlog_err("%s: priv is null \n", __func__);
		return;
	}
    mutex_lock(&priv->pa_manufacturer_lock);
	ret = strcpy_s(priv->all_received_info.from_driver_info.pa_manufacturer,
		sizeof(priv->all_received_info.from_driver_info.pa_manufacturer), manufacturer);
    if (ret != EOK) {
        hwlog_err("Failed to strcpy_s pa_manufacturer");
		mutex_unlock(&priv->pa_manufacturer_lock);
        return;
    }
	hwlog_info("%s: deviler pa_manufacturer is :%s\n", __func__, manufacturer);
    mutex_unlock(&priv->pa_manufacturer_lock);
}
EXPORT_SYMBOL(send_pa_manufacturer_to_collec_audio_info);

void send_analog_headphone_state_to_collection(short state)
{
	if (!priv) {
		hwlog_err("%s: priv is null\n", __func__);
		return;
	}
	priv->all_received_info.from_driver_info.analog_headphone_state = state;
}
EXPORT_SYMBOL(send_analog_headphone_state_to_collection);