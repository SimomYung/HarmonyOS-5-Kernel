/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: APIs for fsverity kobj
 * Author: Huawei OS Kernel Lab
 * Created: Sat May 4 13:36:49 2024
 */

#ifndef INTAPI_CTRLMEM_KOBJS_FSVERITY_H
#define INTAPI_CTRLMEM_KOBJS_FSVERITY_H

#include <intapi/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/fsverity.h>

extern struct __ctrlmem_kobj_fsverity_s *ctrlmem_kobj_fsverity_spawn(void *info, __u32 info_size);

extern int ctrlmem_kobj_fsverity_destroy(struct __ctrlmem_kobj_fsverity_s *kobj);

extern int ctrlmem_kobj_fsverity_verify(struct __ctrlmem_kobj_fsverity_s *kobj,
					struct __ctrlmem_kobj_fsverity_single_hash_s *hbuffer, __u32 buf_max_level,
					__u64 data_page_va, __u32 data_offset);
extern int ctrlmem_kobj_fsverity_verify_mb(struct __ctrlmem_kobj_fsverity_s *kobj, __u32 buf_max_level,
					   struct __ctrlmem_kobj_fsverity_mb_task_s *mb_task);
#endif
