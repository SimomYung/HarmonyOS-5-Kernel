/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DATAN_CT_TIME_H
#define HVGR_DATAN_CT_TIME_H

#include <linux/types.h>

/* The ids of ioctl commands. */
#define HVGR_DATAN_CT_TIME 0xBEu
#define HVGR_DATAN_CT_TIME_SET_INTERVAL    _IOW(HVGR_DATAN_CT_TIME, 0x30u, u32)
#endif /* HVGR_DATAN_CT_TIME_H */
