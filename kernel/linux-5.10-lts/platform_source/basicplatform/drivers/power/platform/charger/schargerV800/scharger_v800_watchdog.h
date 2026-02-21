/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * scharger_v800_watchdog.h
 *
 * SchargerV800 watchdog header.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __SCHARGER_V800_WATCHDOG_H__
#define __SCHARGER_V800_WATCHDOG_H__

#define WATCHDOG_TIMER_01_S            1
#define WATCHDOG_TIMER_02_S            2
#define WATCHDOG_TIMER_05_S            5
#define WATCHDOG_TIMER_10_S            10
#define WATCHDOG_TIMER_40_S            40
#define WATCHDOG_TIMER_80_S            80
#define WATCHDOG_TIMER_120_S           120

int scharger_v800_set_watchdog_timer(int value);
int scharger_v800_reset_watchdog_timer(void);
int scharger_v800_set_watchdog_timer_ms(int ms);
#endif