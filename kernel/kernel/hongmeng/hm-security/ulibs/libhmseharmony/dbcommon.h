/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for policydb common part
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_DBCOMMON_H
#define HMSEL_DBCOMMON_H

#define HMSEL_OBJECT_R	"object_r"
#define HMSEL_OBJECT_R_VAL	1

#define HMSEL_DEFAULT_UND 0
#define HMSEL_DEFAULT_SOURCE 1
#define HMSEL_DEFAULT_TARGET 2

struct hmsel_role_trans_key {
	uint32_t role;
	uint32_t type;
	uint32_t tclass;
};

#endif
