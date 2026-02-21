/* SPDX-License-Identifier: GPL-2.0 */
/*
 * wireless_rx_monitor.h
 *
 * monitor info for wireless charging
 *
 * Copyright (c) 2021-2021 Huawei Technologies Co., Ltd.
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

#ifndef _WIRELESS_RX_MONITOR_H_
#define _WIRELESS_RX_MONITOR_H_

#define WLRX_MON_WORK_INTERVAL            100 /* 100ms */
#define WLRX_MON_LOG_INTERVAL             5000 /* 5s */

#ifdef CONFIG_WIRELESS_CHARGER
void wlrx_mon_handler(unsigned int drv_type);
void wlrx_mon_reset_para(unsigned int drv_type, int rst_type);
#else
static inline void wlrx_mon_handler(unsigned int drv_type)
{
}

static inline void wlrx_mon_reset_para(unsigned int drv_type, int rst_type)
{
}
#endif /* CONFIG_WIRELESS_CHARGER */

#endif /* _WIRELESS_RX_MONITOR_H_ */
