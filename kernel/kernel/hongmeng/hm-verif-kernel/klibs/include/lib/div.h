/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Implementation of div
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 30 10:57:32 2020
 */
#ifndef KLIBS_DIV_H
#define KLIBS_DIV_H

#include <hmkernel/types.h>

#define __DIV_MUL_MOV_INIT_BIT	32u
#define __DIV_CAL_MAX_DIVIDEND		0xffffffffffffffffull

/*
 * struct divsor_s - record the mul/mov parameter
 * @mul:	the parameter as an amplification factor
 * @mov:	the parameter as a shift factor
 * @src:	the parameter as a denominator
 * @tgt:	the parameter as a numerator
 * @max:	the maximum that can be converted by @mul and @mov
 */
struct divsor_s {
	__u32 mul;
	unsigned int mov;
	__u32 denominator;
	__u32 numerator;
	__u64 max_noof;
	__u64 max_part;
};

struct divsor_s div_cal_exp_divsor(__u32 divisor, __u32 amplification);
__u64 div_cal(__u64 dividend, struct divsor_s *div);

/*
 * div_cal_exp_result - calculate the expression result
 * @dividend:	dividend parameter
 * @div:	struct divsor_s pointer
 * Returns:	multp*mul>>mov
 */
static inline __u64 div_cal_exp_result(__u32 dividend, const struct divsor_s *div)
{
	__u64 result = 0ULL;

	if (div != NULL) {
		result = ((__u64)dividend * (__u64)div->mul) >> div->mov;
	}
	return result;
}
#endif
