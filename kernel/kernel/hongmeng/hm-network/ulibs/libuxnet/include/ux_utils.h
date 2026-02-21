/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network utils
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 10:55:32 2019
 */
#ifndef _UX_UTILS_H_
#define _UX_UTILS_H_

#include <time.h>
#include <libhmsync/raw_sem.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread_cond.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>

#define PREFIX_MAX_LEN (32U)

struct ux_wq_entry {
	int wait;
	struct dlist_node node;
};

struct ux_wq_head {
	struct raw_mutex lock;
	struct dlist_node wq;
};

void ux_get_monotonic_time(struct timespec *ts);

void ux_eth_random_addr(unsigned char *addr, size_t addr_size);

int ux_sem_init(struct raw_sem *sem, unsigned int count);
void ux_sem_signal(struct raw_sem *sem);
int ux_sem_cond_waiti(struct raw_sem *sem, time_t timeout);
int ux_acquire_shm_anon(uint64_t shm_key, uint32_t cnode,
			void **rvaddr, size_t *rlen, int *ret_id);
int ux_release_shm_anon(int32_t shm_id, void *vaddr, size_t len);

static inline int ip_addr_match(uint32_t dst_addr, uint32_t src_addr, uint32_t src_mask)
{
	return (dst_addr & src_mask) == (src_addr & src_mask);
}

uint8_t mask_to_prelen(uint32_t mask);
uint32_t prelen_to_mask(uint8_t prefix_len);

void ux_wq_head_init(struct ux_wq_head *waitqueue);
void ux_wq_prepare_wait(struct ux_wq_head *waitqueue, struct ux_wq_entry *waiter);
int ux_wq_do_wait(struct ux_wq_entry* waiter, time_t timeout);
void ux_wq_finish_wait(struct ux_wq_head *waitqueue, struct ux_wq_entry* waiter);

/**
 * Do not call this directly
 */
void __ux_wq_wakeup(struct ux_wq_head *waitqueue, bool all, bool sync);

static inline void ux_wq_wakeup(struct ux_wq_head *waitqueue)
{
	__ux_wq_wakeup(waitqueue, false, false);
}

static inline void ux_wq_wakeup_all(struct ux_wq_head *waitqueue)
{
	__ux_wq_wakeup(waitqueue, true, false);
}

static inline void ux_wq_wakeup_sync(struct ux_wq_head *waitqueue)
{
	__ux_wq_wakeup(waitqueue, false, true);
}

static inline void ux_wq_wakeup_all_sync(struct ux_wq_head *waitqueue)
{
	__ux_wq_wakeup(waitqueue, true, true);
}
#endif
