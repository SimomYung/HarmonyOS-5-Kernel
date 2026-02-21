/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: exclude_lib.c
* Author: Huawei OS Kernel Lab
* Create: Fri Oct 22 11:21:52 2021
*/

#ifdef HAVE_FOO
int foo(void);
int foo(void)
{
	return 0;
}
#endif
