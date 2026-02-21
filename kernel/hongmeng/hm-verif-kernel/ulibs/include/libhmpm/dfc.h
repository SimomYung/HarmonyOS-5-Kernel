/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definition for dfc native api
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 6 14:46:59 2023
 */

#ifndef ULIBS_LIBHMPM_DFC_H
#define ULIBS_LIBHMPM_DFC_H

#include <libbunch/bunch.h>

enum dfc_constraint_reason {
	DFC_CONSTRAINT_SYSFS,
	DFC_CONSTRAINT_DRG,
	DFC_CONSTRAINT_DRIVER,
	DFC_CONSTRAINT_THERMAL,
	DFC_CONSTRAINT_REASON_NR,
};

int hm_dfc_freq_stat(const char *component, struct bunch *bunch);

#endif
