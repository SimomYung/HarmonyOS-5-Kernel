/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device driver for schargerV800 irq
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regmap.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/of_irq.h>

#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_error_handle.h>
#include <huawei_platform/power/huawei_charger.h>
#include "scharger_v800_buck.h"
#include "scharger_v800_regs.h"
#include "../scharger_dynamic_cv.h"
#include "scharger_v800_dc.h"
#include "scharger_v800_adc.h"
#include "scharger_v800_flash_mix.h"

#define DSM_BUFF_SIZE_MAX       256

static int scharger_get_irq_byname(struct device *dev, const char *name)
{
	if (dev == NULL || name == NULL)
		return -EINVAL;
	if (dev->of_node->name != NULL)
		scharger_err("%s: name:%s\n", __func__, dev->of_node->name);
	else
		scharger_err("%s: of_node->name is NULL!\n", __func__);
	return of_irq_get_byname(dev->of_node, name);
}

static void scharger_chg_irq_prc(struct scharger_buck *buck)
{
	int i;
	u32 val = 0;

	for (i = 0; i < CHG_IRQ_BULK_NUM; i++) {
		val = 0;
		regmap_read(buck->regmap, CHG_IRQ_FLAG_0_REG + i, &val);
		regmap_write(buck->regmap, CHG_IRQ_FLAG_0_REG + i, val);
		if(val)
			scharger_inf("%s 0x%x = 0x%x\n", __func__, CHG_IRQ_FLAG_0_REG + i, val);
	}
}

static int scharger_dsm_report_fault_info(const char *buff, int err_num)
{
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	char dsm_buf[DSM_BUFF_SIZE_MAX] = {0};
	int ret = 0;

	if (buff == NULL)
		return -1;

	ret = sprintf_s(dsm_buf, DSM_BUFF_SIZE_MAX, "%s\n", buff);
	if (ret < 0)
		scharger_err("%s, sprintf_s error\n", __func__);

	return power_dsm_report_dmd(POWER_DSM_BATTERY, err_num, dsm_buf);
#else
	return 0;
#endif
}

static void scharger_buck_vbat_ovp_handle(struct scharger_buck *buck)
{
	int i, ret;
	u8 vbat_ovp_cnt = 0;
	unsigned int irq_st = 0;

	scharger_err("%s\n", __func__);
	for (i = 0; i < 5; i++) { /* status number is 5 */
		ret = regmap_read(buck->regmap, CHG_IRQ_STATUS_2_REG, &irq_st);
		if (ret || ((irq_st & CHG_VBAT_OVP_MSK) == CHG_VBAT_OVP_MSK)) {
			vbat_ovp_cnt++;
			mdelay(2); /* 2: need 2mS for next read */
		} else {
			vbat_ovp_cnt = 0;
			break;
		}
	}
	if (vbat_ovp_cnt >= 5) { /* 5: total cnt of ovp */
		scharger_err("%s:CHARGE_FAULT_VBAT_OVP\n", __func__);
#ifdef CONFIG_HUAWEI_CHARGER_AP
		power_event_anc_notify(POWER_ANT_CHARGE_FAULT,
			POWER_NE_CHG_FAULT_VBAT_OVP, NULL);
#endif
	}
}

static void scharger_direct_charge_fault_report(u32 fault_type, struct nty_data *data)
{
	unsigned int notifier_type;
	struct current_charger_mode *charger_mode;

	charger_mode = scharger_v800_get_current_charge_mode();
	if (charger_mode == NULL) {
		scharger_inf("%s charger_mode is NULL\n", __func__);
		return;
	}
	scharger_inf("%s chg_mode:%d\n", __func__, charger_mode->chg_mode);
	if (!(charger_mode->chg_mode == SC21 || charger_mode->chg_mode == SC41 || charger_mode->chg_mode == SC11))
		return;

	if (charger_mode->chg_mode == SC21)
		notifier_type = POWER_ANT_SC_FAULT;
	else if (charger_mode->chg_mode == SC41)
		notifier_type = POWER_ANT_SC4_FAULT;
	else
		notifier_type = POWER_ANT_LVC_FAULT;

	power_event_anc_notify(notifier_type, fault_type, data);
}

