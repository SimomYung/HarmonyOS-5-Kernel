/*
 * cpu_improve_yield.h
 *
 * CPU IMPROVE YIELD HEAD
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
#ifndef __CPU_IMPROVE_YIELD_H__
#define __CPU_IMPROVE_YIELD_H__

#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <mntn_subtype_exception.h>
#include <cpu_cluster_def.h>
#include "edac_module.h"

#define CPU_IY_NV_INFO_MAX_LENGTH 256

#if defined(CONFIG_IMPROVE_YIELD)
int cpu_register_iy_nvme(char *buffer, unsigned int *length);
#else
static inline int cpu_register_iy_nvme(char *buffer __maybe_unused, unsigned int *length __maybe_unused)
{
	return 0;
}
#endif

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
bool cpu_improve_yield(u32 cpu);
#else
static inline bool cpu_improve_yield(u32 cpu __maybe_unused)
{
	return true;
}
#endif

#endif /* __CPU_IMPROVE_YIELD_H__ */
