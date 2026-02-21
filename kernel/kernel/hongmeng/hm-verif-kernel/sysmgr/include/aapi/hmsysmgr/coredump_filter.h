/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Coredump filter related macro
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 12 10:50:49 CST 2019
 */
#ifndef SYSMGR_AAPI_COREDUMP_FILTER_H
#define SYSMGR_AAPI_COREDUMP_FILTER_H

#define IDUMP_FULL_STACK		0ULL
#define IDUMP_USED_STACK		0x100000000ULL
#define IDUMP_MAX_STACKSIZE		0xFFFFFFFFULL

#define FILTER_TYPE_DEFAULT		0U
#define FILTER_TYPE_IDUMP		1U

#define FILTER_COREDUMP_ALL		0U
#define FILTER_DEFAULT_ANON_PRIAVTE	1U
#define FILTER_DEFAULT_ANON_SHARED	(1U << 1)
#define FILTER_DEFAULT_FILE_PRIVATE	(1U << 2)
#define FILTER_DEFAULT_FILE_SHARED	(1U << 3)
#define FILTER_DEFAULT_ELF_HEADERS	(1U << 4)
#define FILTER_DEFAULT_HUGETLB_PRIVATE	(1U << 5)
#define FILTER_DEFAULT_HUGETLB_SHARED	(1U << 6)
/* DEFAULT_MASK is the max value user can set */
#define FILTER_DEFAULT_MASK		((1U << 7) - 1)

/* basic content, always dumped */
#define FILTER_IDUMP_BASIC		0U
/* dump stack in addition to basic content */
#define FILTER_IDUMP_STACK		1U
#define FILTER_IDUMP_BIN_DATA		(__U(1) << 1)
#define FILTER_IDUMP_BIN_TEXT		(__U(1) << 2)
#define FILTER_IDUMP_LIB_DATA		(__U(1) << 3)
#define FILTER_IDUMP_LIB_TEXT		(__U(1) << 4)
#define FILTER_IDUMP_HEAP		(__U(1) << 5)
#define FILTER_IDUMP_FILE		(__U(1) << 6)
#define FILTER_IDUMP_ELF_HEADERS	(__U(1) << 8)
#define FILTER_ANON_MAPPING		(__U(1) << 9)

#endif
