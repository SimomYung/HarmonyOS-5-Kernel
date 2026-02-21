/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_SCH_DFX_H
#define HVGR_SCH_DFX_H
#include <securec.h>
#include "hvgr_defs.h"

#define CQ_DFX_INFO_MAX_LEN 2048

#define hvgr_snprintf_s(ret, buffer, buffer_len, wrlen, fmt, ...) do { \
	ret = snprintf_s(&buffer[wrlen], buffer_len - wrlen, buffer_len - wrlen - 1, \
		fmt, ##__VA_ARGS__); \
	if (ret < 0) \
		wrlen = 0; \
    else \
        wrlen += ret; \
} while(0)

void hvgr_cq_print_cq_info_by_ctx(struct hvgr_ctx * const ctx);

void hvgr_cq_fault_print_cq_info(struct hvgr_ctx * const ctx);

void hvgr_cq_record_irq_info(struct hvgr_cq_ctx * const cq_ctx,
    uint32_t token, enum hvgr_cq_irq_type type);

long hvgr_ctx_ioctl_record_init(struct hvgr_ctx * const ctx);

void hvgr_ctx_ioctl_record_term(struct hvgr_ctx * const ctx);

void hvgr_ctx_ioctl_record(struct hvgr_ctx * const ctx, uint64_t time, uint32_t cmd, uint32_t para);

void hvgr_ctx_ioctl_record_print(struct hvgr_ctx * const ctx);
#endif
