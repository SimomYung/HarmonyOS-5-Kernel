/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: psi memory head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 17 15:23:15 2023
 */

#ifndef SYSMGR_MEMMGR_MEM_PRESSURE_H
#define SYSMGR_MEMMGR_MEM_PRESSURE_H

#include <stdint.h>
#include <time.h>

#include <libhmsrv_sys/hm_select_common.h>
#include <intapi/process/psi.h>

enum psi_mem_type_e {
	PSI_MEM_SOME,
	PSI_MEM_FULL,
	PSI_TYPE_MAX,
};

enum psi_reason_e {
	PSI_SYNC_RECLAIM,
	PSI_ASYNC_RECLAIM,
	PSI_ASYNC_RECLAIM_ANON,
	PSI_LOW_MEMORY,
	PSI_MEM_COMP,
	PSI_ALLOC_FAILED,
	PSI_REASON_MAX,
};

#define OH_LMK_KILL_WATERMARK_MB	1638 /* MB 1.6G */
#define PSI_MEM_RECORD_TIME_MAX_CNT	5
#define RECLAIM_SERVICE_TIME_THRESHOLD 100000 /* 100ms */
#define RECLAIM_LRU_ANON_TIME_THRESHOLD 200000 /* 200ms */
#define PSI_MEM_LOG_RATE_MS 1000
#define PSI_MEM_LOG_RATE_CNT 1

struct psi_mem_record_s {
	bool is_sleep[PSI_MEM_RECORD_TIME_MAX_CNT];
	struct timespec rtime[PSI_MEM_RECORD_TIME_MAX_CNT];
	unsigned long avail_size[PSI_MEM_RECORD_TIME_MAX_CNT];
	unsigned int rtime_pos;
};

int psi_mem_register(enum psi_mem_type_e type,
		     uint64_t threshold_time, uint64_t window_time,
		     unsigned int cnode_idx);
int psi_mem_acquire(struct psi_avg_s *some,
		    struct psi_avg_s *full);
void psi_mem_stat_update(bool is_sleep);
void psi_epoll_wake(void);
void psi_epoll_write_windows_time(unsigned long long v);
unsigned long long psi_epoll_read_windows_time(void);
#ifdef CONFIG_PSI_MEM
void psi_mem_init(void);
void psi_mem_unregister(unsigned int cnode_idx);
void psi_mem_thread_wakeup(void);
void psi_mem_thread_wakeup_by_reason(enum psi_reason_e reason);
void psi_mem_stat_dump(void);
unsigned long pressure_avail_buffer_size(void);
unsigned long pressure_avail_buffer_size_lite(uint64_t sysfree, uint64_t cache);
unsigned long pressure_avail_buffer_size_raw(uint64_t devhost_avail_size);
unsigned long pressure_stability_rollup_size(void);
int psi_epoll_control(struct epoll_info_arg *ep_arg);
void psi_epoll_remove(cref_t ep_cref);
int psi_epoll_recheck_revents(uint32_t events, uint32_t *revents);
#else /* !CONFIG_PSI_MEM */
static inline void psi_mem_init(void)
{
}
static inline void psi_mem_unregister(unsigned int cnode_idx)
{
	UNUSED(cnode_idx);
}
static inline void psi_mem_thread_wakeup(void)
{
}
static inline void psi_mem_thread_wakeup_by_reason(enum psi_reason_e reason)
{
}
static inline void psi_mem_stat_dump(void)
{
}
static inline unsigned long pressure_avail_buffer_size(void)
{
	return 0UL;
}
static inline unsigned long pressure_avail_buffer_size_lite(uint64_t sysfree, uint64_t cache)
{
	UNUSED(sysfree, cache);
	return 0UL;
}
static inline unsigned long pressure_avail_buffer_size_raw(uint64_t devhost_avail_size)
{
	UNUSED(devhost_avail_size);
	return 0UL;
}
static inline unsigned long pressure_stability_rollup_size(void)
{
	return 0UL;
}
static inline int psi_epoll_control(struct epoll_info_arg *ep_arg)
{
	UNUSED(ep_arg);
	return E_HM_OK;
}
static inline void psi_epoll_remove(cref_t ep_cref)
{
	UNUSED(ep_cref);
}
static inline int psi_epoll_recheck_revents(uint32_t events, uint32_t *revents)
{
	UNUSED(events, revents);
	return E_HM_OK;
}
#endif /* CONFIG_PSI_MEM */

#endif /* SYSMGR_MEMMGR_MEM_PRESSURE_H */
