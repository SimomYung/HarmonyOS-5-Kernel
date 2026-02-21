/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for policydb adapter level
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#include <sepol/policydb/hashtab.h>
#include <sepol/policydb/ebitmap.h>
#include <sepol/policydb/flask_types.h>
#include <sepol/policydb/context.h>
#include <sepol/policydb/services.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include "policydb.h"

static const struct policydb *sepol_db = NULL;

static void sepol_handle_func(void *varg, struct sepol_handle *handle, const char *fmt, ...)
{
	UNUSED(varg, handle);
#if defined(__clang__)
	va_list ap;
	va_start(ap, fmt);
	hm_info(fmt, ap);
	va_end(ap);
#else
	UNUSED(fmt);
#endif
}

int adapter_policydb_read(struct adapter_policydb *policydb, struct hmsel_policy_file *file)
{
	int rc;
	struct policy_file sepol_file;
	if (policydb == NULL || file == NULL) {
		return E_HM_INVAL;
	}
	struct sepol_handle *handle = malloc(sizeof(struct sepol_handle));
	if (handle == NULL) {
		return E_HM_NOMEM;
	}
	mem_zero_s(*handle);
	handle->msg_callback = sepol_handle_func;
	policy_file_init(&sepol_file);
	sepol_file.type = PF_USE_MEMORY;
	sepol_file.data = (char *)(uintptr_t)(file->data);
	sepol_file.len = file->len;
	sepol_file.handle = handle;
	rc = policydb_init(&policydb->p);
	if (rc != 0) {
		hm_error("failed to init policydb in libsepolicy, err=%d\n", rc);
	}
	if (rc == 0) {
		rc = policydb_read(&policydb->p, &sepol_file, 1);
		if (rc != 0) {
			hm_error("failed to load policydb in libsepolicy, rc=%d\n", rc);
		} else {
			/* replace set policydb procedure to update function */
			/* memory sequence is guaranteed by atomic seq in services.c */
			(void)sepol_set_policydb(&policydb->p);
			sepol_db = &policydb->p;
		}
	}
	rc = (rc == 0) ? E_HM_OK : E_HM_INVAL;
	free(handle);
	return rc;
}

static int str_to_perm_check(const struct adapter_policydb *db,
			     hmsel_class_t tclass, const char *name)
{
	int rc = E_HM_OK;
	if (db == NULL || name == NULL ||
	    tclass == 0 || tclass > db->p.p_classes.nprim) {
		rc = E_HM_INVAL;
		hm_error("unrecognized class %"PRIu16"\n", tclass);
	}
	return rc;
}

uint32_t adapter_str_to_perm(const struct adapter_policydb *db, hmsel_class_t tclass, const char *name)
{
	uint32_t ret = 0;
	if (str_to_perm_check(db, tclass, name) == E_HM_OK) {
		perm_datum_t *perm_datum = NULL;
		class_datum_t *class_datum = db->p.class_val_to_struct[tclass - 1];
		common_datum_t *com_datum = class_datum->comdatum;

		if (com_datum != NULL) {
			perm_datum = (perm_datum_t *)hashtab_search(com_datum->permissions.table, name);
		}
		if (perm_datum == NULL) {
			perm_datum = (perm_datum_t *)hashtab_search(class_datum->permissions.table, name);
		}
		if (perm_datum != NULL) {
			ret = (1U << (perm_datum->s.value - 1));
		}
	}
	return ret;
}

void adapter_policydb_context_compute_av(const struct adapter_policydb *db,
					 struct hmsel_chk_context chkcon,
					 struct hmsel_av_decision *avd,
					 hmsel_av_t req,
					 struct hmsel_xperms *xperms)
{
	if (db == NULL || avd == NULL) {
		hm_error("failed to check av compute input params\n");
		return;
	}
	if (sepol_db != (&db->p)) {
		hm_error("input db is not current db\n");
		return;
	}
	/* support mls range context and seqno */
	struct hmsel_context *sconp = chkcon.scon;
	struct hmsel_context *tconp = chkcon.tcon;
	context_struct_t sepol_scon = {0};
	context_struct_t sepol_tcon = {0};
	context_to_sepol(sconp, &sepol_scon);
	context_to_sepol(tconp, &sepol_tcon);

