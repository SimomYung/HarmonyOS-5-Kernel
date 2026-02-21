/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: longjmp_chk
 */
#include "setjmp.h"

void __longjmp_chk(jmp_buf env, int val)
{
	longjmp(env, val);
}
