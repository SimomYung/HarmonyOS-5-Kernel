/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Errno
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 30 17:34:28 2018
 */
#ifndef KLIBS_ERRNO_H
#define KLIBS_ERRNO_H

extern const char *hmstrerror(int error);
extern int hmerrno2posix(int error);

#endif
