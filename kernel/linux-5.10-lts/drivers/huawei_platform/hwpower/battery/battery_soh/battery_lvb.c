// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 */

#include <securec.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/sysfs.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <huawei_platform/hwpower/common_module/power_platform.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>
#include <chipset_common/hwpower/charger/charger_common_interface.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_dsm.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_supply.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <chipset_common/hwpower/common_module/power_time.h>
#include <chipset_common/hwpower/battery/battery_soh.h>
#include <huawei_platform/inputhub/default/contexthub_drop_event.h>

#define HWLOG_TAG battery_lvb
HWLOG_REGIST();

static struct lvb_coul_data {
	long time;
	int batt_index;
	int volt;
	int cur;
	int temp;
	int soc;
	int fcc;
} g_lvb_coul_data = { 0 };

static struct lvb_drop_data {
	long time;
	int type;
	int initial_speed;
	int height;
	int angle_pitch;
	int angle_roll;
	int impact;
	int posture;
} g_lvb_drop_data = { 0 };

struct notifier_block g_lvb_update_notify;

static int lvb_dump_coul_data_to_buf(struct lvb_coul_data *data, char *buf, int len)
{
	return snprintf_s(buf, len, len - 1,
		"BSOH_EVT=EVT_VBAT_DROP@batt_index:%d,volt:%d,cur:%d,temp:%d,soc:%d,fcc:%d,time:%ld",
		data->batt_index, data->volt, data->cur, data->temp, data->soc, data->fcc, data->time);
}

static int lvb_dump_drop_data_to_buf(struct lvb_drop_data *data, char *buf, int len)
{
	return snprintf_s(buf, len, len - 1,
		"BSOH_EVT=EVT_SENSOR_DROP@type:%d,initial_speed:%d,height:%d,angle_pitch:%d,angle_roll:%d,impact:%d,time:%ld,"
		"posture:%d",
		data->type, data->initial_speed, data->height, data->angle_pitch, data->angle_roll, data->impact, data->time,
		data->posture);
}

int lvb_update_notifier_call(struct notifier_block *nb, unsigned long event, void *data)
{
	int len;
	char buf[POWER_EVENT_NOTIFY_SIZE] = { 0 };
	struct power_event_notify_data notify_data;

	if (event != BATT_COUL_DROP && event != SENSOR_DROP)
		return NOTIFY_OK;
	if (event == BATT_COUL_DROP) {
		hwlog_info("LVB received BATT_COUL_DROP\n");
		const struct vbat_drop_data *drop_data = (const struct vbat_drop_data *)data;
		if (!drop_data)
			return NOTIFY_DONE;
		g_lvb_coul_data.time = (long)power_get_current_kernel_time().tv_sec;
		g_lvb_coul_data.batt_index = drop_data->batt_index;
		g_lvb_coul_data.volt = drop_data->volt;
		g_lvb_coul_data.cur = drop_data->cur;
		g_lvb_coul_data.temp = drop_data->temp;
		g_lvb_coul_data.soc = drop_data->soc;
		g_lvb_coul_data.fcc = drop_data->fcc;
		len = lvb_dump_coul_data_to_buf(&g_lvb_coul_data, buf, POWER_EVENT_NOTIFY_SIZE);
		if (len <= 0)
			return NOTIFY_DONE;
		notify_data.event_len = len;
		notify_data.event = buf;
		power_event_report_uevent(&notify_data);
		return NOTIFY_OK;
	} else if (event == SENSOR_DROP) {
		hwlog_info("LVB received SENSOR_DROP\n");
		const struct sensor_drop_data *phone_drop_data = (const struct sensor_drop_data *)data;
		if (!phone_drop_data)
			return NOTIFY_DONE;
		g_lvb_drop_data.time = (long)power_get_current_kernel_time().tv_sec;
		g_lvb_drop_data.type = phone_drop_data->type;
		g_lvb_drop_data.initial_speed = phone_drop_data->initial_speed;
		g_lvb_drop_data.height = phone_drop_data->height;
		g_lvb_drop_data.angle_pitch = phone_drop_data->angle_pitch;
		g_lvb_drop_data.angle_roll = phone_drop_data->angle_roll;
		g_lvb_drop_data.impact = phone_drop_data->impact;
		g_lvb_drop_data.posture = phone_drop_data->posture;
		len = lvb_dump_drop_data_to_buf(&g_lvb_drop_data, buf, POWER_EVENT_NOTIFY_SIZE);
		if (len <= 0)
			return NOTIFY_DONE;
		notify_data.event_len = len;
		notify_data.event = buf;
		power_event_report_uevent(&notify_data);
		return NOTIFY_OK;
	}
	return NOTIFY_OK;
}

