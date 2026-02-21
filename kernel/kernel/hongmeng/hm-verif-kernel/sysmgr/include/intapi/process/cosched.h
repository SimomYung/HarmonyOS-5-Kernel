/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Sysmgr booting implement
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 15:48:21 2023
 */

#ifndef SYSMGR_INTAPI_PROCESS_PREMMPT_H
#define SYSMGR_INTAPI_PROCESS_PREMMPT_H

#include <intapi/process/proc_core.h>

int process_cosched_init_pre(struct process_s *process,
		struct cred_mac *new_mac_cred, uintptr_t *new_cosched);
void process_cosched_init_post(struct process_s *process, uintptr_t new_cosched);
int process_cosched_init(struct process_s *process, struct cred_mac *new_mac_cred);
int process_create_cosched(struct process_s *process);
void process_destroy_cosched(struct process_s *process);

#endif
