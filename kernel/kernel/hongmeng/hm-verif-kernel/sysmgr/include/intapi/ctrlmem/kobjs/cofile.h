/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kobj cofile
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 28 01:47:59 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_COFILE_H
#define INTAPI_CTRLMEM_KOBJS_COFILE_H

#include <intapi/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/cofile.h>

extern struct __ctrlmem_kobj_cofile_s *ctrlmem_kobj_cofile_spawn(unsigned int type, __u64 size);
extern int ctrlmem_kobj_cofile_destroy(struct __ctrlmem_kobj_cofile_s *kobj);

#endif
