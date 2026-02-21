/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for ocontext
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_OCONTEXT_H
#define HMSEL_OCONTEXT_H

#include <libhmseharmony/services.h>

#include "oconcommon.h"
#include "adapter/ocontext.h"

static inline struct hmsel_ocontext *
hmsel_ocon_genfs(const struct hmsel_policydb *db, const char *fsname,
		 const char *fpath, hmsel_class_t sc)
{
	return adapter_ocon_genfs(&db->adapter_policydb, fsname, fpath, sc);
}

static inline struct hmsel_ocontext *
hmsel_ocon_fs_use(const struct hmsel_policydb *db, const char *fsname)
{
	return adapter_ocon_fs_use(&db->adapter_policydb, fsname);
}

static inline unsigned int
hmsel_ocon_behavior(const struct hmsel_ocontext *oconp)
{
	return adapter_ocon_behavior(oconp);
}

static inline hmsel_sid_t
hmsel_ocon_sid(const struct hmsel_ocontext *oconp, unsigned int idx)
{
	return adapter_ocon_sid(oconp, idx);
}

static inline void
hmsel_ocon_sid_set(struct hmsel_ocontext *oconp, unsigned int idx, hmsel_sid_t sid)
{
	adapter_ocon_sid_set(oconp, idx, sid);
}

static inline int
hmsel_ocon_context(const struct hmsel_ocontext *oconp,
		   struct hmsel_context *conp, unsigned int idx)
{
	return adapter_ocon_context(oconp, conp, idx);
}

static inline struct hmsel_ocontext *
hmsel_ocon_isids(const struct hmsel_policydb *db)
{
	return adapter_ocon_isids(&db->adapter_policydb);
}

static inline struct hmsel_ocontext *
hmsel_ocon_next(const struct hmsel_ocontext *oconp)
{
	return adapter_ocon_next(oconp);
}

static inline struct hmsel_ocontext *
hmsel_ocon_port(const struct hmsel_policydb *db, uint8_t protocol, uint16_t port)
{
	return adapter_ocon_port(&db->adapter_policydb, protocol, port);
}

static inline struct hmsel_ocontext *
hmsel_ocon_node(const struct hmsel_policydb *db, uint32_t addr)
{
	return adapter_ocon_node(&db->adapter_policydb, addr);
}

static inline struct hmsel_ocontext *
hmsel_ocon_node6(const struct hmsel_policydb *db, uint32_t *addr)
{
	return adapter_ocon_node6(&db->adapter_policydb, addr);
}

#endif