	sepol_security_class_t sepol_tclass = (sepol_security_class_t)chkcon.tclass;
	struct sepol_av_decision sepol_avd = { avd->allowed, 0, avd->auditallow, avd->auditdeny, 0 };
	sepol_access_vector_t sepol_req = (sepol_access_vector_t)req;

	struct sepol_xperms sepol_xperms = {0};
	(void)context_compute_av_norbuf(&sepol_scon, &sepol_tcon, sepol_tclass,
					sepol_req, &sepol_avd, NULL, &sepol_xperms);
	avd_from_sepol(avd, &sepol_avd);
	if (adapter_policydb_type_permissive(db, sconp->type) != 0) {
		avd->flags |= HMSEL_AVD_FLAGS_PERMISSIVE;
	}
	if (xperms != NULL) {
		xperms_from_sepol(xperms, &sepol_xperms);
	}
	mls_free(&sepol_scon, true);
	mls_free(&sepol_tcon, true);
}

static void
xpermd_from_sepol(struct hmsel_xperms_decision *xpermd,
		  struct sepol_xperms_decision *sel_xpermsd)
{
	xpermd->used = sel_xpermsd->used;
	NOFAIL(memcpy_s(xpermd->allowed->p, sizeof(xpermd->allowed->p),
			sel_xpermsd->allowed->p, sizeof(sel_xpermsd->allowed->p)));
	NOFAIL(memcpy_s(xpermd->auditallow->p, sizeof(xpermd->auditallow->p),
			sel_xpermsd->auditallow->p, sizeof(sel_xpermsd->auditallow->p)));
	NOFAIL(memcpy_s(xpermd->dontaudit->p, sizeof(xpermd->dontaudit->p),
			sel_xpermsd->dontaudit->p, sizeof(sel_xpermsd->dontaudit->p)));
}

void
adapter_policydb_context_compute_xpermsd(const struct adapter_policydb *db,
					 struct hmsel_chk_context chkcon,
					 struct hmsel_xperms_decision *xpermd)
{
	if (db == NULL || xpermd == NULL) {
		hm_error("failed to check av compute input params\n");
		return;
	}
	if (sepol_db != (&db->p)) {
		hm_error("input db is not current db\n");
		return;
	}
	/* support mls range context and seqno */
	struct hmsel_context *sconp = chkcon.scon;
	struct hmsel_context *tconp = chkcon.tcon;
	context_struct_t sepol_scon = {0};
	context_struct_t sepol_tcon = {0};
	context_to_sepol(sconp, &sepol_scon);
	context_to_sepol(tconp, &sepol_tcon);

	sepol_security_class_t sepol_tclass = (sepol_security_class_t)chkcon.tclass;
	struct sepol_xperms_decision sel_xpermsd = {0};
	struct sepol_xperms_data allowed = {0};
	struct sepol_xperms_data auditallow = {0};
	struct sepol_xperms_data dontaudit = {0};
	sel_xpermsd.used = xpermd->used;
	sel_xpermsd.driver = xpermd->driver;
	sel_xpermsd.allowed = &allowed;
	sel_xpermsd.auditallow = &auditallow;
	sel_xpermsd.dontaudit = &dontaudit;
	(void)context_compute_xpermsd(&sepol_scon, &sepol_tcon, sepol_tclass,
				      &sel_xpermsd);
	xpermd_from_sepol(xpermd, &sel_xpermsd);
	mls_free(&sepol_tcon, true);
	mls_free(&sepol_scon, true);
}

