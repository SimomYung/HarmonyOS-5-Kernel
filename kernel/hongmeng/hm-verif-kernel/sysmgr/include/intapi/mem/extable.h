/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Sysmgr exception table
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 19 2019
 */
#ifndef SYSMGR_EXTABLE
#define SYSMGR_EXTABLE

#include <hongmeng/types.h>

struct sysmgr_except_table {
	uintptr_t except_start;
	uintptr_t except_end;
	uintptr_t fixup;
	uintptr_t except_nopf_start; /* start label of safe_copy_nopf func, 'pf' means page fault */
	uintptr_t except_nopf_end; /* end label of safe_copy_nopf func */
};

void extable_init32(struct sysmgr_except_table *extable, uintptr_t vdso_code_base, uintptr_t ehdr_base);
void extable_init64(struct sysmgr_except_table *extable, uintptr_t vdso_code_base, uintptr_t ehdr_base);

uintptr_t get_fixup_pc(const struct sysmgr_except_table *extable, uintptr_t fault_addr);
/*
 * get the fixup pc for safe_copy_nopf func
 */
uintptr_t get_nopf_fixup_pc(const struct sysmgr_except_table *extable, uintptr_t fault_addr);
#endif