static void otg_scp_dmd_report(void)
{
	char buf[DSM_BUFF_SIZE_MAX];
	int ret = 0;
	int vbat, ibat, vbus;

	vbat = coul_drv_battery_voltage();
	ibat = coul_drv_battery_current();
	vbus = scharger_v800_get_vbus();
	scharger_inf("charge_fault:otg scp happened!, vbat:%d, ibat:%d, vbus:%d\n",
		vbat, ibat, vbus);
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	ret = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
		"otg scp happened!, vbat:%d, ibat:%d, vbus:%d\n",
		vbat, ibat, vbus);
	if (ret < 0)
		scharger_err("%s, offset %d, snprintf_s error\n",
			     __func__, ret);
	scharger_dsm_report_fault_info(buf, DSM_SC0_INFO);
#endif
}

static void chg_fault_direct_charge_handle(u32 * chg_irq_state)
{
#ifdef CONFIG_DIRECT_CHARGER
	u32 fault_type = POWER_NE_DC_FAULT_NON;
	struct nty_data data;

	data.addr = 6; /* spmi slave id 6 */

	if (chg_irq_state[CHG_IRQ3] & FAULT_TDIE_OTP) {
		scharger_inf("%s:FAULT_TDIE_OTP, tdie:%d\n", __func__, scharger_v800_get_tdie());
		fault_type = POWER_NE_DC_FAULT_TDIE_OTP;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if (chg_irq_state[CHG_IRQ3] & FAULT_TBAT_OTP) {
		scharger_inf("%s:FAULT_TBAT_OTP, tsbat:%d\n", __func__, scharger_v800_get_tsbat());
		fault_type = POWER_NE_DC_FAULT_TSBAT_OTP;
		scharger_direct_charge_fault_report(fault_type, &data);
		scharger_v800_flash_set_led_en(0);
	}

	if (chg_irq_state[CHG_IRQ3] & FAULT_WDT_TOT) {
		fault_type = POWER_NE_DC_FAULT_IC;
		scharger_direct_charge_fault_report(fault_type, &data);
		scharger_v800_flash_set_led_en(0);
	}

	if (chg_irq_state[CHG_IRQ1] & FAULT_REGN_OCP) {
		scharger_inf("%s:charge_fault:FAULT_REGN_OCP happened!\n", __func__);
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
		scharger_dsm_report_fault_info("FAULT_REGN_OCP happened\n", DSM_SC0_WARNING);
#endif
	}

	if ((chg_irq_state[CHG_IRQ0] & FAULT_VUSB_OVP) ||
		(chg_irq_state[CHG_IRQ1] & FAULT_VPSW_OVP) ||
		(chg_irq_state[CHG_IRQ1] & FAULT_REGN_OCP)) {
		scharger_inf("%s:vusb:%d\n", __func__, scharger_v800_get_vusb());
		fault_type = POWER_NE_DC_FAULT_IC;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if ((chg_irq_state[CHG_IRQ0] & FAULT_VUSB_OVP) ||
		(chg_irq_state[CHG_IRQ1] & FAULT_VPSW_OVP) ||
		(chg_irq_state[CHG_IRQ1] & FAULT_REGN_OCP) ||
		(chg_irq_state[CHG_IRQ3] & FAULT_TDIE_OTP) ||
		(chg_irq_state[CHG_IRQ3] & FAULT_TBAT_OTP))
		sc_charge_disable_for_irq();
#endif
}

static void gate_vdropovp_dmd_report(void)
{
	char buf[DSM_BUFF_SIZE_MAX];
	int ret = 0;
	int coul_vbat, coul_ibat, ibus, vbus;

	coul_vbat = coul_drv_battery_voltage();
	coul_ibat = coul_drv_battery_current();
	ibus = scharger_v800_get_ibus();
	vbus = scharger_v800_get_vbus();
	scharger_inf("charge_fault: gate vdrop ovp happened!, vbat:%d, ibat:%d, ibus:%d, vbus:%d\n",
		coul_vbat, coul_ibat, ibus, vbus);
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	ret = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
		"gate vdrop ovp happened, vbat:%d, ibat:%d, ibus:%d, vbus:%d\n", coul_vbat, coul_ibat, ibus, vbus);
	if (ret < 0)
		scharger_err("%s, offset %d, snprintf_s error\n",
			     __func__, ret);
	scharger_dsm_report_fault_info(buf, DSM_SC0_WARNING);
#endif
}

void scharger_v800_buck_irq_usb_ovp_work(struct work_struct *work)
{
	struct scharger_buck *buck =
		 container_of(work, struct scharger_buck, buck_irq_usb_ovp_work.work);
	int vol;
	unsigned int usb_ovp_mode;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return;
	}

	scharger_inf("%s\n", __func__);
	scharger_v800_mode_lock();
	if ((buck->current_charge_mode.chg_mode != BUCK5V) && (buck->current_charge_mode.chg_mode != UNKNOWN)) {
		scharger_v800_mode_unlock();
		scharger_inf("%s, chg mode is not BUCK5V or UNKNOWN, %d\n", __func__, buck->current_charge_mode.chg_mode);
		return;
	}

	vol = scharger_v800_get_vusb();
	if (vol > VBUS_6000MV) {
		scharger_v800_mode_unlock();
		scharger_inf("%s,vol = %d, continue!\n", __func__, vol);
		queue_delayed_work(system_power_efficient_wq, &buck->buck_irq_usb_ovp_work,
			msecs_to_jiffies(BUCK_IRQ_USB_PSW_OVP_WOEK_DELAY));
		return;
	}
	usb_ovp_mode = scharger_v800_get_usb_ovp_mode();
	if (usb_ovp_mode == USBOVP_FORWARD_MODE) {
		scharger_set_q1q1_ss_buck_mode(FORCE_OPEN);
		udelay(10);
		scharger_v800_set_usb_ovp_mode(USBOVP_FORCE_CLOSE);
		udelay(10);
		scharger_v800_set_usb_ovp_mode(USBOVP_FORWARD_MODE);
		udelay(10);
		scharger_set_q1q1_ss_buck_mode(AUTO_MODE);
	}
	scharger_v800_mode_unlock();
}

