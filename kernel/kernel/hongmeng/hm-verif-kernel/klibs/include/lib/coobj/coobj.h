/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its container
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_COOBJ_H
#define KLIBS_COOBJ_COOBJ_H

#include <lib/coobj/template.h>

#define COOBJ_VSPACEID_CUSTOM_NR 0x10
enum coobj_vspaceid_e {
	COOBJ_VSPACEID_SYSMGR,
	COOBJ_VSPACEID_VFS,
	COOBJ_VSPACEID_HMNETD,
	COOBJ_VSPACEID_DEVMGR,
	COOBJ_VSPACEID_DEVHOST,
	COOBJ_VSPACEID_TPPMGR,
	COOBJ_VSPACEID_HGUARD,
	COOBJ_VSPACEID_AUDITMGR,
	COOBJ_VSPACEID_KEYMGR,
	COOBJ_VSPACEID_VMMGR,
	/* reserved for uapp */
	/*
	COOBJ_VSPACEID_CUSTOM_BASE,
	COOBJ_VSPACEID_CUSTOM_LIMIT = COOBJ_VSPACEID_CUSTOM_BASE + COOBJ_VSPACEID_CUSTOM_NR,
	*/
	COOBJ_VSPACEID_MAX,
};

/* The shared data type list */
enum coobj_typeid_e {
	COOBJ_TYPE_CRED,
	COOBJ_TYPE_SECCOMP,
#ifdef CONFIG_FILEGUARD
	COOBJ_TYPE_FILEGUARD,
#endif
	COOBJ_TYPE_MAX,
};

/* The allocator types, such as array/radix1 */
enum coobj_ator_kind_e {
	COOBJ_ATOR_ARRAY,
	COOBJ_ATOR_RADIX1,
	COOBJ_ATOR_RADIX1VAR,
};

/* The role types. Currently, there are only 3 roles.
 *   * mapper
 *   * holder
 *   * renter
 *
 * However, for the actual data, we only consider from the user's perspective.
 */
enum coobj_role_kind_e {
	COOBJ_ROLE_HOLDER,
	COOBJ_ROLE_RENTER,
};

enum coobj_invalid_kind_e {
	COOBJ_VSPACEID_INVALID = (coobj_vspaceid_t)-1,
	COOBJ_TYPE_INVALID = (coobj_typeid_t)-1,
	COOBJ_ATOR_INVALID = (coobj_ator_kind_t)-1,
	COOBJ_ROLE_INVALID = (coobj_role_kind_t)-1,
};

typedef enum coobj_role_kind_e coobj_role;
/* variable-size ator with 16B min size */
struct coobj_var_s {
	unsigned char min_object[RADIX1VAR_MIN_OBJ_SIZE];
};

#define COOBJ_VAR_TEPAR_CHUNK_ORDER 0x9
#ifdef CONFIG_SECCOMP
#define COOBJ_VAR_TEPAR_LEVEL1_ORDER 0xc
#else
#define COOBJ_VAR_TEPAR_LEVEL1_ORDER 0x8
#endif

coobj_define_radix1var(
	var, struct coobj_var_s,
	COOBJ_VAR_TEPAR_CHUNK_ORDER, COOBJ_VAR_TEPAR_LEVEL1_ORDER);

#endif
