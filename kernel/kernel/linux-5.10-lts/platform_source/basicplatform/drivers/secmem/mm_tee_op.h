/*
 * drivers/secmem/mm_tee_op.h
 *
 * Copyright (C) 2022 Hisilicon, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _MM_TEE_OP_H
#define _MM_TEE_OP_H

#include "teek_client_api.h"
#include "teek_client_constants.h"
#include "teek_client_id.h"

enum SEC_Task {
	SEC_TASK_DRM = 0x0,
	SEC_TASK_SEC,
	SEC_TASK_TINY,
	SEC_TASK_ISP_CORE,
	SEC_TASK_AOD,
	SEC_TASK_MAX,
};

#define SECBOOT_CMD_ID_MEM_ALLOCATE 0x1

#ifdef CONFIG_DMABUF_SECSG_DEBUG
#define sec_debug(fmt, ...) \
	pr_info(fmt, ##__VA_ARGS__)
#else
#define sec_debug(fmt, ...)
#endif

struct mem_chunk_list {
	u32 protect_id;
	union {
		u32 nents;
		u32 buff_id;
	};
	u32 va;
	void *phys_addr;  /* Must be the start addr of struct tz_pageinfo */
	u32 size;
	u32 cache;
	int prot;
	int mode;
	u32 smmuid;
	u32 sid;
	u32 ssid;
	int ta_rw_index;
};

struct tz_pageinfo {
	u64 addr;
	u32 nr_pages;
} __aligned(8);

struct dma_heap_sec_tee {
	TEEC_Context *context;
	TEEC_Session *session;
};

enum dma_heap_sessions_type {
	DMA_HEAP_SESSIONS_SECMEM,
	DMA_HEAP_SESSIONS_VLTMM,
	DMA_HEAP_SESSIONS_TEST, /* not supported at this time */
	DMA_HEAP_SESSIONS_MAX
};

#define TEE_SECMEM_NAME "sec_mem"
#define TEE_VLTMM_NAME  "vltmm"

int secmem_tee_init(TEEC_Context *context, TEEC_Session *session,
				const char *package_name);

int secmem_tee_exec_cmd(TEEC_Session *session,
		       struct mem_chunk_list *mcl, u32 cmd);

void secmem_tee_destroy(TEEC_Context *context, TEEC_Session *session);

void seccg_pre_alloc_wk_func(struct work_struct *work);

int sec_tee_alloc(struct dma_heap_sec_tee *tee);

void sec_tee_free(struct dma_heap_sec_tee *tee);

int sec_tee_init(struct dma_heap_sec_tee *tee, enum dma_heap_sessions_type type);

#endif
