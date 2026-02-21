/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: The hm_ipc_shm common header defination
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 28 17:38:18 2021
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_SHM_COMMON_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_SHM_COMMON_H

/*
 * NOTE: Now the <libhmsrv_sys/hm_ipc_shm.h> might be included when
 * compiling a liblinux module. To support this, cannot include any
 * unsupported-for-liblinux-module header files here.
 */
#include <hmasm/libhmsrv_sys/hm_ipc_shm.h>

/*
 * Hongmeng native flags for hm_ipc_shmget and hm_ipc_shmat.
 * Don't conflict with the flags in `sys/ipc.h`
 * and `sys/shm.h`.
 */
#define HM_IPC_SHM_POPULATE	0x2000

#endif /* !ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_SHM_COMMON_H */
