/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MEM_ZONE_H
#define HVGR_MEM_ZONE_H

#include "hvgr_defs.h"
/* gpu mem addr manage */
/* replay mem: 0x7E 0000 0000 ~ 0x7F FFFF FFFF */
#define HVGR_MEM_REPLAY_END ((uint64_t)0x8000000000)
#define HVGR_MEM_REPLAY_SIZE  ((uint64_t)8 * 1024 * 1024 * 1024)
#define HVGR_MEM_REPLAY_START (HVGR_MEM_REPLAY_END - HVGR_MEM_REPLAY_SIZE)

/* jit mem: 0x7C 0000 0000 ~ 0x7D FFFF FFFF */
#define HVGR_MEM_JIT_END HVGR_MEM_REPLAY_START
#define HVGR_MEM_JIT_SIZE  ((uint64_t)8 * 1024 * 1024 * 1024)
#define HVGR_MEM_JIT_START (HVGR_MEM_JIT_END - HVGR_MEM_JIT_SIZE)

/* import mem: 0x79 0000 0000 ~ 0x7B FFFF FFFF */
#define HVGR_MEM_IMPORT_END HVGR_MEM_JIT_START
#define HVGR_MEM_IMPORT_SIZE  ((uint64_t)12 * 1024 * 1024 * 1024)
#define HVGR_MEM_IMPORT_START (HVGR_MEM_IMPORT_END - HVGR_MEM_IMPORT_SIZE)

#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
/* gpu mem: 0x71 C000 0000 ~ 0x78 FFFF FFFF */
#define HVGR_MEM_GPU_END  HVGR_MEM_IMPORT_START
#define HVGR_MEM_GPU_SIZE  ((uint64_t)29 * 1024 * 1024 * 1024)
#define HVGR_MEM_GPU_START (HVGR_MEM_GPU_END - HVGR_MEM_GPU_SIZE)

/* reserve mcu mem: 0x71 0000 0000 ~ 0x71 BFFF FFFF */
#define HVGR_MEM_MCU_END  HVGR_MEM_GPU_START
#define HVGR_MEM_MCU_SIZE  ((uint64_t)3 * 1024 * 1024 * 1024)
#define HVGR_MEM_MCU_START (HVGR_MEM_MCU_END - HVGR_MEM_MCU_SIZE)

/* 2M mem: 0x70 E000 0000 ~ 0x70 FFFF FFFF */
#define HVGR_MEM_2M_END HVGR_MEM_MCU_START
#else
/* gpu mem: 0x71 0000 0000 ~ 0x78 FFFF FFFF */
#define HVGR_MEM_GPU_END  HVGR_MEM_IMPORT_START
#define HVGR_MEM_GPU_SIZE  ((uint64_t)32 * 1024 * 1024 * 1024)
#define HVGR_MEM_GPU_START (HVGR_MEM_GPU_END - HVGR_MEM_GPU_SIZE)
 /* 2M mem: 0x70 E000 0000 ~ 70 FFFF FFFF */
#define HVGR_MEM_2M_END HVGR_MEM_GPU_START
#endif

#define HVGR_MEM_2M_SIZE  ((uint64_t)512 * 1024 * 1024)
#define HVGR_MEM_2M_START (HVGR_MEM_2M_END - HVGR_MEM_2M_SIZE)

#define HVGR_MEM_SAMEVA_TOP ((unsigned long)HVGR_MEM_2M_START - PAGE_SIZE)


enum hvgr_zone_type {
	ZONE_TYPE_GPU = 0,
	ZONE_TYPE_2M,
	ZONE_TYPE_IMPORT,
	ZONE_TYPE_REPLAY,
	ZONE_TYPE_MAX,
};

bool hvgr_mem_zone_init_proc(struct hvgr_mem_ctx * const pmem);

void hvgr_mem_zone_term_proc(struct hvgr_mem_ctx * const pmem);

bool hvgr_mem_zone_create(struct hvgr_mem_ctx * const pmem,
	uint64_t prot, struct hvgr_zone_opts const * const opts,
	void * const para);

struct hvgr_mem_zone *hvgr_mem_zone_get_zone(
	struct hvgr_mem_ctx * const pmem, uint64_t prot);

long hvgr_mem_zone_alloc_memory(struct hvgr_mem_area * const area);

void hvgr_mem_zone_free_memory(struct hvgr_mem_area * const area);

long hvgr_mem_zone_import_memory(struct hvgr_mem_area * const area,
	struct hvgr_mem_import_record *record);

struct hvgr_mem_area *hvgr_mem_zone_find_area_by_gva(
	struct hvgr_mem_ctx * const pmem, uint64_t gva);

struct hvgr_mem_area *hvgr_mem_zone_find_area_by_gva_range(
	struct hvgr_mem_ctx * const pmem, uint64_t gva);

uint64_t hvgr_mem_zone_alloc_gva(struct hvgr_mem_ctx * const pmem, enum hvgr_zone_type zone_type,
	uint64_t target_addr, uint64_t size);

void hvgr_mem_zone_free_gva(struct hvgr_mem_ctx * const pmem, enum hvgr_zone_type zone_type,
	uint64_t addr, uint64_t size);

#endif
