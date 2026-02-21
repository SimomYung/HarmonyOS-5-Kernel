/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Declare of vfs_dev structs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 25 18:26:43 2023
 */
#ifndef LIBHMSRV_FS_VFS_DEV_H
#define LIBHMSRV_FS_VFS_DEV_H

typedef enum {
	MODE_NODEV = 0,
	MODE_REG = 1,
	MODE_CHR = 2,
	MODE_BLK = 4,
	MODE_FIFO = 8,
	MODE_TMP = 16,
} vfs_mode_t;

#endif /* LIBHMSRV_FS_VFS_DEV_H */
