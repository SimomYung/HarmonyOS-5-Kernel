/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for exported interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 25 16:10:43 2023
 */

#include <libhmlog/hmlog.h>
#include <libhmseharmony/services.h>

#include "services.h"

#ifdef CONFIG_HKIP_SEHARMONY_PROT
#include <libhmkip/prmem.h>
#include <libhmsrv_sys/hm_hkip.h>

static int verify_tampering(void *p, const char *msg)
{
	unsigned char *temp;
	volatile unsigned char tmp_char;
	if (!p) {
		return E_HM_OK;
	}
	temp = (unsigned char *)p;
	tmp_char = *temp;
	if (hm_hkip_write_evil_data(temp, *temp + 1) != E_HM_OK) {
		hm_error("Verify failed, Try change the value failed.\n");
		return E_HM_INVAL;
	}
	if (tmp_char == *temp) {
		return E_HM_OK;
	}
	if (hm_hkip_write_evil_data(temp, tmp_char) != E_HM_OK) {
		hm_error("Verify failed, Try restore the value failed.\n");
		return E_HM_INVAL;
	}
	hm_error("Verify failed, The '%s' can be tampered with.\n", msg);
	return E_HM_INVAL;
}

static int verify_hashtab_node(struct hashtab_node *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->key, "hashtab_node.key");
	ret |= verify_tampering(p->key, "hashtab_node.key.value");
	ret |= verify_tampering(&p->datum, "hashtab_node.datum");
	ret |= verify_tampering(&p->next, "hashtab_node.next");
	return ret;
}

static int verify_hashtab_val(struct hashtab_val *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->htable, "hashtab_val.htable");
	ret |= verify_tampering(p->htable, "hashtab_val.htable.value");
	ret |= verify_hashtab_node(*(p->htable));
	ret |= verify_tampering(&p->size, "hashtab_val.size");
	ret |= verify_tampering(&p->nel, "hashtab_val.nel");
	return ret;
}

static int verify_symtab_t(symtab_t *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->table, "symtab_t.table");
	ret |= verify_hashtab_val(p->table);
	ret |= verify_tampering(&p->nprim, "symtab_t.nprim");
	return ret;
}

static int verify_symtab_datum(struct symtab_datum *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->value, "symtab_datum.value");
	return ret;
}

static int verify_common_datum(struct common_datum *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->s, "common_datum.s");
	ret |= verify_symtab_datum(&p->s);
	ret |= verify_tampering(&p->permissions, "common_datum.permissions");
	ret |= verify_symtab_t(&p->permissions);
	return ret;
}

static int verify_ebitmap_node(struct ebitmap_node *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->startbit, "ebitmap_node.startbit");
	ret |= verify_tampering(&p->map, "ebitmap_node.map");
	ret |= verify_tampering(&p->next, "ebitmap_node.next");
	return ret;
}

static int verify_ebitmap(struct ebitmap *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	if (!p->protectable)
		return ret;
	ret |= verify_tampering(&p->node, "ebitmap.node");
	ret |= verify_ebitmap_node(p->node);
	ret |= verify_tampering(&p->highbit, "ebitmap.highbit");
	return ret;
}

static int verify_type_set(struct type_set *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->types, "type_set.types");
	ret |= verify_ebitmap(&p->types);
	ret |= verify_tampering(&p->negset, "type_set.negset");
	ret |= verify_ebitmap(&p->negset);
	ret |= verify_tampering(&p->flags, "type_set.flags");
	return ret;
}

static int verify_constraint_expr(struct constraint_expr *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->expr_type, "constraint_expr.expr_type");
	ret |= verify_tampering(&p->attr, "constraint_expr.attr");
	ret |= verify_tampering(&p->op, "constraint_expr.op");
	ret |= verify_tampering(&p->names, "constraint_expr.names");
	ret |= verify_ebitmap(&p->names);
	ret |= verify_tampering(&p->type_names, "constraint_expr.type_names");
	ret |= verify_type_set(p->type_names);
	ret |= verify_tampering(&p->next, "constraint_expr.next");
	return ret;
}

