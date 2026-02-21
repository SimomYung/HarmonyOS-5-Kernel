/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for policydb
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef HMSEL_POLICYDB_H
#define HMSEL_POLICYDB_H

#include <libhmseharmony/services.h>

#include "dbcommon.h"
#include "context.h"
#include "policyfile.h"
#include "adapter/policydb.h"

struct hmsel_policydb {
	struct adapter_policydb adapter_policydb;
	hmsel_class_t proc_class;
	unsigned int reject_unknown : 1;
	unsigned int allow_unknown : 1;
};

struct hmsel_ocontext;

int hmsel_policydb_read(struct hmsel_policydb *policydb, struct hmsel_policy_file *policy_file);
int hmsel_policydb_sid_bounded_transition(struct hmsel_policydb *policydb, uint32_t stype, uint32_t ttype);

static inline void
policydb_read_feedback(struct hmsel_policydb *policydb)
{
	policydb->reject_unknown = adapter_policydb_reject_unknown(&policydb->adapter_policydb);
	policydb->allow_unknown = adapter_policydb_allow_unknown(&policydb->adapter_policydb);
	policydb->proc_class = adapter_policydb_proc_class(&policydb->adapter_policydb);
	hm_debug("policy reject unknown value %u\n", policydb->reject_unknown);
	hm_debug("policy allow unknown value %u\n", policydb->allow_unknown);
}

static inline void
hmsel_policydb_destroy(struct hmsel_policydb *policydb)
{
	adapter_policydb_destroy(&policydb->adapter_policydb);
}

static inline hmsel_class_t
hmsel_str_to_class(const struct hmsel_policydb *db, const char *name)
{
	return adapter_str_to_class(&db->adapter_policydb, name);
}

static inline uint32_t
hmsel_str_to_perm(const struct hmsel_policydb *db, hmsel_class_t tclass, const char *name)
{
	return adapter_str_to_perm(&db->adapter_policydb, tclass, name);
}

static inline unsigned int hmsel_policydb_reject_unknown(struct hmsel_policydb *db)
{
	return db->reject_unknown;
}

static inline unsigned int hmsel_policydb_allow_unknown(const struct hmsel_policydb *db)
{
	return db->allow_unknown;
}

static inline int hmsel_policydb_type_permissive(const struct hmsel_policydb *db, uint32_t type)
{
	return adapter_policydb_type_permissive(&db->adapter_policydb, type);
}

static inline void
hmsel_policydb_context_compute_av(const struct hmsel_policydb *db,
				  struct hmsel_chk_context chkcon,
				  struct hmsel_av_decision *avd, hmsel_av_t req,
				  struct hmsel_xperms *xperms)
{
	adapter_policydb_context_compute_av(&db->adapter_policydb, chkcon,
					    avd, req, xperms);
}

static inline void
hmsel_policydb_context_compute_xpermsd(const struct hmsel_policydb *db,
				       struct hmsel_chk_context chkcon,
				       struct hmsel_xperms_decision *xpermd)
{
	adapter_policydb_context_compute_xpermsd(&db->adapter_policydb, chkcon,
						 xpermd);
}

static inline int
hmsel_policydb_user_val(const struct hmsel_policydb *db, const char *name, uint32_t *ret)
{
	return adapter_policydb_user_val(&db->adapter_policydb, name, ret);
}

static inline int
hmsel_policydb_role_val(const struct hmsel_policydb *db, const char *name, uint32_t *ret)
{
	return adapter_policydb_role_val(&db->adapter_policydb, name, ret);
}

static inline int
hmsel_policydb_type_val(const struct hmsel_policydb *db, const char *name, uint32_t *ret)
{
	return adapter_policydb_type_val(&db->adapter_policydb, name, ret);
}

static inline int
hmsel_policydb_mls_val(const struct hmsel_policydb *db, const char *str,
			struct hmsel_context *ctx, struct hmsel_context *def_ctx)
{
	return adapter_policydb_mls_val(&db->adapter_policydb, str, ctx, def_ctx);
}

static inline uint32_t
hmsel_policydb_user_nprim(const struct hmsel_policydb *db)
{
	return adapter_policydb_user_nprim(&db->adapter_policydb);
}

static inline uint32_t
hmsel_policydb_role_nprim(const struct hmsel_policydb *db)
{
	return adapter_policydb_role_nprim(&db->adapter_policydb);
}

static inline uint32_t
hmsel_policydb_type_nprim(const struct hmsel_policydb *db)
{
	return adapter_policydb_type_nprim(&db->adapter_policydb);
}

