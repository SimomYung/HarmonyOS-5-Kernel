/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Boot helper
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 31 14:55:38 2018
 */
#ifndef KLIBS_BOOT_HELPER_H
#define KLIBS_BOOT_HELPER_H

#include <hmkernel/mm/pgtable.h>
#include <hmkernel/pgtblhdlr.h>
#include <hmasm/pgtbl/pgtblattr.h>

extern void
early_map_ng(struct pgentry_s entry,
	     const struct pgcfg_s *cfg,
	     paddr_t pa_start, paddr_t pa_end, u64 pv_offset);

extern void early_map_init(void);

#endif
