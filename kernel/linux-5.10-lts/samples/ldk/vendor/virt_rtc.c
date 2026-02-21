/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: virt rtc device module
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun  5 16:23:10 2023
 */

#include <linux/rtc.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#include <liblinux/pal.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("virt rtc");
MODULE_VERSION("0.2");

static struct liblinux_pal_rtc_ops liblinux_pal_rtc_ops __read_mostly;

static int virt_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	int ret;
	unsigned long sec = 0;

	if (tm == NULL) {
		panic("virt-rtc: alarm is null\n");
	}

	ret = liblinux_pal_rtc_ops.read_time(&sec);
	if (ret == 0) {
		rtc_time64_to_tm(sec, tm);
	}

	return ret;
}

static int virt_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	unsigned long sec;

	if (tm == NULL) {
		panic("virt-rtc: alarm is null\n");
	}

	sec = (unsigned long)rtc_tm_to_time64(tm);
	return liblinux_pal_rtc_ops.set_time(sec);
}

static int virt_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	int ret;
	unsigned long sec = 0;
	unsigned int pending, enable;

	if (alarm == NULL) {
		panic("virt-rtc: alarm is null\n");
	}

	ret = liblinux_pal_rtc_ops.read_alarm(&sec, &pending, &enable);
	if (ret == 0) {
		alarm->pending = pending;
		alarm->enabled = enable;
		rtc_time64_to_tm(sec, &alarm->time);
	}

	return ret;
}

static int virt_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	unsigned long sec;

	if (alarm == NULL) {
		panic("virt-rtc: alarm is null\n");
	}

	sec = (unsigned long)rtc_tm_to_time64(&alarm->time);
	return liblinux_pal_rtc_ops.set_alarm(sec, alarm->enabled);
}

static int virt_rtc_alarm_irq_enable(struct device *dev,
	unsigned int enabled)
{
	int ret = 0;
	ret = liblinux_pal_rtc_ops.irq_enable(enabled);
	return ret;
}

static struct rtc_class_ops virt_rtc_ops __read_mostly = {
	.read_time = virt_rtc_read_time,
	.set_time = virt_rtc_set_time,
	.read_alarm = virt_rtc_read_alarm,
	.set_alarm = virt_rtc_set_alarm,
	.alarm_irq_enable = virt_rtc_alarm_irq_enable,
};

static int virt_rtc_init(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	return -ENOSYS;
#else
	int ret = 0;
	struct platform_device *pdev = NULL;
	struct rtc_device *rtc = NULL;

#ifdef CONFIG_RTC_DRV_PMIC_SPMI
	ret = liblinux_pal_register_rtc_ops(set_power_on_alarm);
	if (ret != 0) {
		return -EINVAL;
	}
#endif

	ret = liblinux_pal_rtc_ops_init(&liblinux_pal_rtc_ops);
	if (ret != 0) {
		return -EINVAL;
	}

	pdev = platform_device_alloc("virt-rtc", PLATFORM_DEVID_NONE);
	if (pdev == NULL) {
		return -ENOMEM;
	}

	ret = platform_device_add(pdev);
	if (ret != 0) {
		platform_device_put(pdev);
		return ret;
	}

	device_init_wakeup(&pdev->dev, true);
	rtc = devm_rtc_allocate_device(&pdev->dev);
	if (rtc == NULL) {
		platform_device_unregister(pdev);
		return -ENOMEM;
	}

	rtc->ops = &virt_rtc_ops;
	ret = rtc_register_device(rtc);
	if (ret != 0) {
		platform_device_unregister(pdev);
	}

	return ret;
#endif
}
module_init(virt_rtc_init);
