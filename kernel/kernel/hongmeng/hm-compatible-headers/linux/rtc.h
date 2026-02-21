/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/rtc.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 19:11:14 2021
 */

#ifndef LINUX_RTC_H
#define LINUX_RTC_H

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

struct rtc_wkalrm {
	unsigned char enabled;
	unsigned char pending;
	struct rtc_time time;
};

#define RTC_UIE_ON	_IO('p', 0x03)
#define RTC_UIE_OFF	_IO('p', 0x04)

#define RTC_RD_TIME	_IOR('p', 0x09, struct rtc_time)
#define RTC_SET_TIME	_IOW('p', 0x0a, struct rtc_time)

#define RTC_EPOCH_READ	_IOR('p', 0x0d, unsigned long)
#define RTC_EPOCH_SET	_IOW('p', 0x0e, unsigned long)

#define RTC_WKALM_SET	_IOW('p', 0x0f, struct rtc_wkalrm)
#define RTC_WKALM_RD	_IOR('p', 0x10, struct rtc_wkalrm)

#define RTC_AF 0x20

#endif
