/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for filename trans
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef ADAPTER_FILENAME_TRANS_H
#define ADAPTER_FILENAME_TRANS_H

#include "../filename_trans_common.h"

static inline const struct hmsel_fname_trans_datum *
adapter_fname_trans_search(const struct adapter_policydb *db, const struct hmsel_fname_trans_key *k)
{
	return (const struct hmsel_fname_trans_datum *)hashtab_search(db->p.filename_trans, (hashtab_key_t)k);
}

static inline void adapter_fname_trans_find_set(const struct hmsel_fname_trans_datum *d,
						struct hmsel_context *conp, uint32_t stype)
{
	filename_trans_datum_t *datum = (filename_trans_datum_t *)d;
	while (datum != NULL) {
		if (ebitmap_get_bit(&datum->stypes, stype - 1) != 0) {
			conp->type = datum->otype;
			return;
		}
		datum = datum->next;
	}
}

#endif
