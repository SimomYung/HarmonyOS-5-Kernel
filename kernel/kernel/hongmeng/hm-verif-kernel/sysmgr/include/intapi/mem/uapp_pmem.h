/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of uapp pmem
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 13 17:09:04 2019
 */
#ifndef SYSMGR_MEM_UAPP_PMEM_H
#define SYSMGR_MEM_UAPP_PMEM_H

#include <intapi/process/process.h>
#include <intapi/mem/pmem_area.h>

cref_t proc_alloc_uapp_pmem(struct process_s *process, uint64_t size);
int proc_remove_uapp_pmem(struct process_s *process, cref_t pmem_cref);
void proc_destroy_uapp_pmem(struct process_s *process);
int proc_record_table_pmem(struct process_s *process, struct pmem_area_s *pmem);
void proc_destroy_table_pmem(struct process_s *process);

#endif
