// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
/*
 *
 * (C) COPYRIGHT 2019-2022 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 */

#include <mali_kbase.h>
#include <csf/mali_kbase_csf_registers.h>
#include <gpu/mali_kbase_gpu_fault.h>
#include "mali_kbase_platform.h"

#define CS_FAULT_DMD_TAB_SIZE (sizeof(cs_fault_dmd_tab) / sizeof(cs_fault_dmd_tab[0]))
#define CS_FATAL_DMD_TAB_SIZE (sizeof(cs_fatal_dmd_tab) / sizeof(cs_fatal_dmd_tab[0]))

const char *kbase_gpu_exception_name(u32 const exception_code)
{
	const char *e;

	switch (exception_code) {
	/* CS exceptions */
	case CS_FAULT_EXCEPTION_TYPE_CS_RESOURCE_TERMINATED:
		e = "CS_RESOURCE_TERMINATED";
		break;
	case CS_FAULT_EXCEPTION_TYPE_CS_INHERIT_FAULT:
		e = "CS_INHERIT_FAULT";
		break;
	/* CS fatal exceptions */
	case CS_FATAL_EXCEPTION_TYPE_CS_CONFIG_FAULT:
		e = "CS_CONFIG_FAULT";
		break;
	case CS_FATAL_EXCEPTION_TYPE_CS_ENDPOINT_FAULT:
		e = "FATAL_CS_ENDPOINT_FAULT";
		break;
	case CS_FATAL_EXCEPTION_TYPE_CS_INVALID_INSTRUCTION:
		e = "FATAL_CS_INVALID_INSTRUCTION";
		break;
	case CS_FATAL_EXCEPTION_TYPE_CS_CALL_STACK_OVERFLOW:
		e = "FATAL_CS_CALL_STACK_OVERFLOW";
		break;
	/*
	 * CS_FAULT_EXCEPTION_TYPE_CS_BUS_FAULT and CS_FATAL_EXCEPTION_TYPE_CS_BUS_FAULT share the same error code
	 * Type of CS_BUS_FAULT will be differentiated by CSF exception handler
	 */
	case CS_FAULT_EXCEPTION_TYPE_CS_BUS_FAULT:
		e = "CS_BUS_FAULT";
		break;
	/* Shader exceptions */
	case CS_FAULT_EXCEPTION_TYPE_INSTR_INVALID_PC:
		e = "INSTR_INVALID_PC";
		break;
	case CS_FAULT_EXCEPTION_TYPE_INSTR_INVALID_ENC:
		e = "INSTR_INVALID_ENC";
		break;
	case CS_FAULT_EXCEPTION_TYPE_INSTR_BARRIER_FAULT:
		e = "INSTR_BARRIER_FAULT";
		break;
	/* Iterator exceptions */
	case CS_FAULT_EXCEPTION_TYPE_KABOOM:
		e = "KABOOM";
		break;
	/* Misc exceptions */
	case CS_FAULT_EXCEPTION_TYPE_DATA_INVALID_FAULT:
		e = "DATA_INVALID_FAULT";
		break;
	case CS_FAULT_EXCEPTION_TYPE_TILE_RANGE_FAULT:
		e = "TILE_RANGE_FAULT";
		break;
	case CS_FAULT_EXCEPTION_TYPE_ADDR_RANGE_FAULT:
		e = "ADDR_RANGE_FAULT";
		break;
	case CS_FAULT_EXCEPTION_TYPE_IMPRECISE_FAULT:
		e = "IMPRECISE_FAULT";
		break;
	/* FW exceptions */
	case CS_FATAL_EXCEPTION_TYPE_FIRMWARE_INTERNAL_ERROR:
		e = "FIRMWARE_INTERNAL_ERROR";
		break;
	case CS_FATAL_EXCEPTION_TYPE_CS_UNRECOVERABLE:
		e = "CS_UNRECOVERABLE";
		break;
	case CS_FAULT_EXCEPTION_TYPE_RESOURCE_EVICTION_TIMEOUT:
		e = "RESOURCE_EVICTION_TIMEOUT";
		break;
	/* GPU Fault */
	case GPU_FAULTSTATUS_EXCEPTION_TYPE_GPU_BUS_FAULT:
		e = "GPU_BUS_FAULT";
		break;
	case GPU_FAULTSTATUS_EXCEPTION_TYPE_GPU_SHAREABILITY_FAULT:
		e = "GPU_SHAREABILITY_FAULT";
		break;
	case GPU_FAULTSTATUS_EXCEPTION_TYPE_SYSTEM_SHAREABILITY_FAULT:
		e = "SYSTEM_SHAREABILITY_FAULT";
		break;
	case GPU_FAULTSTATUS_EXCEPTION_TYPE_GPU_CACHEABILITY_FAULT:
		e = "GPU_CACHEABILITY_FAULT";
		break;
	case CS_FAULT_EXCEPTION_TYPE_PROGRESS_TIMER_TIMEOUT:
		e = "PROGRESS_TIMER_TIMEOUT";
		break;
	/* Any other exception code is unknown */
	default:
		e = "UNKNOWN";
		break;
	}

	return e;
}
#if defined (CONFIG_HUAWEI_DSM) || defined (CONFIG_HUAWEI_OHOS_DSM)
static cs_fault_dmd_info cs_fault_dmd_tab[] = {
	{CS_FAULT_EXCEPTION_TYPE_KABOOM,true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_CS_RESOURCE_TERMINATED, true, 0, CS_FAULT_DMD_TIMES * 5},
	{CS_FAULT_EXCEPTION_TYPE_CS_BUS_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_CS_INHERIT_FAULT, false, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_INSTR_INVALID_PC, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_INSTR_INVALID_ENC, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_INSTR_BARRIER_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_DATA_INVALID_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_TILE_RANGE_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_ADDR_RANGE_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_IMPRECISE_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_RESOURCE_EVICTION_TIMEOUT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FAULT_EXCEPTION_TYPE_PROGRESS_TIMER_TIMEOUT,true, 0, CS_FAULT_DMD_PROGRESS_TIMER_TIMES},
};

