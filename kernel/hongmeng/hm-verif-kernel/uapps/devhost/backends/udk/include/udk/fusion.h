/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: This is the header of udk fusion interface
 * Author: Huawei OS Kernel Lab
 * Create: Thur Mar 7 10:12:53 2024
 */
#ifndef __UDK_PUBLIC_FUSION_H__
#define __UDK_PUBLIC_FUSION_H__

#include <udk/types.h>

struct udk_dt_node;

void udk_fusion_of_dtnode_deps(size_t count, ...);

struct udk_dt_node *udk_fusion_of_root(void);

#endif
