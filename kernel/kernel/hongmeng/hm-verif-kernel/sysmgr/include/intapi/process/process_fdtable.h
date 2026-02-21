/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Process fdtable function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 12 08:45:44 2022
 */
#ifndef SYSMGR_PROCESS_PROCFDTABLE_H
#define SYSMGR_PROCESS_PROCFDTABLE_H

#include <stdint.h>
#include <intapi/process/process.h>

typedef void (*fdtable_callback)(struct process_s *process, int fd, void *args);

struct trav_fdtable_s {
	fdtable_callback cb;
	void *args;
};

int process_fdtable_grant_fdtable(const struct process_s *src_process,
				  rref_t rref, uint8_t rights);
int process_create_fdtable(struct process_s *process);

void process_delete_fdtable(struct process_s *process);

int process_fdtable_set_rlimit(const struct process_s *process, unsigned long long limit);

int process_fdtable_traverse_with_callback(void *ctx);

int fdtable_traverse_in_process_with_callback(struct process_s *process, void *ctx);
int traverse_each_fd_with_callback(struct process_s *process, void *ctx);
#endif
