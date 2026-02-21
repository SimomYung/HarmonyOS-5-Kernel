/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device driver for schargerV800 direct charge
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
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regmap.h>
#include <linux/power_supply.h>
#include <linux/module.h>
#include <securec.h>
#include <linux/workqueue.h>

#include <chipset_common/hwpower/direct_charge/direct_charger.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_adapter.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_ic.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_ic_manager.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_device_id.h>
#include <chipset_common/hwpower/hardware_ic/lvc_mos.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ic_intf.h>
#include <chipset_common/hwpower/hardware_ic/charge_pump.h>

#include "scharger_v800_regs.h"
#include "scharger_v800_watchdog.h"
#include "scharger_v800_buck.h"
#include "scharger_v800_adc.h"
#include "scharger_v800_watchdog.h"
#include "../hisi_fcp/hisi_fcp_drv.h"
#include "scharger_v800_dc.h"

#define SC_TO_BUCK_DELAY   128

static struct scharger_dc *g_scharger_dc;

static struct opt_regs lvc_v800_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x02 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0x16 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x48 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0X24 ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x03 ),
};

static struct opt_regs lvc_v810_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x02 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x70 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0x16 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x48 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0X24 ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x03 ),
};

static struct opt_regs lvc_v800_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xD4 ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs lvc_v810_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x60 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xD4 ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs rev_lvc_v800_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x05 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0x16 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x48 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x0D ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x1A ),
};

static struct opt_regs rev_lvc_v810_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x05 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x70 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0x16 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x48 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x0D ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x1A ),
};

static struct opt_regs rev_lvc_v800_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x336, 0xFF, 0, 0x00 ),
	reg_cfg( 0x337, 0xFF, 0, 0x00 ),
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs rev_lvc_v810_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x336, 0xFF, 0, 0x00 ),
	reg_cfg( 0x337, 0xFF, 0, 0x00 ),
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x60 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs sc_v800_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x03 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x6C ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x2C ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xF4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X2B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X12 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0X02 ),
};

static struct opt_regs sc_v810_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x03 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x70 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x6F ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x2C ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xF4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X2B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X36 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0X02 ),
};

static struct opt_regs sc_v800_post_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x99E, 0xFF, 0, 0x02 ),
	reg_cfg(0x99F, 0xFF, 0, 0x0A ),
	reg_cfg(0x9A0, 0xFF, 0, 0x02 ),
	reg_cfg(0x9A1, 0xFF, 0, 0x0A ),
	reg_cfg(0x9A2, 0xFF, 0, 0x0A ),
	reg_cfg(0x9A3, 0xFF, 0, 0x0A ),
	reg_cfg(0x9A4, 0xFF, 0, 0x0A ),
	reg_cfg(0x9A5, 0xFF, 0, 0x0A ),
	reg_cfg(0x9A6, 0xFF, 0, 0x14 ),
	reg_cfg(0x99D, 0xFF, 0, 0x8A ),
};

static struct opt_regs sc_v800_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xD4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs sc_v810_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x60 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9DC, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xD4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
};

static struct opt_regs rev_sc_v800_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x06 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x936, 0xFF, 0, 0x03 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X6B ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x02 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X12 ),
	reg_cfg( 0x977, 0xFF, 0, 0x10 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x51 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x0D ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x1A ),
};

static struct opt_regs rev_sc_v810_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x06 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x70 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x936, 0xFF, 0, 0x03 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X6B ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x02 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X12 ),
	reg_cfg( 0x977, 0xFF, 0, 0x10 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x51 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x0D ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x1A ),
};

static struct opt_regs rev_sc_v800_post_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x99E, 0xFF, 0, 0x0A ),
	reg_cfg( 0x99F, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x14 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x8A ),
};

static struct opt_regs rev_sc_v800_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x337, 0xFF, 0, 0x00 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x936, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs rev_sc_v810_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x337, 0xFF, 0, 0x00 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x60 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x936, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs rev_wired_sc_v800_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x336, 0xFF, 0, 0x00 ),
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x936, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs rev_wired_sc_v810_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x336, 0xFF, 0, 0x00 ),
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x60 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x936, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs sc4_v800_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x04 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xF4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X6B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X3F ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x06 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0X02 ),
	reg_cfg( 0x9E3, 0xFF, 0, 0xC1 ),
	reg_cfg( 0x9E6, 0xFF, 0, 0X01 ),
	reg_cfg( 0x9E4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E7, 0xFF, 0, 0x00 ),
};

static struct opt_regs sc4_v810_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x04 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x70 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x43 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x08 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xF4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X6B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x03 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X3F ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x06 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0X02 ),
	reg_cfg( 0x9E3, 0xFF, 0, 0xC1 ),
	reg_cfg( 0x9E6, 0xFF, 0, 0X01 ),
	reg_cfg( 0x9E4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E7, 0xFF, 0, 0x00 ),
};

static struct opt_regs sc4_v800_post_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x99E, 0xFF, 0, 0x0A ),
	reg_cfg( 0x99F, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x14 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x8A ),
};

static struct opt_regs sc4_v800_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xD4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E3, 0xFF, 0, 0xEB ),
	reg_cfg( 0x9E6, 0xFF, 0, 0X2B ),
	reg_cfg( 0x9E4, 0xFF, 0, 0x15 ),
	reg_cfg( 0x9E7, 0xFF, 0, 0x15 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs sc4_v810_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x60 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9BD, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DF, 0xFF, 0, 0xD4 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9BC, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E3, 0xFF, 0, 0xEB ),
	reg_cfg( 0x9E6, 0xFF, 0, 0X2B ),
	reg_cfg( 0x9E4, 0xFF, 0, 0x15 ),
	reg_cfg( 0x9E7, 0xFF, 0, 0x15 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9DA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
};

