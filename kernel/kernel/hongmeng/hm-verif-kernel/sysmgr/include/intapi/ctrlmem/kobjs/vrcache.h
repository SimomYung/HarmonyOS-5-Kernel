/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: APIs for vrcache
 * Author: Huawei OS Kernel Lab
 * Created: Sat Jun 03 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_VRCACHE_H
#define INTAPI_CTRLMEM_KOBJS_VRCACHE_H

#include <hmkernel/ctrlmem/kobjs/vrcache.h>
#include <hmkernel/ctrlmem/kobjs/vregion.h>
#include <vmem/vregion.h>

extern
void vrobjcache_update(struct __ctrlmem_kobj_vrcache_s *vrcache, __vaddr_t uva, struct vregion_s *vr);

extern
void vrobjcache_flush(struct __ctrlmem_kobj_vrcache_s *vrcache);

extern
struct vregion_s *vrobjcache_find(struct __ctrlmem_kobj_vrcache_s *vrcache, __vaddr_t uva);

#endif
