/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for policydb adapter level
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef HMSEL_ADAPTER_POLICYDB_H
#define HMSEL_ADAPTER_POLICYDB_H

#include "../dbcommon.h"
#include "../symtab.h"
#include "../context.h"
#include "../policyfile.h"
#include "../policycap.h"
#include "mls_level.h"
#include <sepol/policydb/policydb.h>
#include <sepol/policydb/services.h>

#include <hongmeng/compiler.h>
#include <libhmlog/hmlog.h>
#include <sys/types.h>
#include <libhmseharmony/services.h>
#include <libhwsecurec/securec.h>

raw_static_assert(SYM_COMMONS == HMSEL_SYM_COMMONS, commons_valid);
raw_static_assert(SYM_CLASSES == HMSEL_SYM_CLASSES, classess_valid);
raw_static_assert(SYM_ROLES == HMSEL_SYM_ROLES, roles_valid);
raw_static_assert(SYM_TYPES == HMSEL_SYM_TYPES, types_valid);
raw_static_assert(SYM_USERS == HMSEL_SYM_USERS, users_valid);
raw_static_assert(SYM_BOOLS == HMSEL_SYM_BOOLS, bools_valid);
raw_static_assert(SYM_LEVELS == HMSEL_SYM_LEVELS, levels_valid);
raw_static_assert(SYM_CATS == HMSEL_SYM_CATS, cats_valid);
raw_static_assert(SYM_NUM == HMSEL_SYM_NUM, symnum_valid);

raw_static_assert(OBJECT_R_VAL == HMSEL_OBJECT_R_VAL, objr_valid);
raw_static_assert(HMSEL_DEFAULT_SOURCE == DEFAULT_SOURCE, dsrc_valid);
raw_static_assert(HMSEL_DEFAULT_TARGET == DEFAULT_TARGET, ddst_valid);

raw_static_assert(HMSEL_FS_USE_XATTR == SECURITY_FS_USE_XATTR, xattr_valid);
raw_static_assert(HMSEL_FS_USE_TRANS == SECURITY_FS_USE_TRANS, trans_valid);
raw_static_assert(HMSEL_FS_USE_TASK == SECURITY_FS_USE_TASK, task_valid);
raw_static_assert(HMSEL_FS_USE_GENFS == SECURITY_FS_USE_GENFS, genfs_valid);
raw_static_assert(HMSEL_FS_USE_NONE == SECURITY_FS_USE_NONE, fsuse_none_valid);

struct adapter_policydb {
	struct policydb p;
};

struct sepol_handle {
	int msg_level;
	const char *msg_channel;
	const char *msg_fname;
#ifdef __GNUC__
	__attribute__ ((format(printf, 3, 4)))
#endif
	void (*msg_callback) (void *varg,
	struct sepol_handle *handle, const char *fmt, ...);
	void *msg_callback_arg;
	int disable_dontaudit;
	int expand_consume_base;
	int preserve_tunables;
};

raw_static_assert(sizeof(hmsel_sid_t) == sizeof(sepol_security_id_t), sid_size);
raw_static_assert(sizeof(hmsel_class_t) == sizeof(sepol_security_class_t), class_size);

int adapter_policydb_read(struct adapter_policydb *policydb, struct hmsel_policy_file *file);
uint32_t adapter_str_to_perm(const struct adapter_policydb *db, hmsel_class_t tclass, const char *name);
void adapter_policydb_context_compute_av(const struct adapter_policydb *db,
					 struct hmsel_chk_context chkcon,
					 struct hmsel_av_decision *avd,
					 hmsel_av_t req,
					 struct hmsel_xperms *xperms);
void adapter_policydb_context_compute_xpermsd(const struct adapter_policydb *db,
						struct hmsel_chk_context chkcon,
						struct hmsel_xperms_decision *xpermd);
int adapter_policydb_sid_bounded_transition(const struct adapter_policydb *db, uint32_t stype, uint32_t ttype);
int adapter_policydb_transition_check(const struct adapter_policydb *db, const struct hmsel_context *ocon,
				      const struct hmsel_context *ncon, const struct hmsel_context *tcon,
				      hmsel_class_t classval);
void mls_free(context_struct_t *sepol_conp, bool is_one_block);
int mls_from_sepol(struct hmmls_level **levels, const mls_range_t range);
int adapter_policydb_mls_val(const struct adapter_policydb *db, const char *str,
			struct hmsel_context *ctx, struct hmsel_context *def_ctx);
void mls_to_sepol(const struct hmmls_level *level, mls_range_t *range);
int adapter_policydb_mls_range_trans(const struct adapter_policydb *db, struct hmsel_chk_context ccontext,
					uint32_t specified, struct hmsel_context *conp);
char *adapter_policydb_mlsrange_str(const struct adapter_policydb *db, const struct hmsel_context *conp,
					char *stack_buf, size_t stack_buf_len);
