/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  mas block dump interface
 */

#include <udk/block/mas_blk/mas_blk_dump_interface.h>

int __cfi_mas_blk_panic_notify(unsigned int cmd, unsigned long arg)
{
	return mas_blk_panic_notify(cmd, arg);
}
