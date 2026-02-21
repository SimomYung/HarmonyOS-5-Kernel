/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide interface of dumping lock trace info
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 16 17:21:06 2020
 */
#ifndef SYSMGR_INTAPI_INSPECTOR_LOCKTRACE_DUMP_H
#define SYSMGR_INTAPI_INSPECTOR_LOCKTRACE_DUMP_H

#include <intapi/mem/vspace.h>
#include <intapi/inspector/procdump/procdump.h>

void lock_trace_dump_impl(struct vspace_s *vspace, struct dump_impl *impl,
			  struct thread_lock_info_s *lock_info);
#endif
