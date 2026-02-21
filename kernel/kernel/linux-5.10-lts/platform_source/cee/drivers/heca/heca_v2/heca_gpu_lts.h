/*
 * heca_gpu_lts.c
 *
 * hisilicon efficinecy control algorithm lts.
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

#ifndef _HECA_GPU_LTS_H_
#define _HECA_GPU_LTS_H_

struct heca_gpu_conf {
	u32 gpu_pred_instr;
	u32 gpu_allow_time;
	u32 avg_frame_len;
	u8 boost_flag;
	u8 gpu_load;
	u32 period_time;
};

#ifdef CONFIG_HECA_GPU_LTS

#define HECA_CLUSTER_NUM	3

#ifdef CONFIG_NR_CPUS
#define HECA_MAX_NR_CPUS 	CONFIG_NR_CPUS
#else
#define HECA_MAX_NR_CPUS 	12
#endif

#define HECA_FREQ_HZ_PER_MHZ		(1000000)

enum heca_freq_id {
	HECA_FREQ_CPU,
	HECA_FREQ_HC,
	HECA_FREQ_DDR,
	HECA_FREQ_QICE,
	HECA_FREQ_MAX,
	HECA_FREQ_GPU = HECA_FREQ_MAX,
};

struct heca_freq_stat {
	u8 cpu_id;
	u8 class;
	u16 cal_cap;
	u16 real_cap;
	u16 nonidle_cap;
	u16 min_freq;
	u16 max_freq;
	u16 vote_freq[HECA_FREQ_MAX];
	u16 final_freq[HECA_FREQ_MAX];
	u32 count_num[2];
};

struct heca_gpu_capacity {
	u8 class;
	u8 gpu_load;
	u16 cal_cap;
	u16 ave_freq;
	u16 min_freq;
	u16 max_freq;
	u16 final_freq;
    u16 ddr_freq;
	u8 new_frame;
};

struct heca_capacity {
	struct heca_freq_stat freq_stat[HECA_MAX_NR_CPUS];
	unsigned int cal_capacity[HECA_MAX_NR_CPUS];
	unsigned int cpu_min_freq_mhz[HECA_CLUSTER_NUM];
	unsigned int cpu_max_freq_mhz[HECA_CLUSTER_NUM];
	struct heca_gpu_capacity gpu_cap_msg;
};
void heca_gpu_init(void);
void set_gpu_heca_conf(unsigned long arg);
void set_gpu_heca_state(unsigned long arg);
void heca_start_timer(enum heca_ipc_id type);
void heca_stop_timer(enum heca_ipc_id type);
#else
static inline void heca_gpu_init(void) {};
static inline void set_gpu_heca_conf(unsigned long arg) {};
static inline void set_gpu_heca_state(unsigned long arg) {};
#endif

#endif