static struct opt_regs rev_sc4_v800_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x938, 0xFF, 0, 0x40 ),
	reg_cfg( 0x936, 0xFF, 0, 0x08 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x994, 0xFF, 0, 0x10 ),
	reg_cfg( 0x9BA, 0xFF, 0, 0x9F ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x02 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X4B ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X3F ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x06 ),
	reg_cfg( 0x977, 0xFF, 0, 0x18 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x51 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x0D ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x1A ),
};

static struct opt_regs rev_sc4_v810_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x30 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0xC0 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x70 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x40 ),
	reg_cfg( 0x938, 0xFF, 0, 0x40 ),
	reg_cfg( 0x936, 0xFF, 0, 0x08 ),
	reg_cfg( 0x935, 0xFF, 0, 0xF6 ),
	reg_cfg( 0x994, 0xFF, 0, 0x10 ),
	reg_cfg( 0x9BA, 0xFF, 0, 0x9F ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x02 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X4B ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X3F ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x06 ),
	reg_cfg( 0x977, 0xFF, 0, 0x18 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x51 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x0D ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x1A ),
};

static struct opt_regs rev_sc4_v800_post_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x994, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x0A ),
	reg_cfg( 0x99F, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x0A ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x14 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x8A ),
};

static struct opt_regs rev_sc4_v800_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x337, 0xFF, 0, 0x00 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
	reg_cfg( 0x938, 0xFF, 0, 0x00 ),
	reg_cfg( 0x936, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9BA, 0xFF, 0, 0x95 ),
	reg_cfg( 0x994, 0xFF, 0, 0x00 ),
};

static struct opt_regs rev_sc4_v810_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x337, 0xFF, 0, 0x00 ),
	reg_cfg( 0x999, 0xFF, 0, 0x07 ),
	reg_cfg( 0x99E, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99F, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A0, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A1, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A2, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A4, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A5, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A6, 0xFF, 0, 0x00 ),
	reg_cfg( 0x99D, 0xFF, 0, 0x80 ),
	reg_cfg( 0x9A7, 0xFF, 0, 0x40 ),
	reg_cfg( 0x9AB, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9A8, 0xFF, 0, 0x60 ),
	reg_cfg( 0x9AA, 0xFF, 0, 0x00 ),
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E8, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E1, 0xFF, 0, 0X0B ),
	reg_cfg( 0x9EB, 0xFF, 0, 0X00 ),
	reg_cfg( 0x9F3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x977, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9E0, 0xFF, 0, 0x01 ),
	reg_cfg( 0x9DB, 0xFF, 0, 0x09 ),
	reg_cfg( 0x9DD, 0xFF, 0, 0x12 ),
	reg_cfg( 0x938, 0xFF, 0, 0x00 ),
	reg_cfg( 0x936, 0xFF, 0, 0x00 ),
	reg_cfg( 0x9BA, 0xFF, 0, 0x95 ),
	reg_cfg( 0x994, 0xFF, 0, 0x00 ),
};

static int scharger_dc_ibus_ucp_enable(struct scharger_dc *dc, bool enable)
{
	int ret = 0;

	scharger_inf("%s en:%u\n", __func__, enable);

	ret += regmap_update_bits(dc->regmap, CHG_SC_PRO_CFG_REG_1_REG, CHG_S1Q1_UCP_EN_MSK,
		enable << CHG_S1Q1_UCP_EN_SHIFT);
	ret += regmap_update_bits(dc->regmap, CHG_SC_PRO_CFG_REG_1_REG, CHG_S1Q1_RCP_EN_MSK,
		enable << CHG_S1Q1_RCP_EN_SHIFT);
	ret += regmap_update_bits(dc->regmap, CHG_SC_PRO_CFG_REG_3_REG, CHG_S2Q1_UCP_EN_MSK,
		enable << CHG_S2Q1_UCP_EN_SHIFT);
	ret += regmap_update_bits(dc->regmap, CHG_SC_PRO_CFG_REG_3_REG, CHG_S2Q1_RCP_EN_MSK,
		enable << CHG_S2Q1_RCP_EN_SHIFT);

	return ret;
}

static void opt_regs_set(struct scharger_dc *dc, struct opt_regs *opt, unsigned int len)
{
	unsigned int i;

	for (i = 0; i < len; i++)
		(void)regmap_update_bits(dc->regmap, opt[i].reg, opt[i].mask,
			 opt[i].val << opt[i].shift);
}

