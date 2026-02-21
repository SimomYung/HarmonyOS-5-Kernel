/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Prototypes for kbox support
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 9 17:35:00 2020
 */
#ifndef __DEVHOST_API_KBOX_H__
#define __DEVHOST_API_KBOX_H__

#include <stddef.h>
#include <sys/types.h> /* ssize_t */

int devhost_kbox_create(const char *name, size_t size);
int devhost_kbox_open(const char *name);
ssize_t devhost_kbox_write(int rid, const void *buf, size_t size);
void devhost_kbox_close(int rid);

#endif /* __DEVHOST_API_KBOX_H__ */
