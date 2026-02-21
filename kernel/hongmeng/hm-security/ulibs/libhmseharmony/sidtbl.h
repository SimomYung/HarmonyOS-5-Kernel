/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for sid table search
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef HMSEL_SIDTBL_H
#define HMSEL_SIDTBL_H

#include <stdlib.h>
#include <hongmeng/errno.h>
#include <libmem/utils.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsync/raw_mutex.h>
#include <libhmseharmony/pac.h>

/* 30 = (catmap_nodes per level)16 * 2(low and high level) - 2(hmmls_level is already contain one catmap_node) */
#define ENT_SIZE (sizeof(struct hmsel_context) + ((sizeof(struct hmmls_level) + 30 * sizeof(struct hm_catmap_node))))

#define HMSEL_BUF_CFG_REMOTE 1u
#define HMSEL_BUF_CFG_RDONLY 2u
#define HMSEL_BUF_CFG_FILE   4u
#define OFFSET_INVALID	0xffffffff

struct hmsel_buf {
	int shm_id;
	unsigned flags;
	__sec_data(seharmony, ptr) void *data;
	size_t size;
	size_t mapped_size;
};

#define HMSEL_SID_CTX_TBL_CFG_REMOTE 1u
#define HMSEL_SID_CTX_TBL_CFG_RDONLY 2u
#define HMSEL_SID_CTX_TBL_CFG_FILE   4u

struct hmsel_sid_ctx_tbl {
	struct hmsel_buf buf;
	hmsel_sid_t sid_base;
	hmsel_sid_t sid;
	hmsel_sid_t sid_end;
	size_t offset_end;
	size_t *sid_to_offset;
	struct raw_mutex lock;
};

struct hmsel_sid_ctx_tbl_cfg {
	int shm_id;
	unsigned flags;
	hmsel_sid_t sid_base;
	size_t table_length;
	size_t preallocated_sid_num;
};

#endif
