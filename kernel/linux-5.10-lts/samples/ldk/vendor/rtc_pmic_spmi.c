/*
 * rtc module, Provide rtc read and write interface for pmu&soc
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/version.h>
#include <linux/arm-smccc.h>
#include <linux/compiler.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/reboot.h>
#include <linux/syscalls.h>
#include <linux/time.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <linux/of.h>
#include <linux/rtc.h>
#include <linux/sizes.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <pmic_interface.h>
#include <linux/version.h>
#include <linux/of_irq.h>
#include <platform_include/basicplatform/linux/spmi_platform.h>
#include <platform_include/basicplatform/linux/of_platform_spmi.h>
#include <platform_include/see/bl31_smc.h>
#include <securec.h>

#ifdef RTC_SYNC_TIMER
#include "time_sync.h"
#endif

#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif
#ifdef CONFIG_RTC_SECURE_FEATURE
#include <platform_include/basicplatform/linux/nve/nve_ap_kernel_interface.h>
#define NVE_RTC_NUM 272
#define NVE_NV_RTC_OFFSET_SIZE 4
#endif
#define IRQ_ENABLE 0x1
#define IRQ_DISABLE 0x0
#define RTC_NVE_INIT 0x0
#define RTC_PMU_BULK_BYTE 8
#define RTC_PMU_BULK_INT_LEN 4
#define RTC_PMU_BULK_INIT 0xff
#define RTC_YEAR_LINUX_INIT 1900
#define RTC_MONTH_INIT 1
#define RTC_YEAR_VALID 75
#define PMU_RTC_SET_DELAY 200

#define  SHOW_TIME(time)   \
	pr_err("[%s]: %d-%d-%d %d:%d:%d\n", __func__,\
		(time)->tm_year + RTC_YEAR_LINUX_INIT,\
		(time)->tm_mon + RTC_MONTH_INIT,\
		(time)->tm_mday, (time)->tm_hour,\
		(time)->tm_min, (time)->tm_sec)

/* Register definitions */
#define PMU_RTC_DR 0x00 /* Data read register */
#define PMU_RTC_MR 0x04 /* Match register */
#define PMU_RTC_LR 0x08 /* Data load register */
#define PMU_RTC_CR 0x0c /* Control register */

#ifdef CONFIG_HUAWEI_DSM
#define DSM_RTC_BUF_SIZE 256
#define DSM_RTC_PMU_READCOUNT_ERROR_NUM 925005000
#define DSM_RTC_SET_RTC_TMIE_WARNING_NUM 925005001
#endif

struct vendor_rtc_dev {
	struct rtc_device *rtc_dev;
	int pmu_rtc_irq;
	unsigned int pmu_rtc_baseaddr;
	unsigned int pmu_rtc_imr; /* IRQ Mask register */
	unsigned int pmu_rtc_icr; /* IRQ & IRQ Clear register */
	unsigned int pmu_rtc_ib;  /* IRQ Bit in register */

	unsigned int pmu_rtc_xo_thrshd_pwroff_val;
	/* work to notify here's a rtc alram */
	struct work_struct rtc_alarm_work;
#ifdef CONFIG_RTC_SECURE_FEATURE
	struct work_struct rtc_nv_work; /* work to write offset to nv */
	struct workqueue_struct *nv_work_queue;
	int offset; /* offset of PMU RTC and SOC RTC */
#endif
};

#ifdef CONFIG_HUAWEI_DSM
struct dsm_client *get_rtc_dsm_client(void);
static unsigned long last_time;

