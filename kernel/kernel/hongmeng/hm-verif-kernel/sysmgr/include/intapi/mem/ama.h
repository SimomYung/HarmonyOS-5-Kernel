/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of ama
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 22 11:08:48 UTC 2024
 */

#ifndef __SYSMGR_INTAPI_MEM_AMA_H__
#define __SYSMGR_INTAPI_MEM_AMA_H__

#ifdef CONFIG_HISI_MM_AMA
#define AMA_MAX_NODES CONFIG_HISI_MM_AMA_MAX_NODES
#define IS_PRIMARY_NODE(nid)     (g_active_ama_node_num > 1 && (nid) == LOCAL_NODE0)
#define IS_SUBSEQUENT_NODE(nid)  (g_active_ama_node_num > 1 && (nid) != LOCAL_NODE0)

uint32_t file_get_preferred_nid(void);
#else
#define AMA_MAX_NODES 1
#define IS_PRIMARY_NODE(nid)     false
#define IS_SUBSEQUENT_NODE(nid)  false

static inline uint32_t file_get_preferred_nid(void)
{
	return 0;
}
#endif
#endif /* __SYSMGR_INTAPI_MEM_PAGE_H__ */
