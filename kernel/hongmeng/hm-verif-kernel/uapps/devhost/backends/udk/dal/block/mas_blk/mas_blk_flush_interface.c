/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  mas block flush reduce interface
 */

#include <udk/block/mas_blk/mas_blk_flush_interface.h>

void __cfi_mas_blk_flush_work_fn(unsigned long val)
{
	mas_blk_flush_work_fn(val);
}

int __cfi_mas_blk_poweroff_flush_notifier_call(unsigned int cmd, unsigned long arg)
{
	return mas_blk_poweroff_flush_notifier_call(cmd, arg);
}

int __init __cfi_mas_blk_flush_init(void)
{
	return mas_blk_flush_init();
}
