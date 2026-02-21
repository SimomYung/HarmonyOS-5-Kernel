/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of ama
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 22 11:08:48 UTC 2024
 */

#ifndef __SYSMGR_INTAPI_MEM_AMA_H__
#define __SYSMGR_INTAPI_MEM_AMA_H__

#ifdef CONFIG_MM_AMA
#define AMA_MAX_NODES CONFIG_MM_AMA_MAX_NODES
#else
#define AMA_MAX_NODES 1
#endif

#ifdef CONFIG_MM_AMA_POLICY
int sysmgr_ama_migrate_info_read(char *buf, size_t len);
#endif

#endif /* __SYSMGR_INTAPI_MEM_PAGE_H__ */
