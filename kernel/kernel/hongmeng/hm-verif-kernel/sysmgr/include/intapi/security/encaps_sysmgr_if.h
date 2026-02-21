/*
* Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
* Description: Sysmgr interface for encaps ioctl
* Author: Huawei OS Kernel Lab
* Create: Wed Apr 24 01:19:14 2024
*/
#ifndef ENCAPS_SYSMGR_VFS
#define ENCAPS_SYSMGR_VFS
#include <stdint.h>

int sysmgr_set_proc_type(uint32_t type);

int sysmgr_assign_encaps(char *encaps_str);

int sysmgr_sync_encaps(void);

#endif
