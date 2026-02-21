/*
 * cpu_sh.h
 *
 * CPU SH HEAD
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
#ifndef __CPU_SH_H__
#define __CPU_SH_H__

#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <mntn_subtype_exception.h>
#include <cpu_cluster_def.h>
#include "edac_module.h"

extern unsigned int g_cpu_pll_source[FREQ_DOMAIN_MAX];

#ifdef CONFIG_DFX_SH
void ras_ce_sh(struct edac_device_ctl_info *edac_dev, u32 cpu, char* instance_name);
void ras_de_sh(struct edac_device_ctl_info *edac_dev, u32 cpu, char* instance_name);
void ras_ue_sh(struct edac_device_ctl_info *edac_dev, u32 cpu, char* instance_name);
#else
static inline void ras_ce_sh(struct edac_device_ctl_info *edac_dev __maybe_unused,
	u32 cpu __maybe_unused, char* instance_name __maybe_unused)
{
}

static inline void ras_de_sh(struct edac_device_ctl_info *edac_dev __maybe_unused,
	u32 cpu __maybe_unused, char* instance_name __maybe_unused)
{
}

static inline void ras_ue_sh(struct edac_device_ctl_info *edac_dev __maybe_unused,
	u32 cpu __maybe_unused, char* instance_name __maybe_unused)
{
}
#endif

#endif /* __CPU_SH_H__ */
