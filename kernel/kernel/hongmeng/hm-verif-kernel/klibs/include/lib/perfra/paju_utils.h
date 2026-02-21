/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The Patch-Jump utilites
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 29 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_PAJU_UTILS_H
#define KLIBS_PERFRA_PAJU_UTILS_H

#include <hmasm/types.h>
#include <hmkernel/errno.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __PAJU_IS_LE				1
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define __PAJU_IS_LE				0
#else
#define __PAJU_IS_LE				1
#endif

#ifndef __PAJU_NULL
#ifdef __cplusplus
#define __PAJU_NULL				(0L)
#else
#define __PAJU_NULL				((void *)0)
#endif
#endif

#define __PAJU_VALUE_TRUE			1
#define __PAJU_VALUE_FALSE			0

#define __paju_str_(...) 			#__VA_ARGS__
#define __paju_str(...)				__paju_str_(__VA_ARGS__)

#define __paju_comb_(a, b)			a##b
#define __paju_comb(a, b)			__paju_comb_(a, b)

#define __paju_asm_goto(n...) 			do { asm goto(n); asm(""); } while (0)
#define __paju_asm_volatile(n...)		do { asm (n);  asm(""); } while (0)

#define __paju_sec_start_(sec_name)			__start_##sec_name
#define __paju_sec_start(sec_name)			__paju_sec_start_(sec_name)
#define __paju_sec_stop_(sec_name)			__stop_##sec_name
#define __paju_sec_stop(sec_name)			__paju_sec_stop_(sec_name)

#define __paju_name_(group_name, point_name)			group_name##_##point_name##_point
#define __paju_name(group_name, point_name)			__paju_name_(group_name, point_name)
#define __paju_name_str(group_name, point_name)		__paju_str(__paju_name(group_name, point_name))

#define __paju_pointsec_name_(group_name)			group_name##_pointsec
#define __paju_pointsec_name(group_name)			__paju_pointsec_name_(group_name)
#define __paju_pointsec_name_str(group_name)		__paju_str(__paju_pointsec_name(group_name))

#define __paju_pointsec_start_(group_name)			__paju_sec_start(__paju_pointsec_name(group_name))
#define __paju_pointsec_start(group_name)			__paju_pointsec_start_(group_name)
#define __paju_pointsec_start_str(group_name)		__paju_str(__paju_pointsec_start(group_name))

#define __paju_pointsec_stop_(group_name)			__paju_sec_stop(__paju_pointsec_name(group_name))
#define __paju_pointsec_stop(group_name)			__paju_pointsec_stop_(group_name)
#define __paju_pointsec_stop_str(group_name)		__paju_str(__paju_pointsec_stop(group_name))

/*
 * Naming rule:
 *	*_point: the variable for the target points
 *	*_pointsec: the corresponding point variable's section
 */
#define __paju_group_data_name_(group_name)		group_name##_group_data
#define __paju_group_data_name(group_name)		__paju_group_data_name_(group_name)
#define __paju_group_data_name_str(group_name)		__paju_str(__paju_group_data_name(group_name))

#define __paju_group_datasec_name			paju_group_datasec
#define __paju_group_datasec_start			__paju_sec_start(__paju_group_datasec_name)
#define __paju_group_datasec_stop			__paju_sec_stop(__paju_group_datasec_name)

#define __paju_group_datasec_name_str			__paju_str(__paju_group_datasec_name)
#define __paju_group_datasec_start_str			__paju_str(__paju_group_datasec_start)
#define __paju_group_datasec_stop_str			__paju_str(__paju_group_datasec_stop)

#define __paju_asm_size_weak_type(symbol_name_str, type) \
	"\n\t.size " symbol_name_str ", .-" symbol_name_str \
	"\n\t.weak " symbol_name_str \
	"\n\t.type " symbol_name_str ", " type

#define __paju_asm_size_global_type(symbol_name_str, type, symbol_size) \
	"\n\t.size " symbol_name_str ", " symbol_size \
	"\n\t.global " symbol_name_str \
	"\n\t.type " symbol_name_str ", " type

#define __PAJU_BIT32				32
static inline __u32 __paju_mask32(
	__u32 val,
	__u32 bit,
	__u32 shift)
{
	__u32 bit_inrange = bit % __PAJU_BIT32;
	__u32 shift_inrange = shift % __PAJU_BIT32;
	return (val & ((1 << bit_inrange) - 1)) << shift_inrange;
}
#undef __PAJU_BIT32

static inline __u32 __paju_bswap32(
	__u32 val,
	__bool swap)
{
	__u32 tmp = val;
	if (swap) {
		tmp = __builtin_bswap32(val);
	}
	return tmp;
}

#define __PAJU_SECTION_NAME_LIMIT 64

#endif
