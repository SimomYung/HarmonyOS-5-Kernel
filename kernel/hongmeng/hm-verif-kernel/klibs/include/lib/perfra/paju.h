/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The Patch-Jump infrastructure
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 29 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_PAJU_H
#define KLIBS_PERFRA_PAJU_H

#include <lib/perfra/paju_arm.h>
#include <lib/perfra/paju_x86_64.h>
#include <lib/perfra/paju_aarch64.h>

/*
 * We use a separated storing method to store patch-jumping:
 *	* __paju_point_data_s: the recorded point, which contains the 2 jumping locations.
 */
struct __paju_point_data_s {
	void *src_location;
	void *dst_location;
};

struct __paju_point32_data_s {
	__u32 src_location;
	__u32 dst_location;
};

struct __paju_point64_data_s {
	__u64 src_location;
	__u64 dst_location;
};

#define __pajugroup_true(group_name, point_name)	__paju_arch_true(group_name, point_name)
#define __paju_true(point_name) 			__pajugroup_true(paju_default_group, point_name)

#define paju_default_group_datasec_start	__paju_pointsec_start(paju_default_group)
#define paju_default_group_datasec_stop	__paju_pointsec_stop(paju_default_group)

#define paju_true(point_name)			__paju_true(point_name)
#define paju_false(point_name)			(!(paju_true(point_name)))

/*
 * A pajugroup_{true|false}(group_name, point_name) emits the following symbol and section
 *	Section:
 *		* __paju_group_datasec_name:		`paju_group_datasec`
 *		* __paju_pointsec_name(group_name):	`<group_name>_pointsec`
 *	Symbol:
 *		* __paju_group_datasec_start:		`__start_paju_group_datasec`
 *		* __paju_group_datasec_stop:		`__stop_paju_group_datasec`
 *		* __paju_pointsec_start(group_name):	`__start_<group_name>_pointsec`
 *		* __paju_pointsec_stop(group_name):	`__stop_<group_name>_pointsec`
 *		* __paju_group_data_name(group_name):	`<group_name>_group_data`
 *		* __paju_name(group_name, point_name):		`<group_name>_<point_name>_point`
 */
#define pajugroup_true(group_name, point_name) 	__pajugroup_true(group_name, point_name)
#define pajugroup_false(group_name, point_name) 	(!(__pajugroup_true(group_name, point_name)))

extern struct __paju_point_data_s paju_default_group_datasec_start[];
extern struct __paju_point_data_s paju_default_group_datasec_stop[];

/*
 * Currently, the reflecting only supports function ('tT' in link script) for sysmgr. So
 * to support variable reflecting, we have to manually declare the start/stop of paju
 * point section, so that they could be referenced directy in sysmgr.
 */
struct __paju_group_data_s {
	/* The start of the target section */
	struct __paju_point_data_s *group_start;
	/* The stop of the target section */
	struct __paju_point_data_s *group_stop;
};
struct __paju_group32_point_s {
	__u32 group_start;
	__u32 group_stop;
};
struct __paju_group64_point_s {
	__u64 group_start;
	__u64 group_stop;
};

extern struct __paju_group_data_s __paju_group_datasec_start[];
extern struct __paju_group_data_s __paju_group_datasec_stop[];

/*
 * A __paju_group_index(group_name) requires the following symbols:
 *	* __paju_group_data_name(group_name):		`<group_name>_group_data`
 *	* __paju_group_datasec_start:			`__start_paju_group_datasec`
 */
#define __paju_group_index__(group_name) \
	({ \
		extern struct __paju_group_data_s __paju_group_data_name(group_name); \
		&__paju_group_data_name(group_name) - (&__paju_group_datasec_start[0]); \
	})
#define __paju_group_index_(group_name)		((unsigned int)(__paju_group_index__(group_name)))
#define __paju_group_index(group_name)		__paju_group_index_(group_name)

/*
 * A __paju_patch_index(group_name, point_name) requires the following symbols:
 *	* __paju_pointsec_start(group_name):		`__start_<group_name>_pointsec`
 *	* __paju_name(group_name, point_name):			`<group_name>_<point_name>_point`
 */
#define __paju_patch_index__(group_name, point_name) \
	({ \
		extern struct __paju_point_data_s __paju_pointsec_start(group_name)[]; \
		extern struct __paju_point_data_s __paju_name(group_name, point_name); \
		&__paju_name(group_name, point_name) - (&__paju_pointsec_start(group_name)[0]); \
	})
#define __paju_patch_index_(group_name, point_name)	((unsigned int)__paju_patch_index__(group_name, point_name))
#define __paju_patch_index(group_name, point_name)	__paju_patch_index_(group_name, point_name)

enum __paju_arch_e {
	__PAJU_ARM,
	__PAJU_AARCH64,
	__PAJU_X86_64,
	__PAJU_ARCH_MAX,
};
struct __paju_creator_s {
	int (*create_jump)(void *buf, __u64 bufsize, __u64 dst, __u64 src, __bool is_le);
	int (*create_nop)(void *buf, __u64 bufsize);
	unsigned int sizeof_insn;
};

static inline int paju_create_jump(
	void *buf,
	__u64 bufsize,
	__u64 dst,
	__u64 src,
	__bool is_le)
{
	return __arch_paju_create_jump(buf, bufsize, dst, src, is_le);
}

static inline int paju_create_nop(
	void *buf,
	__u64 bufsize)
{
	return __arch_paju_create_nop(buf, bufsize);
}

static inline int paju_switch_jump_in_vspace(
	struct __paju_point_data_s *paju_point)
{
	void *src = paju_point->src_location;
	void *dst = paju_point->dst_location;
	__u64 insn = *((__u64 *)src);
	int err = paju_create_jump(&insn, sizeof(__u64), (__u64)(__uptr_t)dst, (__u64)(__uptr_t)src, __PAJU_IS_LE);
	if (err == E_HM_OK) {
		*((__u64 *)src) = insn;
	}
	return err;
}

static inline int paju_switch_nop_in_vspace(
	struct __paju_point_data_s *paju_point)
{
	void *src = paju_point->src_location;
	__u64 insn = *((__u64 *)src);
	int err = paju_create_nop(&insn, sizeof(__u64));
	if (err == E_HM_OK) {
		*((__u64 *)src) = insn;
	}
	return err;
}

/* The in vspace paju selector */
struct __paju_selector_s {
	const char *dso_name;
	unsigned int group_index;
	unsigned int point_index;
};

#endif
