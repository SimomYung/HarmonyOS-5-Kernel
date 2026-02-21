// SPDX-License-Identifier: GPL-2.0
/*
 * battery_vit.c
 *
 * driver adapter for vit.
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/limits.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_supply.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <huawei_platform/hwpower/common_module/power_platform.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>
#include "battery_vit.h"

#define HWLOG_TAG                               battery_vit
#define VIT_DEFAULT_ENABLE_MONITOR              0
#define VIT_DEFAULT_BATT_NUM                    1
#define VIT_MAX_BATT_NUM                        5
#define VIT_DEFAULT_MONITOR_INTERVAL            1000
#define VIT_DEFAULT_UPDATE_INTERVAL             60
#define VIT_MAX_QUEUE_SIZE                      80
#define MAX_INT                                 0x7ffffff
#define VIT_PARAM_NUM_CHARGE                    5
#define VIT_UPDATE_DELAY_TIME                   60
#define VIT_UEVENT_BUFF                         "BMS_EVT=EVT_VIT_UPDATE"
HWLOG_REGIST();

static struct vit_device_info *g_vit_di = NULL;
static char* g_vit_batt_name[] = {"battery_gauge", "battery_gauge_aux", "battery_gauge_third"};

enum vit_mask_bits {
	VIT_MASK_BEGIN = 0,
	VIT_MASK_PLUGGED = VIT_MASK_BEGIN,
	VIT_MASK_CHARGING,
	VIT_MASK_CHARGEDONE,
	VIT_MASK_OCVUPDATED,
	VIT_MASK_END
};

static void vit_update_uevent(void)
{
	struct power_event_notify_data notify_data = { 0 };

	notify_data.event = VIT_UEVENT_BUFF;
	notify_data.event_len = strlen(VIT_UEVENT_BUFF);
	power_event_report_uevent(&notify_data);
}

static int vit_queue_enqueue(struct vit_data_queue *q, struct vit_batt_data *data)
{
	struct vit_data_queue_node *node;
	struct vit_data_queue_node *old_node;

	node = kzalloc(sizeof(struct vit_data_queue_node), GFP_KERNEL);
	if (!node) {
		hwlog_err("kzalloc vit_data_queue_node fail\n");
		return -ENOMEM;
	}

	spin_lock(&q->lock);
	node->batt = data;
	// Check if the queue exceeds the threshold.
	while (q->count >= VIT_MAX_QUEUE_SIZE) {
		old_node = list_first_entry(&q->head, struct vit_data_queue_node, list);
		list_del(&old_node->list);
		kfree(old_node->batt);
		kfree(old_node);
		q->count--;
		hwlog_info("delete vit_data due to queue size over limit, cur_count:%d\n", q->count);
	}

	list_add_tail(&node->list, &q->head);
	q->count++;
	spin_unlock(&q->lock);
	hwlog_debug("enqueue vit_data, cur_count:%d\n", q->count);
	return 0;
}

static void vit_queue_destroy(struct vit_data_queue *q)
{
	struct list_head *pos, *next;
	struct vit_data_queue_node *node;

	spin_lock(&q->lock);
	list_for_each_safe(pos, next, &q->head) {
		node = list_entry(pos, struct vit_data_queue_node, list);
		list_del(pos);
		kfree(node->batt);
		kfree(node);
	}
	q->count = 0;
	spin_unlock(&q->lock);
}

struct vit_batt_data *vit_queue_dequeue(struct vit_data_queue *q)
{
	struct vit_data_queue_node *node = NULL;
	struct vit_batt_data *data = NULL;

	spin_lock(&q->lock);
	if (!list_empty(&q->head)) {
		node = list_first_entry(&q->head, struct vit_data_queue_node, list);
		list_del(&node->list);
		data = node->batt;
		kfree(node);
		q->count--;
		hwlog_info("dequeue vit_data, cur_count:%d\n", q->count);
	}

	spin_unlock(&q->lock);
	return data;
}

static void vit_clear_queue()
{
	int i;

	if (!g_vit_di) {
		hwlog_err("g_vit_di is null\n");
		return;
	}

	for (i = 0; i < g_vit_di->batt_num; i++)
		vit_queue_destroy(&g_vit_di->batt_queues[i]);
}

static void vit_print_data(struct vit_batt_data *data)
{
	hwlog_debug("batt_index: %u, time: %ld s, volt: %u mV, cur: %d mA "
		"bat_temp: %d, cycle: %u, cc: %d, ufsoc: %u, msoc: %u, uisoc: %u "
		"shell_temp: %u, event_mask: 0x%04X\n",
		data->batt_index, data->time, data->volt, data->cur, data->temp,
		data->cycle, data->cc, data->ufsoc, data->msoc, data->uisoc,
		data->shell_temp, data->event_mask);
}

static void vit_update_evt_mask(struct vit_batt_data *data, int index)
{
	if (!g_vit_di || index >= g_vit_di->batt_num) {
		hwlog_err("g_vit_di is null or index invalid\n");
		return;
	}

	data->event_mask |= (!!g_vit_di->is_plugged << VIT_MASK_PLUGGED);
	data->event_mask |= (!!g_vit_di->is_charging << VIT_MASK_CHARGING);
	data->event_mask |= (!!g_vit_di->is_charge_done << VIT_MASK_CHARGEDONE);
	data->event_mask |= (!!g_vit_di->ocv_updated[index] << VIT_MASK_OCVUPDATED);
	g_vit_di->is_charge_done = 0;
	g_vit_di->ocv_updated[index] = 0;
}

static u8 vit_check_soc_value(int soc)
{
	if ((soc < 0) || (soc > U8_MAX))
		return U8_MAX;
	return (u8)soc;
}

static void vit_fill_batt_data(struct vit_batt_data *data, int index)
{
	char *name = NULL;
	int cc;
	int unfilter_soc;
	int temp_msoc = 0;
	int temp_uisoc = 0;

	if (index >= ARRAY_SIZE(g_vit_batt_name)) {
		hwlog_err("index is invalid\n");
		return;
	}

	name = g_vit_batt_name[index];
	if (!name) {
		hwlog_err("name is null\n");
		return;
	}

	data->batt_index = index;
	data->time = (long)power_get_current_kernel_time().tv_sec;
	power_supply_get_int_prop(name,
		POWER_SUPPLY_PROP_CURRENT_NOW, &data->cur, 0, 1);
	power_supply_get_int_prop(name,
		POWER_SUPPLY_PROP_VOLTAGE_NOW, &data->volt, 0, POWER_UV_PER_MV);
	power_supply_get_int_property_value(name,
		POWER_SUPPLY_PROP_CYCLE_COUNT, (int *)&data->cycle);
	power_supply_get_int_property_value(name,
		POWER_SUPPLY_PROP_TEMP, &data->temp);
	power_supply_get_int_property_value(name,
		POWER_SUPPLY_PROP_CAPACITY, &temp_msoc);
	power_supply_get_int_prop(POWER_PLATFORM_BAT_PSY_NAME,
		POWER_SUPPLY_PROP_CAPACITY, &temp_uisoc, POWER_SUPPLY_DEFAULT_CAPACITY, 1);
	data->msoc = (u8)vit_check_soc_value(temp_msoc);
	data->uisoc = (u8)vit_check_soc_value(temp_uisoc);
	data->ufsoc = (u8)vit_check_soc_value(coul_drv_battery_unfiltered_capacity());
	cc = coul_drv_battery_cc();
	if (cc == -EPERM)
		cc = INT_MAX;
	data->cc = cc;
	vit_update_evt_mask(data, index);
	vit_print_data(data);
}

static void vit_handle_uevent_update()
{
	int i;
	u32 now_time;
	bool is_updated = true;

	if (!g_vit_di) {
		hwlog_err("g_vit_di is null\n");
		return;
	}

	for (i = 0; i < g_vit_di->batt_num; i++)
		is_updated &= (g_vit_di->batt_queues[i].count >= g_vit_di->update_interval);

	if (is_updated) {
		now_time = (u32)power_get_current_kernel_time().tv_sec;
		if ((g_vit_di->last_update_time == 0) ||
			(now_time - g_vit_di->last_update_time >= VIT_UPDATE_DELAY_TIME)) {
			g_vit_di->last_update_time = now_time;
			vit_update_uevent();
		}
	}
}

static void vit_monitor_work(struct work_struct *work)
{
	int ret;
	int i;
	struct vit_batt_data *data = NULL;

	if (!g_vit_di) {
		hwlog_err("g_vit_di is null\n");
		return;
	}

	if (!g_vit_di->is_charging && !g_vit_di->enable_discharge_monitor) {
		hwlog_info("not charging and not enable discharge monitor, return\n");
		return;
	}

	if (g_vit_di->is_charging && !g_vit_di->enable_charge_monitor) {
		hwlog_info("charging and not enable charge monitor, return\n");
		return;
	}

	for (i = 0; i < g_vit_di->batt_num; i++) {
		data = kzalloc(sizeof(struct vit_batt_data), GFP_KERNEL);
		if (!data) {
			hwlog_err("malloc vit_data is null\n");
			continue;
		}
		vit_fill_batt_data(data, i);
		ret = vit_queue_enqueue(&g_vit_di->batt_queues[i], data);
		if (ret) {
			hwlog_err("vit_data enqueue fail\n");
			kfree(data);
		}
	}

	vit_handle_uevent_update();
	if (g_vit_di->is_charging)
		schedule_delayed_work(&g_vit_di->vit_monitor_work,  msecs_to_jiffies(g_vit_di->charge_monitor_interval));
	else
		schedule_delayed_work(&g_vit_di->vit_monitor_work,  msecs_to_jiffies(g_vit_di->discharge_monitor_interval));
}

static int vit_update_notifier_call(struct notifier_block *ocv_update_nb,
	unsigned long event, void *data)
{
	struct ocv_update_data *ocv_data = NULL;

	if (event != BATT_OCV_UPDATE)
		return NOTIFY_OK;

	if (!data || !g_vit_di)
		return NOTIFY_BAD;

	ocv_data = (struct ocv_update_data *)data;
	if (!ocv_data)
		return NOTIFY_BAD;

	if (ocv_data->batt_index >= g_vit_di->batt_num)
		return NOTIFY_BAD;

	g_vit_di->ocv_updated[ocv_data->batt_index] = 1;
	return NOTIFY_OK;
}

static void vit_init_dts_data(struct bsoh_device *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG),
		di->dev->of_node, "enable_discharge_monitor",
		&g_vit_di->enable_discharge_monitor, VIT_DEFAULT_ENABLE_MONITOR);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG),
		di->dev->of_node, "enable_charge_monitor",
		&g_vit_di->enable_charge_monitor, VIT_DEFAULT_ENABLE_MONITOR);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG),
		di->dev->of_node, "batt_num",
		&g_vit_di->batt_num, VIT_DEFAULT_BATT_NUM);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG),
		di->dev->of_node, "charge_monitor_interval",
		&g_vit_di->charge_monitor_interval, VIT_DEFAULT_MONITOR_INTERVAL);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG),
		di->dev->of_node, "discharge_monitor_interval",
		&g_vit_di->discharge_monitor_interval, VIT_DEFAULT_MONITOR_INTERVAL);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG),
		di->dev->of_node, "update_interval",
		&g_vit_di->update_interval, VIT_DEFAULT_UPDATE_INTERVAL);
}

static void vit_queue_init(struct vit_data_queue *q)
{
	INIT_LIST_HEAD(&q->head);
	spin_lock_init(&q->lock);
	q->count = 0;
}

static void vit_init_data_queue(void)
{
	int i;

	if (!g_vit_di) {
		hwlog_err("g_vit_di is null\n");
		return;
	}

	if ((g_vit_di->batt_num < 0) || (g_vit_di->batt_num > VIT_MAX_BATT_NUM)) {
		hwlog_err("batt_num error\n");
		return;
	}

	g_vit_di->batt_queues =
		kzalloc(sizeof(struct vit_data_queue) * g_vit_di->batt_num, GFP_KERNEL);
	if (!g_vit_di->batt_queues) {
		hwlog_err("batt_queues is null\n");
		return;
	}

	for (i = 0; i < g_vit_di->batt_num; i++)
		vit_queue_init(&g_vit_di->batt_queues[i]);
}

#ifdef CONFIG_SYSFS
static ssize_t vit_data_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int i;
	int ret = 0;
	char buftmp[PAGE_SIZE] = { 0 };
	struct vit_batt_data *data = NULL;

	if (!g_vit_di) {
		hwlog_err("g_vit_di is null\n");
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "data_error\n");
	}

	for (i = 0; i < g_vit_di->batt_num; i++) {
		data = vit_queue_dequeue(&g_vit_di->batt_queues[i]);
		if (data != NULL) {
			ret = snprintf_s(buftmp, PAGE_SIZE, PAGE_SIZE - 1, "batt_index:%u time:%ld volt:%u cur:%d "
				"bat_temp:%d cycle:%u cc:%d ufsoc:%u msoc:%u uisoc:%u "
				"shell_temp:%u event_mask:0x%04X\n",
				data->batt_index, data->time, data->volt, data->cur, data->temp,
				data->cycle, data->cc, data->ufsoc, data->msoc, data->uisoc,
				data->shell_temp, data->event_mask);
			kfree(data);
			if (strncat_s(buf, PAGE_SIZE - 1, buftmp, strlen(buftmp)) || (ret < 0)) {
				hwlog_err("strncat_s vit data fail\n");
				break;
			}
		}
		memset_s(buftmp, PAGE_SIZE, 0, sizeof(buftmp));
	}
	if (ret < 0)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "data_error\n");
	return strlen(buf);
}

static ssize_t vit_data_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	int ret;

	if (!g_vit_di) {
		hwlog_err("g_vit_di is null\n");
		return count;
	}

	ret = sscanf_s(buf, "chg_monitor:%d,dischg_monitor:%d,charge_i:%d,discharge_i:%d,update_i:%d",
		&g_vit_di->enable_charge_monitor, &g_vit_di->enable_discharge_monitor,
		&g_vit_di->charge_monitor_interval, &g_vit_di->discharge_monitor_interval,
		&g_vit_di->update_interval);
	if (ret != VIT_PARAM_NUM_CHARGE) {
		hwlog_err("failed to sscanf vit_data\n");
		return count;
	}

	cancel_delayed_work_sync(&g_vit_di->vit_monitor_work);
	schedule_delayed_work(&g_vit_di->vit_monitor_work, 0);
	return count;
}

static DEVICE_ATTR_RW(vit_data);
static struct attribute *g_vit_attrs[] = {
	&dev_attr_vit_data.attr,
	NULL,
};

static struct attribute_group g_vit_group = {
	.name = "vit",
	.attrs = g_vit_attrs
};

static int vit_sysfs_create_group(struct bsoh_device *di)
{
	return sysfs_create_group(&di->dev->kobj, &g_vit_group);
}

static void vit_sysfs_remove_group(struct bsoh_device *di)
{
	sysfs_remove_group(&di->dev->kobj, &g_vit_group);
}
#else
static inline int vit_sysfs_create_group(struct bsoh_device *di)
{
	return 0;
}

static inline void vit_sysfs_remove_group(struct bsoh_device *di) {}
#endif /* CONFIG_SYSFS */

