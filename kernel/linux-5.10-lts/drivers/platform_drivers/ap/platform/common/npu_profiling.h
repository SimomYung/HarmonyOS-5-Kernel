/*
 * npu_profiling.h
 *
 * about npu profiling config
 *
 * Copyright (c) 2012-2021 Huawei Technologies Co., Ltd.
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
#ifndef __NPU_PROFILING_H__
#define __NPU_PROFILING_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AICPU_MAX_PMU_NUM      8
#define AI_CORE_MAX_PMU_NUM     8
#define PROF_SETTING_INFO_SIZE  256
#define PROF_HEAD_MANAGER_SIZE  0x1000
#define NPU_MAILBOX_RESERVED_SIZE 256

enum profiling_mode { //用户态 enum ProfilingMode
	PROF_NORMAL_MODE,
	PROF_USER_MODE,
	PROF_MODE_CNT,
};

enum profiling_type {
	PROF_TYPE_TSCPU = 0,
	PROF_TYPE_HWTS_LOG,
	PROF_TYPE_HWTS_PROFILING,
	PROF_TYPE_CNT,
};

enum aicore_profiling_enable_switch {
	AICORE_PROF_ENABLE_TASK_DISABLE = 0,
	AICORE_PROF_ENABLE_TASK_PMU = 1 << 0,
	AICORE_PROF_ENABLE_TASK_LOG = 1 << 1,
	AICORE_PROF_ENABLE_BLOCK_PMU = 1 << 2,
	AICORE_PROF_ENABLE_BLOCK_LOG = 1 << 3,
};

enum profiling_channel {
	PROF_CHANNEL_TSCPU = 0,
	PROF_CHANNEL_AICPU = 1, /* for V100 */
	PROF_CHANNEL_HWTS_LOG = 1, /* for V200 */
	PROF_CHANNEL_AICORE = 2, /* for V100 */
	PROF_CHANNEL_HWTS_PROFILING = 2, /* for V200 */
	PROF_CHANNEL_MAX,
};

struct prof_ts_cpu_config {
	unsigned int en; /* enable=1 or disable=0 */
	unsigned int task_state_switch; /* 1 << TsTaskState means report */
	unsigned int task_type_switch; /* 1 << TsTaskType means report */
};

struct prof_ai_cpu_config {
	unsigned int en;
	unsigned int event_num;
	unsigned int event[AICPU_MAX_PMU_NUM];
};

struct prof_ai_core_config {
	unsigned int en;
	unsigned int event_num;
	unsigned int event[AI_CORE_MAX_PMU_NUM];
	unsigned int mode;
};

struct prof_setting_info {
	struct prof_ts_cpu_config tscpu;
	struct prof_ai_cpu_config aicpu;
	struct prof_ai_core_config aicore;
};

struct profiling_ring_buff_manager {
	volatile unsigned int read;
	volatile unsigned int length;
	volatile unsigned int base_addr_l;
	volatile unsigned int base_addr_h;
	volatile unsigned long long vir_addr;
	unsigned int reserved0[10]; /* because cache line size is 64 bytes */
	volatile unsigned int write;
	volatile unsigned int iova_addr;
	unsigned int reserved1[14]; /* because cache line size is 64 bytes */
};

struct profiling_buff_manager {
	union {
		struct prof_setting_info info;
		char data[PROF_SETTING_INFO_SIZE];
	} cfg;
	struct profiling_ring_buff_manager ring_buff[PROF_TYPE_CNT];
	unsigned int start_flag;
};

struct npu_prof_info {
	union {
		struct profiling_buff_manager manager;
		char data[PROF_HEAD_MANAGER_SIZE];
	} head;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __DRV_PROFILING_H__ */
