/*
 * pm_common.h
 *
 * define public cliend id for suspen/resume vote
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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

#ifndef __H_PM_COMMON_H__
#define __H_PM_COMMON_H__

/*
 * subsystem vote mask
 * CLIENT_ID defined in sys_state_mgr_common.h
 */
#define AP_MASK BIT(0)
#define MODEM_MASK BIT(1)
#define HIFI_MASK BIT(2)
#define IOMCU_MASK BIT(3)
#define LPMCU_MASK BIT(4)
#define GENERAL_SEE_MASK BIT(5)
#define CODEC_MASK BIT(6)
#define ASP_MASK BIT(7)
#define MODEM_NR_MASK BIT(8)
#define GPU_MASK BIT(9)

/**************************************************for ulsr define**********************************************/
/*
 * is_ulsr:
 * setted by user "echo 1 > /sys/power/ulsr"
 * cleared by user "echo 0 > /sys/power/ulsr"
 * recorded in the sctrl register, will be restored by xloader
 * SCTRL SC_LP_FLAG offset 0x018, bit 15: 0 normal sr, 1 ultral lowpower sr
 */

#define SC_LP_FLAG_ULSR_BIT     15
#define SC_LP_FLAG_ULSR_MASK	0x00008000
/**************************************************for ulsr define**********************************************/

/**************************************************for gt define**********************************************/
/*
 * SCTRL SC_LP_FLAG offset 0x018, bit5~bit6: 0 aw mode, 1 ap shutdown to GT mode, 2 STR to GT mode, 3 GT boot mode.
 */

#define SC_LP_FLAG_GT_MODE_SHIFT	5
#define SC_LP_FLAG_GT_MODE_ALL		0x3
#define SC_LP_FLAG_GT_MODE_MASK		((SC_LP_FLAG_GT_MODE_ALL << SC_LP_FLAG_GT_MODE_SHIFT) << 16)

#define SC_LP_FLAG_AW				0x0
#define SC_LP_FLAG_GT_STR	        0x1
#define SC_LP_FLAG_GT_BOOT			0x2
#define SC_LP_FLAG_MAX			    0x3

#define SC_LP_FLAG_GT_STR_LPMCU		BIT(7)
#define SC_LP_FLAG_GT_STR_LPMCU_MASK	(SC_LP_FLAG_GT_STR_LPMCU << 16)
/**************************************************for gt define**********************************************/

/*************************************for sr & ulsr features trace define*************************************/
#define SR_TRACE_REG(base) SOC_SCTRL_SCPERSTAT_POR_RESERVED0_ADDR(base)
/**
 * 1 ~ 999 for ap kernel suspend
 * 1000 ~ 1999 for host suspend
 * 2000 ~ 2999 for atf suspend
 * 3000 ~ 3999 for lpm3 suspend
 * 4000 ~ 4999 for xloader resume
 * 5000 ~ 6799 for lpm3 resume
 * 6800 ~ 6999 for atf resume
 * 7000 ~ 7999 for host resume
 * 8000 ~ 8999 for ap kernel resume
 * 10000 ~ 10499 for slt uefi suspend
 * 10500 ~ 10999 for slt lpm3 suspend
 * 11000 ~ 11999 for slt lpm3 resume
 * 11500 ~ 11999 for slt uefi resume
 * 12000 ~ 12999 for resource power-on and power-off
 * others are reserved now
 */
/*************************************for sr & ulsr features trace define*************************************/

#endif