#define rtc_dsm_report(err_no, fmt, args...)\
do {\
	if (!get_rtc_dsm_client()) {\
		pr_err("[RTC DSM]rtc dsm client is null\n");\
	} else if (!dsm_client_ocuppy(get_rtc_dsm_client())) {\
		dsm_client_record(get_rtc_dsm_client(), (fmt), ##args);\
		dsm_client_notify(get_rtc_dsm_client(), (err_no));\
		pr_err("[RTC DSM]rtc dsm report err = %d\n", (err_no));\
	} else {\
		pr_err("[RTC DSM]rtc dsm is busy err = %d\n", (err_no));\
	} \
} while (0)

struct dsm_dev dsm_rtc = {
	.name = "dsm_rtc",
	.device_name = "rtc",
	.fops = NULL,
	.buff_size = DSM_RTC_BUF_SIZE,
};

static struct dsm_client *rtc_dsm_client;

struct dsm_client *get_rtc_dsm_client(void)
{
	return rtc_dsm_client;
}

static void rtc_register_dsm_client(void)
{
	if (rtc_dsm_client)
		return;

	rtc_dsm_client = dsm_register_client(&dsm_rtc);
	if (!rtc_dsm_client)
		pr_err("[RTC DSM]Rtc dsm register failed\n");
}

static int dsm_rtc_valid_tm(struct rtc_time *tm)
{
	if (tm->tm_year < RTC_YEAR_VALID)
		return -EINVAL;

	return 0;
}
#endif

#ifdef CONFIG_RTC_SECURE_FEATURE
enum pmu_rtc_ops {
	PMU_RTC_ENABLE = 0,
	PMU_RTC_XO_THRESHOLD_SET,
};

#define RTC_REGISTER_FN_ID RTC_FID_VALUE
noinline int atfd_service_rtc_smc(
	u64 _function_id, u64 _arg0, u64 _arg1, u64 _arg2)
{
	struct arm_smccc_res res = {0};

	arm_smccc_smc(_function_id, _arg0, _arg1, _arg2, 0, 0, 0, 0, &res);
	return (int)res.a0;
}
#endif

/* extern from kernel source code to avoid intrusion */
#if defined(CONFIG_PRODUCT_ARMPC) || defined(CONFIG_RTC_DISABLE_POWEROFF_ALARM)
struct rtc_wkalrm poweroff_rtc_alarm = { 0, 0, {0} };
#else
extern struct rtc_wkalrm poweroff_rtc_alarm;
#endif
static struct vendor_rtc_dev *g_ldata;

static unsigned int get_pd_charge_flag(void);
static unsigned int pd_charge_flag;

/*
 * parse powerdown charge cmdline which is passed from fastoot
 * Format : pd_charge=0 or 1
 */
static int __init early_parse_pdcharge_cmdline(char *p)
{
	if (p) {
		if (!strncmp(p, "charger", strlen("charger")))
			pd_charge_flag = 1;
		else
			pd_charge_flag = 0;

		pr_err("power down charge p:%s, pd_charge_flag :%u\n", p,
			pd_charge_flag);
	}

	return 0;
}

early_param("ohos.boot.mode", early_parse_pdcharge_cmdline);

static unsigned int get_pd_charge_flag(void)
{
	return pd_charge_flag;
}

static inline struct vendor_rtc_dev *rtc_ldata_get(void)
{
	return (struct vendor_rtc_dev *)g_ldata;
}

#ifdef CONFIG_RTC_SECURE_FEATURE
static int rtc_nve_write(unsigned int index,
	const int *data, unsigned int len)
{
	int ret;
	struct opt_nve_info_user pinfo;

	pinfo.nv_operation = NV_READ;
	pinfo.nv_number = NVE_RTC_NUM;
	pinfo.valid_size = NVE_NV_RTC_OFFSET_SIZE;

	/*
	 * The dest buffer size is always greater than
	 * the length of the character set to the dest buffer
	 */
	ret = memset_s(pinfo.nv_data, NVE_NV_DATA_SIZE, IRQ_DISABLE,
		(unsigned long)pinfo.valid_size);
	if (ret) {
		pr_err("write rtc nve memset failed!\n");
		return -1;
	}

	ret = nve_direct_access_interface(&pinfo);
	if (ret) {
		pr_err("rtc Read nve failed!\n");
		return -1;
	}

	pinfo.nv_operation = NV_WRITE;
	ret = memcpy_s(pinfo.nv_data + index, NVE_NV_DATA_SIZE - index,
		data, (unsigned long)len);
	if (ret) {
		pr_err("rtc nve memcpy failed!\n");
		return -1;
	}
	ret = nve_direct_access_interface(&pinfo);
	if (ret) {
		pr_err("rtc nve write failed!\n");
		return -1;
	}

	return 0;
}

static int rtc_nve_read(unsigned int index, int *buf,
	unsigned long buffsize, unsigned int len)
{
	int ret;
	struct opt_nve_info_user pinfo;

	pinfo.nv_operation = NV_READ;
	pinfo.nv_number = NVE_RTC_NUM;
	pinfo.valid_size = NVE_NV_RTC_OFFSET_SIZE;

	/*
	 * The dest buffer size is always greater than
	 * the length of the character set to the dest buffer
	 */
	ret = memset_s(pinfo.nv_data, NVE_NV_DATA_SIZE, IRQ_DISABLE,
		(unsigned long)pinfo.valid_size);
	if (ret) {
		pr_err("read rtc nve memset failed!\n");
		return -1;
	}

	ret = nve_direct_access_interface(&pinfo);
	if (ret) {
		pr_err("rtc nve read failed!\n");
		return -1;
	}

	ret = memcpy_s(buf, buffsize, pinfo.nv_data + index,
		(unsigned long)len);
	if (ret) {
		pr_err("rtc memcpy nve failed!\n");
		return -1;
	}

	return 0;
}

static void rtc_nv_write_work(struct work_struct *work)
{
	int ret;
	struct vendor_rtc_dev *s_ldata = rtc_ldata_get();

	ret = rtc_nve_write(0, &s_ldata->offset, NVE_NV_RTC_OFFSET_SIZE);
	if (ret)
		pr_err("[%s]write offset to nv failed!\n", __func__);
}

static void pmu_rtc_irq_enable(unsigned int enable)
{
	struct irq_desc *desc = NULL;
	struct vendor_rtc_dev *ldata = NULL;

	ldata = rtc_ldata_get();
	if (!ldata) {
		pr_err("[%s]get rtc ldata failed\n", __func__);
		return;
	}

	desc = irq_to_desc(ldata->pmu_rtc_irq);
	if (!desc) {
		pr_err("%s:desc is NULL!\n", __func__);
		return;
	}

	if (enable == IRQ_ENABLE) {
		if (desc->irq_data.chip->irq_unmask)
			desc->irq_data.chip->irq_unmask(&desc->irq_data);
	} else {
		if (desc->irq_data.chip->irq_mask)
			desc->irq_data.chip->irq_mask(&desc->irq_data);
	}
}
#endif

static void pmu_rtc_write_bulk(unsigned int base, unsigned long data)
{
	unsigned int value, i;

	for (i = 0; i < RTC_PMU_BULK_INT_LEN; i++) {
		value = (data >> (i * RTC_PMU_BULK_BYTE)) & RTC_PMU_BULK_INIT;
		pmic_write_reg((base + i), value);
	}
}

static unsigned long pmu_rtc_read_bulk(unsigned int base)
{
	unsigned long data = 0;
	unsigned int value, i;

	for (i = 0; i < RTC_PMU_BULK_INT_LEN; i++) {
		value = pmic_read_reg(base + i);
		data |= (value & RTC_PMU_BULK_INIT) << (i * RTC_PMU_BULK_BYTE);
	}

	return data;
}

void pmu_rtc_readtime(struct rtc_time *tm)
{
	unsigned long time;
	struct vendor_rtc_dev *ldata = NULL;
#ifdef CONFIG_RTC_SECURE_FEATURE
	int ret;
	int offset;
#endif
	if (!tm)
		return;

	ldata = rtc_ldata_get();
	if (!ldata) {
		pr_err("[%s]get rtc ldata failed\n", __func__);
		return;
	}

	time = pmu_rtc_read_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_DR);

#ifdef CONFIG_RTC_SECURE_FEATURE
	ret = rtc_nve_read(0, &offset, (unsigned long)sizeof(int),
		NVE_NV_RTC_OFFSET_SIZE);
	if (ret)
		pr_err("[%s]read offset from nv failed\n", __func__);
	time += offset;
#endif

	rtc_time64_to_tm(time, tm);
}