#ifdef CONFIG_SYSFS
static ssize_t lvb_coul_data_show(struct device *dev, struct device_attribute *attr, char *buff)
{
	if (g_lvb_coul_data.time == 0)
		return 0;
	return lvb_dump_coul_data_to_buf(&g_lvb_coul_data, buff, PAGE_SIZE);
}

static ssize_t lvb_uevent_notify_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct power_event_notify_data data;
	data.event_len = count;
	data.event = buf;
	power_event_report_uevent(&data);
	return count;
}

static ssize_t lvb_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d", g_lvb_coul_data.time > 0);
}

static DEVICE_ATTR_RO(lvb_coul_data);
static DEVICE_ATTR_WO(lvb_uevent_notify);
static DEVICE_ATTR_RO(lvb_status);
static struct attribute *g_lvb_attrs[] = {
	&dev_attr_lvb_coul_data.attr,
	&dev_attr_lvb_uevent_notify.attr,
	&dev_attr_lvb_status.attr,
	NULL,
};

static struct attribute_group g_lvb_group = {
	.name = "lvb",
	.attrs = g_lvb_attrs
};

static int lvb_sysfs_create_group(struct bsoh_device *di)
{
	return sysfs_create_group(&di->dev->kobj, &g_lvb_group);
}

static void lvb_sysfs_remove_group(struct bsoh_device *di)
{
	sysfs_remove_group(&di->dev->kobj, &g_lvb_group);
}
#else
static inline int lvb_sysfs_create_group(struct bsoh_device *di)
{
	return 0;
}

static inline void lvb_sysfs_remove_group(struct bsoh_device *di) {}
#endif /* CONFIG_SYSFS */

static int lvb_sys_init(struct bsoh_device *di)
{
	int ret;
	hwlog_info("lvb init begin\n");
	if (!di || !di->dev)
		return -ENODEV;
	ret = lvb_sysfs_create_group(di);
	if (ret)
		return ret;
	/* register notifier for coul core */
	g_lvb_update_notify.notifier_call = lvb_update_notifier_call;
	ret = coul_register_blocking_notifier(&g_lvb_update_notify);
	if (ret < 0)
		return ret;
	ret = drop_register_blocking_notifier(&g_lvb_update_notify);
	if (ret < 0)
		return ret;
	hwlog_info("lvb init ok\n");
	return 0;
}

static void lvb_sys_exit(struct bsoh_device *di)
{
	lvb_sysfs_remove_group(di);
}

static const struct bsoh_sub_sys g_lvb_sys = {
	.sys_init = lvb_sys_init,
	.sys_exit = lvb_sys_exit,
	.event_notify = NULL,
	.dmd_prepare = NULL,
	.type_name = "lvb",
	.notify_node = NULL
};

static int __init lvb_init(void)
{
	hwlog_info("lvb init");
	bsoh_register_sub_sys(BSOH_SUB_SYS_LVB, &g_lvb_sys);
	return 0;
}

static void __exit lvb_exit(void) {}

subsys_initcall(lvb_init);
module_exit(lvb_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("battery lvb driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");