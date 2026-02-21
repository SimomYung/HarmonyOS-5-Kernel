/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Descrption: CMA
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 10 10:40:52 2025
 */
#ifndef SYSMGR_INTAPI_MEM_CMA_H
#define SYSMGR_INTAPI_MEM_CMA_H

enum cma_reuse_lv {
	CMA_REUSE_NONE,
	CMA_REUSE_FILE,
	CMA_REUSE_ANON,
	CMA_REUSE_ALL,
	CMA_REUSE_INVAL
};

void cma_set_reuse_lv(enum cma_reuse_lv lv);
enum cma_reuse_lv cma_reuse_lv_of(void);
bool cma_enable_anon_reuse(void);
bool cma_enable_file_reuse(void);
#endif
