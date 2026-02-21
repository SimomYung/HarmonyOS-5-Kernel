/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Memory Control
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 23 18:55:24 2021
 */
#ifndef SYSMGR_MEM_RECLAIM_H
#define SYSMGR_MEM_RECLAIM_H

#include <libhmsrv_sys/hm_timer.h>
#include <libhwsecurec/securec.h>
#include <lib/dlist.h>

#include <intapi/process/service.h>
#include <intapi/res/memory.h>
#include <intapi/mem/reclaim_shrink.h>
#include <intapi/mem/reclaim_swappiness.h>
#include <intapi/mem/reclaim_dfx.h>

enum reclaim_process_event {
	/* for /proc/pid/reclaim */
	RECLAIM_PROCESS_PAGE_FILE = 1, /* 1 for file */
	RECLAIM_PROCESS_PAGE_ANON, /* 2 for anon */
	RECLAIM_PROCESS_PAGE_ALL, /* 3 for all */
	RECLAIM_PROCESS_PRIVATE_PAGE_FILE, /* 4 for private file */
	RECLAIM_PROCESS_BUTT,

	/* for madvise */
	RECLAIM_MADVISE_PAGEOUT = RECLAIM_PROCESS_BUTT,
};

#define TIME_DIFF_NS(end, start) \
	(unsigned long)((end).tv_sec * NSEC_PER_SEC + (end).tv_nsec - \
	((start).tv_sec * NSEC_PER_SEC + (start).tv_nsec))

#define TIME_DIFF_US(end, start) ((TIME_DIFF_NS((end), (start))) / NSEC_PER_USEC)
#define TIME_DIFF_MS(end, start) ((TIME_DIFF_NS((end), (start))) / NSEC_PER_MSEC)

#define TIME_NS_FROM_SPEC(time) ((time).tv_sec * NSEC_PER_SEC + (time).tv_nsec)
#define TIME_US_FROM_SPEC(time) (TIME_NS_FROM_SPEC(time) / NSEC_PER_USEC)
#define TIME_MS_FROM_SPEC(time) (TIME_NS_FROM_SPEC(time) / NSEC_PER_MSEC)

size_t reclaim_pages_memcg(struct memcg_s *memcg, size_t nr_to_reclaim);
int reclaim_min_free_kbytes_write(size_t min_free_kbytes);
size_t reclaim_min_free_kbytes_read(void);
int reclaim_watermark_scale_factor_write(size_t value);
size_t reclaim_watermark_scale_factor_read(void);

#ifdef CONFIG_SWAP
int min_swap_kbytes_write(size_t min_swap_kbytes);
size_t min_swap_kbytes_read(void);
#else /* CONFIG_SWAP */
static inline int min_swap_kbytes_write(size_t min_swap_kbytes)
{
	UNUSED(min_swap_kbytes);
	return E_HM_NOSYS;
}

static inline size_t min_swap_kbytes_read(void)
{
	return 0UL;
}
#endif /* CONFIG_SWAP */

enum reclaim_wmark_type {
	RECLAIM_WMARK_MIN,
	RECLAIM_WMARK_LOW,
	RECLAIM_WMARK_HIGH,
	RECLAIM_WMARK_ANON, /* history watermark, to be deleted in the future */
	RECLAIM_WMARK_SWAP, /* history watermark, to be deleted in the future */
	RECLAIM_WMARK_LMK, /* history watermark, to be deleted in the future */
	RECLAIM_WMARK_DEVHOST, /* history watermark, to be deleted in the future */
	RECLAIM_WMARK_SLAB, /* history watermark, to be deleted in the future */
	RECLAIM_WMARK_NR
};

/* AvailBuffer watermark management functions */
size_t reclaim_wmark_avail_buf_read(void);
size_t reclaim_wmark_min_avail_buf_read(void);
size_t reclaim_wmark_high_avail_buf_read(void);
size_t reclaim_wmark_free_swap_threshold_read(void);
uint64_t reclaim_slab_watermark_read(void);
int reclaim_wmark_avail_buffers_write(size_t avail_buf_mb, size_t min_avail_buf_mb,
				      size_t high_avail_buf_mb, size_t free_swap_threshold_mb);
uint64_t reclaim_watermark_read(enum reclaim_wmark_type type);
void reclaim_watermark_write(enum reclaim_wmark_type type, uint64_t val);
void check_avail_buf_wmark(uint64_t sys_free);
bool check_avai_buf_wmark_valid(void);
uint64_t get_last_avail_buffer(void);
void set_last_avail_buffer(uint64_t buffer);

size_t reclaim_thread_sleep_ms_read(void);
int reclaim_thread_sleep_ms_write(size_t sleep_ms);
int reclaim_mm_eval_read(char *data, size_t data_len);
int reclaim_mm_eval_write(uint64_t pos, const char *src, size_t size, size_t *wsize);
void reclaim_process_pages(struct process_s *p, int type);
#ifdef CONFIG_HISI_SYSMGR_PARA_RECLAIM
int reclaim_para_anon_stat_read(char *data, size_t data_len);
int set_para_anon_enable(int set_val);
int get_para_anon_enable(void);
#endif
void reclaim_context_set_flag(uint32_t flag);

uint64_t reclaim_query_pages_all(void);
#ifdef CONFIG_SYSMGR_PM_HIBERNATE
uint64_t hibernate_reclaim_memory(void);
bool hibernate_reclaim_enable(void);
void hibernate_reclaim_enter(void);
void hibernate_reclaim_leave(void);
#else
static inline uint64_t hibernate_reclaim_memory(void)
{
	return 0UL;
}
static inline bool hibernate_reclaim_enable(void)
{
	return false;
}
static inline void hibernate_reclaim_enter(void)
{
}
static inline void hibernate_reclaim_leave(void)
{
}
#endif
#endif
