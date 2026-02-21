/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of native auxv elements
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 27 17:37:09 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_AUXV_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_AUXV_H

#include <sys/auxv.h>

#define AT_DSOINFO				(AT_L3_CACHEGEOMETRY + 1)
#define AT_SYS_CNODE_IDX			(AT_L3_CACHEGEOMETRY + 2)
#define AT_LIBCMAPLEN				(AT_L3_CACHEGEOMETRY + 3)
/* AT_MINSIGSTKSZ defined as 51, which equals to AT_L3_CACHEGEOMETRY + 4 */
#define AT_SPIF_KSUS				(AT_L3_CACHEGEOMETRY + 5)
#define AT_TRACE_SHM_ADDR			(AT_L3_CACHEGEOMETRY + 6)
#define AT_TRACE_SNAPSHOT_SHM_ADDR		(AT_L3_CACHEGEOMETRY + 7)
#define AT_TRACE_SAVED_TASKINFO_SHM_ADDR	(AT_L3_CACHEGEOMETRY + 8)
#define AT_APP_CNODE_IDX			(AT_L3_CACHEGEOMETRY + 9)
#define AT_MAIN_TCB_INNER_IDX			(AT_L3_CACHEGEOMETRY + 10)

/* AT_AUXV_NUM should be the last one item + 2 */
#define AT_AUXV_NUM				(AT_MAIN_TCB_INNER_IDX + 2)

#define STEP_SIZE 2U

#endif
