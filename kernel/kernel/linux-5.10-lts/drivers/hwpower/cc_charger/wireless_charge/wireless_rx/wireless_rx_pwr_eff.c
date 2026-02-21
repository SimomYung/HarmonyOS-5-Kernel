// SPDX-License-Identifier: GPL-2.0
/*
 * wireless_rx_pwr_eff.c
 *
 * wireless charging pwr efficiency
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/atomic.h>
#include <linux/errno.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/protocol/wireless_protocol.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_acc.h>
#include <chipset_common/hwpower/wireless_charge/wireless_trx_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_trx_ic_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ic_intf.h>

#define HWLOG_TAG wireless_rx_pwr_eff
HWLOG_REGIST();

#define WLRX_PWR_EFF_GET_MAX_TIMES          10
#define WLRX_PWR_EFF_GET_INTERVAL           3000   /* 3s */
#define WLRX_PWR_EFF_GET_DC_DELAY           15000  /* 15s */

struct wlrx_pwr_eff_dev {
	int pwr_eff;
	unsigned int drv_type;
	struct delayed_work wlrx_pwr_eff_calc_work;
	struct notifier_block wlc_nb;
	atomic_t count;
};

static struct wlrx_pwr_eff_dev *g_rx_pwr_eff_di[WLTRX_DRV_MAX];

static bool wlrx_support_get_tx_actual_power(unsigned int drv_type)
{
	struct wlprot_acc_cap *cap = wlrx_acc_get_cap(drv_type);

	return cap && cap->support_fod_info;
}

static int wlrx_calculate_pwr_eff(struct wlrx_pwr_eff_dev *di)
{
	unsigned int ic_type;
	int ptx = 0;
	int prx = 0;
	int ret;

	ic_type = wltrx_get_dflt_ic_type(di->drv_type);
	ret = wireless_get_tx_actual_power(ic_type, WIRELESS_PROTOCOL_QI, &ptx);
	if (ret) {
		hwlog_err("%s: wireless_get_tx_actual_power fail, ret=%d\n", __func__, ret);
		return ret;
	}

	ret = wlrx_ic_get_prx(ic_type, &prx);
	if (ret) {
		hwlog_err("%s: wlrx_ic_get_prx fail, ret=%d\n", __func__, ret);
		return ret;
	}

	if ((ptx <= 0) || (prx <= 0)) {
		hwlog_err("%s:ptx:%d ptx:%d, is illegal\n", __func__, ptx, prx);
		return -EINVAL;
	}

	di->pwr_eff = prx * POWER_PERCENT / ptx;
	hwlog_info("[%s] ptx=%d prx=%d ratio=%d", __func__, ptx, prx, di->pwr_eff);

	return 0;
}

static void wlrx_pwr_eff_calc_work(struct work_struct *work)
{
	struct wlrx_pwr_eff_dev *di = container_of(work,
		struct wlrx_pwr_eff_dev, wlrx_pwr_eff_calc_work.work);

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}

	if (atomic_read(&di->count) >= WLRX_PWR_EFF_GET_MAX_TIMES) {
		hwlog_info("[%s]:finished %d times getting pwr_eff\n", __func__, WLRX_PWR_EFF_GET_MAX_TIMES);
		return;
	}

	if (wlrx_calculate_pwr_eff(di))
		hwlog_err("%s:calculate power efficiency fail\n", __func__);

	atomic_inc(&di->count);
	mod_delayed_work(system_wq, &di->wlrx_pwr_eff_calc_work,
		msecs_to_jiffies(WLRX_PWR_EFF_GET_INTERVAL));
}

static int wlrx_pwr_eff_wlc_call(struct notifier_block *nb, unsigned long event, void *data)
{
	struct wlrx_pwr_eff_dev *di = container_of(nb, struct wlrx_pwr_eff_dev, wlc_nb);

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return NOTIFY_OK;
	}

	switch (event) {
	case POWER_NE_WLC_DC_START_CHARGING:
		if (!wlrx_support_get_tx_actual_power(di->drv_type)) {
			hwlog_info("[%s]: get_tx_actual_power not support\n", __func__);
			return NOTIFY_OK;
		}
		atomic_set(&di->count, 0);
		mod_delayed_work(system_wq, &di->wlrx_pwr_eff_calc_work,
			msecs_to_jiffies(WLRX_PWR_EFF_GET_DC_DELAY));
		hwlog_info("[%s]: enter dc, wait for %dms\n", __func__, WLRX_PWR_EFF_GET_DC_DELAY);
		break;
	case POWER_NE_WLC_DC_STOP_CHARGING:
		cancel_delayed_work_sync(&di->wlrx_pwr_eff_calc_work);
		hwlog_info("[%s]: exit dc, stop get pwr_effn\n", __func__);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

int wlrx_pwr_eff_init(unsigned int drv_type)
{
	int ret;
	struct wlrx_pwr_eff_dev *di = NULL;

	if (!wltrx_is_drv_type_valid(drv_type))
		return -EINVAL;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	atomic_set(&di->count, 0);
	INIT_DELAYED_WORK(&di->wlrx_pwr_eff_calc_work, wlrx_pwr_eff_calc_work);

	di->wlc_nb.notifier_call = wlrx_pwr_eff_wlc_call;
	di->wlc_nb.priority = 0;
	ret = power_event_bnc_register(POWER_BNT_WLC, &di->wlc_nb);
	if (ret) {
		hwlog_err("%s: register wlc_nb fail\n", __func__);
		kfree(di);
		return ret;
	}

	di->drv_type = drv_type;
	g_rx_pwr_eff_di[drv_type] = di;

	return 0;
}

void wlrx_pwr_eff_deinit(unsigned int drv_type)
{
	struct wlrx_pwr_eff_dev *di = g_rx_pwr_eff_di[drv_type];

	if (!di || !wltrx_is_drv_type_valid(drv_type))
		return;

	cancel_delayed_work_sync(&di->wlrx_pwr_eff_calc_work);
	(void)power_event_bnc_unregister(POWER_BNT_WLC, &di->wlc_nb);

	kfree(g_rx_pwr_eff_di[drv_type]);
	g_rx_pwr_eff_di[drv_type] = NULL;
}
