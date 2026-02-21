/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for adapter avtab
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_ADAPTER_AVTAB_H
#define HMSEL_ADAPTER_AVTAB_H

#include <sepol/policydb/avtab.h>
#include "../avtabcommon.h"
#include "policydb.h"

raw_static_assert(AVTAB_ALLOWED == HM_AVTAB_ALLOWED, allow);
raw_static_assert(AVTAB_AUDITALLOW == HM_AVTAB_AUDITALLOW, auditallow);
raw_static_assert(AVTAB_AUDITDENY == HM_AVTAB_AUDITDENY, auditdeny);
raw_static_assert(AVTAB_AV == HM_AVTAB_AV, av);
raw_static_assert(AVTAB_TRANSITION == HM_AVTAB_TRANSITION, transition);
raw_static_assert(AVTAB_MEMBER == HM_AVTAB_MEMBER, member);
raw_static_assert(AVTAB_CHANGE == HM_AVTAB_CHANGE, change);
raw_static_assert(AVTAB_TYPE == HM_AVTAB_TYPE, type);
raw_static_assert(AVTAB_XPERMS_ALLOWED == HM_AVTAB_XPERMS_ALLOWED, xallow);
raw_static_assert(AVTAB_XPERMS_AUDITALLOW == HM_AVTAB_XPERMS_AUDITALLOW, xauditallow);
raw_static_assert(AVTAB_XPERMS_DONTAUDIT == HM_AVTAB_XPERMS_DONTAUDIT, xdontdeny);
raw_static_assert(AVTAB_XPERMS == HM_AVTAB_XPERMS, xperms);
raw_static_assert(AVTAB_ENABLED_OLD == HM_AVTAB_ENABLED_OLD, enabled_old);
raw_static_assert(AVTAB_ENABLED == HM_AVTAB_ENABLED, enabled);

raw_static_assert(sizeof(struct hmsel_avtab_key) == sizeof(avtab_key_t), avtab_size);
raw_static_assert(offsetof(struct hmsel_avtab_key, stype) ==
		  offsetof(avtab_key_t, source_type), st_offset);
raw_static_assert(offsetof(struct hmsel_avtab_key, ttype) ==
		  offsetof(avtab_key_t, target_type), tt_offset);
raw_static_assert(offsetof(struct hmsel_avtab_key, tclass) ==
		  offsetof(avtab_key_t, target_class), tclass_offset);
raw_static_assert(offsetof(struct hmsel_avtab_key, specified) ==
		  offsetof(avtab_key_t, specified), spec_offset);

static inline avtab_datum_t *avtab_cond_search(const avtab_key_t *k, avtab_ptr_t ptr)
{
	avtab_ptr_t pos = ptr;
	for (; pos != NULL; pos = avtab_search_node_next(pos, k->specified)) {
		if (pos->key.specified & AVTAB_ENABLED) {
			return &pos->datum;
		}
	}
	return NULL;
}

static inline void
avtab_set(const avtab_datum_t *sepol_v, struct hmsel_context *con)
{
	if(sepol_v != NULL) {
		con->type = sepol_v->data;
	}
}

static inline void adapter_avtab_find_set(struct adapter_policydb *db,
					  struct hmsel_context *con, const struct hmsel_avtab_key *k)
{
	avtab_datum_t *sepol_v = avtab_search(&db->p->te_avtab, (avtab_key_t *)k);
	if (sepol_v == NULL) {
		avtab_ptr_t node = avtab_search_node(&db->p->te_cond_avtab, (avtab_key_t *)k);
		sepol_v = avtab_cond_search((avtab_key_t *)k, node);
	}
	avtab_set(sepol_v, con);
}

#endif
