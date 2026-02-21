/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of memory types
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 28 14:41:21 2019
 */
#ifndef SYSMGR_MEM_MEMDEF_H
#define SYSMGR_MEM_MEMDEF_H
#include <stdint.h>
#include <stdbool.h>
#include <libmem/flags.h>
#include <libhmlog/hmlog.h>

#define STR_BACKEND_NONE		"[reserve]"
#define STR_BACKEND_ANON		"[anonymous]"
#define STR_BACKEND_FILE		"[file]"
#ifdef CONFIG_HMKIT_FDOFS
#define STR_BACKEND_VNODES		"[vnodes]"
#endif
#define STR_BACKEND_IOMM		"[io]"
#define STR_BACKEND_PREH		"[prehistoric]"
#define STR_BACKEND_KSHARE		"[kshare]"
#define STR_BACKEND_SHMM		"[shmm]"
#define STR_BACKEND_UNKN		"[unknown]"
#define STR_BACKEND_CONTIG		"[contiguous]"

static inline bool is_safe_map(__maybe_unused uint32_t flags)
{
#ifdef CONFIG_SFI
	return !((flags & (uint32_t)MFLAG_MAP_SAFE) == 0U);
#else
	return false;
#endif
}

static inline bool is_fixed_map(uint32_t flags)
{
	bool rc = true;

	if (((flags & MFLAG_MAP_FIXED) == 0U) &&
	    ((flags & MFLAG_MAP_FIXED_NOREPLACE) == 0U)) {
		rc = false;
	}
	return rc;
}

static inline bool is_map_to_xpm(uint32_t flags)
{
#ifdef CONFIG_SECURITY_XPM
	return ((flags & MFLAG_MAP_XPM) != 0U);
#else
	return false;
#endif
}

#endif /* SYSMGR_MEM_MEMDEF_H */
