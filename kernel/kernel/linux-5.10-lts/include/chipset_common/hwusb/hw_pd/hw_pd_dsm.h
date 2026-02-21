/*
 * hw_pd_dsm.h
 *
 * Header file of internal interfaces for huawei PD dsm driver
 *
 * Copyright (c) 2019-2023 Huawei Technologies Co., Ltd.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __HW_PD_DSM_H__
#define __HW_PD_DSM_H__

#define DSM_USB_PORT_ELECTRIFIED        920002024
#define TIME_T_MAX                      (time64_t)((1UL << ((sizeof(time64_t) << 3) - 1)) - 1)

void hw_pd_dsm_vbus_only(void);
bool hw_pd_time_interval(int time_interval, struct timespec64 *last_time);

#endif
