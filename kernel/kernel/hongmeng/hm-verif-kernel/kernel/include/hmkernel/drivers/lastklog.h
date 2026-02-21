/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Kernel lastklog headfile
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 29 15:32:32 2024
 */

#ifndef HMKERNEL_LASTKLOG_H
#define HMKERNEL_LASTKLOG_H

#include <hmkernel/lastklog.h>
#include <hmkernel/compiler.h>

#ifdef CONFIG_LASTKLOG

void lastklog_save_raw(bool overwrite);

#else

static inline void lastklog_save_raw(__maybe_unused bool overwrite)
{
}

#endif /* CONFIG_LASTKLOG */

#endif
