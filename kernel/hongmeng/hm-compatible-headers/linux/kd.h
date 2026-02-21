/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/kd.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 15:30:57 2021
 */

#ifndef LINUX_KD_H
#define LINUX_KD_H

#define		K_RAW		0x00
#define		K_XLATE		0x01
#define		K_MEDIUMRAW	0x02
#define		K_UNICODE	0x03

#define KDGKBMODE	0x4B44

#define	KDGKBLED	0x4B64

#endif
