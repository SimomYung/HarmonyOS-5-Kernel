/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: get cpu rt freq and volt info from rdr
 * Create: 2024-10-11
 */
#ifndef __RDR_CPU_RT_FV_PARAMS_H__
#define __RDR_CPU_RT_FV_PARAMS_H__

#define CPU_FV_INFO_LOGIC_BASE 300 /* mV */
#define CPU_FV_INFO_LOGIC_STEP 5 /* mV */
#define CPU_FV_INFO_MEM_BASE 600 /* mV */
#define CPU_FV_INFO_MEM_STEP 10 /* mV */
#define CUR_FREQ_OFFSET 0
#define CUR_LOGIC_OFFSET 1
#define CUR_MEM_OFFSET 2
#define LAST_FREQ_OFFSET 3
#define TAR_FREQ_OFFSET 4
#define MAX_PROF_NUM_OFFSET 5
#define CUR_FREQ_INDEX_OFFSET 6
#define CPU_RT_FV_SHM_SIZE 30
#define CPU_RT_FV_BUFFER_SIZE 300
#define EACH_CLUSTER_FV_INFO_ITEM 7
#define CPU_MAX_PROFILE_NUMBER 25

#endif /* __RDR_CPU_RT_FV_PARAMS_H__ */
