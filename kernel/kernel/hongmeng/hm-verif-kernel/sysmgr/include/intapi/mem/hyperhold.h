/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file of the core of hyperhold definition and related functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 13 12:25:18 2023
 */

#ifndef SYSMGR_INTAPI_MEM_HYPERHOLD_H
#define SYSMGR_INTAPI_MEM_HYPERHOLD_H

#include <libstrict/strict.h>
#include <intapi/res/memory.h>
#include <intapi/process/pid.h>
#include <swap/hyperhold/hp_no_cmp.h>

enum zswap_stat {
	CACHE_SIZE = 0,
	SWAP_SIZE,
	CACHE_PAGE,
	SWAP_PAGE,
	CACHE_FAULT,
	SWAP_FAULT,
	READ_SIZE,
	READ_PAGES,
	WRITE_SIZE,
	WRITE_PAGES,
	PEAK_SWAP_SIZE,
	SWAPIN_CNT,
	SWAPOUT_CNT,
	REOUT_PAGES,
	REOUT_SIZE,
#ifdef CONFIG_HYPERHOLD_COMPRESSION_STATS
	COMP_CNT,
	COMP_LAT,
	DECOMP_CNT,
	DECOMP_LAT,
#endif
	/* number of total reclaim anon pages */
	TOTAL_CACHE_PAGE,
	ZSWAP_STAT_MAX,
};

enum hyperhold_io_latency {
	READ_TOTAL,
	WRITE_TOTAL,
	READ_MAX,
	WRITE_MAX,
	READ_TIMEOUT_CNT,
	WRITE_TIMEOUT_CNT,
	READ_AVG,
	WRITE_AVG,
};

enum hyperhold_latency {
	TOTAL_LATENCY,
	MAX_LATENCY,
	TIMEOUT_CNT,
};

enum hyperhold_scenario {
	HYPERHOLD_RECLAIM_IN = 0,
	HYPERHOLD_FAULT_OUT,
	HYPERHOLD_BATCH_OUT,
	HYPERHOLD_SCENARIO_BUTT,
};

enum mem_info {
	MEM_AVAILABLE = 0,
	MEM_FREE,
	BUFFER_SIZE,
	INACTIVE_FILE,
	ACTIVE_FILE,
	INACTIVE_ANON,
	ACTIVE_ANON,
};

#ifdef CONFIG_ZSWAP_GROUP
uint64_t zswapin_memcg(struct memcg_s *memcg, uint64_t req_size);
uint64_t zswapout_memcg(struct memcg_s *memcg, uint64_t req_size);
uint64_t memcg_data_size(struct memcg_s *memcg, int type, bool compress_flag);
uint64_t memcg_total_data_size(int type);
pid_t get_zswapd_pid(void);
bool need_to_report_critical(void);
void zswapout_bg_tasks(struct memcg_s *mcg, unsigned int type, unsigned int param);
int zswapd_get_empty_round_skip_interval(void);
void zswapd_set_empty_round_skip_interval(int skip_interval);
int zswapd_get_max_skip_interval(void);
void zswapd_set_max_skip_interval(int skip_interval);
int zswapd_get_empty_round_check_threshold(void);
void zswapd_set_empty_round_check_threshold(int threshold);
void zswapd_update_empty_skip_interval(void);
void zswapd_clear_empty_skip_interval(void);
#else  /* !CONFIG_ZSWAP_GROUP */
static inline uint64_t zswapin_memcg(struct memcg_s *memcg, uint64_t req_size)
{
	UNUSED(memcg, req_size);
	return E_HM_OK;
}
static inline uint64_t zswapout_memcg(struct memcg_s *memcg, uint64_t req_size)
{
	UNUSED(memcg, req_size);
	return E_HM_OK;
}
static inline uint64_t memcg_data_size(struct memcg_s *memcg, int type, bool compress_flag)
{
	UNUSED(memcg, type);
	return E_HM_OK;
}
static inline uint64_t memcg_total_data_size(int type)
{
	UNUSED(type);
	return E_HM_OK;
}
static inline pid_t get_zswapd_pid(void)
{
	return PID_NONE;
}
static inline bool need_to_report_critical(void)
{
	return false;
}
static inline void zswapout_bg_tasks(struct memcg_s *mcg, unsigned int type, unsigned int param)
{
	UNUSED(mcg, type, param);
}
static inline int zswapd_get_empty_round_skip_interval(void)
{
	return 0;
}
static inline void zswapd_set_empty_round_skip_interval(int skip_interval)
{
	UNUSED(skip_interval);
}
static inline int zswapd_get_max_skip_interval(void)
{
	return 0;
}
static inline void zswapd_set_max_skip_interval(int skip_interval)
{
	UNUSED(skip_interval);
}
static inline int zswapd_get_empty_round_check_threshold(void)
{
	return 0;
}
static inline void zswapd_set_empty_round_check_threshold(int threshold)
{
	UNUSED(threshold);
}
static inline void zswapd_update_empty_skip_interval(void)
{
}
static inline void zswapd_clear_empty_skip_interval(void)
{
}
#endif  /* CONFIG_ZSWAP_GROUP */

