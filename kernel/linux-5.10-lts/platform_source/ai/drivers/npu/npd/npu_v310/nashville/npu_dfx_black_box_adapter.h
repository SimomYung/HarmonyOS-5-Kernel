/*
 * npu_dfx_black_box_adapter.h
 *
 * about npu black box adapter
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef __NPU_BLACK_BOX_ADAPTER_H
#define __NPU_BLACK_BOX_ADAPTER_H

#include <platform_include/basicplatform/linux/dfx_qic_v300_modid_para.h>

#include "soc_pctrl_interface.h"
#include "npu_soc_defines.h"
#include "soc_qic_bus.h"
#include "mntn_subtype_exception.h"
#include "bbox/npu_dfx_black_box.h"

#define PERI_CRG_BASE   (0xFFB85000U)
#define ACTRL_BASE      (0xFDB22000U)
#define PCTRL_BASE      (0xFF00C000U)
#define SCTRL_BASE      (0xFDB23000U)

#define SOC_NPUTS_QIC_TSCPU_ERR_CODE_START  (0x0)
#define SOC_NPUTS_QIC_TSCPU_ERR_CODE_END    (0x1)
#define SOC_NPUTS_QIC_HWTS_ERR_CODE_START   (0x40)
#define SOC_NPUTS_QIC_HWTS_ERR_CODE_END     (0x60)

#define ACTRL_ISO_EN_GROUP0_PERI_OFFSET              (0x0)
#define ACTRL_AO_MEM_CTRL10_OFFSET                   (0x848)
 
#define CRGPERIPH_GENERAL_SEC_RSTSTAT_OFFSET        (0xD08)
#define CRGPERIPH_PERSTAT12_OFFSET                  (0x47C)
#define CRGPERIPH_GENERAL_SEC_PERSTAT_OFFSET        (0xE0C)
#define CRGPERIPH_GENERAL_SEC_RSTSTAT2_OFFSET       (0xD20)
#define CRGPERIPH_GENERAL_SEC_PERSTAT3_OFFSET       (0xE3C)
#define CRGPERIPH_PERSTAT4_OFFSET                   (0x04C)
#define CRGPERIPH_GENERAL_SEC_PEREN5_OFFSET         (0xE50)
#define CRGPERIPH_GENERAL_SEC_CLKDIV1_OFFSET        (0xB04)
#define CRGPERIPH_PERRSTSTAT0_OFFSET                (0x068)
 
#define SCTRL_SC_NPU_LPCTRL0_SEC_OFFSET             (0xE20)
#define SCTRL_SC_NPU_LPCTRL1_SEC_OFFSET             (0xE24)
#define SCTRL_SC_NPU_LPCTRL2_SEC_OFFSET             (0xE28)

static struct rdr_exception_info_s npu_rdr_excetption_info[] = {
	{
		.e_modid = (u32)EXC_TYPE_TS_AICORE_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_TS_AICORE_EXCEPTION,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_NPU,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_NPU,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_exce_subtype = AICORE_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "NPU",
		.e_desc = "AICORE_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_TS_AICORE_TIMEOUT,
		.e_modid_end = (u32)EXC_TYPE_TS_AICORE_TIMEOUT,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_NPU,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_NPU,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_exce_subtype = AICORE_TIMEOUT,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "NPU",
		.e_desc = "AICORE_TIMEOUT",
	},
	{
		.e_modid = (u32)EXC_TYPE_TS_AIV_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_TS_AIV_EXCEPTION,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_NPU,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_NPU,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_exce_subtype = AIV_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "NPU",
		.e_desc = "AIV_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_TS_AIV_TIMEOUT,
		.e_modid_end = (u32)EXC_TYPE_TS_AIV_TIMEOUT,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_NPU,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_NPU,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_exce_subtype = AIV_TIMEOUT,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "NPU",
		.e_desc = "AIV_TIMEOUT",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_TS_RUNNING_TIMEOUT,
		.e_modid_end = (u32)RDR_EXC_TYPE_TS_RUNNING_TIMEOUT,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask  = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = TS_RUNNING_TIMEOUT,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "TS_RUNNING_TIMEOUT",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_TS_INIT_EXCEPTION,
		.e_modid_end = (u32)RDR_EXC_TYPE_TS_INIT_EXCEPTION,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = TS_INIT_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "TS_INIT_EXCP",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NPU_POWERUP_FAIL,
		.e_modid_end = (u32)RDR_EXC_TYPE_NPU_POWERUP_FAIL,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = POWERUP_FAIL,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "POWERUP_FAIL",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NPU_POWERDOWN_FAIL,
		.e_modid_end = (u32)RDR_EXC_TYPE_NPU_POWERDOWN_FAIL,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask  = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = POWERDOWN_FAIL,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "POWERDOWN_FAIL",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NPU_SMMU_EXCEPTION,
		.e_modid_end = (u32)RDR_EXC_TYPE_NPU_SMMU_EXCEPTION,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = SMMU_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "SMMU_EXCP",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NOC_NORESET_NPU1,
		.e_modid_end = (u32)RDR_EXC_TYPE_NOC_NORESET_NPU1,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask  = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = NPU_AICORE0_NOC_ERR,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "NPU_AICORE0_NOC_ERR",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NOC_NPU5,
		.e_modid_end = (u32)RDR_EXC_TYPE_NOC_NPU5,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask  = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = NPU_TS_CPU_NOC_ERR,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "NPU_TS_CPU_NOC_ERR",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NOC_NORESET_NPU0,
		.e_modid_end = (u32)RDR_EXC_TYPE_NOC_NORESET_NPU0,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask  = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = NPU_TS_HWTS_NOC_ERR,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "NPU_TS_HWTS_NOC_ERR",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NOC_NPU7,
		.e_modid_end = (u32)RDR_EXC_TYPE_NOC_NPU7,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask  = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = NPU_TCU_NOC_ERR,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "NPU_TCU_NOC_ERR",
	},
	{
		.e_modid = (u32)RDR_TYPE_HWTS_BUS_ERROR,
		.e_modid_end = (u32)RDR_TYPE_HWTS_BUS_ERROR,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = HWTS_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "HWTS_EXCP",
	},
	{
		.e_modid = (u32)RDR_TYPE_HWTS_EXCEPTION_ERROR,
		.e_modid_end = (u32)RDR_TYPE_HWTS_EXCEPTION_ERROR,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = HWTS_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "HWTS_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_TS_AICORE_EFF_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_TS_AICORE_EFF_EXCEPTION,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_NPU,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_NPU,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_exce_subtype = EFF_AICORE_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "NPU",
		.e_desc = "EFF_AICORE_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_TS_AICORE_EFF_TIMEOUT,
		.e_modid_end = (u32)EXC_TYPE_TS_AICORE_EFF_TIMEOUT,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_NPU,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_NPU,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_exce_subtype = EFF_AICORE_TIMEOUT,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "NPU",
		.e_desc = "EFF_AICORE_TIMEOUT",
	},
	{
		.e_modid = (u32)RDR_EXC_TYPE_NOC_NORESET_NPU2,
		.e_modid_end = (u32)RDR_EXC_TYPE_NOC_NORESET_NPU2,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask  = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = NPU_AICORE1_NOC_ERR,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "NPU_AICORE1_NOC_ERR",
	}, {
		.e_modid = (u32)RDR_TYPE_HWTS_TIMEOUT,
		.e_modid_end = (u32)RDR_TYPE_HWTS_TIMEOUT,
		.e_process_priority = RDR_ERR,
		.e_reboot_priority = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_NPU,
		.e_reset_core_mask = RDR_NPU,
		.e_from_core = RDR_NPU,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_exce_type = NPU_S_EXCEPTION,
		.e_exce_subtype = HWTS_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)RDR_SAVE_BL31_LOG,
		.e_from_module = "NPU",
		.e_desc = "HWTS_TIMEOUT",
	},
};

static struct npu_qic_register_info_s npu_qic_modid_array[] = {
	// hwts_lite
	{(u32)SOC_NPU_HWTS_LITE_MID, (u32)SOC_NPU_HWTS_LITE_MID, (u32)QIC_NPU_BUS, (u32)RDR_EXC_TYPE_NOC_NORESET_NPU0},
	// lite_core0
	{(u32)SOC_NPU_LITE_CORE0_MID, (u32)SOC_NPU_LITE_CORE0_MID, (u32)QIC_NPU_BUS, (u32)RDR_EXC_TYPE_NOC_NORESET_NPU1},
	// lite_core1
	{(u32)SOC_NPU_LITE_CORE1_MID, (u32)SOC_NPU_LITE_CORE1_MID, (u32)QIC_NPU_BUS, (u32)RDR_EXC_TYPE_NOC_NORESET_NPU2},
	// tscpu
	{(u32)SOC_NPU_TS_CPU_MID, (u32)SOC_NPU_TS_CPU_MID, (u32)QIC_NPU_BUS, (u32)RDR_EXC_TYPE_NOC_NPU5},
	// npu_tcu
	{(u32)SOC_NPU_SMMU_TCU_MID, (u32)SOC_NPU_SMMU_TCU_MID, (u32)QIC_NPU_BUS, (u32)RDR_EXC_TYPE_NOC_NPU7},
	// hwts_lite NPUTS_BUS
	{(u32)SOC_NPU_HWTS_LITE_MID, (u32)SOC_NPU_HWTS_LITE_MID, (u32)QIC_NPUTS_BUS, (u32)RDR_EXC_TYPE_NOC_NORESET_NPU0},
	// tscpu NPUTS_BUS
	{(u32)SOC_NPU_TS_CPU_MID, (u32)SOC_NPU_TS_CPU_MID, (u32)QIC_NPUTS_BUS, (u32)RDR_EXC_TYPE_NOC_NPU5},
	{SOC_NPUTS_QIC_TSCPU_ERR_CODE_START, SOC_NPUTS_QIC_TSCPU_ERR_CODE_END, QIC_NPUTS_BUS, RDR_EXC_TYPE_NOC_NPU5},
	{SOC_NPUTS_QIC_HWTS_ERR_CODE_START, SOC_NPUTS_QIC_HWTS_ERR_CODE_END, QIC_NPUTS_BUS, RDR_EXC_TYPE_NOC_NORESET_NPU0},
};

static struct npu_dump_offset pctrl_regs[] = {{(unsigned int)SOC_PCTRL_PERI_CTRL2_ADDR(0UL), "PCTRL_PERI_CTRL2"}};

static struct npu_dump_offset actrl_regs[] = {
	{(unsigned int)ACTRL_AO_MEM_CTRL10_OFFSET,        "ACTRL_AO_MEM_CTRL10"},
	{(unsigned int)ACTRL_ISO_EN_GROUP0_PERI_OFFSET,   "ACTRL_ISO_EN_GROUP0"}
};

static struct npu_dump_offset peri_crg_regs[] = {
	{(unsigned int)CRGPERIPH_GENERAL_SEC_RSTSTAT_OFFSET,       "CRGPERIPH_GENERAL_SEC_RSTSTAT"},
	{(unsigned int)CRGPERIPH_PERSTAT12_OFFSET,                 "CRGPERIPH_PERSTAT12"},
	{(unsigned int)CRGPERIPH_GENERAL_SEC_PERSTAT_OFFSET,       "CRGPERIPH_GENERAL_SEC_PERSTAT"},
	{(unsigned int)CRGPERIPH_GENERAL_SEC_RSTSTAT2_OFFSET,      "CRGPERIPH_GENERAL_SEC_RSTSTAT2"},
	{(unsigned int)CRGPERIPH_GENERAL_SEC_PERSTAT3_OFFSET,      "CRGPERIPH_GENERAL_SEC_PERSTAT3"},
	{(unsigned int)CRGPERIPH_PERSTAT4_OFFSET,                  "CRGPERIPH_PERSTAT4"},
	{(unsigned int)CRGPERIPH_GENERAL_SEC_PEREN5_OFFSET,        "CRGPERIPH_GENERAL_SEC_PEREN5"},
	{(unsigned int)CRGPERIPH_GENERAL_SEC_CLKDIV1_OFFSET,       "CRGPERIPH_GENERAL_SEC_CLKDIV1"},
	{(unsigned int)CRGPERIPH_GENERAL_SEC_PERSTAT3_OFFSET,      "CRGPERIPH_GENERAL_SEC_PERSTAT3"},
	{(unsigned int)CRGPERIPH_PERRSTSTAT0_OFFSET,               "CRGPERIPH_PERRSTSTAT0"},
};

static struct npu_dump_offset sctrl_regs[] = {
	{(unsigned int)SCTRL_SC_NPU_LPCTRL0_SEC_OFFSET,   "SCTRL_SC_NPU_LPCTRL0_SEC"},
	{(unsigned int)SCTRL_SC_NPU_LPCTRL1_SEC_OFFSET,   "SCTRL_SC_NPU_LPCTRL1_SEC"},
	{(unsigned int)SCTRL_SC_NPU_LPCTRL2_SEC_OFFSET,   "SCTRL_SC_NPU_LPCTRL2_SEC"}
};

static struct npu_dump_reg peri_regs[] = {
	{PCTRL_BASE, 0x1FFF, pctrl_regs, (unsigned int)ARRAY_SIZE(pctrl_regs)},
	{ACTRL_BASE, 0xFFF, actrl_regs, (unsigned int)ARRAY_SIZE(actrl_regs)},
	{PERI_CRG_BASE, 0xFFF, peri_crg_regs, (unsigned int)ARRAY_SIZE(peri_crg_regs)},
	{SCTRL_BASE, 0xFFF, sctrl_regs, (unsigned int)ARRAY_SIZE(sctrl_regs)},
};

#endif /* __NPU_BLACK_BOX_ADAPTER_H */
