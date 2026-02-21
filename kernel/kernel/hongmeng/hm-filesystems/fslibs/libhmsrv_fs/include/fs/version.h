/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of version
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 19:30:08 2019
 */

#ifndef FS_VERSION_H
#define FS_VERSION_H
#ifdef CONFIG_VFS_FUSION
const char* fs_version(void);
#else
static inline const char* fs_version(void)
{
	return "202200219";
}
#endif
#endif
