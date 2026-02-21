/*
 * l3cache_common.h
 *
 * L3cache Common Head File
 *
 * Copyright (c) 2017-2021 Huawei Technologies Co., Ltd.
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
#ifndef __L3CACHE_COMMON_H__
#define __L3CACHE_COMMON_H__

#include <asm/compiler.h>
#ifdef CONFIG_L3CACHE_SHARE
#include <platform_include/cee/linux/l3cache_share.h>
#endif
#include <linux/printk.h>

#define MAX_COUNT_VAL			0xFFFFFFFF
#define MAX_COUNT_LEN			32
#define BYTES_PER_KBYTE			1024
#define BYTES_PER_MBYTE			(1024 * 1024)

/* start: lower bit, from 0, end: higher bit; start, end itself is included */
#define hc_k_get_bits(x, start, end)	(((x) & ~(~0ULL << ((end) - (start) + 1)) << (start)) >> (start))

/*
 * start: the start bit(the lower end, start from 0), including itself;
 * length: the length of bits(m-n+1)
 * when the new_val is out of the bit range of length, the set val will be cut
 * from lower bit to higher end for the length, then it will be
 * filled in the base_val.
 */
#define hc_k_set_bits(base_val, start, length, new_val) \
	(((base_val) = ((base_val) & (~(((1ULL << (length)) - 1) << (start)))) | \
	((u32)((new_val) & ((1ULL << (length)) - 1)) << (start))))

enum l3cache_share_cmd {
	CMD_ENABLE_ACP = 0,
	CMD_DISABLE_ACP,
};

enum dsu_pctrl_cmd {
	CMD_SET_PWR_CTRL = 0,
	CMD_SET_PORTION_CTRL,
};

enum l3cache_extension_cmd {
	CMD_READ_HIT_MISS = 0,
	CMD_ENABLE_WE,
};

enum cpu_cache_type {
	D_CACHE,
	I_CACHE,
	CACHE_TYPE_MAX,
};

enum l3cache_pwrctlr_cmd {
	CMD_READ_PWRCTLR = 0,
	CMD_WRITE_PWRCTLR,
	CMD_READ_PWRSTAT,
	CMD_READ_RETCTLR,
	CMD_WRITE_RETCTLR,
#ifdef CONFIG_HYBRID_CACHE_DEBUG
	CMD_READ_TAG_HASH,
	CMD_WRITE_TAG_HASH,
	CMD_READ_HC_REGS_IN_MRS,
#endif
};

#ifdef CONFIG_L3CACHE_SHARE_PERF
extern struct mutex g_l3c_acp_lock;
extern bool g_req_pending_flag;
extern bool g_acp_enable_flag;
extern bool g_acp_first_flag;

void l3_cache_enable_acp(unsigned int id, unsigned int size);
void l3_cache_disable_acp(unsigned int id);
#endif /* CONFIG_L3CACHE_SHARE_PERF */

#ifdef CONFIG_L3CACHE_EXTENSION_DYNAMIC
extern struct spinlock g_l3extension_lock;
extern bool g_dpc_extension_flag;
extern bool g_extension_enable_status;
#endif /* CONFIG_L3CACHE_EXTENSION_DYNAMIC */

#ifdef CONFIG_LPCPU_MULTIDRV_CPUIDLE
extern bool lpcpu_fcm_cluster_pwrdn(void);
#else
static inline bool lpcpu_fcm_cluster_pwrdn(void)
{
	return 0;
}
#endif /* CONFIG_LPCPU_MULTIDRV_CPUIDLE */

#endif /* __L3CACHE_COMMON_H__ */
