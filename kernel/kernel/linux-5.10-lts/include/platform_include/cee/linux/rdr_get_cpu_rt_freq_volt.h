/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * rdr_get_cpu_rt_freq_volt.h
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _RDR_GET_CPU_RT_FV_H
#define _RDR_GET_CPU_RT_FV_H
#include <linux/cpufreq.h>

#define RDR_CPU_RT_FV_GET_SUCC 0
#define RDR_CPU_RT_FV_GET_FAIL (-1)
#define CPU_MNT_RT_MAGIC_NUM 0x5454494C /* ascii number of LITT, for runtime seek */
#define CPU_PROFILE_BLOCK (sizeof(struct cpu_profile))
#define CPU_RUNTIME_BLOCK (sizeof(struct cpu_dvfs_runtime))

#ifdef CONFIG_RDR_GET_CPU_RT_FV
int get_cpu_freq_info_for_diag(const void *bbox_base, size_t size, char *out_buffer, size_t buffer_maxsize);
int get_cpu_volt_info_for_diag(const void *bbox_base, size_t size, char *out_buffer, size_t buffer_maxsize);
#else
static inline int get_cpu_freq_info_for_diag(const void *bbox_base __maybe_unused, size_t size __maybe_unused,
	char *out_buffer __maybe_unused, size_t buffer_maxsize __maybe_unused)
{
	return -1;
}
static inline int get_cpu_volt_info_for_diag(const void *bbox_base __maybe_unused, size_t size __maybe_unused,
	char *out_buffer __maybe_unused, size_t buffer_maxsize __maybe_unused)
{
	return -1;
}
#endif /* CONFIG_RDR_GET_CPU_RT_FV */

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
int get_cpu_rt_freq_index_via_tzsp(unsigned int cluster);
int set_cpu_improve_yield_data_via_tzsp(u64 cluster, u64 freq_idx_mask, u64 is_all_freq);
#endif
#endif /* _RDR_GET_CPU_RT_FV_H */