void mls_level_copy(struct hmmls_level *level, const struct hmmls_level *src_level);

static inline int
adapter_policydb_user_val(const struct adapter_policydb *db, const char *name, uint32_t *ret)
{
	user_datum_t *user = (user_datum_t *)hashtab_search(db->p.p_users.table, name);
	int rc = E_HM_INVAL;
	if (user != NULL) {
		*ret = user->s.value;
		rc = E_HM_OK;
	}
	return rc;
}

static inline int
adapter_policydb_role_val(const struct adapter_policydb *db, const char *name, uint32_t *ret)
{
	role_datum_t *role = (role_datum_t *)hashtab_search(db->p.p_roles.table, name);
	int rc = E_HM_INVAL;
	if (role != NULL) {
		*ret = role->s.value;
		rc = E_HM_OK;
	}
	return rc;
}

static inline int
adapter_policydb_type_val(const struct adapter_policydb *db, const char *name, uint32_t *ret)
{
	type_datum_t *type = (type_datum_t *)hashtab_search(db->p.p_types.table, name);
	int rc = E_HM_INVAL;
	if (type != NULL) {
		*ret = type->s.value;
		rc = E_HM_OK;
	}
	return rc;
}

static inline unsigned int
adapter_policydb_reject_unknown(struct adapter_policydb *policydb)
{
	return ((policydb->p.handle_unknown & REJECT_UNKNOWN) != 0U) ? 1U : 0U;
}

static inline unsigned int
adapter_policydb_allow_unknown(struct adapter_policydb *policydb)
{
	return ((policydb->p.handle_unknown & ALLOW_UNKNOWN) != 0U) ? 1U : 0U;
}

static inline void
adapter_policydb_destroy(struct adapter_policydb *policydb)
{
	policydb_destroy(&policydb->p);
}

static inline hmsel_class_t
adapter_str_to_class(const struct adapter_policydb *db, const char *name)
{
	hmsel_class_t ret = 0;
	class_datum_t *class_datum = hashtab_search(db->p.p_classes.table, name);
	if (class_datum != NULL) {
		ret = class_datum->s.value;
	}
	return ret;
}

static inline int
adapter_policydb_type_permissive(const struct adapter_policydb *db, uint32_t type)
{
	return ebitmap_get_bit(&db->p.permissive_map, type);
}

static inline void
context_to_sepol(const struct hmsel_context *conp, context_struct_t *sepol_conp)
{
	sepol_conp->user = conp->user;
	sepol_conp->role = conp->role;
	sepol_conp->type = conp->type;
	if (conp->flag) {
		mls_to_sepol(conp->levels, &sepol_conp->range);
	} else {
		mls_to_sepol(conp->level, &sepol_conp->range);
	}
}

static inline int
context_mls_from_sepol(struct hmsel_context *conp, const context_struct_t *sepol_conp)
{
	int ret = E_HM_OK;
	ret = mls_from_sepol(&conp->levels, sepol_conp->range);
	if (ret == E_HM_OK) {
		conp->flag = 1;
	}
	return ret;
}

static inline int
context_from_sepol(struct hmsel_context *conp, const context_struct_t *sepol_conp)
{
	conp->user = sepol_conp->user;
	conp->role = sepol_conp->role;
	conp->type = sepol_conp->type;
	return context_mls_from_sepol(conp, sepol_conp);
}

static inline void
avd_from_sepol(struct hmsel_av_decision *avdp, const struct sepol_av_decision *sepol_avdp)
{
	avdp->allowed = sepol_avdp->allowed;
	avdp->auditallow = sepol_avdp->auditallow;
	avdp->auditdeny = sepol_avdp->auditdeny;
}

static inline void
xperms_from_sepol(struct hmsel_xperms *xperms,
		  const struct sepol_xperms *sepol_xperms)
{
	xperms->len = sepol_xperms->len;
	NOFAIL(memcpy_s(xperms->drivers.p, sizeof(xperms->drivers.p),
			sepol_xperms->drivers.p, sizeof(sepol_xperms->drivers.p)));
}

static inline uint32_t
adapter_policydb_user_nprim(const struct adapter_policydb *db)
{
	return db->p.p_users.nprim;
}

static inline uint32_t
adapter_policydb_role_nprim(const struct adapter_policydb *db)
{
	return db->p.p_roles.nprim;
}

static inline uint32_t
adapter_policydb_type_nprim(const struct adapter_policydb *db)
{
	return db->p.p_types.nprim;
}

static inline bool
adapter_policydb_type_ok(const struct adapter_policydb *db, uint32_t type, uint32_t role)
{
	bool ret = true;
	role_datum_t *rolep = db->p.role_val_to_struct[role - 1];
	if (rolep == NULL || ebitmap_get_bit(&rolep->cache, type - 1) == 0) {
		ret = false;
	}
	return ret;
}

