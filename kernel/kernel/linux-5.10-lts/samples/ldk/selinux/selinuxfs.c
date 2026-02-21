// SPDX-License-Identifier: GPL-2.0
#include <trace/hooks/liblinux.h>
#include <liblinux/prmem.h>
#include <security.h>

#ifdef CONFIG_HM_SEPOL_POLICYDB
#include <conditional.h>
#include <policydb.h>
#endif

#include "sec.h"

#define nop_free(nop_parm)

static void hook_sel_load_policy(void *args, const char *buffer, unsigned long size, ssize_t *ret)
{
	int rc = sec_load_policy(buffer, size);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_load_policy, hook_sel_load_policy);

#ifdef CONFIG_HM_SEPOL_POLICYDB
static int policydb_init(struct policydb *p)
{
	memset(p, 0, sizeof(*p));

	avtab_init(&p->te_avtab);
	cond_policydb_init(p);

	ebitmap_init(&p->policycaps, HKIP_SELINUX_EBITMAP_RO);
	ebitmap_init(&p->permissive_map, HKIP_SELINUX_EBITMAP_RO);

	p->filename_trans = (struct hashtab *)pmalloc(&selinux_pool, sizeof(*p->filename_trans), PRMEM_NO_FLAGS);
	if (!p->filename_trans)
		goto err;
	p->range_tr = (struct hashtab *)pmalloc(&selinux_pool, sizeof(*p->range_tr), PRMEM_NO_FLAGS);
	if (!p->range_tr)
		goto err;

	return 0;
err:
	if (p->filename_trans)
		nop_free(p->filename_trans);
	return -ENOMEM;
}

static void hook_policydb_init(void *args, struct policydb *p, int *ret)
{
	int rc = policydb_init(p);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_policydb_init, hook_policydb_init);

static int role_trans_read(struct policydb *p, struct policy_file *fp)
{
	role_trans_t **t = &p->role_tr;
	unsigned int i;
	uint32_t buf[3], nel;
	role_trans_t *tr, *ltr;
	int rc;

	rc = next_entry(buf, fp, sizeof(u32));
	if (rc)
		return rc;
	nel = le32_to_cpu(buf[0]);
	ltr = NULL;
	for (i = 0; i < nel; i++) {
		tr = pzalloc(&selinux_pool, sizeof(*tr), PRMEM_NO_FLAGS);
		if (!tr) {
			return -ENOMEM;
		}
		if (ltr) {
			ltr->next = tr;
		} else {
			*t = tr;
		}
		rc = next_entry(buf, fp, sizeof(u32) * 3);
		if (rc)
			return rc;
		tr->role = le32_to_cpu(buf[0]);
		tr->type = le32_to_cpu(buf[1]);
		tr->new_role = le32_to_cpu(buf[2]);
		if (p->policyvers >= POLICYDB_VERSION_ROLETRANS) {
			rc = next_entry(buf, fp, sizeof(u32));
			if (rc)
				return rc;
			tr->tclass = le32_to_cpu(buf[0]);
		} else
			tr->tclass = p->process_class;
		if (!policydb_role_isvalid(p, tr->role) ||
		    !policydb_type_isvalid(p, tr->type) ||
		    !policydb_class_isvalid(p, tr->tclass) ||
		    !policydb_role_isvalid(p, tr->new_role))
			return -EINVAL;
		ltr = tr;
	}
	return 0;
}

static void hook_role_trans_read(void *args, struct policydb *p, struct policy_file *fp, int *ret)
{
	int rc = role_trans_read(p, fp);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_role_trans_read, hook_role_trans_read);

static int role_trans_write_one(struct policydb *p, void *fp, struct role_trans *tr)
{
	__le32 buf[3];
	int rc;

	buf[0] = cpu_to_le32(tr->role);
	buf[1] = cpu_to_le32(tr->type);
	buf[2] = cpu_to_le32(tr->new_role);
	rc = put_entry(buf, sizeof(u32), 3, fp);
	if (rc)
		return rc;
	if (p->policyvers >= POLICYDB_VERSION_ROLETRANS) {
		buf[0] = cpu_to_le32(tr->tclass);
		rc = put_entry(buf, sizeof(u32), 1, fp);
		if (rc)
			return rc;
	}
	return 0;
}