unsigned long pmu_rtc_readcount(void)
{
#ifdef CONFIG_HUAWEI_DSM
	unsigned long time;
#endif
	struct vendor_rtc_dev *data = rtc_ldata_get();

	if (!data) {
		pr_err("[%s]data is NULL!\n", __func__);
		return 0;
	}
#ifdef CONFIG_HUAWEI_DSM
	time = pmu_rtc_read_bulk(data->pmu_rtc_baseaddr + PMU_RTC_DR);
	if (last_time > time) {
		/* DMD RTC PRINT INFO */
		rtc_dsm_report(DSM_RTC_PMU_READCOUNT_ERROR_NUM,
			"Pmu rtc readcount error, time : %lu, lasttime : %lu\n",
			time, last_time);
	}
	last_time = time;

	return time;
#else
	return pmu_rtc_read_bulk(data->pmu_rtc_baseaddr + PMU_RTC_DR);
#endif
}

void pmu_rtc_setalarmtime(unsigned long time)
{
#ifdef CONFIG_RTC_SECURE_FEATURE
	struct vendor_rtc_dev *ldata = NULL;
	struct rtc_time tm;
	int ret;
	int offset;

	ret = rtc_nve_read(0, &offset, (unsigned long)sizeof(int),
		NVE_NV_RTC_OFFSET_SIZE);
	if (ret)
		pr_err("[%s]read offset from nv failed\n", __func__);

	ldata = rtc_ldata_get();
	if (!ldata) {
		pr_err("[%s]get rtc ldata failed\n", __func__);
		return;
	}

	pr_err("[%s]time %lu, offset %d\n", __func__, time, offset);
	if (time > offset) {
		pmu_rtc_write_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_MR, time - offset);
	} else {
		pmu_rtc_write_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_MR, 0);
	}

	atfd_service_rtc_smc(RTC_REGISTER_FN_ID, PMU_RTC_XO_THRESHOLD_SET,
			     ldata->pmu_rtc_xo_thrshd_pwroff_val, 0);
	pmu_rtc_irq_enable(IRQ_ENABLE);

	rtc_time64_to_tm(time, &tm);
	SHOW_TIME(&tm);
