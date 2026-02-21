/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for avtab
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_AVTAB_H
#define HMSEL_AVTAB_H

#include "adapter/avtab.h"
#include "policydb.h"

static inline void
hmsel_avtab_find_set(struct hmsel_policydb *db,
		     struct hmsel_context *con, const struct hmsel_avtab_key *k)
{
	return adapter_avtab_find_set(&db->adapter_policydb, con, k);
}

#endif