static int vit_sys_init(struct bsoh_device *di)
{
	int ret;
	int i;
	struct vit_device_info *info = NULL;

	if (!di || !di->dev)
		return -ENODEV;

	info = devm_kzalloc(di->dev, sizeof(struct vit_device_info), GFP_KERNEL);
	if (!info) {
		hwlog_err("info devm_kzalloc fail\n");
		return -ENOMEM;
	}

	g_vit_di = info;
	vit_init_dts_data(di);

	/* register notifier for coul core */
	g_vit_di->vit_update_notify.notifier_call = vit_update_notifier_call;
	ret = coul_register_blocking_notifier(&g_vit_di->vit_update_notify);
	if (ret < 0) {
		hwlog_err("failed to register vit update notifier\n");
		return ret;
	}

	vit_init_data_queue();
	ret = vit_sysfs_create_group(di);
	if (ret) {
		hwlog_err("failed to create sysfs group\n");
		if (g_vit_di) {
			vit_clear_queue();
			kfree(g_vit_di->batt_queues);
			g_vit_di->batt_queues = NULL;
			coul_unregister_blocking_notifier(&g_vit_di->vit_update_notify);
		}
		return ret;
	}

	INIT_DELAYED_WORK(&g_vit_di->vit_monitor_work, vit_monitor_work);
	return 0;
}

