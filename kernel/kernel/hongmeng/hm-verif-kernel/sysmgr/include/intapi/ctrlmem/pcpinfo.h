/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Sysmgr pcpinfo utils
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 26 17:25:44 2024
 */

#ifndef SYSMGR_INTAPI_CTRLMEM_PCPINFO_H
#define SYSMGR_INTAPI_CTRLMEM_PCPINFO_H

#include <hmkernel/ctrlmem/pcpinfo.h>
#include <intapi/process/process.h>

extern void ctrlmem_pcpinfo_init(const __u64 *meta);

extern struct __ctrlmem_pcpinfo_s *ctrlmem_pcpinfo_of_cpu(unsigned int cpu);

extern const struct __ctrlmem_pcpinfo_s *ctrlmem_pcpinfo_current(void);

extern int ctrlmem_mmap_pcpinfo(struct vspace_s *vs);

#endif