/* Parameters: mode:SC11/SC21/SC41/RSC11/RSC12/RSC14/WDRC12 */
static void scharger_dc_opt_param(enum charger_mode mode)
{
	struct scharger_dc *dc = g_scharger_dc;

	if (dc == NULL) {
		scharger_err("[%s]dc is null.\n", __func__);
		return;
	}

	if (scharger_chip_version_is_v80x()) {
		switch (mode) {
		case SC11:
			opt_regs_set(dc, lvc_v800_opt_regs, ARRAY_SIZE(lvc_v800_opt_regs));
			break;
		case SC21:
			opt_regs_set(dc, sc_v800_opt_regs, ARRAY_SIZE(sc_v800_opt_regs));
			break;
		case SC41:
			opt_regs_set(dc, sc4_v800_opt_regs, ARRAY_SIZE(sc4_v800_opt_regs));
			break;
		case RSC11:
			opt_regs_set(dc, rev_lvc_v800_opt_regs, ARRAY_SIZE(rev_lvc_v800_opt_regs));
			break;
		case RSC12:
		case WDRC12:
			opt_regs_set(dc, rev_sc_v800_opt_regs, ARRAY_SIZE(rev_sc_v800_opt_regs));
			break;
		case RSC14:
			opt_regs_set(dc, rev_sc4_v800_opt_regs, ARRAY_SIZE(rev_sc4_v800_opt_regs));
			break;
		default:
			scharger_err("%s mode not match!\n", __func__);
			break;
		}
	} else {
		switch (mode) {
		case SC11:
			opt_regs_set(dc, lvc_v810_opt_regs, ARRAY_SIZE(lvc_v810_opt_regs));
			break;
		case SC21:
			opt_regs_set(dc, sc_v810_opt_regs, ARRAY_SIZE(sc_v810_opt_regs));
			if (charge_get_charger_type() != CHARGER_TYPE_WIRELESS)
				regmap_update_bits(dc->regmap, CHG_REF_TOP_CFG_REG_3_REG, CHG_FAST_DROPMIN_EN_MSK,
					1 << CHG_FAST_DROPMIN_EN_SHIFT);
			break;
		case SC41:
			opt_regs_set(dc, sc4_v810_opt_regs, ARRAY_SIZE(sc4_v810_opt_regs));
			if (charge_get_charger_type() != CHARGER_TYPE_WIRELESS)
				regmap_update_bits(dc->regmap, CHG_REF_TOP_CFG_REG_3_REG, CHG_FAST_DROPMIN_EN_MSK,
					1 << CHG_FAST_DROPMIN_EN_SHIFT);
			break;
		case RSC11:
			opt_regs_set(dc, rev_lvc_v810_opt_regs, ARRAY_SIZE(rev_lvc_v810_opt_regs));
			break;
		case RSC12:
		case WDRC12:
			opt_regs_set(dc, rev_sc_v810_opt_regs, ARRAY_SIZE(rev_sc_v810_opt_regs));
			break;
		case RSC14:
			opt_regs_set(dc, rev_sc4_v810_opt_regs, ARRAY_SIZE(rev_sc4_v810_opt_regs));
			break;
		default:
			scharger_err("%s mode not match!\n", __func__);
			break;
		}
	}
}

/* Parameters: mode:SC11/SC21/SC41/RSC11/RSC12/RSC14/WDRC12 */
static void scharger_dc_post_opt_param(enum charger_mode mode)
{
	struct scharger_dc *dc = g_scharger_dc;

	if (dc == NULL) {
		scharger_err("[%s]dc is null.\n", __func__);
		return;
	}

	switch (mode) {
	case SC11:
		break;
	case SC21:
		opt_regs_set(dc, sc_v800_post_opt_regs, ARRAY_SIZE(sc_v800_post_opt_regs));
		break;
	case SC41:
		opt_regs_set(dc, sc4_v800_post_opt_regs, ARRAY_SIZE(sc4_v800_post_opt_regs));
		break;
	case RSC11:
		break;
	case RSC12:
	case WDRC12:
		opt_regs_set(dc, rev_sc_v800_post_opt_regs, ARRAY_SIZE(rev_sc_v800_post_opt_regs));
		break;
	case RSC14:
		opt_regs_set(dc, rev_sc4_v800_post_opt_regs, ARRAY_SIZE(rev_sc4_v800_post_opt_regs));
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		break;
	}
}

/* Parameters: mode:SC11/SC21/SC41/RSC11/RSC12/RSC14/WDRC12 */
static void scharger_dc_exit_opt_param(enum charger_mode mode)
{
	struct scharger_dc *dc = g_scharger_dc;

	if (dc == NULL) {
		scharger_err("[%s]dc is null.\n", __func__);
		return;
	}

	if (scharger_chip_version_is_v80x()) {
		switch (mode) {
		case SC11:
			opt_regs_set(dc, lvc_v800_exit_opt_regs, ARRAY_SIZE(lvc_v800_exit_opt_regs));
			break;
		case SC21:
			opt_regs_set(dc, sc_v800_exit_opt_regs, ARRAY_SIZE(sc_v800_exit_opt_regs));
			break;
		case SC41:
			opt_regs_set(dc, sc4_v800_exit_opt_regs, ARRAY_SIZE(sc4_v800_exit_opt_regs));
			break;
		case RSC11:
			opt_regs_set(dc, rev_lvc_v800_exit_opt_regs, ARRAY_SIZE(rev_lvc_v800_exit_opt_regs));
			break;
		case RSC12:
			opt_regs_set(dc, rev_sc_v800_exit_opt_regs, ARRAY_SIZE(rev_sc_v800_exit_opt_regs));
			break;
		case WDRC12:
			opt_regs_set(dc, rev_wired_sc_v800_exit_opt_regs, ARRAY_SIZE(rev_wired_sc_v800_exit_opt_regs));
			break;
		case RSC14:
			opt_regs_set(dc, rev_sc4_v800_exit_opt_regs, ARRAY_SIZE(rev_sc4_v800_exit_opt_regs));
			break;
		default:
			scharger_err("%s mode not match!\n", __func__);
			break;
		}
	} else {
		switch (mode) {
		case SC11:
			opt_regs_set(dc, lvc_v810_exit_opt_regs, ARRAY_SIZE(lvc_v810_exit_opt_regs));
			break;
		case SC21:
			opt_regs_set(dc, sc_v810_exit_opt_regs, ARRAY_SIZE(sc_v810_exit_opt_regs));
			break;
		case SC41:
			opt_regs_set(dc, sc4_v810_exit_opt_regs, ARRAY_SIZE(sc4_v810_exit_opt_regs));
			break;
		case RSC11:
			opt_regs_set(dc, rev_lvc_v810_exit_opt_regs, ARRAY_SIZE(rev_lvc_v810_exit_opt_regs));
			break;
		case RSC12:
			opt_regs_set(dc, rev_sc_v810_exit_opt_regs, ARRAY_SIZE(rev_sc_v810_exit_opt_regs));
			break;
		case WDRC12:
			opt_regs_set(dc, rev_wired_sc_v810_exit_opt_regs, ARRAY_SIZE(rev_wired_sc_v810_exit_opt_regs));
			break;
		case RSC14:
			opt_regs_set(dc, rev_sc4_v810_exit_opt_regs, ARRAY_SIZE(rev_sc4_v810_exit_opt_regs));
			break;
		default:
			scharger_err("%s mode not match!\n", __func__);
			break;
		}
	}
}

