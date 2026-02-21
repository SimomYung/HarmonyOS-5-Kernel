/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: internel API of kobject thread
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 29 06:35:02 2023
 */
#ifndef INTAPI_CTRLMEM_KOBJS_THREAD_H
#define INTAPI_CTRLMEM_KOBJS_THREAD_H

#include <hmkernel/ctrlmem/kobjs/thread.h>

struct ctrlmem_kobj_thread_wrapper_s {
	struct __ctrlmem_kobj_thread_s *kobj;
	struct ctrlmem_kobj_thread_page_s *page;
};

extern struct ctrlmem_kobj_thread_wrapper_s *ctrlmem_kobj_thread_spawn(void);
extern void ctrlmem_kobj_thread_destroy(struct ctrlmem_kobj_thread_wrapper_s *wrapper);

#endif
