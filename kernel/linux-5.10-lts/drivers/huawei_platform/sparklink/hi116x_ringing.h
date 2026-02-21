/*
 * Huawei hi116x ringing
 *
* interface for huawei hi116x_ringing driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef _HI116X_RINGING_H_
#define _HI116X_RINGING_H_
#include "hi116x_driver.h"

#define HI116X_RINGING_REG_LEN                6
#define HI116X_RINGING_REPORT_REG             0x0062
#define HI116X_RINGING_ACK_REG                0x001b

#define HI116X_RINGING_MSG_MAX_LEN            8
#define HI116X_RINGING_ACK_TYPE_LOC           0
#define HI116X_RINGING_ACK_LEN_LOC            1
#define HI116X_RINGING_ACK_VAL_LOC            5
#define HI116X_RINGING_REPORT_FLAG            0x01
#define HI116X_RINGING_ACK_FLAG               0x02
#define HI116X_RINGING_DATA_LEN               0x04
#define HI116X_RINGING_OPEN_FLAG              0x01
#define HI116X_RINGING_CLOSE_FLAG             0x02
#define HI116X_RINGING_END_FLAG               0x03
#define HI116X_RINGING_ACK_VAL_MAX            3

enum ringing_ack_value {
	RINGING_OPEN = 1,
	RINGING_CLOSE,
	RINGING_END,
};


int hi116x_report_ringing_cmd(struct hi1162_device_info *di);

#endif /* _HI116X_RINGING_H_ */