static bool dc_is_iwatt_lvc_adapter(void)
{
	u32 value[ADAPTER_VENDOR_ID_SIZE] = { 0 };
	int adp_type = -1;
	int id;

	scharger_scp_read_block(ADAPTER_VENDOR_ID, value, ADAPTER_VENDOR_ID_SIZE);
	id = (value[0] << 8) | value[1];
	if (id != IWATT_ADAPTER_ID)
		return false;

	scharger_scp_read_block(ADAPTER_TYPE, &adp_type, 1);
	if (adp_type != 0)
		return false;

	scharger_inf("use iwatt lvc adapter\n");
	return true;
}

static int dc_get_iadpter(void)
{
	int value = 0;

	dc_get_adapter_current(&value);
	scharger_inf("%s iadpter=%d\n", __func__, value);
	return value;
}

static int sc_charge_init(void *dev_data)
{
	int ret = 0;
	struct scharger_dc *dc = dev_data;

	scharger_err("%s +\n", __func__);
	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = buck_init(VBUS_VSET_5V);

	scharger_v800_mode_lock();

	if (is_current_charge_mode(SC21)) {
		scharger_v800_mode_unlock();
		return 0;
	}

	scharger_dc_opt_param(SC21);

	/* set usb ovp 11v or set psw ovp 11v */
	ret += scharger_set_vusb_vpsw_ovp(SC21);

	ret += scharger_v800_set_current_charge_mode(SC21);
	scharger_v800_set_dc_charging(true);

	scharger_v800_mode_unlock();

	if (ret != 0)
		scharger_err("%s fail!\n", __func__);
	return ret;
}

static int sc4_charge_init(void *dev_data)
{
	int ret = 0;
	struct scharger_dc *dc = dev_data;

	scharger_err("%s +\n", __func__);
	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = buck_init(VBUS_VSET_5V);

	scharger_v800_mode_lock();

	if (is_current_charge_mode(SC41)) {
		scharger_v800_mode_unlock();
		return 0;
	}

	scharger_dc_opt_param(SC41);

	/* set usb ovp 20v or set psw ovp 20v */
	ret += scharger_set_vusb_vpsw_ovp(SC41);

	ret += scharger_v800_set_current_charge_mode(SC41);
	scharger_v800_set_dc_charging(true);
	scharger_v800_mode_unlock();

	if (ret != 0)
		scharger_err("%s fail!\n", __func__);
	return ret;
}

static int lvc_charge_init(void *dev_data)
{
	int ret;
	struct scharger_dc *dc = dev_data;

	scharger_err("%s +\n", __func__);
	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = buck_init(VBUS_VSET_5V);

	scharger_v800_mode_lock();

	if (is_current_charge_mode(SC11)) {
		scharger_v800_mode_unlock();
		return 0;
	}

	scharger_dc_opt_param(SC11);

	/* set usb ovp 6.5v and set psw ovp 6.5v */
	ret += scharger_v800_set_vpsw_ovp(SC11);
	ret += scharger_v800_set_vusb_ovp(SC11);

	ret += scharger_v800_set_current_charge_mode(SC11);
	scharger_v800_set_dc_charging(true);
	scharger_v800_mode_unlock();

	dc->iwatt_lvc_adap = dc_is_iwatt_lvc_adapter();

	if (ret != 0)
		scharger_err("%s fail!\n", __func__);
	return ret;
}

static int lvc_charge_exit(void *dev_data)
{
	struct scharger_dc *dc = dev_data;
	int ret = 0;

	scharger_err("%s!\n", __func__);

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_dc_exit_opt_param(SC11);
	ret += scharger_v800_dc_switch_to_buck_mode();
	scharger_v800_set_dc_charging(false);

	dc->iwatt_lvc_adap = false;
	dc->dc_init_flag = false;

	return ret;
}

static int sc_charge_exit(void *dev_data)
{
	struct scharger_dc *dc = dev_data;
	int ret = 0;

	scharger_err("%s!\n", __func__);

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_dc_exit_opt_param(SC21);
	ret += scharger_v800_dc_switch_to_buck_mode();
	scharger_v800_set_dc_charging(false);

	dc->iwatt_lvc_adap = false;
	dc->dc_init_flag = false;

	return ret;
}

static int sc4_charge_exit(void *dev_data)
{
	struct scharger_dc *dc = dev_data;
	int ret = 0;

	scharger_err("%s!\n", __func__);

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_dc_exit_opt_param(SC41);
	ret += scharger_v800_dc_switch_to_buck_mode();
	scharger_v800_set_dc_charging(false);
	dc->iwatt_lvc_adap = false;
	dc->dc_init_flag = false;

	return ret;
}