void scharger_v800_buck_irq_psw_ovp_work(struct work_struct *work)
{
	struct scharger_buck *buck =
		 container_of(work, struct scharger_buck, buck_irq_psw_ovp_work.work);
	int vpsw;
	unsigned int psw_ovp_mode;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return;
	}

	scharger_inf("%s\n", __func__);
	scharger_v800_mode_lock();
	if ((buck->current_charge_mode.chg_mode != BUCK5V) && (buck->current_charge_mode.chg_mode != UNKNOWN)) {
		scharger_v800_mode_unlock();
		scharger_inf("%s, chg mode is not BUCK5V or UNKNOWN, %d\n", __func__, buck->current_charge_mode.chg_mode);
		return;
	}

	vpsw = scharger_v800_get_vpsw();
	if (vpsw > VBUS_6000MV) {
		scharger_v800_mode_unlock();
		scharger_inf("%s,vpsw = %d, continue!\n", __func__, vpsw);
		queue_delayed_work(system_power_efficient_wq, &buck->buck_irq_psw_ovp_work,
			msecs_to_jiffies(BUCK_IRQ_USB_PSW_OVP_WOEK_DELAY));
		return;
	}

	psw_ovp_mode = scharger_v800_get_psw_ovp_mode();
	if (psw_ovp_mode == PSWOVP_FORWARD_MODE) {
		scharger_set_q1q1_ss_buck_mode(FORCE_OPEN);
		udelay(10);
		scharger_v800_set_psw_ovp_mode(PSWOVP_FORCE_CLOSE);
		udelay(10);
		scharger_v800_set_psw_ovp_mode(PSWOVP_FORWARD_MODE);
		udelay(10);
		scharger_set_q1q1_ss_buck_mode(AUTO_MODE);
	}
	scharger_v800_mode_unlock();
}

