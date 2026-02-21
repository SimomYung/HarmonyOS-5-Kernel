/*
 * loadmonitor_common.h
 *
 * common monitor device
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

#ifndef LOADMONITOR_COMMON_KERNEL_H
#define LOADMONITOR_COMMON_KERNEL_H

#include <linux/types.h>
#include <linux/semaphore.h>
#include <loadmonitor_plat.h>

#define LOADMONITOR_OFF		0
#define LOADMONITOR_ON		1

enum LOADMONITOR_ON_OFF_VOTE {
	LOADMONITOR_VOTE_OFF = 0x0,
	LOADMONITOR_VOTE_BY_FREQDUMP = 0x1,
	LOADMONITOR_VOTE_BY_DUBAI = 0x2,
	LOADMONITOR_VOTE_BY_SR = 0x4,
};

extern void __iomem *g_loadmonitor_virt_addr;
extern phys_addr_t loadmonitor_phy_addr;
extern struct loadmonitor_data g_loadmonitor_data_kernel;
extern struct delay_time_with_freq g_monitor_delay_value;
extern struct semaphore g_sem_loadmonior_on_off_vote;
int read_format_cs_loadmonitor_data(u32 flags);
void sec_loadmonitor_switch_disable(u32 en_flags, bool open_sr_ao_monitor, enum LOADMONITOR_ON_OFF_VOTE vote);
void sec_loadmonitor_switch_enable(bool open_sr_ao_monitor, enum LOADMONITOR_ON_OFF_VOTE vote);
void sec_loadmonitor_data_read(unsigned int enable_flags);
noinline int atfd_service_freqdump_smc(u64 _function_id, u64 _arg0,
					    u64 _arg1, u64 _arg2);

#endif
