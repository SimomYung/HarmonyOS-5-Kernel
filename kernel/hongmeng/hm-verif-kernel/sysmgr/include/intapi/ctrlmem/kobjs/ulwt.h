/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kobj ulwt
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 1 15:37:42 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_ULWT_H
#define INTAPI_CTRLMEM_KOBJS_ULWT_H

#include <intapi/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/ulwt.h>

extern struct __ctrlmem_kobj_ulwt_s *ctrlmem_kobj_ulwt_spawn(void);
extern int ctrlmem_kobj_ulwt_destroy(struct __ctrlmem_kobj_ulwt_s *kobj);

#endif
