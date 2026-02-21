/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Sysevent header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 9 20:44:58 2024
 */

#ifndef KLIBS_SYSEVENT_H
#define KLIBS_SYSEVENT_H

#include <stddef.h>
#include <hmkernel/types.h>
#ifdef __KERNEL__
#include <hmkernel/macro.h>
#include <hmkernel/errno.h>
#include <hmkernel/bitops/bits.h>
#include <hmkernel/atomic.h>
#include <lib/string.h>
#else
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
#include <hongmeng/macro.h>
#include <hongmeng/errno.h>
#include <libhmsync/atomic.h>

#endif /* __KERNEL__ */

#include <lib/sysevent/sysevent_enum.h>
#include <lib/sysevent/sysevent_macro.h>
#include <lib/sysevent/sysevent_struct.h>
#include <lib/sysevent/sysevent_data.h>

#endif