static int verify_constraint_node(struct constraint_node *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->permissions, "constraint_node.permissions");
	ret |= verify_tampering(&p->expr, "constraint_node.expr");
	ret |= verify_constraint_expr(p->expr);
	ret |= verify_tampering(&p->next, "constraint_node.next");
	return ret;
}

static int verify_class_datum(struct class_datum *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->s, "class_datum.s");
	ret |= verify_symtab_datum(&p->s);
	ret |= verify_tampering(&p->comkey, "class_datum.comkey");
	ret |= verify_tampering(p->comkey, "class_datum.comkey.value");
	ret |= verify_tampering(&p->comdatum, "class_datum.comdatum");
	ret |= verify_common_datum(p->comdatum);
	ret |= verify_tampering(&p->permissions, "class_datum.permissions");
	ret |= verify_symtab_t(&p->permissions);
	ret |= verify_tampering(&p->constraints, "class_datum.constraints");
	ret |= verify_constraint_node(p->constraints);
	ret |= verify_tampering(&p->validatetrans, "class_datum.validatetrans");
	ret |= verify_constraint_node(p->validatetrans);
	ret |= verify_tampering(&p->default_user, "class_datum.default_user");
	ret |= verify_tampering(&p->default_role, "class_datum.default_role");
	ret |= verify_tampering(&p->default_type, "class_datum.default_type");
	ret |= verify_tampering(&p->default_range, "class_datum.default_range");
	return ret;
}

static int verify_role_datum(struct role_datum *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->s, "role_datum.s");
	ret |= verify_symtab_datum(&p->s);
	ret |= verify_tampering(&p->dominates, "role_datum.dominates");
	ret |= verify_ebitmap(&p->dominates);
	ret |= verify_tampering(&p->types, "role_datum.types");
	ret |= verify_type_set(&p->types);
	ret |= verify_tampering(&p->cache, "role_datum.cache");
	ret |= verify_ebitmap(&p->cache);
	ret |= verify_tampering(&p->bounds, "role_datum.bounds");
	ret |= verify_tampering(&p->flavor, "role_datum.flavor");
	ret |= verify_tampering(&p->roles, "role_datum.roles");
	ret |= verify_ebitmap(&p->roles);
	return ret;
}

static int verify_role_set(struct role_set *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->roles, "role_set.roles");
	ret |= verify_ebitmap(&p->roles);
	ret |= verify_tampering(&p->flags, "role_set.flags");
	return ret;
}

static int verify_mls_semantic_level(struct mls_semantic_level *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->sens, "mls_semantic_level.sens");
	return ret;
}

static int verify_mls_level(struct mls_level *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->sens, "mls_level.sens");
	ret |= verify_tampering(&p->cat, "mls_level.cat");
	ret |= verify_ebitmap(&p->cat);
	return ret;
}

static int verify_user_datum(struct user_datum *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->s, "user_datum.s");
	ret |= verify_symtab_datum(&p->s);
	ret |= verify_tampering(&p->roles, "user_datum.roles");
	ret |= verify_role_set(&p->roles);
	ret |= verify_tampering(&p->range, "user_datum.range");
	ret |= verify_tampering(&p->dfltlevel, "user_datum.dfltlevel");
	ret |= verify_mls_semantic_level(&p->dfltlevel);
	ret |= verify_tampering(&p->cache, "user_datum.cache");
	ret |= verify_ebitmap(&p->cache);
	ret |= verify_tampering(&p->exp_range, "user_datum.exp_range");
	ret |= verify_tampering(&p->exp_dfltlevel, "user_datum.exp_dfltlevel");
	ret |= verify_mls_level(&p->exp_dfltlevel);
	ret |= verify_tampering(&p->bounds, "user_datum.bounds");
	return ret;
}

static int verify_type_datum(struct type_datum *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->s, "type_datum.s");
	ret |= verify_symtab_datum(&p->s);
	ret |= verify_tampering(&p->primary, "type_datum.primary");
	ret |= verify_tampering(&p->flavor, "type_datum.flavor");
	ret |= verify_tampering(&p->types, "type_datum.types");
	ret |= verify_ebitmap(&p->types);
	ret |= verify_tampering(&p->flags, "type_datum.flags");
	ret |= verify_tampering(&p->bounds, "type_datum.bounds");
	return ret;
}

