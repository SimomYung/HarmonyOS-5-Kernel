/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _DMA_BUF_INFO_H
#define _DMA_BUF_INFO_H

#include <linux/types.h>
#include <linux/sched.h>
#include <uapi/linux/dma-buf.h>

#define FENCE_NAME_LEN 32

struct dma_buf_info {
	size_t size;
	unsigned long long magic;
	int allocator_pid;
	char allocator_comm[TASK_COMM_LEN];
};

struct dma_fence_info {
	char fence_name[FENCE_NAME_LEN];
	unsigned long i_ino;
	unsigned int total_num_fences;
	unsigned int num_fences;
};

struct fences_info {
	char timeline_name[FENCE_NAME_LEN];
	char driver_name[FENCE_NAME_LEN];
	int status;
	size_t timestamp;
};

struct ion_info {
	size_t size;
	unsigned long i_ino;
	int allocator_tgid;
	int allocator_pid;
	char exp_task_comm[TASK_COMM_LEN];
	char exp_thread_comm[TASK_COMM_LEN];
	char name[DMA_BUF_NAME_LEN];
	char exp_name[DMA_BUF_NAME_LEN];
	bool reclaimable;
	bool is_reclaimed;
};

#endif /* __DMA_BUF_INFO_H__ */
