/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2024-3-4
*/
#ifndef __F2FS_EXT_H__
#define __F2FS_EXT_H__

#include <f2fs_log.h>
#include <f2fs_dmd.h>

#define F2FS_EXT_EXIT() \
do {                    \
    SlogExit();         \
    DmdReport();        \
} while (0)

#endif