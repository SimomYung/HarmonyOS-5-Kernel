/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Define type of val in pgstr table which is arch related (arm64) - uapi.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 03 04:03:59 2022
 */

#ifndef AARCH64_MAPI_UAPI_HMASM_PGSTR_BASICTYPES_H
#define AARCH64_MAPI_UAPI_HMASM_PGSTR_BASICTYPES_H

#include <hmkernel/types.h>

/*
 * This defines API created by hmkernel/pgstr/types.h,
 * decode and populate.
 * Use macro through builder and API layer so pgstrdesc is allowed to directly
 * use rawtype (__u64 and __u32), avoid type casting from/to __arch_pgstr_val_t.
 * Provide a __arch_pgstr_val_t so user is easier to use.
 *
 * Avoid using decode_val and populate_val in production code. Use entry.e.u.ptr. Sometypes
 * it is required to be casted to atomic values.
 */
#define __ARCH_PGSTR_VAL_TYPE	__u64
#define __ARCH_PGSTR_VAL_INVAL ((__u64)(0u))

typedef __ARCH_PGSTR_VAL_TYPE __arch_pgstr_val_t;

#endif
