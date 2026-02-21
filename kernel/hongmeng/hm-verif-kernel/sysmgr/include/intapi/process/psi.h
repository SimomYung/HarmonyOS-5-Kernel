/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: psi head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 26 20:06:09 2023
 */

#ifndef SYSMGR_INTAPI_PROCESS_PSI_H
#define SYSMGR_INTAPI_PROCESS_PSI_H

#include <stdint.h>

struct psi_avg_s {
	uint64_t avg10;
	uint64_t avg60;
	uint64_t avg300;
};

#endif
