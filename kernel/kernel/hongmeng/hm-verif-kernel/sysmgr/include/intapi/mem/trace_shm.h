/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: trace shm interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 22 21:42:02 2023
 */
#ifndef SYSMGR_MEM_TRACE_SHM_H
#define SYSMGR_MEM_TRACE_SHM_H
#include <intapi/process/process.h>

int trace_shm_init(void);
int trace_shm_snapshot_init(void);
int trace_shm_saved_taskinfo_init(void);

void trace_shm_init_for_process(struct process_s *process, struct vspace_s *vspace);
void trace_shm_snapshot_init_for_process(struct process_s *process, struct vspace_s *vspace);
void trace_shm_saved_taskinfo_init_for_process(struct process_s *process, struct vspace_s *vspace);

int trace_shm_resize(size_t size_kb);
int trace_shm_set_snapshot_status(int action_type);
#endif