int adapter_policydb_sid_bounded_transition(const struct adapter_policydb *db, uint32_t stype, uint32_t ttype)
{
	uint32_t i;
	int ret = E_HM_PERM;
	type_datum_t *type_datum = NULL;
	if (db == NULL) {
		hm_error("input db invalid\n");
		return E_HM_INVAL;
	}

	/* type or domain unchanged */
	if (stype == ttype) {
		return E_HM_OK;
	}

	i = ttype;
	type_datum = db->p.type_val_to_struct[i - 1];
	while ((type_datum != NULL) && (type_datum->bounds != 0)) {
		if (type_datum->bounds == stype) {
			ret = E_HM_OK;
			break;
		}
		i = type_datum->bounds;
		type_datum = db->p.type_val_to_struct[i - 1];
	}

	return ret;
}

#define CONSTRAIN_NODE_MAX 1024
int adapter_policydb_transition_check(const struct adapter_policydb *db, const struct hmsel_context *ocon,
				      const struct hmsel_context *ncon, const struct hmsel_context *tcon,
				      hmsel_class_t classval)
{
	if (db == NULL || ocon == NULL || ncon == NULL || tcon == NULL ||
	    classval == 0u || classval > db->p.p_classes.nprim) {
		hm_error("invalid input parameter\n");
		return E_HM_INVAL;
	}
	context_struct_t sepol_ocon = { 0 };
	context_struct_t sepol_ncon = { 0 };
	context_struct_t sepol_tcon = { 0 };
	context_to_sepol(ocon, &sepol_ocon);
	context_to_sepol(ncon, &sepol_ncon);
	context_to_sepol(tcon, &sepol_tcon);
	class_datum_t *cdatum = db->p.class_val_to_struct[classval - 1];
	int try_times = 0;
	constraint_node_t *cons = cdatum->validatetrans;
	while (cons != NULL && try_times < CONSTRAIN_NODE_MAX) {
		if (constraint_expr_eval(&sepol_ocon, &sepol_ncon, &sepol_tcon, classval, cons) == 0) {
			mls_free(&sepol_ocon, true);
			mls_free(&sepol_ncon, true);
			mls_free(&sepol_tcon, true);
			return E_HM_PERM;
		}
		cons = cons->next;
		try_times++;
	}
	mls_free(&sepol_ocon, true);
	mls_free(&sepol_ncon, true);
	mls_free(&sepol_tcon, true);
	return E_HM_OK;
}

void mls_free(context_struct_t *sepol_conp, bool is_one_block)
{
	mls_level_t low_level = sepol_conp->range.level[0];
	mls_level_t high_level = sepol_conp->range.level[1];
	if (is_one_block) {
		if (low_level.cat.node != NULL) {
			free(low_level.cat.node);
			low_level.cat.node = NULL;
			high_level.cat.node = NULL;
		} else if (high_level.cat.node != NULL) {
			free(high_level.cat.node);
			high_level.cat.node = NULL;
		}
	} else {
		ebitmap_destroy_nohkip(&low_level.cat);
		ebitmap_destroy_nohkip(&high_level.cat);
	}
}

