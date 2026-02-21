/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: bigdata dmd report
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <securec.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/power_supply.h>
#include <linux/workqueue.h>
#ifdef CONFIG_HUAWEI_OHOS_DSM
#include <dfx/hiview_hisysevent.h>
#elif defined(CONFIG_HUAWEI_DSM)
#include <log/hiview_hievent.h>
#endif
#include <platform_include/basicplatform/linux/power/platform/coul/coul_merge_drv.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_drv.h>
#include <platform_include/basicplatform/linux/power/platform/bci_battery.h>
#ifdef CONFIG_HUAWEI_CHARGER_AP
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#endif

#define coul_merge_debug(fmt, args...) pr_debug("[coul_merge_bigdata]" fmt, ## args)
#define coul_merge_info(fmt, args...) pr_info("[coul_merge_bigdata]" fmt, ## args)
#define coul_merge_warn(fmt, args...) pr_warn("[coul_merge_bigdata]" fmt, ## args)
#define coul_merge_err(fmt, args...) pr_err("[coul_merge_bigdata]" fmt, ## args)

#define THOUSAND					1000
#define COUL_MERGE_BIGDATA_OLD_ECM_VOLT			3200
#define COUL_MERGE_BIGDATA_NOT_CHG_DONE_CURR		100
#define COUL_MERGE_BIGDATA_PSY_NAME			"Battery"
#define COUL_MERGE_BIGDATA_EXTDMD_NO			930009012
#define COUL_MERGE_BIGDATA_SINGLE_NAME			"BAT_DISCHARGE_STATISTICS"
#define COUL_MERGE_BIGDATA_SINGLE_DOMAIN		"BATTERY"

#ifdef CONFIG_HUAWEI_OHOS_DSM
typedef struct hiview_hisysevent COUL_BIGDATA_EVENT;
#define COUL_BIGDATA_CREATE_EVENT(...)		\
hisysevent_create(COUL_MERGE_BIGDATA_SINGLE_DOMAIN, COUL_MERGE_BIGDATA_SINGLE_NAME, STATISTIC)
#define COUL_BIGDATA_PUT_INTEGER(evt, key, val)	hisysevent_put_integer(evt, key, val)
#define COUL_BIGDATA_WRITE_EVENT(evt)		hisysevent_write(evt)
#define COUL_BIGDATA_DESTROY_EVENT(evt)		hisysevent_destroy(&evt)
#elif defined(CONFIG_HUAWEI_DSM)
typedef struct hiview_hievent COUL_BIGDATA_EVENT;
#define COUL_BIGDATA_CREATE_EVENT(...)		\
hiview_hievent_create(COUL_MERGE_BIGDATA_EXTDMD_NO)
#define COUL_BIGDATA_PUT_INTEGER(evt, key, val)	hiview_hievent_put_integral(evt, key, val)
#define COUL_BIGDATA_WRITE_EVENT(evt)		hiview_hievent_report(evt)
#define COUL_BIGDATA_DESTROY_EVENT(evt)		hiview_hievent_destroy(evt)
#else
typedef void COUL_BIGDATA_EVENT;
#define COUL_BIGDATA_CREATE_EVENT(...) 		NULL
#define COUL_BIGDATA_PUT_INTEGER(evt, key, val)
#define COUL_BIGDATA_WRITE_EVENT(evt) 		0
#define COUL_BIGDATA_DESTROY_EVENT(evt)
#endif

enum coul_merge_bigdata_type {
	COUL_MERGE_BIGDATA_BY_CAP_ZERO = 0,
	COUL_MERGE_BIGDATA_BY_CAP_ONE = 1,
	COUL_MERGE_BIGDATA_BY_CHARGE_DONE = 101,
	COUL_MERGE_BIGDATA_BY_OLD_ECM_VOLT = -2,
};

enum coul_merge_bigdata_ecm_cap_one_method {
	COUL_MERGE_BIGDATA_CAP_ONE_NONE,
	COUL_MERGE_BIGDATA_CAP_ONE_SOC,
	COUL_MERGE_BIGDATA_CAP_ONE_VTH,
};

