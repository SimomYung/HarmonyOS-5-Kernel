/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2024. All rights reserved.
 * Description: hisilicon ffts_soc_defines.h
 * Create: 2024-08-25
 */

#ifndef __FFTS_SOC_DEFINES_H__
#define __FFTS_SOC_DEFINES_H__

#include "soc_ffts_interface.h"

#define FFTS_KERNEL_REG_LENGTH 0x380
#define FFTS_PMU_EVENT_TYPE_MAX 58

typedef enum {
	FFTS_MEM_USER_REG,
	FFTS_MEM_RTB,
	FFTS_MEM_SQ,
	FFTS_MEM_FUTEX,
	FFTS_MEM_MAX,
} ffts_mem_type;

typedef union {
	unsigned int value;
	struct {
		unsigned int type : 4;
		unsigned int uid : 12;
		unsigned int qos : 3;
		unsigned int rsd : 13;
	} data;
} ffts_acpu_intrq_normal_info_u;
 
typedef union {
	unsigned int value;
	struct {
		unsigned int type : 4;
		unsigned int other : 28;
	} data;
	struct {
		unsigned int type : 4;
		unsigned int uid : 12;
		unsigned int task_idx : 16;
	} data_a;
	struct {
		unsigned int type : 4;
		unsigned int uid : 12;
		unsigned int tid : 16;
	} data_b;
	struct {
		unsigned int type : 4;
		unsigned int uid : 12;
		unsigned int qos : 3;
		unsigned int rsd : 13;
	} data_c;
	struct {
		unsigned int type : 4;
		unsigned int uid : 12;
		unsigned int event_id : 12;
		unsigned int rsd : 4;
	} data_d;
} ffts_acpu_intrq_abnormal_info_u;
 
typedef enum {
	FFTS_INTR_NORMAL_LIST,
	FFTS_INTR_NORMAL_GPU,
	FFTS_INTR_NORMAL_NPU,
	FFTS_INTR_NORMAL_PROC,
	FFTS_INTR_NORMAL_CACHE,
	FFTS_INTR_NORMAL_MAX
} ffts_intr_normal_type;
 
typedef enum {
	FFTS_INTR_ABNORMAL_LIST,
	FFTS_INTR_ABNORMAL_GPU,
	FFTS_INTR_ABNORMAL_NPU,
	FFTS_INTR_ABNORMAL_DFX,
	FFTS_INTR_ABNORMAL_MAX
} ffts_intr_abnormal_type;
 
typedef enum {
	FFTS_INTRQ_ABNORMAL_ACC_ABNM_0 = 0,
	FFTS_INTRQ_ABNORMAL_ACC_ABNM_1,
	FFTS_INTRQ_ABNORMAL_TASKDONE_RELEASE_AUTH_FAIL,
	FFTS_INTRQ_ABNORMAL_RCV_EVENT_AUTH_FAIL,
	FFTS_INTRQ_ABNORMAL_TASKDONE_RELEASE_EXEC_FAIL,
	FFTS_INTRQ_ABNORMAL_SQE_SIG_COLLISSION,
	FFTS_INTRQ_ABNORMAL_SQE_SIGOUT_INVALID,
	FFTS_INTRQ_ABNORMAL_SQE_PROC_EXCEED,
	FFTS_INTRQ_ABNORMAL_SQE_TOTAL_EXCEED,
	FFTS_INTRQ_ABNORMAL_SQE_INVALID,
	FFTS_INTRQ_ABNORMAL_SQE_PTASKID_UID_AUTH_FAIL,
	FFTS_INTRQ_ABNORMAL_SQE_EVENT_AUTH_FAIL,
	FFTS_INTRQ_ABNORMAL_SQE_PTASK_ST_ERR,
	FFTS_INTRQ_ABNORMAL_RTB_EXCEED,
	FFTS_INTRQ_ABNORMAL_TASKSTART_AUTH_FAIL,
	FFTS_INTRQ_ABNORMAL_MAX,
} ffts_intrq_abnormal_type_e;

#endif /* __FFTS_SOC_DEFINES_H__ */