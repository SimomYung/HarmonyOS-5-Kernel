/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Tls function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019
 */

#ifndef ULIBS_LIBHMSYNC_RAW_TLS_H
#define ULIBS_LIBHMSYNC_RAW_TLS_H

#if defined __HOST_LLT__
#include "llt_raw_tls.h"
#else
#if defined __aarch64__
#include "aarch64_raw_tls.h"
#elif defined __arm__
#include "arm_raw_tls.h"
#else
#error "Arch not supported"
#endif
#endif

struct raw_tls_module {
	struct raw_tls_module *next;
	size_t mem_sz;
	size_t file_sz;
	size_t offset;
	size_t align;
	void *image;
};

struct global_tls_info {
	size_t *auxv;
	struct raw_tls_module *head;
	size_t size;
	size_t align;
	size_t cnt;
};

#define SYSMGR_TLS_PREV 0xfefefefeul

#endif
