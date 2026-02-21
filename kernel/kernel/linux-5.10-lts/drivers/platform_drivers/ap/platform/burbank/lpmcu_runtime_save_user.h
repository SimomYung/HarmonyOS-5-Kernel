/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: lpmcu runtime sensitive data define
 * Author: zhuzhangwei
 * Create: 2019-07-19
 */
#ifndef _LPMCU_RUNTIME_SAVE_USER_H_
#define _LPMCU_RUNTIME_SAVE_USER_H_

#include <lpm_runtime_offset.h>

struct rdr_runtime_save {
	u64 offset;
	u64 size;
};

#define ALIGN_BY_WORD_ADDR(addr)		((addr) & ~0x3U)
#define ALIGN_BY_WORD_SIZE(start, end)		((end) - (ALIGN_BY_WORD_ADDR(start)))
#define RDR_RT_OFFSET(addr)		((ALIGN_BY_WORD_ADDR(addr) - RUNTIME_VAR_BASE))
/* runtime offset in rdr + runtime offset */
#define RDR_RT_INFO_FILL(start, end)	{.offset = RDR_RT_OFFSET(start), .size = end}

const struct rdr_runtime_save g_rdr_runtime_save_info[] = {
	/* (runtime start address)    (runtime end address + end variable size) */
	RDR_RT_INFO_FILL(RUNTIME_CPU_VAR_ADDR,             RUNTIME_CPU_VAR_SIZE),
	RDR_RT_INFO_FILL(RUNTIME_GPU_VAR_ADDR,             RUNTIME_GPU_VAR_SIZE),
	RDR_RT_INFO_FILL(RUNTIME_TMP_VAR_ADDR,             RUNTIME_TMP_VAR_SIZE),
	RDR_RT_INFO_FILL(RUNTIME_PERIL_VAR_ADDR,           RUNTIME_PERIL_VAR_SIZE),
	RDR_RT_INFO_FILL(RUNTIME_DDR_VAR_ADDR,             RUNTIME_DDR_VAR_SIZE),
	RDR_RT_INFO_FILL(RUNTIME_LPMCU_RUN_VAR_ADDR,       RUNTIME_LPMCU_RUN_VAR_SIZE),
	RDR_RT_INFO_FILL(RUNTIME_SR_VAR_BASE_ADDR,         RUNTIME_SR_VAR_BASE_SIZE),
};

#endif
