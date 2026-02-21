/*
* Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
* Description: ENCAPS ioctl header
* Author: Huawei OS Kernel Lab
* Create: Wed Apr 24 01:19:14 2024
*/


#ifndef VFS_DEV_ENCAPS_H
#define VFS_DEV_ENCAPS_H
#include <sys/ioctl.h>
#include <sys/types.h>

#define DEV_ENCAPS_PROC_TYPE_MINOR 0x20
#define HM_ENCAPS_MAGIC 'E'
#define HM_ENCAPS_PROC_TYPE_BASE 0x18
#define HM_ENCAPS_SYNC_BASE 0x19
#define HM_ENCAPS_ASSIGN_BASE 0x1A
#define HM_ENCAPS_CHECK_TEST_BASE 0x1B
#define HM_ENCAPS_GET_TEST_BASE 0x1C

#define SET_PROC_TYPE_CMD _IOW(HM_ENCAPS_MAGIC, HM_ENCAPS_PROC_TYPE_BASE, uint32_t)
#define SYNC_ENCAPS_CMD _IO(HM_ENCAPS_MAGIC, HM_ENCAPS_SYNC_BASE)
#define ASSIGN_ENCAPS_CMD _IOW(HM_ENCAPS_MAGIC, HM_ENCAPS_ASSIGN_BASE, char *)
#define CHECK_ENCAPS_TEST_CMD _IOW(HM_ENCAPS_MAGIC, HM_ENCAPS_CHECK_TEST_BASE, char *)
#define GET_ENCAPS_TEST_CMD _IOW(HM_ENCAPS_MAGIC, HM_ENCAPS_GET_TEST_BASE, char *)

#endif