int mls_from_sepol(struct hmmls_level **levels, const mls_range_t range)
{
	// caculate level_size and alloc
	uint32_t low_cat_nums = 0, high_cat_nums = 0;
	ebitmap_node_t *tmp = NULL;
	for (tmp = range.level[0].cat.node; tmp; tmp = tmp->next) low_cat_nums++;
	for (tmp = range.level[1].cat.node; tmp; tmp = tmp->next) high_cat_nums++;

	size_t cat_size = (low_cat_nums + high_cat_nums) * sizeof(struct hm_catmap_node);
	size_t range_size = cat_size + MLS_LEVEL_NUM * (sizeof(struct hmmls_level) - sizeof(struct hm_catmap_node));
	*levels = (struct hmmls_level *)malloc(range_size);
	if (*levels == NULL) {
		hm_error("failed to malloc for mls levels!\n");
		return E_HM_NOMEM;
	}

	// copy low_level
	struct hmmls_level *low_level = (struct hmmls_level *)(*levels);
	low_level->sens = range.level[0].sens;
	low_level->cat_num = (uint16_t)low_cat_nums;
	low_level->highbit = (uint16_t)range.level[0].cat.highbit;
	tmp = range.level[0].cat.node;
	for (uint32_t i = 0; i < low_cat_nums && tmp; i++, tmp = tmp->next) {
		low_level->node[i].startbit = (uint16_t)tmp->startbit;
		low_level->node[i].map = tmp->map;
	}
	// copy high_level
	struct hmmls_level *high_level = get_high_level(low_level);
	high_level->sens = range.level[1].sens;
	high_level->cat_num = (uint16_t)high_cat_nums;
	high_level->highbit = (uint16_t)range.level[1].cat.highbit;
	tmp = range.level[1].cat.node;
	for (uint32_t i = 0; i < high_cat_nums && tmp; i++, tmp = tmp->next) {
		high_level->node[i].startbit = (uint16_t)tmp->startbit;
		high_level->node[i].map = tmp->map;
	}
	return E_HM_OK;
}

void mls_level_copy(struct hmmls_level *level, const struct hmmls_level *src_level)
{
	level->sens = src_level->sens;
	level->cat_num = src_level->cat_num;
	level->highbit = src_level->highbit;
	for (int i = 0; i < level->cat_num; i++) {
		level->node[i] = src_level->node[i];
	}
}

static int copy_mls_from_defcon(struct hmsel_context *dst_ctx, struct hmsel_context *ctx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	size_t level_size = hmsel_level_size(ctx->level) + hmsel_level_size(get_high_level(ctx->level));
	dst_ctx->flag = 1;
	dst_ctx->levels = malloc(level_size);
	if (dst_ctx->levels == NULL) {
		return E_HM_NOMEM;
	}
	struct hmmls_level *dst_level = dst_ctx->levels;
	struct hmmls_level *src_level = ctx->level;
	mls_level_copy(dst_level, src_level);
	dst_level = get_high_level(dst_level);
	src_level = get_high_level(src_level);
	mls_level_copy(dst_level, src_level);
	return E_HM_OK;
}

int adapter_policydb_mls_val(const struct adapter_policydb *db, const char *str,
			struct hmsel_context *ctx, struct hmsel_context *def_ctx)
{
	if (!hmsel_mls_enabled()) {
		if (str != NULL && def_ctx == NULL) {
			return E_HM_INVAL;
		} else {
			return E_HM_OK;
		}
	}
	if (str == NULL) {
		if (def_ctx == NULL) {
			return E_HM_INVAL;
		}
		return copy_mls_from_defcon(ctx, def_ctx);
	}
	int ret = E_HM_OK;
	char *tmp = strdup(str);
	char *tmp_cp = tmp;
	context_struct_t mls = {0};
	if (!tmp) {
		return E_HM_NOMEM;
	}

	if (mls_context_to_sid(&db->p, '$', &tmp_cp, &mls) < 0) {
		free(tmp);
		mls_free(&mls, false);
		return E_HM_INVAL;
	}
	ret = context_mls_from_sepol(ctx, &mls);

	free(tmp);
	mls_free(&mls, false);
	return ret;
}

