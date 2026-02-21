/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Process notice function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 16 11:52:01 2020
 */
#ifndef SYSMGR_PROCESS_PROCNOTICE_H
#define SYSMGR_PROCESS_PROCNOTICE_H

#include <stdint.h>
#include <intapi/process/process.h>

void process_notice_exit(const struct process_s *process);
int process_notice_fork(uint32_t src_cidx, uint32_t dst_cidx, uint32_t root_cidx, bool copy_mfile);
void fsmgr_exit_notice(const struct process_s *process);
#endif
