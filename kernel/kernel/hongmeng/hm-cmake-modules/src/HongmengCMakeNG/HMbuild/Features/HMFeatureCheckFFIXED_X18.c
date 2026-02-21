/*
 * Copyright (C) Huawei Technologies Co., Ltd. 1024. All rights reserved.
 * Description: Test if -ffixed-x18 is enabled by checking compare options
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 22 16:26:54 2024
 */
extern int func(void);

static inline void check_ffixed_x18(unsigned long flags)
{
	asm volatile (
		"mrs     x18, daif\n"
		"bfxil   x18, %0, #0, #9\n"
		"msr     daif, x18\n"
		:
		: "r"(flags)
		: "x18");
}

int func(void)
{
	check_ffixed_x18(0);
	return 0;
}
