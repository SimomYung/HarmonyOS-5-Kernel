/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Parameter passing for sysmgr's test_if.c
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 18 02:06:34 CST 2022
 */
#ifndef ULIBS_LIBTEST_TEST_IF_H
#define ULIBS_LIBTEST_TEST_IF_H

/*
 * In general, we should define marco/enum of cmd in a common location for caller & callee, and
 * the common location is usually the sysif file. However, modifying sysif file would usually
 * require rebuilding the whole sdk, or we have to manually invoke hm-dsl to re-generate the
 * header file if sysif file is changed even for adding a single item in the enum. Since there
 * is no better location for sharing testing data between sysmgr and hm-test, we leave the cmd
 * here.
 */
#define ACTVCALL_HMCALL_M35_KSUS 		0x10000
#define ACTVCALL_HMCALL_M35_PAJU 		0x10001
#define ACTVCALL_HMCALL_M35_PAJU_QUERY		0x10002

#endif
