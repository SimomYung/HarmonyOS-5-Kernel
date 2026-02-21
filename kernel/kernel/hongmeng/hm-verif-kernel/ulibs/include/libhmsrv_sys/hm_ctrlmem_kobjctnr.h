/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Supply epoll server common interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 12 21:00:44 2020
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_HM_KOBJ_COMMON_H
#define ULIBS_INCLUDE_LIBHMSRV_HM_KOBJ_COMMON_H
#include <hmkernel/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/polllist.h>
#include <hmkernel/ctrlmem/kobjs/sysevent.h>
#include <libhmsrv_sys/hm_ctrlmem_syscalls_layer.h>
#include <libhmsrv_sys/hm_ctrlmem_eventpoll.h>
#include <hmkernel/ctrlmem/syscall.h>
#include <hmkernel/types.h>

#ifdef __HOST_UT__
int host_llt_cleanup_page_pool();
#endif
struct __ctrlmem_kobj_polllist_s *
hm_ctrlmem_kobjpool_spawn_polllist(unsigned long file, __u32 listener_cnt,
				   vatomic32_t *revent);
struct __ctrlmem_kobj_sysevent_s *hm_ctrlmem_kobjpool_spawn_sysevent(void);
int hm_ctrlmem_kobjpool_destroy(void *kobj);

#endif