static bool g_bigdata_is_charge_done = false;
static bool g_bigdata_is_charge_done_now = false;
static int g_bigdata_cap_one_method = COUL_MERGE_BIGDATA_CAP_ONE_NONE;
static int g_bigdata_last_soc = -1;
struct bigdata_report_work {
	struct work_struct work;
	enum coul_merge_bigdata_type type;
};
static struct bigdata_report_work g_report_work;

static s64 coul_merge_bigdata_get_battery_cc_now(int batt_index)
{
	return ((coul_merge_drv_iscd_full_update_cc(batt_index) +
		coul_merge_drv_battery_cc_uah(batt_index)) / THOUSAND);
}

static int coul_merge_bigdata_get_ui_soc(void)
{
	int ret;
	struct power_supply *psy = NULL;
	union power_supply_propval val = {0};

	psy = power_supply_get_by_name(COUL_MERGE_BIGDATA_PSY_NAME);
	if (!psy) {
		coul_merge_err("get power supply by name failed\n");
		return -1;
	}

	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_CAPACITY, &val);
	power_supply_put(psy);
	if (ret == 0)
		return val.intval;
	coul_merge_err("get capacity from power_supply failed: %d\n", ret);
	return -1;
}

static void coul_merge_bigdata_event_report_work(struct work_struct *work)
{
	struct bigdata_report_work *report_work = container_of(work, struct bigdata_report_work, work);
	int type = report_work->type;
	int ret;
	int batt_index;
	COUL_BIGDATA_EVENT *event;

	coul_merge_info("%s %d\n", __func__, type);
	for (batt_index = 0; batt_index < coul_merge_drv_get_batt_count(); batt_index++) {
		event = COUL_BIGDATA_CREATE_EVENT();
		if (event == NULL) {
			coul_merge_err("%s hievent create fail\n", __func__);
			return;
		}
		COUL_BIGDATA_PUT_INTEGER(event, "Batt_Index", batt_index);
		COUL_BIGDATA_PUT_INTEGER(event, "Cap_One_Method", g_bigdata_cap_one_method);
		COUL_BIGDATA_PUT_INTEGER(event, "BatVolt", coul_merge_drv_battery_voltage_mv(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "RM", coul_merge_drv_battery_rm(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "FCC", coul_merge_drv_battery_fcc(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "BatTemp", coul_merge_drv_battery_temperature(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "UUC", coul_merge_drv_battery_uuc(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "CurAvg", coul_merge_drv_battery_uuc_average_current(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "CC", coul_merge_bigdata_get_battery_cc_now(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "SOC", coul_merge_drv_battery_capacity(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "UISOC", coul_merge_bigdata_get_ui_soc());
		COUL_BIGDATA_PUT_INTEGER(event, "ChrgDone", g_bigdata_is_charge_done ? 1 : 0);
		COUL_BIGDATA_PUT_INTEGER(event, "Off_Find_EN", coul_merge_drv_get_off_find(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "DecFullCutOff_BatVolt", coul_merge_drv_get_basp_nondc_volt_dec(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "SetShutdown_BatVolt", coul_merge_drv_get_basp_zero_ocv(batt_index));
		COUL_BIGDATA_PUT_INTEGER(event, "TriggerType", type);
		ret = COUL_BIGDATA_WRITE_EVENT(event);
		COUL_BIGDATA_DESTROY_EVENT(event);
		if (ret < 0) {
			coul_merge_err("%s hievent report fail: %d\n", __func__, ret);
			return;
		}
	}
	coul_merge_info("%s %d end\n", __func__, type);
}

static void coul_merge_bigdata_event_report(enum coul_merge_bigdata_type type)
{
	g_report_work.type = type;
	queue_work(system_power_efficient_wq, &g_report_work.work);
}

static int coul_merge_bigdata_charger_event_rcv(struct notifier_block *nb,
	unsigned long event, void *data)
{
	coul_merge_info("receive charge event %u\n", event);
	switch (event) {
	case VCHRG_START_USB_CHARGING_EVENT:
	case VCHRG_START_AC_CHARGING_EVENT:
	case VCHRG_START_CHARGING_EVENT:
		g_bigdata_is_charge_done = false;
		g_bigdata_is_charge_done_now = false;
		break;
	case VCHRG_CHARGE_DONE_EVENT:
		g_bigdata_is_charge_done = true;
		if (!g_bigdata_is_charge_done_now)
			coul_merge_bigdata_event_report(COUL_MERGE_BIGDATA_BY_CHARGE_DONE);
		g_bigdata_is_charge_done_now = true;
		break;
	case VCHRG_STOP_CHARGING_EVENT:
		g_bigdata_is_charge_done_now = false;
		break;
	case BATTERY_LOW_SHUTDOWN:
		coul_merge_bigdata_event_report(COUL_MERGE_BIGDATA_BY_CAP_ZERO);
		break;
	case BATTERY_CAP_ONE_BY_VTH:
		if (g_bigdata_cap_one_method == COUL_MERGE_BIGDATA_CAP_ONE_NONE)
			g_bigdata_cap_one_method = COUL_MERGE_BIGDATA_CAP_ONE_VTH;
		break;
	default:
		break;
	}
	return 0;
}

static int coul_merge_bigdata_power_supply_event_rcv(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct power_supply *psy = data;
	int soc = 0;

	if (strcmp(psy->desc->name, COUL_MERGE_BIGDATA_PSY_NAME) != 0)
		return 0;

	soc = coul_merge_bigdata_get_ui_soc();
	if (soc >= 0) {
		if (g_bigdata_last_soc > CAPACITY_ONE && soc == CAPACITY_ONE)
			coul_merge_bigdata_event_report(COUL_MERGE_BIGDATA_BY_CAP_ONE);
		if (g_bigdata_last_soc > CAPACITY_ZERO && soc == CAPACITY_ZERO)
			coul_merge_bigdata_event_report(COUL_MERGE_BIGDATA_BY_CAP_ZERO);
		g_bigdata_last_soc = soc;

		if (soc > CAPACITY_ONE)
			g_bigdata_cap_one_method = COUL_MERGE_BIGDATA_CAP_ONE_NONE;
		else if (soc == CAPACITY_ONE && g_bigdata_cap_one_method == COUL_MERGE_BIGDATA_CAP_ONE_NONE)
			g_bigdata_cap_one_method = COUL_MERGE_BIGDATA_CAP_ONE_SOC;
	}

	return 0;
}

static struct notifier_block coul_merge_bigdata_charger_notifier = {
	.notifier_call = coul_merge_bigdata_charger_event_rcv,
};

static struct notifier_block coul_merge_bigdata_power_supply_notifier = {
	.notifier_call = coul_merge_bigdata_power_supply_event_rcv,
};

#ifdef CONFIG_HUAWEI_CHARGER_AP
static int coul_merge_bigdata_low_power_event_rcv(struct notifier_block *nb, unsigned long action, void *data)
{
	if (action == POWER_NE_UI_LOW_POWER_FORCE_ZERO) {
		coul_merge_info("receive POWER_NE_UI_LOW_POWER_FORCE_ZERO\n");
		coul_merge_bigdata_event_report(COUL_MERGE_BIGDATA_BY_CAP_ZERO);
	}
	return 0;
}

static struct notifier_block coul_merge_bigdata_low_power_notifier = {
	.notifier_call = coul_merge_bigdata_low_power_event_rcv,
};
#endif

static int __init coul_merge_bigdata_init(void)
{
	coul_merge_info("%s\n", __func__);
	INIT_WORK(&g_report_work.work, coul_merge_bigdata_event_report_work);
	bci_register_notifier(&coul_merge_bigdata_charger_notifier, 1);
	power_supply_reg_notifier(&coul_merge_bigdata_power_supply_notifier);
#ifdef CONFIG_HUAWEI_CHARGER_AP
	power_event_bnc_register(POWER_BNT_COUL, &coul_merge_bigdata_low_power_notifier);
#endif
	coul_merge_info("%s end\n", __func__);
	return 0;
}

static void __exit coul_merge_bigdata_exit(void)
{
	coul_merge_info("%s\n", __func__);
}

device_initcall(coul_merge_bigdata_init)
module_exit(coul_merge_bigdata_exit);

MODULE_DESCRIPTION("coul merge bigdata");
MODULE_LICENSE("GPL");
