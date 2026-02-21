/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of common functions in hmld
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 9:24:58 2019
 */

#ifndef HMLDSO_INCLUDE_HM_LDSO_H
#define HMLDSO_INCLUDE_HM_LDSO_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <hmasm/types.h>
#include <sys/stat.h>
#include <vfs.h>
#include <fs_ldso.h>

void _hm_ldso_main(void *sp, int argc, char *argv[]);

void *ldso_mmap(const void *addr, size_t len, int prot, int32_t flags, int fd, int64_t offset);
int ldso_munmap(const void *addr, size_t len);

#endif
