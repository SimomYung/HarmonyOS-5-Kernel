/*
 * hw_pd_core.h
 *
 * Header file of internal interfaces for huawei PD driver
 *
 * Copyright (c) 2019-2023 Huawei Technologies Co., Ltd.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef HW_PD_CORE_H
#define HW_PD_CORE_H

#include <linux/platform_device.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_state.h>

/* used to describe abnormal interrupt from pd(cc int or vbus int) */
enum abnomal_change_type {
	PD_DPM_ABNORMAL_CC_CHANGE,
	PD_DPM_UNATTACHED_VBUS_ONLY,
};

/* Register a new PD driver instance */
void hw_pd_register_instance(void *context);

/*
 * Open vbus power-supply for testing while usb is not connected.
 * Param sec : seconds later to do power-supply operation.
 */
int hw_pd_set_rtb_success(unsigned int sec);

/*
 * Notify pd driver that vbus enter low-power in otg mode.
 * This function should be called while usb-audio device is connected.
 */
void hw_pd_issue_vbus_low(void);

/* This function is used to abnormal cc detection init. */
void hw_pd_abnormal_cc_detection_init(void);
void hw_pd_abnomal_cc_change(enum abnomal_change_type event);

void hw_pd_force_report_port(void);

#ifdef CONFIG_HUAWEI_OHOS_DSM
/* This function is used to DMD information init. */
void pd_power_dsm_init(void);
#endif /* CONFIG_HUAWEI_OHOS_DSM */

#endif

