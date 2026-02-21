/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: APIs for actv frame kobj
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 21 06:55:51 2024
 */
#ifndef INTAPI_CTRLMEM_KOBJS_ACTV_FRAME_H
#define INTAPI_CTRLMEM_KOBJS_ACTV_FRAME_H

#include <intapi/ctrlmem/kobjctnr.h>

#include <hmkernel/ctrlmem/kobjs/actv_frame.h>

struct __ctrlmem_kobj_actv_frame_s *ctrlmem_kobj_actv_frame_spawn(void);

int ctrlmem_kobj_actv_frame_destroy(struct __ctrlmem_kobj_actv_frame_s *kobj);

#endif /* INTAPI_CTRLMEM_KOBJS_ACTV_FRAME_H */
