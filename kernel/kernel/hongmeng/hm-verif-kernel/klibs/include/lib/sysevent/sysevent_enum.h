/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Sysevent header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 9 20:44:58 2024
 */

#ifndef KLIBS_SYSEVENT_ENUM_H
#define KLIBS_SYSEVENT_ENUM_H

#ifdef __cplusplus
extern "C" {
#endif

enum sysevent_type {
	FAULT = 1,
	STATISTIC = 2,
	SECURITY = 3,
	BEHAVIOR = 4
};

enum value_type {
	INT64 = 8,
	STRING = 12,
};

#ifdef __cplusplus
}
#endif

#endif
