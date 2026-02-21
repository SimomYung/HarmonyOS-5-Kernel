/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition of RTC framework
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 10 21:10:55 2023
 */
#ifndef MAPI_HMKERNEL_RTC_H
#define MAPI_HMKERNEL_RTC_H

#include <hmkernel/module_proto.h>

/* The minimum second for the system to suspend based on the RTC timer */
#define MIN_SUSPEND_RTC_SECOND		2ULL
#define RTC_HANDLE_ALARM_TIMES_NR	3U

struct rtc_ops;
struct rtc_device_s;
struct rtc_timer_s;
struct __rtc_time_s;

enum rtc_timer_state {
	RTC_TIMER_STATE_FREE,
	RTC_TIMER_STATE_ONTREE,
};

extern struct rtc_device_s *acquire_curr_rtc(void);
extern int register_rtc(struct rtc_device_s *rdev);
extern int rtc_read_sec(u32 *sec);
extern int rtc_set_sec(u32 sec);
extern int rtc_read_alarm(struct __rtc_time_s *alrm);
extern int rtc_set_alarm(u32 sec, unsigned int enable);
extern int rtc_enable_alarm_irq(void);
extern int rtc_disable_alarm_irq(void);
extern u64 rtc_read_offset(void);
extern u64 rtc_read_lastts(void);

extern void rtc_timer_base_init(void);
extern unsigned long handle_rtc_timer(void);
extern void rtc_timer_init(struct rtc_timer_s *rtc_timer,
			   void (*callback)(struct rtc_timer_s *rtc_timer, u64 curr_time));
extern int rtc_timer_add(struct rtc_timer_s *rtc_timer, u64 expire);
extern void rtc_timer_remove(struct rtc_timer_s *rtc_timer);

#endif
