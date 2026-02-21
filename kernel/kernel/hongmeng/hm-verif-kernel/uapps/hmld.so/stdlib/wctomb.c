/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Wctomb
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 15 15:57:04 2019
 */
#include <stdlib.h>

#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

int wctomb(char *s, wchar_t wc)
{
	UNUSED(s, wc);
	hm_panic("unexpected use of wctomb\n");
	return -1;
}
