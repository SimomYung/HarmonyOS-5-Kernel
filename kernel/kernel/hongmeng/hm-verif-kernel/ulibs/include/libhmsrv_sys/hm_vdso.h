/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: hm vdso header file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 18 19:38:19 2021
 */

#ifndef ULIBS_LIBHMSRV_SYS_HM_VDSO_H
#define ULIBS_LIBHMSRV_SYS_HM_VDSO_H

#include <hmasm/libhmsrv_sys/hm_vdso.h>

void *hm_lookup_vdso_sym(const char *name);

#endif
