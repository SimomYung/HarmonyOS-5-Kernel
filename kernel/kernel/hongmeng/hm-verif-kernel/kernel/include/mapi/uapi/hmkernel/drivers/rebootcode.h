/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Definition of rebootcode check driver
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 20 16:14:10 2022
 */
#ifndef UAPI_HMKERNEL_DRIVERS_REBOOTCODE_H
#define UAPI_HMKERNEL_DRIVERS_REBOOTCODE_H

#include <hmkernel/types.h>

#define REBOOT_STR_MAX_LEN			(64)

#define REBOOTCODE_KBOX_REGION_NAME_PREFIX 	"rebootcode"
#define REBOOTCODE_KBOX_STR_SIZE 		(40)

struct rebootcode_record_s {
	__u32 reboot_index;
	__u32 reboot_code;
	__s64 time_sec;
	__s64 time_nsec;
	__u8  str[REBOOTCODE_KBOX_STR_SIZE];
};

#endif