static void vit_sys_exit(struct bsoh_device *di)
{
	if (g_vit_di) {
		vit_clear_queue();
		kfree(g_vit_di->batt_queues);
		g_vit_di->batt_queues = NULL;
		coul_unregister_blocking_notifier(&g_vit_di->vit_update_notify);
	}
	vit_sysfs_remove_group(di);
}

static void vit_event_notify(struct bsoh_device *di, unsigned int event)
{
	if (!g_vit_di)
		return;

	hwlog_debug("vit_event_notify event:%d\n", event);
	switch (event) {
	case VCHRG_START_AC_CHARGING_EVENT:
	case VCHRG_START_USB_CHARGING_EVENT:
		g_vit_di->is_charging = 1;
		g_vit_di->is_plugged = 1;
		cancel_delayed_work(&g_vit_di->vit_monitor_work);
		schedule_delayed_work(&g_vit_di->vit_monitor_work, 0);
		break;
	case VCHRG_STOP_CHARGING_EVENT:
		g_vit_di->is_plugged = 0;
	case VCHRG_NOT_CHARGING_EVENT:
		g_vit_di->is_charging = 0;
		break;
	case VCHRG_CHARGE_DONE_EVENT:
		g_vit_di->is_charging = 0;
		g_vit_di->is_charge_done = 1;
		break;
	default:
		break;
	};
}

static const struct bsoh_sub_sys g_vit_sys = {
	.sys_init = vit_sys_init,
	.sys_exit = vit_sys_exit,
	.event_notify = vit_event_notify,
	.dmd_prepare = NULL,
	.type_name = "vit",
	.notify_node = NULL
};

static int __init vit_init(void)
{
	hwlog_info("vit init");
	bsoh_register_sub_sys(BSOH_SUB_SYS_VIT, &g_vit_sys);
	return 0;
}

static void __exit vit_exit(void) {}

subsys_initcall(vit_init);
module_exit(vit_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("battery vit driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
