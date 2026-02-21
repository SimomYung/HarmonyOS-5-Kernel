/*
 * hw_pd_reverse_sc.h
 *
 * Header file of reverse supercharge of PD driver
 *
 * Copyright (C) 2024 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _HW_PD_REVERSE_SC_H_
#define _HW_PD_REVERSE_SC_H_

#include <linux/workqueue.h>

#define REVERSE_SC_ERR_VID              (-1)
#define REVERSE_SC_HUAWEI_DEV_VID       0x12d1
#define REVERSE_SC_HUAWEI_QCOM_DEV_PID  0x107d
#define REVERSE_SC_HUAWEI_HISI_DEV_PID  0x107e
#define REVERSE_SC_DETECTED_TIMES       20
#define REVERSE_SC_BC12_DETECTED        BIT(0)
#define REVERSE_SC_PD_DEV_DETECTED      BIT(1)
#define REVERSE_SC_PLUG_IN              BIT(2)
#define REVERSE_SC_MODE_SUPER           2
#define REVERSE_SC_DETECTED_TIME        50

struct hw_pd_reverse_sc_info {
	/* reverse sc charge */
	int reverse_sc_mode;
	unsigned int reverse_sc_detect_result;
	bool reverse_sc_exit;
	struct delayed_work reverse_sc_attach_work;
	u16 reverse_sc_dev_vid;
	u16 reverse_sc_dev_pid;
};

void hw_pd_reverse_sc_state_clear(void);
void hw_pd_reverse_sc_init(void);
ssize_t reverse_sc_mode_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count);
ssize_t reverse_sc_mode_show(struct device *dev,
	struct device_attribute *attr, char *buf);
int reverse_sc_device_detect(void);

#endif /* __HW_PD_REVERSE_SC_H_ */