static int scharger_dc_sc_enable(struct scharger_dc *dc, int enable)
{
	scharger_inf("%s enable:%d\n", __func__, enable);

	return regmap_update_bits(dc->regmap, CHG_DA_SC_EN_REG, CHG_SC_EN_MSK,
			 !!enable << CHG_SC_EN_SHIFT);
}

static int scharger_dc_drop_ovp_enable(struct scharger_dc *dc, bool enable)
{
	if (dc->scharger_version == CHIP_ID_V800)
		return regmap_update_bits(dc->regmap, CHG_SYS_DET_CFG_REG_5_REG, CHG_DROP_OVP_EN_MSK,
				 enable << CHG_DROP_OVP_EN_SHIFT);

	return 0;
}

int sc_charge_enable(int enable, void *dev_data)
{
	struct scharger_dc *dc = dev_data;
	unsigned int en;
	int ret = 0;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	en = !!enable;
	scharger_inf("%s en %u\n", __func__, en);

	ret += scharger_dc_sc_enable(dc, en);
	dc->dc_init_flag = en;
	if (en)
		queue_delayed_work(system_power_efficient_wq, &dc->ucp_en_work,
			msecs_to_jiffies(0));
	return ret;
}

static int dc_discharge_dummy_func(int val, void *dev_data)
{
	unused(dev_data);

	return 0;
}

static int charge_is_close(void *dev_data)
{
	struct scharger_dc *dc = dev_data;
	int ret, is_close;
	unsigned int val = 0;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}
	ret = regmap_read(dc->regmap, CHG_DA_SC_EN_REG, &val);
	if (ret == 0 && (val & CHG_SC_EN_MSK))
		is_close = 0;
	else
		is_close = 1;
	return is_close;
}

static int sc4_charge_is_close(void *dev_data)
{
	struct scharger_dc *dc = dev_data;
	int ret;
	int vbat;
	int vbus_uv;
	int is_close;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	is_close = charge_is_close(dev_data);
	if (is_close == 0) {
		vbat = scharger_v800_get_vbat();
		/* According to the battery voltage around 3400mv, configure vbus_uv */
		if (vbat < 3400)
			vbus_uv = VBUS_UV_10V;
		else
			vbus_uv = VBUS_UV_12V;

		ret = regmap_update_bits(dc->regmap, CHG_REF_TOP_CFG_REG_3_REG, CHG_VBUS_UV_SET_MSK,
			vbus_uv << CHG_VBUS_UV_SET_SHIFT);
		if (ret != 0)
			scharger_err("%s set vbus_uv fail\n", __func__);
	}

	return is_close;
}

static int get_loadswitch_id(void *dev_data)
{
	unused(dev_data);
	return 0;
}

static int dc_config_watchdog_ms(int time, void *dev_data)
{
	unused(dev_data);
	return scharger_v800_set_watchdog_timer_ms(time);
}

static int dc_kick_watchdog_timer(void *dev_data)
{
	unused(dev_data);
	return scharger_v800_reset_watchdog_timer();
}

static int get_sc_device_id(void *dev_data)
{
	unused(dev_data);
	return SWITCHCAP_SCHARGERV800;
}

static int scharger_dc_scp_protect_enable(struct scharger_dc *dc, bool enable)
{
	scharger_inf("%s enable:%u\n", __func__, enable);
	return regmap_update_bits(dc->regmap, CHG_USB_OVP_CFG_REG_4_REG,
				 CHG_SCP_PRO_EN_MSK, !enable << CHG_SCP_PRO_EN_SHIFT);
}

int sc4_reverse_charge_enable(void *dev_data, bool enable)
{
	int ret = 0;
	struct scharger_dc *dc = dev_data;

	scharger_err("%s en:%d\n", __func__, enable);

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	if (enable) {
		ret = buck_init(VBUS_VSET_5V);
		scharger_v800_mode_lock();

		if (is_current_charge_mode(RSC14)) {
			scharger_v800_mode_unlock();
			return 0;
		}
		scharger_v800_set_watchdog_timer(0);
		scharger_dc_opt_param(RSC14);

		/* set psw ovp 20v */
		ret += scharger_v800_set_vpsw_ovp(RSC14);
		ret += scharger_v800_set_psw_ovp_mode(PSWOVP_FORCE_OPEN);
		mdelay(2);
		ret += scharger_dc_sc_enable(dc, 1);
		ret += scharger_v800_set_current_charge_mode(RSC14);

		scharger_v800_mode_unlock();

		queue_delayed_work(system_power_efficient_wq, &dc->after_rev_sc_work,
				   msecs_to_jiffies(AFTER_REV_SC_DELAY));
	} else {
		ret += scharger_dc_sc_enable(dc, 0);
		scharger_dc_exit_opt_param(RSC14);
		mdelay(35);
		ret += buck_init(VBUS_VSET_5V);
	}
	if (ret != 0)
		scharger_err("%s fail!\n", __func__);

	return ret;
}

