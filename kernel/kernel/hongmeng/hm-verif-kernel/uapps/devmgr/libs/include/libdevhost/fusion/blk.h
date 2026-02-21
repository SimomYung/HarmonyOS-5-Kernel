/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header of fusion blk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 12 16:12:35 2023
 */
#ifndef ULIBS_LIBDEVHOST_FUSION_BLK_H
#define ULIBS_LIBDEVHOST_FUSION_BLK_H

#include <libhmsrv_io/type.h>
#include <libhmsrv_io/blk.h>

/*
 * Following definitions are the same as libhmsrv_io/blk.h
 *
 * These interfaces are used to transform IPC to selfcall when libdh
 * and libhmsrv_io are linked together.
 *
 * libdh_blk is a stub that returns E_HM_NOSYS as default.
 */
int libdh_blk_submit_bio(const struct hmsrv_io_ctx *ctx, unsigned long long rw,
			 unsigned long long sector,
			 struct hmsrv_io_blk_vec *vec, int cnt,
			 unsigned int priority, void *bi_crypt_context,
			 void (*dump_fs)(void));

int libdh_blk_submit_cmd(const struct hmsrv_io_ctx *ctx, unsigned long long cmd,
			 unsigned long long sector, unsigned int size,
			 unsigned int priority);

void libdh_blk_bio_polling(const struct hmsrv_io_ctx *ctx, int rw);
#endif
