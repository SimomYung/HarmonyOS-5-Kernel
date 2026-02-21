/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: kobj_stat.h
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 16 21:55:06 2024
 */
#ifndef SYSMGR_MEM_KOBJCNT_H
#define SYSMGR_MEM_KOBJCNT_H

enum kobj_type {
	KOBJ_PMEM,
	KOBJ_POOL,
	KOBJ_PROBE,
	KOBJ_CTNRPG,
	KOBJ_TBLPG,
	KOBJ_TCB,
	__NR_KOBJ_TYPE,
};

void kobjcnt_increase(enum kobj_type type, uint64_t count);
void kobjcnt_decrease(enum kobj_type type, uint64_t count);
uint64_t kobjcnt_count_of(size_t type);
const char *kobjcnt_name_of(size_t type);

#endif
