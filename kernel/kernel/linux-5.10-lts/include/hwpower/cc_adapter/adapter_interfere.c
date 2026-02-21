// SPDX-License-Identifier: GPL-2.0
/*
 * adapter_interfere.c
 *
 * adapter interfere driver
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

#include <chipset_common/hwpower/adapter/adapter_interfere.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_dts.h>

#define HWLOG_TAG adpt_intfr
HWLOG_REGIST();

#define ADPT_INTRF_RETRY_TIMES     3
#define ADPT_INTRF_DELAY           10000
#define ADPT_INTRF_MAX_PARA        32

enum adpt_intfr_type {
	ADPT_INTRF_TYPE_CCH_COMP_MAYBE_LOW = 0,
	ADPT_INTRF_TYPE_CABLE,
	ADPT_INTRF_TYPE_END,
};

struct adpt_intfr_dev {
	struct notifier_block plugged_nb;
	u32 intfr_type;
	enum adpt_intfr_cable_type cable_type;
	int cch_comp_low_adap_count;
	u8 *cch_comp_low_adap;
	struct delayed_work usb_disconnect_work;
	struct delayed_work resume_work;
};

static struct adpt_intfr_dev *g_adpt_intfr_dev;

static struct adpt_intfr_dev *adpt_intfr_get_dev(void)
{
	if (!g_adpt_intfr_dev) {
		hwlog_err("g_adpt_intfr_dev is null\n");
		return NULL;
	}

	return g_adpt_intfr_dev;
}

static void adpt_intfr_set_type(enum adpt_intfr_type type, bool enable)
{
	u32 intfr_type = 0;
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev)
		return;

	intfr_type = l_dev->intfr_type;
	if (enable) {
		l_dev->intfr_type |= (u32)(1 << type);
		if (!intfr_type)
			power_event_report_str_uevent("BMS_EVT=ADAP_CCH_COMP_MAYBE_LOW");
		cancel_delayed_work(&l_dev->resume_work);
	} else {
		l_dev->intfr_type &= (u32)(~(1 << type));
		if (intfr_type && !l_dev->intfr_type) {
			power_event_report_str_uevent("BMS_EVT=ADAP_CCH_COMP_NORMAL");
			cancel_delayed_work(&l_dev->resume_work);
		}
	}

	hwlog_info("adpt_intfr_set_type %u\n", l_dev->intfr_type);
}

static bool adpt_intfr_is_enable_type(enum adpt_intfr_type type)
{
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev)
		return false;

	return (l_dev->intfr_type & (u32)(1 << type)) > 0;
}

void adpt_intfr_recv_cch_comp_maybe_low(void)
{
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev || (l_dev->cch_comp_low_adap_count <= 0))
		return;

	adpt_intfr_set_type(ADPT_INTRF_TYPE_CCH_COMP_MAYBE_LOW, true);
	schedule_delayed_work(&l_dev->resume_work, msecs_to_jiffies(ADPT_INTRF_DELAY));
}

void adpt_intfr_check_adpt_code(int adpt_code)
{
	int i;
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev || !adpt_intfr_is_enable_type(ADPT_INTRF_TYPE_CCH_COMP_MAYBE_LOW))
		return;

	for (i = 0; i < l_dev->cch_comp_low_adap_count; i++) {
		if (l_dev->cch_comp_low_adap[i] == adpt_code) {
			adpt_intfr_set_type(ADPT_INTRF_TYPE_CCH_COMP_MAYBE_LOW, true);
			return;
		}
	}

	adpt_intfr_set_type(ADPT_INTRF_TYPE_CCH_COMP_MAYBE_LOW, false);
}

void adpt_intfr_set_cable_type(enum adpt_intfr_cable_type cable_type)
{
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev)
		return;

	l_dev->cable_type = cable_type;
}

void adpt_intfr_check_cable(int rpath, int intfr_resist)
{
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev || (intfr_resist <= 0))
		return;

	if ((rpath > intfr_resist) || (l_dev->cable_type == ADPT_INTRF_CABLE_TYPE_1M8))
		adpt_intfr_set_type(ADPT_INTRF_TYPE_CABLE, true);
}

static void adpt_intfr_resume_work(struct work_struct *work)
{
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev)
		return;

	adpt_intfr_set_type(ADPT_INTRF_TYPE_CCH_COMP_MAYBE_LOW, false);
}

static void adpt_intfr_usb_disconnect_work(struct work_struct *work)
{
	int i;
	struct adpt_intfr_dev *l_dev = NULL;

	for (i = 0; i < ADPT_INTRF_RETRY_TIMES; i++) {
		l_dev = adpt_intfr_get_dev();
		if (!l_dev || l_dev->intfr_type)
			return;
		power_event_report_str_uevent("BMS_EVT=CCH_COMP_LOW_ADAP_DISCONNECT");
		(void)power_msleep(DT_MSLEEP_50MS, 0, NULL);
	}
}

static void adpt_intfr_reset(struct adpt_intfr_dev *l_dev)
{
	l_dev->cable_type = ADPT_INTRF_CABLE_TYPE_NONE;
	if (l_dev->intfr_type) {
		l_dev->intfr_type = 0;
		cancel_delayed_work(&l_dev->resume_work);
		schedule_delayed_work(&l_dev->usb_disconnect_work, msecs_to_jiffies(0));
	}
}

static int adpt_intfr_notifier_call(struct notifier_block *nb, unsigned long event, void *data)
{
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_USB_DISCONNECT:
		adpt_intfr_reset(l_dev);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int adpt_intfr_parse_dts(struct adpt_intfr_dev *l_dev)
{
	int len;
	struct device_node *np = of_find_compatible_node(NULL, NULL, "huawei,charger");

	if (!l_dev || !np)
		return -EINVAL;

	len = of_property_count_u8_elems(np, "cch_comp_low_adap");
	if ((len <= 0) || (len > ADPT_INTRF_MAX_PARA))
		return -EINVAL;

	l_dev->cch_comp_low_adap = kzalloc(len * sizeof(*(l_dev->cch_comp_low_adap)),
		GFP_KERNEL);
	if (!l_dev->cch_comp_low_adap)
		return -ENOMEM;

	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"cch_comp_low_adap", l_dev->cch_comp_low_adap, len)) {
		kfree(l_dev->cch_comp_low_adap);
		l_dev->cch_comp_low_adap = NULL;
		return -EINVAL;
	}
	l_dev->cch_comp_low_adap_count = len;

	return 0;
}

static int __init adpt_intfr_init(void)
{
	struct adpt_intfr_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	if (adpt_intfr_parse_dts(l_dev)) {
		kfree(l_dev);
		return -EINVAL;
	}

	INIT_DELAYED_WORK(&l_dev->resume_work, adpt_intfr_resume_work);
	INIT_DELAYED_WORK(&l_dev->usb_disconnect_work, adpt_intfr_usb_disconnect_work);

	l_dev->plugged_nb.notifier_call = adpt_intfr_notifier_call;
	(void)power_event_bnc_register(POWER_BNT_CONNECT, &l_dev->plugged_nb);

	g_adpt_intfr_dev = l_dev;
	return 0;
}

static void __exit adpt_intfr_exit(void)
{
	struct adpt_intfr_dev *l_dev = adpt_intfr_get_dev();

	if (!l_dev)
		return;

	power_event_bnc_unregister(POWER_BNT_CONNECT, &l_dev->plugged_nb);
	if (l_dev->cch_comp_low_adap) {
		kfree(l_dev->cch_comp_low_adap);
		l_dev->cch_comp_low_adap = NULL;
	}
	kfree(l_dev);
	g_adpt_intfr_dev = NULL;
}

late_initcall(adpt_intfr_init);
module_exit(adpt_intfr_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("adapter interfere driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
