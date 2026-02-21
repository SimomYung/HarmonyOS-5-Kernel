/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2024-3-4
*/

#ifndef __F2FS_DMD_H__
#define __F2FS_DMD_H__

#include "f2fs_dfx_common.h"
#include "f2fs_dmd_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/* dmd report the error number */
#define DMD_ADD_ERROR(type, err) DmdInsertError(type, err, __func__, __LINE__)

int DmdReport(void);
void DmdInsertError(int type, int err, const char *func, int line);
void DmdMarkReport(void);

#ifdef __cplusplus
}
#endif
#endif
