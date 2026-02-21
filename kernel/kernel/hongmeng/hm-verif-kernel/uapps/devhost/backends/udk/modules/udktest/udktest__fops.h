/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 10:37:35 2019
 */

#ifndef __UDKTEST_FOPS_H__
#define __UDKTEST_FOPS_H__

#include <udk/device.h>

int udktest__fops_open(const struct device_info *devinfo, void **ctx);
int udktest__fops_release(void *ctx);
int udktest__fops_ioctl(const void *ctx, unsigned int cmd, unsigned long arg);
int udktest__fops_read(void *ctx, void *buf, unsigned long count,
		       unsigned long long off);
int udktest__fops_write(void *ctx, const void *buf, unsigned long count,
			unsigned long long off);
long long udktest__fops_llseek(void *ctx, long long offset, int whence);
int udktest_setpid(const void *ctx, unsigned int cmd, unsigned long arg);
int udktest__fops_mmap(const void *ctx, unsigned long addr, unsigned long length,
		       unsigned long prot, unsigned long offset);

#endif /* __UDKTEST_FOPS_H__ */