static int role_trans_write(struct policydb *p, void *fp)

{
	role_trans_t *r = p->role_tr;
	role_trans_t *tr;
	__le32 buf[1];
	int rc;
	size_t nel;

	nel = 0;
	for (tr = r; tr; tr = tr->next)
		nel++;

	buf[0] = cpu_to_le32(nel);
	rc = put_entry(buf, sizeof(u32), 1, fp);
	if (rc)
		return rc;

	for (tr = r; tr; tr = tr->next) {
		rc = role_trans_write_one(p, fp, tr);
		if (rc)
			return rc;
        }
	return 0;
}

static void hook_role_trans_write(void *args, struct policydb *p, void *fp, int *ret)
{
	int rc = role_trans_write(p, fp);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_role_trans_write, hook_role_trans_write);

static void hook_role_trans_destroy(void *args, struct policydb *p)
{
	role_trans_t *tr, *ltr = NULL;

	for (tr = p->role_tr; tr; tr = tr->next) {
		if (ltr)
			nop_free(ltr);
		ltr = tr;
	}
	if (ltr)
		nop_free(ltr);
}
INIT_VENDOR_HOOK(ldk_vh_sel_role_trans_destroy, hook_role_trans_destroy);

static void hook_role_trans_search(void *args, struct policydb *p, struct context *newcon,
				uint32_t srole, uint32_t ttype, uint32_t tclass)
{
	for (struct role_trans *roletr = p->role_tr; roletr;
				roletr = roletr->next) {
		if (roletr->role == srole &&
		    roletr->type == ttype &&
		    roletr->tclass == tclass) {
			/* Use the role transition rule. */
			newcon->role = roletr->new_role;
			break;
		}
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_role_trans_search, hook_role_trans_search);

static int ldk_policydb_write(struct policydb *p, struct policy_file *fp, void **data, size_t *len)
{
	int rc;
	/* Compute the length for the new policy image. */
	fp->type = PF_LEN;
	fp->len = 0;
	rc = policydb_write(p, fp);
	if (rc)
		return rc;

	*len = fp->len;
	*data = vmalloc_user(fp->len);
	if (!*data)
		return -ENOMEM;

	fp->data = *data;
	fp->type = PF_USE_MEMORY;

	rc = policydb_write(p, fp);
	return rc;
}

static void hook_policydb_write(void *args, struct policydb *p, struct policy_file *fp,
		void **data, size_t *len, int *ret)
{
	int rc = ldk_policydb_write(p, fp, data, len);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_policydb_write, hook_policydb_write);
#endif

#ifdef CONFIG_SECURITY_SELINUX_DEVELOP
static void hook_sel_update_enforce(void *args, int value, ssize_t *ret)
{
	int rc = sec_update_enforce(value);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_update_enforce, hook_sel_update_enforce);
#endif /* CONFIG_SECURITY_SELINUX_DEVELOP */

static void hook_sel_update_cache_threshold(void *args, int value, ssize_t *ret)
{
	int rc = sec_update_cache_threshold(value);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_update_cache_threshold, hook_sel_update_cache_threshold);

struct common_audit_data;

int __override avc_has_perm(struct selinux_state *state,
			    u32 ssid, u32 tsid,
			    u16 tclass, u32 requested,
			    struct common_audit_data *auditdata)
{
	return sec_avc_perm(tsid, tclass, requested);
}

#ifdef CONFIG_HM_HKIP_SEHARMONY_PROT
void __override selinux_protection_init(struct selinux_state *state)
{
	selinux_pool.hm_prmem_pool = liblinux_pal_sel_seharmony_pool_init();
	selinux_prot_pool.hm_prmem_pool = liblinux_pal_sel_seharmony_prot_pool_init();

	state->prot = (struct selinux_prot *)pzalloc(&selinux_prot_pool, sizeof(struct selinux_prot *), PRMEM_NO_FLAGS);
	if (!state->prot) {
		pr_err("SELinux: hkip protection init err\n");
		BUG();
	}
}
#endif

#ifdef CONFIG_HM_SEPOL_POLICYDB
static inline bool check_unknown_value_pass(struct policydb *p)
{
	unsigned int handle_unknown = *(unsigned int *)(&p->handle_unknown);
	unsigned int pad_unknown = p->pad_unknown;
	unsigned int reject_unknown = p->reject_unknown;
	unsigned int allow_unknown = p->allow_unknown;
	unsigned int unused_unknown = p->unused_unknown;
	return (allow_unknown == !!(handle_unknown & ALLOW_UNKNOWN)) &&
	       (reject_unknown == !!(handle_unknown & REJECT_UNKNOWN)) &&
	       (pad_unknown == 0u) && (unused_unknown == 0u);
}

static bool check_sel_struct_size_pass(void)
{
	struct sel_struct_size sel_struct = {0};

	security_init_sel_struct_size(&sel_struct);
	if (sel_struct.policydb_size != sizeof(struct policydb)) {
		pr_err("SELinux: check policydb size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct policydb), sel_struct.policydb_size);
		goto fail;
	} else if (sel_struct.hashtab_size != sizeof(struct hashtab)) {
		pr_err("SELinux: check hashtab size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct hashtab), sel_struct.hashtab_size);
		goto fail;
	} else if (sel_struct.hashtab_node_size != sizeof(struct hashtab_node)) {
		pr_err("SELinux: check hashtab_node size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct hashtab_node), sel_struct.hashtab_node_size);
		goto fail;
	} else if (sel_struct.symtab_size != sizeof(struct symtab)) {
		pr_err("SELinux: check symtab size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct symtab), sel_struct.symtab_size);
		goto fail;
	} else if (sel_struct.class_datum_size != sizeof(struct class_datum)) {
		pr_err("SELinux: check class_datum size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct class_datum), sel_struct.class_datum_size);
		goto fail;
	} else if (sel_struct.common_datum_size != sizeof(struct common_datum)) {
		pr_err("SELinux: check common_datum size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct common_datum), sel_struct.common_datum_size);
		goto fail;
	} else if (sel_struct.constraint_node_size != sizeof(struct constraint_node)) {
		pr_err("SELinux: check constraint_node size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct constraint_node), sel_struct.constraint_node_size);
		goto fail;
	} else if (sel_struct.constraint_expr_size != sizeof(struct constraint_expr)) {
		pr_err("SELinux: check constraint_expr size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct constraint_expr), sel_struct.constraint_expr_size);
		goto fail;
	} else if (sel_struct.type_set_size != sizeof(struct type_set)) {
		pr_err("SELinux: check type_set size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct type_set), sel_struct.type_set_size);
		goto fail;
	} else if (sel_struct.ebitmap_node_size != sizeof(struct ebitmap_node)) {
		pr_err("SELinux: check ebitmap_node size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct ebitmap_node), sel_struct.ebitmap_node_size);
		goto fail;
	} else if (sel_struct.ebitmap_size != sizeof(struct ebitmap)) {
		pr_err("SELinux: check ebitmap size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct ebitmap), sel_struct.ebitmap_size);
		goto fail;
	} else if (sel_struct.role_datum_size != sizeof(struct role_datum)) {
		pr_err("SELinux: check role_datum size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct role_datum), sel_struct.role_datum_size);
		goto fail;
	} else if (sel_struct.user_datum_size != sizeof(struct user_datum)) {
		pr_err("SELinux: check user_datum size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct user_datum), sel_struct.user_datum_size);
		goto fail;
	} else if (sel_struct.type_datum_size != sizeof(struct type_datum)) {
		pr_err("SELinux: check type_datum size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct type_datum), sel_struct.type_datum_size);
		goto fail;
	} else if (sel_struct.cond_bool_datum_size != sizeof(struct cond_bool_datum)) {
		pr_err("SELinux: check cond_bool_datum size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct cond_bool_datum), sel_struct.cond_bool_datum_size);
		goto fail;
	} else if (sel_struct.avtab_key_size != sizeof(struct avtab_key)) {
		pr_err("SELinux: check avtab_key size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct avtab_key), sel_struct.avtab_key_size);
		goto fail;
	} else if (sel_struct.avtab_ext_perms_size != sizeof(struct avtab_extended_perms)) {
		pr_err("SELinux: check avtab_extended_perms size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct avtab_extended_perms), sel_struct.avtab_ext_perms_size);
		goto fail;
	} else if (sel_struct.avtab_datum_size != sizeof(struct avtab_datum)) {
		pr_err("SELinux: check avtab_datum size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct avtab_datum), sel_struct.avtab_datum_size);
		goto fail;
	} else if (sel_struct.avtab_node_size != sizeof(struct avtab_node)) {
		pr_err("SELinux: check avtab_node size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct avtab_node), sel_struct.avtab_node_size);
		goto fail;
	} else if (sel_struct.avtab_size != sizeof(struct avtab)) {
		pr_err("SELinux: check avtab size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct avtab), sel_struct.avtab_size);
		goto fail;
	} else if (sel_struct.role_trans_size != sizeof(struct role_trans)) {
		pr_err("SELinux: check role_trans size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct role_trans), sel_struct.role_trans_size);
		goto fail;
	} else if (sel_struct.role_allow_size != sizeof(struct role_allow)) {
		pr_err("SELinux: check role_allow size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct role_allow), sel_struct.role_allow_size);
		goto fail;
	} else if (sel_struct.context_size != sizeof(struct context)) {
		pr_err("SELinux: check context size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct context), sel_struct.context_size);
		goto fail;
	} else if (sel_struct.ocontext_size != sizeof(struct ocontext)) {
		pr_err("SELinux: check ocontext size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct ocontext), sel_struct.ocontext_size);
		goto fail;
	} else if (sel_struct.genfs_size != sizeof(struct genfs)) {
		pr_err("SELinux: check genfs size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct genfs), sel_struct.genfs_size);
		goto fail;
	} else if (sel_struct.mls_level_size != sizeof(struct mls_level)) {
		pr_err("SELinux: check mls_level size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct mls_level), sel_struct.mls_level_size);
		goto fail;
	} else if (sel_struct.mls_range_size != sizeof(struct mls_range)) {
		pr_err("SELinux: check mls_range size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct mls_range), sel_struct.mls_range_size);
		goto fail;
	} else if (sel_struct.cond_node_size != sizeof(struct cond_node)) {
		pr_err("SELinux: check cond_node size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct cond_node), sel_struct.cond_node_size);
		goto fail;
	} else if (sel_struct.cond_expr_size != sizeof(struct cond_expr)) {
		pr_err("SELinux: check cond_expr size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct cond_expr), sel_struct.cond_expr_size);
		goto fail;
	} else if (sel_struct.cond_av_list_size != sizeof(struct cond_av_list)) {
		pr_err("SELinux: check cond_av_list size failed, selinuxfs:%u, libsepol:%u\n",
				sizeof(struct cond_av_list), sel_struct.cond_av_list_size);
		goto fail;
	}
	pr_info("SELinux: check structs size success!\n");
	return true;
fail:
	return false;
}

int __override policydb_read(struct policydb *p, void *fp)
{
	BUG_ON(!check_sel_struct_size_pass());
	struct policydb *pol = security_get_sel_policy();
	BUG_ON(!check_unknown_value_pass(pol));
	*p = *pol;
	return 0;
}

void __override policydb_destroy(struct policydb *p)
{
	return ;
}

int __override security_set_bools(struct selinux_state *state, u32 len, int *values)
{
	return -EINVAL;
}
#endif
