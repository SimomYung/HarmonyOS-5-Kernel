/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header files of commom functions of kernfslib
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 13:40:20 2020
 */

#ifndef LIBHMSRV_KERNFS_COMMON_H
#define LIBHMSRV_KERNFS_COMMON_H

#include <libhmucap/ucap.h>

int kernfslib_get_rref(const char *path, rref_t *fs_rref);

#endif