static void chg_fault_handle(struct scharger_buck *buck, u32 *chg_irq_state)
{
	int vbus, vbat, ichg, dpm_state;

	if (chg_irq_state[CHG_IRQ0] & IRQ_PSW_PLUGOUT) {
		scharger_v800_mode_lock();
		(void)scharger_v800_set_vpsw_ovp(BUCK5V);
		scharger_v800_mode_unlock();
	}

	if (chg_irq_state[CHG_IRQ0] & IRQ_OVP_VUSB_OVP)
		queue_delayed_work(system_power_efficient_wq, &buck->buck_irq_usb_ovp_work,
			msecs_to_jiffies(BUCK_IRQ_USB_PSW_OVP_WOEK_DELAY));

	if (chg_irq_state[CHG_IRQ1] & IRQ_OVP_VPSW_OVP)
		queue_delayed_work(system_power_efficient_wq, &buck->buck_irq_psw_ovp_work,
			msecs_to_jiffies(BUCK_IRQ_USB_PSW_OVP_WOEK_DELAY));

	if (chg_irq_state[CHG_IRQ2] & FAULT_BUCK_VBAT_OVP) {
		scharger_err("%s:FAULT_BUCK_VBAT_OVP, vbat:%d\n", __func__, scharger_v800_get_vbat());
		scharger_buck_vbat_ovp_handle(buck);
	}

	if (chg_irq_state[CHG_IRQ1] & FAULT_OTG_SCP)
		otg_scp_dmd_report();

	if (chg_irq_state[CHG_IRQ3] & FAULT_REVERSBST) {
		vbus = scharger_v800_get_vbus();
		vbat = scharger_v800_get_vbat();
		ichg = coul_drv_battery_current();
		dpm_state = scharger_check_input_dpm_state();
		scharger_err("%s:charge_fault:irq_reversbst, cnt:%d, vbus:%d, vbat:%d, ichg:%d, dpm_state:%d\n",
			     __func__, buck->reverbst_cnt, vbus, vbat, ichg, dpm_state);
		buck->reverbst_cnt++;
		if (buck->reverbst_cnt < REVERBST_RETRY) {
			scharger_v800_set_anti_reverbst_reset(buck);
		} else {
			scharger_v800_set_boot_weaksource_flag();
			scharger_err("%s:charge_fault:CHARGE_FAULT_WEAKSOURCE\n", __func__);
			power_event_anc_notify(POWER_ANT_CHARGE_FAULT,
				POWER_NE_CHG_FAULT_WEAKSOURCE, NULL);
		}
	}

	if (chg_irq_state[CHG_IRQ3] & FAULT_CHG_DONE) {
#ifdef CONFIG_SCHARGER_DYNAMIC_CV
		if (scharger_cv_is_force_trim() == 1) {
			scharger_v800_set_term_enable(CHG_TERM_DIS);
			scharger_v800_set_charge_enable(CHG_DISABLE);
			mdelay(5);
			scharger_v800_set_charge_enable(CHG_ENABLE);
			scharger_inf("[%s]force trim CV\n", __func__);
		} else {
			power_event_anc_notify(POWER_ANT_CHARGE_FAULT,
				POWER_NE_CHG_FAULT_CHARGE_DONE, NULL);
			scharger_err("%s : CHARGE_FAULT_CHARGE_DONE\n", __func__);
		}
#else
		power_event_anc_notify(POWER_ANT_CHARGE_FAULT,
			POWER_NE_CHG_FAULT_CHARGE_DONE, NULL);
		scharger_err("%s : CHARGE_FAULT_CHARGE_DONE\n", __func__);
#endif
	}

	if (chg_irq_state[CHG_IRQ3] & FAULT_CHG_VBAT_LV) {
		vbat = scharger_v800_get_vbat();
		scharger_err("%s:FAULT_CHG_VBAT_LV vbat:%d\n", __func__, vbat);
		scharger_v800_vbat_lv_handle(buck);
	}

	if (chg_irq_state[CHG_IRQ5] & FAULT_GATE_VDROP_OVP)
		gate_vdropovp_dmd_report();

	if ((chg_irq_state[CHG_IRQ2] & IRQ_VSYS_H_SCP) ||
		(chg_irq_state[CHG_IRQ2] & IRQ_VSYS_H_OVP) ||
		(chg_irq_state[CHG_IRQ1] & IRQ_BUCK_OCP))
		scharger_v800_flash_set_led_en(0);

	chg_fault_direct_charge_handle(chg_irq_state);
}

