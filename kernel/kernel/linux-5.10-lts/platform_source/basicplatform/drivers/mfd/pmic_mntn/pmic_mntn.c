/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * pmic_mntn.c
 *
 * Device driver for PMU DRIVER
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

#include "pmic_mntn_inner.h"
#include <platform_include/basicplatform/linux/mfd/pmic_mntn.h>
#include <linux/platform_drivers/usb/chip_usb.h>
#include <linux/of_address.h>
#include <linux/version.h>
#include <platform_include/basicplatform/linux/spmi_platform.h>
#include <platform_include/basicplatform/linux/of_platform_spmi.h>
#include <linux/notifier.h>
#include <linux/export.h>
#include <linux/regmap.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/pr_log.h>
#define PR_LOG_TAG PMIC_MNTN_TAG
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif

#define PMIC_DSM_MASK_STATE 0xFF
#define PMIC_DSM_EXP_NUM 400

#if defined CONFIG_PMIC_55V200_PMU || \
	defined CONFIG_PMIC_55V300_PMU || \
	defined CONFIG_PMIC_21V700_PMU || \
	defined CONFIG_PMIC_21V800_PMU
#define PMIC_DSM_IGNORE_NUM 99
#else
/* BALT and later platform */
#define PMIC_DSM_IGNORE_NUM 999
#endif

#ifdef CONFIG_GCOV_KERNEL
#define STATIC
#else
#define STATIC static
#endif

#define OVP_IRQ_MASK 0x4
#define OVP_IRQ_UNMASK 0

#ifdef PMIC_FER_PMU_IRQ_MSK0_ADDR
#define PMU_IRQ_MASK_OVP_IRQ PMIC_FER_PMU_IRQ_MSK0_ADDR(0)
#else
#define PMU_IRQ_MASK_OVP_IRQ 0
#endif

#define PMU0	0
#define PMU1	1
#define PMU2	2
#define PMU_NUM	3
#define PMU_INVALID_FER_NUM	8
static struct pmic_mntn_desc *g_pmic_mntn[PMU_NUM];
static void __iomem *g_sysctrl_base;

#ifdef CONFIG_HUAWEI_OHOS_DSM
static int g_dsm_register_count = 0;
#endif

#ifdef CONFIG_PMIC_PLATFORM_DEBUG
static int g_mntn_notifier_flag;
#endif

