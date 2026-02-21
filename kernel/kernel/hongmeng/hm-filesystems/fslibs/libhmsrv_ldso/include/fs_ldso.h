/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of fs ldso
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 28 17:26:50 2019
 */
#ifndef LIBVFS_FS_H
#define LIBVFS_FS_H

#include <unistd.h>
#include <hmasm/types.h>
#include <sys/stat.h>

int ldso_open(const char* name, unsigned int flags, unsigned int mode);
ssize_t ldso_read(int fd, void *data, size_t len);
int ldso_close(int fd);
#endif