#endif
}

#ifndef CONFIG_PMU_RTC_DISABLED_SET
int pmu_rtc_settime(struct rtc_time *tm)
{
	int err;
	unsigned long time;
	struct vendor_rtc_dev *ldata = NULL;

	if (!tm)
		return -1;

	SHOW_TIME(tm);
	err = rtc_valid_tm(tm);
	if (err) {
		pr_err("[%s]Time is invalid\n", __func__);
		return err;
	}
	time = rtc_tm_to_time64(tm);

	ldata = rtc_ldata_get();
	if (!ldata) {
		pr_err("[%s]get rtc ldata failed\n", __func__);
		return -1;
	}

	pmu_rtc_write_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_LR, time);

	return 0;
}
#endif
#ifdef CONFIG_RTC_SECURE_FEATURE
static int pmu_rtc_synctime_by_nve(time64_t rtc_lastts)
{
	struct vendor_rtc_dev *ldata = rtc_ldata_get();
	if (!ldata) {
		pr_err("[%s]get rtc ldata failed\n", __func__);
		return -1;
	}

	unsigned long pmu_curr_time = pmu_rtc_read_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_DR);
	long rtc_pmu_offset = (time64_t)rtc_lastts - (time64_t)pmu_curr_time;
	ldata->offset = rtc_pmu_offset;
	queue_work(ldata->nv_work_queue, &ldata->rtc_nv_work);

	/* Post log to avoid long-time logging that might affect the result. Although, the syncing
	 * should be finished in 1 second.
	 */
	struct rtc_time pmu_curr_tm;
	struct rtc_time rtc_lastts_tm;
	rtc_time64_to_tm(pmu_curr_time, &pmu_curr_tm);
	rtc_time64_to_tm(rtc_lastts, &rtc_lastts_tm);
	SHOW_TIME(&pmu_curr_tm);
	pr_info("[%s] rtc_lastts=%ld rtc_pmu_offset=%ld", __func__, (long)rtc_lastts, (long)rtc_pmu_offset);
	SHOW_TIME(&rtc_lastts_tm);
	return 0;
}
#endif