void mls_to_sepol(const struct hmmls_level *level, mls_range_t *range)
{
	// copy low_level
	struct hmmls_level *low_level = (struct hmmls_level *)level;
	struct hmmls_level *high_level = get_high_level(low_level);
	uint32_t i = 1;
	ebitmap_node_t *tmp = NULL, *new = NULL;
	range->level[0].sens = low_level->sens;
	range->level[0].cat.highbit = low_level->highbit;
	range->level[1].sens = high_level->sens;
	range->level[1].cat.highbit = high_level->highbit;
	size_t cat_size = sizeof(ebitmap_node_t) * low_level->cat_num + sizeof(ebitmap_node_t) * high_level->cat_num;

	if (cat_size > 0) {
		new = (ebitmap_node_t*)malloc(cat_size);
		if (new == NULL) {
			hm_error("hmsel_context mls_range to sepol_context malloc failed\n");
			range->level[0].cat.highbit = 0;
			range->level[1].cat.highbit = 0;
			return;
		}
		memset_s(new, cat_size, 0, cat_size);
	} else {
		return;
	}

	if (low_level->cat_num > 0) {
		range->level[0].cat.node = new;
		tmp = new;
		for (i = 1; i < low_level->cat_num; i++) {
			tmp->startbit = low_level->node[i - 1].startbit;
			tmp->map = low_level->node[i - 1].map;
			tmp->next = new + i;
			tmp = new + i;
		}
		tmp->startbit = low_level->node[i - 1].startbit;
		tmp->map = low_level->node[i - 1].map;
		tmp->next = NULL;
	}

	if (high_level->cat_num > 0) {
		range->level[1].cat.node = &new[low_level->cat_num];
		tmp = &new[low_level->cat_num];
		for (i = 1; i < high_level->cat_num; i++) {
			tmp->startbit = high_level->node[i - 1].startbit;
			tmp->map = high_level->node[i - 1].map;
			tmp->next = new + low_level->cat_num + i;
			tmp = new + low_level->cat_num + i;
		}
		tmp->startbit = high_level->node[i - 1].startbit;
		tmp->map = high_level->node[i - 1].map;
		tmp->next = NULL;
	}
}

int adapter_policydb_mls_range_trans(const struct adapter_policydb *db, struct hmsel_chk_context ccontext,
					uint32_t specified, struct hmsel_context *conp)
{
	if (!hmsel_mls_enabled()) {
		return E_HM_OK;
	}
	int ret = E_HM_OK;
	context_struct_t scontext = {0};
	context_struct_t tcontext = {0};
	context_struct_t newcontext = {0};
	context_to_sepol(ccontext.scon, &scontext);
	context_to_sepol(ccontext.tcon, &tcontext);
	newcontext.user = conp->user;
	newcontext.role = conp->role;
	newcontext.type = conp->type;
	ret = mls_compute_sid_nohkip(&db->p, &scontext, &tcontext, ccontext.tclass, specified, &newcontext);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}
	if (ret != E_HM_OK) {
		mls_free(&scontext, true);
		mls_free(&tcontext, true);
		mls_free(&newcontext, false);
	} else {
		ret = context_from_sepol(conp, &newcontext);
		mls_free(&scontext, true);
		mls_free(&tcontext, true);
		mls_free(&newcontext, false);
	}
	return ret;
}

char *adapter_policydb_mlsrange_str(const struct adapter_policydb *db, const struct hmsel_context *conp,
							char *stack_buf, size_t stack_buf_len)
{
	context_struct_t mls = {0};
	char *p = NULL, *ptr = NULL, *ptr2 = NULL;
	int ret;
	context_to_sepol(conp, &mls);
	size_t buf_len;

	/* Temporary buffer - length + NULL terminator */
	size_t len = (size_t)(unsigned int)mls_compute_context_len(&db->p, &mls) + 1;
	buf_len = len + len - 1;
	if (buf_len > stack_buf_len) {
		/* alloc ptr and ptr2 at a time */
		p = (char *)malloc(buf_len);
		if (p == NULL) {
			mls_free(&mls, true);
			return NULL;
		}
	} else {
		p = stack_buf;
	}
	/* Final string w/ ':' cut off */
	ptr2 = p;
	ptr = p + len - 1;

	mls_sid_to_context(&db->p, &mls, &ptr);
	ptr -= (len - 1);
	ret = strcpy_s(ptr2, len - 1, ptr + 1);
	if (ret != 0) {
		if (buf_len > stack_buf_len)
			free(p);
		mls_free(&mls, true);
		hm_error("failed to strcpy from sepol ptr!\n");
		return NULL;
	}

	mls_free(&mls, true);
	return ptr2;
}
