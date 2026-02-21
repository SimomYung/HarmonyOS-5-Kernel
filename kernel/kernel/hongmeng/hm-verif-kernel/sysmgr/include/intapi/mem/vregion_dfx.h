/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: dump stack when vregion overlimit
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 20 20:05:06 2025
 */
#ifndef INTAPI_MEM_VREGION_DFX_H
#define INTAPI_MEM_VREGION_DFX_H

#include <vmem/vspace.h>

#ifdef CONFIG_VREGION_OVERLIMIT_DUMP
void vregion_dfx_may_report_leak(struct vspace_s *vs, unsigned long margin);
void vregion_dfx_try_dump_stack(void);
#else
static void vregion_dfx_may_report_leak(struct vspace_s *vs, unsigned long margin) {}
static void vregion_dfx_try_dump_stack(void) {}
#endif

#endif