static int verify_avtab_key(struct avtab_key *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->source_type, "avtab_key.source_type");
	ret |= verify_tampering(&p->target_type, "avtab_key.target_type");
	ret |= verify_tampering(&p->target_class, "avtab_key.target_class");
	ret |= verify_tampering(&p->specified, "avtab_key.specified");
	return ret;
}

static int verify_avtab_extended_perms(struct avtab_extended_perms *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->specified, "avtab_extended_perms.specified");
	ret |= verify_tampering(&p->driver, "avtab_extended_perms.driver");
	ret |= verify_tampering(&p->perms, "avtab_extended_perms.perms");
	return ret;
}

static int verify_avtab_datum(struct avtab_datum *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->data, "avtab_datum.data");
	ret |= verify_tampering(&p->xperms, "avtab_datum.xperms");
	ret |= verify_avtab_extended_perms(p->xperms);
	return ret;
}

static int verify_avtab_node(struct avtab_node *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->key, "avtab_node.key");
	ret |= verify_avtab_key(&p->key);
	ret |= verify_tampering(&p->datum, "avtab_node.datum");
	ret |= verify_avtab_datum(&p->datum);
	ret |= verify_tampering(&p->next, "avtab_node.next");
	ret |= verify_tampering(&p->parse_context, "avtab_node.parse_context");
	ret |= verify_tampering(&p->merged, "avtab_node.merged");
	return ret;
}

static int verify_avtab(struct avtab *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->htable, "type_datum.htable");
	ret |= verify_tampering(p->htable, "type_datum.htable.value");
	ret |= verify_avtab_node(*(p->htable));
	ret |= verify_tampering(&p->nel, "avtab.nel");
	ret |= verify_tampering(&p->nslot, "avtab.nslot");
	ret |= verify_tampering(&p->mask, "avtab.mask");
	return ret;
}

static int verify_role_trans(struct role_trans *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->role, "role_trans.role");
	ret |= verify_tampering(&p->type, "role_trans.type");
	ret |= verify_tampering(&p->tclass, "role_trans.tclass");
	ret |= verify_tampering(&p->new_role, "role_trans.new_role");
	ret |= verify_tampering(&p->next, "role_trans.next");
	return ret;
}

static int verify_role_allow(struct role_allow *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->role, "role_allow.role");
	ret |= verify_tampering(&p->new_role, "role_allow.new_role");
	ret |= verify_tampering(&p->next, "role_allow.next");
	return ret;
}

static int verify_genfs(struct genfs *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->fstype, "genfs.fstype");
	ret |= verify_tampering(p->fstype, "genfs.fstype.value");
	ret |= verify_tampering(&p->next, "genfs.next");
	return ret;
}

