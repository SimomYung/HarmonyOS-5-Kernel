/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Ctrlmem rwlock related defs
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jun 27 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_RWLOCK_H
#define INTAPI_CTRLMEM_KOBJS_RWLOCK_H

#include <hmkernel/ctrlmem/kobjs/rwlock.h>
#include <hmkernel/compiler.h>
#include <vsync/atomic.h>
#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <libhmlog/hmlog.h>
struct ctrlmem_kobj_rwlock_s {
	vatomic32_t counter;
};

extern void ctrlmem_disable_rwlock(void);

extern void ctrlmem_enable_rwlock(void);

extern bool is_ctrlmem_rwlock_disable(void);

extern void ctrlmem_kobj_wrlock(struct ctrlmem_kobj_rwlock_s *lock);

extern void ctrlmem_kobj_wrunlock(struct ctrlmem_kobj_rwlock_s *lock);

struct page_s;
extern void ctrlmem_page_lock(struct page_s *pg);

extern void ctrlmem_page_unlock(struct page_s *pg);

#endif
