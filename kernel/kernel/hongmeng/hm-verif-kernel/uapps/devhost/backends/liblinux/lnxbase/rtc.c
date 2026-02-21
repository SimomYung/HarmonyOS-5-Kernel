/*
* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
* Description: lnxbase interface for virt rtc device module
* Author: Huawei OS Kernel Lab
* Create: Thu Jun  1 19:54:30 2023
*/

#include <vfs.h>
#include <errno.h>
#include <devhost/log.h>
#include <devhost/device.h>
#include <liblinux/pal.h>
#include <lnxbase/lnxbase.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmsrv_sys/hm_timer.h>

#define ALARM_UTIMEMGR_NAME	"utimermgr-a"

int lnxbase_rtc_read_pid_and_name(int *pid_rp, char *name_rp, int name_size)
{
	int ret;
	struct __rtc_time_s time = {0};

	if (pid_rp == NULL || name_rp == NULL) {
		return -EINVAL;
	}

	ret = sysfast_rtc_time_ctrl(&time, __RTC_READ_PID_AND_NAME);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	if (strncmp(time.name, ALARM_UTIMEMGR_NAME, name_size) == 0) {
		ret = vfs_timerfd_next_alarm_pid_and_name(pid_rp, name_rp, name_size);
	} else {
		*pid_rp = time.pid;
		(void)strncpy_s(name_rp, name_size, time.name, name_size - 1);
	}

	return ret;
}

static int lnxbase_rtc_op_read_time(unsigned long *sec)
{
	int ret;
	struct __rtc_time_s time = { 0 };

	if (sec == NULL) {
		return -EINVAL;
	}

	ret = sysfast_rtc_time_ctrl(&time, __RTC_READ_TIME);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	*sec = time.sec;
	return 0;
}

static int lnxbase_rtc_op_set_time(unsigned long sec)
{
	int ret;
	struct __rtc_time_s time = { 0 };

	time.sec = sec;
	ret = sysfast_rtc_time_ctrl(&time, __RTC_SET_TIME);
	return ret == E_HM_OK ? 0 : -hmerrno2posix(ret);
}

static int lnxbase_rtc_op_read_alarm(unsigned long *sec, unsigned int *pending,
				unsigned int *enable)
{
	int ret;
	struct __rtc_time_s time = { 0 };

	if (sec == NULL || pending == NULL || enable == NULL) {
		return -EINVAL;
	}

	ret = sysfast_rtc_time_ctrl(&time, __RTC_READ_ALARM);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	*sec = time.sec;
	*pending = time.pending;
	*enable = time.enable;

	return 0;
}

static int lnxbase_rtc_op_set_alarm(unsigned long sec, unsigned int enable)
{
	int ret;
	struct __rtc_time_s time = { 0 };

	time.sec = sec;
	time.enable = enable;
	ret = sysfast_rtc_time_ctrl(&time, __RTC_SET_ALARM);
	return ret == E_HM_OK ? 0 : -hmerrno2posix(ret);
}

static int lnxbase_rtc_op_irq_enable(unsigned int enable)
{
	int ret;
	struct __rtc_time_s time = { 0 };

	time.enable = enable;
	ret = sysfast_rtc_time_ctrl(&time, __RTC_SET_IRQ);
	return ret == E_HM_OK ? 0 : -hmerrno2posix(ret);
}

int liblinux_pal_rtc_ops_init(struct liblinux_pal_rtc_ops *ops)
{
	int ret;
	unsigned long sec = 0;
	if (ops == NULL) {
		return -EINVAL;
	}

	/* test whether the rtc driver exists */
	ret = lnxbase_rtc_op_read_time(&sec);
	if (ret != 0) {
		dh_debug("lnxbase rtc device may not exist\n");
		return -EINVAL;
	}

	ops->read_time = lnxbase_rtc_op_read_time;
	ops->set_time = lnxbase_rtc_op_set_time;
	ops->read_alarm = lnxbase_rtc_op_read_alarm;
	ops->set_alarm = lnxbase_rtc_op_set_alarm;
	ops->irq_enable = lnxbase_rtc_op_irq_enable;
	return 0;
}

bool lnxbase_hmkernel_is_support_rtc(void)
{
	int ret;
	unsigned long sec = 0;

	/* test whether the rtc driver exists */
	ret = lnxbase_rtc_op_read_time(&sec);
	if (ret == 0) {
		return true;
	} else {
		return false;
	}
}

int liblinux_pal_register_rtc_ops(set_power_on_alarm_t set_power_on_alarm)
{
	return devhost_register_rtc_ops(set_power_on_alarm);
}
