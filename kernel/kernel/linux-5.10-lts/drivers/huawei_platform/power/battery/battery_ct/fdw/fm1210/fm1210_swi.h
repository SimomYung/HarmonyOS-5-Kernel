/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * fm1210_swi.h
 *
 * fm1210_swi driver head file for single line communication
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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
#ifndef FM1210_SWI_H
#define FM1210_SWI_H

#include "fm1210.h"
#include "../../batt_aut_checker.h"

#define HIGH            1
#define LOW             0
#define BIT_OF_BYTE     8
#define BIT_OFFSET      7

uint8_t fm1210_ow_dev_reset(struct fm1210_dev *di);
void fm1210_write_byte_8m(struct fm1210_dev *di, uint8_t val);
uint8_t fm1210_read_byte_8m(struct fm1210_dev *di);
#endif