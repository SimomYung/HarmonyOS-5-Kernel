/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: mas block core interface
 */

#include <udk/block/mas_blk/mas_blk_core_interface.h>

int __cfi_mas_queue_status_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize)
{
	return mas_queue_status_show(ctx, dst, pos, size, rsize);
}