static void pswovp_vdrop_min_dmd_report(void)
{
	char buf[DSM_BUFF_SIZE_MAX];
	int ret = 0;
	int vbat, vpsw;
	unsigned int sc_mode;

	vbat = coul_drv_battery_voltage();
	vpsw = scharger_v800_get_vpsw();
	sc_mode = scharger_v800_get_sc_mode();
	scharger_inf("charge_fault:vdrop min happened!, vbat:%d, vpsw:%d, sc_mode:%u\n",
		vbat, vpsw, sc_mode);
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	ret = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
		"vdrop min happend, vbat:%d, vpsw:%d, sc_mode:%u\n", vbat, vpsw, sc_mode);
	if (ret < 0)
		scharger_err("%s, offset %d, snprintf_s error\n",
			     __func__, ret);
	scharger_dsm_report_fault_info(buf, DSM_SC0_INFO);
#endif
}

static void ocp_peak_dmd_report(u32 ocp_peak_state)
{
	char buf[DSM_BUFF_SIZE_MAX];
	int ret = 0;

#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	ret = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
		"OCP PEAK 0x%x happened\n", ocp_peak_state);
	if (ret < 0)
		scharger_err("%s, offset %d, snprintf_s error\n",
			     __func__, ret);
	scharger_dsm_report_fault_info(buf, DSM_SC0_WARNING);
#endif
}

