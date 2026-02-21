/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header for ability hierachy representation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 07 15:45:01 2021
 */
#ifndef ULIBS_LIBSEC_ABILITY_HIERACHY_H
#define ULIBS_LIBSEC_ABILITY_HIERACHY_H

#define __ability_hierachy_arg_nr(a0, a1, a2, a3, a4, a5, a6, a7, a8, n, ...) n
#define ability_hierachy_arg_nr(...) __ability_hierachy_arg_nr(0, ##__VA_ARGS__, 		\
							       8, 7, 6, 5, 4, 3, 2, 1, 0)

#define ___ability_hierachy_concat(x, y) x##y
#define __ability_hierachy_concat(x, y) ___ability_hierachy_concat(x, y)
#define ___ability_hierachy_given_depth_0(...)
#define ___ability_hierachy_given_depth_1(level1, ...) __##level1
#define ___ability_hierachy_given_depth_2(level2, ...) __ability_hierachy_concat(__##level2, \
	___ability_hierachy_given_depth_1(__VA_ARGS__))
#define ___ability_hierachy_given_depth_3(level3, ...) __ability_hierachy_concat(__##level3, \
	___ability_hierachy_given_depth_2(__VA_ARGS__))
#define ___ability_hierachy_given_depth_4(level4, ...) __ability_hierachy_concat(__##level4, \
	___ability_hierachy_given_depth_3(__VA_ARGS__))
#define ___ability_hierachy_given_depth_5(level5, ...) __ability_hierachy_concat(__##level5, \
	___ability_hierachy_given_depth_4(__VA_ARGS__))
#define ___ability_hierachy_given_depth_6(level6, ...) __ability_hierachy_concat(__##level6, \
	___ability_hierachy_given_depth_5(__VA_ARGS__))
#define ___ability_hierachy_given_depth_7(level7, ...) __ability_hierachy_concat(__##level7, \
	___ability_hierachy_given_depth_6(__VA_ARGS__))
#define ___ability_hierachy_given_depth_8(level8, ...) __ability_hierachy_concat(__##level8, \
	___ability_hierachy_given_depth_7(__VA_ARGS__))
#define ___ability_hierachy_given_depth(n, ...) ___ability_hierachy_given_depth_##n(__VA_ARGS__)
#define __ability_hierachy_given_depth(n, ...) __ability_hierachy_concat(ah, \
	___ability_hierachy_given_depth(n, ##__VA_ARGS__))

#define ability_hierachy(...) __ability_hierachy_given_depth(ability_hierachy_arg_nr(__VA_ARGS__), ##__VA_ARGS__)

#endif

