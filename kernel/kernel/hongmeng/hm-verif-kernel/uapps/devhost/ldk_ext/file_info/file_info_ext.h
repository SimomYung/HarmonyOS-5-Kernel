/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Define functions for file_info ext module
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 1 12:51:35 2023
 */
#ifndef __DEVHOST_FILE_INFO_EXT_H__
#define __DEVHOST_FILE_INFO_EXT_H__
int devhost_file_info_vfs_cnode_idx_init(void);
/* vmalloc_info */
#ifdef CONFIG_DFX_MEMCHECK
int devhost_vmalloc_info_create_file(void);
int memcheck_vmalloc_info_show(bool is_emergency);
#else /* CONFIG_DFX_MEMCHECK */
static inline int devhost_vmalloc_info_create_file(void) { return 0; }
static inline int memcheck_vmalloc_info_show(bool is_emergency) { return 0; }
#endif /* CONFIG_DFX_MEMCHECK */
#endif
