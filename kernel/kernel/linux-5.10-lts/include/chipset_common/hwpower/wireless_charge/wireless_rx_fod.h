/* SPDX-License-Identifier: GPL-2.0 */
/*
 * wireless_rx_fod.h
 *
 * rx fod head file for wireless charging
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

#ifndef _WIRELESS_RX_FOD_H_
#define _WIRELESS_RX_FOD_H_

#include <chipset_common/hwpower/common_module/power_nv.h>

#ifdef CONFIG_WIRELESS_CHARGER
void wlrx_fod_reset_para(unsigned int drv_type, int rst_type);
void wlrx_send_fod_status(unsigned int drv_type, unsigned int tx_type);
u8 wlrx_fod_get_support_mode(unsigned int drv_type);
int wlrx_fod_get_cali_para(unsigned int drv_type, struct power_nv_fod_qval *qval);
int wlrx_fod_set_cali_para(unsigned int drv_type, struct power_nv_fod_qval qval);
#else
static inline void wlrx_fod_reset_para(unsigned int drv_type, int rst_type)
{
}

static inline void wlrx_send_fod_status(unsigned int drv_type, unsigned int tx_type)
{
}

static inline u8 wlrx_fod_get_support_mode(unsigned int drv_type)
{
	return 0;
}

static inline int wlrx_fod_get_cali_para(unsigned int drv_type, struct power_nv_fod_qval *qval)
{
	return 0;
}

static inline int wlrx_fod_set_cali_para(unsigned int drv_type, struct power_nv_fod_qval qval)
{
	return 0;
}
#endif /* CONFIG_WIRELESS_CHARGER */

#endif /* _WIRELESS_RX_FOD_H_ */