static int pmu_rtc_synctime_by_pmu(time64_t rtc_offset)
{
	struct vendor_rtc_dev *ldata = rtc_ldata_get();
	if (!ldata) {
		pr_err("[%s]get rtc ldata failed\n", __func__);
		return -1;
	}

	unsigned long pmu_curr_time = pmu_rtc_read_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_DR);
	unsigned long pmu_sync_time = (unsigned long)((time64_t)pmu_curr_time + (time64_t)rtc_offset);
#ifndef CONFIG_PMU_RTC_DISABLED_SET
	pmu_rtc_write_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_LR, pmu_sync_time);
#endif
	/* Post log to avoid long-time logging that might affect the result. Although, the syncing
	 * should be finished in 1 second.
	 */
	struct rtc_time pmu_curr_tm;
	struct rtc_time pmu_sync_tm;
	rtc_time64_to_tm(pmu_curr_time, &pmu_curr_tm);
	rtc_time64_to_tm(pmu_sync_time, &pmu_sync_tm);
	SHOW_TIME(&pmu_curr_tm);
	pr_info("[%s] rtc_offset=%ld", __func__, (long)rtc_offset);
	SHOW_TIME(&pmu_sync_tm);
	return 0;
}

int pmu_rtc_synctime(time64_t rtc_lastts, time64_t rtc_offset)
{
	int err = 0;
#ifdef CONFIG_RTC_SECURE_FEATURE
	int ret;
	int offset = 0;
	ret = rtc_nve_read(0, &offset, (unsigned long)sizeof(int),
		NVE_NV_RTC_OFFSET_SIZE);
	pr_info("[%s]read offset from nv: %d offset=%d\n", __func__, ret, offset);
	if (ret == 0) {
		err = pmu_rtc_synctime_by_nve(rtc_lastts);
	} else {
		err = pmu_rtc_synctime_by_pmu(rtc_offset);
	}
#else
	err = pmu_rtc_synctime_by_pmu(rtc_offset);
#endif
	return err;
}

unsigned long pmu_rtc_readalarmtime(void)
{
	unsigned long time;
	struct vendor_rtc_dev *ldata = NULL;
#ifdef CONFIG_RTC_SECURE_FEATURE
	int ret;
	int offset;
#endif
	ldata = rtc_ldata_get();
	if (!ldata) {
		pr_err("[%s]get ldata failed\n", __func__);
		return 0;
	}
	time = pmu_rtc_read_bulk(ldata->pmu_rtc_baseaddr + PMU_RTC_MR);

#ifdef CONFIG_RTC_SECURE_FEATURE
	ret = rtc_nve_read(0, &offset, (unsigned long)sizeof(int),
		NVE_NV_RTC_OFFSET_SIZE);
	if (ret)
		pr_err("[%s]read offset from nv failed\n", __func__);
	time += offset;
#endif
	return time;
}

static void rtc_alarm_notify_work(struct work_struct *work)
{
	struct vendor_rtc_dev *ldata =
		container_of(work, struct vendor_rtc_dev, rtc_alarm_work);
	pr_err("Entering rtc alarm notify work!\n");

	if (ldata->rtc_dev == NULL) {
		ldata->rtc_dev = rtc_class_open("rtc0");
		if (ldata->rtc_dev == NULL) {
			pr_err("[%s] open rtc class device failed\n", __func__);
		}
	}

	if ((ldata) && (ldata->rtc_dev) && (&(ldata->rtc_dev->dev))) {
		kobject_uevent(&(ldata->rtc_dev->dev.kobj), KOBJ_CHANGE);
	}
}

