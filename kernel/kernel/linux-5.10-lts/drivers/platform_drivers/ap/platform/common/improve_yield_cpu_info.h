/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: improve_yield_common_info header file
 * Create: 2025-06-25
 */
#ifndef __IMPROVE_YIELD_CPU_INFO_H__
#define __IMPROVE_YIELD_CPU_INFO_H__

#include <cpu_cluster_def.h>

typedef union {
	unsigned int value;
	struct {
		unsigned int reserved         :  5; /* bit[0-4] reserved */
		unsigned int all_freq_pv_flag :  1; /* bit[5] is all freq boost volt flag? 1: yes 0:no */
		unsigned int pv_flag          :  1; /* bit[6] is cur freq boost volt flag? 1: yes 0:no */
		unsigned int enable_flag      :  1; /* bit[7] is enable flag, 1:enable 0:disable */
		unsigned int freq_ce_mask     : 24; /* bit[8-31] freq ce mask, per bit: freq happen ecc ce? 1: yes 0:no */
	} bits;
} improve_yield_cluster_ecc_nv_t;

typedef struct {
	improve_yield_cluster_ecc_nv_t cluster_ecc_info[CPU_CLUSTER_CNT];
} improve_yield_cpu_ecc_nv_t;

typedef union {
	unsigned char value;
	struct {
		unsigned char reserved         :  6; /* bit[0-5] reserved */
		unsigned char pv_flag          :  1; /* bit[6] is cluster boost volt flag? 1: yes 0:no */
		unsigned char enable_flag      :  1; /* bit[7] is enable flag, 1:enable 0:disable */
	} bits;
} improve_yield_cluster_panic_nv_t;

typedef struct {
	improve_yield_cluster_panic_nv_t cluster_panic_info[CPU_CLUSTER_CNT];
} improve_yield_cpu_panic_nv_t;

#endif /* __IMPROVE_YIELD_CPU_INFO_H__ */