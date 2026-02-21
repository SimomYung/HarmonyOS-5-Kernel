/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: header for refresh_sc_data related spincall
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 7 11:34:45 2025
 */

#ifndef INTAPI_CTRLMEM_KOBJS_SECCOMP_H
#define INTAPI_CTRLMEM_KOBJS_SECCOMP_H

#include <hmkernel/ctrlmem/kobjs/thread.h>

extern int ctrlmem_kobj_thread_seccomp_refresh_sc_data(struct __ctrlmem_kobj_thread_s *kobj,
						       void *data, __u32 data_size);

#endif
