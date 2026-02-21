/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fops core implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 25 15:50:54 2023
 */

#ifndef __UDK_FUSION_FILE_H__
#define __UDK_FUSION_FILE_H__

#include <intapi/mem/mcache.h>
#include <devhost/file.h>

DECLARE_MCACHE(block_ctx)
struct filp_node *udk_fusion_filp_node_alloc(__u32 cnode_idx, struct device_info *info);
int udk_fusion_filp_node_free(struct filp_node *node);
#endif
