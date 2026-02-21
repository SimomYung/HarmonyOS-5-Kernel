// SPDX-License-Identifier: GPL-2.0
/*
 * wireless_protocol_sysfs.c
 *
 * sysfs of wireless protocol driver
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
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

#include "wireless_protocol_sysfs.h"
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_sysfs.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_acc.h>
#include <chipset_common/hwpower/wireless_charge/wireless_trx_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_common.h>

#define HWLOG_TAG wireless
HWLOG_REGIST();

static struct wireless_dev *g_wireless_dev;

static void wireless_sysfs_uevent_report_work(struct work_struct *work)
{
	int i;

	for (i = 0; i < WLRX_ACC_REPORT_UPGRADE_TIMES; i++) {
		power_event_report_str_uevent("BMS_EVT=TX_UPGRADE_DETECTED");
		(void)power_msleep(DT_MSLEEP_5S, 0, wlrx_msleep_exit);
	}
}

static int wireless_sysfs_event_notifier_call(struct notifier_block *rx_event_nb,
	unsigned long event, void *data)
{

	if (!g_wireless_dev)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_WLRX_TX_VERSION:
		(void)memcpy_s(&g_wireless_dev->tx_fwver, WIRELESS_TX_FWVER_LEN, data, WIRELESS_TX_FWVER_LEN);
		schedule_delayed_work(&g_wireless_dev->uevent_report_work, msecs_to_jiffies(0));
		break;
	default:
		return NOTIFY_OK;
	}

	hwlog_info("[sysfs_tx_fwver] tx_fwver:%s\n", g_wireless_dev->tx_fwver);
	return NOTIFY_OK;
}

static void wireless_sysfs_reset_tx_fwver(struct work_struct *work)
{
	if (!g_wireless_dev)
		return;

	(void)memset_s(g_wireless_dev->tx_fwver, WIRELESS_TX_FWVER_LEN, 0, WIRELESS_TX_FWVER_LEN);
}

#ifdef CONFIG_SYSFS
static ssize_t wireless_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);

static struct power_sysfs_attr_info wireless_sysfs_field_tbl[] = {
	power_sysfs_attr_ro(wireless, 0440, WIRELESS_SYSFS_TX_FWVER, tx_fwver),
	power_sysfs_attr_ro(wireless, 0440, WIRELESS_SYSFS_TX_ID, tx_id),
	power_sysfs_attr_ro(wireless, 0440, WIRELESS_SYSFS_TX_TYPE, tx_type),
	power_sysfs_attr_ro(wireless, 0440, WIRELESS_SYSFS_TX_SN, tx_sn),
	power_sysfs_attr_ro(wireless, 0440, WIRELESS_SYSFS_TX_AUTH, tx_auth),
	power_sysfs_attr_ro(wireless, 0440, WIRELESS_SYSFS_TX_ADP_TYPE, tx_adp_type),
	power_sysfs_attr_ro(wireless, 0440, WIRELESS_SYSFS_TX_BD_INFO, tx_bd_info),
};

#define WIRELESS_SYSFS_ATTRS_SIZE  ARRAY_SIZE(wireless_sysfs_field_tbl)

static struct attribute *wireless_sysfs_attrs[WIRELESS_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group wireless_sysfs_attr_group = {
	.attrs = wireless_sysfs_attrs,
};

static ssize_t wireless_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct wireless_protocol_dev *prot_dev = wireless_get_protocol_dev();

	if (!g_wireless_dev || !prot_dev)
		return -EINVAL;

	info = power_sysfs_lookup_attr(attr->attr.name,
		wireless_sysfs_field_tbl, WIRELESS_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case WIRELESS_SYSFS_TX_FWVER:
		schedule_delayed_work(&g_wireless_dev->reset_tx_fwver_work,
			msecs_to_jiffies(WIRELESS_RESET_FWVER_DELAY_TIME));
		return scnprintf(buf, WIRELESS_RD_BUF_SIZE, "%s\n",
			g_wireless_dev->tx_fwver);
	case WIRELESS_SYSFS_TX_ID:
		return scnprintf(buf, WIRELESS_RD_BUF_SIZE, "%x\n",
			prot_dev->info.tx_id);
	case WIRELESS_SYSFS_TX_TYPE:
		return scnprintf(buf, WIRELESS_RD_BUF_SIZE, "%d\n",
			prot_dev->info.tx_type);
	case WIRELESS_SYSFS_TX_SN:
		return scnprintf(buf, WIRELESS_RD_BUF_SIZE, "%s\n",
			prot_dev->info.tx_serial_num);
	case WIRELESS_SYSFS_TX_AUTH:
		return scnprintf(buf, WIRELESS_RD_BUF_SIZE, "%d\n",
			wlrx_acc_auth_succ(WLTRX_DRV_MAIN) ? 1 : 0);
	case WIRELESS_SYSFS_TX_ADP_TYPE:
		return scnprintf(buf, WIRELESS_RD_BUF_SIZE, "%d\n",
			prot_dev->info.tx_adp_type);
	case WIRELESS_SYSFS_TX_BD_INFO:
		return scnprintf(buf, WIRELESS_RD_BUF_SIZE, "%s",
			prot_dev->info.tx_bd_info);
	default:
		return 0;
	}
}

static struct device *wireless_sysfs_create_group(void)
{
	power_sysfs_init_attrs(wireless_sysfs_attrs,
		wireless_sysfs_field_tbl, WIRELESS_SYSFS_ATTRS_SIZE);
	return power_sysfs_create_group("hw_power", "wireless",
		&wireless_sysfs_attr_group);
}

static void wireless_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_group(dev, &wireless_sysfs_attr_group);
}
#else
static inline struct device *wireless_sysfs_create_group(void)
{
	return NULL;
}

static inline void wireless_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int __init wireless_init(void)
{
	int ret;
	struct wireless_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	l_dev->rx_event_nb.notifier_call = wireless_sysfs_event_notifier_call;
	ret = power_event_bnc_register(POWER_BNT_WLRX, &l_dev->rx_event_nb);
	if (ret < 0) {
		hwlog_err("register rx_event notifier failed\n");
		return -EPERM;
	}

	INIT_DELAYED_WORK(&l_dev->reset_tx_fwver_work, wireless_sysfs_reset_tx_fwver);
	INIT_DELAYED_WORK(&l_dev->uevent_report_work, wireless_sysfs_uevent_report_work);
	l_dev->dev = wireless_sysfs_create_group();
	g_wireless_dev = l_dev;
	return 0;
}

static void __exit wireless_exit(void)
{
	if (!g_wireless_dev)
		return;

	wireless_sysfs_remove_group(g_wireless_dev->dev);
	power_event_bnc_unregister(POWER_BNT_WLRX, &g_wireless_dev->rx_event_nb);
	kfree(g_wireless_dev);
	g_wireless_dev = NULL;
}

subsys_initcall_sync(wireless_init);
module_exit(wireless_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wireless driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