static int sc_reverse_charge_enable(void *dev_data, bool enable)
{
	int ret = 0;
	struct scharger_dc *dc = dev_data;

	scharger_err("%s en:%d\n", __func__, enable);

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	if (enable) {
		ret = buck_init(VBUS_VSET_5V);
		scharger_v800_mode_lock();

		if (is_current_charge_mode(RSC12)) {
			scharger_v800_mode_unlock();
			return 0;
		}

		scharger_v800_set_watchdog_timer(0);
		scharger_dc_opt_param(RSC12);

		/* set psw ovp 11v */
		ret = scharger_v800_set_vpsw_ovp(RSC12);
		ret += scharger_v800_set_psw_ovp_mode(PSWOVP_REVERSE_MODE);
		mdelay(2);
		ret += scharger_dc_sc_enable(dc, 1);
		ret += scharger_v800_set_current_charge_mode(RSC12);

		scharger_v800_mode_unlock();

		queue_delayed_work(system_power_efficient_wq, &dc->after_rev_sc_work,
				   msecs_to_jiffies(AFTER_REV_SC_DELAY));
	} else {
		ret += scharger_dc_sc_enable(dc, 0);
		scharger_dc_exit_opt_param(RSC12);
		mdelay(35);
		ret += buck_init(VBUS_VSET_5V);
	}
	if (ret != 0)
		scharger_err("%s fail!\n", __func__);

	return ret;
}

static int sc_wired_reverse_charge_enable(bool enable, void *dev_data)
{
	int ret = 0;
	struct scharger_dc *dc = dev_data;

	scharger_err("%s en:%d\n", __func__, enable);

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}

	if (enable) {
		ret = buck_init(VBUS_VSET_5V);
		scharger_v800_mode_lock();

		if (is_current_charge_mode(WDRC12)) {
			scharger_v800_mode_unlock();
			return 0;
		}

		scharger_v800_set_watchdog_timer(0);
		scharger_dc_opt_param(WDRC12);

		ret = scharger_v800_set_vusb_ovp(WDRC12);
		ret += scharger_v800_set_usb_ovp_mode(USBOVP_REVERSE_MODE);
		mdelay(2);
		ret += scharger_dc_sc_enable(dc, 1);
		 ret += scharger_v800_set_current_charge_mode(WDRC12);

		 scharger_v800_mode_unlock();

		 queue_delayed_work(system_power_efficient_wq, &dc->after_rev_sc_work,
				   msecs_to_jiffies(AFTER_REV_SC_DELAY));
	} else {
		ret += scharger_dc_sc_enable(dc, 0);
		scharger_dc_exit_opt_param(WDRC12);
		mdelay(35);
		ret += buck_init(VBUS_VSET_5V);
	}
	if (ret != 0)
		scharger_err("%s fail!\n", __func__);

	return ret;
}

void sc_charge_disable_for_irq(void)
{
	struct scharger_dc *dc = g_scharger_dc;
	struct current_charger_mode *charger_mode;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return;
	}

	charger_mode = scharger_v800_get_current_charge_mode();
	if(charger_mode == NULL) {
		scharger_err("%s charger_mode is NULL!\n", __func__);
		return;
	}

	if (charger_mode->chg_mode == SC11) {
		sc_charge_enable(0, dc);
		scharger_v800_gate1_enable(0);
	} else if (charger_mode->chg_mode == SC21 || charger_mode->chg_mode == SC41) {
		sc_charge_enable(0, dc);
	} else if (charger_mode->chg_mode == RSC12) {
		sc_reverse_charge_enable(dc, 0);
	} else if (charger_mode->chg_mode == RSC14) {
		sc4_reverse_charge_enable(dc, 0);
	} else if (charger_mode->chg_mode == WDRC12) {
		sc_wired_reverse_charge_enable(0, dc);
	}
}

struct scharger_dc *scharger_v800_get_dc_device(void)
{
	return g_scharger_dc;
}

static struct dc_ic_ops lvc_ops = {
	.dev_name = "schargerV800",
	.ic_init = lvc_charge_init,
	.ic_exit = lvc_charge_exit,
	.ic_enable = sc_charge_enable,
	.ic_discharge = dc_discharge_dummy_func,
	.is_ic_close = charge_is_close,
	.get_ic_id = get_loadswitch_id,
	.config_ic_watchdog = dc_config_watchdog_ms,
	.kick_ic_watchdog = dc_kick_watchdog_timer,
};

static struct dc_ic_ops sc_ops = {
	.dev_name = "schargerV800",
	.ic_init = sc_charge_init,
	.ic_exit = sc_charge_exit,
	.ic_enable = sc_charge_enable,
	.ic_discharge = dc_discharge_dummy_func,
	.is_ic_close = charge_is_close,
	.get_ic_id = get_sc_device_id,
	.config_ic_watchdog = dc_config_watchdog_ms,
	.kick_ic_watchdog = dc_kick_watchdog_timer,
	.set_ic_thld = NULL,
};

static struct dc_ic_ops sc4_ops = {
	.dev_name = "schargerV800",
	.ic_init = sc4_charge_init,
	.ic_exit = sc4_charge_exit,
	.ic_enable = sc_charge_enable,
	.ic_discharge = dc_discharge_dummy_func,
	.is_ic_close = sc4_charge_is_close,
	.get_ic_id = get_sc_device_id,
	.config_ic_watchdog = dc_config_watchdog_ms,
	.kick_ic_watchdog = dc_kick_watchdog_timer,
};

static int dummy_func(void *dev_data)
{
	return 0;
}

static struct charge_pump_ops rvs_ops = {
	.cp_type = CP_TYPE_MAIN,
	.chip_name = "schargerV800",
	.dev_check = dummy_func,
	.post_probe = dummy_func,
	.rvs_cp_chip_init = sc_reverse_charge_enable,
	.rvs_cp4_chip_init = sc4_reverse_charge_enable,
};

static struct rchg_pd_ic_ops wired_rc_ops = {
	.dev_name = "schargerV800",
	.enable_reverse_charge = sc_wired_reverse_charge_enable,
};

