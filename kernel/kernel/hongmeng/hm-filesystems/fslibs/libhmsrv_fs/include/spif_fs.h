/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: FS SPIF interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 13 10:10:54 2022
 */
#ifndef HM_FILESYSTEMS_SPIF_H
#define HM_FILESYSTEMS_SPIF_H

#include <lib/perfra/spif.h>

#define __spif_trace_vfs_scoped(point_name)	__spif_trace_group_scoped(vfs, point_name)
#define __spif_trace_fslibs_scoped(point_name)	__spif_trace_group_scoped(fslibs, point_name)

#define __spif_trace_vfs(point_name)		__spif_trace_group(vfs, point_name)
#define __spif_trace_fslibs(point_name)		__spif_trace_group(fslibs, point_name)

#endif
