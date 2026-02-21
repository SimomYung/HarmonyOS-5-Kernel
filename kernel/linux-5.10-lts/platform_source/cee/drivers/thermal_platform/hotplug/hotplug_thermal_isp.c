// SPDX-License-Identifier: GPL-2.0
/*
 * hotplug_thermal_isp.c
 *
 * isp thermal hotplug
 *
 * Copyright (C) 2017-2022 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <platform_include/basicplatform/linux/ipc_rproc.h>
#include <platform_include/basicplatform/linux/ipc_msg.h>
#include "hotplug_thermal.h"

#define ISP_HOTPLUG_NAME	"isp"
static struct hotplug_ops g_isphp_ops;
static struct notifier_block g_isp_nb;

struct isphp_data {
	struct delayed_work isphp_work;

#ifdef CONFIG_HOTPLUG_EMULATION
	int emul_temp;
#endif

	int level;
	int level_base;
	int level_internal;
	int level_max;

	u32 polling_delay;
};

static struct isphp_data g_isphp_data;

int (*g_isp_hotplug_handler)(unsigned int) = NULL;
int (*g_ddr_to_isp_handler)(unsigned int) = NULL;

#ifdef CONFIG_HOTPLUG_EMULATION

#define show_isphp_attr(elem)					\
static ssize_t show_isphp_##elem				\
(struct device *dev, struct device_attribute *attr, char *buf)	\
{								\
	if (dev == NULL || attr == NULL || buf == NULL)		\
		return 0;					\
	return snprintf_s(buf, MAX_LEN,  MAX_LEN - 1,		\
			  "%d\n", g_isphp_data.elem);		\
}

#define store_isphp_attr(elem)				\
static ssize_t store_isphp_##elem			\
(struct device *dev, struct device_attribute *attr,	\
	const char *buf, size_t count)			\
{							\
	int value = 0;					\
							\
	if (dev == NULL || attr == NULL || buf == NULL)	\
		return 0;				\
	if (kstrtoint(buf, DECIMAL, &value))		\
		return -EINVAL;				\
	g_isphp_data.elem = value;			\
	pr_err("isphp set %s %d\n", #elem, value);	\
	return (ssize_t)count;				\
}

#define isphp_attr_rw(elem)		\
static DEVICE_ATTR(isphp_data_##elem, 0644, show_isphp_##elem, store_isphp_##elem)

show_hotplug_attr(isp, down_threshold);
store_hotplug_attr(isp, down_threshold);
show_hotplug_attr(isp, up_threshold);
store_hotplug_attr(isp, up_threshold);
show_hotplug_attr(isp, emul_temp);
store_hotplug_attr(isp, emul_temp);
show_isphp_attr(level_base);
store_isphp_attr(level_base);
show_isphp_attr(level_internal);
store_isphp_attr(level_internal);
show_isphp_attr(emul_temp);
store_isphp_attr(emul_temp);

hotplug_attr_rw(isp_down_threshold, isp, down_threshold);
hotplug_attr_rw(isp_up_threshold, isp, up_threshold);
hotplug_attr_rw(isp_hotplug_emul_temp, isp, emul_temp);
isphp_attr_rw(level_base);
isphp_attr_rw(level_internal);
isphp_attr_rw(emul_temp);

static struct attribute *isphp_dev_attributes[] = {
	&dev_attr_isp_hotplug_emul_temp.attr,
	&dev_attr_isp_down_threshold.attr,
	&dev_attr_isp_up_threshold.attr,
	&dev_attr_isphp_data_level_base.attr,
	&dev_attr_isphp_data_level_internal.attr,
	&dev_attr_isphp_data_emul_temp.attr,
	NULL
};

static const struct attribute_group isphp_dev_attr_group = {
	.attrs = isphp_dev_attributes,
};

static int thermal_hotplug_create_isp_node(struct device *dev)
{
	return device_add_group(dev, &isphp_dev_attr_group);
}

static void thermal_hotplug_remove_isp_node(struct device *dev)
{
	device_remove_group(dev, &isphp_dev_attr_group);
}
#else
static int thermal_hotplug_create_isp_node(struct device *dev __maybe_unused) { return 0; }

static void thermal_hotplug_remove_isp_node(struct device *dev __maybe_unused) {}
#endif

void isp_hotplug_callback_register(int (*isp_hotplug_handler)(unsigned int))
{
	if (isp_hotplug_handler == NULL) {
		pr_err("isp hotplug callback register failed!\n");
		return;
	}
	g_isp_hotplug_handler = isp_hotplug_handler;
}
EXPORT_SYMBOL(isp_hotplug_callback_register);

void isp_hotplug_callback_unregister(void)
{
	g_isp_hotplug_handler = NULL;
}
EXPORT_SYMBOL(isp_hotplug_callback_unregister);

void ddr_to_isp_callback_register(int (*ddr_to_isp_handler)(unsigned int))
{
	if (ddr_to_isp_handler == NULL) {
		pr_err("ddr to isp callback register failed!\n");
		return;
	}
	g_ddr_to_isp_handler = ddr_to_isp_handler;
}
EXPORT_SYMBOL(ddr_to_isp_callback_register);

void ddr_to_isp_callback_unregister(void)
{
	g_ddr_to_isp_handler = NULL;
}
EXPORT_SYMBOL(ddr_to_isp_callback_unregister);

static int ipa_get_isp_temp(int *val)
{
	return ipa_get_dev_temp(ISP_HOTPLUG_NAME, val);
}

static void thermal_hotplug_handle_isp(unsigned int level)
{
	if (g_isp_hotplug_handler == NULL) {
		pr_err("%s: invalid callback\n", __func__);
		return;
	}
	g_isp_hotplug_handler(level);
}

static void notify_isp(int level, int isp_temp)
{
	if (level == g_isphp_data.level)
		return;

	g_isphp_data.level = level;
	pr_err("isp level=%d, current_temp:%d\n", level, isp_temp);
	thermal_hotplug_handle_isp((unsigned int)level);
}

static void isp_thermal_poll_work(struct work_struct *poll_work)
{
	int level;
	int isp_temp = 0;
	struct delayed_work *delayed_work = to_delayed_work(poll_work);

	if (ipa_get_isp_temp(&isp_temp) != 0) {
		pr_err("isp get_temp fail\n");
		goto poll_work_out;
	}
#ifdef CONFIG_HOTPLUG_EMULATION
	if (g_isphp_data.emul_temp != 0)
		isp_temp = g_isphp_data.emul_temp;
#endif

	if (isp_temp < g_isphp_data.level_base || g_isphp_data.level_internal <= 0) {
		level = 0;
	} else {
		level = (isp_temp - g_isphp_data.level_base) / g_isphp_data.level_internal;
		level = min(level, g_isphp_data.level_max);
	}
	notify_isp(level, isp_temp);

poll_work_out:
	mod_delayed_work(system_power_efficient_wq, delayed_work,
			 msecs_to_jiffies(g_isphp_data.polling_delay));
}

static void isp_fsm_callback(enum HOTPLUG_STATUS status)
{
	pr_err("isp status=%d, current_temp:%d\n", status, g_isphp_ops.current_temp);

	switch (status) {
	case HOTPLUG_NONE:
		notify_isp(0, g_isphp_ops.current_temp);
		cancel_delayed_work(&g_isphp_data.isphp_work);
		break;
	case HOTPLUG_NORMAL:
	case HOTPLUG_CRITICAL:
		mod_delayed_work(system_power_efficient_wq, &g_isphp_data.isphp_work,
				 msecs_to_jiffies(g_isphp_data.polling_delay));
		break;
	default:
		break;
	}
}

static int thermal_isp_hotplug_dts_parse(struct device_node *np, struct hotplug_ops *ops, struct isphp_data *data)
{
	int ret;

	ret = of_property_read_u32(np, "ithermal,isp_down_threshold", (u32 *)&ops->down_threshold);
	if (ret != 0) {
		pr_err("%s isp_down_threshold read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,isp_up_threshold", (u32 *)&ops->up_threshold);
	if (ret != 0) {
		pr_err("%s isp_up_threshold read err\n", __func__);
		return ret;
	}
	ops->critical_down_threshold = ops->down_threshold;
	ops->critical_up_threshold = ops->up_threshold;

	ret = of_property_read_u32(np, "ithermal,isp_level_base", (u32 *)&data->level_base);
	if (ret != 0) {
		pr_err("%s isp level_base read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,isp_level_internal", (u32 *)&data->level_internal);
	if (ret != 0) {
		pr_err("%s isp level_internal read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,isp_level_max", (u32 *)&data->level_max);
	if (ret != 0) {
		pr_err("%s isp level_max read err\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "ithermal,isp_polling_delay", (u32 *)&data->polling_delay);
	if (ret != 0) {
		pr_err("%s isp polling_delay read err\n", __func__);
		return ret;
	}

	return 0;
}

static int ddr_ipc_handler(struct notifier_block *nb, unsigned long len,
	void *mbox_msg)
{
	union ipc_data *msg = (union ipc_data *)mbox_msg;
	unsigned int level = msg->data[1];

	if (g_ddr_to_isp_handler == NULL) {
		pr_err("%s: invalid callback\n", __func__);
		return 0;
	}
	g_ddr_to_isp_handler(level);
	return 0;
}

static int thermal_isp_mbox_init(void)
{
	int ret;

	g_isp_nb.next = NULL;
	g_isp_nb.notifier_call = ddr_ipc_handler;

	/* register the rx notify callback */
	ret = RPROC_MONITOR_REGISTER(IPC_LPM3_ACPU_MBX_7, &g_isp_nb);
	if (ret)
		pr_err("rproc monitor register failed");

	return ret;
}

