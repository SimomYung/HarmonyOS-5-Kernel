/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition for type of process
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 28 20:09:06 2023
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROC_TYPE_H
#define SYSMGR_INTAPI_PROCESS_PROC_TYPE_H

enum process_model {
	PROCESS_MODEL_32,
	PROCESS_MODEL_64,
	PROCESS_MODEL_ILP32,
	PROCESS_MODEL_INVAL,
};
#endif
