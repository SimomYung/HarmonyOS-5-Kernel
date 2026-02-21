/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for services
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef HMSEL_SERVICES_H
#define HMSEL_SERVICES_H

#include "policydb.h"
#include <libhmseharmony/pac.h>
#include <libhmseharmony/policycap.h>

#define MAX_PERM 8

struct hmsel_state {
#ifdef CONFIG_HKIP_SEHARMONY_PROT
	bool *policycap;
#else
	bool policycap[HMSEL_POLICYDB_CAPABILITY_MAX];
#endif
	__sec_data(seharmony, ptr) struct hmsel_policy *policy;
	struct raw_mutex policy_lock;
	struct raw_mutex load_lock;
	struct raw_mutex load_fallback_lock;
};

struct hmsel_mapping {
	uint16_t value;
	unsigned int num_perms;
	uint32_t perms[sizeof(uint32_t) * MAX_PERM];
};

struct hmsel_map {
	__sec_data(seharmony, ptr) struct hmsel_mapping *mapping;
	uint16_t size;
};

struct hmsel_policy {
	__sec_data(seharmony, ptr) struct hmsel_sidtab *sidtab;
	struct hmsel_policydb policydb;
	struct hmsel_map map;
	uint32_t latest_granting;
};

static inline void avd_init(struct hmsel_av_decision *avd)
{
	avd->allowed = HMSEL_AVD_NONE;
	avd->auditallow = HMSEL_AVD_NONE;
	avd->auditdeny = HMSEL_AVD_ALL;
	avd->flags = 0;
	avd->audited = 0;
	avd->cmd_idx = 0;
	avd->cmd_len = 0;
}

void hmsel_compute_av(struct hmsel_sid_context sidcon,
		      struct hmsel_av_decision *avd, hmsel_av_t req,
		      struct hmsel_xperms *xperms);
void hmsel_compute_xperms_decisions(struct hmsel_sid_context sidcon,
				    uint8_t driver,
				    struct hmsel_xperms_decision *xpd);
#endif
