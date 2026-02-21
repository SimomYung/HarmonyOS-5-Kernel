/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/falloc.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 12:11:17 2021
 */

#ifndef LINUX_FD_H
#define LINUX_FD_H

#define FALLOC_FL_KEEP_SIZE	0x01
#define FALLOC_FL_PUNCH_HOLE	0x02

#define FALLOC_FL_COLLAPSE_RANGE	0x08
#define FALLOC_FL_ZERO_RANGE		0x10
#define FALLOC_FL_INSERT_RANGE		0x20

#endif
