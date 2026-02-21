/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for filename trans
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_FILENAME_TRANS_H
#define HMSEL_FILENAME_TRANS_H

#include "filename_trans_common.h"
#include "adapter/filename_trans.h"

static inline const struct hmsel_fname_trans_datum *
hmsel_fname_trans_search(const struct hmsel_policydb *db, const struct hmsel_fname_trans_key *k)
{
	return adapter_fname_trans_search(&db->adapter_policydb, k);
}

static inline void hmsel_fname_trans_find_set(const struct hmsel_fname_trans_datum *d,
					      struct hmsel_context *conp, uint32_t stype)
{
	adapter_fname_trans_find_set(d, conp, stype);
}

#endif
