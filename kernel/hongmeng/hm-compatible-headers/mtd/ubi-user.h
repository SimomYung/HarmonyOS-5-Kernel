/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add mtd/ubi-user.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 18 15:49:31 2022
 */

#ifndef __UBI_USER_H__
#define __UBI_USER_H__

#include <linux/types.h>

#define UBI_VOL_IOC_MAGIC 'O'
#define UBI_IOCVOLUP _IOW(UBI_VOL_IOC_MAGIC, 0, __s64)

#endif