static irqreturn_t pmu_rtc_interrupt(int irq, void *dev_id)
{
	struct vendor_rtc_dev *ldata = dev_id;

	pr_err("[%s]RTC Interrupt Comes\n", __func__);

#ifdef CONFIG_PMIC_21BV100_PMU
	pmic_write_reg(ldata->pmu_rtc_icr, 0x1);
#endif

	if (unlikely(get_pd_charge_flag())) {
		schedule_work(&g_ldata->rtc_alarm_work);
		return IRQ_HANDLED;
	}

	return IRQ_HANDLED;
}

static void rtc_shutdown(struct spmi_device *pdev)
{
	unsigned long time;
	struct vendor_rtc_dev *ldata = NULL;

	ldata = dev_get_drvdata(&pdev->dev);
	if (!ldata)
		return;

	if (unlikely(get_pd_charge_flag()))
		return;

	if (!poweroff_rtc_alarm.enabled) {
		pmu_rtc_write_bulk(
			ldata->pmu_rtc_baseaddr + PMU_RTC_MR, 0);
	}
}

static int _rtc_remove(struct spmi_device *pdev)
{
	struct vendor_rtc_dev *ldata = NULL;
#ifdef CONFIG_RTC_SECURE_FEATURE
	struct vendor_rtc_dev *s_ldata = NULL;
#endif

	ldata = dev_get_drvdata(&pdev->dev);
	if (!ldata)
		return -1;

#ifdef CONFIG_RTC_SECURE_FEATURE
	s_ldata = rtc_ldata_get();
	if (!s_ldata)
		return -1;

	cancel_work_sync(&s_ldata->rtc_nv_work);
	flush_workqueue(s_ldata->nv_work_queue);
	destroy_workqueue(s_ldata->nv_work_queue);
#endif

	dev_set_drvdata(&pdev->dev, NULL);

	return 0;
}

static void rtc_remove(struct spmi_device *pdev)
{
	(void)_rtc_remove(pdev);
}

static int get_rtc_irq(struct vendor_rtc_dev *prtc,
		       struct spmi_device *pdev)
{
	int ret = 0;

	prtc->pmu_rtc_irq = spmi_get_irq_byname(pdev, NULL, "pmic-rtc");
	if (prtc->pmu_rtc_irq <= 0) {
		pr_err("[%s]get pmu_rtc_irq err\n", __func__);
		ret = -ENOENT;
	}
	return ret;
}

static int rtc_get_property(
	struct device_node *np, struct vendor_rtc_dev *prtc)
{
	int ret;

	ret = of_property_read_u32(np, "pmic-rtc-base",
		&prtc->pmu_rtc_baseaddr);
	if (ret) {
		pr_err("[%s]Get pmic-rtc-base failed\n",
			__func__);
		return -ENODEV;
	}

	ret = of_property_read_u32(
		np, "pmic-rtc-imr", &prtc->pmu_rtc_imr);
	if (ret) {
		pr_err("[%s]Get pmic-rtc-imr failed\n",
			__func__);
		return -ENODEV;
	}

	ret = of_property_read_u32(np, "pmic-rtc-icr",
		&prtc->pmu_rtc_icr);
	if (ret) {
		pr_err("[%s]Get pmic-rtc-icr failed\n",
			__func__);
		return -ENODEV;
	}

	ret = of_property_read_u32(np, "pmic-rtc-ib",
		&prtc->pmu_rtc_ib);
	if (ret) {
		pr_err("[%s]Get pmic-rtc-ib failed\n",
			__func__);
		return -ENODEV;
	}

	ret = of_property_read_u32(np,
		"pmic-rtc-xo-thrshd-pwroff-val",
		&prtc->pmu_rtc_xo_thrshd_pwroff_val);
	if (ret) {
		pr_err(
		"[%s]Get pmic-rtc-xo-thrshd-pwroff-val failed\n",
			__func__);
		return -ENODEV;
	}

	return 0;
}

static int rtc_init_cfg(struct vendor_rtc_dev *prtc,
	struct device *dev, struct spmi_device *pdev)
{
	int ret;
	struct device_node *np = dev->of_node;