static int dc_batinfo_init(void *dev_data)
{
	return 0;
}

static int dc_batinfo_exit(void *dev_data)
{
	return 0;
}

static int dc_get_vbat_mv(void *dev_data)
{
	int vbat;
	unused(dev_data);

	vbat = scharger_v800_get_vbat();
	scharger_inf("%s vbat=%d\n", __func__, vbat);
	return vbat;
}

static int dc_get_vbus_mv(int *vbus, void *dev_data)
{
	unused(dev_data);
	if (vbus == NULL)
		return -EINVAL;

	*vbus = scharger_v800_get_vbus();

	scharger_inf("%s vbus=%d\n", __func__, *vbus);
	return 0;
}

static int dc_get_coul_ibat(const char *batt_name, int *ibat)
{
	int ret;
	struct power_supply *psy = NULL;
	union power_supply_propval val = {0};

	if (ibat == NULL)
		return -EINVAL;

	psy = power_supply_get_by_name(batt_name);
	if (psy == NULL) {
		scharger_err("power supply %s not exist.\n", batt_name);
		return -EINVAL;
	}

	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_CURRENT_NOW, &val);
	power_supply_put(psy);

	*ibat = val.intval;

	return ret;
}

static int dc_get_ibat_ma(int *ibat, void *dev_data)
{
	int ret = 0;
	struct scharger_dc *dc = dev_data;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}
	if (ibat == NULL)
		return -EINVAL;

	ret += dc_get_coul_ibat("battery_gauge", ibat);

	scharger_inf("%s ibat=%d\n", __func__, *ibat);
	if (*ibat < 0)
		*ibat = 0;

	return ret;
}

static int dc_calculate_ibus_ma(int value, struct scharger_dc *dc)
{
	int total_ibus = 0;
	int aux_ibus = 0;
	unsigned int stage;

	if (!dc->dc_init_flag || dc->iwatt_lvc_adap)
		return value;

	stage = direct_charge_get_stage_status();
	if (stage >= DC_STAGE_CHARGE_INIT && stage <= DC_STAGE_CHARGING)
		total_ibus = dc_get_iadpter();
	else
		wlrx_ic_get_iout(WLTRX_IC_MAIN, &total_ibus);

	if (dc_ic_get_ic_ops(SC_MODE, IC_TWO))
		dcm_get_ic_ibus(SC_MODE, CHARGE_IC_AUX, &aux_ibus);

	scharger_inf("%s stage=%u total_ibus=%d, aux_ibus=%d\n", __func__, stage, total_ibus, aux_ibus);
	return (total_ibus - aux_ibus > 0) ? (total_ibus - aux_ibus) : (value - aux_ibus);
}

static int dc_get_ibus_ma(int *ibus, void *dev_data)
{
	int ibat = 0;
	int ret = 0;
	int ratio = 1;
	struct scharger_dc *dc = dev_data;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return -EINVAL;
	}
	if (ibus == NULL)
		return -EINVAL;

	if (dc->scharger_version == CHIP_ID_V800) {
		ret += dc_get_coul_ibat("battery_gauge", &ibat);
		if (dc->dc_chg_mode == SC4)
			ratio = 4; /* ibus ratio is 4 */
		else if (dc->dc_chg_mode == SC)
			ratio = 2; /* ibus ratio is 2 */
		ibat /= ratio;

		*ibus = dc_calculate_ibus_ma(ibat, dc);

		scharger_inf("%s ibat=%d, ibus=%d\n", __func__, ibat, *ibus);
		if (*ibus < 0)
			*ibus = 0;
	} else {
		*ibus = scharger_v800_get_ibus();
		scharger_inf("%s ibus=%d\n", __func__, *ibus);
	}

	return ret;
}

static int dc_get_chip_temp(int *temp, void *dev_data)
{
	unused(dev_data);
	if (temp == NULL)
		return -EINVAL;

	*temp = scharger_v800_get_tdie();
	return *temp;
}

static int dc_get_vusb_mv(int *vusb, void *dev_data)
{
	unused(dev_data);
	if (vusb == NULL)
		return -EINVAL;

	*vusb = scharger_v800_get_vusb();

	return 0;
}

static struct dc_batinfo_ops batinfo_ops = {
	.init = dc_batinfo_init,
	.exit = dc_batinfo_exit,
	.get_bat_btb_voltage = dc_get_vbat_mv,
	.get_bat_package_voltage = dc_get_vbat_mv,
	.get_vbus_voltage = dc_get_vbus_mv,
	.get_bat_current = dc_get_ibat_ma,
	.get_ic_ibus = dc_get_ibus_ma,
	.get_ic_temp = dc_get_chip_temp,
	.get_ic_vusb = dc_get_vusb_mv,
};

static struct lvc_mos_ic_ops lvc_mos_ops = {
	.ic_enable = scharger_v800_gate1_enable,
	.get_bat_btb_voltage = dc_get_vbat_mv,
	.get_vbus_voltage = dc_get_vbus_mv,
	.get_ic_ibus = dc_get_ibus_ma,
	.get_ic_vusb = dc_get_vusb_mv,
};

