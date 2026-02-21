/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Contexthub dump extract myprintf head file
 * Author: Shengzexuan Shengzexuan@huawei.com
 * Create: 2024-05-13
 */
#ifndef __MYPRINTF_H__
#define __MYPRINTF_H__
 
#include "securec.h"
#include <linux/types.h>
#include <platform_include/smart/linux/iomcu_dump.h>
 
int progprintf(struct file * fp, char *fmt_in, uint32_t argv[]);
 
#endif /* __MYPRINTF_H__ */