static int verify_policydb(struct policydb *p)
{
	int ret = E_HM_OK;
	ret |= verify_tampering(&p->policy_type, "policydb.policy_type");
	ret |= verify_tampering(&p->target_platform, "policydb.target_platform");
	ret |= verify_tampering(&p->unsupported_format, "policydb.unsupported_format");
	ret |= verify_tampering(&p->p_commons, "policydb.symtab");
	ret |= verify_symtab_t(&p->p_commons);
	ret |= verify_tampering(&p->p_common_val_to_name, "policydb.sym_val_to_name");
	ret |= verify_tampering(p->p_common_val_to_name, "policydb.sym_val_to_name.value");
	ret |= verify_tampering(&p->class_val_to_struct, "policydb.class_val_to_struct");
	ret |= verify_tampering(p->class_val_to_struct, "policydb.class_val_to_struct.value");
	ret |= verify_class_datum(*(p->class_val_to_struct));
	ret |= verify_tampering(&p->role_val_to_struct, "policydb.role_val_to_struct");
	ret |= verify_tampering(p->role_val_to_struct, "policydb.role_val_to_struct.value");
	ret |= verify_role_datum(*(p->role_val_to_struct));
	ret |= verify_tampering(&p->user_val_to_struct, "policydb.user_val_to_struct");
	ret |= verify_tampering(p->user_val_to_struct, "policydb.user_val_to_struct.value");
	ret |= verify_user_datum(*(p->user_val_to_struct));
	ret |= verify_tampering(&p->type_val_to_struct, "policydb.type_val_to_struct");
	ret |= verify_tampering(p->type_val_to_struct, "policydb.type_val_to_struct.value");
	ret |= verify_type_datum(*(p->type_val_to_struct));
	ret |= verify_tampering(&p->te_avtab, "policydb.te_avtab");
	ret |= verify_avtab(&p->te_avtab);
	ret |= verify_tampering(&p->te_cond_avtab, "policydb.te_cond_avtab");
	ret |= verify_avtab(&p->te_cond_avtab);
	ret |= verify_tampering(&p->role_tr, "policydb.role_tr");
	ret |= verify_role_trans(p->role_tr);
	ret |= verify_tampering(&p->role_allow, "policydb.role_allow");
	ret |= verify_role_allow(p->role_allow);
	ret |= verify_tampering(&p->genfs, "policydb.genfs");
	ret |= verify_genfs(p->genfs);
	ret |= verify_tampering(&p->range_tr, "policydb.range_tr");
	ret |= verify_hashtab_val(p->range_tr);
	ret |= verify_tampering(&p->filename_trans, "policydb.filename_trans");
	ret |= verify_hashtab_val(p->filename_trans);
	ret |= verify_tampering(&p->filename_trans_count, "policydb.filename_trans_count");
	ret |= verify_tampering(&p->type_attr_map, "policydb.type_attr_map");
	ret |= verify_ebitmap(p->type_attr_map);
	ret |= verify_tampering(&p->attr_type_map, "policydb.attr_type_map");
	ret |= verify_ebitmap(p->attr_type_map);
	ret |= verify_tampering(&p->policycaps, "policydb.policycaps");
	ret |= verify_ebitmap(&p->policycaps);
	ret |= verify_tampering(&p->permissive_map, "policydb.permissive_map");
	ret |= verify_ebitmap(&p->permissive_map);
	ret |= verify_tampering(&p->policyvers, "policydb.policyvers");
	ret |= verify_tampering(&p->handle_unknown, "policydb.handle_unknown");
	ret |= verify_tampering(&p->process_class, "policydb.process_class");
	ret |= verify_tampering(&p->dir_class, "policydb.dir_class");
	ret |= verify_tampering(&p->process_trans, "policydb.process_trans");
	ret |= verify_tampering(&p->process_trans_dyntrans, "policydb.process_trans_dyntrans");
	return ret;
}

static int verify_adapter_policydb(struct adapter_policydb *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->p, "adapter_policydb.policydb");
	ret |= verify_policydb(&p->p);
	return ret;
}

static int verify_hmsel_policydb(struct hmsel_policydb *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->adapter_policydb, "hmsel_policydb.value");
	ret |= verify_adapter_policydb(&p->adapter_policydb);
	ret |= verify_tampering(&p->proc_class, "hmsel_policydb.proc_class");
	return ret;
}

static int verify_hmsel_mapping(struct hmsel_mapping *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_tampering(&p->value, "hmsel_mapping.value");
	ret |= verify_tampering(&p->num_perms, "hmsel_mapping.num_perms");
	ret |= verify_tampering(&p->perms, "hmsel_mapping.perms");
	return ret;
}

static int verify_hmsel_map(struct hmsel_map *p)
{
	int ret = E_HM_OK;
	if (!p)
		return ret;
	ret |= verify_hmsel_mapping(p->mapping);
	ret |= verify_tampering(&p->size, "hmsel_map.size");
	return ret;
}
#endif

int hmsel_policydb_test(void)
{
	int ret = E_HM_OK;
#ifdef CONFIG_HKIP_SEHARMONY_PROT
	struct hmsel_policy *policy;
	if (!prmem_active()) {
		hm_error("The HKIP function is disabled.\n");
		return E_HM_INVAL;
	}

	policy = hmsel_states_get()->policy;

	ret |= verify_tampering(&policy->policydb, "hmsel_policy.policydb");
	ret |= verify_hmsel_policydb(&policy->policydb);
	ret |= verify_tampering(&policy->map, "hmsel_policy.map");
	ret |= verify_hmsel_map(&policy->map);
	ret |= verify_tampering(&policy->latest_granting, "hmsel_policy.latest_granting");
#endif
	return ret;
}