static int thermal_hotplug_isp_init(struct device_node *np, struct device *dev)
{
	int ret;

	if (np == NULL || dev == NULL) {
		pr_err("%s fail\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	ret = thermal_isp_hotplug_dts_parse(np, &g_isphp_ops, &g_isphp_data);
	if (ret != 0) {
		pr_err("%s parse dts fail\n", __func__);
		goto out;
	}

	ret = thermal_hotplug_create_isp_node(dev);
	if (ret != 0) {
		pr_err("%s create_node fail\n", __func__);
		goto out;
	}

	INIT_DELAYED_WORK(&g_isphp_data.isphp_work, isp_thermal_poll_work);

	ret = thermal_isp_mbox_init();
	if (ret != 0) {
		pr_err("%s thermal_isp_mbox_init failed\n", __func__);
		goto out;
	}
out:
	return ret;
}

static void thermal_hotplug_isp_exit(struct device *dev)
{
	thermal_hotplug_remove_isp_node(dev);
}

static struct hotplug_ops g_isphp_ops = {
	.name = ISP_HOTPLUG_NAME,
	.physical_status = HOTPLUG_NONE,
	.status = HOTPLUG_NONE,

	.get_temp = ipa_get_isp_temp,
	.current_temp = 0,

	.fsm_callback = isp_fsm_callback,

	.init = thermal_hotplug_isp_init,
	.exit = thermal_hotplug_isp_exit,
};

static int __init thermal_isphp_init(void)
{
	return hotplug_ops_register(&g_isphp_ops);
}

static void __exit thermal_isphp_exit(void)
{
	hotplug_ops_unregister(&g_isphp_ops);
}

module_init(thermal_isphp_init);
module_exit(thermal_isphp_exit);