static inline bool
adapter_policydb_role_ok(const struct adapter_policydb *db, uint32_t role, uint32_t user)
{
	bool ret = true;
	user_datum_t *userp = db->p.user_val_to_struct[user - 1];
	if (userp == NULL || ebitmap_get_bit(&userp->cache, role - 1) == 0) {
		ret = false;
	}
	return ret;
}

static inline bool
adapter_policydb_mls_ok(const struct adapter_policydb *db, const struct hmsel_context *conp)
{
	if (!hmsel_mls_enabled()) {
		return true;
	}
	context_struct_t mls = {0};
	context_to_sepol(conp, &mls);
	if (mls_context_isvalid(&db->p, &mls) == 0) {
		mls_free(&mls, true);
		return false;
	}
	mls_free(&mls, true);
	return true;
}

static inline void
adapter_defcon_set(struct class_datum *c, char *duserp, char *drolep, char *dtypep)
{
	*duserp = c->default_user;
	*drolep = c->default_role;
	*dtypep = c->default_type;
}

static inline bool
adapter_policydb_tclass_valid(const struct adapter_policydb *db, hmsel_class_t tclass,
			      char *du, char *dr, char *dt)
{
	bool ret = (tclass != 0 && tclass <= db->p.p_classes.nprim);
	if (ret) {
		adapter_defcon_set(db->p.class_val_to_struct[tclass - 1], du, dr, dt);
	}
	return ret;
}

static inline hmsel_class_t
adapter_policydb_proc_class(const struct adapter_policydb *db)
{
	return db->p.process_class;
}

static inline bool
role_trans_eq(const struct role_trans *tr, const struct hmsel_role_trans_key *k)
{
	return (tr->role == k->role &&
		tr->type == k->type && tr->tclass == k->tclass);
}

static inline bool role_trans_eq_set(uint32_t *rolep, const struct role_trans *tr,
				     const struct hmsel_role_trans_key *k)
{
	bool ret = false;
	if (role_trans_eq(tr, k)) {
		*rolep = tr->new_role;
		ret = true;
	}
	return ret;
}

static inline void adapter_policydb_role_trans(const struct adapter_policydb *db,
					       uint32_t *rolep, const struct hmsel_role_trans_key *k)
{
	struct role_trans *tr = db->p.role_tr;
	for (; tr != NULL; tr = tr->next) {
		if (role_trans_eq_set(rolep, tr, k)) {
			break;
		}
	}
}

static inline int
adapter_policydb_cap(const struct adapter_policydb *db, unsigned int idx)
{
	return ebitmap_get_bit(&db->p.policycaps, idx);
}

static inline void
unknown_cap_log(unsigned int idx)
{
	if (idx >= hmsel_policycap_num()) {
		hm_warn("seharmony unknown policy capability %u\n", idx);
	}
}

static inline void
adapter_policydb_unknown_cap_log(const struct adapter_policydb *db)
{
	ebitmap_node_t *node;
	unsigned int i;
	ebitmap_for_each_positive_bit(&db->p.policycaps, node, i) {
		unknown_cap_log(i);
	}
}

static inline const char *
adapter_policydb_user_name(const struct adapter_policydb *db, uint32_t user)
{
	return db->p.p_user_val_to_name[user - 1];
}

static inline const char *
adapter_policydb_role_name(const struct adapter_policydb *db, uint32_t role)
{
	return db->p.p_role_val_to_name[role - 1];
}

static inline const char *
adapter_policydb_type_name(const struct adapter_policydb *db, uint32_t type)
{
	return db->p.p_type_val_to_name[type - 1];
}

static inline const char *
adapter_policydb_class_name(const struct adapter_policydb *db, uint32_t cls)
{
	return db->p.p_class_val_to_name[cls - 1];
}

static inline bool
adapter_policydb_mls_enabled(const struct adapter_policydb *db)
{
	return db->p.mls != 0;
}

#ifdef CONFIG_SEHARMONY_POLICY_SPLIT
static inline void
adapter_class_callback_register(int (*fn)(unsigned char *, size_t))
{
	class_callback_register(fn);
}

static inline void
adapter_class_callback_deregister(void)
{
	class_callback_deregister();
}
#endif

#ifdef CONFIG_HKIP_SEHARMONY_PROT
static inline void
adapter_prmem_callback_register(void *(*malloc_fn)(size_t),
	void *(*calloc_fn)(size_t, size_t), char *(*strdup_fn)(const char *), void (*free_fn)(void *))
{
	prmem_callback_register(malloc_fn, calloc_fn, strdup_fn, free_fn);
}

static inline void
adapter_prmem_callback_deregister(void)
{
	prmem_callback_deregister();
}
#endif

static inline uint32_t adapter_avtab_num(const struct adapter_policydb *db)
{
	return db->p.te_avtab.nel;
}

#endif
