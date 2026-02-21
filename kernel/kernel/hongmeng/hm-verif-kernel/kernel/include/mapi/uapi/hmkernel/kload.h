/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Kernel load infomation
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 8 15:31:13 2022
 */
#ifndef UAPI_HMKERNEL_KLOAD_H
#define UAPI_HMKERNEL_KLOAD_H

#include <hmasm/types.h>

struct kload_s {
	unsigned long long base;
	unsigned long long offset;
	unsigned long long base_len;
};

#ifndef __KSYM_NAME_LEN
#define __KSYM_NAME_LEN 128
#endif

struct __ksyminfo_s {
	__u64 __symbol_addr;
	__s8 __symbol_name[__KSYM_NAME_LEN];
};

#endif
