/*
 * Copyright (C) 2021 Hisilicon
 * Author: Hisillicon <>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _LOCAL_USB_TYPEC_H_
#define _LOCAL_USB_TYPEC_H_

#include <securec.h>

#define APDO_MAX_NUM 7

enum usb_cable_event {
	USB_CABLE_DP_IN,
	USB_CABLE_DP_OUT,
};

struct hisi_usb_apdo_info {
	int num;
	int min_volt[APDO_MAX_NUM];
	int max_volt[APDO_MAX_NUM];
	int cur[APDO_MAX_NUM];
};

#endif
