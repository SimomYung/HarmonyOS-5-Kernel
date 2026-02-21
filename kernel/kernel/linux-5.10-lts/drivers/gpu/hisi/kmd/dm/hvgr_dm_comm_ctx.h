/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DM_COMM_CTX_H
#define HVGR_DM_COMM_CTX_H

#include "hvgr_defs.h"

struct hvgr_mem_alloc_para;

#if hvgr_version_ge(350)
/*
 * common ctx use the defined va for gpu va
 * each va range size is reserved for 1GB
 */
#define COMMON_CTX_FCP_BUF_START                    0x40000000U
/* fast power buffer must under 4G va,size is 640B */
#define COMMON_CTX_FAST_PWR_BUF_START               0x80000000U
/* mcu common buffer gva start,buffer size is 5MB */
#define COMMON_CTX_MCU_BUF_START                    0xC0000000U
/* ffts issueq, size is 200KB */
#define COMMON_CTX_FFTS_COMM_START                  0x140000000U
/* common buffer gva start, for perf counter */
#define COMMON_CTX_PERF_BUF_START                   0x1000000000U

/*
 * @brief comm ctx init when module init.
 */
int hvgr_dm_comm_ctx_init(struct hvgr_device * const gdev);

/*
 * @brief comm ctx term when module term.
 */
void hvgr_dm_comm_ctx_term(struct hvgr_device * const gdev);

/*
 * @brief enable comm ctx and map to gpu when gpu power on.
 */
int hvgr_dm_comm_ctx_enable(struct hvgr_device * const gdev);

/*
 * @brief alloc mem from comm ctx.
 */
struct hvgr_mem_area *hvgr_comm_ctx_alloc_area(struct hvgr_device * const gdev,
	struct hvgr_mem_alloc_para *para);

/*
 * @brief free mem from comm ctx.
 */
void hvgr_comm_ctx_free_area(struct hvgr_device * const gdev,
	struct hvgr_mem_area *area);

#else

static inline int hvgr_dm_comm_ctx_init(struct hvgr_device * const gdev)
{
	return 0;
}
static inline void hvgr_dm_comm_ctx_term(struct hvgr_device * const gdev) {}
static inline int hvgr_dm_comm_ctx_enable(struct hvgr_device * const gdev)
{
	return 0;
}
static inline struct hvgr_mem_area *hvgr_comm_ctx_alloc_area(struct hvgr_device * const gdev,
	struct hvgr_mem_alloc_para *para)
{
	return NULL;
}
static inline void hvgr_comm_ctx_free_area(struct hvgr_device * const gdev,
	struct hvgr_mem_area *area) {}

#endif

#endif
