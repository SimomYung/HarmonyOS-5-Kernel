/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declare interface of slab user track dfx
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 05 20:13:38 UTC 2024
 */

#ifndef __ULIBS_INCLUDE_LIBMEM_SLAB_TRACK_H__
#define __ULIBS_INCLUDE_LIBMEM_SLAB_TRACK_H__

#include <unistd.h>
#include <stdbool.h>

#define SLAB_DEBUG_COMM_LEN	16
#define SLAB_DEBUG_OFFSET	8

#define SLAB_DEBUG_TRACE_NR 5
struct __attribute__((aligned(SLAB_DEBUG_OFFSET))) slab_user_info {
	char comm[SLAB_DEBUG_COMM_LEN];
	pid_t tid;
	pid_t pid;
	uintptr_t trace[SLAB_DEBUG_TRACE_NR];
#ifdef CONFIG_PER_CPU_SLAB
	bool in_cache;
#endif
};

#ifdef CONFIG_SLAB_DEBUG_USAGE
#define SLAB_DEBUG_SIZE sizeof(struct slab_user_info)
#else
#define SLAB_DEBUG_SIZE 0
#endif

struct track_info {
	unsigned long count;
	pid_t pid;
	pid_t tid;
	char comm[SLAB_DEBUG_COMM_LEN];
	uintptr_t trace[SLAB_DEBUG_TRACE_NR];
};

struct track_head;
struct process_s;
typedef int (*slab_count_func_t)(struct track_head *th, const struct slab_user_info *info);

struct track_head {
	unsigned long max;
	unsigned long items;
	unsigned long slots;
#ifdef CONFIG_PER_CPU_SLAB
	unsigned long caches;
#endif
	struct track_info *ti;
	slab_count_func_t count_usage;
};

struct slab_ator;

int slab_init_track(struct track_head *th, unsigned long max, slab_count_func_t count_func);
void slab_free_track(struct track_head *th);
void slab_dump_ator_usage(struct track_head *th, struct slab_ator *ator, uint32_t s_flags);
int memm_slab_dump_usage(struct track_head *th, uint32_t s_flags, const char *name);
int mcache_dump_usage(struct track_head *th, uint32_t s_flags, const char *name);
int slab_count_amount(struct track_head *th, const struct slab_user_info *info);
int slab_count_trace(struct track_head *th, const struct slab_user_info *info);
void set_in_cache_mark(struct slab_ator *s_ator, void *slot);
void clear_in_cache_mark(struct slab_ator *s_ator, void *slot);
bool is_obj_in_cache(struct slab_ator *s_ator, void *slot);
struct process_s *slab_fill_user_info(struct slab_user_info *info, uint32_t s_flags);
void slab_store_user_info(struct slab_ator *s_ator, void *slot);
void slab_clear_user_info(struct slab_ator *s_ator, void *slot, pid_t *rpid);
uint32_t slab_userzone_calc_obj_size(uint32_t size, uint32_t s_flags);

#endif /* __ULIBS_INCLUDE_LIBMEM_SLAB_TRACK_H__ */