#ifdef CONFIG_HYPERHOLD
int hyperhold_init(void);
void hyperhold_exit(void);
void hyperhold_disable(bool force);
bool hyperhold_is_full(bool compress_flag);
bool hyperhold_is_enable(bool compress_flag);
bool hyperhold_is_inited(bool compress_flag);
void hyperhold_enable(void);
uint64_t hyperhold_io_latency_show(enum hyperhold_io_latency io_latency, bool compress_flag);
uint64_t hyperhold_total_latency_show(enum hyperhold_scenario scenario, enum hyperhold_latency latency,
			bool compress_flag);
uint64_t hyperhold_psi_health_mem_info_show(enum mem_info info);
char *hyperhold_device(bool compress_flag);
struct raw_mutex *hyperhold_global_mutex(bool compress_flag);
int hyperhold_set_space_size(unsigned long space_size);
uint32_t hyperhold_extent_size(bool compress_flag);
int hyperhold_remove_files(void);
#else /* !CONFIG_HYPERHOLD */
static inline int hyperhold_init(void)
{
	return E_HM_OK;
}
static inline void hyperhold_exit(void) {}
static inline void hyperhold_disable(bool force)
{
	UNUSED(force);
}
static inline bool hyperhold_is_full(bool compress_flag)
{
	UNUSED(compress_flag);
	return false;
}
static inline bool hyperhold_is_enable(bool compress_flag)
{
	UNUSED(compress_flag);
	return false;
}
static inline bool hyperhold_is_inited(bool compress_flag)
{
	UNUSED(compress_flag);
	return false;
}
static inline void hyperhold_enable(void) {}
static inline uint64_t hyperhold_io_latency_show(enum hyperhold_io_latency io_latency, bool compress_flag)
{
	UNUSED(io_latency, compress_flag);
	return E_HM_OK;
}
static inline uint64_t hyperhold_total_latency_show(enum hyperhold_scenario scenario, enum hyperhold_latency latency)
{
	UNUSED(scenario, latency);
	return E_HM_OK;
}
static inline uint64_t hyperhold_psi_health_mem_info_show(enum mem_info info)
{
	UNUSED(info);
	return E_HM_OK;
}
static inline char *hyperhold_device(bool compress_flag)
{
	UNUSED(compress_flag);
	return NULL;
}
static inline struct raw_mutex *hyperhold_global_mutex(bool compress_flag)
{
	UNUSED(compress_flag);
	return NULL;
}
static inline int hyperhold_set_space_size(unsigned long space_size)
{
	UNUSED(space_size);
	return E_HM_OK;
}
static inline uint32_t hyperhold_extent_size(bool compress_flag)
{
	UNUSED(compress_flag);
	return 0U;
}
static inline int hyperhold_remove_files(void)
{
	return E_HM_OK;
}
#endif /* CONFIG_HYPERHOLD */

#ifdef CONFIG_HYPERHOLD_CACHE
unsigned long hyperhold_cache_extent_num(bool compress_flag);
uint32_t hyperhold_cache_level_get(bool compress_flag);
int hyperhold_cache_level_set(uint32_t level, bool compress_flag);
#else
static unsigned long hyperhold_cache_extent_num(bool compress_flag)
{
	UNUSED(compress_flag);
	return 0UL;
}
static uint32_t hyperhold_cache_level_get(bool compress_flag)
{
	UNUSED(compress_flag);
	return 0UL;
}
static int hyperhold_cache_level_set(uint32_t level, bool compress_flag)
{
	UNUSED(level);
	UNUSED(compress_flag);
	return 0;
}
#endif /* CONFIG_HYPERHOLD_CACHE */

#endif /* SYSMGR_INTAPI_MEM_HYPERHOLD_H */