static cs_fault_dmd_info cs_fatal_dmd_tab[] = {
	{CS_FATAL_EXCEPTION_TYPE_CS_CONFIG_FAULT,true, 0, CS_FAULT_DMD_TIMES},
	{CS_FATAL_EXCEPTION_TYPE_CS_UNRECOVERABLE, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FATAL_EXCEPTION_TYPE_CS_ENDPOINT_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FATAL_EXCEPTION_TYPE_CS_BUS_FAULT, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FATAL_EXCEPTION_TYPE_CS_INVALID_INSTRUCTION, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FATAL_EXCEPTION_TYPE_CS_CALL_STACK_OVERFLOW, true, 0, CS_FAULT_DMD_TIMES},
	{CS_FATAL_EXCEPTION_TYPE_FIRMWARE_INTERNAL_ERROR, true, 0, CS_FAULT_DMD_TIMES},
};

void kbase_gpu_fault_dmd_report(struct kbase_device *kbdev, const u32 exception_code, const u32 type)
{
	if (WARN_ON(kbdev == NULL))
		return;

	struct cs_fault_dmd_info *dmd_tab = NULL;
	if (type == CS_FAULT) {
		dmd_tab = cs_fault_dmd_tab;
	} else if (type == CS_FATAL) {
		dmd_tab = cs_fatal_dmd_tab;
	} else {
		dev_err(kbdev->dev, "Invalid type: %u\n", type);
		return;
	}

	if (dmd_tab == NULL) {
		dev_err(kbdev->dev, "dmd_tab is NULL\n");
        return;
	}

	u32 i;
	u32 size = (type == CS_FAULT) ? CS_FAULT_DMD_TAB_SIZE : CS_FATAL_DMD_TAB_SIZE;
	for (i = 0; i < size; i++) {
		if (dmd_tab[i].cs_fault_type != exception_code) {
			continue;
		} else {
			dmd_tab[i].fault_num++;
			if (dmd_tab[i].fault_enable &&
				(dmd_tab[i].fault_num % dmd_tab[i].report_fault_cnt == 0)) {
				gpu_dsm_report(kbdev, GPU_BIT_STUCK_DSM_NO,
					"%s: 0x%x (%s),%llu\n",
					type == CS_FAULT ? "CS_FAULT_TYPE" : "CS_FATAL_TYPE",
					exception_code, kbase_gpu_exception_name(exception_code),
					dmd_tab[i].fault_num);
				dev_err(kbdev->dev, "%s: 0x%x (%s),%llu\n",
					type == CS_FAULT ? "CS_FAULT_TYPE" : "CS_FATAL_TYPE",
					exception_code, kbase_gpu_exception_name(exception_code),
					dmd_tab[i].fault_num);
			}
			break;
		}
	}
}
#endif