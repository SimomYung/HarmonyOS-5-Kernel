/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Register the kind of property provided by property mechanism
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 16 11:26:50 2022
 */

/*
 * PROP_REGISTER_FIXED_LEN_TYPE has fixed length of datatype. Including:
 * property name, property type, data type, real memory unit saved on property database, default value
 *
 * PROP_REGISTER_UNFIXED_LEN_TYPE has unfixed length of datatype. Including:
 * property name, property type, data type
 *
 */

#ifdef PROP_REGISTER_FIXED_LEN_TYPE_PROT
PROP_REGISTER_FIXED_LEN_TYPE_PROT(system_service,
				SYSTEM_SERVICE, PROPT_BOOL, uint8_t, 0u)
#else
PROP_REGISTER_FIXED_LEN_TYPE(system_service,
				SYSTEM_SERVICE, PROPT_BOOL, uint8_t, 0u)
#endif

PROP_REGISTER_FIXED_LEN_TYPE(sync_reclaimable,
			     SYNC_RECLAIMABLE, PROPT_BOOL, uint8_t, 1u)

PROP_REGISTER_UNFIXED_LEN_TYPE(integrity,
			       INTEGRITY, PROPT_BINARY)

PROP_REGISTER_FIXED_LEN_TYPE(unkillable,
			     KILLABLE, PROPT_BOOL, uint8_t, 0u)

PROP_REGISTER_FIXED_LEN_TYPE(coredump,
			     COREDUMP, PROPT_BOOL, uint8_t, 1u)

PROP_REGISTER_FIXED_LEN_TYPE(root_fs,
			     ROOT_FS, PROPT_BOOL, uint8_t, 0u)

PROP_REGISTER_FIXED_LEN_TYPE(livepatchable,
			     LIVEPATCHABLE, PROPT_BOOL, uint8_t, 0u)

PROP_REGISTER_FIXED_LEN_TYPE(unstoppable,
			     STOPPABLE, PROPT_BOOL, uint8_t, 0u)

PROP_REGISTER_FIXED_LEN_TYPE(dpa_mode,
			     DPA_MODE, PROPT_UINT32, uint32_t, 0ul)

PROP_REGISTER_FIXED_LEN_TYPE(res_controllable,
			     RES_CONTROLLABLE, PROPT_BOOL, uint8_t, 1u)

PROP_REGISTER_FIXED_LEN_TYPE(getset_affinity,
			     GETSET_AFFINITY, PROPT_BOOL, uint8_t, 0u)

PROP_REGISTER_FIXED_LEN_TYPE(inherit_on_fork,
			     INHERIT_ON_FORK, PROPT_BOOL, uint8_t, 1u)

PROP_REGISTER_FIXED_LEN_TYPE(privilege_num,
			     PRIVILEGE_NUM, PROPT_UINT32, uint32_t, 0u)

PROP_REGISTER_FIXED_LEN_TYPE(support_hkip,
			     SUPPORT_HKIP, PROPT_BOOL, uint8_t, 0u)

PROP_REGISTER_FIXED_LEN_TYPE(devhost_service,
			     DEVHOST_SERVICE, PROPT_BOOL, uint8_t, 0u)
/* New property should register at the end */
