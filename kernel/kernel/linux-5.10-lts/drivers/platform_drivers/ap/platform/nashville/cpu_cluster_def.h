/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: platform cpu's cluster define
 * Create: 2021/03/24
 * Notes: used for xloader/lpmcu/tf-a code
 */
#ifndef __CPU_CLUSTER_DEF_H__
#define __CPU_CLUSTER_DEF_H__

#if defined(__LINKER__) || defined(__ASSEMBLER__) || defined( __SOC_H_FOR_ASM__)
#define CPU_LIT_CLUSTER		0
#define CPU_MID_CLUSTER		1
#define CPU_BIG_CLUSTER		2
#else
enum cpu_cluster_def {
	CPU_LIT_CLUSTER = 0,
	CPU_MID_CLUSTER, /* 1 */
	CPU_BIG_CLUSTER, /* 2 */
	CPU_CLUSTER_CNT, /* 3 */
	CPU_FCM_CLUSTER = CPU_CLUSTER_CNT, /* 3 */
	CPU_SL2_CLUSTER = CPU_CLUSTER_CNT, /* 3 */
	CPU_FCM_ALL_CNT = CPU_CLUSTER_CNT, /* 3 */
	CPU_HC_CLUSTER, /* 4 */
	CPU_CLUSTER_ALL_CNT, /* 5 */
	CPU_MEM_DEVICE = 7, /* MUST be SAME as lpmcu defination */
};

enum cpu_freq_domain {
	FREQ_DOMAIN_LIT = 0,
	FREQ_DOMAIN_MID,
	FREQ_DOMAIN_BIG,
	FREQ_DOMAIN_MAX,
};
#endif
#endif /* __CPU_CLUSTER_DEF_H__ */
