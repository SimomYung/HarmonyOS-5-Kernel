/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Eabi
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 27 12:16:22 2018
 */
#include <hmkernel/compiler.h>

void __aeabi_unwind_cpp_pr0(void);
void __aeabi_unwind_cpp_pr1(void);
void __aeabi_unwind_cpp_pr2(void);

__attribute__((visibility ("hidden"), weak)) void __used __aeabi_unwind_cpp_pr0(void)
{
}

__attribute__((visibility ("hidden"), weak)) void __used __aeabi_unwind_cpp_pr1(void)
{
}

__attribute__((visibility ("hidden"), weak)) void __used __aeabi_unwind_cpp_pr2(void)
{
}
