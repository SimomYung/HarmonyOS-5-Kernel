/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: policydb page operations
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 25 10:44:40 2025
 */

#ifndef HMKERNEL_CTRLMEM_POLICYDB_H
#define HMKERNEL_CTRLMEM_POLICYDB_H

#include <mapi/hmkernel/ctrlmem/policydb.h>
#include <hmkernel/ctrlmem/syscall/dispatch.h>

#define CTRLMEM_PDB_OP_SET_UAPPS_PR_RO __CTRLMEM_PDB_OP_SET_UAPPS_PR_RO

extern long ctrlmem_policydb_dispatch_write(const struct ctrlmem_write_callinfo_s *callinfo,
					     struct ctrlmem_dispatch_ap_s *aps);

#endif