static void scp_ops_register(struct scharger_dc *dc)
{
	int ret = 0;

	lvc_ops.dev_data = (void *)dc;
	sc_ops.dev_data = (void *)dc;
	sc4_ops.dev_data = (void *)dc;
	batinfo_ops.dev_data = (void *)dc;
	rvs_ops.dev_data = (void *)dc;

	ret += dc_ic_ops_register(LVC_MODE, IC_ONE, &lvc_ops);
	ret += dc_ic_ops_register(SC_MODE, IC_ONE, &sc_ops);
	ret += dc_ic_ops_register(SC4_MODE, IC_ONE, &sc4_ops);
	ret += dc_batinfo_ops_register(IC_ONE, &batinfo_ops, SWITCHCAP_SCHARGERV800);
	if (dc->lvc_mos_support) {
		lvc_mos_ops.dev_data = (void *)dc;
		ret += lvc_mos_ic_ops_register(&lvc_mos_ops);
	}
	ret += charge_pump_ops_register(&rvs_ops);
	if (ret != 0)
		scharger_err("register scp ops failed!\n");

	if (dc->wired_rvs_support) {
		wired_rc_ops.dev_data = (void *)dc;
		(void)rchg_pd_ops_register(&wired_rc_ops);
	}
}

static int dc_event_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct scharger_dc *dc = g_scharger_dc;

	if (!dc)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_USB_DISCONNECT:
		dc->dc_init_flag = false;
		scharger_inf("usb disconnect, set init flag false\n");
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static void ucp_en_delay_work(struct work_struct *work)
{
	struct scharger_dc *dc = g_scharger_dc;
	int ibus;
	struct current_charger_mode *charger_mode;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return;
	}

	charger_mode = scharger_v800_get_current_charge_mode();
	if(charger_mode == NULL) {
		scharger_err("%s charger_mode is NULL!\n", __func__);
		return;
	}

	if ((charger_mode->chg_mode != SC11) && (charger_mode->chg_mode != SC21) && (charger_mode->chg_mode != SC41)) {
		scharger_err("%s chg mode:%u, exit\n", __func__, charger_mode->chg_mode);
		return;
	}

	dc_get_ibus_ma(&ibus, dc);
	scharger_inf("%s ibus:%d\n", __func__, ibus);

	if (ibus >= IBUS_500MA) {
		scharger_dc_ibus_ucp_enable(dc, 1);
		scharger_dc_post_opt_param(charger_mode->chg_mode);
	} else {
		queue_delayed_work(system_power_efficient_wq, &dc->ucp_en_work,
			   msecs_to_jiffies(UCP_EN_DELAY));
	}
}

static void after_rev_sc_delay_work(struct work_struct *work)
{
	struct scharger_dc *dc = g_scharger_dc;
	struct current_charger_mode* charger_mode;

	if (dc == NULL) {
		scharger_err("%s scharger_dc is NULL!\n", __func__);
		return;
	}

	charger_mode = scharger_v800_get_current_charge_mode();
	if(charger_mode == NULL) {
		scharger_err("%s charger_mode is NULL!\n", __func__);
		return;
	}

	if ((charger_mode->chg_mode != RSC11) && (charger_mode->chg_mode != RSC12) && (charger_mode->chg_mode != RSC14) && (charger_mode->chg_mode != WDRC12)) {
		scharger_err("%s chg mode:%u, exit\n", __func__, charger_mode->chg_mode);
		return;
	}
	scharger_dc_post_opt_param(charger_mode->chg_mode);
}

static void scharger_dc_parse_dts(struct device_node *np, struct scharger_dc *dc)
{
	if (of_property_read_u32(np, "lvc_mos_support", &dc->lvc_mos_support) != 0)
		dc->lvc_mos_support = 0;

	if (of_property_read_u32(np, "wired_rvs_support", &dc->wired_rvs_support) != 0)
		dc->wired_rvs_support = 0;

	scharger_inf("lvc_mos_support=%u, wired_rvs_support = %u\n", dc->lvc_mos_support, dc->wired_rvs_support);
}

static int scharger_dc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct scharger_dc *dc = NULL;

	dc = devm_kzalloc(dev, sizeof(*dc), GFP_KERNEL);
	if (dc == NULL) {
		scharger_err("[%s]dc is null.\n", __func__);
		return -ENOMEM;
	}

	dc->dev = dev;
	dc->regmap = dev_get_regmap(dev->parent, NULL);
	if (dc->regmap == NULL) {
		scharger_err("Parent regmap unavailable.\n");
		return -ENXIO;
	}

	scharger_dc_parse_dts(dev->of_node, dc);
	dc->nb.notifier_call = dc_event_notifier_call;
	(void)power_event_bnc_register(POWER_BNT_CONNECT, &dc->nb);
	INIT_DELAYED_WORK(&dc->ucp_en_work, ucp_en_delay_work);
	INIT_DELAYED_WORK(&dc->after_rev_sc_work, after_rev_sc_delay_work);

	scp_ops_register(dc);
	dc->dc_chg_mode = NONE;
	dc->scharger_version = scharger_v800_device_version();
	g_scharger_dc = dc;

	return 0;
}

static void scharger_dc_shutdown(struct platform_device *pdev)
{
}

const static struct of_device_id of_scharger_dc_match_tbl[] = {
	{
		.compatible = "hisilicon,scharger-v800-dc",
		.data = NULL,
	},
	{},
};

static struct platform_driver scharger_dc_driver = {
	.driver = {
		.name = "scharger_v800_dc",
		.owner = THIS_MODULE,
		.of_match_table = of_scharger_dc_match_tbl,
	},
	.probe = scharger_dc_probe,
	.shutdown = scharger_dc_shutdown,
};

static int __init scharger_dc_init(void)
{
	return platform_driver_register(&scharger_dc_driver);
}

static void __exit scharger_dc_exit(void)
{
	platform_driver_unregister(&scharger_dc_driver);
}

fs_initcall(scharger_dc_init);
module_exit(scharger_dc_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("schargerV800 direct charge driver");