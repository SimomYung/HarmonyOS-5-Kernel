/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for pathmgr hongmeng native api
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 17 13:19:10 2019
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_PATH_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_PATH_H

#include <sys/stat.h>
#include <sys/types.h>
#include <hmkernel/capability.h>
#include <libbunch/bunch.h>
#include "hm_stat.h"

int hm_path_add(cref_t cref, unsigned long grant_info, const char *path);
int hm_path_add_with_mode(cref_t cref, unsigned long grant_info, const char *path, mode_t mode);
int hm_path_delete(const char *path);
int hm_path_acquire(const char *path, rref_t *rref);
int hm_path_release(const char *path, rref_t rref);
int hm_path_stat(const char *path, struct bunch *bunch);
int hm_path_chmod(const char *path, mode_t mode);

/*
 * This interface will be deprecated, for that now the srv would be
 * grant to sysmgr in hm_path_add when needed.
 */
int hm_path_grant(const char *path);

#endif