static void sc_fault_handle(struct scharger_buck *buck, u32 *sc_irq_state)
{
#ifdef CONFIG_DIRECT_CHARGER
	u32 fault_type = POWER_NE_DC_FAULT_NON;
	struct nty_data data;
	u32 ocp_peak_state;

	data.addr = 6; /* spmi slave id 6 */

	if (sc_irq_state[SC_IRQ1] & FAULT_IBUS_OCP) {
		scharger_inf("%s:FAULT_IBUS_OCP, ibus:%d\n", __func__, scharger_v800_get_ibus());
		fault_type = POWER_NE_DC_FAULT_IBUS_OCP;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if (sc_irq_state[SC_IRQ0] & FAULT_VDROP_OVP) {
		scharger_inf("%s:FAULT_VDROP_OVP, vbus:%d\n", __func__, scharger_v800_get_vbus());
		fault_type = POWER_NE_DC_FAULT_VDROP_OVP;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if (sc_irq_state[SC_IRQ1] & FAULT_VBAT_OVP) {
		scharger_inf("%s:FAULT_VBAT_OVP, vbat:%d\n", __func__, scharger_v800_get_vbat());
		fault_type = POWER_NE_DC_FAULT_VBAT_OVP;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if (sc_irq_state[SC_IRQ1] & FAULT_IBAT_OCP) {
		scharger_inf("%s:FAULT_IBAT_OCP, ibat:%d\n", __func__, scharger_v800_get_ibat());
		fault_type = POWER_NE_DC_FAULT_IBAT_OCP;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if (sc_irq_state[SC_IRQ1] & FAULT_REVERSE_OCP) {
		scharger_inf("%s:FAULT_REVERSE_OCP, ibus:%d\n", __func__, scharger_v800_get_ibus());
		fault_type = POWER_NE_DC_FAULT_REVERSE_OCP;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if (sc_irq_state[SC_IRQ1] & FAULT_IBUS_UCP) {
		scharger_inf("%s:FAULT_IBUS_UCP, ibus:%d\n", __func__, scharger_v800_get_ibus());
		fault_type = POWER_NE_DC_FAULT_IC;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	ocp_peak_state = sc_irq_state[SC_IRQ2] & (FAULT_SC1_Q4_OCP_PEAK | FAULT_SC1_Q5_OCP_PEAK |
		FAULT_SC1_Q10_OCP_PEAK | FAULT_SC2_Q4_OCP_PEAK | FAULT_SC2_Q5_OCP_PEAK |
		FAULT_SC2_Q10_OCP_PEAK);
	if (ocp_peak_state) {
		scharger_inf("%s:charge_fault:ocp peak 0x%x happened!\n", __func__, ocp_peak_state);
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
		ocp_peak_dmd_report(ocp_peak_state);
#endif
	}

	if ((sc_irq_state[SC_IRQ1] & (FAULT_VOUT_OVP | FAULT_VOUT_UVLO |
		FAULT_CFLY1_SCP)) || ocp_peak_state) {
		fault_type = POWER_NE_DC_FAULT_IC;
		scharger_direct_charge_fault_report(fault_type, &data);
	}

	if (sc_irq_state[SC_IRQ0] & FAULT_PSW_VDROP_MIN) {
		pswovp_vdrop_min_dmd_report();
		fault_type = POWER_NE_DC_FAULT_IC;
		scharger_direct_charge_fault_report(fault_type, &data);
	}
	sc_charge_disable_for_irq();
#endif
}

void scharger_v800_irq_work(struct work_struct *work)
{
	u32 irq_state = 0;
	u32 chg_irq_state[CHG_IRQ_BULK_NUM] = {0};
	u32 sc_irq_state[SC_IRQ_BULK_NUM] = {0};
	int ret = 0;
	int i;
	struct irq_desc *chg_desc = NULL;
	struct irq_desc *sc_desc = NULL;
	struct scharger_buck *buck =
		 container_of(work, struct scharger_buck, irq_work);

	ret = regmap_read(buck->regmap, CHG_IRQ_FLAG_REG, &irq_state);
	if (ret)
		scharger_inf("%s: return error\n", __func__);

	scharger_inf("%s :irq_state:0x%x\n", __func__, irq_state);
	if (irq_state & CHG_IRQ) {
		/* CHG IRQ:0x802~0x807 */
		for (i = 0; i < CHG_IRQ_BULK_NUM; i++) {
			regmap_read(buck->regmap, CHG_IRQ_FLAG_0_REG + i, &chg_irq_state[i]);
			scharger_inf("%s:CHG_IRQ, chg_irq_state[0x%x]:0x%x\n",
				__func__, CHG_IRQ_FLAG_0_REG + i, chg_irq_state[i]);
		}
		chg_fault_handle(buck, chg_irq_state);
		for (i = 0; i < CHG_IRQ_BULK_NUM; i++)
			regmap_write(buck->regmap, CHG_IRQ_FLAG_0_REG + i, chg_irq_state[i]);
	}

	if (irq_state & SC_IRQ) {
		/* SC IRQ:0x808~0x80A */
		for (i = 0; i < SC_IRQ_BULK_NUM; i++) {
			regmap_read(buck->regmap, CHG_IRQ_FLAG_6_REG + i, &sc_irq_state[i]);
			scharger_inf("%s:SC_IRQ, sc_irq_state[0x%x]:0x%x\n",
				__func__, CHG_IRQ_FLAG_6_REG + i, sc_irq_state[i]);
		}
		sc_fault_handle(buck, sc_irq_state);
		for (i = 0; i < SC_IRQ_BULK_NUM; i++)
			regmap_write(buck->regmap, CHG_IRQ_FLAG_6_REG + i, sc_irq_state[i]);
	}

	chg_desc = irq_to_desc(buck->chg_irq);
	sc_desc = irq_to_desc(buck->sc_irq);
	if (chg_desc == NULL ||  sc_desc == NULL) {
		scharger_err("[%s]irq_to_desc failed\n", __func__);
		return;
	}
	if (chg_desc->irq_data.chip->irq_unmask != NULL)
		chg_desc->irq_data.chip->irq_unmask(&chg_desc->irq_data);
	if (sc_desc->irq_data.chip->irq_unmask != NULL)
		sc_desc->irq_data.chip->irq_unmask(&sc_desc->irq_data);
}

static irqreturn_t scharger_irq_handle(int irq, void *_buck)
{
	struct scharger_buck *buck = _buck;
	struct irq_desc *chg_desc = NULL;
	struct irq_desc *sc_desc = NULL;

	chg_desc = irq_to_desc(buck->chg_irq);
	sc_desc = irq_to_desc(buck->sc_irq);
	if (chg_desc == NULL ||  sc_desc == NULL) {
		scharger_err("[%s]irq_to_desc failed\n", __func__);
		return IRQ_NONE;
	}

	if (chg_desc->irq_data.chip->irq_mask != NULL)
		chg_desc->irq_data.chip->irq_mask(&chg_desc->irq_data);
	if (sc_desc->irq_data.chip->irq_mask != NULL)
		sc_desc->irq_data.chip->irq_mask(&sc_desc->irq_data);

	(void)queue_work(system_power_efficient_wq, &buck->irq_work);
	return IRQ_HANDLED;
}

int scharger_v800_chg_irq_init(struct scharger_buck *buck)
{
	int ret = 0;

	buck->chg_irq = scharger_get_irq_byname(buck->dev, "chg_irq");
	if (buck->chg_irq < 0) {
		scharger_err("failed to get chg_irq %d\n", buck->chg_irq);
		return -ENOENT;
	}
	scharger_inf("chg_irq: %d\n", buck->chg_irq);
	scharger_chg_irq_prc(buck);
	ret = request_irq(buck->chg_irq, scharger_irq_handle, IRQF_NO_SUSPEND, "chg_irq", buck);
	if (ret != 0) {
		free_irq(buck->chg_irq, buck);
		scharger_err("request chg_irq failed\n");
		return ret;
	}

	scharger_err("%s -\n", __func__);
	return ret;
}

static void scharger_sc_irq_prc(struct scharger_buck *buck)
{
	int i;
	u32 val = 0;

	for (i = 0; i < SC_IRQ_BULK_NUM; i++) {
		val = 0;
		regmap_read(buck->regmap, CHG_IRQ_FLAG_6_REG + i, &val);
		regmap_write(buck->regmap, CHG_IRQ_FLAG_6_REG + i, val);
		if(val)
			scharger_inf("%s 0x%x = 0x%x\n", __func__, CHG_IRQ_FLAG_6_REG + i, val);
	}
}

int scharger_v800_sc_irq_init(struct scharger_buck *buck)
{
	int ret = 0;

	buck->sc_irq = scharger_get_irq_byname(buck->dev, "sc_irq");
	if (buck->sc_irq < 0) {
		scharger_err("failed to get sc_irq %d\n", buck->sc_irq);
		return -ENOENT;
	}
	scharger_inf("sc_irq: %d\n", buck->sc_irq);
	scharger_sc_irq_prc(buck);
	ret = request_irq(buck->sc_irq, scharger_irq_handle, IRQF_NO_SUSPEND, "sc_irq", buck);
	if (ret != 0) {
		free_irq(buck->sc_irq, buck);
		scharger_err("request sc_irq failed\n");
		return ret;
	}

	scharger_err("%s -\n", __func__);
	return ret;
}

static void flash_fault_handle(u32 *flash_irq_state)
{
	if ((flash_irq_state[FLASH_IRQ0] &
		(IRQ_LED1_SCP | IRQ_LED1_OPEN | IRQ_LED2_SCP | IRQ_LED2_OPEN)))
		scharger_v800_flash_set_led_en(0);
}

void scharger_v800_flash_irq_work(struct work_struct *work)
{
	u32 irq_state = 0;
	u32 flash_irq_state[FLASH_IRQ_BULK_NUM] = {0};
	int ret = 0;
	int i;
	struct irq_desc *flash_desc = NULL;
	struct scharger_v800_private_data_t *pdata =
		 container_of(work, struct scharger_v800_private_data_t, flash_irq_work);

	ret = regmap_read(pdata->regmap, CHG_IRQ_FLAG_REG, &irq_state);
	if (ret)
		scharger_inf("%s: return error\n", __func__);

	scharger_inf("%s :irq_state:0x%x\n", __func__, irq_state);
	if (irq_state & FLASH_IRQ) {
		/* FLASH IRQ:0x80E~0x80F */
		for (i = 0; i < FLASH_IRQ_BULK_NUM; i++) {
			regmap_read(pdata->regmap, CHG_IRQ_FLAG_12_REG + i, &flash_irq_state[i]);
			scharger_inf("%s:FLASH_IRQ, flash_irq_state[0x%x]:0x%x\n",
				__func__, CHG_IRQ_FLAG_12_REG + i, flash_irq_state[i]);
		}
		flash_fault_handle(flash_irq_state);
		for (i = 0; i < FLASH_IRQ_BULK_NUM; i++)
			regmap_write(pdata->regmap, CHG_IRQ_FLAG_12_REG + i, flash_irq_state[i]);
	}

	flash_desc = irq_to_desc(pdata->flash_irq);
	if (flash_desc == NULL) {
		scharger_err("[%s]irq_to_desc failed\n", __func__);
		return;
	}
	if (flash_desc->irq_data.chip->irq_unmask != NULL)
		flash_desc->irq_data.chip->irq_unmask(&flash_desc->irq_data);
}

static irqreturn_t scharger_flash_irq_handle(int irq, void *_pdata)
{
	struct scharger_v800_private_data_t *pdata = _pdata;
	struct irq_desc *flash_desc = NULL;

	flash_desc = irq_to_desc(pdata->flash_irq);
	if (flash_desc == NULL) {
		scharger_err("[%s]irq_to_desc failed\n", __func__);
		return IRQ_NONE;
	}

	if (flash_desc->irq_data.chip->irq_mask != NULL)
		flash_desc->irq_data.chip->irq_mask(&flash_desc->irq_data);

	(void)queue_work(system_power_efficient_wq, &pdata->flash_irq_work);
	return IRQ_HANDLED;
}

static void scharger_flash_irq_prc(struct scharger_v800_private_data_t *pdata)
{
	int i;
	u32 val = 0;

	for (i = 0; i < FLASH_IRQ_BULK_NUM; i++) {
		val = 0;
		regmap_read(pdata->regmap, CHG_IRQ_FLAG_12_REG + i, &val);
		regmap_write(pdata->regmap, CHG_IRQ_FLAG_12_REG + i, val);
		if(val)
			scharger_inf("%s 0x%x = 0x%x\n", __func__, CHG_IRQ_FLAG_12_REG + i, val);
	}
}

int scharger_v800_flash_irq_init(struct scharger_v800_private_data_t *pdata)
{
	int ret = 0;

	pdata->flash_irq = scharger_get_irq_byname(pdata->dev, "flash_irq");
	if (pdata->flash_irq < 0) {
		scharger_err("failed to get flash_irq %d\n", pdata->flash_irq);
		return -ENOENT;
	}
	scharger_inf("flash_irq: %d\n", pdata->flash_irq);
	scharger_flash_irq_prc(pdata);
	ret = request_irq(pdata->flash_irq, scharger_flash_irq_handle, IRQF_NO_SUSPEND, "flash_irq", pdata);
	if (ret != 0) {
		free_irq(pdata->flash_irq, pdata);
		scharger_err("request flash_irq failed\n");
		return ret;
	}

	return ret;
}
