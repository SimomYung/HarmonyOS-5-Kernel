/*
 * audio_maintain.c
 *
 * audio maintain err upload driver
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
#include <linux/ioctl.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <huawei_platform/log/hw_log.h>
#include "audio_maintain.h"
#include "securec.h"
#ifndef CONFIG_HISYSEVENT
#include "huawei_platform/log/imonitor.h"
#include "huawei_platform/log/imonitor_keys.h"
#else
#include <dfx/hiview_hisysevent.h>
#endif /* CONFIG_HISYSEVENT */

#define HWLOG_TAG audio_maintain
#define MOD_NAME "audio_maintain"

HWLOG_REGIST();

#define AUDIO_MAINTAIN_DMD_ID         921002008
#define AUDIO_MAINTAIN_IOC_MAGIC      'Z'
#define AUDIO_CUST_MAINTAIN_UPLOAD    _IOW(AUDIO_MAINTAIN_IOC_MAGIC, 0x1, struct audio_maintain_info)

static struct audio_maintain_priv *g_priv;

#ifndef CONFIG_HISYSEVENT
static void audio_maintain_dmd_report(const char *value)
{
	struct imonitor_eventobj *obj = NULL;
	int ret;

	obj = imonitor_create_eventobj(AUDIO_MAINTAIN_DMD_ID);
	if (!obj) {
		hwlog_err("%s: create eventobj is NULL\n", __func__);
		return;
	}

    imonitor_set_param_string_v2(obj, "module_name", value);

	ret = imonitor_send_event(obj);
	if (ret <= 0)
		hwlog_err("%s: audio_maintain dmd sent fail\n", __func__);

	imonitor_destroy_eventobj(obj);
}
#else
static void audio_maintain_dmd_report(const char *value)
{
	struct hiview_hisysevent *event = NULL;
	int ret;

	event = hisysevent_create("AUDIO_DRV", "ANC_HS_UNHANDLED_IRQ", FAULT);
	if (!event) {
		hwlog_err("failed to create event\n");
		return;
	}

	ret = hisysevent_put_string(event, "MODULE_NAME", value);
	if (ret != 0) {
		hwlog_err("failed to put to event, ret=%d\n", ret);
		goto hisysevent_end;
	}

	ret = hisysevent_write(event);
	if (ret < 0)
		hwlog_err("%s: hisysevent_write fail\n", __func__);

hisysevent_end:
	hisysevent_destroy(&event);
}
#endif /* CONFIG_HISYSEVENT */

int audio_cust_maintain_upload(unsigned long arg)
{
	int ret = -1;
	struct audio_maintain_info info;
	void __user *argp = (void __user *)(uintptr_t)arg;

	ret = copy_from_user(&info, argp, sizeof(info));
	if (ret != 0) {
		hwlog_err("%s: copy_from_user error\n", __func__);
		return ret;
	}

	audio_maintain_dmd_report(info.name);

	return 0;
}

static long audio_maintain_ioctl(unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	if (arg == 0) {
		hwlog_err("%s: invalid arg\n", __func__);
		return -1;
	}

	mutex_lock(&g_priv->do_ioctl_lock);
	switch (cmd) {
		case AUDIO_CUST_MAINTAIN_UPLOAD:
			ret = audio_cust_maintain_upload(arg);
			break;
		default:
			hwlog_err("%s: not support cmd = 0x%x\n", __func__, cmd);
			ret = -EIO;
			break;
	}
	mutex_unlock(&g_priv->do_ioctl_lock);
	return ret;
}

static void audio_maintain_remove(void *priv)
{
	struct audio_maintain_priv *ca_priv = (struct audio_maintain_priv *)priv;
	kfree(ca_priv);
	ca_priv = NULL;
	g_priv = NULL;
}

static const struct audio_misc_module_ops audio_maintain_info_ops = {
	.remove = audio_maintain_remove,
	.ioctl = audio_maintain_ioctl,
};

int audio_maintain_probe(struct device *dev, struct device_node *np)
{
	int ret;
	hwlog_info("%s: audio_maintain_probe in\n", __func__);
	g_priv = kzalloc(sizeof(*g_priv), GFP_KERNEL);
	if (!g_priv) {
		return -ENOMEM;
	}

	mutex_init(&g_priv->do_ioctl_lock);

    ret = audio_misc_module_constructor(MOD_NAME, g_priv, AUDIO_MAINTAIN_IOC_MAGIC, &audio_maintain_info_ops);
    if (ret) {
        ret = -ENOMEM;
        kfree(g_priv);
        g_priv = NULL;
	}
	return ret;
}