/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Internel API for ipcmgr shm
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 05 15:41:21 2021
 */
#ifndef SYSMGR_INTAPI_IPC_SHM_H
#define SYSMGR_INTAPI_IPC_SHM_H
#include <stdint.h>

int ipcmgr_shm_get_node_info(int shmid, int *node_id, size_t *node_len);
int ipcmgr_shm_put_node_info(int shmid, int node_id, size_t node_len);
int ipcmgr_shmflg_to_prot(uint32_t shmflg, uint32_t *prot, uint32_t *mode);

#endif
