/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for sid table
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef HMSEL_SIDTAB_H
#define HMSEL_SIDTAB_H

#include <libhmsync/raw_rwlock.h>
#include "policydb.h"
#include "sidtbl.h"

#ifdef __HOST_LLT__
#define STATIC
#define SIDTAB_RSTBL_START 1
#define SIDTAB_GLOBAL_START 1
#define SIDTAB_GLOBAL_MAX 50
#define SIDTAB_LOCAL_START SIDTAB_GLOBAL_MAX
#define SIDTAB_LOCAL_MAX 50
#define SIDTAB_MAX (SIDTAB_LOCAL_MAX + SIDTAB_GLOBAL_MAX)
#else
#define STATIC static
#define SIDTAB_RSTBL_START 1
#define SIDTAB_GLOBAL_START 1
#define SIDTAB_GLOBAL_MAX CONFIG_SEHARMONY_SIDTAB_GLOBAL_MAX
#define SIDTAB_LOCAL_START SIDTAB_GLOBAL_MAX
#define SIDTAB_LOCAL_MAX CONFIG_SEHARMONY_SIDTAB_LOCAL_MAX
#define SIDTAB_MAX (SIDTAB_LOCAL_MAX + SIDTAB_GLOBAL_MAX)
#endif

struct sid_rsearch_entry {
	hmsel_sid_t sid;
};

struct hmsel_sidtab {
	struct hmsel_sid_ctx_tbl global_tab;
	struct hmsel_sid_ctx_tbl local_tab;
	struct sid_rsearch_entry *rstable; // start from entry 1
	int gsid_shm_fd;
	uint32_t rsnum; // first free rstable entry idx
	struct raw_rwlock rsearch_lock;
};

struct hmsel_context *hmsel_sidtab_search(struct hmsel_sidtab *s, hmsel_sid_t sid);
struct hmsel_context *hmsel_sidtab_search_force(struct hmsel_sidtab *s, hmsel_sid_t sid);
int hmsel_sidtab_context_to_sid(struct hmsel_sidtab *s, struct hmsel_context *context, hmsel_sid_t *sidp);
int hmsel_sidtab_global_context_to_sid(struct hmsel_sidtab *s, struct hmsel_context *context, hmsel_sid_t *sidp);
int hmsel_sidtab_init(struct hmsel_policydb *db, struct hmsel_sidtab *s, unsigned int flags);
void hmsel_sidtab_destroy(struct hmsel_sidtab *s);
const char *isid_to_context_str(hmsel_sid_t sid);
int hmsel_context_cmp(const struct hmsel_context *s, const struct hmsel_context *t);
struct hmsel_context *sidtab_context_of(struct hmsel_sidtab *s, hmsel_sid_t sid);

static inline void hmsel_context_free(struct hmsel_context *context)
{
	if (context->flag == 1 && context->levels != NULL) {
		free(context->levels);
		context->flag = 0;
		context->levels = NULL;
	}
}

#endif
