/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 25 04:45:49 2018
 */
#ifndef AARCH64_ASM_KVSPACE_H
#define AARCH64_ASM_KVSPACE_H

#include <hmkernel/mm/pgtable.h>

extern void arch_kvspace_remap(const struct pgentry_vpgdreg *pd0, const struct pgentry_vpgdreg *dummy_pd0);
extern void arch_kvspace_kunmap_done(vaddr_t va, size_t size);
extern void arch_idmap_park(paddr_t pstart, paddr_t pend);

/*
 * Only arm64 needs this.
 */
extern void arch_kvspace_copyto(void *new_page);

#endif
