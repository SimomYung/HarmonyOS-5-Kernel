/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Prototypes for driver module loader support
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 09 17:31:00 2019
 */

#ifndef __DEVHOST_LOADER_H__
#define __DEVHOST_LOADER_H__

#include <stddef.h>

void devhost_loader_init(void);
int devhost_load_module(const char *libname, const void *data, size_t size, char *args);
int devhost_unload_module(const char *libname);

#endif /* __DEVHOST_LOADER_H__ */
