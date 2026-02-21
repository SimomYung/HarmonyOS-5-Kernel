/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Kbox UDK interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 25 17:12:53 2019
 */
#ifndef _UDK_KBOX_H
#define _UDK_KBOX_H

#include <udk/types.h>

int udk_kbox_create(const char *name, size_t size);
int udk_kbox_open(const char *name);
ssize_t udk_kbox_write(int rid, const void *buf, size_t size);
void udk_kbox_close(int rid);

#endif /* _UDK_KBOX_H */
