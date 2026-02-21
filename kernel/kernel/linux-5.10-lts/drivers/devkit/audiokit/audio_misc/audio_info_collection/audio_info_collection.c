/*
 * audio_info_collection.c
 *
 * audio_info_collection driver
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
#include "audio_info_collection.h"
#include "securec.h"

#define HWLOG_TAG audio_info_collection
#define MOD_NAME "audio_info_collection"

HWLOG_REGIST();

static struct audio_info_collection_priv *priv = NULL;

static long audio_info_collection_ioctl(unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	if (!priv || arg == 0) {
		return -1;
	}
    void __user *argp = (void __user *)(uintptr_t)arg;
	mutex_lock(&priv->do_ioctl_lock);
	switch (cmd) {
		case SEND_UNDERPATH_W:
			ret = copy_from_user(&priv->all_received_info.states_from_hal.path, argp, sizeof(struct underpath_info));
			hwlog_info("%s: succeed recived cmd SEND_UNDERPATH_W. output_path: %s, input_path: %s\n", __func__,
				priv->all_received_info.states_from_hal.path.output_path, priv->all_received_info.states_from_hal.path.input_path);
			break;
		case SEND_PASTATE_W:
			ret = copy_from_user(&priv->all_received_info.states_from_hal.pa_state, argp, sizeof(int));
			hwlog_info("%s: succeed recived cmd SEND_PASTATE_W, pa_state :%d\n", __func__,
				priv->all_received_info.states_from_hal.pa_state);
			break;
		case SEND_MUTESTATE_W:
			ret = copy_from_user(&priv->all_received_info.states_from_hal.mute_state, argp, sizeof(int));
			hwlog_info("%s: succeed recived cmd SEND_MUTESTATE_W, mute_state : %d\n", __func__,
				priv->all_received_info.states_from_hal.mute_state);
			break;
		case SEND_TDDSTATE_W:
			ret = copy_from_user(&priv->all_received_info.states_from_hal.tdd_state, argp, sizeof(int));
			hwlog_info("%s: succeed recived cmd SEND_TDDSTATE_W, tdd_state : %d\n", __func__,
				priv->all_received_info.states_from_hal.tdd_state);
			break;
		default:
			hwlog_err("%s: not support cmd = 0x%x\n", __func__, cmd);
			ret = -EIO;
			break;
	}
	mutex_unlock(&priv->do_ioctl_lock);
	return ret;
}

static void audio_info_collection_remove(void *priv_to_remove)
{
	struct audio_info_collection_priv *ca_priv = (struct audio_info_collection_priv *)priv_to_remove;
	kfree(ca_priv);
	ca_priv = NULL;
	priv = NULL;
}


static const struct audio_misc_module_ops audio_info_collection_ops = {
	.remove = audio_info_collection_remove,
	.ioctl = audio_info_collection_ioctl,
};

int audio_info_collection_probe(struct device *dev, struct device_node *np)
{
	int ret;
	hwlog_info("audio_info_collection_probe in\n");
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		return -ENOMEM;
	}
    priv->all_received_info.states_from_kernel.pa_interrupt_times = 0;
    priv->all_received_info.states_from_kernel.write_exception_times = 0;
	
	mutex_init(&priv->do_ioctl_lock);
	mutex_init(&priv->pa_interrupt_times_lock);
	priv->pa_interrupt_times_lock_state = true;
    mutex_init(&priv->write_exception_times_lock);
	priv->write_exception_times_lock_state = true;
    mutex_init(&priv->pa_manufacturer_lock);
	priv->pa_manufacturer_lock_state = true;

    ret = audio_misc_module_constructor(MOD_NAME, priv, GET_INFO_IOC_MAGIC, &audio_info_collection_ops);
    if (ret) {
        ret = -ENOMEM;
        kfree(priv);
        priv = NULL;
	}
	return ret;
}

void get_collected_audio_info(struct received_info *info)
{
	if (!priv || !info) {
		hwlog_err("%s: priv is null\n", __func__);
		return;
	}
	info->states_from_hal = priv->all_received_info.states_from_hal;
    info->states_from_kernel = priv->all_received_info.states_from_kernel;
	hwlog_info("%s: start, inpput_path:%s, output_path: %s, pastate:%d, mutestate:%d, tddstate:%d, pa_interrupt_times:%d,\
		write_exception_times:%d , pa_manufacturer:%s, ana_hs:%hd.\n", __func__,
		info->states_from_hal.path.input_path, info->states_from_hal.path.output_path, info->states_from_hal.pa_state,  
		info->states_from_hal.mute_state, info->states_from_hal.tdd_state, info->states_from_kernel.pa_interrupt_times, 
		info->states_from_kernel.write_exception_times, info->states_from_kernel.pa_manufacturer,
		info->states_from_kernel.ana_hs_state);
}
EXPORT_SYMBOL(get_collected_audio_info);

void send_painterrupt_to_collection(void)
{
	if (!priv || priv->pa_interrupt_times_lock_state == false) {
		hwlog_err("%s: priv is null\n", __func__);
		return;
	}
    mutex_lock(&priv->pa_interrupt_times_lock);
    priv->all_received_info.states_from_kernel.pa_interrupt_times++;
    mutex_unlock(&priv->pa_interrupt_times_lock);
}
EXPORT_SYMBOL(send_painterrupt_to_collection);

void send_write_exception_to_collection(void)
{
	if (!priv || priv->write_exception_times_lock_state == false) {
		hwlog_err("%s: priv is null\n", __func__);
		return;
	}
    mutex_lock(&priv->write_exception_times_lock);
    priv->all_received_info.states_from_kernel.write_exception_times++;
    mutex_unlock(&priv->write_exception_times_lock);
}
EXPORT_SYMBOL(send_write_exception_to_collection);

void send_pa_manufacturer_to_collection(const char *manufacturer)
{
	int ret;
	if (!priv || priv->pa_manufacturer_lock_state == false) {
		hwlog_err("%s: priv is null\n", __func__);
		return;
	}
    mutex_lock(&priv->pa_manufacturer_lock);
	ret = strcpy_s(priv->all_received_info.states_from_kernel.pa_manufacturer,
		sizeof(priv->all_received_info.states_from_kernel.pa_manufacturer), manufacturer);
    if (ret != EOK) {
        hwlog_err("Failed to strcpy_s pa_manufacturer");
		mutex_unlock(&priv->pa_manufacturer_lock);
        return;
    }
    mutex_unlock(&priv->pa_manufacturer_lock);
}
EXPORT_SYMBOL(send_pa_manufacturer_to_collection);

void send_ana_hs_state_to_collection(short state)
{
	if (!priv) {
		hwlog_err("%s: priv is null\n", __func__);
		return;
	}
	priv->all_received_info.states_from_kernel.ana_hs_state = state;
}
EXPORT_SYMBOL(send_ana_hs_state_to_collection);