/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: platform cpu's cluster define
 * Create: 2021/04/15
 * Notes: used for xloader/lpmcu/tf-a code
 */
#ifndef __CPU_CLUSTER_DEF_H__
#define __CPU_CLUSTER_DEF_H__

enum cpu_cluster_def {
	CPU_LIT_CLUSTER = 0,
	CPU_MID_CLUSTER,
	CPU_BIG_CLUSTER,
	CPU_CLUSTER_CNT,
	CPU_FCM_CLUSTER = CPU_CLUSTER_CNT,
	CPU_FCM_ALL_CNT
};

#endif
