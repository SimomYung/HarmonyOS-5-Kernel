/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Ftrace modify code interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#ifndef SYSMGR_INTAPI_INSPECTOR_HMTRACE_FTRACE_H
#define SYSMGR_INTAPI_INSPECTOR_HMTRACE_FTRACE_H

#include <stdint.h>
#include <intapi/mem/vspace.h>
#include <libhmsync/raw_mutex.h>

#define FTRACE_STUB_SITE_SECTION ".ftrace_stub_site"

/* modify instruction of ip address by vspace */
int ftrace_stub_modify_code_by_vspace(uintptr_t ip, unsigned int insn,
				      struct vspace_s *vspace);

/* modify instruction of ip address to NOP instruction by vspace */
int ftrace_stub_make_nop_by_vspace(uintptr_t ip, struct vspace_s *vspace);
#endif
