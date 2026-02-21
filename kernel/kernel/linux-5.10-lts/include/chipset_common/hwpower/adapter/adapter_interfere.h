/* SPDX-License-Identifier: GPL-2.0 */
/*
 * adapter_interfere.h
 *
 * adapter interfere driver
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

#ifndef _ADAPTER_INTERFERE_H_
#define _ADAPTER_INTERFERE_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <linux/notifier.h>

enum adpt_intfr_cable_type {
	ADPT_INTRF_CABLE_TYPE_NONE = 0,
	ADPT_INTRF_CABLE_TYPE_1M8,
	ADPT_INTRF_CABLE_END,
};

#ifdef CONFIG_ADAPTER_PROTOCOL
void adpt_intfr_recv_cch_comp_maybe_low(void);
void adpt_intfr_check_adpt_code(int adap_code);
void adpt_intfr_set_cable_type(enum adpt_intfr_cable_type cable_type);
void adpt_intfr_check_cable(int rpath, int intfr_resist);
#else
static inline void adpt_intfr_recv_cch_comp_maybe_low(void)
{
}

static inline void adpt_intfr_check_adpt_code(int adap_code)
{
}

static inline void adpt_intfr_set_cable_type(enum adpt_intfr_cable_type cable_type)
{
}

static inline void adpt_intfr_check_cable(int rpath, int intfr_resist)
{
}
#endif /* CONFIG_ADAPTER_PROTOCOL */

#endif /* _ADAPTER_INTERFERE_H_ */
