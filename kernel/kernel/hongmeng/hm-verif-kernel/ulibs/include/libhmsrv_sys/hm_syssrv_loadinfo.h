/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definition of saving loadinfo for system services
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 13 16:24:46 2024
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SYSSRV_LOADINFO_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SYSSRV_LOADINFO_H

#include <sys/types.h>

void hm_syssrv_loadinfo_save(void);
void hm_syssrv_loadinfo_save_pid(pid_t pid);

#endif
