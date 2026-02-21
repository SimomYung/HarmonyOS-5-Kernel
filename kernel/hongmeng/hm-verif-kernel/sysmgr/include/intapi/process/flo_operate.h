/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2024. All rights reserved.
 * Description: frame learning Optimization
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 15 2024
 */
#ifndef FLO_OPERATE_H
#define FLO_OPERATE_H

#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>

#ifdef CONFIG_FLO_CTRL
struct flo_data {
	int pid;
	struct dlist_node *ctx_list;
	raw_atomic_int_t ctx_cnt;
	struct raw_mutex lock;
};

struct flo_cfg {
	int pid;
	int ctx_id;
	unsigned int size;
	unsigned int port;
	unsigned int offset;
	bool ffrt;
	bool init;
};

int do_flo_cfg(struct flo_cfg *cfg);
int handle_flo_ioctl(unsigned int cmd, unsigned long arg);
void process_flo_init(struct process_s *process);
void process_flo_exit(struct process_s *process);
#endif

int dev_flo_ioctl(unsigned int cmd, unsigned long arg);
#endif