static inline bool
hmsel_policydb_type_ok(const struct hmsel_policydb *db, uint32_t type, uint32_t role)
{
	return adapter_policydb_type_ok(&db->adapter_policydb, type, role);
}

static inline bool
hmsel_policydb_role_ok(const struct hmsel_policydb *db, uint32_t role, uint32_t user)
{
	return adapter_policydb_role_ok(&db->adapter_policydb, role, user);
}

static inline bool
hmsel_policydb_mls_ok(const struct hmsel_policydb *db, const struct hmsel_context *conp)
{
	return adapter_policydb_mls_ok(&db->adapter_policydb, conp);
}

static inline bool
hmsel_policydb_tclass_valid(const struct hmsel_policydb *db, hmsel_class_t tclass,
			    char *duserp, char *drolep, char *dtypep)
{
	return adapter_policydb_tclass_valid(&db->adapter_policydb, tclass, duserp, drolep, dtypep);
}

static inline hmsel_class_t
hmsel_policydb_proc_class(const struct hmsel_policydb *db)
{
	return db->proc_class;
}

static inline void
hmsel_policydb_role_trans(const struct hmsel_policydb *db, uint32_t *rolep,
			  uint32_t role, uint32_t type, hmsel_class_t tclass)
{
	struct hmsel_role_trans_key k = {role, type, (uint32_t)tclass};
	adapter_policydb_role_trans(&db->adapter_policydb, rolep, &k);
}

static inline int
hmmls_compute_sid(const struct hmsel_policydb *db, struct hmsel_chk_context ccontext,
		uint32_t specified, struct hmsel_context *conp)
{
	return adapter_policydb_mls_range_trans(&db->adapter_policydb, ccontext, specified, conp);
}

static inline int
hmsel_policydb_cap(const struct hmsel_policydb *db, unsigned int idx)
{
	return adapter_policydb_cap(&db->adapter_policydb, idx);
}

static inline void
hmsel_policydb_unknown_cap_log(const struct hmsel_policydb *db)
{
	adapter_policydb_unknown_cap_log(&db->adapter_policydb);
}

static inline const char *
hmsel_policydb_user_name(const struct hmsel_policydb *db, uint32_t user)
{
	return adapter_policydb_user_name(&db->adapter_policydb, user);
}

static inline const char *
hmsel_policydb_role_name(const struct hmsel_policydb *db, uint32_t role)
{
	return adapter_policydb_role_name(&db->adapter_policydb, role);
}

static inline const char *
hmsel_policydb_type_name(const struct hmsel_policydb *db, uint32_t type)
{
	return adapter_policydb_type_name(&db->adapter_policydb, type);
}

static inline char *
hmsel_policydb_mls_str(const struct hmsel_policydb *db, const struct hmsel_context *conp,
				char *stack_buf, size_t stack_buf_len)
{
	return adapter_policydb_mlsrange_str(&db->adapter_policydb, conp,
						stack_buf, stack_buf_len);
}

static inline const char *
hmsel_policydb_class_name(const struct hmsel_policydb *db, uint32_t cls)
{
	return adapter_policydb_class_name(&db->adapter_policydb, cls);
}

static inline int
hmsel_policydb_transition_check(const struct hmsel_policydb *db, const struct hmsel_context *ocon,
				const struct hmsel_context *ncon, const struct hmsel_context *tcon,
				hmsel_class_t classval)
{
	return adapter_policydb_transition_check(&db->adapter_policydb, ocon, ncon, tcon, classval);
}

static inline bool
hmsel_policydb_mls_enabled(const struct hmsel_policydb *db)
{
	return adapter_policydb_mls_enabled(&db->adapter_policydb);
}

#ifdef CONFIG_SEHARMONY_POLICY_SPLIT
static inline void
hmsel_class_callback_register(int (*fn)(unsigned char *, size_t))
{
	adapter_class_callback_register(fn);
}

static inline void
hmsel_class_callback_deregister(void)
{
	adapter_class_callback_deregister();
}
#endif

#ifdef CONFIG_HKIP_SEHARMONY_PROT
static inline void
hmsel_prmem_callback_register(void *(*malloc_fn)(size_t),
	void *(*calloc_fn)(size_t, size_t), char *(*strdup_fn)(const char *), void (*free_fn)(void *))
{
	adapter_prmem_callback_register(malloc_fn, calloc_fn, strdup_fn, free_fn);
}

static inline void
hmsel_prmem_callback_deregister(void)
{
	adapter_prmem_callback_deregister();
}
#endif

static inline uint32_t hmsel_avtab_num(const struct hmsel_policydb *db)
{
	return adapter_avtab_num(&db->adapter_policydb);
}
#endif