	ret = get_rtc_irq(prtc, pdev);
	if (ret) {
		pr_err("[%s]get pmu soc rtc irq failed\n", __func__);
		return ret;
	}

	ret = rtc_get_property(np, prtc);
	if (ret) {
		pr_err("[%s]rtc_get_property error\n", __func__);
		return ret;
	}

	return 0;
}

static int request_rtc_irq(struct device *dev, struct vendor_rtc_dev *prtc)
{
	int ret = 0;

	if (devm_request_irq(dev, prtc->pmu_rtc_irq, pmu_rtc_interrupt,
		IRQF_NO_SUSPEND, "vendor-pmu-rtc", prtc)) {
		pr_err("[%s]request_pmu_rtc_irq error\n", __func__);
		ret = -EIO;
	}

	return ret;
}

static void rtc_enable(struct vendor_rtc_dev *prtc)
{
#ifndef CONFIG_RTC_SECURE_FEATURE
	unsigned long time;
	unsigned int reg_value;
#endif

#ifdef CONFIG_RTC_SECURE_FEATURE
	g_ldata->nv_work_queue = create_workqueue("rtc_nv_work");
	INIT_WORK(&g_ldata->rtc_nv_work, rtc_nv_write_work);
	atfd_service_rtc_smc(RTC_REGISTER_FN_ID, PMU_RTC_ENABLE, 0, 0);
#else
	reg_value = pmic_read_reg(prtc->pmu_rtc_baseaddr + PMU_RTC_CR);
	if (!(reg_value & IRQ_ENABLE)) {
		reg_value |= IRQ_ENABLE;
		pmic_write_reg(
			prtc->pmu_rtc_baseaddr + PMU_RTC_CR, reg_value);
		mdelay(PMU_RTC_SET_DELAY);
	}
#endif
}

static int rtc_probe(struct spmi_device *pdev)
{
	int ret;
	struct vendor_rtc_dev *prtc = NULL;
	struct device *dev = &pdev->dev;

	pr_err("PMU RTC Init Start----------------\n");

	prtc = devm_kzalloc(dev, sizeof(struct vendor_rtc_dev), GFP_KERNEL);
	if (!prtc)
		return -ENOMEM;

	g_ldata = prtc;
	dev_set_drvdata(dev, prtc);

	ret = rtc_init_cfg(prtc, dev, pdev);
	if (ret) {
		pr_err("[%s]rtc init failed\n", __func__);
		goto init_cfg_err;
	}

	device_init_wakeup(&pdev->dev, 1);

	ret = request_rtc_irq(dev, prtc);
	if (ret) {
		pr_err("[%s]request rtc irq failed\n", __func__);
		goto out_no_rtc;
	}

/* enable pmu rtc */
	rtc_enable(prtc);

	INIT_WORK(&g_ldata->rtc_alarm_work, rtc_alarm_notify_work);

#ifdef CONFIG_HUAWEI_DSM
	rtc_register_dsm_client();
#endif

	pr_err("PMU RTC Init End----------------\n");

	return 0;

out_no_rtc:
init_cfg_err:
	dev_set_drvdata(dev, NULL);
	return ret;
}

#define RTC_PM NULL

static const struct of_device_id rtc_of_match[] = {
	{ .compatible = "rtc-spmi" },
	{ }
};

MODULE_DEVICE_TABLE(of, rtc_of_match);

static const struct spmi_device_id rtc_spmi_id[] = {
	{ "rtc-spmi", 0 },
	{ }
};

static struct spmi_driver rtc_driver = {
	.probe = rtc_probe,
	.remove = rtc_remove,
	.shutdown = rtc_shutdown,
	.driver = {
		.name = "rtc",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(rtc_of_match),
		.pm = RTC_PM,
	},
	.id_table = rtc_spmi_id,
};

static int __init rtc_init(void)
{
	return spmi_driver_register(&rtc_driver);
}

static void __exit rtc_exit(void)
{
	spmi_driver_unregister(&rtc_driver);
}

module_init(rtc_init);
module_exit(rtc_exit);

MODULE_LICENSE("GPL");
