/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declare interface of vregion
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 15 18:10:41 UTC 2024
 */

#ifndef __SYSMGR_INCLUDE_INTAPI_MEM_VREGION_H__
#define __SYSMGR_INCLUDE_INTAPI_MEM_VREGION_H__

struct vregion_s;

struct vregion_s *vregion_alloc(void);
void vregion_put(struct vregion_s *vr);

#endif /* __SYSMGR_INCLUDE_INTAPI_MEM_VREGION_H__ */
