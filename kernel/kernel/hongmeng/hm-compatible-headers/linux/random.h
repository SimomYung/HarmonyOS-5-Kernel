/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/random.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 24 10:53:44 2021
 */

#ifndef LINUX_RANDOM_H
#define LINUX_RANDOM_H

#include <linux/ioctl.h>

#define RNDGETENTCNT	_IOR( 'R', 0x00, int  )
#define RNDADDENTROPY	_IOW( 'R', 0x03, int [2]  )

#endif