#ifdef CONFIG_HUAWEI_OHOS_DSM
struct hisysevent_error_no_map pmic_event_map[] = {
	{ 920005002, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_VSYS_OV_D200UR", "CONTENT", 0 },
	{ 920005003, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_VSYS_UV_D10MR", "CONTENT", 0 },
	{ 920005004, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_VSYS_PWROFF_ABS_2D", "CONTENT", 0 },
	{ 920005005, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_VSYS_PWROFF_DEB_D80MR", "CONTENT", 0 },
	{ 920005006, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_VSYS_THSD_OTMP140_D1MR", "CONTENT", 0 },
	{ 920005007, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_VSYS_THSD_OTMP125_D1MR", "CONTENT", 0 },
	{ 920007000, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO2", "CONTENT", 0 },
	{ 920007001, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO1", "CONTENT", 0 },
	{ 920007002, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO0", "CONTENT", 0 },
	{ 920007003, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK4", "CONTENT", 0 },
	{ 920007004, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK3", "CONTENT", 0 },
	{ 920007005, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK2", "CONTENT", 0 },
	{ 920007006, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK1", "CONTENT", 0 },
	{ 920007007, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK0", "CONTENT", 0 },
	{ 920007008, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO11", "CONTENT", 0 },
	{ 920007009, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO10", "CONTENT", 0 },
	{ 920007010, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO9", "CONTENT", 0 },
	{ 920007011, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO8", "CONTENT", 0 },
	{ 920007012, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO7", "CONTENT", 0 },
	{ 920007013, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO5", "CONTENT", 0 },
	{ 920007014, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO4", "CONTENT", 0 },
	{ 920007015, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO3", "CONTENT", 0 },
	{ 920007016, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO20", "CONTENT", 0 },
	{ 920007017, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO19", "CONTENT", 0 },
	{ 920007018, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO17", "CONTENT", 0 },
	{ 920007019, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO16", "CONTENT", 0 },
	{ 920007020, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO15", "CONTENT", 0 },
	{ 920007021, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO14", "CONTENT", 0 },
	{ 920007022, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO13", "CONTENT", 0 },
	{ 920007023, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO12", "CONTENT", 0 },
	{ 920007024, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO28", "CONTENT", 0 },
	{ 920007025, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO27", "CONTENT", 0 },
	{ 920007026, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO26", "CONTENT", 0 },
	{ 920007027, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO25", "CONTENT", 0 },
	{ 920007028, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO24", "CONTENT", 0 },
	{ 920007029, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO23", "CONTENT", 0 },
	{ 920007030, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO22", "CONTENT", 0 },
	{ 920007031, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO21", "CONTENT", 0 },
	{ 920007032, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CLASSD_OCP", "CONTENT", 0 },
	{ 920007036, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO32", "CONTENT", 0 },
	{ 920007037, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO31", "CONTENT", 0 },
	{ 920007038, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO30", "CONTENT", 0 },
	{ 920007039, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO29", "CONTENT", 0 },
	{ 920007040, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK4_SCP", "CONTENT", 0 },
	{ 920007041, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK3_SCP", "CONTENT", 0 },
	{ 920007042, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK2_SCP", "CONTENT", 0 },
	{ 920007043, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK1_SCP", "CONTENT", 0 },
	{ 920007044, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK0_SCP", "CONTENT", 0 },
	{ 920007045, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK7", "CONTENT", 0 },
	{ 920007046, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK6", "CONTENT", 0 },
	{ 920007047, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK5", "CONTENT", 0 },
	{ 920007048, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK8", "CONTENT", 0 },
	{ 920007049, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO6", "CONTENT", 0 },
	{ 920007050, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO33", "CONTENT", 0 },
	{ 920007051, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO34", "CONTENT", 0 },
	{ 920007052, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO35", "CONTENT", 0 },
	{ 920007053, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO36", "CONTENT", 0 },
	{ 920007054, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK7_SCP", "CONTENT", 0 },
	{ 920007055, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK6_SCP", "CONTENT", 0 },
	{ 920007056, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK5_SCP", "CONTENT", 0 },
	{ 920007057, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK8_SCP", "CONTENT", 0 },
	{ 920007058, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_LDOBUFF_SCP", "CONTENT", 0 },
	{ 920007059, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO18", "CONTENT", 0 },
	{ 920007060, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPPMUH", "CONTENT", 0 },
	{ 920007061, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO37", "CONTENT", 0 },
	{ 920007062, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK00", "CONTENT", 0 },
	{ 920007063, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK01", "CONTENT", 0 },
	{ 920007064, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK02", "CONTENT", 0 },
	{ 920007065, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK9", "CONTENT", 0 },
	{ 920007066, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO41", "CONTENT", 0 },
	{ 920007067, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO40", "CONTENT", 0 },
	{ 920007068, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO39", "CONTENT", 0 },
	{ 920007069, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO38", "CONTENT", 0 },
	{ 920007070, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK00_SCP", "CONTENT", 0 },
	{ 920007071, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK02_SCP", "CONTENT", 0 },
	{ 920007072, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK9_SCP", "CONTENT", 0 },
	{ 920007073, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPACR", "CONTENT", 0 },
	{ 920007074, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK10", "CONTENT", 0 },
	{ 920007075, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_BUCK10_SCP", "CONTENT", 0 },
	{ 920007076, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "LGA_CHECK", "CONTENT", 0 },
	{ 920007077, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK40", "CONTENT", 0 },
	{ 920007078, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK41", "CONTENT", 0 },
	{ 920007079, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK42", "CONTENT", 0 },
	{ 920007080, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK43", "CONTENT", 0 },
	{ 920007081, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK44", "CONTENT", 0 },
	{ 920007082, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK40", "CONTENT", 0 },
	{ 920007083, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK41", "CONTENT", 0 },
	{ 920007084, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK42", "CONTENT", 0 },
	{ 920007085, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK43", "CONTENT", 0 },
	{ 920007086, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK44", "CONTENT", 0 },
	{ 920007087, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK40", "CONTENT", 0 },
	{ 920007088, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK41", "CONTENT", 0 },
	{ 920007089, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK42", "CONTENT", 0 },
	{ 920007090, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK43", "CONTENT", 0 },
	{ 920007091, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK44", "CONTENT", 0 },
	{ 920007092, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK40", "CONTENT", 0 },
	{ 920007093, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK41", "CONTENT", 0 },
	{ 920007094, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK42", "CONTENT", 0 },
	{ 920007095, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO42", "CONTENT", 0 },
	{ 920007096, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO43", "CONTENT", 0 },
	{ 920007097, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO44", "CONTENT", 0 },
	{ 920007098, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO45", "CONTENT", 0 },
	{ 920007099, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO46", "CONTENT", 0 },
	{ 920007100, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO47", "CONTENT", 0 },
	{ 920007101, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO48", "CONTENT", 0 },
	{ 920007102, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO49", "CONTENT", 0 },
	{ 920007103, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO50", "CONTENT", 0 },
	{ 920007104, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPSW1", "CONTENT", 0 },
	{ 920007105, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_VSYSSURGE", "CONTENT", 0 },
	{ 920007106, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OTMP125", "CONTENT", 0 },
	{ 920007107, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK11", "CONTENT", 0 },
	{ 920007108, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK20", "CONTENT", 0 },
	{ 920007109, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK21", "CONTENT", 0 },
	{ 920007110, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK30", "CONTENT", 0 },
	{ 920007111, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK31", "CONTENT", 0 },
	{ 920007112, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCKBOOST", "CONTENT", 0 },
	{ 920007113, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK12", "CONTENT", 0 },
	{ 920007114, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK13", "CONTENT", 0 },
	{ 920007115, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK20", "CONTENT", 0 },
	{ 920007116, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK30", "CONTENT", 0 },
	{ 920007117, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCKBOOST", "CONTENT", 0 },
	{ 920007118, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK12", "CONTENT", 0 },
	{ 920007119, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK13", "CONTENT", 0 },
	{ 920007120, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCKBOOST", "CONTENT", 0 },
	{ 920007121, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK12", "CONTENT", 0 },
	{ 920007122, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK20", "CONTENT", 0 },
	{ 920007123, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK30", "CONTENT", 0 },
	{ 920007124, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK12", "CONTENT", 0 },
	{ 920007125, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK00", "CONTENT", 0 },
	{ 920007126, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK10", "CONTENT", 0 },
	{ 920007127, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK20", "CONTENT", 0 },
	{ 920007128, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_CURBUCK30", "CONTENT", 0 },
	{ 920007129, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPSUBPMUH", "CONTENT", 0 },
	{ 920007130, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPSW10", "CONTENT", 0 },
	{ 920007131, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO51", "CONTENT", 0 },
	{ 920007132, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO52", "CONTENT", 0 },
	{ 920007133, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO53", "CONTENT", 0 },
	{ 920007134, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPLDO54", "CONTENT", 0 },
	{ 920007135, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OTMP125SUBPMU", "CONTENT", 0 },
	{ 920007136, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK8", "CONTENT", 0 },
	{ 920007137, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK00", "CONTENT", 0 },
	{ 920007138, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK10", "CONTENT", 0 },
	{ 920007139, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OTMP140SUBPMU", "CONTENT", 0 },
	{ 920007140, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK70", "CONTENT", 0 },
	{ 920007141, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPBUCK71", "CONTENT", 0 },
	{ 920007142, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OCPSINK", "CONTENT", 0 },
	{ 920007143, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK70", "CONTENT", 0 },
	{ 920007144, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPBUCK71", "CONTENT", 0 },
	{ 920007145, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SCPSINK", "CONTENT", 0 },
	{ 920007146, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK70", "CONTENT", 0 },
	{ 920007147, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_OVPBUCK71", "CONTENT", 0 },
	{ 925206406, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU0_ERROR", "CONTENT", 0 },
	{ 925206407, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU0_WARNING", "CONTENT", 0 },
	{ 925206408, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU0_INFO", "CONTENT", 0 },
	{ 925206409, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU1_ERROR", "CONTENT", 0 },
	{ 925206410, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU1_WARNING", "CONTENT", 0 },
	{ 925206411, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU1_INFO", "CONTENT", 0 },
	{ 925206412, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU2_ERROR", "CONTENT", 0 },
	{ 925206413, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU2_WARNING", "CONTENT", 0 },
	{ 925206414, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU2_INFO", "CONTENT", 0 },
	{ 925206415, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU3_ERROR", "CONTENT", 0 },
	{ 925206416, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU3_WARNING", "CONTENT", 0 },
	{ 925206417, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU3_INFO", "CONTENT", 0 },
	{ 925206418, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU4_ERROR", "CONTENT", 0 },
	{ 925206419, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU4_WARNING", "CONTENT", 0 },
	{ 925206420, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU4_INFO", "CONTENT", 0 },
	{ 925206421, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU5_ERROR", "CONTENT", 0 },
	{ 925206422, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU5_WARNING", "CONTENT", 0 },
	{ 925206423, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU5_INFO", "CONTENT", 0 },
	{ 925206424, POWER_DSM_LEVEL_FAULT, "PMU_OCP", "PMU_SC_DCM", "CONTENT", 0 },
};
#endif

static unsigned int pmic_mntn_reg_read(
	struct pmic_mntn_desc *pmic_mntn, int addr)
{
	unsigned int val = 0;
	int ret = 0;

	if (pmic_mntn == NULL || pmic_mntn->dev == NULL)
		return 0;

	if (pmic_mntn->regmap) {
		ret = regmap_read(pmic_mntn->regmap, addr, &val);
		if (ret < 0) {
			pr_err("%s addr 0x%x failed\n", __func__, addr);
			return 0;
		}
		return val;
	}

	return 0;
}

static void pmic_mntn_reg_write(
	struct pmic_mntn_desc *pmic_mntn, int addr, int val)
{
	int ret;

	if (pmic_mntn == NULL || pmic_mntn->dev == NULL)
		return;

	if (pmic_mntn->regmap) {
		ret = regmap_write(pmic_mntn->regmap, addr, val);
		if (ret < 0)
			pr_err("%s regmap write 0x%x failed\n", __func__, addr);

		return;
	}

	return;
}

int zodiac_pmic_set_cold_reset(unsigned char status)
{
	unsigned char reg_value;
	struct pmic_mntn_desc *pmic_mntn = g_pmic_mntn[PMU0];

	if (pmic_mntn == NULL) {
		pr_err("%s:pmic mntn is null\n", __func__);
		return -EPERM;
	}

	reg_value = pmic_mntn_reg_read(
		pmic_mntn, pmic_mntn->otmp_hreset_pwrdown_reg.addr);
	pr_err("%s:Into:HRESET_PWRDOWN_CTRL register val is 0x%x!\n",
		__func__, reg_value);

	if ((status == PMIC_HRESET_COLD) || (status == PMIC_HRESET_HOT))
		pmic_write_reg(pmic_mntn->otmp_hreset_pwrdown_reg.addr,
			status);

	reg_value = pmic_mntn_reg_read(
		pmic_mntn, pmic_mntn->otmp_hreset_pwrdown_reg.addr);
	pr_err("%s:Out:HRESET_PWRDOWN_CTRL register val is 0x%x!\n",
		__func__, reg_value);

	return 0;
}

static BLOCKING_NOTIFIER_HEAD(pmic_mntn_notifier_list);
int pmic_mntn_register_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(
		&pmic_mntn_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(pmic_mntn_register_notifier);

int pmic_mntn_unregister_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(
		&pmic_mntn_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(pmic_mntn_unregister_notifier);

int pmic_mntn_call_notifiers(int val, void *v)
{
	return blocking_notifier_call_chain(
		&pmic_mntn_notifier_list, (unsigned long)val, v);
}
EXPORT_SYMBOL_GPL(pmic_mntn_call_notifiers);

#ifdef CONFIG_PMIC_PLATFORM_DEBUG
static int pmic_mntn_test_notifier_call(
	struct notifier_block *nb, unsigned long event, void *data)
{
	PMIC_MNTN_EXCP_INFO *ocp_ldo_msg = (PMIC_MNTN_EXCP_INFO *)data;

	if (ocp_ldo_msg == NULL) {
		pr_err("[%s] test pmic mnt ocp ldo msg is NULL!\n", __func__);
		return -EPERM;
	}

	if (event == PMIC_MNTN_OCP_EVENT)
		pr_err("[%s] test pmic mnt %s ocp event!\n",
			__func__, ocp_ldo_msg->ldo_num);
	else
		pr_err("[%s]invalid event %d!\n", __func__, (int)event);
	return 0;
}

static struct notifier_block pmic_mntn_test_nb = {
	.notifier_call = pmic_mntn_test_notifier_call,
	.priority = INT_MIN,
};
#endif

void pmic_mntn_panic_handler(void)
{
#ifdef CONFIG_DFX_BB
	rdr_syserr_process_for_ap(MODID_AP_S_PMU, 0, 0);
#else
	machine_restart("AP_S_PMU");
#endif
}

static struct pmic_mntn_desc *_find_pmic_by_power_search_for(
	const char *power_name, struct pmic_mntn_desc *pmic_mntn)
{
	struct pmic_exch_reg *exch_desc = NULL;
	unsigned int index, bit, reg_num;
	char *bit_name = NULL;
	int len;

	exch_desc = pmic_mntn->ocp_exch_desc;
	reg_num = pmic_mntn->ocp_reg_n;
	for (index = 0; index < reg_num; index++) {
		for (bit = 0; bit < pmic_mntn->data_width; bit++) {
			bit_name = exch_desc[index].event_bit_name[bit];
			len = (strlen(bit_name)) > (strlen(power_name)) ?
				(strlen(bit_name)) : (strlen(power_name));
			if (!strncmp(bit_name, power_name, len))
				return pmic_mntn;
		}
	}
	return NULL;
}

static struct pmic_mntn_desc *_find_pmic_by_power(const char *power_name)
{
	struct pmic_mntn_desc *pmic_mntn = NULL;
	struct pmic_mntn_desc *pmic_mntn_match = NULL;
	int pmu;

	if (power_name == NULL) {
		pr_err("[%s]power_name is null\n", __func__);
		return NULL;
	}

	for (pmu = 0; pmu < PMU_NUM; pmu++) {
		pmic_mntn = g_pmic_mntn[pmu];
		if (pmic_mntn == NULL)
			continue;

		pmic_mntn_match =
			_find_pmic_by_power_search_for(power_name, pmic_mntn);
		if (pmic_mntn_match != NULL)
			return pmic_mntn_match;
	}
	return NULL;
}

int pmic_special_ocp_register(
	char *power_name, pmic_ocp_callback handler)
{
	struct pmic_mntn_desc *pmic_mntn = NULL;
	struct special_ocp_attr *head = NULL;
	struct special_ocp_attr *cur = NULL;

	if (power_name == NULL) {
		pr_err("[%s]power_name is null\n", __func__);
		return -EPERM;
	}
	if (!handler) {
		pr_err("[%s]handler is null\n", __func__);
		return -EPERM;
	}

	/* find pmic info */
	pmic_mntn = _find_pmic_by_power((const char *)power_name);
	if (pmic_mntn == NULL) {
		pr_err("[%s]error, power name %s\n", __func__, power_name);
		return -EPERM;
	}

	head = pmic_mntn->ocp_list_head;
	cur = pmic_mntn->ocp_list_tail;
	if (head == NULL) {
		head = kzalloc(sizeof(*head), GFP_KERNEL);
		if (head == NULL)
			return -ENOMEM;

		cur = head;
		pmic_mntn->ocp_list_head = head;
	} else {
		cur->next = kzalloc(sizeof(*cur), GFP_KERNEL);
		if (cur->next == NULL)
			return -ENOMEM;

		cur = cur->next;
	}
	pmic_mntn->ocp_list_tail = cur;

	cur->power_name = power_name;
	cur->handler = handler;

	pr_info("[%s]%s registered\n", __func__, power_name);

	return 0;
}
EXPORT_SYMBOL(pmic_special_ocp_register);
#ifndef CONFIG_AB_APCP_NEW_INTERFACE
int hisi_pmic_special_ocp_register(
	char *power_name, pmic_ocp_callback handler)
{
	return pmic_special_ocp_register(power_name,handler); // for modem
}
EXPORT_SYMBOL(hisi_pmic_special_ocp_register);

int hisi_pmic_mntn_register_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(
		&pmic_mntn_notifier_list, nb);
}  // for modem
EXPORT_SYMBOL_GPL(hisi_pmic_mntn_register_notifier);
#endif
static int pmic_mntn_ocp_special_handler(
	char *power_name, struct pmic_mntn_desc *pmic_mntn)
{
	struct special_ocp_attr *cur = pmic_mntn->ocp_list_head;
	int ret;

	if (cur == NULL) {
		pr_err("[%s]no power registered\n", __func__);
		return PMIC_OCP_NONE;
	}

	do {
		if (!strncmp(cur->power_name, power_name,
			    ((strlen(cur->power_name)) > (strlen(power_name)) ?
					    (strlen(cur->power_name)) :
					    (strlen(power_name))))) {
			pr_err("%s ocp special handler\n", power_name);
			ret = cur->handler(power_name);
			if (ret)
				return PMIC_OCP_NONE;
			else
				return PMIC_OCP_HANDLED;
		}
		cur = cur->next;
	} while (cur != NULL);

	return PMIC_OCP_NONE;
}

static void get_pmu_key_register_info(struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int i;

	for (i = 0; i < pmic_mntn->pmu_record_reg_n; i++)
		pr_err("[%s]addr[0x%x] = value[0x%x]\n", __func__,
			pmic_mntn->pmu_record_regs[i],
			pmic_mntn_reg_read(
				pmic_mntn, pmic_mntn->pmu_record_regs[i]));
}

#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
static void pmic_mntn_ocp_dsm_client(const char *bit_name, int dmd_offset)
{
	if (dmd_offset >= PMIC_DSM_EXP_NUM)
		power_dsm_report_format_dmd(POWER_DSM_PMU_OCP, DSM_PMU_EXP - PMIC_DSM_EXP_NUM + dmd_offset,
			"pmic %s happened, please pay attention!\n", bit_name);
	else
		power_dsm_report_format_dmd(POWER_DSM_PMU_OCP, DSM_PMU_OCP_ERROR_NO_BASE + dmd_offset,
			"pmic %s_ocp happened, please pay attention!\n", bit_name);

	pr_err("pmic %s happened, dmd is %d\n", bit_name, dmd_offset);
}

static void pmic_mntn_ocp_dsm_report(struct pmic_mntn_desc *pmic_mntn,
	unsigned int index, unsigned int bit, unsigned int reset)
{
	char *bit_name = NULL;
	struct pmic_exch_reg *exch_desc = NULL;
	int offset = pmic_mntn->data_width * index + bit;

	if (reset)
		exch_desc = pmic_mntn->record_exch_desc;
	else
		exch_desc = pmic_mntn->ocp_exch_desc;

	bit_name = exch_desc[index].event_bit_name[bit];
	if ((pmic_mntn->dsm_record_regs_mask[index] & BIT(bit)) &&
		(reset == (!!(pmic_mntn->dsm_ocp_reset_mask[index] &
				  BIT(bit)))) &&
		((unsigned int)offset < pmic_mntn->ocp_error_dmd_offset_n) &&
		(pmic_mntn->ocp_error_dmd_offset[offset] !=
			PMIC_DSM_IGNORE_NUM))
		pmic_mntn_ocp_dsm_client(
			bit_name, pmic_mntn->ocp_error_dmd_offset[offset]);
}

static int pmic_mntn_record_mntn_dsm_data_get(
	struct pmic_mntn_desc *pmic_mntn)
{
	s32 ret;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;

	pmic_mntn->dsm_record_regs_mask = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->record_reg_n, GFP_KERNEL);
	if (pmic_mntn->dsm_record_regs_mask == NULL)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "pmic-mnt-dsm-record-regs-mask",
		pmic_mntn->dsm_record_regs_mask, pmic_mntn->record_reg_n);
	if (ret) {
		dev_err(dev, "[%s]get dsm-record-regs-mask attribute failed\n",
			__func__);
		return -ENODEV;
	}

	pmic_mntn->dsm_ocp_reset_mask = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->record_reg_n, GFP_KERNEL);
	if (pmic_mntn->dsm_ocp_reset_mask == NULL)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "pmic-mnt-dsm-ocp-reset-mask",
		pmic_mntn->dsm_ocp_reset_mask, pmic_mntn->record_reg_n);
	if (ret) {
		dev_err(dev, "[%s]get dsm-ocp-reset-mask attribute failed\n",
			__func__);
		return -ENODEV;
	}
	ret = of_property_read_u32(np, "pmic-mnt-ocp-error-dmd-offset-number",
		(u32 *)&pmic_mntn->ocp_error_dmd_offset_n);
	if (ret) {
		dev_err(dev, "[%s]get ocp-error-dmd-offset-number attribute failed\n",
			__func__);
		return -ENODEV;
	}
	pmic_mntn->ocp_error_dmd_offset = (u32 *)devm_kzalloc(dev,
		sizeof(u32) * pmic_mntn->ocp_error_dmd_offset_n, GFP_KERNEL);
	if (pmic_mntn->ocp_error_dmd_offset == NULL)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "pmic-mnt-ocp-error-dmd-offset",
		pmic_mntn->ocp_error_dmd_offset,
		pmic_mntn->ocp_error_dmd_offset_n);
	if (ret) {
		dev_err(dev, "[%s]get ocp-error-dmd-offset attribute failed\n",
			__func__);
		return -ENODEV;
	}

	return 0;
}
#endif

static void pmic_mntn_ocp_normal_handler(struct pmic_mntn_desc *pmic_mntn,
	unsigned int care_bit, unsigned int index, unsigned int bit)
{
	char *bit_name = NULL;
	unsigned int bit_reg;
	struct pmic_exch_reg *exch_desc = pmic_mntn->ocp_exch_desc;
	PMIC_MNTN_EXCP_INFO ocp_ldo_msg;
	errno_t err;

	bit_name = exch_desc[index].event_bit_name[bit];
	bit_reg = exch_desc[index].event_ops_reg[bit];

	err = memset_s(
		&ocp_ldo_msg, sizeof(ocp_ldo_msg), 0, sizeof(ocp_ldo_msg));
	if (err != EOK) {
		pr_err("[%s]memset_s fail, err= %d\n", __func__, err);
		return;
	}

	if (exch_desc[index].check_count[bit] <
		exch_desc[index].check_ocp_num[bit]) {
		exch_desc[index].check_count[bit]++;
		/* open ldo */
		if (bit_reg != INVALID_REG_ADDR) {
			pmic_mntn_reg_write(
				pmic_mntn, bit_reg, OPEN_REGULATOR_CONTROL);
			/* clear record event */
			pmic_mntn_reg_write(pmic_mntn,
				pmic_mntn->record_regs[index],
				pmic_mntn->record_event_buff[index]);
		}
	} else if (care_bit & exch_desc[index].check_ocp_nofify) {
		exch_desc[index].check_count[bit] = 0;

		/* notify the terminal exception handling system */
		err = strncpy_s(ocp_ldo_msg.ldo_num, PMIC_OCP_LDO_NAME,
			bit_name, strlen(bit_name));
		if (err != EOK) {
			pr_err("[%s]strncpy_s fail, err= %d\n", __func__, err);
			return;
		}
		pmic_mntn_call_notifiers(
			PMIC_MNTN_OCP_EVENT, (void *)&ocp_ldo_msg);
	} else if (care_bit & exch_desc[index].check_ocp_reset) {
		pmic_mntn->health |= BIT(PMIC_HEALTH_OCP_EXCH_HAPPENED);
	} else {
		pr_err("%s: %s has been opened %u tiems,the time do nothing!\n",
			__func__, bit_name, exch_desc[index].check_count[bit]);
	}
}

static void pmic_mntn_ocp_event_proc(struct pmic_mntn_desc *pmic_mntn,
	unsigned int care_bit, unsigned int index, unsigned int bit)
{
	char *bit_name = NULL;
	char *cur_str = NULL;
	unsigned int bit_reg;
	struct pmic_exch_reg *exch_desc = pmic_mntn->ocp_exch_desc;
	int ret;

	bit_name = exch_desc[index].event_bit_name[bit];
	bit_reg = exch_desc[index].event_ops_reg[bit];
	cur_str = pmic_mntn->irq_log_show;

	(void)snprintf_s(cur_str + strlen(cur_str), PMIC_PRINT_BUF_SIZE,
		PMIC_PRINT_BUF_SIZE - 1, "PMU EVENT TYPE:ocp_%s\n", bit_name);
	get_pmu_key_register_info(pmic_mntn);
	/* PMU irq event is 0, PMU reset event is 1 */
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	pmic_mntn_ocp_dsm_report(pmic_mntn, index, bit, 0);
#endif
	/* close ldo */
	if (bit_reg != INVALID_REG_ADDR)
		pmic_mntn_reg_write(
			pmic_mntn, bit_reg, CLOSE_REGULATOR_CONTROL);

	/* clear interrupt */
	pmic_mntn_reg_write(
		pmic_mntn, pmic_mntn->ocp_regs[index], care_bit);
	ret = pmic_mntn_ocp_special_handler(bit_name, pmic_mntn);
	if (ret == PMIC_OCP_HANDLED)
		return;

	if (care_bit & exch_desc[index].board_reboot_event) {
		pmic_mntn->health |=
				BIT(PMIC_HEALTH_OCP_EXCH_HAPPENED);
	} else if (unlikely(strstr(saved_command_line, "ohos.boot.swtype=factory") ||
		strstr(saved_command_line, "androidboot.swtype=factory"))) {
		if (care_bit & exch_desc[index].inacceptable_event)
			pmic_mntn->health |=
				BIT(PMIC_HEALTH_OCP_EXCH_HAPPENED);
	} else {
		pmic_mntn_ocp_normal_handler(pmic_mntn, care_bit, index, bit);
	}
}

static void pmic_mntn_ocp_scan(struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int index, bit;
	int *reg_buff = NULL;
	unsigned int reg_num;
	unsigned int care_bit;

	reg_buff = pmic_mntn->ocp_event_buff;
	reg_num = pmic_mntn->ocp_reg_n;
	for (index = 0; index < reg_num; index++) {
		if (reg_buff[index] == 0)
			continue;
		pr_err("[%s]reg = 0x%x, value = 0x%x\n", __func__,
			pmic_mntn->ocp_regs[index], reg_buff[index]);
		for (bit = 0; bit < pmic_mntn->data_width; bit++) {
			care_bit = ((unsigned int)reg_buff[index]) & BIT(bit);
			if (care_bit)
				pmic_mntn_ocp_event_proc(
					pmic_mntn, care_bit, index, bit);
		}
	}
}

static void pmic_mntn_record_events(struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int index;
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	unsigned int i;
#endif
	int len = PMIC_PRINT_BUF_SIZE;

	(void)snprintf_s(pmic_mntn->init_log_show, PMIC_PRINT_BUF_SIZE,
		PMIC_PRINT_BUF_SIZE- 1, "\nPMIC REGISTER DUMP\n");
	len -= strlen(pmic_mntn->init_log_show);
	(void)snprintf_s(pmic_mntn->init_log_show + strlen(pmic_mntn->init_log_show),
		len, len - 1, "  addr     data\n");

	for (index = 0; index < pmic_mntn->record_reg_n; index++) {
		pmic_mntn->record_event_buff[index] = pmic_mntn_reg_read(
			pmic_mntn, pmic_mntn->record_regs[index]);

		pmic_mntn_reg_write(pmic_mntn,
			pmic_mntn->record_regs[index],
			pmic_mntn->record_event_buff[index]);
		len = PMIC_PRINT_BUF_SIZE - strlen(pmic_mntn->init_log_show);
		(void)snprintf_s(pmic_mntn->init_log_show +
			strlen(pmic_mntn->init_log_show),
			len,
			len - 1,
			"  0x%x    0x%x\n", pmic_mntn->record_regs[index],
			pmic_mntn->record_event_buff[index]);
		if ((pmic_mntn->record_event_buff[index] &
			    PMIC_DSM_MASK_STATE) == 0)
			continue;
	}

	pr_err("%s", pmic_mntn->init_log_show);

#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	queue_delayed_work(system_power_efficient_wq,
			&pmic_mntn->record_dmd_wk,
			msecs_to_jiffies(RECORD_DMD_DELAYED_TIME)); /* default 1min (60000ms) */
#endif
}

#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
static void pmic_mntn_record_dsm_report(struct work_struct *work) {
	unsigned int index, i;
	struct pmic_mntn_desc *pmic_mntn = container_of(work,
		struct pmic_mntn_desc, record_dmd_wk.work);
	for (index = 0; index < pmic_mntn->record_reg_n; index++)
		for (i = 0; i < pmic_mntn->data_width; i++) {
			if (pmic_mntn->record_event_buff[index] & BIT(i))
				/* PMU irq event is 0, PMU reset event is 1 */
				pmic_mntn_ocp_dsm_report(
					pmic_mntn, index, i, 1);
		}
}
#endif

static void pmic_mntn_clear_ocp(struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int index;

	for (index = 0; index < pmic_mntn->ocp_reg_n; index++) {
		pmic_mntn->ocp_event_buff[index] = pmic_mntn_reg_read(
			pmic_mntn, pmic_mntn->ocp_regs[index]);
		pmic_mntn_reg_write(pmic_mntn, pmic_mntn->ocp_regs[index],
			pmic_mntn->ocp_event_buff[index]);
	}

	memset_s((void *)pmic_mntn->ocp_event_buff, sizeof(u32) * pmic_mntn->ocp_reg_n,
	 0, sizeof(u32) * pmic_mntn->ocp_reg_n);
}

static irqreturn_t pmic_mntn_ocp_irq_handler(
	int irq, void *data)
{
	struct irq_desc *desc = NULL;
	struct pmic_mntn_desc *pmic_mntn = (struct pmic_mntn_desc *)data;

	dev_err(pmic_mntn->dev, "[%s] +\n", __func__);

	desc = irq_to_desc(irq);
	if (desc == NULL) {
		pr_err("[%s]irq_to_desc failed\n", __func__);
		return IRQ_NONE;
	}

	if (desc->irq_data.chip->irq_mask != NULL)
		desc->irq_data.chip->irq_mask(&desc->irq_data);

	if (pmic_mntn->need_mask_ocp_irq != 0)
		pmic_mntn_reg_write(pmic_mntn, PMU_IRQ_MASK_OVP_IRQ, OVP_IRQ_MASK);

	queue_work(system_highpri_wq, &pmic_mntn->ocp_wk);

	dev_err(pmic_mntn->dev, "[%s] -\n", __func__);
	return IRQ_HANDLED;
}

static void pmic_mntn_ocp_wq_handler(struct work_struct *work)
{
	unsigned int index;
	struct irq_desc *desc = NULL;
	struct pmic_mntn_desc *pmic_mntn = NULL;

	pmic_mntn = container_of(work, struct pmic_mntn_desc, ocp_wk);

	dev_err(pmic_mntn->dev, "[%s] +\n", __func__);

	memset_s((void *)pmic_mntn->irq_log_show, PMIC_PRINT_BUF_SIZE,
		'\0', PMIC_PRINT_BUF_SIZE);

	for (index = 0; index < pmic_mntn->ocp_reg_n; index++) {
		pmic_mntn->ocp_event_buff[index] = pmic_mntn_reg_read(
			pmic_mntn, pmic_mntn->ocp_regs[index]);
		pmic_mntn->record_event_buff[index] = pmic_mntn_reg_read(
			pmic_mntn, pmic_mntn->record_regs[index]);
	}

	pmic_mntn_ocp_scan(pmic_mntn);
	dev_err(pmic_mntn->dev, "\n%s\n", pmic_mntn->irq_log_show);

	desc = irq_to_desc(pmic_mntn->ocp_irq);
	if (desc == NULL) {
		pr_err("[%s]irq_to_desc(pmic_mntn->ocp_irq) error\n", __func__);
		return;
	}

	if (desc->irq_data.chip->irq_unmask != NULL)
		desc->irq_data.chip->irq_unmask(&desc->irq_data);

	if (pmic_mntn->need_mask_ocp_irq != 0)
		pmic_mntn_reg_write(pmic_mntn, PMU_IRQ_MASK_OVP_IRQ, OVP_IRQ_UNMASK);

	if (pmic_mntn->health & BIT(PMIC_HEALTH_OCP_EXCH_HAPPENED))
		pmic_mntn_panic_handler();

	dev_err(pmic_mntn->dev, "[%s] -\n", __func__);
}

static void pmic_mntn_otmp_event_proc(struct pmic_mntn_desc *pmic_mntn,
	unsigned int care_bit, unsigned int index, unsigned int bit)
{
	char *bit_name = NULL;
	char *cur_str = NULL;
	unsigned int bit_reg;
	struct pmic_exch_reg *exch_desc = pmic_mntn->otmp_exch_desc;
	int ret;

	bit_name = exch_desc[index].event_bit_name[bit];
	cur_str = pmic_mntn->irq_log_show;

	/* clear interrupt */
	pmic_mntn_reg_write(
		pmic_mntn, pmic_mntn->otmp_regs[index], care_bit);

	if (care_bit & exch_desc[index].inacceptable_event)
		(void)snprintf_s(cur_str + strlen(cur_str), PMIC_PRINT_BUF_SIZE,
			PMIC_PRINT_BUF_SIZE - 1, "PMU EVENT TYPE:otmp_%s\n", bit_name);
}

static void pmic_mntn_otmp_scan(struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int index, bit;
	int *reg_buff = NULL;
	unsigned int reg_num;
	unsigned int care_bit;

	reg_buff = pmic_mntn->otmp_event_buff;
	reg_num = pmic_mntn->otmp_reg_n;
	for (index = 0; index < reg_num; index++) {
		if (reg_buff[index] == 0)
			continue;
		pr_err("[%s]reg = 0x%x, value = 0x%x\n", __func__,
			pmic_mntn->otmp_regs[index], reg_buff[index]);
		for (bit = 0; bit < pmic_mntn->data_width; bit++) {
			care_bit = ((unsigned int)reg_buff[index]) & BIT(bit);
			if (care_bit)
				pmic_mntn_otmp_event_proc(
					pmic_mntn, care_bit, index, bit);
		}
	}
}

static void pmic_mntn_clear_otmp(struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int index;

	for (index = 0; index < pmic_mntn->otmp_reg_n; index++) {
		pmic_mntn->otmp_event_buff[index] = pmic_mntn_reg_read(
			pmic_mntn, pmic_mntn->otmp_regs[index]);
		pmic_mntn_reg_write(pmic_mntn, pmic_mntn->otmp_regs[index],
			pmic_mntn->otmp_event_buff[index]);
	}

	memset_s((void *)pmic_mntn->otmp_event_buff, sizeof(u32) * pmic_mntn->otmp_reg_n,
		0, sizeof(u32) * pmic_mntn->otmp_reg_n);
}

static irqreturn_t pmic_mntn_otmp_irq_handler(
	int irq, void *data)
{
	struct irq_desc *desc = NULL;
	struct pmic_mntn_desc *pmic_mntn = (struct pmic_mntn_desc *)data;

	desc = irq_to_desc(irq);
	if (desc == NULL) {
		pr_err("[%s]irq_to_desc failed\n", __func__);
		return IRQ_NONE;
	}

	if (desc->irq_data.chip->irq_mask != NULL)
		desc->irq_data.chip->irq_mask(&desc->irq_data);

	queue_work(system_highpri_wq, &pmic_mntn->otmp_wk);
	return IRQ_HANDLED;
}

static void pmic_mntn_otmp_wk_handler(struct work_struct *work)
{
	struct irq_desc *desc = NULL;
	struct pmic_mntn_desc *pmic_mntn = NULL;
	unsigned char reg_value;
	int index;

	pmic_mntn = container_of(work, struct pmic_mntn_desc, otmp_wk);

	memset_s((void *)pmic_mntn->irq_log_show, PMIC_PRINT_BUF_SIZE,
		'\0', PMIC_PRINT_BUF_SIZE);
	(void)snprintf_s(pmic_mntn->irq_log_show, PMIC_PRINT_BUF_SIZE,
			PMIC_PRINT_BUF_SIZE - 1, "%s", "PMIC OTMP EVENT HAPPEN.\n");

	if (pmic_mntn->otmp_reg_n == 0) {
		pr_err("[%s]otmp_reg_n is %d\n", __func__, pmic_mntn->otmp_reg_n);
		goto no_otmp_regs;
	}

	for (index = 0; index < pmic_mntn->otmp_reg_n; index++)
		pmic_mntn->otmp_event_buff[index] = pmic_mntn_reg_read(
			pmic_mntn, pmic_mntn->otmp_regs[index]);

	pmic_mntn_otmp_scan(pmic_mntn);

no_otmp_regs:
	pr_info("\n%s\n", pmic_mntn->irq_log_show);

	desc = irq_to_desc(pmic_mntn->otmp_irq);
	if (desc == NULL) {
		pr_err("[%s]irq_to_desc err\n", __func__);
		return;
	}

	if (desc->irq_data.chip->irq_unmask != NULL)
		desc->irq_data.chip->irq_unmask(&desc->irq_data);

	/*
	 * hreset powerdown ctrl,
	 * if otmp_hreset_pwrdown_flag is not 0, do cold reset
	 */
	if (pmic_mntn->otmp_hreset_pwrdown_flag != 0) {
		reg_value = pmic_mntn_reg_read(
			pmic_mntn, pmic_mntn->otmp_hreset_pwrdown_reg.addr);
		set_reg_bit(reg_value, pmic_mntn->otmp_hreset_pwrdown_reg.shift,
			pmic_mntn->otmp_hreset_pwrdown_reg.mask,
			pmic_mntn->otmp_hreset_pwrdown_val);
		pmic_mntn_reg_write(pmic_mntn,
			pmic_mntn->otmp_hreset_pwrdown_reg.addr, reg_value);
	}
	pmic_mntn_panic_handler();
}

#if defined CONFIG_HUAWEI_VSYS_PWROFF_ABS_PD
int pmic_mntn_config_smpl(bool enable)
{
	unsigned int reg_value;
	unsigned int value;

	if (g_pmic_mntn[PMU0] == NULL)
		return 1;

	if (enable) {
		value = 1;
		pr_info("%s %d enable smpl in kernel\n", __func__, __LINE__);
	} else {
		value = 0;
		pr_info("%s %d disable smpl in kernel\n", __func__, __LINE__);
	}

	reg_value = pmic_read_reg(g_pmic_mntn[PMU0]->smpl_en_reg.addr);
	set_reg_bit(reg_value, g_pmic_mntn[PMU0]->smpl_en_reg.shift,
		g_pmic_mntn[PMU0]->smpl_en_reg.mask, value);
	pmic_write_reg(g_pmic_mntn[PMU0]->smpl_en_reg.addr, reg_value);

	return 0;
}

int pmic_mntn_config_vsys_pwroff_abs_pd(bool enable)
{
	unsigned int reg_value;
	unsigned int value;

	if (g_pmic_mntn[PMU0] == NULL)
		return 1;

	if (enable) {
		value = g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_en_val;
		pr_info("%s %d enable abs_pd in kernel\n", __func__, __LINE__);
	} else {
		value = g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_dis_val;
		pr_info("%s %d disable abs_pd in kernel\n", __func__, __LINE__);
	}

	reg_value = pmic_read_reg(
		g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_tm_reg.addr);
	set_reg_bit(reg_value,
		g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_tm_reg.shift,
		g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_tm_reg.mask, value);
	pmic_reg_write_lock(
		g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_tm_reg.addr, reg_value);

	return 0;
}

int pmic_mntn_vsys_pwroff_abs_pd_state(void)
{
	int ret;
	unsigned int reg_value;

	if (g_pmic_mntn[PMU0] == NULL)
		return -1;

	reg_value = pmic_read_reg(
		g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_tm_reg.addr);
	ret = get_reg_bit(reg_value,
		g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_tm_reg.shift,
		g_pmic_mntn[PMU0]->vsys_pwroff_abs_pd_tm_reg.mask);

	return ret;
}

static int pmic_mntn_vsys_pwroff_abs_pd_mntn_initial(
		struct pmic_mntn_desc *pmic_mntn)
{
	struct device_node *root = NULL;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	s32 ret = 0;

	root = of_find_compatible_node(
		np, NULL, "pmic-vsys-pwroff-abs-pd");
	if (root == NULL) {
		dev_err(dev, "[%s]no vsys-pwroff-abs-pd root node\n",
			__func__);
		return -ENODEV;
	}

	pmic_mntn->vsys_pwroff_abs_pd_en_val = 0;
	pmic_mntn->vsys_pwroff_abs_pd_dis_val = 0;
	memset_s(&pmic_mntn->vsys_pwroff_abs_pd_tm_reg, sizeof(pmic_mntn->vsys_pwroff_abs_pd_tm_reg),
		0, sizeof(pmic_mntn->vsys_pwroff_abs_pd_tm_reg));

	ret |= of_property_read_u32_array(root,
		"pmic-mnt-vsys-pwroff-abs-pd-ctrl-en",
		&pmic_mntn->vsys_pwroff_abs_pd_en_val, 0x1); /* width:1 */
	ret |= of_property_read_u32_array(root,
		"pmic-mnt-vsys-pwroff-abs-pd-ctrl-dis",
		&pmic_mntn->vsys_pwroff_abs_pd_dis_val, 0x1); /* width:1 */
	ret |= of_property_read_u32_array(root,
		"pmic-mnt-vsys-pwroff-abs-pd-ctrl-reg",
		(u32 *)&pmic_mntn->vsys_pwroff_abs_pd_tm_reg, 0x3); /* width:3 */
	if (ret) {
		dev_err(dev,
			"[%s]get pmic vsys-pwroff-abs-pd attribute failed\n",
			__func__);
		return -ENODEV;
	}

	return 0;
}

#else
int pmic_mntn_config_smpl(bool enable)
{
	return 0;
}
int pmic_mntn_config_vsys_pwroff_abs_pd(bool enable)
{
	return 0;
}
int pmic_mntn_vsys_pwroff_abs_pd_state(void)
{
	return 0;
}
static int pmic_mntn_vsys_pwroff_abs_pd_mntn_initial(
		struct pmic_mntn_desc *pmic_mntn)
{
	return 0;
}
#endif /* CONFIG_VSYS_PWROFF_ABS_PD */

static int pmic_mntn_otmp_mntn_spec_get_dts(struct pmic_mntn_desc *pmic_mntn,
	struct device_node *root, struct device *dev, unsigned int index)
{
	s32 ret;
	unsigned int bit;
	struct pmic_exch_reg *exch_reg_tmp = NULL;

	exch_reg_tmp = pmic_mntn->otmp_exch_desc;

	ret = of_property_read_u32(root, "pmic-mnt-inacceptable-event",
		(u32 *)&exch_reg_tmp[index].inacceptable_event);
	if (ret)
		dev_err(dev, "[%s]read attribute of inacceptable_event\n",
			__func__);

	for (bit = 0; bit < pmic_mntn->data_width; bit++)
		ret = ret || of_property_read_string_index(root,
				     "pmic-mnt-event-bit-name", bit,
				     (const char **)&exch_reg_tmp[index]
					     .event_bit_name[bit]);

	return ret;
}

static int pmic_mntn_otmp_mntn_spec_data_init(
	struct pmic_mntn_desc *pmic_mntn)
{
	s32 ret;
	unsigned int index;
	char compatible_string[PMIC_DTS_ATTR_LEN] = {0};
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	struct device_node *root = NULL;
	struct pmic_exch_reg *exch_reg_tmp = NULL;

	exch_reg_tmp = pmic_mntn->otmp_exch_desc;

	for (index = 0; index < pmic_mntn->otmp_reg_n; index++) {
		(void)snprintf_s(compatible_string, PMIC_DTS_ATTR_LEN,
			PMIC_DTS_ATTR_LEN - 1,
			"pmic-mntn-otmp-reg0x%x",
			pmic_mntn->otmp_regs[index]);
		root = of_find_compatible_node(np, NULL, compatible_string);
		if (root == NULL) {
			dev_err(dev, "[%s]no %s root node\n", __func__,
				compatible_string);
			return -ENODEV;
		}

		exch_reg_tmp[index].event_bit_name = (char **)devm_kzalloc(dev,
			pmic_mntn->data_width * sizeof(char *), GFP_KERNEL);
		if (exch_reg_tmp[index].event_bit_name == NULL)
			return -ENOMEM;

		ret = pmic_mntn_otmp_mntn_spec_get_dts(
			pmic_mntn, root, dev, index);
		if (ret) {
			dev_err(dev,
				"[%s]pmic_otmp_mntn_spec_data_get_pro\n",
				__func__);
			return ret;
		}
	}
	return 0;
}

static int pmic_mntn_otmp_mntn_irq_init(
				struct pmic_mntn_desc *pmic_mntn)
{
	struct device *dev = pmic_mntn->dev;
	s32 ret;
	unsigned char reg_value;

	/* Set the otmp threshold */
	reg_value = pmic_mntn_reg_read(
		pmic_mntn, pmic_mntn->otmp_thshd_reg.addr);
	set_reg_bit(reg_value, pmic_mntn->otmp_thshd_reg.shift,
		pmic_mntn->otmp_thshd_reg.mask, pmic_mntn->otmp_thshd_val);
	pmic_mntn_reg_write(
		pmic_mntn, pmic_mntn->otmp_thshd_reg.addr, reg_value);

	pmic_mntn->otmp_irq = of_irq_get_byname(dev->of_node, "otmp");
	if (pmic_mntn->ocp_irq < 0) {
		dev_err(dev, "[%s]spmi_get_irq_byname otmp_irq failed\n",
			__func__);
		return -ENODEV;
	}

	INIT_WORK(&pmic_mntn->otmp_wk, pmic_mntn_otmp_wk_handler);

	ret = devm_request_irq(dev, pmic_mntn->otmp_irq,
		pmic_mntn_otmp_irq_handler, IRQF_NO_SUSPEND, "pmic-otmp-irq",
		(void *)pmic_mntn);
	if (ret) {
		dev_dbg(dev, "[%s] request_irq otmp_irq ret %d\n", __func__,
			ret);
		return -ENODEV;
	}
	return 0;
}

static int pmic_mntn_sysctrl_get(struct pmic_mntn_desc *pmic_mntn)
{
	struct device_node *root = NULL;
	struct device *dev = pmic_mntn->dev;

#ifdef CONFIG_PMIC_26V100_PMU
	root = of_find_compatible_node(NULL, NULL, "hisilicon,awake_crg");
#else
	root = of_find_compatible_node(NULL, NULL, "hisilicon,sysctrl");
#endif
	if (root == NULL) {
		dev_err(dev, "%s: No compatible node found\n",
			__func__);
		return -ENODEV;
	}

	g_sysctrl_base = of_iomap(root, 0);
	if (g_sysctrl_base == NULL) {
		dev_err(dev, "%s: sysctrl_base is NULL\n", __func__);
		return -ENODEV;
	}
	return 0;
}
static int pmic_mntn_otmp_mntn_initial(
	struct pmic_mntn_desc *pmic_mntn)
{
	struct device_node *root = NULL;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	int ret0, ret1, ret2;
	s32 ret;

	root = of_get_child_by_name(np, "OTMP");
	if (root == NULL) {
		dev_err(dev, "[%s]no pmic-mntn-otmp root node\n",
			__func__);
		return -ENODEV;
	}

	dev_info(dev, "[%s]get pmic-mntn-otmp root node\n", __func__);
	ret0 = of_property_read_u32_array(root,
	    "pmic-mnt-otmp-threshold-val",
	    (u32 *)&pmic_mntn->otmp_thshd_val, 0x1); /* width:1 */
	ret1 = of_property_read_u32_array(root,
	    "pmic-mnt-otmp-threshold-reg",
	    (u32 *)&pmic_mntn->otmp_thshd_reg, 0x3); /* width:3 */
	ret = (ret0 || ret1);
	if (ret) {
		dev_err(dev, "[%s]get pmic otmp attribute failed\n", __func__);
		return -ENODEV;
	}

	ret0 = of_property_read_u32_array(root,
	    "pmic-mnt-otmp-hreset-pwrdown-flag",
	    (u32 *)&pmic_mntn->otmp_hreset_pwrdown_flag, 0x1); /* width:1 */
	ret1 = of_property_read_u32_array(root,
	    "pmic-mnt-otmp-hreset-pwrdown-val",
	    (u32 *)&pmic_mntn->otmp_hreset_pwrdown_val, 0x1); /* width:1 */
	ret2 = of_property_read_u32_array(root,
	    "pmic-mnt-otmp-hreset-pwrdown-reg",
	    (u32 *)&pmic_mntn->otmp_hreset_pwrdown_reg, 0x3); /* width:3 */
	ret = (ret0 || ret1 || ret2);
	if (ret) {
		dev_err(dev, "[%s]get pmic otmp attribute failed\n", __func__);
		return -ENODEV;
	}

	ret = pmic_mntn_sysctrl_get(pmic_mntn);
	if (ret)
		return ret;

	ret = of_property_read_u32(
		np, "pmic-mnt-otmp-reg-num", (u32 *)&pmic_mntn->otmp_reg_n);
	if (ret || !pmic_mntn->otmp_reg_n) {
		dev_err(dev, "[%s]get otmp-reg-num attribute failed\n",
			__func__);
		goto otmp_irq_init;
	}

	pmic_mntn->otmp_regs = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->otmp_reg_n, GFP_KERNEL);
	if (pmic_mntn->otmp_regs == NULL) {
		ret = -ENOMEM;
		goto free_iomap;
	}

	ret = of_property_read_u32_array(np, "pmic-mnt-otmp-regs",
		pmic_mntn->otmp_regs, pmic_mntn->otmp_reg_n);
	if (ret) {
		dev_err(dev, "[%s]get otmp-regs attribute failed\n", __func__);
		ret = -ENODEV;
		goto free_iomap;
	}

	pmic_mntn->otmp_exch_desc = (struct pmic_exch_reg *)devm_kzalloc(
		dev, sizeof(*pmic_mntn->otmp_exch_desc) * pmic_mntn->otmp_reg_n,
			GFP_KERNEL);
	if (pmic_mntn->otmp_exch_desc == NULL) {
		ret = -ENOMEM;
		goto free_iomap;
	}

	ret = pmic_mntn_otmp_mntn_spec_data_init(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]pmic_otmp_mntn_spec_data_init failed\n",
			__func__);
		goto free_iomap;
	}

	pmic_mntn->otmp_event_buff = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->otmp_reg_n, GFP_KERNEL);
	if (pmic_mntn->otmp_event_buff == NULL) {
		ret = -ENOMEM;
		goto free_iomap;
	}

	pmic_mntn_clear_otmp(pmic_mntn);

otmp_irq_init:
	ret = pmic_mntn_otmp_mntn_irq_init(pmic_mntn);
	if (ret) {
		dev_dbg(dev, "[%s] pmic_mntn_otmp_mntn_irq_init failed\n",
			__func__);
		goto free_iomap;
	}
	return 0;

free_iomap:
	iounmap(g_sysctrl_base);
	g_sysctrl_base = NULL;
	return ret;
}

static void pmic_mntn_set_smpl(
	struct pmic_mntn_desc *pmic_mntn)
{
	struct device *dev = pmic_mntn->dev;
	unsigned char reg_value;

	/* Set SMPL on/off */
	reg_value =
		pmic_mntn_reg_read(pmic_mntn, pmic_mntn->smpl_en_reg.addr);
	dev_info(dev, "read reg_value:0x%x\n", reg_value);
	set_reg_bit(reg_value, pmic_mntn->smpl_en_reg.shift,
		pmic_mntn->smpl_en_reg.mask, pmic_mntn->smpl_en_val);
	pmic_mntn_reg_write(
		pmic_mntn, pmic_mntn->smpl_en_reg.addr, reg_value);
	reg_value =
		pmic_mntn_reg_read(pmic_mntn, pmic_mntn->smpl_en_reg.addr);
	dev_info(dev, "write reg_value:0x%x\n", reg_value);

	/* Set SMPL effective time */
	reg_value =
		pmic_mntn_reg_read(pmic_mntn, pmic_mntn->smpl_tm_reg.addr);
	set_reg_bit(reg_value, pmic_mntn->smpl_tm_reg.shift,
		pmic_mntn->smpl_tm_reg.mask, pmic_mntn->smpl_tm_val);
	pmic_mntn_reg_write(
		pmic_mntn, pmic_mntn->smpl_tm_reg.addr, reg_value);
}

static int pmic_mntn_smpl_mntn_initial(
	struct pmic_mntn_desc *pmic_mntn)
{
	struct device_node *root = NULL;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	s32 ret, ret0, ret1, ret2, ret3;
	unsigned int smpl_en_val = 0;

	root = of_get_child_by_name(np, "SMPL");
	if (root == NULL) {
		dev_err(dev, "[%s]no pmic-mntn-smpl root node\n",
			__func__);
		return -ENODEV;
	}

	dev_info(dev, "[%s]get pmic-mntn-smpl root node\n", __func__);
	ret0 = of_property_read_u32_array(root, "pmic-mnt-smpl-ctrl-en",
	    &pmic_mntn->smpl_en_val, 0x1); /* width:1 */
	ret1 = of_property_read_u32_array(root, "pmic-mnt-smpl-ctrl-en-reg",
	    (u32 *)&pmic_mntn->smpl_en_reg, 0x3); /* width:3 */
	ret2 = of_property_read_u32_array(root, "pmic-mnt-smpl-ctrl-time",
	    &pmic_mntn->smpl_tm_val, 0x1); /* width:1 */
	ret3 = of_property_read_u32_array(root, "pmic-mnt-smpl-ctrl-time-reg",
	    (u32 *)&pmic_mntn->smpl_tm_reg, 0x3); /* width:3 */
	ret = (ret0 || ret1 || ret2 || ret3);
	if (ret) {
		dev_err(dev, "[%s]get pmic smpl attribute failed\n", __func__);
		return -ENODEV;
	}

	ret = of_property_read_u32_array(root,
		"hisilicon,poweroff-charging-smpl-ctrl-en",
		&smpl_en_val, 0x1); /* width:1 */
	if (!ret) {
		dev_info(dev, "poweroff-charging-smpl-ctrl-en: %u\n",
			smpl_en_val);
		if (strstr(saved_command_line, "androidboot.mode=charger") ||
			strstr(saved_command_line, "ohos.boot.mode=charger")) {
			pmic_mntn->smpl_en_val = smpl_en_val;
			dev_info(dev, "pmic_mntn->smpl_en_val:%u\n",
				pmic_mntn->smpl_en_val);
		}
	}

	pmic_mntn_set_smpl(pmic_mntn);
	return 0;
}

static int pmic_mntn_ocp_mntn_spec_get_dts(struct pmic_mntn_desc *pmic_mntn,
	struct device_node *root, struct device *dev, unsigned int index)
{
	s32 ret;
	unsigned int bit;
	struct pmic_exch_reg *exch_reg_tmp = NULL;

	exch_reg_tmp = pmic_mntn->ocp_exch_desc;

	ret = of_property_read_u32(root, "pmic-mnt-inacceptable-event",
		(u32 *)&exch_reg_tmp[index].inacceptable_event);
	if (ret)
		dev_err(dev, "[%s]read attribute of inacceptable_event\n",
			__func__);

	(void)of_property_read_u32(root, "pmic-mnt-board-reboot-event",
		(u32 *)&exch_reg_tmp[index].board_reboot_event);

	for (bit = 0; bit < pmic_mntn->data_width; bit++)
		ret = ret || of_property_read_string_index(root,
				     "pmic-mnt-event-bit-name", bit,
				     (const char **)&exch_reg_tmp[index]
					     .event_bit_name[bit]);

	ret = ret || of_property_read_u32(root, "pmic-mnt-check_ocp_nofify",
			     (u32 *)&exch_reg_tmp[index].check_ocp_nofify);
	if (ret)
		dev_err(dev, "[%s]read attribute of check_ocp_nofify\n",
			__func__);

	ret = of_property_read_u32(root, "pmic-mnt-check_ocp_reset",
		(u32 *)&exch_reg_tmp[index].check_ocp_reset);
	if (ret)
		dev_err(dev, "[%s]read attribute of check_ocp_reset\n",
			__func__);

	ret = of_property_read_u32_array(root, "pmic-mnt-check_ocp_num",
		(u32 *)exch_reg_tmp[index].check_ocp_num,
		pmic_mntn->data_width);
	if (ret)
		dev_err(dev, "[%s]read attribute of check_ocp_num\n", __func__);

	ret = of_property_read_u32_array(root, "pmic-mnt-event-ops-reg",
		(u32 *)exch_reg_tmp[index].event_ops_reg,
		pmic_mntn->data_width);
	if (ret) {
		dev_err(dev, "[%s]read attribute of event-ops-reg\n", __func__);
		return -ENODEV;
	}
	return ret;
}

static int pmic_mntn_ocp_mntn_spec_data_init(
	struct pmic_mntn_desc *pmic_mntn)
{
	s32 ret;
	unsigned int index;
	char compatible_string[PMIC_DTS_ATTR_LEN] = {0};
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	struct device_node *root = NULL;
	struct pmic_exch_reg *exch_reg_tmp = NULL;

	exch_reg_tmp = pmic_mntn->ocp_exch_desc;

	for (index = 0; index < pmic_mntn->ocp_reg_n; index++) {
		(void)snprintf_s(compatible_string, PMIC_DTS_ATTR_LEN,
			PMIC_DTS_ATTR_LEN - 1,
			"pmic-mntn-ocp-reg0x%x",
			pmic_mntn->ocp_regs[index]);
		root = of_find_compatible_node(np, NULL, compatible_string);
		if (root == NULL) {
			dev_err(dev, "[%s]no %s root node\n", __func__,
				compatible_string);
			return -ENODEV;
		}

		exch_reg_tmp[index].event_bit_name = (char **)devm_kzalloc(dev,
			pmic_mntn->data_width * sizeof(char *), GFP_KERNEL);
		if (exch_reg_tmp[index].event_bit_name == NULL)
			return -ENOMEM;

		exch_reg_tmp[index].event_ops_reg = (u32 *)devm_kzalloc(
			dev, pmic_mntn->data_width * sizeof(u32), GFP_KERNEL);
		if (exch_reg_tmp[index].event_ops_reg == NULL)
			return -ENOMEM;

		exch_reg_tmp[index].check_ocp_num = (u32 *)devm_kzalloc(
			dev, pmic_mntn->data_width * sizeof(u32), GFP_KERNEL);
		if (exch_reg_tmp[index].check_ocp_num == NULL)
			return -ENOMEM;

		exch_reg_tmp[index].check_count = (u32 *)devm_kzalloc(
			dev, pmic_mntn->data_width * sizeof(u32), GFP_KERNEL);
		if (exch_reg_tmp[index].check_count == NULL)
			return -ENOMEM;

		ret = pmic_mntn_ocp_mntn_spec_get_dts(
			pmic_mntn, root, dev, index);
		if (ret) {
			dev_err(dev,
				"[%s]pmic_ocp_mntn_spec_data_get_pro\n",
				__func__);
			return ret;
		}
	}
	return 0;
}

static int pmic_mntn_ocp_mntn_irq_initial(
	struct pmic_mntn_desc *pmic_mntn)
{
	struct device *dev = pmic_mntn->dev;
	s32 ret;

	pmic_mntn->ocp_irq = of_irq_get_byname(dev->of_node, "ocp");
	if (pmic_mntn->ocp_irq < 0) {
		dev_err(dev, "[%s] of_irq_get_byname ocp_irq failed\n",
			__func__);
		return -ENODEV;
	}
	dev_err(dev, "[%s] of_irq_get_byname ocp_irq %d\n",
		__func__, pmic_mntn->ocp_irq);

	INIT_WORK(&pmic_mntn->ocp_wk, pmic_mntn_ocp_wq_handler);

	ret = devm_request_irq(dev, pmic_mntn->ocp_irq,
			pmic_mntn_ocp_irq_handler, IRQF_NO_SUSPEND,
			"pmu-ocp-irq", (void *)pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]request_irq ocp_irq failed\n", __func__);
		return -ENODEV;
	}

#ifdef CONFIG_PMIC_PLATFORM_DEBUG
	if (!g_mntn_notifier_flag) {
		g_mntn_notifier_flag = 1;
		ret = pmic_mntn_register_notifier(&pmic_mntn_test_nb);
		if (ret)
			pr_err("%s:pmic mntn test nb register fail!\n",
				__func__);
	}
#endif
	return 0;
}

static int pmic_mntn_ocp_mntn_initial(
	struct pmic_mntn_desc *pmic_mntn)
{
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	s32 ret;

	ret = of_property_read_u32(
		np, "pmic-mnt-ocp-reg-num", (u32 *)&pmic_mntn->ocp_reg_n);
	if (ret || !pmic_mntn->ocp_reg_n) {
		dev_err(dev, "[%s]get ocp-reg-num attribute failed\n",
			__func__);
		return -ENODEV;
	}

	pmic_mntn->ocp_regs = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->ocp_reg_n, GFP_KERNEL);
	if (pmic_mntn->ocp_regs == NULL)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "pmic-mnt-ocp-regs",
		pmic_mntn->ocp_regs, pmic_mntn->ocp_reg_n);
	if (ret) {
		dev_err(dev, "[%s]get ocp-regs attribute failed\n", __func__);
		return -ENODEV;
	}

	pmic_mntn->ocp_exch_desc = (struct pmic_exch_reg *)devm_kzalloc(
		dev, sizeof(*pmic_mntn->ocp_exch_desc) * pmic_mntn->ocp_reg_n,
			GFP_KERNEL);
	if (pmic_mntn->ocp_exch_desc == NULL)
		return -ENOMEM;

	ret = pmic_mntn_ocp_mntn_spec_data_init(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]pmic_ocp_mntn_spec_data_init failed\n",
			__func__);
		return ret;
	}

	pmic_mntn->ocp_event_buff = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->ocp_reg_n, GFP_KERNEL);
	if (pmic_mntn->ocp_event_buff == NULL)
		return -ENOMEM;

	ret = of_property_read_u32(
		np, "pmic-mnt-need-mask-ocp-irq",
		(u32 *)&pmic_mntn->need_mask_ocp_irq);
	if (ret)
		pmic_mntn->need_mask_ocp_irq = 0;

	pmic_mntn_clear_ocp(pmic_mntn);

	return 0;
}

static int pmic_mntn_record_buf_data_initial(
	struct device_node *root, struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int index, bit;
	char compatible_string[PMIC_DTS_ATTR_LEN] = {0};
	s32 ret;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	struct pmic_exch_reg *exch_reg_tmp = NULL;

	exch_reg_tmp = pmic_mntn->record_exch_desc;

	for (index = 0; index < pmic_mntn->record_reg_n; index++) {
		(void)snprintf_s(compatible_string, PMIC_DTS_ATTR_LEN,
			PMIC_DTS_ATTR_LEN - 1,
			"pmic-mntn-record-reg0x%x",
			pmic_mntn->record_regs[index]);
		root = of_find_compatible_node(np, NULL, compatible_string);
		if (root == NULL) {
			dev_err(dev, "[%s]no %s root node.index %u\n", __func__,
				compatible_string, index);
			return -ENODEV;
		}

		exch_reg_tmp[index].event_bit_name = (char **)devm_kzalloc(dev,
			pmic_mntn->data_width * sizeof(char *), GFP_KERNEL);
		if (exch_reg_tmp[index].event_bit_name == NULL)
			return -ENOMEM;

		ret = of_property_read_u32(root, "pmic-mnt-inacceptable-event",
			(u32 *)&exch_reg_tmp[index].inacceptable_event);
		for (bit = 0; bit < pmic_mntn->data_width; bit++)
			ret = ret || of_property_read_string_index(root,
				 "pmic-mnt-event-bit-name", bit,
				(const char **)&exch_reg_tmp[index].event_bit_name[bit]);

		if (ret) {
			dev_err(dev, "[%s]read attribute of %s\n", __func__,
				compatible_string);
			return -ENODEV;
		}
	}
	return 0;
}

static int pmic_mntn_record_mntn_initial(
	struct pmic_mntn_desc *pmic_mntn)
{
	struct device_node *root = NULL;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	s32 ret;

	ret = of_property_read_u32(np, "pmic-mnt-record-reg-num",
		(u32 *)&pmic_mntn->record_reg_n);
	if (ret || !pmic_mntn->record_reg_n) {
		dev_err(dev, "[%s]get record-reg-num attribute failed\n",
			__func__);
		return -ENODEV;
	}

	pmic_mntn->record_regs = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->record_reg_n, GFP_KERNEL);
	if (pmic_mntn->record_regs == NULL)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "pmic-mnt-record-regs",
		pmic_mntn->record_regs, pmic_mntn->record_reg_n);
	if (ret) {
		dev_err(dev, "[%s]get record-regs attribute failed\n",
			__func__);
		return -ENODEV;
	}
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	INIT_DELAYED_WORK(
		&pmic_mntn->record_dmd_wk, pmic_mntn_record_dsm_report);
	ret = pmic_mntn_record_mntn_dsm_data_get(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s] dsm_record_regs_mask buffer failed\n",
			__func__);
		return ret;
	}
#endif
	pmic_mntn->record_exch_desc = (struct pmic_exch_reg *)devm_kzalloc(dev,
		sizeof(*pmic_mntn->record_exch_desc) * pmic_mntn->record_reg_n,
			GFP_KERNEL);
	if (pmic_mntn->record_exch_desc == NULL)
		return -ENOMEM;

	ret = pmic_mntn_record_buf_data_initial(root, pmic_mntn);
	if (ret) {
		dev_err(dev,
			"[%s] pmic_mntn_record_buf_data_initial failed\n",
			__func__);
		return ret;
	}

	pmic_mntn->record_event_buff = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->record_reg_n, GFP_KERNEL);
	if (pmic_mntn->record_event_buff == NULL)
		return -ENOMEM;

	return 0;
}

static int pmic_mntn_key_register_record_initial(
				struct pmic_mntn_desc *pmic_mntn)
{
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;
	s32 ret;

	ret = of_property_read_u32(np, "pmu-record-reg-num",
		(u32 *)&pmic_mntn->pmu_record_reg_n);
	if (ret) {
		dev_err(dev, "[%s]get pmu-record-reg-num attribute failed\n",
		    __func__);
		return -ENODEV;
	}

	if (pmic_mntn->pmu_record_reg_n <= 0)
		return ret;

	pmic_mntn->pmu_record_regs = (u32 *)devm_kzalloc(
		dev, sizeof(u32) * pmic_mntn->pmu_record_reg_n, GFP_KERNEL);
	if (pmic_mntn->pmu_record_regs == NULL)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "pmu-record-regs",
		pmic_mntn->pmu_record_regs, pmic_mntn->pmu_record_reg_n);
	if (ret) {
		dev_err(dev,
			"[%s]get pmu-record-regs attribute failed\n",
			__func__);
		return -ENODEV;
	}

	return ret;
}

static int pmic_mntn_funs_initial(
				struct pmic_mntn_desc *pmic_mntn)
{
	int ret;
	struct device *dev = pmic_mntn->dev;

	ret = pmic_mntn_key_register_record_initial(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]pmu_key_register_record_initial error\n",
			__func__);
		return ret;
	}

	ret = pmic_mntn_otmp_mntn_initial(pmic_mntn);
	if (ret)
		dev_dbg(dev, "[%s]pmic_mntn_otmp_mntn_initial ret %d\n",
			__func__, ret);

	ret = pmic_mntn_smpl_mntn_initial(pmic_mntn);
	if (ret)
		dev_dbg(dev, "[%s]pmic_mntn_smpl_mntn_initial ret %d\n",
			__func__, ret);

	/* just read dtsi vsys_pwroff_abs_pd  property */
	ret = pmic_mntn_vsys_pwroff_abs_pd_mntn_initial(pmic_mntn);
	if (ret)
		dev_dbg(dev, "[%s]vsys_pwroff_abs_pd_mntn_initial ret %d\n",
			__func__, ret);

	ret = pmic_mntn_ocp_mntn_initial(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]pmic_ocp_mntn_initial error\n",
			__func__);
		return ret;
	}

	ret = pmic_mntn_record_mntn_initial(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]pmic_record_mntn_initial error\n",
			__func__);
		return ret;
	}

	if (of_property_read_bool(dev->parent->of_node, "handle_irq")) {
		ret = pmic_mntn_ocp_mntn_irq_initial(pmic_mntn);
		if (ret) {
			dev_err(dev, "[%s]pmic_ocp_mntn_irq_initial failed\n",
				__func__);
			return -ENODEV;
		}
	}

	return ret;
}

#ifdef CONFIG_HUAWEI_OHOS_DSM
void pmic_power_dsm_init(void)
{
	struct power_dsm_err_map *pmic_dsm_map = NULL;
	int i;
	g_dsm_register_count++;
	pmic_dsm_map = power_dsm_single_map_register("PMU_OCP");
	if (pmic_dsm_map) {
		pmic_dsm_map->event_num = sizeof(pmic_event_map) / sizeof(pmic_event_map[0]);
		for (i = 0; i < pmic_dsm_map->event_num; i++) {
			pmic_dsm_map->map[i].error_no = pmic_event_map[i].error_no;
			pmic_dsm_map->map[i].level = pmic_event_map[i].level;
			(void)strncpy_s(pmic_dsm_map->map[i].domain, POWER_DSM_BUF_SIZE_0016,
							pmic_event_map[i].domain, strlen(pmic_event_map[i].domain));
			(void)strncpy_s(pmic_dsm_map->map[i].name, POWER_DSM_BUF_SIZE_0032,
							pmic_event_map[i].name, strlen(pmic_event_map[i].name));
			(void)strncpy_s(pmic_dsm_map->map[i].key, POWER_DSM_BUF_SIZE_0048,
							pmic_event_map[i].key, strlen(pmic_event_map[i].key));
		}
	}
	return;
}
#endif

int pmic_mntn_drv_init(struct pmic_mntn_desc *pmic_mntn)
{
	int ret;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;

	pmic_mntn->health = (unsigned int)PMIC_HEALTH_STATUS_NORMAL;

	pmic_mntn->init_log_show =
		(char *)devm_kzalloc(dev, PMIC_PRINT_BUF_SIZE, GFP_KERNEL);
	if (pmic_mntn->init_log_show == NULL)
		return -ENOMEM;

	pmic_mntn->irq_log_show =
		(char *)devm_kzalloc(dev, PMIC_PRINT_BUF_SIZE, GFP_KERNEL);
	if (pmic_mntn->irq_log_show == NULL)
		return -ENOMEM;

	ret = of_property_read_u32(
		np, "pmic-mnt-data-width", &pmic_mntn->data_width);
	if (ret || !pmic_mntn->data_width) {
		dev_err(dev, "[%s]get pmic data-width failed\n", __func__);
		return -ENODEV;
	}

#ifdef CONFIG_HUAWEI_OHOS_DSM
	if (g_dsm_register_count == 0)
		pmic_power_dsm_init();
#endif

	ret = pmic_mntn_funs_initial(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]pmic_mntn_funs_initial failed\n",
			__func__);
		return ret;
	}

	pmic_mntn_record_events(pmic_mntn);

	if (!strcmp(np->parent->name, "pmic2")) {
		g_pmic_mntn[PMU2] = pmic_mntn;
	} else if (!strcmp(np->parent->name, "pmic1")) {
		g_pmic_mntn[PMU1] = pmic_mntn;
	} else if (!strcmp(np->parent->name, "pmic")) {
		g_pmic_mntn[PMU0] = pmic_mntn;
	}

	return ret;
}

static void pmic_mntn_vsys_surge_record_event(
	struct pmic_mntn_desc *pmic_mntn)
{
	unsigned int value;
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	const char *bit_name = "pmic vsys surge";
#endif

	value = pmic_read_reg(pmic_mntn->record_vsys_surge.vsys_surge_reg);
	if ((value & pmic_mntn->record_vsys_surge.vsys_surge_reg_mask) ==
	pmic_mntn->record_vsys_surge.vsys_surge_reg_mask) {
		pr_err("%s vsys surge reg 0x%x, value 0x%x\n", __func__,
			pmic_mntn->record_vsys_surge.vsys_surge_reg, value);
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
		pmic_mntn_ocp_dsm_client(bit_name,
			pmic_mntn->record_vsys_surge.vsys_surge_dmd_offset);
#endif
	/* clear sys surge record */
		value = (value & (~pmic_mntn->record_vsys_surge.vsys_surge_reg_mask)) |
			(pmic_mntn->record_vsys_surge.vsys_surge_reg_mask);
		pmic_write_reg(pmic_mntn->record_vsys_surge.vsys_surge_reg, value);
	}
}

static int pmic_mntn_usb_notifier_call(struct notifier_block *usb_nb,
	unsigned long event, void *data)
{
	struct pmic_mntn_desc *pmic_mntn = container_of(usb_nb, struct pmic_mntn_desc, usb_nb);

	pmic_mntn_vsys_surge_record_event(pmic_mntn);
	return NOTIFY_OK;
}

void pmic_vsys_surge_init(struct pmic_mntn_desc *pmic_mntn)
{
	int ret;
	struct device *dev = pmic_mntn->dev;
	struct device_node *np = dev->of_node;

	ret = of_property_read_u32_array(np, "pmic_mntn_vsys_surge_event",
		(int *)&pmic_mntn->record_vsys_surge, 3); /* vsys surge parm is 3 */
	if (!ret) {
		pmic_mntn_vsys_surge_record_event(pmic_mntn);
		pmic_mntn->usb_nb.notifier_call = pmic_mntn_usb_notifier_call;
		ret = chip_charger_type_notifier_register(&pmic_mntn->usb_nb);
		if (ret < 0)
			dev_err(dev, "[%s]chip_charger_type_notifier_register failed\n",
				__func__);
	}
}

void pmic_mntn_drv_deinit(void)
{
#ifdef CONFIG_PMIC_PLATFORM_DEBUG
		int ret;

		if (!g_mntn_notifier_flag)
			return;

		ret = pmic_mntn_unregister_notifier(&pmic_mntn_test_nb);
		if (ret)
			pr_err("%s: pmic mntn test nb unregister fail!\n",
				__func__);
		g_mntn_notifier_flag = 0;
#endif
}

static int pmic_mntn_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pmic_mntn_desc *pmic_mntn = NULL;
	int ret;

	dev_err(dev, "[%s] +\n", __func__);
	pmic_mntn = (struct pmic_mntn_desc *)devm_kzalloc(
		dev, sizeof(*pmic_mntn), GFP_KERNEL);
	if (pmic_mntn == NULL)
		return -ENOMEM;

	pmic_mntn->dev = dev;

	pmic_mntn->regmap = dev_get_regmap(dev->parent, NULL);
	if (!pmic_mntn->regmap)
		return -ENODEV;

	ret = pmic_mntn_drv_init(pmic_mntn);
	if (ret) {
		dev_err(dev, "[%s]pmic_mntn_drv_init failed\n",
			__func__);
		return ret;
	}

	pmic_vsys_surge_init(pmic_mntn);

	dev_err(dev, "[%s] succ\n", __func__);

	return 0;
}

#ifdef CONFIG_DFX_PLATFORM_MAINTAIN
static int pmic_mntn_sdcard_ocp_handler(char *power_name)
{
	static struct regulator *power_sd = NULL;
	static struct regulator *power_sdio = NULL;
	int ret;

	if (power_name)
		pr_err("%s ocp special handler\n", power_name);

	if (power_sd == NULL) {
		power_sd = regulator_get(NULL, SUPPLY_SD);
		if (IS_ERR(power_sd)) {
			pr_err("[%s]sd regulator found\n", __func__);
			return -ENODEV;
		}
	}

	ret = regulator_force_disable(power_sd);
	if (ret) {
		pr_err("[%s]disable sd regulator error\n", __func__);
		return ret;
	}

	if (power_sdio == NULL) {
		power_sdio = regulator_get(NULL, SUPPLY_SD_IO);
		if (IS_ERR(power_sdio)) {
			pr_err("[%s]sdio regulator found\n", __func__);
			return -ENODEV;
		}
	}

	ret = regulator_force_disable(power_sdio);
	if (ret) {
		pr_err("[%s]disable sdio regulator error\n", __func__);
		return ret;
	}

	return ret;
}
#endif

static int __init pmic_mntn_register_special_ocp(void)
{
#ifdef CONFIG_DFX_PLATFORM_MAINTAIN
	int ret;

	if (!check_mntn_switch(MNTN_SD2JTAG) && !check_mntn_switch(MNTN_SD2DJTAG)) {
		ret = pmic_special_ocp_register(
			SUPPLY_SD, pmic_mntn_sdcard_ocp_handler);
		if (ret) {
			pr_err("[%s]supply_sd register error\n", __func__);
			return ret;
		}

		ret = pmic_special_ocp_register(
			SUPPLY_SD_IO, pmic_mntn_sdcard_ocp_handler);
		if (ret) {
			pr_err("[%s]supply_sd_io register error\n", __func__);
			return ret;
		}
	}
#endif
	return 0;
}

/* The PMU can be registered only after all PMUs are initialized */
late_initcall(pmic_mntn_register_special_ocp);

static int pmic_mntn_remove(struct platform_device *pdev)
{
	pmic_mntn_drv_deinit();
	return 0;
}

const static struct of_device_id pmic_mntn_match_tbl[] = {
	{
		.compatible = "hisilicon,pmic-mntn",
	},

	{}
};

static struct platform_driver pmic_mntn_driver = {
	.driver = {
		.name = "pmic-platform-mntn",
		.owner  = THIS_MODULE,
		.of_match_table = pmic_mntn_match_tbl,
	},
	.probe = pmic_mntn_probe,
	.remove = pmic_mntn_remove,

};

static int __init pmic_mntn_init(void)
{
	return platform_driver_register(&pmic_mntn_driver);
}

static void __exit pmic_mntn_exit(void)
{
	platform_driver_unregister(&pmic_mntn_driver);
}


module_init(pmic_mntn_init);
module_exit(pmic_mntn_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("PMU MNTN Driver");
